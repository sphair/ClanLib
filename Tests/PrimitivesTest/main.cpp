#include <ClanLib/application.h>
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

/////////////////////////////////////////////////////////////////////////////
// Application class

class Application : public CL_ClanApplication
{
public:
	int main(int argc, char** argv);
} app;

/////////////////////////////////////////////////////////////////////////////
// Application main entry point:

int Application::main(int argc, char** argv)
{
	CL_ConsoleWindow debug_console("Debug console");
	debug_console.redirect_stdio();

	try
	{
		CL_SetupCore::init();
		CL_SetupDisplay::init();
		CL_SetupGL::init();

		{
			CL_DisplayWindow window("ClanLib Primitives Test", 320, 200, true);

			CL_Surface surface("images/primitives.png");
			
			while(!CL_Keyboard::get_keycode(CL_KEY_ESCAPE))
			{
				CL_Display::clear();

				if(!CL_Keyboard::get_keycode(CL_KEY_SPACE))
					surface.draw();
				else
				{
					CL_Display::draw_rect(CL_Rect(0, 0, 320, 200), CL_Color::white);

					CL_Display::draw_pixel(1, 1, CL_Color::red);
					CL_Display::draw_pixel(3, 3, CL_Color::red);
					CL_Display::draw_pixel(1, 198, CL_Color::red);
					CL_Display::draw_pixel(3, 196, CL_Color::red);
					CL_Display::draw_pixel(318, 1, CL_Color::red);
					CL_Display::draw_pixel(316, 3, CL_Color::red);
					CL_Display::draw_pixel(318, 198, CL_Color::red);
					CL_Display::draw_pixel(316, 196, CL_Color::red);

					CL_Display::draw_line(2, 2, 2, 198, CL_Color::yellow);
					CL_Display::draw_line(4, 4, 316, 4, CL_Color::yellow);
					CL_Display::draw_line(4, 195, 316, 195, CL_Color::yellow);
					CL_Display::draw_line(317, 2, 317, 198, CL_Color::yellow);
					
					CL_Display::fill_rect(CL_Rect(5, 5, 15, 15), CL_Color::blue);
				}

				CL_Display::flip();
				CL_System::keep_alive(10);
			}

			while(CL_Keyboard::get_keycode(CL_KEY_ESCAPE))
				CL_System::keep_alive(10);
		}
				
		{
			CL_DisplayWindow window("ClanLib Primitives Test", 640, 480, false);

			while(!CL_Keyboard::get_keycode(CL_KEY_ESCAPE))
			{
				CL_Display::clear();

				for(int y=0; y<480; ++y)
					for(int x=0; x<640; x+=4)
						CL_Display::draw_line(x, y, x + 2, y, CL_Color::white);

				CL_Display::flip();
				CL_System::keep_alive(10);
			}

			while(CL_Keyboard::get_keycode(CL_KEY_ESCAPE))
				CL_System::keep_alive(10);
		}
		
		{
			CL_DisplayWindow window("ClanLib Primitives Test", 480, 480, false);

			while(!CL_Keyboard::get_keycode(CL_KEY_ESCAPE))
			{
				CL_Display::clear();

				for(int y=0; y<=480; ++y)
					CL_Display::draw_rect(CL_Rect(0, 0, y, y), CL_Color::white);

				CL_Display::flip();
				CL_System::keep_alive(10);
			}
		}

		CL_SetupGL::deinit();
		CL_SetupDisplay::deinit();
		CL_SetupCore::deinit();
	}
	catch (CL_Error error)
	{
		std::cout << "Uncaught error: " << error.message.c_str() << std::endl;
		debug_console.display_close_message();
	}

	return 0;
}
