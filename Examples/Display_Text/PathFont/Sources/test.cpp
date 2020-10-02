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
*/

#include "precomp.h"
#include "test.h"

clan::ApplicationInstance<Test> clanapp;

Test::Test()
{
//	clan::D3DTarget::set_current();
	clan::OpenGLTarget::set_current();

	// Set the window
	clan::DisplayWindowDescription desc;
	desc.set_title("ClanLib Path Font Example");
	desc.set_size(clan::Size(900, 600), true);
	desc.set_allow_resize(true);

	window = clan::DisplayWindow(desc);
	canvas = clan::Canvas(window);

	// Connect the Window close event
 	sc.connect(window.sig_window_close(), [&](){quit = true; });

	// Connect a keyboard handler to on_key_up()
	sc.connect(window.get_keyboard().sig_key_up(), clan::bind_member(this, &Test::on_input_up));

	// Describe a custom font face:

	clan::FontFamily font_face("Lobster Two");
	{
		clan::FontDescription regular;
		font_face.add(regular, "Resources/Lobster_Two/LobsterTwo-Regular.ttf");

		clan::FontDescription bold;
		bold.set_weight(clan::FontWeight::bold);
		font_face.add(bold, "Resources/Lobster_Two/LobsterTwo-Bold.ttf");

		clan::FontDescription italic;
		italic.set_style(clan::FontStyle::italic);
		font_face.add(italic, "Resources/Lobster_Two/LobsterTwo-Italic.ttf");

		clan::FontDescription bold_italic;
		bold_italic.set_weight(clan::FontWeight::bold);
		bold_italic.set_style(clan::FontStyle::italic);
		font_face.add(bold_italic, "Resources/Lobster_Two/LobsterTwo-BoldItalic.ttf");
	}

	font = clan::Font("tahoma", 16);

	path_font = clan::Font(font_face, font_height);

	brush = clan::Brush::solid(clan::Colorf::white);

	elapsed = 0.0f;
	sc.connect(window.get_mouse().sig_key_up(), [&](const clan::InputEvent &key)
	{
		float delta = 50.0f * elapsed;
		if (key.id == clan::mouse_wheel_up)
		{
			font_height += delta;
			font_height = clan::min(font_height, 300.0f);
		}
		if (key.id == clan::mouse_wheel_down)
		{
			font_height -= delta;
			font_height = clan::max(font_height, 1.0f);
		}
	});

	game_time.reset();
}

bool Test::update()
{
	game_time.update();

	path_font.set_height(font_height);
	path_font.set_weight(window.get_keyboard().get_keycode(clan::keycode_b) ? clan::FontWeight::bold : clan::FontWeight::normal);
	path_font.set_style(window.get_keyboard().get_keycode(clan::keycode_i) ? clan::FontStyle::italic : clan::FontStyle::normal);

	elapsed = game_time.get_tick_time_elapsed();

	canvas.clear(clan::Colorf(0.95f, 0.95f, 0.95f));

	font.draw_text(canvas, 17, 40, clan::string_format("%1 FPS", game_time.get_updates_per_second()), clan::Colorf::black);
	font.draw_text(canvas, canvas.get_width() - 200, 40, clan::string_format("Font Height %1", clan::StringHelp::float_to_text(font_height, 1)), clan::Colorf::black);
	font.draw_text(canvas, 17, 70, "Use Left Mouse Button to remove font information. Use Mouse Wheel to control font height", clan::Colorf::black);
	font.draw_text(canvas, 17, 100, "Use B and I buttons to control font weight and font style", clan::Colorf::black);

	std::string message = clan::StringHelp::ucs2_to_utf8({ 0xc5, 'A', 'g', 'j', 'l', 'M' });

	// Find metrics for the text:
	clan::GlyphMetrics text_metrics = path_font.measure_text(canvas, message);
	clan::FontMetrics font_metrics = path_font.get_font_metrics(canvas);

	// Center text vertically by finding where the top of the text should begin:
	float top_y = canvas.get_height() * 0.5f - font_metrics.get_height() * 0.5f;

	// Calculate font designer metrics:
	float internal_leading_y = top_y + font_metrics.get_internal_leading();
	float baseline_y = top_y + font_metrics.get_ascent();
	float bottom_y = top_y + font_metrics.get_height();
	float linegap_y = bottom_y + font_metrics.get_external_leading();

	// Layout line height: (defaults to font designer line height (ascent+descent+linegap), but can be overridden when creating Font)
	float line_top = top_y + (font_metrics.get_height() - font_metrics.get_line_height()) * 0.5f;
	float line_bottom = line_top + font_metrics.get_line_height();

	// Text bounding box:
	clan::Rectf bbox(text_metrics.bbox_offset + clan::Pointf(100.0f, baseline_y), text_metrics.bbox_size);

	bool disable_information = window.get_mouse().get_keycode(clan::mouse_left);

	if (!disable_information)
	{
		// Render our metrics lines:
		canvas.fill_rect(0.0f, line_top, canvas.get_width(), line_bottom, clan::Colorf(0.9f, 0.9f, 0.9f));
		canvas.fill_rect(bbox, clan::Colorf(0.85f, 0.85f, 0.85f));
		canvas.draw_line(0.0f, top_y, canvas.get_width(), top_y, clan::Colorf::red);
		canvas.draw_line(0.0f, internal_leading_y, canvas.get_width(), internal_leading_y, clan::Colorf::red);
		canvas.draw_line(0.0f, baseline_y, canvas.get_width(), baseline_y, clan::Colorf::red);
		canvas.draw_line(0.0f, bottom_y, canvas.get_width(), bottom_y, clan::Colorf::red);
		canvas.draw_line(0.0f, linegap_y, canvas.get_width(), linegap_y, clan::Colorf::red);
	}

	// Draw the text:
	path_font.draw_text(canvas, clan::Pointf(100.0f, baseline_y), message, clan::Colorf::black);

	if (!disable_information)
	{
		// Circle for cursor start and end:
		clan::Path::circle(100.0f, baseline_y, 4.0f).fill(canvas, clan::Colorf::dodgerblue);
		clan::Path::circle(100.0f + text_metrics.advance.width, baseline_y + text_metrics.advance.height, 4.0f).fill(canvas, clan::Colorf::dodgerblue);
	}

	window.flip(0);

	return !quit;
}

// A key was pressed
void Test::on_input_up(const clan::InputEvent &key)
{
	if(key.id == clan::keycode_escape)
	{
		quit = true;
	}
}


