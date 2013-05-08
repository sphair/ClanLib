/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
*/

#include "Display/precomp.h"
#include "glyph_cache.h"
#include "FontEngine/font_engine.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/Text/string_format.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/Image/pixel_buffer_help.h"
#include "API/Display/2D/color.h"
#include "API/Display/2D/subtexture.h"
#include "API/Display/2D/texture_group.h"
#include "API/Display/2D/subtexture.h"
#include "API/Display/Render/texture.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/Font/font_system.h"
#include "API/Display/2D/canvas.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/utf8_reader.h"
#include "../2D/render_batch_triangle.h"
#include "../Render/graphic_context_impl.h"
#include "API/Display/2D/canvas.h"
#include "../2D/canvas_impl.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// GlyphCache Construction:

GlyphCache::GlyphCache()
{
	glyph_list.reserve(256);

	// Note, the user can specify a different texture group size using set_texture_group()
	texture_group = TextureGroup(Size(256,256));

	// Set default font metrics
	font_metrics = FontMetrics(
		0,0, 0, 0,0,0,0,0, 0,0,
		false, false, false, false);

	anti_alias = true;
	enable_subpixel = true;
}

GlyphCache::~GlyphCache()
{
	for (int cnt = 0; cnt < glyph_list.size(); cnt++)
		delete glyph_list[cnt];
}

/////////////////////////////////////////////////////////////////////////////
// GlyphCache Attributes:

FontMetrics GlyphCache::get_font_metrics()
{
	return font_metrics;
}

Size GlyphCache::get_text_size(FontEngine *font_engine, GraphicContext &gc, const std::string &text)
{
	int width = 0;

	UTF8_Reader reader(text.data(), text.length());
	while(!reader.is_end())
	{
		unsigned int glyph = reader.get_char();
		reader.next();
		Font_TextureGlyph *gptr = get_glyph(font_engine, gc, glyph);
		if (gptr == NULL) continue;
		width += gptr->increment.x;
	}
	int height;
	if (width == 0)
	{
		height = 0;
	}
	else
	{
		height = font_metrics.get_ascent() + font_metrics.get_descent();
	}

	return (Size(width, height));
}

Font_TextureGlyph *GlyphCache::get_glyph(FontEngine *font_engine, GraphicContext &gc, unsigned int glyph)
{
	std::vector< Font_TextureGlyph * >::size_type size = glyph_list.size();
	for (int cnt=0; cnt<size; cnt++)
	{
		if (glyph_list[cnt]->glyph == glyph)
			return &(*glyph_list[cnt]);
	}

	// If glyph does not exist, create one automatically

	insert_glyph(font_engine, gc, glyph);

	// Search for the glyph again
	size = glyph_list.size();
	for (int cnt=0; cnt<size; cnt++)
	{
		if (glyph_list[cnt]->glyph == glyph)
			return &(*glyph_list[cnt]);
	}

	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// GlyphCache Operations:

int GlyphCache::get_character_index(FontEngine *font_engine, GraphicContext &gc, const std::string &text, const Point &point)
{
	int dest_x = 0;
	int dest_y = 0;

	int character_counter = 0;

	FontMetrics fm = get_font_metrics();
	int font_height = fm.get_height();
	int font_ascent = fm.get_ascent();
	int font_external_leading = fm.get_external_leading();

	std::vector<std::string> lines = StringHelp::split_text(text, "\n", false);
	for (std::vector<std::string>::size_type i=0; i<lines.size(); i++)
	{
		int xpos = dest_x;
		int ypos = dest_y;

		std::string &textline = lines[i];
		std::string::size_type string_length = textline.length();

		// Scan the string

		UTF8_Reader reader(textline.data(), textline.length());
		while(!reader.is_end())
		{
			unsigned int glyph = reader.get_char();
			std::string::size_type glyph_pos = reader.get_position();
			reader.next();

			Font_TextureGlyph *gptr = get_glyph(font_engine, gc, glyph);
			if (gptr == NULL) continue;

			Rect position(xpos, ypos - font_ascent, Size(gptr->increment.x, gptr->increment.y + font_height + font_external_leading));
			if (position.contains(point))
			{
				return glyph_pos + character_counter;
			}
		
			xpos += gptr->increment.x;
			ypos += gptr->increment.y;
		}

		dest_y += font_height + font_external_leading;

		character_counter += string_length + 1;		// (Including the '\n')

	}
	return -1;	// Not found
}

void GlyphCache::insert_glyph(GraphicContext &gc, FontPixelBuffer &pb)
{
	// Search for duplicated glyph's, if found silently ignore them
	std::vector< Font_TextureGlyph * >::size_type size = glyph_list.size();
	for (int cnt=0; cnt<size; cnt++)
	{
		if (glyph_list[cnt]->glyph == pb.glyph)
			return ;
	}

	Font_TextureGlyph *font_glyph = new Font_TextureGlyph();
	
	glyph_list.push_back(font_glyph);
	font_glyph->glyph = pb.glyph;
	font_glyph->offset = pb.offset;
	font_glyph->increment = pb.increment;

	if (!pb.empty_buffer)
	{
		PixelBuffer buffer_with_border = PixelBufferHelp::add_border(pb.buffer, glyph_border_size, pb.buffer_rect);

		Subtexture sub_texture = texture_group.add(gc, Size(buffer_with_border.get_width(), buffer_with_border.get_height() ));
		font_glyph->texture = sub_texture.get_texture();
		font_glyph->geometry = Rect(sub_texture.get_geometry().left + glyph_border_size, sub_texture.get_geometry().top + glyph_border_size, pb.buffer_rect.get_size() );
		sub_texture.get_texture().set_subimage(gc, sub_texture.get_geometry().left, sub_texture.get_geometry().top, buffer_with_border, buffer_with_border.get_size());
	}
}

void GlyphCache::insert_glyph(FontEngine *font_engine, GraphicContext &gc, const std::string &text)
{
	for (std::string::size_type p = 0; p < text.length(); ++p)
	{
		insert_glyph(font_engine, gc, text[p]);
	}
}

void GlyphCache::insert_glyph(GraphicContext &gc, unsigned int glyph, Subtexture &sub_texture, const Point &offset, const Point &increment)
{
	// Search for duplicated glyph's, if found silently ignore them
	std::vector< Font_TextureGlyph * >::size_type size = glyph_list.size();
	for (int cnt=0; cnt<size; cnt++)
	{
		if (glyph_list[cnt]->glyph == glyph)
			return;
	}

	Font_TextureGlyph *font_glyph = new Font_TextureGlyph();
	
	glyph_list.push_back(font_glyph);
	font_glyph->glyph = glyph;
	font_glyph->offset = offset;
	font_glyph->increment = increment;

	if ( (sub_texture.get_geometry().get_width() > 0 ) && (sub_texture.get_geometry().get_height() > 0) )
	{
		font_glyph->texture = sub_texture.get_texture();
		font_glyph->geometry = sub_texture.get_geometry();
	}

}

void GlyphCache::insert_glyph(FontEngine *font_engine, GraphicContext &gc, int glyph)
{
	if (enable_subpixel)
	{
		FontPixelBuffer pb = font_engine->get_font_glyph_subpixel(glyph);
		if (pb.glyph)	// Ignore invalid glyphs
		{
			insert_glyph(gc, pb);
		}
	}
	else
	{
		FontPixelBuffer pb = font_engine->get_font_glyph_standard(glyph, anti_alias);
		if (pb.glyph)	// Ignore invalid glyphs
		{
			insert_glyph(gc, pb);
		}
	}
}

void GlyphCache::draw_text(FontEngine *font_engine, Canvas &canvas, float xpos, float ypos, const std::string &text, const Colorf &color) 
{
	std::string::size_type string_length = text.length();
	if (string_length==0)
	{
		return;
	}

	RenderBatchTriangle *batcher = canvas.impl->get_triangle_batcher();
	GraphicContext &gc = canvas.get_gc();
	// Scan the string
	UTF8_Reader reader(text.data(), text.length());
	while(!reader.is_end())
	{
		unsigned int glyph = reader.get_char();
		reader.next();

		Font_TextureGlyph *gptr = get_glyph(font_engine, gc, glyph);
		if (gptr == NULL) continue;

		if (!gptr->texture.is_null())
		{
			float xp = xpos + gptr->offset.x;
			float yp = ypos + gptr->offset.y;

			Rectf dest_size(xp, yp, Sizef(gptr->geometry.get_size()));
			if (enable_subpixel)
			{
				batcher->draw_glyph_subpixel(canvas, gptr->geometry, dest_size, color, gptr->texture);
			}else
			{
				batcher->draw_image(canvas, gptr->geometry, dest_size, color, gptr->texture);
			}
		}
		xpos += gptr->increment.x;
		ypos += gptr->increment.y;
	}
}

void GlyphCache::set_texture_group(TextureGroup &new_texture_group)
{
	if (new_texture_group.is_null())
	{
		throw Exception("Specified texture group is not valid");
	}

	if (!glyph_list.empty())
	{
		throw Exception("Cannot specify a new texture group after the font has been used");
	}

	texture_group = new_texture_group;
}

void GlyphCache::set_font_metrics(const FontMetrics &metrics)
{
	font_metrics = metrics;
}

/////////////////////////////////////////////////////////////////////////////
// GlyphCache Implementation:

}
