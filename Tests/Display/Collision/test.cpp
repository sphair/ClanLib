#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

void print_usage()
{
	// Explain the keys in the terminal
	CL_Console::write_line( "----------: Controls :----------");
	CL_Console::write_line( "Arrows: move the triangle");
	CL_Console::write_line( "Shift+Arrows: move the other outline");
	CL_Console::write_line( "'e':    rotate triangle");
	CL_Console::write_line( "'r':    rotate other figure");
	CL_Console::write_line( "'i':    toggle drawing of sub-circles on triangle");
	CL_Console::write_line( "'o':    toggle drawing of sub-circles on other figure");
	CL_Console::write_line( "'s':    toggle drawing of surfaces");
	CL_Console::write_line( "+/-:    (in/de)crease the radius-multiplier");
	CL_Console::write_line( "'g':    use smallest enclosing disc only");
	CL_Console::write_line( "'c':    replace contours with convex hull");
	CL_Console::write_line( "'h':    show this help again");
	CL_Console::write_line( "1-8:    scale the middle outline");
};


class App
{
public:
	int start(const std::vector<CL_String> &args);

private:
	void on_input_up(const CL_InputEvent &key, const CL_InputState &state);
	void on_window_close();

private:
	bool quit;
};

class Program
{
public:
	static int main(const std::vector<CL_String> &args)
	{
		CL_SetupCore setup_core;
		CL_SetupDisplay setup_display;
		CL_SetupGL setup_gl;

		App app;
		return app.start(args);
	}
};

CL_ClanApplication app(&Program::main);

int App::start(const std::vector<CL_String> &args)
{
	quit = false;

	CL_ConsoleWindow console("Console", 80, 200);

	print_usage();

	try
	{
		CL_DisplayWindowDescription desc;
		desc.set_title(cl_text("ClanLib Collision Test"));
		desc.set_size(CL_Size(800, 600), true);
		CL_DisplayWindow window(desc);

		CL_Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close);
		CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

		CL_GraphicContext gc = window.get_gc();

//////////////////////////////////////////////////////////////////////////
		CL_String file1("images/triangle.png");
		CL_String file2("images/weird.png");
		//CL_String file2("images/edge_test2.png");
		//CL_String file2("images/inside_test5.png");

		if( args.size() == 3 )
		{
			file1 = args[1];
			file2 = args[2];
		}

		int tri_x_pos = 0;
		int tri_y_pos = 0;
		int other_x_pos = window.get_geometry().get_width()/2;
		int other_y_pos = window.get_geometry().get_height()/2;

		// draw_limit = 0;

		bool draw_sub_on_co1 = true;
		bool draw_sub_on_co2 = true;
		bool draw_surfaces = false;
		float sub_circle_multiplier = 3.5f;

		////////////////////////////////////////////
		// load resources:

		CL_Sprite surface(gc, file1);
		CL_Sprite surface2(gc, file2);

		surface.set_rotation_hotspot(origin_center);
		surface.set_alignment(origin_center);

		surface2.set_rotation_hotspot(origin_center);
		surface2.set_alignment(origin_center);

		////////////////////////////////////////////
		// Collision code

		// load outlines
		CL_VirtualFileSystem vfs(".");
		CL_VirtualDirectory vdir = vfs.get_root_directory();
		CL_CollisionOutline co1(file1);
		CL_CollisionOutline co2(file2, vdir, 128, accuracy_medium, true);

		// print some info about the outlines:
		unsigned int size = co1.get_contours().size();

		CL_Console::write_line(cl_format("outline 1: %1 contour(s)", (int) co1.get_contours().size()));
		CL_Console::write_line(cl_format("outline 2: %1 contour(s)", (int) co2.get_contours().size()));

		std::vector<CL_Contour>::const_iterator it;

		CL_Console::write_line("outline 1:");
		int i=1;
		for( it = co1.get_contours().begin(); it!=co1.get_contours().end(); ++it )
		{
			CL_Console::write_line(cl_format("\tcontour %1: %2 points", i, (int) (*it).get_points().size()));
			i++;
		}

		CL_Console::write_line("outline 2:");
		i=1;
		for( it = co2.get_contours().begin(); it!=co2.get_contours().end(); ++it )
		{
			CL_Console::write_line("\tcontour %1: %2 points", i, (int) (*it).get_points().size());
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

		CL_InputDevice keyboard = window.get_ic().get_keyboard();

		// Loop until the user hits escape:
		while (!quit)
		{
			gc.clear(CL_Colorf::ghostwhite);

			if (keyboard.get_keycode(CL_KEY_SHIFT))
			{
				// Control Other
				if( keyboard.get_keycode(CL_KEY_RIGHT) )
					other_x_pos+=1;
				if( keyboard.get_keycode(CL_KEY_LEFT) )
					other_x_pos-=1;

				if( keyboard.get_keycode(CL_KEY_UP) )
					other_y_pos-=1;
				if( keyboard.get_keycode(CL_KEY_DOWN) )
					other_y_pos+=1;
			}
			else
			{
				// Control Triangle
				if( keyboard.get_keycode(CL_KEY_RIGHT) )
					tri_x_pos+=1;
				if( keyboard.get_keycode(CL_KEY_LEFT) )
					tri_x_pos-=1;

				if( keyboard.get_keycode(CL_KEY_UP) )
					tri_y_pos-=1;
				if( keyboard.get_keycode(CL_KEY_DOWN) )
					tri_y_pos+=1;
			}

			if( keyboard.get_keycode(CL_KEY_E) )
			{
				surface.rotate(CL_Angle(0.1f, cl_degrees));
				co1.rotate(CL_Angle(0.1f, cl_degrees));
			}
			if( keyboard.get_keycode(CL_KEY_R) )
			{
				co2.rotate(CL_Angle(0.1f, cl_degrees));
				surface2.rotate(CL_Angle(0.1f, cl_degrees));
			}
			if( keyboard.get_keycode(CL_KEY_1) )
			{
				co2.set_scale(1.0f, 1.0f);
				surface2.set_scale(1.0f, 1.0f);
				CL_System::sleep(100);
			}
			if( keyboard.get_keycode(CL_KEY_2) )
			{
				co2.set_scale(2.0f, 2.0f);
				surface2.set_scale(2.0f, 2.0f);
				CL_System::sleep(100);
			}
			if( keyboard.get_keycode(CL_KEY_3) )
			{
				co2.set_scale(3.0f, 3.0f);
				surface2.set_scale(3.0f, 3.0f);
				CL_System::sleep(100);
			}
			if( keyboard.get_keycode(CL_KEY_4) )
			{
				co2.set_scale(4.0f, 4.0f);
				surface2.set_scale(4.0f, 4.0f);
				CL_System::sleep(100);
			}
			if( keyboard.get_keycode(CL_KEY_5) )
			{
				co2.set_scale(5.0f, 5.0f);
				surface2.set_scale(5.0f, 5.0f);
				CL_System::sleep(100);
			}
			if( keyboard.get_keycode(CL_KEY_6) )
			{
				co2.set_scale(6.0f, 6.0f);
				surface2.set_scale(6.0f, 6.0f);
				CL_System::sleep(100);
			}
			if( keyboard.get_keycode(CL_KEY_7) )
			{
				co2.set_scale(7.0f, 7.0f);
				surface2.set_scale(7.0f, 7.0f);
				CL_System::sleep(100);
			}
			if( keyboard.get_keycode(CL_KEY_8) )
			{
				co2.set_scale(8.0f, 8.0f);
				surface2.set_scale(8.0f, 8.0f);
				CL_System::sleep(100);
			}
			if( keyboard.get_keycode(CL_KEY_I) )
			{
				draw_sub_on_co1 = !draw_sub_on_co1;
				CL_System::sleep(100);
			}
			if( keyboard.get_keycode(CL_KEY_O) )
			{
				draw_sub_on_co2 = !draw_sub_on_co2;
				CL_System::sleep(100);
			}
			if( keyboard.get_keycode(CL_KEY_S) )
			{
				draw_surfaces = !draw_surfaces;
				CL_System::sleep(100);
			}
			if( keyboard.get_keycode(CL_KEY_SUBTRACT) )
			{
				sub_circle_multiplier -= 0.2f;
				co1.calculate_sub_circles(sub_circle_multiplier);
				co2.calculate_sub_circles(sub_circle_multiplier);
				CL_System::sleep(50);
			}
			if( keyboard.get_keycode(CL_KEY_ADD) )
			{
				sub_circle_multiplier += 0.2f;
				co1.calculate_sub_circles(sub_circle_multiplier);
				co2.calculate_sub_circles(sub_circle_multiplier);
				CL_System::sleep(50);
			}
			if( keyboard.get_keycode(CL_KEY_G) )
			{
				co1.calculate_smallest_enclosing_discs();
				co2.calculate_smallest_enclosing_discs();
				CL_System::sleep(50);
			}
			if( keyboard.get_keycode(CL_KEY_C) )
			{
				co1.calculate_convex_hulls();
				co2.calculate_convex_hulls();
				CL_System::sleep(200);
			}
			if( keyboard.get_keycode(CL_KEY_H) )
			{
				print_usage();
				CL_System::sleep(200);
			}

			// -----------------------------------
			// surfaces
			if(draw_surfaces)
			{
				surface.draw(gc, (float)tri_x_pos, (float)tri_y_pos);
				surface2.draw(gc, (float)other_x_pos, (float)other_y_pos);
			}

			// -----------------------------------
			// co1
			co1.set_translation((float)tri_x_pos, (float)tri_y_pos);
			co1.draw(0.0, 0.0, CL_Colorf::limegreen, gc);
			if(draw_sub_on_co1)
				co1.draw_sub_circles(0.0, 0.0, CL_Colorf::blue, gc);

			// -----------------------------------
			// co2 
			co2.set_translation((float)other_x_pos, (float)other_y_pos);
			co2.draw(0.0, 0.0, CL_Colorf::red, gc );
			if(draw_sub_on_co2)
				co2.draw_sub_circles(0.0, 0.0, CL_Colorf::blue, gc);


			// -----------------------------------
			// collision testing
			if( co2.collide(co1) )
			{
				CL_Draw::fill(gc, CL_Rect(0,0,800,600), CL_Colorf(CL_Color(55,40,250,20)));
				const std::vector<CL_CollidingContours> &colpointinfo = co2.get_collision_info();
				for(unsigned int c = 0; c < colpointinfo.size(); c++)
				{
					// CL_Console::write_line(cl_format("c1: %1 c2: %2 inside: %3", colpointinfo[c].contour1, colpointinfo[c].contour2, colpointinfo[c].inside));

					for(unsigned int p = 0; p < colpointinfo[c].points.size(); p++)
					{
						CL_Pointf p1 = colpointinfo[c].points[p].point;
						CL_Pointf p2 = p1 + CL_Pointf(colpointinfo[c].points[p].normal.x * 20.0f, colpointinfo[c].points[p].normal.y * 20.0f);
						CL_Draw::line(gc, 
							int(p1.x+0.5f), int(p1.y+0.5f),
							int(p2.x+0.5f), int(p2.y+0.5f),
							(colpointinfo[c].points[p].is_entry ? CL_Colorf::green : CL_Colorf::red));
						// CL_Console::write_line(cl_format("p1: (%1,%2), ", p1.x, p1.y, colpointinfo[c].points[p].is_entry ? "entry" : "exit"));
					}
					// Paint the pen-depth and normal from the deepest points
					{
						CL_Pointf p1 = colpointinfo[c].contour1_deep_point;
						CL_Pointf p2 = p1 + CL_Pointf(colpointinfo[c].penetration_normal.x * colpointinfo[c].penetration_depth, colpointinfo[c].penetration_normal.y * colpointinfo[c].penetration_depth);
						CL_Draw::line(gc, int(p1.x+0.5f), int(p1.y+0.5f),int(p2.x+0.5f), int(p2.y+0.5f),CL_Colorf::blue);

						p1 = colpointinfo[c].contour2_deep_point;
						p2 = p1 + CL_Pointf(colpointinfo[c].penetration_normal.x * colpointinfo[c].penetration_depth, colpointinfo[c].penetration_normal.y * colpointinfo[c].penetration_depth);
						CL_Draw::line(gc, int(p1.x+0.5f), int(p1.y+0.5f),int(p2.x+0.5f), int(p2.y+0.5f),CL_Colorf::blue);
					}
				}
			}

			// Update keyboard input and handle system events:
			window.flip(1);
			CL_DisplayMessageQueue::process();
		}


//////////////////////////////////////////////////////////////////////////

	}
	catch(CL_Exception& exception)
	{
		CL_Console::write_line("Exception caught:");
		CL_Console::write_line(exception.message);

		// Display the stack trace (if available)
		std::vector<CL_String> stacktrace = exception.get_stack_trace();
		int size = stacktrace.size();
		if (size > 0)
		{
			CL_Console::write_line("Stack Trace:");
			for (int cnt=0; cnt < size; cnt++)
			{
				CL_Console::write_line(stacktrace[cnt]);
			}
		}

		console.display_close_message();

		return -1;
	}
	return 0;
}

void App::on_input_up(const CL_InputEvent &key, const CL_InputState &state)
{
	if(key.id == CL_KEY_ESCAPE)
	{
		quit = true;
	}
}

void App::on_window_close()
{
	quit = true;
}
