
#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>


void print_usage()
{
	// Explain the keys in the terminal
	std::cout << "----------: Controls :----------" << std::endl;
	std::cout << "Arrows: move the triangle" << std::endl;
	std::cout << "Shift+Arrows: move the other outline" << std::endl;
	std::cout << "'e':    rotate triangle" << std::endl;
	std::cout << "'r':    rotate other figure" << std::endl;
	std::cout << "'i':    toggle drawing of sub-circles on triangle" << std::endl;
	std::cout << "'o':    toggle drawing of sub-circles on other figure" << std::endl;
	std::cout << "'s':    toggle drawing of surfaces" << std::endl;
	std::cout << "+/-:    (in/de)crease the radius-multiplier" << std::endl;
	std::cout << "'g':    use smallest enclosing disc only" << std::endl;
	std::cout << "'c':    replace contours with convex hull" << std::endl;
	std::cout << "'h':    show this help again" << std::endl;
	std::cout << "1-8:    scale the middle outline" << std::endl;
};

class SurfaceApp : public CL_ClanApplication
{
public:
	virtual int main(int argc, char **argv)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();

		try
		{
			std::string file1("images/triangle.png");
			//std::string file2("images/weird.png");
			//std::string file2("images/edge_test2.png");
			std::string file2("images/inside_test5.png");

			if( argc == 3 )
			{
				file1 = argv[1];
				file2 = argv[2];
			}

			CL_SetupCore c_init;
			CL_SetupDisplay d_init;
			CL_SetupGL g_init;
			
			CL_DisplayWindow window("Surface outline", 800, 600);
			
			int tri_x_pos = 0;
			int tri_y_pos = 0;
			int other_x_pos = CL_Display::get_width()/2;
			int other_y_pos = CL_Display::get_height()/2;
			
			draw_limit = 0;

			bool draw_sub_on_co1 = true;
			bool draw_sub_on_co2 = true;
			bool draw_surfaces = false;
			float sub_circle_multiplier = 3.5f;

			////////////////////////////////////////////
			// load resources:

			CL_Surface surface(file1);
			CL_Surface surface2(file2);

			surface.set_rotation_hotspot(origin_center);
			surface.set_alignment(origin_center);

			surface2.set_rotation_hotspot(origin_center);
			surface2.set_alignment(origin_center);
			
			////////////////////////////////////////////
			// Collision code
			
			// load outlines
			CL_CollisionOutline co1(file1);
			CL_CollisionOutline co2(file2, 128, accuracy_raw, true);

			// print some info about the outlines:
			unsigned int size = co1.get_contours().size();

			std::cout << "outline 1: " << co1.get_contours().size() <<  " contour"<<(co1.get_contours().size()==1 ? "" : "s")<<"\n";
			std::cout << "outline 2: " << co2.get_contours().size() <<  " contour"<<(co2.get_contours().size()==1 ? "" : "s")<<"\n";

			std::vector<CL_Contour>::iterator it;

			std::cout << "outline 1: " << std::endl;
			int i=1;
			for( it = co1.get_contours().begin(); it!=co1.get_contours().end(); ++it )
			{
				std::cout << "\tcontour " << i << ": " << (*it).points.size() << " points\n";
				i++;
			}

			std::cout << "outline 2: " << std::endl;
			i=1;
			for( it = co2.get_contours().begin(); it!=co2.get_contours().end(); ++it )
			{
				std::cout << "\tcontour " << i << ": " << (*it).points.size() << " points\n";
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
			
			// Loop until the user hits escape:
			while (CL_Keyboard::get_keycode(CL_KEY_ESCAPE) == false)
			{
				CL_Display::clear(CL_Color::ghostwhite);

				if( CL_Keyboard::get_keycode(CL_KEY_SHIFT) )
				{
					// Control Other
					if( CL_Keyboard::get_keycode(CL_KEY_RIGHT) )
						other_x_pos+=1;
					if( CL_Keyboard::get_keycode(CL_KEY_LEFT) )
						other_x_pos-=1;
	
					if( CL_Keyboard::get_keycode(CL_KEY_UP) )
						other_y_pos-=1;
					if( CL_Keyboard::get_keycode(CL_KEY_DOWN) )
						other_y_pos+=1;
				}
				else
				{
					// Control Triangle
					if( CL_Keyboard::get_keycode(CL_KEY_RIGHT) )
						tri_x_pos+=1;
					if( CL_Keyboard::get_keycode(CL_KEY_LEFT) )
						tri_x_pos-=1;
	
					if( CL_Keyboard::get_keycode(CL_KEY_UP) )
						tri_y_pos-=1;
					if( CL_Keyboard::get_keycode(CL_KEY_DOWN) )
						tri_y_pos+=1;
				}
					
				if( CL_Keyboard::get_keycode(CL_KEY_E) )
				{
					surface.rotate(0.1f);
					co1.rotate(0.1f);
				}
				if( CL_Keyboard::get_keycode(CL_KEY_R) )
				{
					co2.rotate(0.1f);
					surface2.rotate(0.1f);
				}
				if( CL_Keyboard::get_keycode(CL_KEY_1) )
				{
					co2.set_scale(1.0f, 1.0f);
					surface2.set_scale(1.0f, 1.0f);
					CL_System::sleep(100);
				}
				if( CL_Keyboard::get_keycode(CL_KEY_2) )
				{
					co2.set_scale(2.0f, 2.0f);
					surface2.set_scale(2.0f, 2.0f);
					CL_System::sleep(100);
				}
				if( CL_Keyboard::get_keycode(CL_KEY_3) )
				{
					co2.set_scale(3.0f, 3.0f);
					surface2.set_scale(3.0f, 3.0f);
					CL_System::sleep(100);
				}
				if( CL_Keyboard::get_keycode(CL_KEY_4) )
				{
					co2.set_scale(4.0f, 4.0f);
					surface2.set_scale(4.0f, 4.0f);
					CL_System::sleep(100);
				}
				if( CL_Keyboard::get_keycode(CL_KEY_5) )
				{
					co2.set_scale(5.0f, 5.0f);
					surface2.set_scale(5.0f, 5.0f);
					CL_System::sleep(100);
				}
				if( CL_Keyboard::get_keycode(CL_KEY_6) )
				{
					co2.set_scale(6.0f, 6.0f);
					surface2.set_scale(6.0f, 6.0f);
					CL_System::sleep(100);
				}
				if( CL_Keyboard::get_keycode(CL_KEY_7) )
				{
					co2.set_scale(7.0f, 7.0f);
					surface2.set_scale(7.0f, 7.0f);
					CL_System::sleep(100);
				}
				if( CL_Keyboard::get_keycode(CL_KEY_8) )
				{
					co2.set_scale(8.0f, 8.0f);
					surface2.set_scale(8.0f, 8.0f);
					CL_System::sleep(100);
				}
				if( CL_Keyboard::get_keycode(CL_KEY_I) )
				{
					draw_sub_on_co1 = !draw_sub_on_co1;
					CL_System::sleep(100);
				}
				if( CL_Keyboard::get_keycode(CL_KEY_O) )
				{
					draw_sub_on_co2 = !draw_sub_on_co2;
					CL_System::sleep(100);
				}
				if( CL_Keyboard::get_keycode(CL_KEY_S) )
				{
					draw_surfaces = !draw_surfaces;
					CL_System::sleep(100);
				}
				if( CL_Keyboard::get_keycode(CL_KEY_SUBTRACT) )
				{
					sub_circle_multiplier -= 0.2f;
					co1.calculate_sub_circles(sub_circle_multiplier);
					co2.calculate_sub_circles(sub_circle_multiplier);
					CL_System::sleep(50);
				}
				if( CL_Keyboard::get_keycode(CL_KEY_ADD) )
				{
					sub_circle_multiplier += 0.2f;
					co1.calculate_sub_circles(sub_circle_multiplier);
					co2.calculate_sub_circles(sub_circle_multiplier);
					CL_System::sleep(50);
				}
				if( CL_Keyboard::get_keycode(CL_KEY_G) )
				{
					co1.calculate_smallest_enclosing_discs();
					co2.calculate_smallest_enclosing_discs();
					CL_System::sleep(50);
				}
				if( CL_Keyboard::get_keycode(CL_KEY_C) )
				{
					co1.calculate_convex_hulls();
					co2.calculate_convex_hulls();
					CL_System::sleep(200);
				}
				if( CL_Keyboard::get_keycode(CL_KEY_H) )
				{
					print_usage();
					CL_System::sleep(200);
				}

				// -----------------------------------
				// surfaces
				if(draw_surfaces)
				{
					surface.draw(tri_x_pos,tri_y_pos);
					surface2.draw(other_x_pos, other_y_pos);
				}

				// -----------------------------------
				// co1
				co1.set_translation((float)tri_x_pos, (float)tri_y_pos);
				co1.draw(0.0,0.0,CL_Color::limegreen);
				if(draw_sub_on_co1)
					co1.draw_sub_circles(0.0, 0.0, CL_Color::blue);

				// -----------------------------------
				// co2 
				co2.set_translation((float)other_x_pos, (float)other_y_pos);
				co2.draw(0.0, 0.0, CL_Color::red );
				if(draw_sub_on_co2)
					co2.draw_sub_circles(0.0, 0.0, CL_Color::blue );
				

				// -----------------------------------
				// collision testing
				if( co2.collide(co1) )
				{
					CL_Display::fill_rect(CL_Rect(0,0,800,600), CL_Color(55,40,250,20));
					const std::vector<CL_CollidingContours> &colpointinfo = co2.get_collision_info();
					for(int c = 0; c < colpointinfo.size(); c++)
					{
						std::cout << "c1:("<<colpointinfo[c].contour1<<",c2:"<<colpointinfo[c].contour2<<") (inside:"<<colpointinfo[c].inside<<")\n";
						for(int p = 0; p < colpointinfo[c].points.size(); p++)
						{
							CL_Pointf p1 = colpointinfo[c].points[p].point;
							CL_Pointf p2 = p1 + CL_Pointf(colpointinfo[c].points[p].normal.x * 20.0f, colpointinfo[c].points[p].normal.y * 20.0f);
							CL_Display::draw_line(
								int(p1.x+0.5f), int(p1.y+0.5f),
								int(p2.x+0.5f), int(p2.y+0.5f),
								(colpointinfo[c].points[p].is_entry ? CL_Color::green : CL_Color::red));
							std::cout << "p1:("<<p1.x<<","<<p1.y<<") " << (colpointinfo[c].points[p].is_entry ? "entry" : "exit") << "\n";
						}
						// Paint the pen-depth and normal from the deepest points
						{
							CL_Pointf p1 = colpointinfo[c].contour1_deep_point;
							CL_Pointf p2 = p1 + CL_Pointf(colpointinfo[c].penetration_normal.x * colpointinfo[c].penetration_depth, colpointinfo[c].penetration_normal.y * colpointinfo[c].penetration_depth);
							CL_Display::draw_line(int(p1.x+0.5f), int(p1.y+0.5f),int(p2.x+0.5f), int(p2.y+0.5f),CL_Color::blue);

							p1 = colpointinfo[c].contour2_deep_point;
							p2 = p1 + CL_Pointf(colpointinfo[c].penetration_normal.x * colpointinfo[c].penetration_depth, colpointinfo[c].penetration_normal.y * colpointinfo[c].penetration_depth);
							CL_Display::draw_line(int(p1.x+0.5f), int(p1.y+0.5f),int(p2.x+0.5f), int(p2.y+0.5f),CL_Color::blue);
						}
					}
				}

				// Flip front and backbuffer. This makes the changes visible:
				CL_Display::flip();

				// Update keyboard input and handle system events:
				CL_System::keep_alive(10);
			}
		}
		catch (CL_Error err)
		{
			std::cout << "Error: " << err.message.c_str() << std::endl;

			// Display console close message and wait for a key
			console.display_close_message();
		}

		return 0;
	}

	unsigned int draw_limit;

} app;
