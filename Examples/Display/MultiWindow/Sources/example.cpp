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
**    Mark Page
*/

#include "precomp.h"
#include "example.h"

// The start of the Application
int Example::start(const std::vector<std::string> &args)
{
	quit = false;

	// Set the window 1 description
	DisplayWindowDescription desc_window_1;
	desc_window_1.set_title("MultiWindow Example - Window 1");
	desc_window_1.set_allow_resize(true);
	desc_window_1.set_position(Rect(50, 50, Size(350, 350)), false);

	// Set the window 2 description
	DisplayWindowDescription desc_window_2;
	desc_window_2.set_title("MultiWindow Example - Window 2");
	desc_window_2.set_allow_resize(true);
	desc_window_2.set_position(Rect(50 + 350, 50, Size(350, 350)), false);

	// Open the windows
	DisplayWindow window_1(desc_window_1);
	DisplayWindow window_2(desc_window_2);

	// Connect the Window close event - to both windows
	Slot slot_quit_window_1 = window_1.sig_window_close().connect(this, &Example::on_window_close, &window_1);
	Slot slot_quit_window_2 = window_2.sig_window_close().connect(this, &Example::on_window_close, &window_2);

	// Connect a keyboard handler to on_key_up() - to both windows
	Slot slot_input_up_window_1 = (window_1.get_ic().get_keyboard()).sig_key_up().connect(this, &Example::on_input_up, 1);
	Slot slot_input_up_window_2 = (window_2.get_ic().get_keyboard()).sig_key_up().connect(this, &Example::on_input_up, 2);

	// Get the graphic context - for both windows
	Canvas canvas_1(window_1.get_gc());
	Canvas canvas_2(window_2.get_gc());

	// Load the example text - Note, any window can create the font
	Font font(canvas_1, "tahoma", 64);
	std::string example_text("This is an example ClanLib application that uses 2 windows. Try resizing and moving the windows. Press a key to flash selected window. ");
	Size text_size = font.get_text_size(canvas_1, example_text);	
	FontMetrics font_metrics = font.get_font_metrics();
	int font_yoffset = (int)( font_metrics.get_ascent() - font_metrics.get_internal_leading() ) / 2;
	int font_xoffset = 0;
	int font_counter = 0;

	// Start program timer
	unsigned int time_start = System::get_time();
	int last_elapsed = 0;

	// Set the window colors
	window_1_color = 0.2f;
	window_2_color = 0.2f;

	// Run until someone presses escape
	while (!quit)
	{
		// Get program timer
		int elapsed = System::get_time() - time_start;
		int time_delta = elapsed - last_elapsed;
		last_elapsed = elapsed;

		// Control the window colors
		window_1_color -= ((float) time_delta) / 1000.0f;
		if (window_1_color < 0.2f)
			window_1_color = 0.2f;

		window_2_color -= ((float) time_delta) / 1000.0f;
		if (window_2_color < 0.2f)
			window_2_color = 0.2f;

		// Clear both displays
		canvas_1.clear(Colorf(0.0f,0.0f,window_1_color, 1.0f));
		canvas_2.clear(Colorf(0.0f,0.0f,window_2_color, 1.0f));

		// Get the window geometry
		Rect window_geometry_1 = window_1.get_geometry();
		Rect window_geometry_2 = window_2.get_geometry();

		// Get the connecting point (right edge of window 1 and left edge of window 2)
		Point window_1_connect(window_geometry_1.get_center());
		Point window_2_connect(window_geometry_2.get_center());

		// Scroll the text
		const int font_speed = 5;
		font_counter += time_delta;
		font_counter %= text_size.width * font_speed;		// Wrap around counter
		font_xoffset = font_counter / font_speed;

		// Calculate the angle between the 2 windows
		float angle = atan2( (float) (window_2_connect.y - window_1_connect.y), (float) (window_2_connect.x - window_1_connect.x));

		// Draw the text for window 1
		// To keep the example simple, the scroll text is drawn 3 times
		canvas_1.push_modelview();
		canvas_1.mult_translate( (float) canvas_1.get_width() / 2.0f, (float) canvas_1.get_height() / 2.0f );
		canvas_1.mult_rotate( Angle(angle, angle_radians), 0.0f, 0.0f, 1.0f, false );
		font.draw_text(canvas_1, -font_xoffset, font_yoffset, example_text, Colorf::white);
		font.draw_text(canvas_1, text_size.width - font_xoffset, font_yoffset, example_text, Colorf::white);
		font.draw_text(canvas_1, -text_size.width - font_xoffset, font_yoffset, example_text, Colorf::white);
		canvas_1.pop_modelview();

		// Draw the text for window 2
		canvas_2.push_modelview();
		canvas_2.mult_translate( (float) canvas_1.get_width() / 2.0f, (float) canvas_1.get_height() / 2.0f );
		canvas_2.mult_rotate( Angle(angle, angle_radians), 0.0f, 0.0f, 1.0f, false );
		font.draw_text(canvas_2, -font_xoffset-canvas_1.get_width(), font_yoffset, example_text, Colorf::white);
		font.draw_text(canvas_2, -font_xoffset + text_size.width-canvas_1.get_width(), font_yoffset, example_text, Colorf::white);
		font.draw_text(canvas_2, -font_xoffset - text_size.width-canvas_1.get_width(), font_yoffset, example_text, Colorf::white);
		canvas_2.pop_modelview();

		canvas_1.flush();
		canvas_2.flush();

		// Flip the displays
		window_1.flip(0);
		window_2.flip(1);	// Sync to vertical blanking on the second (last) window

		// This call processes user input and other events
		KeepAlive::process(0);
	}

	return 0;
}

// A key was pressed
void Example::on_input_up(const InputEvent &key, int window_number)
{
	if(key.id == keycode_escape)
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
void Example::on_window_close(DisplayWindow *window)
{
	quit = true;
}
