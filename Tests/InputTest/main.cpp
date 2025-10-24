#include <ClanLib/application.h>
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

/////////////////////////////////////////////////////////////////////////////
// Application class

class Application : public CL_ClanApplication
{
public:
	virtual int main(int argc, char** argv);

private:
	void input_test();

	void on_keyboard_down(const CL_InputEvent &key);
	void on_keyboard_up(const CL_InputEvent &key);
	void on_keyboard_dblclk(const CL_InputEvent &key);
	void on_mouse_down(const CL_InputEvent &key);
	void on_mouse_up(const CL_InputEvent &key);
	void on_mouse_dblclk(const CL_InputEvent &key);
	void on_mouse_move(const CL_InputEvent &key);
	void on_joystick_down(const CL_InputEvent &key);
	void on_joystick_up(const CL_InputEvent &key);
	void on_joystick_dblclk(const CL_InputEvent &key);
	void on_joystick_move(const CL_InputEvent &key);
	
	CL_InputEvent last_key;
	std::string last_event;
	
	CL_SlotContainer slots;

	CL_Font *font;
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

		CL_DisplayWindow window("ClanLib Input test program", 800, 600, false, true);
		
		CL_ResourceManager resources("resources.xml", false);
		font = new CL_Font("font_gray", &resources);
		
		input_test();

		delete font;

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

void Application::input_test()
{
	slots.connect(CL_Keyboard::sig_key_down(), this, &Application::on_keyboard_down);
	slots.connect(CL_Keyboard::sig_key_up(), this, &Application::on_keyboard_up);
	slots.connect(CL_Keyboard::sig_key_dblclk(), this, &Application::on_keyboard_dblclk);

	slots.connect(CL_Mouse::sig_key_down(), this, &Application::on_mouse_down);
	slots.connect(CL_Mouse::sig_key_up(), this, &Application::on_mouse_up);
	slots.connect(CL_Mouse::sig_key_dblclk(), this, &Application::on_mouse_dblclk);
	slots.connect(CL_Mouse::sig_move(), this, &Application::on_mouse_move);
	
/*	slots.connect(CL_Joystick::sig_key_down(), this, &Application::on_joystick_down);
	slots.connect(CL_Joystick::sig_key_up(), this, &Application::on_joystick_up);
	slots.connect(CL_Joystick::sig_key_dblclk(), this, &Application::on_joystick_dblclk);
	slots.connect(CL_Joystick::sig_move(), this, &Application::on_joystick_move);
*/
	while (!CL_Keyboard::get_keycode(CL_KEY_ESCAPE))
	{
		CL_Display::clear();

		int count, i;

		count = CL_Mouse::get_device_count();
		font->draw(20, 0, "Mouse devices: " + CL_String::from_int(count));
		for(i = 0; i < count; i++)
		{
			std::string str = CL_String::format("- get_device(%1): %2",
				i,
				(int)&CL_Mouse::get_device(i)
			);
			font->draw(20, 10 + i*10, str);
		}

		int x = CL_Mouse::get_x();
		int y = CL_Mouse::get_y();
		
		font->draw(180, 00, "Mouse x: " + CL_String::from_int(x));
		font->draw(180, 10, "Mouse y: " + CL_String::from_int(y));

		font->draw(300, 00, CL_String::format("Mouse %1 (%2): %3", "left", CL_Mouse::get_key_name(CL_MOUSE_LEFT), CL_String::from_int(CL_Mouse::get_keycode(CL_MOUSE_LEFT))));
		font->draw(300, 10, CL_String::format("Mouse %1 (%2): %3", "middle", CL_Mouse::get_key_name(CL_MOUSE_MIDDLE), CL_String::from_int(CL_Mouse::get_keycode(CL_MOUSE_MIDDLE))));
		font->draw(300, 20, CL_String::format("Mouse %1 (%2): %3", "right", CL_Mouse::get_key_name(CL_MOUSE_RIGHT), CL_String::from_int(CL_Mouse::get_keycode(CL_MOUSE_RIGHT))));
		font->draw(300, 30, CL_String::format("Mouse %1 (%2): %3", "xbutton1", CL_Mouse::get_key_name(CL_MOUSE_XBUTTON1), CL_String::from_int(CL_Mouse::get_keycode(CL_MOUSE_XBUTTON1))));
		font->draw(300, 40, CL_String::format("Mouse %1 (%2): %3", "xbutton2", CL_Mouse::get_key_name(CL_MOUSE_XBUTTON2), CL_String::from_int(CL_Mouse::get_keycode(CL_MOUSE_XBUTTON2))));

		font->draw(180, 80, "Press 1, 2 or 3 to position mouse");
		if(CL_Keyboard::get_keycode(CL_KEY_1))
			CL_Mouse::set_position(0, 0);
		if(CL_Keyboard::get_keycode(CL_KEY_2))
			CL_Mouse::set_position(400, 300);
		if(CL_Keyboard::get_keycode(CL_KEY_3))
			CL_Mouse::set_position(799, 599);
			
		count = CL_Keyboard::get_device_count();
		font->draw(20, 120, "Keyboard devices: " + CL_String::from_int(count));
		for(i = 0; i < count; i++)
		{
			std::string str = CL_String::format("- get_device(%1): %2",
				i,
				(int)&CL_Keyboard::get_device(i)
			);
			font->draw(20, 130 + i*10, str);
		}

		font->draw(300, 120, CL_String::format("Keyboard %1 (%2): %3", "a", CL_Keyboard::get_key_name(CL_KEY_A), CL_String::from_int(CL_Keyboard::get_keycode(CL_KEY_A))));
		font->draw(300, 130, CL_String::format("Keyboard %1 (%2): %3", "backspace", CL_Keyboard::get_key_name(CL_KEY_BACKSPACE), CL_String::from_int(CL_Keyboard::get_keycode(CL_KEY_BACKSPACE))));
		font->draw(300, 140, CL_String::format("Keyboard %1 (%2): %3", "shift", CL_Keyboard::get_key_name(CL_KEY_SHIFT), CL_String::from_int(CL_Keyboard::get_keycode(CL_KEY_SHIFT))));
		font->draw(300, 150, CL_String::format("Keyboard %1 (%2): %3", "left shift", CL_Keyboard::get_key_name(CL_KEY_LSHIFT), CL_String::from_int(CL_Keyboard::get_keycode(CL_KEY_LSHIFT))));
		font->draw(300, 160, CL_String::format("Keyboard %1 (%2): %3", "right shift", CL_Keyboard::get_key_name(CL_KEY_RSHIFT), CL_String::from_int(CL_Keyboard::get_keycode(CL_KEY_RSHIFT))));
		font->draw(300, 170, CL_String::format("Keyboard %1 (%2): %3", "control", CL_Keyboard::get_key_name(CL_KEY_CONTROL), CL_String::from_int(CL_Keyboard::get_keycode(CL_KEY_CONTROL))));
		font->draw(300, 180, CL_String::format("Keyboard %1 (%2): %3", "menu", CL_Keyboard::get_key_name(CL_KEY_MENU), CL_String::from_int(CL_Keyboard::get_keycode(CL_KEY_MENU))));
		font->draw(300, 190, CL_String::format("Keyboard %1 (%2): %3", "numpad 0", CL_Keyboard::get_key_name(CL_KEY_NUMPAD0), CL_String::from_int(CL_Keyboard::get_keycode(CL_KEY_NUMPAD0))));
		font->draw(300, 200, CL_String::format("Keyboard %1 (%2): %3", "f1", CL_Keyboard::get_key_name(CL_KEY_F1), CL_String::from_int(CL_Keyboard::get_keycode(CL_KEY_F1))));

		font->draw(550, 120, CL_String::format("Keyboard %1 (%2): %3", "up", CL_Keyboard::get_key_name(CL_KEY_UP), CL_String::from_int(CL_Keyboard::get_keycode(CL_KEY_UP))));
		font->draw(550, 130, CL_String::format("Keyboard %1 (%2): %3", "left", CL_Keyboard::get_key_name(CL_KEY_LEFT), CL_String::from_int(CL_Keyboard::get_keycode(CL_KEY_LEFT))));
		font->draw(550, 140, CL_String::format("Keyboard %1 (%2): %3", "right", CL_Keyboard::get_key_name(CL_KEY_RIGHT), CL_String::from_int(CL_Keyboard::get_keycode(CL_KEY_RIGHT))));
		font->draw(550, 150, CL_String::format("Keyboard %1 (%2): %3", "down", CL_Keyboard::get_key_name(CL_KEY_DOWN), CL_String::from_int(CL_Keyboard::get_keycode(CL_KEY_DOWN))));

		count = CL_Joystick::get_device_count();
		font->draw(20, 240, "Joystick devices: " + CL_String::from_int(count));
		for(i = 0; i < count; i++)
		{
			std::string str = CL_String::format("- get_device(%1): %2",
				i,
				(int)&CL_Joystick::get_device(i)
			);
			font->draw(20, 250 + i*10, str);
		}

		if(!last_event.empty())
		{
			font->draw(20, 420, last_event);
			font->draw(20, 430, "str: " + last_key.str);
			font->draw(20, 440, "id: " + CL_String::from_int(last_key.id));
			font->draw(60, 440, last_key.device.get_key_name(last_key.id));
/*			if(last_key.device.get_type() == CL_InputDevice::keyboard)
				font->draw(60, 440, CL_String(CL_Keyboard::get_key_name(last_key.id)));
			if(last_key.device.get_type() == CL_InputDevice::mouse)
				font->draw(60, 440, CL_String(CL_Mouse::get_key_name(last_key.id)));
			if(last_key.device.get_type() == CL_InputDevice::joystick)
				font->draw(60, 440, CL_String(CL_Joystick::get_key_name(last_key.id)));
*/			font->draw(20, 450, "type: " + CL_String::from_int(last_key.type));
			font->draw(20, 460, "left_alt: " + CL_String::from_int(last_key.left_alt));
			font->draw(20, 470, "left_ctrl: " + CL_String::from_int(last_key.left_ctrl));
			font->draw(20, 480, "left_shift: " + CL_String::from_int(last_key.left_shift));
			font->draw(20, 490, "right_alt: " + CL_String::from_int(last_key.right_alt));
			font->draw(20, 500, "right_ctrl: " + CL_String::from_int(last_key.right_ctrl));
			font->draw(20, 510, "right_shift: " + CL_String::from_int(last_key.right_shift));
			font->draw(20, 520, "mouse_pos.x: " + CL_String::from_int(last_key.mouse_pos.x));
			font->draw(20, 530, "mouse_pos.y: " + CL_String::from_int(last_key.mouse_pos.y));
			font->draw(20, 540, "repeat_count: " + CL_String::from_int(last_key.repeat_count));
//			font->draw(20, 550, "Device: " + last_key.device);
		}

		CL_Display::flip();
		CL_System::keep_alive(10);
	}
}

void Application::on_keyboard_down(const CL_InputEvent &key)
{
	last_key = key;
	last_event = "sig_keyboard_down";
}

void Application::on_keyboard_up(const CL_InputEvent &key)
{
	last_key = key;
	last_event = "sig_keyboard_up";
}

void Application::on_keyboard_dblclk(const CL_InputEvent &key)
{
	last_key = key;
	last_event = "sig_keyboard_dblclk";
}

void Application::on_mouse_down(const CL_InputEvent &key)
{
	last_key = key;
	last_event = "sig_mouse_down";
}

void Application::on_mouse_up(const CL_InputEvent &key)
{
	last_key = key;
	last_event = "sig_mouse_up";
}

void Application::on_mouse_dblclk(const CL_InputEvent &key)
{
	last_key = key;
	last_event = "sig_mouse_dblclk";
}

void Application::on_mouse_move(const CL_InputEvent &key)
{
	last_key = key;
	last_event = "sig_mouse_move";
}

void Application::on_joystick_down(const CL_InputEvent &key)
{
	last_key = key;
	last_event = "sig_joystick_down";
}

void Application::on_joystick_up(const CL_InputEvent &key)
{
	last_key = key;
	last_event = "sig_joystick_up";
}

void Application::on_joystick_dblclk(const CL_InputEvent &key)
{
	last_key = key;
	last_event = "sig_joystick_dblclk";
}

void Application::on_joystick_move(const CL_InputEvent &key)
{
	last_key = key;
	last_event = "sig_joystick_move";
}
