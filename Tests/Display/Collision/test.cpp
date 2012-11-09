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
**    Harry Storbacka
*/

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
using namespace clan;

void print_usage()
{
	// Explain the keys in the terminal
	Console::write_line( "----------: Controls :----------");
	Console::write_line( "Arrows: move the triangle");
	Console::write_line( "Shift+Arrows: move the other outline");
	Console::write_line( "'e':    rotate triangle");
	Console::write_line( "'r':    rotate other figure");
	Console::write_line( "'i':    toggle drawing of sub-circles on triangle");
	Console::write_line( "'o':    toggle drawing of sub-circles on other figure");
	Console::write_line( "'t':    toggle drawing of get_minimum_enclosing_disc on triangle");
	Console::write_line( "'y':    toggle drawing of get_minimum_enclosing_disc on other figure");
	Console::write_line( "'s':    toggle drawing of surfaces");
	Console::write_line( "+/-:    (in/de)crease the radius-multiplier");
	Console::write_line( "'g':    use smallest enclosing disc only");
	Console::write_line( "'c':    replace contours with convex hull");
	Console::write_line( "'h':    show this help again");
	Console::write_line( "1-8:    scale the middle outline");
	Console::write_line( "'x':    save then reload outline");
	Console::write_line( "'d':    toggle drawing of deep point");
};


class App
{
public:
	int start(const std::vector<std::string> &args);

private:
	void on_input_up(const InputEvent &key);
	void on_window_close();
	void draw_point_normal(Canvas &canvas, const Pointf &point, const Pointf &normal, const Colorf &color);
private:
	bool quit;
};

class Program
{
public:
	static int main(const std::vector<std::string> &args)
	{
		SetupCore setup_core;
		SetupDisplay setup_display;
		SetupGL setup_gl;

		App app;
		return app.start(args);
	}
};

Application app(&Program::main);

int App::start(const std::vector<std::string> &args)
{
	quit = false;

	ConsoleWindow console("Console", 80, 200);

	print_usage();

	try
	{
		DisplayWindowDescription desc;
		desc.set_title("ClanLib Collision Test");
		desc.set_size(Size(950, 700), true);
		Canvas canvas(desc);

		Slot slot_quit = canvas.get_window().sig_window_close().connect(this, &App::on_window_close);
		Slot slot_input_up = canvas.get_window().get_ic().get_keyboard().sig_key_up().connect(this, &App::on_input_up);

		Font font(canvas, "Tahoma", 16);

//////////////////////////////////////////////////////////////////////////
		std::string file1("images/triangle.png");
		//std::string file2("images/triangle.png");
		std::string file2("images/weird.png");
		//std::string file2("images/edge_test2.png");
		//std::string file2("images/inside_test5.png");

		if( args.size() == 3 )
		{
			file1 = args[1];
			file2 = args[2];
		}

		double tri_x_pos = 0;
		double tri_y_pos = 0;
		double other_x_pos = canvas.get_width()/2;
		double other_y_pos = canvas.get_height()/2;

		// draw_limit = 0;

		bool draw_min_on_co1 = false;
		bool draw_min_on_co2 = false;
		bool draw_sub_on_co1 = false;
		bool draw_sub_on_co2 = false;
		bool draw_surfaces = false;
		bool draw_deep_point = false;
		float sub_circle_multiplier = 3.5f;

		////////////////////////////////////////////
		// load resources:

		Sprite surface(canvas, file1);
		Sprite surface2(canvas, file2);

		surface.set_rotation_hotspot(origin_center);
		surface.set_alignment(origin_center);

		surface2.set_rotation_hotspot(origin_center);
		surface2.set_alignment(origin_center);

		////////////////////////////////////////////
		// Collision code

		// load outlines
		VirtualFileSystem vfs(".");
		VirtualDirectory vdir = vfs.get_root_directory();
		CollisionOutline co1(file1);
		CollisionOutline co2(file2, vdir, 128, accuracy_medium, true);

		// Save now before alignment and positions have been applied
		co1.save("collision_1_test_outline_file.out");
		co2.save("collision_2_test_outline_file.out");

		// print some info about the outlines:
		unsigned int size = co1.get_contours().size();

		Console::write_line(string_format("outline 1: %1 contour(s)", (int) co1.get_contours().size()));
		Console::write_line(string_format("outline 2: %1 contour(s)", (int) co2.get_contours().size()));

		std::vector<Contour>::const_iterator it;

		Console::write_line("outline 1:");
		int i=1;
		for( it = co1.get_contours().begin(); it!=co1.get_contours().end(); ++it )
		{
			Console::write_line(string_format("\tcontour %1: %2 points", i, (int) (*it).get_points().size()));
			i++;
		}

		Console::write_line("outline 2:");
		i=1;
		for( it = co2.get_contours().begin(); it!=co2.get_contours().end(); ++it )
		{
			Console::write_line("\tcontour %1: %2 points", i, (int) (*it).get_points().size());
			i++;
		}			

		co1.set_alignment(origin_center);
		co1.set_rotation_hotspot(origin_center);
		co1.enable_collision_info(true,true,true,true);
		co1.set_inside_test(true);

		co2.set_alignment(origin_center);
		co2.set_rotation_hotspot(origin_center);
		co2.enable_collision_info(true,true,true,true);
		co2.set_inside_test(true);

		print_usage();

		InputDevice keyboard = canvas.get_window().get_ic().get_keyboard();

		// Loop until the user hits escape:
		while (!quit)
		{
			canvas.clear(Colorf::ghostwhite);

			if (keyboard.get_keycode(keycode_shift))
			{
				// Control Other
				if( keyboard.get_keycode(keycode_right) )
					other_x_pos+=0.25;
				if( keyboard.get_keycode(keycode_left) )
					other_x_pos-=0.25;

				if( keyboard.get_keycode(keycode_up) )
					other_y_pos-=0.25;
				if( keyboard.get_keycode(keycode_down) )
					other_y_pos+=0.25;
			}
			else
			{
				// Control Triangle
				if( keyboard.get_keycode(keycode_right) )
					tri_x_pos+=0.25;
				if( keyboard.get_keycode(keycode_left) )
					tri_x_pos-=0.25;

				if( keyboard.get_keycode(keycode_up) )
					tri_y_pos-=0.25;
				if( keyboard.get_keycode(keycode_down) )
					tri_y_pos+=0.25;
			}

			if( keyboard.get_keycode(keycode_e) )
			{
				surface.rotate(Angle(0.1f, angle_degrees));
				co1.rotate(Angle(0.1f, angle_degrees));
			}
			if( keyboard.get_keycode(keycode_r) )
			{
				co2.rotate(Angle(0.1f, angle_degrees));
				surface2.rotate(Angle(0.1f, angle_degrees));
			}
			if( keyboard.get_keycode(keycode_1) )
			{
				co2.set_scale(1.0f, 1.0f);
				surface2.set_scale(1.0f, 1.0f);
				System::sleep(100);
			}
			if( keyboard.get_keycode(keycode_2) )
			{
				co2.set_scale(2.0f, 2.0f);
				surface2.set_scale(2.0f, 2.0f);
				System::sleep(100);
			}
			if( keyboard.get_keycode(keycode_3) )
			{
				co2.set_scale(3.0f, 3.0f);
				surface2.set_scale(3.0f, 3.0f);
				System::sleep(100);
			}
			if( keyboard.get_keycode(keycode_4) )
			{
				co2.set_scale(4.0f, 4.0f);
				surface2.set_scale(4.0f, 4.0f);
				System::sleep(100);
			}
			if( keyboard.get_keycode(keycode_5) )
			{
				co2.set_scale(5.0f, 5.0f);
				surface2.set_scale(5.0f, 5.0f);
				System::sleep(100);
			}
			if( keyboard.get_keycode(keycode_6) )
			{
				co2.set_scale(6.0f, 6.0f);
				surface2.set_scale(6.0f, 6.0f);
				System::sleep(100);
			}
			if( keyboard.get_keycode(keycode_7) )
			{
				co2.set_scale(7.0f, 7.0f);
				surface2.set_scale(7.0f, 7.0f);
				System::sleep(100);
			}
			if( keyboard.get_keycode(keycode_8) )
			{
				co2.set_scale(8.0f, 8.0f);
				surface2.set_scale(8.0f, 8.0f);
				System::sleep(100);
			}
			if( keyboard.get_keycode(keycode_i) )
			{
				draw_sub_on_co1 = !draw_sub_on_co1;
				System::sleep(100);
			}
			if( keyboard.get_keycode(keycode_o) )
			{
				draw_sub_on_co2 = !draw_sub_on_co2;
				System::sleep(100);
			}
			if( keyboard.get_keycode(keycode_t) )
			{
				draw_min_on_co1 = !draw_min_on_co1;
				System::sleep(100);
			}
			if( keyboard.get_keycode(keycode_y) )
			{
				draw_min_on_co2 = !draw_min_on_co2;
				System::sleep(100);
			}
			if( keyboard.get_keycode(keycode_d) )
			{
				draw_deep_point = !draw_deep_point;
				System::sleep(100);
			}
			if( keyboard.get_keycode(keycode_s) )
			{
				draw_surfaces = !draw_surfaces;
				System::sleep(100);
			}
			if( keyboard.get_keycode(keycode_x) )
			{
				// Load, ensuring recreated
				co1 = CollisionOutline("collision_1_test_outline_file.out");
				co2 = CollisionOutline("collision_2_test_outline_file.out");
		
				// Reset the options
				co1.set_alignment(origin_center);
				co1.set_rotation_hotspot(origin_center);
				co1.enable_collision_info(true,true,true,true);
				co1.set_inside_test(true);
				co2.set_alignment(origin_center);
				co2.set_rotation_hotspot(origin_center);
				co2.enable_collision_info(true,true,true,true);
				co2.set_inside_test(true);

				System::sleep(100);
			}

			if( keyboard.get_keycode(keycode_subtract) )
			{
				sub_circle_multiplier -= 0.2f;
				co1.calculate_sub_circles(sub_circle_multiplier);
				co2.calculate_sub_circles(sub_circle_multiplier);
				System::sleep(50);
			}
			if( keyboard.get_keycode(keycode_add) )
			{
				sub_circle_multiplier += 0.2f;
				co1.calculate_sub_circles(sub_circle_multiplier);
				co2.calculate_sub_circles(sub_circle_multiplier);
				System::sleep(50);
			}
			if( keyboard.get_keycode(keycode_g) )
			{
				co1.calculate_smallest_enclosing_discs();
				co2.calculate_smallest_enclosing_discs();
				System::sleep(50);
			}
			if( keyboard.get_keycode(keycode_c) )
			{
				co1.calculate_convex_hulls();
				co2.calculate_convex_hulls();
				System::sleep(200);
			}
			if( keyboard.get_keycode(keycode_h) )
			{
				print_usage();
				System::sleep(200);
			}

			// -----------------------------------
			// surfaces
			if(draw_surfaces)
			{
				surface.draw(canvas, (float)tri_x_pos, (float)tri_y_pos);
				surface2.draw(canvas, (float)other_x_pos, (float)other_y_pos);
			}

			// -----------------------------------
			// co1
			co1.set_translation((float)tri_x_pos, (float)tri_y_pos);
			co1.draw(0.0, 0.0, Colorf::limegreen, canvas);
			if(draw_sub_on_co1)
				co1.draw_sub_circles(0.0, 0.0, Colorf::blue, canvas);

			// -----------------------------------
			// co2 
			co2.set_translation((float)other_x_pos, (float)other_y_pos);
			co2.draw(0.0, 0.0, Colorf::red, canvas );
			if(draw_sub_on_co2)
				co2.draw_sub_circles(0.0, 0.0, Colorf::blue, canvas);
			if(draw_min_on_co1)
				canvas.circle(co1.get_minimum_enclosing_disc().position, co1.get_minimum_enclosing_disc().radius, Colorf(0.4f, 0.0f, 0.0f, 0.5f));

			if(draw_min_on_co2)
				canvas.circle(co2.get_minimum_enclosing_disc().position, co2.get_minimum_enclosing_disc().radius, Colorf(0.0f, 0.4f, 0.0f, 0.5f));

			int font_ypos = 20;

			// -----------------------------------
			// collision testing
			if( co2.collide(co1) )
			{
				canvas.fill(canvas.get_size(), Colorf(Color(55,40,250,20)));

				const std::vector<CollidingContours> &colpointinfo = co2.get_collision_info();
				for(unsigned int c = 0; c < colpointinfo.size(); c++)
				{
					// Console::write_line(string_format("c1: %1 c2: %2 inside: %3", colpointinfo[c].contour1, colpointinfo[c].contour2, colpointinfo[c].inside));

					for(unsigned int p = 0; p < colpointinfo[c].points.size(); p++)
					{
						const CollisionPoint &collision_point = colpointinfo[c].points[p];
						draw_point_normal(canvas, collision_point.point, collision_point.normal, collision_point.is_entry ? Colorf::green : Colorf::red);
	
						// Draw information
						std::string output(string_format("Collision(%1). Point Number (%2). ", c, p));
						output = output + string_format("Point(%1,%2). Normal(%3,%4). ", collision_point.point.x, collision_point.point.y, collision_point.normal.x, collision_point.normal.y);
						if (collision_point.is_entry)
						{
							output = output + string_format("Entry(true). ");
						}
						else
						{
							output = output + string_format("Entry(false). ");
						}

						output = output + string_format("Contour1(%1,%2), Contour2(%3,%4).", collision_point.contour1_line_start, collision_point.contour1_line_end, collision_point.contour2_line_start, collision_point.contour2_line_end);

						font.draw_text(canvas, 0, font_ypos, output, Colorf(0.0f, 0.0f, 0.0f, 0.5f));
						font_ypos += 20;
					}
					// Paint the pen-depth and normal from the deepest points
					{
						if (draw_deep_point)
						{
							draw_point_normal(canvas, colpointinfo[c].contour1_deep_point, colpointinfo[c].penetration_normal, Colorf::blue);
							draw_point_normal(canvas, colpointinfo[c].contour2_deep_point, colpointinfo[c].penetration_normal, Colorf::blue);
						}

						// Draw information

						std::string output(string_format("Collision(%1). ", c));
						if (colpointinfo[c].inside)
						{
							output = output + string_format("Inside(true). ");
						}
						else
						{
							output = output + string_format("Inside(false). ");
						}
						output = output + string_format("PenNormal(%1,%2). ", colpointinfo[c].penetration_normal.x, colpointinfo[c].penetration_normal.y);
						output = output + string_format("PenDepth(%1). ", colpointinfo[c].penetration_depth);
						output = output + string_format("DeepPoint1(%1,%2). ", colpointinfo[c].contour1_deep_point.x, colpointinfo[c].contour1_deep_point.y);
						output = output + string_format("DeepPoint2(%1,%2). ", colpointinfo[c].contour2_deep_point.x, colpointinfo[c].contour2_deep_point.y);
						font.draw_text(canvas, 0, font_ypos, output, Colorf(0.0f, 0.0f, 0.0f, 0.5f));
						font_ypos += 20;

					}
				}
			}

			// Update keyboard input and handle system events:
			canvas.flip(1);
			KeepAlive::process();
		}


//////////////////////////////////////////////////////////////////////////

	}
	catch(Exception& exception)
	{
		Console::write_line("Exception caught:");
		Console::write_line(exception.message);

		// Display the stack trace (if available)
		std::vector<std::string> stacktrace = exception.get_stack_trace();
		int size = stacktrace.size();
		if (size > 0)
		{
			Console::write_line("Stack Trace:");
			for (int cnt=0; cnt < size; cnt++)
			{
				Console::write_line(stacktrace[cnt]);
			}
		}

		console.display_close_message();

		return -1;
	}
	return 0;
}

void App::on_input_up(const InputEvent &key)
{
	if(key.id == keycode_escape)
	{
		quit = true;
	}
}

void App::on_window_close()
{
	quit = true;
}

void App::draw_point_normal(Canvas &canvas, const Pointf &point, const Pointf &normal, const Colorf &color)
{
	Pointf p1 = point;
	Pointf p2 = p1 + Pointf(normal.x * 20.0f, normal.y * 20.0f);
	
	canvas.line(
			int(p1.x+0.5f), int(p1.y+0.5f),
			int(p2.x+0.5f), int(p2.y+0.5f),
			color);
}
