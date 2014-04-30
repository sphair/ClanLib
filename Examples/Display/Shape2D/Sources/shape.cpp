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
#include "shape.h"

// The start of the Application
int ShapeApp::start(const std::vector<std::string> &args)
{
	quit = false;

	// Set the window
	clan::DisplayWindowDescription desc;
	desc.set_title("ClanLib Shape Example");
	desc.set_size(clan::Size(800, 400), true);
	desc.set_allow_resize(true);

	clan::DisplayWindow window(desc);
	clan::Canvas canvas(window);
    clan::CallbackContainer cc;

	// Connect the Window close event
	cc.connect(window.sig_window_close(), {this, &ShapeApp::on_window_close});

	// Connect a keyboard handler to on_key_up()
	cc.connect(window.get_ic().get_keyboard().sig_key_up(), {this, &ShapeApp::on_input_up});

	clan::Shape2D rounded_rect_shape;
	rounded_rect_shape.add_rounded_rect(clan::Pointf(32.0f, 32.0f), clan::Sizef(256.0f, 256.0f), 64, false );

	clan::Shape2D complex_shape;
	complex_shape.add_circle(400.0f, 200.0f, 100.0f, false);
	complex_shape.add_circle(400.0f, 200.0f, 50.0f, true);

	std::vector<clan::Vec2f> rounded_rect_triangles;
	std::vector< std::vector<clan::Vec2f> > rounded_rect_outlines;
	rounded_rect_shape.get_triangles(rounded_rect_triangles);
	rounded_rect_shape.get_outline(rounded_rect_outlines);

	std::vector<clan::Vec2f> complex_triangles;
	complex_shape.get_triangles(complex_triangles);

	clan::Texture2D texture(canvas, "../../Display/Shape2D/Resources/lobby_background2.png");

	// Run until someone presses escape
	while (!quit)
	{
		canvas.clear(clan::Colorf(0.4f,0.4f,0.9f));

		canvas.fill_triangles(rounded_rect_triangles, texture, clan::Rect(100, 100, 400, 400));
		
		for (unsigned int cnt=0; cnt<rounded_rect_outlines.size(); cnt++)
		{
			if (!rounded_rect_outlines[cnt].empty())
				canvas.draw_line_strip(&rounded_rect_outlines[cnt][0], rounded_rect_outlines[cnt].size(), clan::Colorf::white);
		}

		canvas.fill_triangles(complex_triangles, clan::Colorf::red);
		canvas.mult_translate(250, 0);
		canvas.fill_triangles(complex_triangles, texture, clan::Rect(100, 100, 400, 400));
		canvas.set_modelview(clan::Mat4f::identity());

		window.flip(1);

		// This call processes user input and other events
		clan::KeepAlive::process(0);
	}

	return 0;
}

// A key was pressed
void ShapeApp::on_input_up(const clan::InputEvent &key)
{
	if(key.id == clan::keycode_escape)
	{
		quit = true;
	}
}

// The window was closed
void ShapeApp::on_window_close()
{
	quit = true;
}


