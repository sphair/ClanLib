/*
	TextStyler Example
*/

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class TextStylerApp : public CL_ClanApplication
{
public:
	int main(int, char **)
	{
		try
		{
			CL_SetupCore::init();
			CL_SetupDisplay::init();
			CL_SetupGL::init();
			
			CL_ResourceManager resources("ts.xml", false);
            		
			CL_DisplayWindow window("ClanLib TextStyler Example", 640, 480);
			CL_TextStyler ts("ts", &resources);
			
			string str;
			str += "The rain in Spain} falls mainly on the plains. Also, all your base are belong to us! Who set up us the bomb? ";
			str += "{{{red}}Foo Bar Narf Bork and many other pastries.{/red} And also, {{narf! ";
			str += "[s 2.0]This text should be doublesize.[s 2.0]This text quadsize.[/s][/s]This text normal. ";
			str += "[s 1.0 2.0]This text doubletall.[/s][s 2.0 1.0]This text doublewide.[/s]This text normal.\n\n";
			str += "Left aligned.\n";
			str += "[j right]Right aligned.\n";
			str += "Still right aligned.\n";
			str += "Center aligne[j center]d.\n";
			str += "[/j]Right aligned.\n";
			str += "[/j]Left aligned.\n";
			
			CL_Rect drect(25, 25, 600, 440);
			CL_GlyphBuffer gb;
			ts.draw_to_gb(str, gb, drect.get_size());
			
			unsigned int start_time = CL_System::get_time();
			int fcount = 0;
			while(!CL_Keyboard::get_keycode(CL_KEY_ESCAPE))
			{
				++fcount;
				
				CL_Display::clear(CL_Color::white);
				float sx, sy;
				ts.get_scale(sx, sy);
				
				if (CL_Keyboard::get_keycode(CL_KEY_W))
				{
					gb.clear();
					ts.set_scale(sx + 0.01, sy + 0.01);
					ts.draw_to_gb(str, gb, drect.get_size());
				}
				
				if (CL_Keyboard::get_keycode(CL_KEY_S))
				{
					gb.clear();
					ts.set_scale(sx - 0.01, sy - 0.01);
					ts.draw_to_gb(str, gb, drect.get_size());
				}
				
				CL_Display::draw_rect(drect, CL_Color::blue);
				CL_Display::draw_rect(gb.bounding_rect(drect), CL_Color::red);
				gb.draw(drect);
				
				unsigned int elapsed_time = CL_System::get_time() - start_time;
				if (elapsed_time >= 1000)
				{
					cerr << "FPS: " << fcount << "\n";
					fcount = 0;
					start_time = CL_System::get_time();
				}
				
				CL_Display::flip();
				CL_System::keep_alive();
			}
			
			CL_SetupGL::deinit();
			CL_SetupDisplay::deinit();
			CL_SetupCore::deinit();
		}
		catch (CL_Error err)
		{
			std::cout << "Exception caught: " << err.message.c_str() << std::endl;
			return -1;
		}
		
		return 0;
	}
} app;
