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
#include "basic2d.h"

// The start of the Application
int Basic2D::start(const std::vector<std::string> &args)
{
	quit = false;

	// Set the window
	DisplayWindowDescription desc;
	desc.set_title("ClanLib Basic2D Example");
	desc.set_size(Size(640, 480), true);
	desc.set_allow_resize(true);

	Canvas canvas(desc);

	// Connect the Window close event
	Slot slot_quit = canvas.get_window().sig_window_close().connect(this, &Basic2D::on_window_close);

	// Connect a keyboard handler to on_key_up()
	Slot slot_input_up = canvas.get_window().get_ic().get_keyboard().sig_key_up().connect(this, &Basic2D::on_input_up);

	// Get the graphic context

	// Load a sprite from a png-file
	Image spr_logo(canvas, "Resources/logo.png");

	float sin_count = 0.0f;
	float ypos = 0.0f;
	float ydir = 0.3f;

	unsigned int last_time = System::get_time();

	// Run until someone presses escape
	while (!quit)
	{
		unsigned int current_time = System::get_time();
		float time_delta_ms = static_cast<float> (current_time - last_time);
		last_time = current_time;

		// Clear the display in a dark blue nuance
		canvas.clear(Colorf(0.0f,0.0f,0.2f));

		Size canvas_size = canvas.get_size();

		// Move the lines
		ypos += ydir * time_delta_ms;
		if (ydir > 0.0f)
		{
			if ((ypos+200.0f) >= canvas_size.height)
			{
				ypos = (float) (canvas_size.height - 200);
				ydir *= -1.0f;
			}
		}
		else
		{
			if (ypos <= 0.0f)
			{
				ypos = 0.0f;
				ydir *= -1.0f;
			}
		}
			
		canvas.line(0, ypos-1.0f, (float) canvas_size.width, ypos-1.0f,Colorf(0.5f, 0.0f, 0.0f));
		canvas.line(0, ypos+198.0f, (float) canvas_size.width, ypos+198.0f, Colorf(0.5f, 0.0f, 0.0f));

		// Show the logo image.
		spr_logo.draw(canvas, canvas_size.width-spr_logo.get_width(), canvas_size.height-spr_logo.get_height());
		canvas.push_cliprect(Rect(0, (int)(ypos), canvas_size.width, (int)(ypos+198)));

		// Draw a rectangle in the center of the screen
		// going from (240, 140) -> (440, 340) _not_ including the 
		// pixels in the right-most column and bottom-most row (440, 340)
		canvas.fill(Rectf(240.0f, 140.0f, 440.0f, 340.0f), Colorf::white);

		// Frame the rectangle with red lines
		canvas.box(240.0f, 140.0f, 440.0f, 340.0f, Colorf(1.0f, 0.0f, 0.0f));

		// Show a few alpha-blending moving rectangles that moves in circles
		float x = cos(sin_count)*120.0f;
		float y = sin(sin_count)*120.0f;
		sin_count += 0.004f * time_delta_ms;
		canvas.fill(Rectf( 320.0f + x -30.0f, 240.0f + y -30.0f, Sizef(30.0f, 30.0f)), Colorf(0.0f, 1.0f, 0.0, 0.5f));
		x = cos(sin_count+PI)*120.0f;
		y = sin(sin_count+PI)*120.0f;
		canvas.fill(Rectf( 320.0f + x -30.0f, 240 + y -30.0f, Sizef(30.0f, 30.0f)), Colorf(1.0f, 1.0f, 0.0, 0.5f));

		canvas.pop_cliprect();

		canvas.flip(1);

		// This call processes user input and other events
		KeepAlive::process(0);
	}

	return 0;
}

// A key was pressed
void Basic2D::on_input_up(const InputEvent &key)
{
	if(key.id == keycode_escape)
	{
		quit = true;
	}
}

// The window was closed
void Basic2D::on_window_close()
{
	quit = true;
}


