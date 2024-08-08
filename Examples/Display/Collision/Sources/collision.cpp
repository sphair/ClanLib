/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
#include "collision.h"

clan::ApplicationInstance<CollisionApp> clanapp;

CollisionApp::CollisionApp()
{
#if defined(WIN32) && !defined(__MINGW32__)
	clan::D3DTarget::set_current();
#else
	clan::OpenGLTarget::set_current();
#endif

	// Set the window
	clan::DisplayWindowDescription desc;
	desc.set_title("ClanLib Legacy Collision Source Code Test");
	desc.set_size(clan::Sizef(1020.0f, 700.0f), true);
	desc.set_allow_resize(false);

	window = clan::DisplayWindow(desc);
	canvas = clan::Canvas(window);
	font = clan::Font("Arial", 16);

	other_x_pos = canvas.get_width() / 2;
	other_y_pos = canvas.get_height() / 2;

	//////////////////////////////////////////////////////////////////////////
	std::string file1("Images/triangle.png");
	//std::string file2("Images/triangle.png");
	std::string file2("Images/weird.png");
	//std::string file2("Images/edge_test2.png");
	//std::string file2("Images/inside_test5.png");

	// load outlines
	clan::FileSystem vfs(".");
	co1 = CollisionOutline(file1);
	co2 = CollisionOutline(file2, vfs, 128, accuracy_medium, true);

	// Save now before alignment and positions have been applied
	co1.save("collision_1_test_outline_file.out");
	co2.save("collision_2_test_outline_file.out");

	co1.set_alignment(clan::Origin::center);
	co1.set_rotation_hotspot(clan::Origin::center);
	co1.enable_collision_info(true, true, true, true);
	co1.set_inside_test(true);

	co2.set_alignment(clan::Origin::center);
	co2.set_rotation_hotspot(clan::Origin::center);
	co2.enable_collision_info(true, true, true, true);
	co2.set_inside_test(true);

	////////////////////////////////////////////
	// load resources:

	surface = clan::Sprite(canvas, file1);
	surface2 = clan::Sprite(canvas, file2);

	surface.set_rotation_hotspot(clan::Origin::center);
	surface.set_alignment(clan::Origin::center);

	surface2.set_rotation_hotspot(clan::Origin::center);
	surface2.set_alignment(clan::Origin::center);


	// Connect the Window close event
	sc.connect(window.sig_window_close(), [&](){quit = true; });

	// Connect a keyboard handler to on_key_up()
	sc.connect(window.get_keyboard().sig_key_up(), clan::bind_member(this, &CollisionApp::on_input_up));
}

bool CollisionApp::update()
{
	canvas.clear(clan::Colorf::ghostwhite);

	display_keys();
	control_input();

	// -----------------------------------
	// surfaces
	if (draw_surfaces)
	{
		surface.draw(canvas, (float)tri_x_pos, (float)tri_y_pos);
		surface2.draw(canvas, (float)other_x_pos, (float)other_y_pos);
	}

	// -----------------------------------
	// co1
	co1.set_translation((float)tri_x_pos, (float)tri_y_pos);
	co1.draw(0.0, 0.0, clan::Colorf::limegreen, canvas);
	if (draw_sub_on_co1)
		co1.draw_sub_circles(0.0, 0.0, clan::Colorf::blue, canvas);

	// -----------------------------------
	// co2 
	co2.set_translation((float)other_x_pos, (float)other_y_pos);
	co2.draw(0.0, 0.0, clan::Colorf::red, canvas);
	if (draw_sub_on_co2)
		co2.draw_sub_circles(0.0, 0.0, clan::Colorf::blue, canvas);
	if (draw_min_on_co1)
		canvas.fill_circle(co1.get_minimum_enclosing_disc().position, co1.get_minimum_enclosing_disc().radius, clan::Colorf(0.4f, 0.0f, 0.0f, 0.5f));

	if (draw_min_on_co2)
		canvas.fill_circle(co2.get_minimum_enclosing_disc().position, co2.get_minimum_enclosing_disc().radius, clan::Colorf(0.0f, 0.4f, 0.0f, 0.5f));

	int font_ypos = 20;

	// -----------------------------------
	// collision testing
	if (co2.collide(co1))
	{
		canvas.fill_rect(canvas.get_size(), clan::Colorf(clan::Color(55, 40, 250, 20)));

		const std::vector<CollidingContours> &colpointinfo = co2.get_collision_info();
		for (unsigned int c = 0; c < colpointinfo.size(); c++)
		{
			// Console::write_line(string_format("c1: %1 c2: %2 inside: %3", colpointinfo[c].contour1, colpointinfo[c].contour2, colpointinfo[c].inside));

			for (unsigned int p = 0; p < colpointinfo[c].points.size(); p++)
			{
				const CollisionPoint &collision_point = colpointinfo[c].points[p];
				draw_point_normal(canvas, collision_point.point, collision_point.normal, collision_point.is_entry ? clan::Colorf::green : clan::Colorf::red);

				// Draw information
				std::string output(clan::string_format("Collision(%1). Point Number (%2). ", c, p));
				output = output + clan::string_format("Point(%1,%2). Normal(%3,%4). ", collision_point.point.x, collision_point.point.y, collision_point.normal.x, collision_point.normal.y);
				if (collision_point.is_entry)
				{
					output = output + clan::string_format("Entry(true). ");
				}
				else
				{
					output = output + clan::string_format("Entry(false). ");
				}

				output = output + clan::string_format("Contour1(%1,%2), Contour2(%3,%4).", collision_point.contour1_line_start, collision_point.contour1_line_end, collision_point.contour2_line_start, collision_point.contour2_line_end);

				font.draw_text(canvas, 0, font_ypos, output, clan::Colorf(0.0f, 0.0f, 0.0f, 0.5f));
				font_ypos += 20;
			}
			// Paint the pen-depth and normal from the deepest points
			{
				if (draw_deep_point)
				{
					draw_point_normal(canvas, colpointinfo[c].contour1_deep_point, colpointinfo[c].penetration_normal, clan::Colorf::blue);
					draw_point_normal(canvas, colpointinfo[c].contour2_deep_point, colpointinfo[c].penetration_normal, clan::Colorf::blue);
				}

				// Draw information

				std::string output(clan::string_format("Collision(%1). ", c));
				if (colpointinfo[c].inside)
				{
					output = output + clan::string_format("Inside(true). ");
				}
				else
				{
					output = output + clan::string_format("Inside(false). ");
				}
				output = output + clan::string_format("PenNormal(%1,%2). ", colpointinfo[c].penetration_normal.x, colpointinfo[c].penetration_normal.y);
				output = output + clan::string_format("PenDepth(%1). ", colpointinfo[c].penetration_depth);
				output = output + clan::string_format("DeepPoint1(%1,%2). ", colpointinfo[c].contour1_deep_point.x, colpointinfo[c].contour1_deep_point.y);
				output = output + clan::string_format("DeepPoint2(%1,%2). ", colpointinfo[c].contour2_deep_point.x, colpointinfo[c].contour2_deep_point.y);
				font.draw_text(canvas, 0, font_ypos, output, clan::Colorf(0.0f, 0.0f, 0.0f, 0.5f));
				font_ypos += 20;

			}
		}
	}

	window.flip(1);

	return !quit;
}

// A key was pressed
void CollisionApp::on_input_up(const clan::InputEvent &key)
{
	if(key.id == clan::keycode_escape)
	{
		quit = true;
	}
}

void CollisionApp::draw_point_normal(clan::Canvas &canvas, const clan::Pointf &point, const clan::Pointf &normal, const clan::Colorf &color)
{
	clan::Pointf p1 = point;
	clan::Pointf p2 = p1 + clan::Pointf(normal.x * 20.0f, normal.y * 20.0f);

	canvas.draw_line(
		int(p1.x + 0.5f), int(p1.y + 0.5f),
		int(p2.x + 0.5f), int(p2.y + 0.5f),
		color);
}

void CollisionApp::display_keys()
{
	clan::Colorf colour = clan::Colorf::blue;
	int xpos = canvas.get_width() - 420;
	int ypos = 400;
	int ygap = 16;
	font.draw_text(canvas, xpos, ypos, "----------: Controls :----------", colour); ypos += ygap;
	font.draw_text(canvas, xpos, ypos, "Arrows: move the triangle", colour); ypos += ygap;
	font.draw_text(canvas, xpos, ypos, "Shift+Arrows: move the other outline", colour); ypos += ygap;
	font.draw_text(canvas, xpos, ypos, "'e': rotate triangle", colour); ypos += ygap;
	font.draw_text(canvas, xpos, ypos, "'r': rotate other figure", colour); ypos += ygap;
	font.draw_text(canvas, xpos, ypos, "'i': toggle drawing of sub-circles on triangle", colour); ypos += ygap;
	font.draw_text(canvas, xpos, ypos, "'o': toggle drawing of sub-circles on other figure", colour); ypos += ygap;
	font.draw_text(canvas, xpos, ypos, "'t': toggle drawing of get_minimum_enclosing_disc on triangle", colour); ypos += ygap;
	font.draw_text(canvas, xpos, ypos, "'y': toggle drawing of get_minimum_enclosing_disc on other figure", colour); ypos += ygap;
	font.draw_text(canvas, xpos, ypos, "'s': toggle drawing of surfaces", colour); ypos += ygap;
	font.draw_text(canvas, xpos, ypos, "+/-: (in/de)crease the radius-multiplier", colour); ypos += ygap;
	font.draw_text(canvas, xpos, ypos, "'g': use smallest enclosing disc only", colour); ypos += ygap;
	font.draw_text(canvas, xpos, ypos, "'c': replace contours with convex hull", colour); ypos += ygap;
	font.draw_text(canvas, xpos, ypos, "1-8: scale the middle outline", colour); ypos += ygap;
	font.draw_text(canvas, xpos, ypos, "'x': save then reload outline", colour); ypos += ygap;
	font.draw_text(canvas, xpos, ypos, "'d': toggle drawing of deep point", colour); ypos += ygap;
}

void CollisionApp::control_input()
{
	double movement_rate = 1.0f;	// This should be a delta time since last call

	clan::InputDevice keyboard = window.get_keyboard();
	if (keyboard.get_keycode(clan::keycode_shift))
	{
		// Control Other
		if (keyboard.get_keycode(clan::keycode_right))
			other_x_pos += movement_rate;
		if (keyboard.get_keycode(clan::keycode_left))
			other_x_pos -= movement_rate;

		if (keyboard.get_keycode(clan::keycode_up))
			other_y_pos -= movement_rate;
		if (keyboard.get_keycode(clan::keycode_down))
			other_y_pos += movement_rate;
	}
	else
	{
		// Control Triangle
		if (keyboard.get_keycode(clan::keycode_right))
			tri_x_pos += movement_rate;
		if (keyboard.get_keycode(clan::keycode_left))
			tri_x_pos -= movement_rate;

		if (keyboard.get_keycode(clan::keycode_up))
			tri_y_pos -= movement_rate;
		if (keyboard.get_keycode(clan::keycode_down))
			tri_y_pos += movement_rate;
	}

	if (keyboard.get_keycode(clan::keycode_e))
	{
		surface.rotate(clan::Angle(0.1f, clan::AngleUnit::degrees));
		co1.rotate(clan::Angle(0.1f, clan::AngleUnit::degrees));
	}
	if (keyboard.get_keycode(clan::keycode_r))
	{
		co2.rotate(clan::Angle(0.1f, clan::AngleUnit::degrees));
		surface2.rotate(clan::Angle(0.1f, clan::AngleUnit::degrees));
	}
	if (keyboard.get_keycode(clan::keycode_1))
	{
		co2.set_scale(1.0f, 1.0f);
		surface2.set_scale(1.0f, 1.0f);
		clan::System::sleep(100);
	}
	if (keyboard.get_keycode(clan::keycode_2))
	{
		co2.set_scale(2.0f, 2.0f);
		surface2.set_scale(2.0f, 2.0f);
		clan::System::sleep(100);
	}
	if (keyboard.get_keycode(clan::keycode_3))
	{
		co2.set_scale(3.0f, 3.0f);
		surface2.set_scale(3.0f, 3.0f);
		clan::System::sleep(100);
	}
	if (keyboard.get_keycode(clan::keycode_4))
	{
		co2.set_scale(4.0f, 4.0f);
		surface2.set_scale(4.0f, 4.0f);
		clan::System::sleep(100);
	}
	if (keyboard.get_keycode(clan::keycode_5))
	{
		co2.set_scale(5.0f, 5.0f);
		surface2.set_scale(5.0f, 5.0f);
		clan::System::sleep(100);
	}
	if (keyboard.get_keycode(clan::keycode_6))
	{
		co2.set_scale(6.0f, 6.0f);
		surface2.set_scale(6.0f, 6.0f);
		clan::System::sleep(100);
	}
	if (keyboard.get_keycode(clan::keycode_7))
	{
		co2.set_scale(7.0f, 7.0f);
		surface2.set_scale(7.0f, 7.0f);
		clan::System::sleep(100);
	}
	if (keyboard.get_keycode(clan::keycode_8))
	{
		co2.set_scale(8.0f, 8.0f);
		surface2.set_scale(8.0f, 8.0f);
		clan::System::sleep(100);
	}
	if (keyboard.get_keycode(clan::keycode_i))
	{
		draw_sub_on_co1 = !draw_sub_on_co1;
		clan::System::sleep(100);
	}
	if (keyboard.get_keycode(clan::keycode_o))
	{
		draw_sub_on_co2 = !draw_sub_on_co2;
		clan::System::sleep(100);
	}
	if (keyboard.get_keycode(clan::keycode_t))
	{
		draw_min_on_co1 = !draw_min_on_co1;
		clan::System::sleep(100);
	}
	if (keyboard.get_keycode(clan::keycode_y))
	{
		draw_min_on_co2 = !draw_min_on_co2;
		clan::System::sleep(100);
	}
	if (keyboard.get_keycode(clan::keycode_d))
	{
		draw_deep_point = !draw_deep_point;
		clan::System::sleep(100);
	}
	if (keyboard.get_keycode(clan::keycode_s))
	{
		draw_surfaces = !draw_surfaces;
		clan::System::sleep(100);
	}
	if (keyboard.get_keycode(clan::keycode_x))
	{
		// Load, ensuring recreated
		co1 = CollisionOutline("collision_1_test_outline_file.out");
		co2 = CollisionOutline("collision_2_test_outline_file.out");

		// Reset the options
		co1.set_alignment(clan::Origin::center);
		co1.set_rotation_hotspot(clan::Origin::center);
		co1.enable_collision_info(true, true, true, true);
		co1.set_inside_test(true);
		co2.set_alignment(clan::Origin::center);
		co2.set_rotation_hotspot(clan::Origin::center);
		co2.enable_collision_info(true, true, true, true);
		co2.set_inside_test(true);

		clan::System::sleep(100);
	}

	if (keyboard.get_keycode(clan::keycode_subtract))
	{
		sub_circle_multiplier -= 0.2f;
		co1.calculate_sub_circles(sub_circle_multiplier);
		co2.calculate_sub_circles(sub_circle_multiplier);
		clan::System::sleep(50);
	}
	if (keyboard.get_keycode(clan::keycode_add))
	{
		sub_circle_multiplier += 0.2f;
		co1.calculate_sub_circles(sub_circle_multiplier);
		co2.calculate_sub_circles(sub_circle_multiplier);
		clan::System::sleep(50);
	}
	if (keyboard.get_keycode(clan::keycode_g))
	{
		co1.calculate_smallest_enclosing_discs();
		co2.calculate_smallest_enclosing_discs();
		clan::System::sleep(50);
	}
	if (keyboard.get_keycode(clan::keycode_c))
	{
		co1.calculate_convex_hulls();
		co2.calculate_convex_hulls();
		clan::System::sleep(200);
	}
}
