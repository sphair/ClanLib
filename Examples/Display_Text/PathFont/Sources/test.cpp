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

#include "precomp.h"
#include "test.h"

// The start of the Application
int Test::start(const std::vector<std::string> &args)
{
	quit = false;


	// Set the window
	clan::DisplayWindowDescription desc;
	desc.set_title("ClanLib Path Font Example");
	desc.set_size(clan::Size(800, 600), true);
	desc.set_allow_resize(true);

	clan::DisplayWindow window(desc);
	clan::Canvas canvas(window);

	// Connect the Window close event
    clan::SlotContainer sc;
	sc.connect(window.sig_window_close(), [&](){quit = true; });

	// Connect a keyboard handler to on_key_up()
	sc.connect(window.get_ic().get_keyboard().sig_key_up(), clan::bind_member(this, &Test::on_input_up));

	clan::Font font(canvas, "tahoma", 16);

	clan::PathFont path_font("arial", 250);

	clan::Brush brush = clan::Brush::solid(clan::Colorf::white);

	clan::GameTime game_time;

	// Run until someone presses escape
	while (!quit)
	{
		game_time.update();

		canvas.clear(clan::Colorf(0.0f,0.0f,0.3f));

		font.draw_text(canvas, 17, 40, clan::string_format("%1 FPS", game_time.get_updates_per_second()), clan::Colorf::white);

		std::string message = "joy";
		clan::GlyphMetrics text_metrics = path_font.measure_text(canvas, message);
		clan::FontMetrics font_metrics = path_font.get_font_metrics();

		clan::Pointf box_position((canvas.get_width() - text_metrics.bbox_size.width) / 2.0f, (canvas.get_height() - text_metrics.bbox_size.height) / 2.0f);
		clan::Rectf inner_rect(box_position, text_metrics.bbox_size);
		clan::Rectf offset_rect = inner_rect;
		offset_rect.left = offset_rect.left + text_metrics.bbox_offset.x;
		offset_rect.top = offset_rect.bottom - text_metrics.bbox_offset.y;

		canvas.fill_rect(inner_rect, clan::Colorf::black);
		canvas.fill_rect(offset_rect, clan::Colorf(1.0f, 1.0f, 1.0f, 0.2f));
		canvas.draw_box(inner_rect, clan::Colorf::green);
		canvas.draw_box(offset_rect, clan::Colorf::green);

		clan::Pointf text_base(inner_rect.left, text_metrics.bbox_offset.y + inner_rect.top);

		draw_info(canvas, font, "Ascender", text_base.y, text_base.y - font_metrics.get_ascent(), 10.0f);
		draw_info(canvas, font, "Decender", text_base.y, text_base.y + font_metrics.get_descent(), 15.0f);
		draw_info(canvas, font, "External Leading", text_base.y + font_metrics.get_descent(), text_base.y + font_metrics.get_descent() + font_metrics.get_external_leading(), 20.0f);
		draw_info(canvas, font, "Internal Leading", text_base.y - font_metrics.get_ascent(), text_base.y - font_metrics.get_ascent() + font_metrics.get_internal_leading(), 20.0f);

		path_font.draw_text(canvas, text_base, message, brush);

		window.flip(1);

		// This call processes user input and other events
		clan::KeepAlive::process(0);
	}

	return 0;
}

void Test::draw_info(clan::Canvas &canvas, clan::Font &font, const std::string &info, float ypos_a, float ypos_b, float info_xpos)
{
	canvas.draw_line(0.0f, ypos_a, canvas.get_width(), ypos_a, clan::Colorf::red);
	canvas.draw_line(0.0f, ypos_b, canvas.get_width(), ypos_b, clan::Colorf::red);

	canvas.draw_line(info_xpos, ypos_a, info_xpos, ypos_b, clan::Colorf::white);

	font.draw_text(canvas, info_xpos + 2.0f, (ypos_a + ypos_b) / 2.0f + 4.0f, info, clan::Colorf::yellow);
}

// A key was pressed
void Test::on_input_up(const clan::InputEvent &key)
{
	if(key.id == clan::keycode_escape)
	{
		quit = true;
	}
}


