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
**    Mark Page
*/

#include "precomp.h"
#include "example.h"

clan::ApplicationInstance<Example> clanapp;

Example::Example()
{
#if defined(WIN32) && !defined(__MINGW32__)
	clan::D3DTarget::set_current();
#else
	clan::OpenGLTarget::set_current();
#endif
	
	// Set the window 1 description
	clan::DisplayWindowDescription desc_window_1;
	desc_window_1.set_title("MultiWindow Example - Window 1");
	desc_window_1.set_allow_resize(true);
	desc_window_1.set_position(clan::Rect(50, 50, clan::Size(350, 350)), false);

	// Set the window 2 description
	clan::DisplayWindowDescription desc_window_2;
	desc_window_2.set_title("MultiWindow Example - Window 2");
	desc_window_2.set_allow_resize(true);
	desc_window_2.set_position(clan::Rect(50 + 350, 50, clan::Size(350, 350)), false);

	// Open the windows
	window_1 = clan::DisplayWindow(desc_window_1);
	window_2 = clan::DisplayWindow(desc_window_2);

	// Connect the Window close event - to both windows
	sc.connect(window_1.sig_window_close(), [&](){on_window_close(&window_1); });
	sc.connect(window_2.sig_window_close(), [&](){on_window_close(&window_2); });

	// Connect a keyboard handler to on_key_up() - to both windows
	sc.connect(window_1.get_keyboard().sig_key_up(), [=](const clan::InputEvent &input_event){on_input_up(input_event, 1); });
	sc.connect(window_2.get_keyboard().sig_key_up(), [=](const clan::InputEvent &input_event){on_input_up(input_event, 2); });

	// Get the canvas - for both windows
	canvas_1 = clan::Canvas(window_1);
	canvas_2 = clan::Canvas(window_2);

	// Load the example text - Note, any window can create the font
	font = clan::Font("tahoma", 64);

	game_time.reset();
}

bool Example::update()
{
	std::string example_text("This is an example ClanLib application that uses 2 windows. Try resizing and moving the windows. Press a key to flash selected window. ");
	clan::Size text_size = clan::Size(font.measure_text(canvas_1, example_text).bbox_size);
	clan::FontMetrics font_metrics = font.get_font_metrics(canvas_1);

	game_time.update();

	// Control the window colors
	window_1_color -= game_time.get_time_elapsed();
	if (window_1_color < 0.2f)
		window_1_color = 0.2f;

	window_2_color -= game_time.get_time_elapsed();
	if (window_2_color < 0.2f)
		window_2_color = 0.2f;

	// Clear both displays
	canvas_1.clear(clan::Colorf(0.0f,0.0f,window_1_color, 1.0f));
	canvas_2.clear(clan::Colorf(0.0f,0.0f,window_2_color, 1.0f));

	// Get the window geometry
	clan::Rect window_geometry_1 = window_1.get_geometry();
	clan::Rect window_geometry_2 = window_2.get_geometry();

	// Get the connecting point (right edge of window 1 and left edge of window 2)
	clan::Point window_1_connect(window_geometry_1.get_center());
	clan::Point window_2_connect(window_geometry_2.get_center());

	// Scroll the text
	const int font_speed = 5;
	font_counter += game_time.get_time_elapsed_ms();
	font_counter %= text_size.width * font_speed;		// Wrap around counter
	font_xoffset = font_counter / font_speed;

	// Calculate the angle between the 2 windows
	float angle = atan2( (float) (window_2_connect.y - window_1_connect.y), (float) (window_2_connect.x - window_1_connect.x));

	// Draw the text for window 1
	// To keep the example simple, the scroll text is drawn 3 times
	int font_yoffset = (int)(font_metrics.get_ascent() - font_metrics.get_internal_leading()) / 2;
	canvas_1.set_transform(clan::Mat4f::translate((float)canvas_1.get_width() / 2.0f, (float)canvas_1.get_height() / 2.0f, 0.0f) * clan::Mat4f::rotate(clan::Angle(angle, clan::AngleUnit::radians), 0.0f, 0.0f, 1.0f, false));
	font.draw_text(canvas_1, -font_xoffset, font_yoffset, example_text, clan::Colorf::white);
	font.draw_text(canvas_1, text_size.width - font_xoffset, font_yoffset, example_text,clan:: Colorf::white);
	font.draw_text(canvas_1, -text_size.width - font_xoffset, font_yoffset, example_text, clan::Colorf::white);

	// Draw the text for window 2
	canvas_2.set_transform(clan::Mat4f::translate((float)canvas_1.get_width() / 2.0f, (float)canvas_1.get_height() / 2.0f, 0.0f) * clan::Mat4f::rotate(clan::Angle(angle, clan::AngleUnit::radians), 0.0f, 0.0f, 1.0f, false));
	font.draw_text(canvas_2, -font_xoffset - canvas_1.get_width(), font_yoffset, example_text, clan::Colorf::white);
	font.draw_text(canvas_2, -font_xoffset + text_size.width-canvas_1.get_width(), font_yoffset, example_text, clan::Colorf::white);
	font.draw_text(canvas_2, -font_xoffset - text_size.width-canvas_1.get_width(), font_yoffset, example_text, clan::Colorf::white);

	// Flip the displays
	window_1.flip(0);
	window_2.flip(1);	// Sync to vertical blanking on the second (last) window

	return !quit;
}

// A key was pressed
void Example::on_input_up(const clan::InputEvent &key, int window_number)
{
	if(key.id == clan::keycode_escape)
	{
		quit = true;
	}

	if (window_number == 1)
	{
		window_1_color = 1.0f;
	}
	else
	{
		window_2_color = 1.0f;
	}
}

// The window was closed
void Example::on_window_close(clan::DisplayWindow *window)
{
	quit = true;
}
