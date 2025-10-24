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

private:
	void statetree_test();
} app;

/////////////////////////////////////////////////////////////////////////////
// Application main entry point:

int Application::main(int argc, char** argv)
{
	CL_ConsoleWindow debug_console("Debug console", 80, 1000);
	debug_console.redirect_stdio();

	try
	{
		CL_SetupCore::init();
		CL_SetupDisplay::init();
		CL_SetupGL::init();

		CL_DisplayWindow window("ClanLib StateTree test program", 800, 600, false, true);
		
		statetree_test();

		CL_SetupGL::deinit();
		CL_SetupDisplay::deinit();
		CL_SetupCore::deinit();
	}
	catch (CL_Error error)
	{
		std::cout << "Uncaught error: " << error.message.c_str() << std::endl;
	}

	debug_console.display_close_message();
	return 0;
}

void Application::statetree_test()
{
	CL_Surface image1("images/test32.tga");
	
	while (!CL_Keyboard::get_keycode(CL_KEY_ESCAPE))
	{
//		image1.draw(200,200);
		image1.draw(CL_Rect(0,0,100,100), CL_Rect(200,200,300,300));

		CL_Display::draw_line(0,0,100,100, CL_Color(255,255,255));
		CL_Display::fill_rect(CL_Rect(100,100,200,200), CL_Color(255,255,255));
		
		CL_System::keep_alive();
		CL_Display::flip();
	}
}
