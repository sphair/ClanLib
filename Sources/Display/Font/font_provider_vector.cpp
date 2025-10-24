/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
**    Harry Storbacka
**    Magnus Norddahl
**    Mark Page
*/

#include "Display/precomp.h"
#include "font_provider_vector.h"

#include "FontEngine/font_engine_freetype.h"

#include "API/Core/IOData/file.h"
#include "API/Core/IOData/virtual_directory.h"
#include "API/Core/IOData/virtual_file_system.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Core/Text/string_format.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/2D/color.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/Font/font_vector.h"
#include "glyph_outline.h"

/////////////////////////////////////////////////////////////////////////////
// CL_FontProvider_Vector Construction:

CL_FontProvider_Vector::CL_FontProvider_Vector() : font_engine(NULL)
{
}

void CL_FontProvider_Vector::load_font(const CL_FontDescription &desc)
{
	CL_String path = CL_PathHelp::get_fullpath(desc.get_typeface_name(), CL_PathHelp::path_type_file);
	CL_String filename = CL_PathHelp::get_filename(desc.get_typeface_name(), CL_PathHelp::path_type_file);
	CL_VirtualFileSystem vfs(path);
	CL_IODevice file = vfs.get_root_directory().open_file_read(filename);
	load_font(desc, file);
}

void CL_FontProvider_Vector::load_font(const CL_FontDescription &desc, const CL_VirtualDirectory &directory)
{
	CL_IODevice file = directory.open_file_read(desc.get_typeface_name());
	load_font(desc, file);
}

void CL_FontProvider_Vector::load_font(const CL_FontDescription &desc, CL_IODevice &io_dev)
{
	int average_width = desc.get_average_width();
	int height = desc.get_height();

	// Ensure width and height are positive
	if (average_width < 0) average_width =-average_width;
	if (height < 0) height =-height;

	size_height = height;

	font_engine = new CL_FontEngine_Freetype(io_dev, height, average_width);

	metrics = font_engine->get_metrics();
}

CL_FontProvider_Vector::~CL_FontProvider_Vector()
{
	if (font_engine)
	{
		delete font_engine;
	}

}

/////////////////////////////////////////////////////////////////////////////
// CL_FontProvider_Vector Attributes:


CL_FontMetrics CL_FontProvider_Vector::get_font_metrics(CL_GraphicContext &gc)
{
	return metrics;
}

/////////////////////////////////////////////////////////////////////////////
// CL_FontProvider_Vector Operations:

int CL_FontProvider_Vector::get_character_index(CL_GraphicContext &gc, const CL_String &text, const CL_Point &point)
{
	throw CL_Exception("FIXME");
}

void CL_FontProvider_Vector::destroy()
{
	delete this;
}


void CL_FontProvider_Vector::draw_text(CL_GraphicContext &gc, float x, float y, const CL_StringRef &text, const CL_Colorf &color)
{
	if (text.length() == 0)
		return;

	unsigned int text_length = text.length();
	CL_DataBuffer buf_out_glyphs(sizeof(int) * (text_length+1));
	CL_DataBuffer buf_interspacing_x(sizeof(float) * (text_length+1));
	int *out_glyphs = (int *) buf_out_glyphs.get_data();
	float *interspacing_x = (float *) buf_interspacing_x.get_data();
 
	get_glyphs(text, out_glyphs, interspacing_x, 0);
	// TODO: Note, "filled" is always true
	draw_glyphs(gc, x, y, out_glyphs, text_length, true, interspacing_x, 0, color);

}

CL_Size CL_FontProvider_Vector::get_text_size(CL_GraphicContext &gc, const CL_StringRef &text)
{
	if (text.length() == 0)
		return CL_Size(0, 0);

	unsigned int text_length = text.length();
	CL_DataBuffer buf_out_glyphs(sizeof(int) * (text_length+1));
	CL_DataBuffer buf_interspacing_x(sizeof(float) * (text_length+1));
	int *out_glyphs = (int *) buf_out_glyphs.get_data();
	float *interspacing_x = (float *) buf_interspacing_x.get_data();
 
	get_glyphs(text, out_glyphs, interspacing_x, 0);

	float offset_x=0.0f;
	float offset_y=metrics.get_height();

	float max_x = 0.0f;

	for( int i=0; i<text_length; i++ )
	{
		offset_x += interspacing_x[i];

		if (offset_x > max_x)
			max_x = offset_x;

		if( out_glyphs[i] == '\n' )
		{
			offset_x = 0;
			offset_y += metrics.get_height();
		}
	}
	return CL_Size(max_x, offset_y);
}

int CL_FontProvider_Vector::get_glyph_count(const CL_StringRef &text)
{
	return (int) text.length();
}

CL_GlyphPrimitivesArrayOutline CL_FontProvider_Vector::get_glyph_outline(int glyph)
{
	std::map<int, CL_GlyphOutline*>::iterator it = char_cache.find(glyph);
	if (it != char_cache.end())
		return it->second->get_outline();
	else
		return CL_GlyphPrimitivesArrayOutline(0);
}

CL_GlyphPrimitivesArray CL_FontProvider_Vector::get_glyph_triangles(int glyph)
{
	std::map<int, CL_GlyphOutline*>::iterator it = char_cache.find(glyph);
	if (it != char_cache.end())
		return it->second->get_triarray();
	else
		return CL_GlyphPrimitivesArray(0);
}

void CL_FontProvider_Vector::get_glyphs(
	const CL_StringRef &text,
	int *out_glyphs,
	float *out_interspacing_x,
	float *out_interspacing_y)
{
	for( unsigned int i=0; i<text.length(); i++ )
	{
		if( char_cache.find(text[i]) == char_cache.end() )
		{
			CL_GlyphOutline *outline = font_engine->load_glyph_outline(text[i]);
			outline->triangulate();
			
			char_cache[text[i]] = outline;
		}
		
		out_glyphs[i] = text[i];

		if( i < text.length() )
		{
			// PERFORMANCE TODO: Cache advance_x and kerning.
			out_interspacing_x[i] = font_engine->get_advance_x( text[i] );
			out_interspacing_x[i] += font_engine->get_kerning( text[i], text[i+1] );
		}
	}
}

void CL_FontProvider_Vector::draw_glyphs(
	CL_GraphicContext &gc,
	float x,
	float y,
	int *glyphs,
	int length,
	bool filled,
	float *interspacing_x,
	float *interspacing_y,
	const CL_Colorf &color)
{
	float offset_x=0;
	float offset_y=0;

	gc.set_program_object(cl_program_color_only);

	for( int i=0; i<length; i++ )
	{
		gc.push_translate(x+offset_x, y+offset_y);

		if( filled )
		{
			CL_GlyphPrimitivesArray &prim_array = char_cache[glyphs[i]]->get_triarray();

			if (prim_array.vertex.size() > 0)
			{
				CL_PrimitivesArray new_prim_array(gc);
				new_prim_array.set_attributes(0, &(prim_array.vertex[0]));
				new_prim_array.set_attribute(1, color);
				gc.draw_primitives(cl_triangles, prim_array.vertex.size(), new_prim_array);
			}
		}
		else
		{
			CL_GlyphPrimitivesArrayOutline &prim_array_outline = char_cache[glyphs[i]]->get_outline();

			std::vector< std::vector<CL_Vec2f> >::iterator it;

			for (it = prim_array_outline.vertex.begin(); it != prim_array_outline.vertex.end(); ++it)
			{
				CL_PrimitivesArray new_prim_array(gc);
				new_prim_array.set_attributes(0, &((*it)[0]));
				new_prim_array.set_attribute(1, color);
				gc.draw_primitives(cl_line_strip, it->size(), new_prim_array);
				
			}

			char_cache[glyphs[i]]->draw_debug_outline(gc);
		}
		
		gc.pop_modelview();
		
		offset_x += interspacing_x[i];
		if( glyphs[i] == '\n' )
		{
			offset_x = 0;
			y += metrics.get_height();
		}
	}

	gc.reset_program_object();
}

CL_Sizef CL_FontProvider_Vector::get_glyphs_size(int *glyphs, int length)
{
	return CL_Sizef();
}

/////////////////////////////////////////////////////////////////////////////
// CL_FontProvider_Vector Implementation:
