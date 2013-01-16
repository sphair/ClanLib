/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
**    Mark Page
**    Harry Storbacka
*/

#include "Display/precomp.h"
#include "API/Display/Font/font_metrics.h"
#include "../glyph_outline.h"
#include "API/Core/Math/bezier_curve.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/2D/color.h"
#include "font_engine_freetype.h"

namespace clan
{

class FontEngine_Freetype_Library
{
private:
	FontEngine_Freetype_Library();
	~FontEngine_Freetype_Library();

public:
	static FontEngine_Freetype_Library &instance();

public:
	FT_Library library;
};


FontEngine_Freetype_Library::FontEngine_Freetype_Library()
{
	FT_Error error = FT_Init_FreeType( &library );
	if ( error )
	{
		throw Exception("FontEngine_Freetype_Library: Initializing FreeType library failed.");
	} 

	FT_Library_SetLcdFilter(library, FT_LCD_FILTER_DEFAULT);
}

FontEngine_Freetype_Library::~FontEngine_Freetype_Library()
{
	FT_Error error = FT_Done_FreeType(library);
	if ( error )
	{
		throw Exception("FontEngine_Freetype_Library: Denitializing FreeType library failed.");
	} 
}

FontEngine_Freetype_Library &FontEngine_Freetype_Library::instance()
{
	static FontEngine_Freetype_Library provider;
	return provider;
}

/////////////////////////////////////////////////////////////////////////////
// FontEngine_Freetype Construction:

FontEngine_Freetype::FontEngine_Freetype(IODevice &io_dev, int average_width, int height) : face(0)
{


	data_buffer = DataBuffer(io_dev.get_size());
	io_dev.read(data_buffer.get_data(), data_buffer.get_size());

	FontEngine_Freetype_Library &library = FontEngine_Freetype_Library::instance();

	FT_Error error = FT_New_Memory_Face( library.library, (FT_Byte*)data_buffer.get_data(), data_buffer.get_size(), 0, &face);

	if ( error == FT_Err_Unknown_File_Format )
	{
		throw Exception("Freetype error: The font file could be opened and read, but it appears  that its font format is unsupported");
	}
	else if ( error )
	{
		throw Exception("Freetype error: Font file could not be opened or read, or is corrupted.");
	}

	if (height >= 0.0f)
		height = height*face->units_per_EM/face->height;
	else
		height = -height;

	// if the device is 72 DCL_PI then 1 point becomes 1 pixel
	FT_Set_Char_Size( face, (int)(average_width*64.0f), (int)(height*64.0f), 72, 72 );
}

FontEngine_Freetype::~FontEngine_Freetype()
{
	if (face)
	{
		FT_Done_Face(face);
	}
}

/////////////////////////////////////////////////////////////////////////////
// FontEngine_Freetype Attributes:

FontMetrics FontEngine_Freetype::get_metrics()
{
	// A glyph has to be loaded to be able to get the scaled metrics information.
	FT_UInt glyph_index = FT_Get_Char_Index( face, FT_ULong(' ') );
	FT_Error error = FT_Load_Glyph( face, glyph_index, FT_LOAD_DEFAULT );
	if ( error )
	throw Exception("freetype: error loading glyph");


	float ascent = face->size->metrics.ascender / 64.0f;
	float descent = -face->size->metrics.descender / 64.0f;
	float height = ascent + descent;
	float max_char_width = face->size->metrics.max_advance / 64.0f;
	float avg_char_width = (max_char_width * 2.0f) / 3.0f;

	return FontMetrics(
		height,
		ascent,
		descent,
		height - face->size->metrics.y_ppem,		// internal_leading
		(face->size->metrics.height / 64.0f) - height,	// external_leading
		avg_char_width,	// average_character_width
		max_char_width,	// max_character_width
		face->style_flags & FT_STYLE_FLAG_BOLD ? 700.0f : 400.0f,	// weight
		0,	// overhang
		96,	// digitized_aspect_x
		96,	// digitized_aspect_y
		face->style_flags & FT_STYLE_FLAG_ITALIC ? true : false,	// italic
		false,	//TODO (underline)
		false,	//TODO (struck_out)
		face->face_flags & FT_FACE_FLAG_FIXED_WIDTH ? true : false
		);

}

float FontEngine_Freetype::get_kerning(const std::string::value_type &lchar, const std::string::value_type &rchar)
{
	FT_Vector kerning;
	
	FT_UInt left_ch_index  = FT_Get_Char_Index( face, FT_ULong(lchar) );
	FT_UInt right_ch_index = FT_Get_Char_Index( face, FT_ULong(rchar) );
	
	int error = FT_Get_Kerning( face, left_ch_index, right_ch_index, FT_KERNING_UNFITTED, &kerning );

	if ( error )
	{
		throw Exception("FreeTypeFont: error retrieving kerning info");
	}
	
	return float(kerning.x) / 64.0f;
}

float FontEngine_Freetype::get_advance_x(const std::string::value_type &ch)
{
	FT_UInt glyph_index = FT_Get_Char_Index( face, FT_ULong(ch) );

	int error = FT_Load_Glyph( face, glyph_index, FT_LOAD_DEFAULT );
	if ( error )
	{
		throw Exception("freetype: error loading glyph");
	}

	return float(face->glyph->advance.x) / 64.0f;
}

Size FontEngine_Freetype::get_size(const std::string &text, int pos)
{
	FT_UInt glyph_index;

	// Get glyph index
	glyph_index = FT_Get_Char_Index(face, text[pos]);

	// Load glyph image
	FT_Error error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT );
	if (error) return Size(0,0);

	FT_Glyph glyph;
	FT_BBox bbox;
	FT_Get_Glyph(face->glyph, &glyph);
	FT_Glyph_Get_CBox( glyph, FT_GLYPH_BBOX_PIXELS, &bbox );

	int y = bbox.yMax + bbox.yMin;	// Include the whitespace, to match the windows GetTextExtentPoint32()

	int x = int(face->glyph->advance.x / 64.0f);

	return (Size(x,y));
}

/////////////////////////////////////////////////////////////////////////////
// FontEngine_Freetype Operations:

std::shared_ptr<GlyphOutline> FontEngine_Freetype::load_glyph_outline(int c, int &out_advance_x)
{
	out_advance_x = 0;

	FT_UInt glyph_index;

	glyph_index = FT_Get_Char_Index( face, FT_ULong(c) );

	FT_Error error = FT_Load_Glyph( face, glyph_index, FT_LOAD_DEFAULT );
	if ( error )
	{
		throw Exception("freetype: error loading glyph");
	}

	FT_Glyph glyph;

	error = FT_Get_Glyph( face->glyph, &glyph );

	if ( error )
	{
		throw Exception("freetype: error getting glyph");
	}

	FT_OutlineGlyph ft_outline_glyph_rec = (FT_OutlineGlyph)glyph;
	FT_Outline ft_outline = ft_outline_glyph_rec->outline;

	std::shared_ptr<GlyphOutline> outline(new GlyphOutline);

//	cl_write_console_line(string_format("Num contours: %1", ft_outline.n_contours));

	for( int cont = 0; cont < ft_outline.n_contours; cont++ )
	{
//		cl_write_console_line(string_format("Num points in contour %1: %2", cont, ft_outline.contours[0]+1));
		
		GlyphContour contour;
		
		// debug: dump contents of points array to terminal
//		for( int i = 0; i <= ft_outline.contours[cont]; ++i )
//		{
//			FT_Vector pos = ft_outline.points[i];
//			cl_write_console_line(string_format("dump points[%1]: (%2,%3) \t type: %4", i, pos.x, pos.y, ft_outline.tags[i]));
//		}
		
		std::vector<TaggedPoint> points = get_contour_points(cont, &ft_outline);
		points.push_back(points.front()); // just to simplify, it's removed later.

		for( unsigned int i = 0; i < points.size()-1; i++ )
		{
			TaggedPoint &tp = points[i];

			if( tp.tag == FT_Curve_Tag_On )
			{
				contour.add_line_to(tp.pos);
			}
			else if( tp.tag == FT_Curve_Tag_Conic )
			{
				// TODO: i - 1 is safe here because we made sure the contour will start with a Tag_On.
				BezierCurve curve;
				curve.add_control_point( points[i-1].pos);
				curve.add_control_point( tp.pos );
				curve.add_control_point( points[i+1].pos );
				contour.add_curve(curve);
			}
			else if( tp.tag == FT_Curve_Tag_Cubic && points[i-1].tag == FT_Curve_Tag_Cubic )
			{
				BezierCurve curve;
				curve.add_control_point( points[i-2].pos);
				curve.add_control_point( points[i-1].pos);
				curve.add_control_point( tp.pos );
				curve.add_control_point( points[i+1].pos );
				contour.add_curve(curve);
			}			
		}

		outline->add_contour(contour);
	}

	FT_Done_Glyph(glyph);

	out_advance_x = get_advance_x( c );

	return outline;
}

FontPixelBuffer FontEngine_Freetype::get_font_glyph_standard(int glyph, bool anti_alias)
{
	FontPixelBuffer font_buffer;
	FT_GlyphSlot slot = face->glyph;
	FT_UInt glyph_index;

	// Get glyph index
	glyph_index = FT_Get_Char_Index(face, glyph);

	FT_Error error;

	// Use FT_RENDER_MODE_NORMAL for 8bit anti-aliased bitmaps. Use FT_RENDER_MODE_MONO for 1-bit bitmaps
	if (anti_alias)
	{
		error = FT_Load_Glyph(face, glyph_index, FT_LOAD_TARGET_LIGHT );
		if (error) return font_buffer;
	
		error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL);
	}
	else
	{
		error = FT_Load_Glyph(face, glyph_index, FT_LOAD_TARGET_MONO);
		if (error) return font_buffer;
				
		error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_MONO);
	}

	font_buffer.glyph = glyph;
	// Set Increment pen position
	font_buffer.increment.x = (slot->advance.x+32) >> 6;
	font_buffer.increment.y = (slot->advance.y+32) >> 6;
 
	if (error || slot->bitmap.rows == 0 || slot->bitmap.width == 0)
		return font_buffer;

	// Set destination offset
	font_buffer.offset.x = slot->bitmap_left;
	font_buffer.offset.y = -slot->bitmap_top;

	int src_width = slot->bitmap.width;
	int src_height = slot->bitmap.rows;
	int src_pitch = slot->bitmap.pitch;

	PixelBuffer pixelbuffer(src_width, src_height, tf_rgba8);
	font_buffer.buffer = pixelbuffer;
	font_buffer.buffer_rect = pixelbuffer.get_size();
	font_buffer.empty_buffer = false;

	unsigned char *src_data = slot->bitmap.buffer;
	unsigned char *pixel_data = (unsigned char *) font_buffer.buffer.get_data();
	unsigned char *dest_data;
	int dest_pitch = font_buffer.buffer.get_pitch();

	// Convert the bitmap
	if (anti_alias)
	{
		// For 8bit bitmaps
		for (int ycnt = 0; ycnt < src_height; ycnt++)
		{
			dest_data = pixel_data;
			for (int xcnt = 0; xcnt < src_width; xcnt++)
			{
				*(dest_data++)= 255;
				*(dest_data++)= 255;
				*(dest_data++)= 255;
				*(dest_data++)= src_data[xcnt];
			}
			pixel_data += dest_pitch;
			src_data += src_pitch;
		}
	}
	else
	{
		int src_byte;
		unsigned char *src_work_ptr;

		// For 1bit bitmaps
		for (int ycnt = 0; ycnt < src_height; ycnt++)
		{
			dest_data = pixel_data;
			src_work_ptr = src_data;
			for (int xcnt = 0; xcnt < src_width; xcnt++)
			{
				*(dest_data++)= 255;
				*(dest_data++)= 255;
				*(dest_data++)= 255;
				if (!(xcnt&7))
				{
					src_byte = *(src_work_ptr++);	// Extract the pixel
				}
				if (src_byte & 0x80)	// Bit set
				{
					*(dest_data++) = 255;
				}
				else
				{
					*(dest_data++) = 0;
				}
				src_byte = src_byte << 1;
			}
			pixel_data += dest_pitch;
			src_data += src_pitch;
		}
	}

	return font_buffer;
}

FontPixelBuffer FontEngine_Freetype::get_font_glyph_subpixel(int glyph)
{
	FontPixelBuffer font_buffer;
	FT_GlyphSlot slot = face->glyph;
	FT_UInt glyph_index;

	// Get glyph index
	glyph_index = FT_Get_Char_Index(face, glyph);

	FT_Error error;

	error = FT_Load_Glyph(face, glyph_index, FT_LOAD_TARGET_LCD );
	if (error) return font_buffer;

	error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_LCD);

	font_buffer.glyph = glyph;
	// Set Increment pen position
	font_buffer.increment.x = (slot->advance.x+32) >> 6;
	font_buffer.increment.y = (slot->advance.y+32) >> 6;
 
	if (error || slot->bitmap.rows == 0 || slot->bitmap.width == 0)
		return font_buffer;

	// Set destination offset
	font_buffer.offset.x = slot->bitmap_left;
	font_buffer.offset.y = -slot->bitmap_top;

	int src_width = slot->bitmap.width;
	int src_height = slot->bitmap.rows;
	int src_pitch = slot->bitmap.pitch;

	// Convert the bitmap

	PixelBuffer pixelbuffer(src_width/3, src_height, tf_rgba8);
	font_buffer.buffer = pixelbuffer;
	font_buffer.buffer_rect = pixelbuffer.get_size();
	font_buffer.empty_buffer = false;

	unsigned char *src_data = slot->bitmap.buffer;
	unsigned char *pixel_data = (unsigned char *) font_buffer.buffer.get_data();
	int dest_pitch = font_buffer.buffer.get_pitch();

	// For 8bit bitmaps
	for (int ycnt = 0; ycnt < src_height; ycnt++)
	{
		unsigned char *dest_data = pixel_data;
		for (int xcnt = 0; xcnt < src_width/3; xcnt++)
		{
			*(dest_data++)= src_data[xcnt*3+2];
			*(dest_data++)= src_data[xcnt*3+1];
			*(dest_data++)= src_data[xcnt*3+0];
			*(dest_data++)= 0;
		}
		pixel_data += dest_pitch;
		src_data += src_pitch;
	}

	return font_buffer;
}

/////////////////////////////////////////////////////////////////////////////
// FontEngine_Freetype Implementation:

Pointf FontEngine_Freetype::FT_Vector_to_Pointf(const FT_Vector &vec)
{
	Pointf P;

	P.x = float(vec.x)/64.0f;
	P.y = float(vec.y)/64.0f;

	return P;
}

TagStruct FontEngine_Freetype::get_tag_struct(int cont, int index, FT_Outline *ft_outline)
{
	TagStruct tags;

	int prev_index = get_index_of_prev_contour_point(cont, index, ft_outline);
	int next_index = get_index_of_next_contour_point(cont, index, ft_outline);

	tags.previous = FT_CURVE_TAG(ft_outline->tags[prev_index]);
	tags.next = FT_CURVE_TAG(ft_outline->tags[next_index]);	
	tags.current = FT_CURVE_TAG(ft_outline->tags[index]);

	return tags;
}

int FontEngine_Freetype::get_index_of_next_contour_point(int cont, int index, FT_Outline *ft_outline)
{
	if( cont == 0 )
	{
		if( index == ft_outline->contours[0] )
			return 0;
		else
			return index + 1;
	}
	else
	{
		if( index == ft_outline->contours[cont] )
			return ft_outline->contours[cont-1] + 1;
		else
			return index + 1;
	}
}

int FontEngine_Freetype::get_index_of_prev_contour_point(int cont, int index, FT_Outline *ft_outline)
{
	if( cont == 0 )
	{
		if( index == 0 )
			return ft_outline->contours[0];
		else
			return index - 1;
	}
	else
	{
		if( index == ft_outline->contours[cont-1] + 1 )
			return ft_outline->contours[cont];
		else
			return index - 1;
	}
}

std::vector<TaggedPoint> FontEngine_Freetype::get_contour_points(int cont, FT_Outline *ft_outline)
{
	int start = 0;
	if (cont != 0)
	{
		start = ft_outline->contours[cont-1]+1;
	}

	std::vector<TaggedPoint> points;

	for( int i = start; i <= ft_outline->contours[cont]; i++ )
	{
		TagStruct tags = get_tag_struct(cont, i, ft_outline);
		
		TaggedPoint tp;
		tp.pos = FT_Vector_to_Pointf(ft_outline->points[i]);
		tp.tag = tags.current;

		points.push_back(tp);

		if( tags.current == 0 && tags.next == 0 )
		{
			TaggedPoint tp_middle;
			int next_index = get_index_of_next_contour_point(cont, i, ft_outline);
			Pointf next_point = FT_Vector_to_Pointf(ft_outline->points[next_index]);
			tp_middle.pos.x = (tp.pos.x + next_point.x) / 2.0f;
			tp_middle.pos.y = (tp.pos.y + next_point.y) / 2.0f;
			tp_middle.tag = FT_Curve_Tag_On;
			points.push_back(tp_middle);
		}
	}

	return points;
}

}
