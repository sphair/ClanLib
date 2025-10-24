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
	void inputbutton_test();
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

		CL_DisplayWindow window("ClanLib InputButton test program", 800, 600, false, true);
		
		inputbutton_test();

		CL_SetupDisplay::deinit();
		CL_SetupCore::deinit();
	}
	catch (CL_Error error)
	{
		std::cout << "Uncaught error: " << error.message.c_str() << std::endl;
		debug_console.wait_for_key();
	}

	return 0;
}

void Application::inputbutton_test()
{
	CL_InputButton move_forward, move_left, move_right, fire;

	move_forward.bind(CL_KeyBinding(CL_KEY_UP, CL_Keyboard::get_device()));
	move_forward.bind(CL_KeyBinding(CL_MOUSE_RIGHT, CL_Mouse::get_device()));
	move_left.bind(CL_KeyBinding(CL_KEY_LEFT, CL_Keyboard::get_device()));
	move_right.bind(CL_KeyBinding(CL_KEY_RIGHT, CL_Keyboard::get_device()));
	fire.bind(CL_KeyBinding(CL_MOUSE_LEFT, CL_Mouse::get_device()));
	fire.bind(CL_KeyBinding(CL_KEY_SPACE, CL_Keyboard::get_device()));
	
	while (!CL_Keyboard::get_keycode(CL_KEY_ESCAPE))
	{
		CL_Display::clear();

		if(move_forward.peek_pressed())
			std::cout << "Move forward " << move_forward.pop_pressed() << std::endl;
		if(move_left.peek_pressed())
			std::cout << "Move left " << move_left.pop_pressed() << std::endl;
		if(move_right.peek_pressed())
			std::cout << "Move right " << move_right.pop_pressed() << std::endl;
		if(fire.peek_pressed())
			std::cout << "Fire " << fire.pop_pressed() << std::endl;

		CL_Display::flip();
		CL_System::keep_alive(10);
	}
}
