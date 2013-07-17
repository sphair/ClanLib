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
	desc.set_title("ClanLib PathApp Example");
	desc.set_size(clan::Size(1024, 680), true);
	desc.set_allow_resize(true);

	clan::DisplayWindow window(desc);
	clan::Canvas canvas(window);

	// Connect the Window close event
	clan::Slot slot_quit = window.sig_window_close().connect(this, &PathApp::on_window_close);

	// Connect a keyboard handler to on_key_up()
	clan::Slot slot_input_up = window.get_ic().get_keyboard().sig_key_up().connect(this, &PathApp::on_input_up);

	clan::Shape2D shape;

	//shape.add_circle(circle_x, circle_y, circle_radius, false);
	//shape.add_circle(circle_x+0.0f, circle_y + 0.0f, 64.0f, true);
	//shape.add_ellipse(circle_x, circle_y, circle_radius, circle_radius / 2.0f, false);
	//shape.add_ellipse(circle_x+0.0f, circle_y + 0.0f, 64.0f, 32.0f, true);
	//shape.add_rounded_rect(clan::Pointf(100.0f, 400.0f), clan::Sizef(256.0f, 128.0f), 32, clan::Angle(45.0f, clan::angle_degrees), false );
	//shape.add_rounded_rect(clan::Pointf(140.0f, 440.0f), clan::Sizef(200.0f, 32.0f), 32, clan::Angle(45.0f, clan::angle_degrees), true );
	shape.add_rounded_rect(clan::Pointf(32.0f, 32.0f), clan::Sizef(512.0f, 512.0f), 128, false );
	//shape.add_rect(clan::Rectf(100.0f, 400.0f, clan::Sizef(256.0f, 128.0f)), false);
	//shape.add_rect(clan::Rectf(100.0f, 400.0f, clan::Sizef(256.0f, 128.0f)), clan::Angle(12.0f, clan::angle_degrees), false);
	//shape.add_rounded_line(clan::Pointf(128.0f, 128.0f), clan::Pointf(400.0f, 600.0f), 32, 16, false);

	std::vector<clan::Vec2f> object_triangles;
	std::vector< std::vector<clan::Vec2f> > object_outline;
	shape.get_triangles(object_triangles);
	shape.get_outline(object_outline);

	clan::Texture2D texture(canvas, "../../../Examples/Game/DiceWar/Resources/lobby_background2.png");

	clan::ubyte64 last_time = clan::System::get_time();
	float angle = 0.0;

	// Run until someone presses escape
	while (!quit)
	{
		clan::ubyte64 current_time = clan::System::get_time();
		float time_delta_ms = static_cast<float> (current_time - last_time);
		last_time = current_time;

		// Update the moving elements
		angle += 0.004f * time_delta_ms;

		// Clear the display in a dark blue nuance
		canvas.clear(clan::Colorf(0.4f,0.4f,0.9f));

	//	canvas.fill_circle(128.0f, 128.0f, 4.0f, clan::Colorf::red);
	//	canvas.fill_circle(256.0f, 128.0f, 4.0f, clan::Colorf::red);
	//	canvas.fill_circle(100.0, 228.0f, 4.0f, clan::Colorf::red);

		std::vector< std::vector<clan::Vec2f> >::const_iterator it;

		float size = 64.0f + sin(angle) * 32.0f;

	//		canvas.fill_triangles(object_triangles, texture, clan::Rect(100, 100, 400, 400));
		canvas.fill_triangles(object_triangles, texture, clan::Gradient(
			clan::Colorf(1.0f, 1.0f, 1.0f, 1.0f),
			clan::Colorf(1.0f, 1.0f, 0.0f, 1.0f),
			clan::Colorf(0.0f, 0.0f, 0.0f, 0.0f),
			clan::Colorf(1.0f, 0.0f, 1.0f, 0.5f)));


		window.flip(1);

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


