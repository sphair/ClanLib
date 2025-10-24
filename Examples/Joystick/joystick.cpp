#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

class JoystickApp : public CL_ClanApplication
{
	void on_joystick_down(const CL_InputEvent &key)
	{
		std::cout << "Button " << key.id << " pressed" << std::endl;
	}

	void on_joystick_up(const CL_InputEvent &key)
	{
		std::cout << "Button " << key.id << " released" << std::endl;
	}

	void on_joystick_move(const CL_InputEvent &key)
	{
		std::cout << "Axis " << key.id << " move " << key.axis_pos << std::endl;
	}

public:
	virtual int main(int, char **)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();

		try
		{
			CL_SetupCore setup_core;
			CL_SetupDisplay setup_display;
			CL_SetupGL setup_gl;

			CL_DisplayWindow window("Joystick Example", 640, 480);

			int width = CL_Display::get_width();
			int height = CL_Display::get_height();
			
			int joysticks = window.get_ic()->get_joystick_count();
			std::cout << "Found " << joysticks << " joysticks." << std::endl;

			if (joysticks != 0)
			{
				CL_InputDevice joystick = window.get_ic()->get_joystick();

				std::cout << "Name: " << joystick.get_name() << std::endl;
				std::cout << "Axis: " << joystick.get_axis_count() << std::endl;
				std::cout << "Buttons: " << joystick.get_button_count() << std::endl;

				CL_Slot slot_joystick_move = joystick.sig_axis_move().connect(this, &JoystickApp::on_joystick_move);
				CL_Slot slot_joystick_button_up   = joystick.sig_key_down().connect(this, &JoystickApp::on_joystick_up);
				CL_Slot slot_joystick_button_down = joystick.sig_key_up().connect(this, &JoystickApp::on_joystick_down);
			
				// Loop until the user presses Escape
				while (CL_Keyboard::get_keycode(CL_KEY_ESCAPE) == false)
				{
					// Clear display
					CL_Display::clear(CL_Color(0, 0, 128));
				
					double x_axis = joystick.get_axis(0);
					double y_axis = joystick.get_axis(1);

					int x = int(width/2+x_axis*width/2);
					int y = int(height/2+y_axis*height/2);
					CL_Rect rect(x-10, y-10, x+10, y+10);
					CL_Color color = CL_Color::white;
					if (joystick.get_keycode(0)) color = CL_Color::blue;
					if (joystick.get_keycode(1)) color = CL_Color::burlywood;
					if (joystick.get_keycode(2)) color = CL_Color::cornflowerblue;
					if (joystick.get_keycode(3)) color = CL_Color::gainsboro;
					if (joystick.get_keycode(4)) color = CL_Color::indianred;
					if (joystick.get_keycode(5)) color = CL_Color::lightskyblue;
					if (joystick.get_keycode(6)) color = CL_Color::lime;
					if (joystick.get_keycode(7)) color = CL_Color::orange;
					
					CL_Display::fill_rect(rect, color);
					
					// Flip front and backbuffer - this makes the changes visible
					CL_Display::flip();
				
					// Update keyboard input and handle system events
					CL_System::keep_alive(10);
				}
			}
		}
		catch (CL_Error err)
		{
			std::cout << "Exception caught: " << err.message.c_str() << std::endl;
		}

		// Display console close message and wait for a key
		console.display_close_message();

		return 0;
	}
} app;
