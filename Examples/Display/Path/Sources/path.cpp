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
#include "path.h"

// The start of the Application
int PathApp::start(const std::vector<std::string> &args)
{
	quit = false;

	// Set the window
	clan::DisplayWindowDescription desc;
	desc.set_title("ClanLib Path Example");
	desc.set_size(clan::Size(800, 600), true);
	desc.set_allow_resize(true);

	clan::DisplayWindow window(desc);
	clan::Canvas canvas(window);
    clan::SlotContainer cc;

	// Connect the Window close event
	cc.connect(window.sig_window_close(), clan::bind_member(this, &PathApp::on_window_close));

	// Connect a keyboard handler to on_key_up()
	cc.connect(window.get_ic().get_keyboard().sig_key_up(), clan::bind_member(this, &PathApp::on_input_up));

	clan::Path rounded_rect_shape = clan::Path::rect(clan::Rectf(32.0f, 32.0f, clan::Sizef(256.0f, 256.0f)), clan::Sizef(64.0f, 64.0f) );

	clan::Path complex_shape = clan::Path::circle(400.0f, 200.0f, 100.0f);
	//clan::Path complex_shape = clan::Path::circle(400.0f, 200.0f, 100.0f) + clan::Path::circle(400.0f, 200.0f, 50.0f);

	clan::Brush brush = clan::Brush::solid_rgba8(50, 200, 150, 255);
	clan::Image image(canvas, "../../Display/Path/Resources/lobby_background2.png");
	clan::Font fps_font(canvas, "tahoma", 20);
	clan::GameTime game_time;

	// Run until someone presses escape
	while (!quit)
	{
		game_time.update();
		canvas.clear(clan::Colorf(0.2f, 0.2f, 0.5f));

		canvas.fill(rounded_rect_shape, brush);
		canvas.fill(complex_shape, brush);

		std::string fps = clan::string_format("%1 fps", clan::StringHelp::float_to_text(game_time.get_updates_per_second(), 1));
		fps_font.draw_text(canvas, canvas.get_width() - 100, 30, fps);

		window.flip(0);

		// This call processes user input and other events
		clan::KeepAlive::process(0);
	}

	return 0;
}

// A key was pressed
void PathApp::on_input_up(const clan::InputEvent &key)
{
	if(key.id == clan::keycode_escape)
	{
		quit = true;
	}
}

// The window was closed
void PathApp::on_window_close()
{
	quit = true;
}


