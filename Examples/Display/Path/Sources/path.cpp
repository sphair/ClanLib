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
	desc.set_size(clan::Size(1024, 768), true);
	desc.set_allow_resize(true);

	clan::DisplayWindow window(desc);
	clan::Canvas canvas(window);
    clan::SlotContainer cc;

	// Connect the Window close event
	cc.connect(window.sig_window_close(), clan::bind_member(this, &PathApp::on_window_close));

	// Connect a keyboard handler to on_key_up()
	cc.connect(window.get_ic().get_keyboard().sig_key_up(), clan::bind_member(this, &PathApp::on_input_up));

	clan::Path rounded_rect_shape = clan::Path::rect(clan::Rectf(0.0f, 0.0f, clan::Sizef(256, 256)), clan::Sizef(64.0f, 64.0f));

	clan::Font test_font(canvas, "tahoma", 20);
	clan::Path complex_shape = clan::Path::circle(128.0f, 128.0f, 128.0f);
	complex_shape += clan::Path::glyph(test_font, 'G').transform_self(clan::Mat3f::translate(58.0f, 198.0f) * clan::Mat3f::scale(10.0f, 10.0f));

	clan::Brush brush_solid = clan::Brush::solid_rgba8(50, 200, 150, 255);
	clan::Brush brush_image;
	brush_image.type = clan::BrushType::image;
	clan::Image image(canvas, "../../Display/Path/Resources/lobby_background2.png");
	brush_image.image = image;

	clan::Brush brush_linear;
	brush_linear.type = clan::BrushType::linear;
	brush_linear.start_point = clan::Pointf(0.0f, 0.0f);
	brush_linear.end_point = clan::Pointf(256.0f, 256.0f);
	brush_linear.stops.push_back(clan::BrushGradientStop(clan::Colorf::aliceblue, 0.0f));
	brush_linear.stops.push_back(clan::BrushGradientStop(clan::Colorf::orangered, 0.3f));
	brush_linear.stops.push_back(clan::BrushGradientStop(clan::Colorf::greenyellow, 0.7f));
	brush_linear.stops.push_back(clan::BrushGradientStop(clan::Colorf::aliceblue, 1.0f));

	clan::Brush brush_radial;
	brush_radial.type = clan::BrushType::radial;
	brush_radial.center_point = clan::Pointf(128.0f, 128.0f);
	brush_radial.radius_x = 128.0f;
	brush_radial.radius_y = 128.0f;
	brush_radial.stops.push_back(clan::BrushGradientStop(clan::Colorf::black, 0.0f));
	brush_radial.stops.push_back(clan::BrushGradientStop(clan::Colorf::white, 0.5f));
	brush_radial.stops.push_back(clan::BrushGradientStop(clan::Colorf::aliceblue, 1.0f));

	clan::Font fps_font(canvas, "tahoma", 20);
	clan::GameTime game_time;

	float angle = 0.0f;

	// Run until someone presses escape
	while (!quit)
	{
		game_time.update();

		angle += game_time.get_time_elapsed() * 32.0f;
		if (angle >= 360.0f) angle = -360.0f;
		clan::Mat4f rotation = clan::Mat4f::translate(128.0f, 128.0f, 0.0f) * clan::Mat4f::rotate(clan::Angle(angle, clan::angle_degrees), 0.0f, 0.0f, 1.0f) * clan::Mat4f::translate(-128.0f, -128.0f, 0.0f);

		canvas.clear(clan::Colorf(0.2f, 0.2f, 0.5f));

		canvas.set_transform(clan::Mat4f::translate(50.0f, 10.0f, 0.0f) * rotation);
		canvas.fill(rounded_rect_shape, brush_solid);
		canvas.set_transform(clan::Mat4f::translate(380.0f, 10.0f, 0.0f) * rotation);
		canvas.fill(complex_shape, brush_image);

		canvas.set_transform(clan::Mat4f::translate(50.0f, 300.0f, 0.0f) * rotation);
		canvas.fill(rounded_rect_shape, brush_linear);

		canvas.set_transform(clan::Mat4f::translate(380.0f, 300.0f, 0.0f) * rotation );
		canvas.fill(rounded_rect_shape, brush_radial);
		
		canvas.set_transform(clan::Mat4f::identity());
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


