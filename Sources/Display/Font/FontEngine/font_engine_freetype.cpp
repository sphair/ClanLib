/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
#include "API/Core/Math/bezier_curve.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/2D/color.h"
#include "font_engine_freetype.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Display/2D/path.h"

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

FontEngine_Freetype::FontEngine_Freetype(const FontDescription &description, DataBuffer &font_databuffer, float new_pixel_ratio) : face(nullptr), pixel_ratio(new_pixel_ratio)
{
	font_description = description.clone();

	float average_width = description.get_average_width();
	float height = description.get_height();

	// Ensure width and height are positive
	if (average_width < 0.0f) average_width = -average_width;
	if (height < 0.0f) height = -height;

	data_buffer = font_databuffer;

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

	int pixel_width = (int)std::round(description.get_average_width() * pixel_ratio);
	int pixel_height = (int)std::round(height * pixel_ratio);

	FT_Set_Pixel_Sizes(face, pixel_width, pixel_height);

	calculate_font_metrics();
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

FontPixelBuffer FontEngine_Freetype::get_font_glyph(int glyph)
{
	if (font_description.get_subpixel())
	{
		return get_font_glyph_subpixel(glyph);
	}
	else
	{
		return get_font_glyph_standard(glyph, font_description.get_anti_alias());
	}
}

/////////////////////////////////////////////////////////////////////////////
// FontEngine_Freetype Operations:

void FontEngine_Freetype::load_glyph_path(unsigned int c, Path &out_path, GlyphMetrics &out_metrics)
{
	out_path.set_fill_mode(PathFillMode::winding);

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

	for( int cont = 0; cont < ft_outline.n_contours; cont++ )
	{
		std::vector<TaggedPoint> points = get_contour_points(cont, &ft_outline);
		points.push_back(points.front()); // just to simplify, it's removed later.

		bool initial = true;
		for( unsigned int i = 0; i < points.size()-1; i++ )
		{
			TaggedPoint &tp = points[i];

			if( tp.tag == FT_Curve_Tag_On )
			{
				if (initial)
				{
					out_path.move_to(tp.pos);
					initial = false;
				}
				else
				{
					out_path.line_to(tp.pos);
				}
			}
			else if( tp.tag == FT_Curve_Tag_Conic )
			{
				if (i)
				{
					out_path.bezier_to(points[i - 1].pos, tp.pos, points[i + 1].pos);
				}
			}
			else if( tp.tag == FT_Curve_Tag_Cubic && points[i-1].tag == FT_Curve_Tag_Cubic )
			{
				// TODO: This needs checking. We do not have a fonts that uses cubics ... and this bezier is currrently not supported by path
				//if (i >= 2)
				//{
					//out_path.bezier_to(points[i - 2].pos, points[i - 1].pos, tp.pos, points[i + 1].pos);
				//}
			}
		}

		out_path.close();
	}

	FT_GlyphSlot slot = face->glyph;

	out_metrics.bbox_offset.x = slot->metrics.horiBearingX / 64.0f;
	out_metrics.bbox_offset.y = -slot->metrics.horiBearingY / 64.0f;
	out_metrics.bbox_size.width = slot->metrics.width / 64.0f;
	out_metrics.bbox_size.height = slot->metrics.height / 64.0f;
	out_metrics.advance.width = slot->advance.x / 64.0f;
	out_metrics.advance.height = slot->advance.y / 64.0f;

	out_metrics.advance.width /= pixel_ratio;
	out_metrics.advance.height /= pixel_ratio;
	out_metrics.bbox_offset.x /= pixel_ratio;
	out_metrics.bbox_offset.y /= pixel_ratio;
	out_metrics.bbox_size.width /= pixel_ratio;
	out_metrics.bbox_size.height /= pixel_ratio;

	FT_Done_Glyph(glyph);

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
	font_buffer.metrics.bbox_offset.x = slot->metrics.horiBearingX / 64.0f;
	font_buffer.metrics.bbox_offset.y = -slot->metrics.horiBearingY / 64.0f;
	font_buffer.metrics.bbox_size.width = slot->metrics.width / 64.0f;
	font_buffer.metrics.bbox_size.height = slot->metrics.height / 64.0f;
	font_buffer.metrics.advance.width = slot->advance.x / 64.0f;
	font_buffer.metrics.advance.height = slot->advance.y / 64.0f;

	font_buffer.metrics.advance.width /= pixel_ratio;
	font_buffer.metrics.advance.height /= pixel_ratio;
	font_buffer.metrics.bbox_offset.x /= pixel_ratio;
	font_buffer.metrics.bbox_offset.y /= pixel_ratio;
	font_buffer.metrics.bbox_size.width /= pixel_ratio;
	font_buffer.metrics.bbox_size.height /= pixel_ratio;

	if (error || slot->bitmap.rows == 0 || slot->bitmap.width == 0)
		return font_buffer;

	// Set destination offset
	font_buffer.offset.x = slot->bitmap_left;
	font_buffer.offset.y = -slot->bitmap_top;
	font_buffer.offset.x /= pixel_ratio;
	font_buffer.offset.y /= pixel_ratio;

	int src_width = slot->bitmap.width;
	int src_height = slot->bitmap.rows;
	int src_pitch = slot->bitmap.pitch;

	PixelBuffer pixelbuffer(src_width, src_height, TextureFormat::rgba8);
	font_buffer.buffer = pixelbuffer;
	font_buffer.buffer_rect = pixelbuffer.get_size();
	font_buffer.empty_buffer = false;

	font_buffer.size = Sizef(pixelbuffer.get_width() / pixel_ratio, pixelbuffer.get_height() / pixel_ratio);

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
	font_buffer.metrics.bbox_offset.x = slot->metrics.horiBearingX / 64.0f;
	font_buffer.metrics.bbox_offset.y = -slot->metrics.horiBearingY / 64.0f;
	font_buffer.metrics.bbox_size.width = slot->metrics.width / 64.0f;
	font_buffer.metrics.bbox_size.height = slot->metrics.height / 64.0f;
	font_buffer.metrics.advance.width = slot->advance.x / 64.0f;
	font_buffer.metrics.advance.height = slot->advance.y / 64.0f;

	font_buffer.metrics.advance.width /= pixel_ratio;
	font_buffer.metrics.advance.height /= pixel_ratio;
	font_buffer.metrics.bbox_offset.x /= pixel_ratio;
	font_buffer.metrics.bbox_offset.y /= pixel_ratio;
	font_buffer.metrics.bbox_size.width /= pixel_ratio;
	font_buffer.metrics.bbox_size.height /= pixel_ratio;

	if (error || slot->bitmap.rows == 0 || slot->bitmap.width == 0)
		return font_buffer;

	// Set destination offset
	font_buffer.offset.x = slot->bitmap_left;
	font_buffer.offset.y = -slot->bitmap_top;
	font_buffer.offset.x /= pixel_ratio;
	font_buffer.offset.y /= pixel_ratio;

	int src_width = slot->bitmap.width;
	int src_height = slot->bitmap.rows;
	int src_pitch = slot->bitmap.pitch;

	// Convert the bitmap

	PixelBuffer pixelbuffer(src_width/3, src_height, TextureFormat::rgba8);
	font_buffer.buffer = pixelbuffer;
	font_buffer.buffer_rect = pixelbuffer.get_size();
	font_buffer.empty_buffer = false;
	font_buffer.size = Sizef(pixelbuffer.get_width() / pixel_ratio, pixelbuffer.get_height() / pixel_ratio);

	unsigned char *src_data = slot->bitmap.buffer;
	unsigned char *pixel_data = (unsigned char *) font_buffer.buffer.get_data();
	int dest_pitch = font_buffer.buffer.get_pitch();

	// For 8bit bitmaps
	for (int ycnt = 0; ycnt < src_height; ycnt++)
	{
		unsigned char *dest_data = pixel_data;
		for (int xcnt = 0; xcnt < src_width/3; xcnt++)
		{
			*(dest_data++)= src_data[xcnt*3+0];
			*(dest_data++)= src_data[xcnt*3+1];
			*(dest_data++)= src_data[xcnt*3+2];
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
	P.y = - float(vec.y)/64.0f;

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

	for (unsigned int cnt=0; cnt<points.size(); cnt++)
	{
		points[cnt].pos /= pixel_ratio;
	}

	return points;
}

void FontEngine_Freetype::calculate_font_metrics()
{
	// A glyph has to be loaded to be able to get the scaled metrics information.
	FT_UInt glyph_index = FT_Get_Char_Index(face, FT_ULong(' '));
	FT_Error error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
	if (error)
		throw Exception("freetype: error loading glyph");

	float ascent = face->size->metrics.ascender / 64.0f;
	float descent = -face->size->metrics.descender / 64.0f;
	float height = ascent + descent;

	float internal_leading = height - face->size->metrics.y_ppem;
	float external_leading = (face->size->metrics.height / 64.0f) - height;
	font_metrics = FontMetrics(
		height / pixel_ratio,
		ascent / pixel_ratio,
		descent / pixel_ratio,
		internal_leading / pixel_ratio,
		external_leading / pixel_ratio,
		font_description.get_line_height(),		// Calculated in FontMetrics as height + metrics.tmExternalLeading if not specified
		pixel_ratio
		);
}

}
