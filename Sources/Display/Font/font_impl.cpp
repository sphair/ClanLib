/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
#include "font_impl.h"
#include "font_face_impl.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/Font/font_description.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/utf8_reader.h"
#include "API/Core/XML/dom_element.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/2D/path.h"
#include "API/Display/Resources/display_cache.h"
#include "API/Core/IOData/path_help.h"
#include "../2D/canvas_impl.h"
#include "FontEngine/font_engine.h"
#include "../2D/sprite_impl.h"

namespace clan
{

Font_Impl::Font_Impl()
{
}

void Font_Impl::set_font_face(FontFace &new_font_face)
{
	font_face = new_font_face;
	selected_description = font_face.impl->get_last_font().engine->get_desc().clone();
	select_font_face();
}

void Font_Impl::select_font_face()
{
	if (!font_engine)
	{
		Font_Cache font_cache = font_face.impl->get_font(selected_description);
		glyph_cache = font_cache.glyph_cache.get();
		font_engine = font_cache.engine.get();
	}
}

Font_Impl::~Font_Impl()
{
}

int Font_Impl::get_character_index(Canvas &canvas, const std::string &text, const Point &point)
{
	select_font_face();

	int dest_x = 0;
	int dest_y = 0;

	int character_counter = 0;

	const FontMetrics &font_metrics = font_engine->get_metrics();

	int font_height = font_metrics.get_height();
	int font_ascent = font_metrics.get_ascent();
	int font_external_leading = font_metrics.get_external_leading();

	//TODO: Fix me, so we do not need to line split

	std::vector<std::string> lines = StringHelp::split_text(text, "\n", false);
	for (std::vector<std::string>::size_type i = 0; i<lines.size(); i++)
	{
		int xpos = dest_x;
		int ypos = dest_y;

		std::string &textline = lines[i];
		std::string::size_type string_length = textline.length();

		// Scan the string

		UTF8_Reader reader(textline.data(), textline.length());
		while (!reader.is_end())
		{
			unsigned int glyph = reader.get_char();
			std::string::size_type glyph_pos = reader.get_position();
			reader.next();

			Font_TextureGlyph *gptr = glyph_cache->get_glyph(canvas, font_engine, glyph);
			if (gptr == nullptr) continue;

			Rect position(xpos, ypos - font_ascent, Size(gptr->metrics.advance.width, gptr->metrics.advance.height + font_height + font_external_leading));
			if (position.contains(point))
			{
				return glyph_pos + character_counter;
			}

			xpos += gptr->metrics.advance.width;
			ypos += gptr->metrics.advance.height;
		}

		dest_y += font_height + font_external_leading;

		character_counter += string_length + 1;		// (Including the '\n')

	}
	return -1;	// Not found
}

const FontMetrics &Font_Impl::get_font_metrics()
{
	select_font_face();
	return font_engine->get_metrics();
}

void Font_Impl::get_glyph_path(unsigned int glyph_index, Path &out_path, GlyphMetrics &out_metrics)
{
	select_font_face();
	return font_engine->load_glyph_path(glyph_index, out_path, out_metrics);
}

void Font_Impl::draw_text(Canvas &canvas, const Pointf &position, const std::string &text, const Colorf &color)
{
	select_font_face();
	int line_spacing = static_cast<int>(font_engine->get_metrics().get_line_height() + 0.5f);

	bool enable_subpixel = font_engine->get_desc().get_subpixel();

	RenderBatchTriangle *batcher = canvas.impl->batcher.get_triangle_batcher();
	GraphicContext &gc = canvas.get_gc();

	Pointf pos = canvas.grid_fit(position);
	float offset_x = 0;
	float offset_y = 0;
	UTF8_Reader reader(text.data(), text.length());
	while (!reader.is_end())
	{
		unsigned int glyph = reader.get_char();
		reader.next();

		if (glyph == '\n')
		{
			offset_x = 0;
			offset_y += line_spacing;
			continue;
		}

		Font_TextureGlyph *gptr = glyph_cache->get_glyph(canvas, font_engine, glyph);
		if (gptr)
		{
			if (!gptr->texture.is_null())
			{
				float xp = offset_x + pos.x + gptr->offset.x;
				float yp = offset_y + pos.y + gptr->offset.y;

				Rectf dest_size(xp, yp, Sizef(gptr->geometry.get_size()));
				if (enable_subpixel)
				{
					batcher->draw_glyph_subpixel(canvas, gptr->geometry, dest_size, color, gptr->texture);
				}
				else
				{
					batcher->draw_image(canvas, gptr->geometry, dest_size, color, gptr->texture);
				}
			}
			offset_x += gptr->metrics.advance.width;
			offset_y += gptr->metrics.advance.height;
		}
	}
}

GlyphMetrics Font_Impl::get_metrics(Canvas &canvas, unsigned int glyph)
{
	select_font_face();
	return glyph_cache->get_metrics(font_engine, canvas, glyph);
}

GlyphMetrics Font_Impl::measure_text(Canvas &canvas, const std::string &string)
{
	select_font_face();
	GlyphMetrics total_metrics;

	int line_spacing = static_cast<int>(font_engine->get_metrics().get_line_height() + 0.5f);
	bool first_char = true;
	Rectf text_bbox;

	UTF8_Reader reader(string.data(), string.length());
	while (!reader.is_end())
	{
		unsigned int glyph = reader.get_char();
		reader.next();

		if (glyph == '\n')
		{
			total_metrics.advance.width = 0;
			total_metrics.advance.height += line_spacing;
			continue;
		}

		GlyphMetrics metrics = glyph_cache->get_metrics(font_engine, canvas, glyph);
		metrics.bbox_offset.x += total_metrics.advance.width;
		metrics.bbox_offset.y += total_metrics.advance.height;

		if (first_char)
		{
			text_bbox = Rectf(metrics.bbox_offset, metrics.bbox_size);
			first_char = false;
		}
		else
		{
			Rectf glyph_bbox(metrics.bbox_offset, metrics.bbox_size);
			text_bbox.bounding_rect(glyph_bbox);
		}

		total_metrics.advance += metrics.advance;
	}

	total_metrics.bbox_offset = text_bbox.get_top_left();
	total_metrics.bbox_size = text_bbox.get_size();
	return total_metrics;
}

void Font_Impl::set_typeface_name(const std::string &name)
{
	selected_description.set_typeface_name(name);
	font_engine = nullptr;
}

void Font_Impl::set_height(float value)
{
	selected_description.set_height(value);
	font_engine = nullptr;
}

void Font_Impl::set_weight(FontWeight value)
{
	selected_description.set_weight(value);
	font_engine = nullptr;
}

void Font_Impl::set_line_height(float height)
{
	selected_description.set_line_height(height);
	font_engine = nullptr;
}
void Font_Impl::set_style(FontStyle setting)
{
	selected_description.set_style(setting);
	font_engine = nullptr;
}

}
