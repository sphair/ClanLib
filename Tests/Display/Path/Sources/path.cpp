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

	clan::PathGroup path_group;
	clan::Path path;
	clan::BezierCurve bezier_curve;
	path = clan::Path();
	path.add_line_to(50.500008f,18.937515f);
	path.add_line_to(50.500008f,57.250004f);
	path.add_line_to(50.500008f,57.250004f);
	bezier_curve = clan::BezierCurve();
	bezier_curve.add_control_point(47.583338f,58.666673f);
	bezier_curve.add_control_point(44.885422f,59.677091f);
	bezier_curve.add_control_point(42.406258f,60.281258f);
	bezier_curve.add_control_point(39.927094f,60.885422f);
	bezier_curve.add_control_point(37.458341f,61.187504f);
	bezier_curve.add_control_point(35.000000f,61.187504f);
	path.add_curve(bezier_curve);
	path.add_line_to(35.000000f,61.187504f);
	bezier_curve = clan::BezierCurve();
	bezier_curve.add_control_point(29.041668f,61.187504f);
	bezier_curve.add_control_point(24.354170f,59.000003f);
	bezier_curve.add_control_point(20.937508f,54.625000f);
	bezier_curve.add_control_point(17.520844f,50.250000f);
	bezier_curve.add_control_point(15.812512f,43.979170f);
	bezier_curve.add_control_point(15.812512f,35.812511f);
	path.add_curve(bezier_curve);
	path.add_line_to(15.812512f,35.812511f);
	bezier_curve = clan::BezierCurve();
	bezier_curve.add_control_point(15.812512f,28.062506f);
	bezier_curve.add_control_point(17.114595f,22.166670f);
	bezier_curve.add_control_point(19.718761f,18.125002f);
	bezier_curve.add_control_point(22.322927f,14.083335f);
	bezier_curve.add_control_point(26.541676f,12.062501f);
	bezier_curve.add_control_point(32.375008f,12.062501f);
	path.add_curve(bezier_curve);
	path.add_line_to(32.375008f,12.062501f);
	bezier_curve = clan::BezierCurve();
	bezier_curve.add_control_point(35.416679f,12.062501f);
	bezier_curve.add_control_point(38.552099f,12.666669f);
	bezier_curve.add_control_point(41.781265f,13.875006f);
	bezier_curve.add_control_point(45.010429f,15.083342f);
	bezier_curve.add_control_point(47.916677f,16.770845f);
	bezier_curve.add_control_point(50.500008f,18.937515f);
	path.add_curve(bezier_curve);
	path_group.add_path(path);
	path = clan::Path();
	path.add_line_to(11.000000f,-11.187488f);
	bezier_curve = clan::BezierCurve();
	bezier_curve.add_control_point(11.791669f,-11.520826f);
	bezier_curve.add_control_point(12.958338f,-11.968744f);
	bezier_curve.add_control_point(14.500008f,-12.531242f);
	bezier_curve.add_control_point(16.041678f,-13.093741f);
	bezier_curve.add_control_point(17.791679f,-13.624989f);
	bezier_curve.add_control_point(19.750011f,-14.124987f);
	path.add_curve(bezier_curve);
	path.add_line_to(19.750011f,-14.124987f);
	bezier_curve = clan::BezierCurve();
	bezier_curve.add_control_point(21.916670f,-14.666662f);
	bezier_curve.add_control_point(23.916668f,-15.104166f);
	bezier_curve.add_control_point(25.750004f,-15.437499f);
	bezier_curve.add_control_point(27.583340f,-15.770832f);
	bezier_curve.add_control_point(29.583342f,-15.937499f);
	bezier_curve.add_control_point(31.750011f,-15.937499f);
	path.add_curve(bezier_curve);
	path.add_line_to(31.750011f,-15.937499f);
	bezier_curve = clan::BezierCurve();
	bezier_curve.add_control_point(35.375006f,-15.937499f);
	bezier_curve.add_control_point(38.447922f,-15.458330f);
	bezier_curve.add_control_point(40.968758f,-14.499992f);
	bezier_curve.add_control_point(43.489594f,-13.541655f);
	bezier_curve.add_control_point(45.458346f,-12.208321f);
	bezier_curve.add_control_point(46.875015f,-10.499992f);
	path.add_curve(bezier_curve);
	path.add_line_to(46.875015f,-10.499992f);
	bezier_curve = clan::BezierCurve();
	bezier_curve.add_control_point(48.208349f,-8.791663f);
	bezier_curve.add_control_point(49.145847f,-6.812497f);
	bezier_curve.add_control_point(49.687511f,-4.562493f);
	bezier_curve.add_control_point(50.229176f,-2.312490f);
	bezier_curve.add_control_point(50.500008f,0.250009f);
	bezier_curve.add_control_point(50.500008f,3.125002f);
	path.add_curve(bezier_curve);
	path.add_line_to(50.500008f,3.125002f);
	path.add_line_to(50.500008f,9.250004f);
	path.add_line_to(50.500008f,9.250004f);
	bezier_curve = clan::BezierCurve();
	bezier_curve.add_control_point(47.333344f,6.500003f);
	bezier_curve.add_control_point(44.229178f,4.468752f);
	bezier_curve.add_control_point(41.187511f,3.156252f);
	bezier_curve.add_control_point(38.145845f,1.843753f);
	bezier_curve.add_control_point(34.333345f,1.187503f);
	bezier_curve.add_control_point(29.750011f,1.187503f);
	path.add_curve(bezier_curve);
	path.add_line_to(29.750011f,1.187503f);
	bezier_curve = clan::BezierCurve();
	bezier_curve.add_control_point(21.791679f,1.187503f);
	bezier_curve.add_control_point(15.458346f,4.145838f);
	bezier_curve.add_control_point(10.750011f,10.062510f);
	bezier_curve.add_control_point(6.041677f,15.979180f);
	bezier_curve.add_control_point(3.687510f,24.479179f);
	bezier_curve.add_control_point(3.687510f,35.562508f);
	path.add_curve(bezier_curve);
	path.add_line_to(3.687510f,35.562508f);
	bezier_curve = clan::BezierCurve();
	bezier_curve.add_control_point(3.687510f,41.354172f);
	bezier_curve.add_control_point(4.500010f,46.489588f);
	bezier_curve.add_control_point(6.125010f,50.968758f);
	bezier_curve.add_control_point(7.750009f,55.447927f);
	bezier_curve.add_control_point(9.937509f,59.250010f);
	bezier_curve.add_control_point(12.687510f,62.375008f);
	path.add_curve(bezier_curve);
	path.add_line_to(12.687510f,62.375008f);
	bezier_curve = clan::BezierCurve();
	bezier_curve.add_control_point(15.312510f,65.375013f);
	bezier_curve.add_control_point(18.416676f,67.687515f);
	bezier_curve.add_control_point(22.000008f,69.312515f);
	bezier_curve.add_control_point(25.583340f,70.937515f);
	bezier_curve.add_control_point(29.291672f,71.750015f);
	bezier_curve.add_control_point(33.125004f,71.750015f);
	path.add_curve(bezier_curve);
	path.add_line_to(33.125004f,71.750015f);
	bezier_curve = clan::BezierCurve();
	bezier_curve.add_control_point(36.750006f,71.750015f);
	bezier_curve.add_control_point(39.885424f,71.322929f);
	bezier_curve.add_control_point(42.531258f,70.468758f);
	bezier_curve.add_control_point(45.177091f,69.614586f);
	bezier_curve.add_control_point(47.833341f,68.395838f);
	bezier_curve.add_control_point(50.500008f,66.812515f);
	path.add_curve(bezier_curve);
	path.add_line_to(50.500008f,66.812515f);
	path.add_line_to(51.187504f,69.812515f);
	path.add_line_to(62.250004f,69.812515f);
	path.add_line_to(62.250004f,7.937514f);
	path.add_line_to(62.250004f,7.937514f);
	bezier_curve = clan::BezierCurve();
	bezier_curve.add_control_point(62.250004f,-3.895827f);
	bezier_curve.add_control_point(59.666672f,-12.583330f);
	bezier_curve.add_control_point(54.500008f,-18.124994f);
	bezier_curve.add_control_point(49.333344f,-23.666658f);
	bezier_curve.add_control_point(41.437512f,-26.437490f);
	bezier_curve.add_control_point(30.812513f,-26.437490f);
	path.add_curve(bezier_curve);
	path.add_line_to(30.812513f,-26.437490f);
	bezier_curve = clan::BezierCurve();
	bezier_curve.add_control_point(27.395845f,-26.437490f);
	bezier_curve.add_control_point(23.906260f,-26.166658f);
	bezier_curve.add_control_point(20.343756f,-25.624992f);
	bezier_curve.add_control_point(16.781252f,-25.083328f);
	bezier_curve.add_control_point(13.458335f,-24.354161f);
	bezier_curve.add_control_point(10.375006f,-23.437490f);
	path.add_curve(bezier_curve);
	path.add_line_to(10.375006f,-23.437490f);
	path.add_line_to(10.375006f,-11.187488f);
	path_group.add_path(path);

	std::vector<clan::Vec2f> primitives_array_g;
	std::vector< std::vector<clan::Vec2f> > primitives_array_outline_g;
	path_group.triangulate_combined(primitives_array_g, primitives_array_outline_g);

	path_group = clan::PathGroup();
	path = clan::Path();
	path.add_line_to(10, -200);	//FIXME - Why negative
	path.add_line_to(108, -210);
	path.add_line_to(100, -300);
	path.add_line_to(20, -310);
	path_group.add_path(path);

	std::vector<clan::Vec2f> primitives_array_polygon;
	std::vector< std::vector<clan::Vec2f> > primitives_array_outline_polygon;
	path_group.triangulate_combined(primitives_array_polygon, primitives_array_outline_polygon);


	#define KAPPA		0.5522847498f
	path_group = clan::PathGroup();
	path = clan::Path();
	float circle_x = 100.0f;
	float circle_y = 400.0f;
	float circle_radius = 64.0f;

	//FIXME - Why negative Y
	bezier_curve = clan::BezierCurve();
	bezier_curve.add_control_point(circle_x + circle_radius, -circle_y);
	bezier_curve.add_control_point(circle_x + circle_radius * 1, -(circle_y + circle_radius * 1 * KAPPA));
	bezier_curve.add_control_point(circle_x + circle_radius * 1 * KAPPA, -(circle_y + circle_radius * 1));
	bezier_curve.add_control_point(circle_x + circle_radius * 0, -(circle_y + circle_radius * 1));
	bezier_curve.add_control_point(circle_x - circle_radius * 1  * KAPPA, -(circle_y + circle_radius * 1));
	bezier_curve.add_control_point(circle_x - circle_radius * 1, -(circle_y + circle_radius * 1 * KAPPA));
	bezier_curve.add_control_point(circle_x - circle_radius * 1, -(circle_y + circle_radius * 0));
	bezier_curve.add_control_point(circle_x - circle_radius * 1, -(circle_y - circle_radius * 1  * KAPPA));
	bezier_curve.add_control_point(circle_x - circle_radius * 1 * KAPPA, -(circle_y - circle_radius * 1));
	bezier_curve.add_control_point(circle_x - circle_radius * 0, -(circle_y - circle_radius * 1));
	bezier_curve.add_control_point(circle_x + circle_radius * 1 * KAPPA, -(circle_y - circle_radius * 1));
	bezier_curve.add_control_point(circle_x + circle_radius * 1, -(circle_y - circle_radius * 1 * KAPPA));
	bezier_curve.add_control_point(circle_x + circle_radius * 1, -(circle_y - circle_radius * 0));
	path.add_curve(bezier_curve);
	path_group.add_path(path);

	std::vector<clan::Vec2f> primitives_array_circle;
	std::vector< std::vector<clan::Vec2f> > primitives_array_outline_circle;
	path_group.triangulate_combined(primitives_array_circle, primitives_array_outline_circle);

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
		std::vector< std::vector<clan::Vec2f> >::iterator it;
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


