/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
int Basic2D::start(const std::vector<CL_String> &args)
{
	quit = false;

	// Set the window
	CL_DisplayWindowDescription desc;
	desc.set_title("ClanLib Basic2D Example");
	desc.set_size(CL_Size(640, 480), true);
	desc.set_allow_resize(true);

	CL_DisplayWindow window(desc);

	// Connect the Window close event
	CL_Slot slot_quit = window.sig_window_close().connect(this, &Basic2D::on_window_close);

	// Connect a keyboard handler to on_key_up()
	CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &Basic2D::on_input_up);

	// Get the graphic context
	CL_GraphicContext gc = window.get_gc();

	// Load a sprite from a png-file
	CL_Sprite spr_logo(gc, "Resources/logo.png");

	float sin_count = 0.0f;
	float ypos = 0.0f;
	float ydir = 0.3f;

	unsigned int last_time = CL_System::get_time();

	// Run until someone presses escape
	while (!quit)
	{
		unsigned int current_time = CL_System::get_time();
		float time_delta_ms = static_cast<float> (current_time - last_time);
		last_time = current_time;

		// Clear the display in a dark blue nuance
		// The four arguments are red, green, blue and alpha
		gc.clear(CL_Colorf(0.0f,0.0f,0.2f));

		// Move the lines
		ypos += ydir * time_delta_ms;
		if (ydir > 0.0f)
		{
			if ((ypos+200.0f) >= gc.get_height())
			{
				ypos = (float) (gc.get_height() - 200);
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
			
		CL_Draw::line(gc, 0, ypos-1.0f, (float) gc.get_width(), ypos-1.0f,CL_Colorf(0.5f, 0.0f, 0.0f));
		CL_Draw::line(gc, 0, ypos+198.0f, (float) gc.get_width(), ypos+198.0f, CL_Colorf(0.5f, 0.0f, 0.0f));

		// Show the logo image.
		// Use the get_width() and get_height() functions of both
		// CL_DisplayWindow and CL_Sprite, to show the image in the bottom right corner
		spr_logo.draw(gc, 
			(float) gc.get_width()-spr_logo.get_width(),
			(float) gc.get_height()-spr_logo.get_height());

		gc.push_cliprect(CL_Rect(0, (int)(ypos), gc.get_width(), (int)(ypos+198)));

		// Draw a rectangle in the center of the screen
		// going from (240, 140) -> (440, 340) _not_ including the 
		// pixels in the right-most column and bottom-most row (440, 340)
		CL_Draw::fill(gc, CL_Rectf(240.0f, 140.0f, 440.0f, 340.0f), CL_Colorf(1.0f, 1.0f, 1.0f));

		// Frame the rectangle with red lines
		CL_Draw::box(gc, 240.0f, 140.0f, 440.0f, 340.0f, CL_Colorf(1.0f, 0.0f, 0.0f));

		// Show a few alpha-blending moving rectangles that moves in circles
		float x = cos(sin_count)*120.0f;
		float y = sin(sin_count)*120.0f;
		sin_count += 0.004f * time_delta_ms;
		CL_Draw::fill(gc, CL_Rectf( 320.0f + x -30.0f, 240.0f + y -30.0f, CL_Sizef(30.0f, 30.0f)), CL_Colorf(0.0f, 1.0f, 0.0, 0.5f));
		x = cos(sin_count+3.14159f)*120.0f;
		y = sin(sin_count+3.14159f)*120.0f;
		CL_Draw::fill(gc, CL_Rectf( 320.0f + x -30.0f, 240 + y -30.0f, CL_Sizef(30.0f, 30.0f)), CL_Colorf(1.0f, 1.0f, 0.0, 0.5f));

		gc.pop_cliprect();

		// Flip the display, showing on the screen what we have drawed
		// since last call to flip()
		window.flip(1);

		// This call processes user input and other events
		CL_KeepAlive::process(0);
	}

	return 0;
}

// A key was pressed
void Basic2D::on_input_up(const CL_InputEvent &key, const CL_InputState &state)
{
	if(key.id == CL_KEY_ESCAPE)
	{
		quit = true;
	}
}

// The window was closed
void Basic2D::on_window_close()
{
	quit = true;
}


