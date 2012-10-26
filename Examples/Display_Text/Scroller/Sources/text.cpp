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
*/

#include "precomp.h"
#include "text.h"

Text::Text()
{
}

void Text::init(GraphicContext &gc, const FontDescription &font_description, const DomElement &configuration, const char *text_name)
{
	font = Font(gc, font_description);

	text = configuration.get_child_string(text_name);
	if (text.length() == 0)
		throw Exception("Cannot find the text");

	UTF8_Reader reader(text);
	init_stage1_count_glyphs(reader);
	init_stage2_examine_glyphs(gc, reader);

	font_metrics = font.get_font_metrics();

	scroller_xoffset = 0;
	time_counter = 0;
}

void Text::run(int time_delta_ms, int speed)
{
	if (time_delta_ms > 1000)	// Do not allow frame rate slower than 1 second a pixel
		time_delta_ms = 1000;

	time_counter += time_delta_ms;
	int pixel_offset = time_counter / speed;

	time_counter -= (pixel_offset * speed); 

	scroller_xoffset += pixel_offset;
	while (scroller_xoffset > total_width)
		scroller_xoffset -=total_width;
}

void Text::draw(GraphicContext &gc, const Rect &rect)
{
	int out_draw_offset;
	std::string text = build_text(gc, rect.get_width(), scroller_xoffset, out_draw_offset);

	int ypos = rect.bottom - ((rect.get_height() - font_metrics.get_ascent())/2);

	// Remove the next line to observe how the clipping works
	gc.set_cliprect(rect);
	font.draw_text(gc, rect.left - out_draw_offset, ypos, text);
	gc.reset_cliprect();
}

std::string Text::build_text(GraphicContext &gc, int width, int xoffset, int &out_draw_offset)
{
	const char *str = text.c_str();

	// Find the start char. This can be made more efficient
	int text_offset = 0;
	while(true)
	{
		xoffset -= glyph_width[text_offset];
		if (xoffset < 0)
			break;

		text_offset++;

		if (text_offset >= glyph_width.size())
			text_offset = 0;
	}
	out_draw_offset = glyph_width[text_offset] + xoffset;
	width +=  glyph_width[text_offset];
	
	// Build the rest of the string (repeating)
	int text_start_offset = text_offset;
	std::string built_string;
	while(true)
	{
		width -= glyph_width[text_offset];
		if (width < 0)
		{
			text_offset++;	// Add an extra glyph, that will be partially clipped
			if (text_offset >= glyph_width.size())
				text_offset--;	// Unless at end of the string

			built_string = built_string + std::string(str + glyph_offset[text_start_offset], glyph_offset[text_offset] - glyph_offset[text_start_offset]);
			break;
		}

		text_offset++;

		if (text_offset >= glyph_width.size())
		{
			built_string = built_string + std::string(str + glyph_offset[text_start_offset]);

			text_offset = 0;
			text_start_offset = 0;
		}

	}
	return built_string;

}

void Text::init_stage1_count_glyphs(UTF8_Reader &reader)
{
	reader.set_position(0);
	int num_glyphs = 0;
	while(!reader.is_end())
	{
		num_glyphs++;
		reader.next();
	}
	glyph_offset.resize(num_glyphs);
	glyph_width.resize(num_glyphs);
}

void Text::init_stage2_examine_glyphs(GraphicContext &gc, UTF8_Reader &reader)
{
	reader.set_position(0);
	int current_glyph_count = 0;
	total_width = 0;
	while(!reader.is_end())
	{
		glyph_offset[current_glyph_count] = reader.get_position();

		Size size = font.get_glyph_size(gc, reader.get_char());
		glyph_width[current_glyph_count] = size.width;
		total_width += size.width;

		current_glyph_count++;
		reader.next();
	}

}
