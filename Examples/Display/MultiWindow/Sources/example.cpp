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
**    Mark Page
*/

#include "precomp.h"
#include "example.h"

// The start of the Application
int Example::start(const std::vector<CL_String> &args)
{
	quit = false;

	// Set the window 1 description
	CL_DisplayWindowDescription desc_window_1;
	desc_window_1.set_title("MultiWindow Example - Window 1");
	desc_window_1.set_allow_resize(true);
	desc_window_1.set_position(CL_Rect(50, 50, CL_Size(350, 350)), false);

	// Set the window 2 description
	CL_DisplayWindowDescription desc_window_2;
	desc_window_2.set_title("MultiWindow Example - Window 2");
	desc_window_2.set_allow_resize(true);
	desc_window_2.set_position(CL_Rect(50 + 350, 50, CL_Size(350, 350)), false);

	// Open the windows
	CL_DisplayWindow window_1(desc_window_1);
	CL_DisplayWindow window_2(desc_window_2);

	// Connect the Window close event - to both windows
	CL_Slot slot_quit_window_1 = window_1.sig_window_close().connect(this, &Example::on_window_close, &window_1);
	CL_Slot slot_quit_window_2 = window_2.sig_window_close().connect(this, &Example::on_window_close, &window_2);

	// Connect a keyboard handler to on_key_up() - to both windows
	CL_Slot slot_input_up_window_1 = (window_1.get_ic().get_keyboard()).sig_key_up().connect(this, &Example::on_input_up, 1);
	CL_Slot slot_input_up_window_2 = (window_2.get_ic().get_keyboard()).sig_key_up().connect(this, &Example::on_input_up, 2);

	// Get the graphic context - for both windows
	CL_GraphicContext gc_1 = window_1.get_gc();
	CL_GraphicContext gc_2 = window_2.get_gc();

	// Load the example text - Note, any window can create the font
	CL_Font font(gc_1, "tahoma", 64);
	CL_String example_text("This is an example ClanLib application that uses 2 windows. Try resizing and moving the windows. Press a key to flash selected window. ");
	CL_Size text_size = font.get_text_size(gc_1, example_text);	
	CL_FontMetrics font_metrics = font.get_font_metrics(gc_1);
	int font_yoffset = (int)( font_metrics.get_ascent() - font_metrics.get_internal_leading() ) / 2;
	int font_xoffset = 0;

	// Start program timer
	unsigned int time_start = CL_System::get_time();
	int last_elapsed = 0;

	// Set the window colors
	window_1_color = 0.2f;
	window_2_color = 0.2f;

	// Run until someone presses escape
	while (!quit)
	{
		// Get program timer
		int elapsed = CL_System::get_time() - time_start;
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
		gc_1.clear(CL_Colorf(0.0f,0.0f,window_1_color, 1.0f));
		gc_2.clear(CL_Colorf(0.0f,0.0f,window_2_color, 1.0f));

		// Get the window geometry
		CL_Rect window_geometry_1 = window_1.get_geometry();
		CL_Rect window_geometry_2 = window_2.get_geometry();

		// Get the connecting point (right edge of window 1 and left edge of window 2)
		CL_Point window_1_connect(window_geometry_1.get_center());
		CL_Point window_2_connect(window_geometry_2.get_center());

		// Scroll the text
		font_xoffset += time_delta/5;
		font_xoffset %= text_size.width;

		// Calculate the angle between the 2 windows
		float angle = atan2( (float) (window_2_connect.y - window_1_connect.y), (float) (window_2_connect.x - window_1_connect.x));

		// Draw the text for window 1
		// To keep the example simple, the scroll text is drawn 3 times
		gc_1.push_modelview();
		gc_1.mult_translate( (float) gc_1.get_width() / 2.0f, (float) gc_1.get_height() / 2.0f );
		gc_1.mult_rotate( CL_Angle(angle, cl_radians), 0.0f, 0.0f, 1.0f, false );
		font.draw_text(gc_1, -font_xoffset, font_yoffset, example_text, CL_Colorf::white);
		font.draw_text(gc_1, text_size.width - font_xoffset, font_yoffset, example_text, CL_Colorf::white);
		font.draw_text(gc_1, -text_size.width - font_xoffset, font_yoffset, example_text, CL_Colorf::white);
		gc_1.pop_modelview();

		// Draw the text for window 2
		gc_2.push_modelview();
		gc_2.mult_translate( (float) gc_1.get_width() / 2.0f, (float) gc_1.get_height() / 2.0f );
		gc_2.mult_rotate( CL_Angle(angle, cl_radians), 0.0f, 0.0f, 1.0f, false );
		font.draw_text(gc_2, -font_xoffset-gc_1.get_width(), font_yoffset, example_text, CL_Colorf::white);
		font.draw_text(gc_2, -font_xoffset + text_size.width-gc_1.get_width(), font_yoffset, example_text, CL_Colorf::white);
		font.draw_text(gc_2, -font_xoffset - text_size.width-gc_1.get_width(), font_yoffset, example_text, CL_Colorf::white);
		gc_2.pop_modelview();

		// Flip the displays
		window_1.flip(0);
		window_2.flip(1);	// Sync to vertical blanking on the second (last) window

		// This call processes user input and other events
		CL_KeepAlive::process(0);
	}

	return 0;
}

// A key was pressed
void Example::on_input_up(const CL_InputEvent &key, const CL_InputState &state, int window_number)
{
	if(key.id == CL_KEY_ESCAPE)
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
void Example::on_window_close(CL_DisplayWindow *window)
{
	quit = true;
}
