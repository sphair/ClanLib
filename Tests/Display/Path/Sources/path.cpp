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
#include "path.h"

// The start of the Application
int PathApp::start(const std::vector<std::string> &args)
{
	quit = false;

	// Set the window
	clan::DisplayWindowDescription desc;
	desc.set_title("ClanLib PathApp Example");
	desc.set_size(clan::Size(800, 680), true);
	desc.set_allow_resize(true);

	clan::Canvas canvas(desc);

	// Connect the Window close event
	clan::Slot slot_quit = canvas.get_window().sig_window_close().connect(this, &PathApp::on_window_close);

	// Connect a keyboard handler to on_key_up()
	clan::Slot slot_input_up = canvas.get_window().get_ic().get_keyboard().sig_key_up().connect(this, &PathApp::on_input_up);

	clan::Shape2D shape;
	clan::Path2D path;
	clan::BezierCurve bezier_curve;

	clan::Font_Vector font(canvas, "tahoma", 128);
	const std::vector<clan::Vec2f> &primitives_array_g = font.get_glyph_filled('g');
	const std::vector< std::vector<clan::Vec2f> > &primitives_array_outline_g = font.get_glyph_outline('g');

	shape = clan::Shape2D();
	path = clan::Path2D();
	path.add_line_to(10, 200);
	path.add_line_to(108, 210);
	path.add_line_to(100, 300);
	path.add_line_to(20, 310);
	shape.add_path(path);

	std::vector<clan::Vec2f> primitives_array_polygon;
	std::vector< std::vector<clan::Vec2f> > primitives_array_outline_polygon;
	shape.get_triangles(primitives_array_polygon);
	shape.get_outline(primitives_array_outline_polygon);

	#define KAPPA		0.5522847498f
	shape = clan::Shape2D();
	path = clan::Path2D();
	float circle_x = 100.0f;
	float circle_y = 400.0f;
	float circle_radius = 64.0f;

	bezier_curve = clan::BezierCurve();
	bezier_curve.add_control_point(circle_x + circle_radius, circle_y);
	bezier_curve.add_control_point(circle_x + circle_radius * 1, (circle_y + circle_radius * 1 * KAPPA));
	bezier_curve.add_control_point(circle_x + circle_radius * 1 * KAPPA, (circle_y + circle_radius * 1));
	bezier_curve.add_control_point(circle_x + circle_radius * 0, (circle_y + circle_radius * 1));
	bezier_curve.add_control_point(circle_x - circle_radius * 1  * KAPPA, (circle_y + circle_radius * 1));
	bezier_curve.add_control_point(circle_x - circle_radius * 1, (circle_y + circle_radius * 1 * KAPPA));
	bezier_curve.add_control_point(circle_x - circle_radius * 1, (circle_y + circle_radius * 0));
	bezier_curve.add_control_point(circle_x - circle_radius * 1, (circle_y - circle_radius * 1  * KAPPA));
	bezier_curve.add_control_point(circle_x - circle_radius * 1 * KAPPA, (circle_y - circle_radius * 1));
	bezier_curve.add_control_point(circle_x - circle_radius * 0, (circle_y - circle_radius * 1));
	bezier_curve.add_control_point(circle_x + circle_radius * 1 * KAPPA, (circle_y - circle_radius * 1));
	bezier_curve.add_control_point(circle_x + circle_radius * 1, (circle_y - circle_radius * 1 * KAPPA));
	bezier_curve.add_control_point(circle_x + circle_radius * 1, (circle_y - circle_radius * 0));
	path.add_curve(bezier_curve);
	shape.add_path(path);

	std::vector<clan::Vec2f> primitives_array_circle;
	std::vector< std::vector<clan::Vec2f> > primitives_array_outline_circle;
	shape.get_triangles(primitives_array_circle);
	shape.get_outline(primitives_array_outline_circle);

	clan::Texture2D texture(canvas, "../../../Examples/Game/DiceWar/Resources/lobby_background2.png");

	std::vector<clan::Vec2f> texture_array_circle;
	texture_array_circle.resize(primitives_array_circle.size());
	for (unsigned int cnt=0; cnt< primitives_array_circle.size(); cnt++)
	{
		clan::Vec2f pos = primitives_array_circle[cnt];
		pos.x -= circle_x;
		pos.y -= circle_y;
		pos.x /= circle_radius*2.0f;
		pos.y /= circle_radius*2.0f;
		pos.x += 0.5f;
		pos.y += 0.5f;
		texture_array_circle[cnt] = pos;
	}

	std::vector<clan::Vec2f> texture_array_g;
	texture_array_g.resize(primitives_array_g.size());
	clan::Rectf bounding_rect(10000.0f, 10000.0f, -10000.0f, -10000.0f);
	for (unsigned int cnt=0; cnt< primitives_array_g.size(); cnt++)
	{
		clan::Vec2f pos = primitives_array_g[cnt];
		bounding_rect.left = clan::min(bounding_rect.left, pos.x);
		bounding_rect.top = clan::min(bounding_rect.top, pos.y);
		bounding_rect.right = clan::max(bounding_rect.right, pos.x);
		bounding_rect.bottom = clan::max(bounding_rect.bottom, pos.y);
	}
	for (unsigned int cnt=0; cnt< primitives_array_g.size(); cnt++)
	{
		clan::Vec2f pos = primitives_array_g[cnt];
		pos.x -= bounding_rect.left;
		pos.y -= bounding_rect.top;
		pos.x /= bounding_rect.get_width();
		pos.y /= bounding_rect.get_height();
		texture_array_g[cnt] = pos;
	}

	// Run until someone presses escape
	while (!quit)
	{
		// Clear the display in a dark blue nuance
		canvas.clear(clan::Colorf(0.0f,0.0f,0.2f));

		canvas.push_translate(64, 128);
		canvas.fill_triangles(&(primitives_array_g[0]), primitives_array_g.size());
		canvas.pop_modelview();

		canvas.push_translate(200, 128);
		std::vector< std::vector<clan::Vec2f> >::const_iterator it;
		for (it = primitives_array_outline_g.begin(); it != primitives_array_outline_g.end(); ++it)
		{
			canvas.draw_line_strip(&((*it)[0]), it->size());
		}
		canvas.pop_modelview();

		canvas.push_translate(360, 160);
		canvas.mult_scale(2.0f, 2.0f);
		canvas.fill_triangles(&primitives_array_g[0], &texture_array_g[0], primitives_array_g.size(), texture);
		canvas.pop_modelview();

		if (!primitives_array_polygon.empty())
			canvas.fill_triangles(&(primitives_array_polygon[0]), primitives_array_polygon.size());

		canvas.push_translate(200, 0);
		for (it = primitives_array_outline_polygon.begin(); it != primitives_array_outline_polygon.end(); ++it)
		{
			canvas.draw_line_strip(&((*it)[0]), it->size());
		}
		canvas.pop_modelview();


		if (!primitives_array_circle.empty())
			canvas.fill_triangles(&(primitives_array_circle[0]), primitives_array_circle.size());

		canvas.push_translate(200, 0);
		for (it = primitives_array_outline_circle.begin(); it != primitives_array_outline_circle.end(); ++it)
		{
			canvas.draw_line_strip(&((*it)[0]), it->size());
		}
		canvas.pop_modelview();

		canvas.push_translate(400, 0);
		if (!primitives_array_circle.empty())
			canvas.fill_triangles(&primitives_array_circle[0], &texture_array_circle[0], primitives_array_circle.size(), texture);
		canvas.pop_modelview();


		canvas.flip(1);

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


