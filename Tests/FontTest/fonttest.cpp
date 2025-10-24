#include "fonttest.h"

#include <string>
#include <iostream>
#include <cmath>

using namespace std;


void do_test(
	const CL_Rect &rect,
	CL_Origin origin,
	const string &descrip,
	CL_Font font,
	bool print_time)
{
	string message = descrip + " Hit escape to exit. ";
	message += "Here is the greatest joke in the world: One day, a pirate walked into a bar. ";
	message += "The pirate had a steering wheel strapped to his crotch. The barman asked, \"Why ";
	message += "do you have a steering wheel strapped to your crotch?\". The pirate responded, ";
	message += "\"Arrrr! It's drivin' me nuts!\"";
	
	font.set_alignment(origin);
	font.set_color(CL_Color::red);
	
	unsigned int start_time = CL_System::get_time();
	font.draw(rect, message);
	int elapsed_time = CL_System::get_time() - start_time;
	
	CL_Display::draw_rect(rect, CL_Color::blue);
	
	if (print_time)
		cout << descrip << ": " << elapsed_time << " milliseconds.\n";
}

int FontTestApp::main(int argc, char** argv)
{
	try
	{
		CL_SetupCore::init();
		CL_SetupDisplay::init();
		CL_SetupGL::init();
		
		CL_ResourceManager resources("data.scr", false);
		CL_DisplayWindow window("ClanLib Font Test", 640, 480);
		CL_Font font("Font", &resources);

		int framec = 0;
		bool show_times = false;
		
		while (!CL_Keyboard::get_keycode(CL_KEY_ESCAPE))
		{
			CL_Display::clear(CL_Color::white);
			
			do_test(CL_Rect(20, 20, 206, 153), origin_top_left, "Top left alignment", font, show_times);
			do_test(CL_Rect(226, 20, 412, 153), origin_top_center, "Top center alignment", font, show_times);
			do_test(CL_Rect(432, 20, 618, 153), origin_top_right, "Top right alignment", font, show_times);
			do_test(CL_Rect(20, 173, 206, 306), origin_center_left, "Center left alignment", font, show_times);
			do_test(CL_Rect(226, 173, 412, 306), origin_center, "Center alignment", font, show_times);
			do_test(CL_Rect(432, 173, 618, 306), origin_center_right, "Center right alignment", font, show_times);
			do_test(CL_Rect(20, 326, 206, 459), origin_bottom_left, "Bottom left alignment", font, show_times);
			do_test(CL_Rect(226, 326, 412, 459), origin_bottom_center, "Bottom center alignment", font, show_times);
			do_test(CL_Rect(432, 326, 618, 459), origin_bottom_right, "Bottom right alignment", font, show_times);
			
			CL_Display::flip();
			CL_System::keep_alive(10);
			
			++framec;
			
			if (framec % 200 == 0)
				show_times = true;
			else
				show_times = false;
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
