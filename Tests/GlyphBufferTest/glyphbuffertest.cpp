#include "glyphbuffertest.h"

#include <string>
#include <iostream>
#include <cmath>

using namespace std;

int GlyphBufferTestApp::main(int argc, char** argv)
{
	try
	{
		CL_SetupCore::init();
		CL_SetupDisplay::init();
		CL_SetupGL::init();
		
		CL_ResourceManager resources("data.scr", false);
		CL_DisplayWindow window("ClanLib Glyph Buffer Test", 640, 480);
		CL_Font font("Font", &resources);
		
		CL_Rect drect(55, 10, 505, 450);
		
		unsigned int start_time = CL_System::get_time();
		
		//Character indexes:      8 10           22      30           42    48
		string message = "Color : R G B\nScale : 0.5 1.0 1.5\nAngle : 15 30 45\n";
		message += "Hit escape to exit. WASD to rotate/scale. Space to reset. ";
		message += "This text should be centered in the blue rectangle. The red rectangle is the unrotated, unscaled bounding rect. ";
		message += "Here is the greatest joke in the world: One day, a pirate walked into a bar. ";
		message += "The pirate had a steering wheel strapped to his crotch. The barman asked, \"Why ";
		message += "do you have a steering wheel strapped to your crotch?\". The pirate responded, ";
		message += "\"Arrrr! It's drivin' me nuts!\"";
		
		CL_GlyphBuffer gb;
		gb.set_alignment(origin_center);
		font.set_alignment(origin_center);
		font.draw_to_gb(message, gb, drect.get_size());
		
		//Assign the effects (note that spaces, newlines, and other characters for which glyphs do not exist, don't go into the glyph buffer)
		map<int, CL_Color>& c = gb.get_color_effects();
		c[8] = CL_Color::red;
		c[10] = CL_Color::blue;
		c[12] = CL_Color::green;
		
		map<int, float>& sx = gb.get_scale_x_effects();
		map<int, float>& sy = gb.get_scale_y_effects();
		sx[22] = sx[23] = sx[24] = sy[22] = sy[23] = sy[24] = 0.5;
		sx[30] = sx[31] = sx[32] = sy[30] = sy[31] = sy[32] = 1.5;
		
		map<int, float>& a = gb.get_angle_effects();
		a[42] = a[43] = 15;
		a[45] = a[46] = 30;
		a[48] = a[49] = 45;
		
		int elapsed_time = CL_System::get_time() - start_time;
		cout << "Time elapsed filling GlyphBuffer is " << elapsed_time << " milliseconds.\n";
		cout << "Glyph Buffer glyphs size is " << gb.get_glyphs().size() << "\n";
		
		while (!CL_Keyboard::get_keycode(CL_KEY_ESCAPE))
		{
			CL_Display::clear(CL_Color::white);
			
			CL_Display::draw_rect(drect, CL_Color::blue);
			CL_Display::draw_rect(gb.bounding_rect(drect), CL_Color::red);
			gb.draw(drect);
			
			if (CL_Keyboard::get_keycode(CL_KEY_A))
				gb.rotate(-1);
			
			if (CL_Keyboard::get_keycode(CL_KEY_D))
				gb.rotate(1);
			
			if (CL_Keyboard::get_keycode(CL_KEY_W))
			{
				float scale_x, scale_y;
				gb.get_scale(scale_x, scale_y);
				gb.set_scale(scale_x + 0.01, scale_y + 0.01);
			}
			
			if (CL_Keyboard::get_keycode(CL_KEY_S))
			{
				float scale_x, scale_y;
				gb.get_scale(scale_x, scale_y);
				gb.set_scale(scale_x - 0.01, scale_y - 0.01);
			}
			
			if (CL_Keyboard::get_keycode(CL_KEY_SPACE))
			{
				gb.set_angle(0);
				gb.set_scale(1.0, 1.0);
			}
			
			CL_Display::flip();
			CL_System::keep_alive(15);

		}
		
		CL_SetupGL::deinit();
		CL_SetupDisplay::deinit();
		CL_SetupCore::deinit();
	}
	catch (CL_Error err)
	{
		cout << "Fatal error: " << err.message << endl;
		abort();
	}
	
	return 0;
}
