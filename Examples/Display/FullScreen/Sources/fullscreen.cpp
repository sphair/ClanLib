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
#include "fullscreen.h"

// The start of the Application
int FullScreen::start(const std::vector<std::string> &args)
{
	quit = false;

	fullscreen_requested = false;
	is_fullscreen = false;

	create_window();

	Canvas canvas(window);

	const float virtual_screen_width = 800.0f;
	const float virtual_screen_height = 600.0f;

	Sprite spr_logo(canvas, "../Basic2D/Resources/logo.png");
	Sprite spr_background(canvas, "../../Game/DiceWar/Resources/lobby_background2.png");

	float sin_count = 0.0f;

	clan::ubyte64 last_time = System::get_time();

	FontDescription desc;
	desc.set_typeface_name("tahoma");
	desc.set_height(24);
	desc.set_subpixel(true);
	Font font(canvas, desc);

	// Run until someone presses escape
	while (!quit)
	{
		// Check for fullscreen switch
		if (fullscreen_requested != is_fullscreen)
		{
			is_fullscreen = fullscreen_requested;
			create_window();
		}

		if (window.get_gc() != canvas.get_gc()) 
		{
			canvas = Canvas(window); // Always get the graphic context, the window may have been recreated

		}

		clan::ubyte64 current_time = System::get_time();
		float time_delta_ms = static_cast<float> (current_time - last_time);
		last_time = current_time;

		canvas.clear(Colorf(0.0f,0.0f,0.2f));

		// Note drawing scaled fonts looks bad, even with sub pixel aliasing off
		canvas.set_modelview(Mat4f::identity());
		if (is_fullscreen)
		{
			font.draw_text(canvas, 16, 32, "Full Screen Mode. Press 'F' to switch to resizable window.");
		}
		else
		{
			font.draw_text(canvas, 16, 32, "Resizable Window. Press 'F' to switch to full screen mode.");
		}

		// Scale the drawing to the screen to a virtual screen size
		Mat4f matrix = Mat4f::scale( (float) canvas.get_width() / virtual_screen_width, (float) canvas.get_height() /virtual_screen_height, 1.0f);
		canvas.set_modelview(matrix);

		spr_logo.draw(canvas, virtual_screen_width-spr_logo.get_width(), virtual_screen_height-spr_logo.get_height());

		spr_background.set_scale(0.5f, 0.5f);
		spr_background.draw(canvas, 100, 100);

		// Show a few alpha-blending moving rectangles that moves in circles
		float x = cos(sin_count)*120.0f;
		float y = sin(sin_count)*120.0f;
		sin_count += 0.002f * time_delta_ms;
		canvas.fill_rect(Rectf( 320.0f + x -30.0f, 300.0f + y -30.0f, Sizef(60.0f, 60.0f)), Colorf(0.0f, 1.0f, 0.0, 0.5f));
		x = cos(sin_count+3.14159f)*120.0f;
		y = sin(sin_count+3.14159f)*120.0f;
		canvas.fill_rect(Rectf( 320.0f + x -30.0f, 300 + y -30.0f, Sizef(60.0f, 60.0f)), Colorf(1.0f, 1.0f, 0.0, 0.5f));

		canvas.flip(1);

		// This call processes user input and other events
		KeepAlive::process(0);
	}

	return 0;
}

// A key was pressed
void FullScreen::on_input_down(const InputEvent &key)
{
	if(key.id == keycode_escape)
	{
		quit = true;
	}

	if ((key.str == "f") ||  (key.str == "F"))
	{
		fullscreen_requested = !fullscreen_requested;
	}
}

// The window was closed
void FullScreen::on_window_close()
{
	quit = true;
}

void FullScreen::create_window()
{
	DisplayWindowDescription window_description;
	window_description.set_title("ClanLib FullScreen Example");
	window_description.set_size(Size(700, 600), true);

	if (is_fullscreen)
	{
		window_description.set_fullscreen(true);
		window_description.show_caption(false);
	}
	else
	{
		window_description.set_allow_resize(true);
	}

	window = DisplayWindow(window_description);

	slot_quit = window.sig_window_close().connect(this, &FullScreen::on_window_close);
	slot_input_up = window.get_ic().get_keyboard().sig_key_down().connect(this, &FullScreen::on_input_down);
}

