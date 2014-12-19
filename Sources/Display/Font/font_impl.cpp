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
	const FontDescription &description = font_face.impl->get_last_font().engine->get_desc();

	selected_description.typeface_name = description.get_typeface_name();
	selected_description.height = description.get_height();
	selected_description.weight = description.get_weight();
	selected_description.style = description.get_style();

	selected_line_height = description.get_line_height();

	select_font_face();
}

void Font_Impl::select_font_face()
{
	if (!font_engine)
	{
		// Copy the required font, setting a scalable font size
		Font_Selected new_selected = selected_description;
		if (selected_description.height >= selected_height_threshold)
			new_selected.height = 256.0f;	// A reasonable scalable size

		Font_Cache font_cache = font_face.impl->get_font(new_selected);
		if (!font_cache.engine)	// Font not found
			font_cache = font_face.impl->copy_font(new_selected);

		font_engine = font_cache.engine.get();
		GlyphCache *glyph_cache = font_cache.glyph_cache.get();
		PathCache *path_cache = font_cache.path_cache.get();

		const FontMetrics &metrics = font_engine->get_metrics();

		// Determine if pathfont method is required. TODO: This feels a bit hacky
		selected_pathfont = font_engine->is_automatic_recreation_allowed();
		if (selected_description.height < selected_height_threshold)
			selected_pathfont = false;

		// Deterimine if font scaling is required
		scaled_height = selected_description.height / font_engine->get_desc().get_height();
		if ((scaled_height >= 0.9999f) && (scaled_height <= 1.0001f))	// Allow for floating point accuracy issues when determining when scaling is not required
			scaled_height = 1.0f;

		// Deterimine the correct drawing engine
		if (selected_pathfont)
		{
			font_draw_path.init(path_cache, font_engine, scaled_height);
			font_draw = &font_draw_path;
		}
		else if (font_engine->get_desc().get_subpixel())
		{
			font_draw_subpixel.init(glyph_cache, font_engine);
			font_draw = &font_draw_subpixel;
		}
		else if (scaled_height == 1.0f)
		{
			font_draw_flat.init(glyph_cache, font_engine);
			font_draw = &font_draw_path;
		}
		else
		{
			font_draw_scaled.init(glyph_cache, font_engine, scaled_height);
			font_draw = &font_draw_scaled;
		}

		selected_metrics = FontMetrics(
			metrics.get_height() * scaled_height,
			metrics.get_ascent() * scaled_height,
			metrics.get_descent() * scaled_height,
			metrics.get_internal_leading() * scaled_height,
			metrics.get_external_leading() * scaled_height,
			selected_line_height	// Do not scale the line height
			);
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

	int font_height = selected_metrics.get_height();
	int font_ascent = selected_metrics.get_ascent();
	int font_external_leading = selected_metrics.get_external_leading();
	int line_spacing = static_cast<int>(selected_line_height + 0.5f);

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

			GlyphMetrics metrics = font_draw->get_metrics(canvas, glyph);

			Rect position(xpos, ypos - font_ascent, Size(metrics.advance.width, metrics.advance.height + font_height + font_external_leading));
			if (position.contains(point))
			{
				return glyph_pos + character_counter;
			}

			xpos += metrics.advance.width;
			ypos += metrics.advance.height;
		}

		dest_y += line_spacing;

		character_counter += string_length + 1;		// (Including the '\n')

	}
	return -1;	// Not found
}

const FontMetrics &Font_Impl::get_font_metrics()
{
	select_font_face();
	return selected_metrics;
}

void Font_Impl::get_glyph_path(unsigned int glyph_index, Path &out_path, GlyphMetrics &out_metrics)
{
	select_font_face();
	return font_engine->load_glyph_path(glyph_index, out_path, out_metrics);
}

void Font_Impl::draw_text(Canvas &canvas, const Pointf &position, const std::string &text, const Colorf &color)
{
	select_font_face();

	int line_spacing = static_cast<int>(selected_line_height + 0.5f);
	Pointf pos = canvas.grid_fit(position);
	font_draw->draw_text(canvas, pos, text, color, line_spacing);
}

GlyphMetrics Font_Impl::get_metrics(Canvas &canvas, unsigned int glyph)
{
	select_font_face();
	GlyphMetrics metrics = font_draw->get_metrics(canvas, glyph);
	metrics.advance *= scaled_height;
	metrics.bbox_offset *= scaled_height;
	metrics.bbox_size *= scaled_height;
	return metrics;
}


GlyphMetrics Font_Impl::measure_text(Canvas &canvas, const std::string &string)
{
	select_font_face();
	GlyphMetrics total_metrics;

	int line_spacing = static_cast<int>(selected_line_height + 0.5f);
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

		GlyphMetrics metrics = font_draw->get_metrics(canvas, glyph);

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

	total_metrics.advance *= scaled_height;
	total_metrics.bbox_offset *= scaled_height;
	total_metrics.bbox_size *= scaled_height;

	return total_metrics;
}

void Font_Impl::set_typeface_name(const std::string &name)
{
	if (selected_description.typeface_name != name)
	{
		selected_description.typeface_name = name;
		font_engine = nullptr;
	}
}

void Font_Impl::set_height(float value)
{
	if (selected_description.height != value)
	{
		selected_description.height = value;
		font_engine = nullptr;
	}
}

void Font_Impl::set_weight(FontWeight value)
{
	if (selected_description.weight != value)
	{
		selected_description.weight = value;
		font_engine = nullptr;
	}
}

void Font_Impl::set_line_height(float height)
{
	selected_line_height = height;
	// (Don't need to reset the font engine)
}

void Font_Impl::set_style(FontStyle setting)
{
	if (selected_description.style != setting)
	{
		selected_description.style = setting;
		font_engine = nullptr;
	}
}

void Font_Impl::set_scalable(float height_threshold)
{
	selected_height_threshold = height_threshold;
	// (Don't need to reset the font engine)
}

}
