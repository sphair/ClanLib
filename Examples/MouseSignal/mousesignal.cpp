/*
	Demonstration of the CL_Mouse signal interface
*/

#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/application.h>
#include <ClanLib/gl.h>
#include <strstream>

class MouseSignalApp : public CL_ClanApplication
{
private:
	bool quit;

	void on_window_close()
	{
		quit = true;
	}

	void on_button_press(const CL_InputEvent &key)
	{
		std::strstream button;

		switch(key.id) {
		case CL_MOUSE_LEFT:
			button << "Mouse left button";
			break;
		case CL_MOUSE_MIDDLE:
			button << "Mouse middle button";
			break;
		case CL_MOUSE_RIGHT:
			button << "Mouse right button";
			break;
		case CL_MOUSE_XBUTTON1:
			button << "Mouse thumb button";
			break;
		case CL_MOUSE_XBUTTON2:
			button << "Mouse wheel button";
			break;
		case CL_MOUSE_WHEEL_UP:
			button << "Mouse wheel up";
			break;
		case CL_MOUSE_WHEEL_DOWN:
			button << "Mouse wheel down";
			break;
		case CL_KEY_LCONTROL:
			button << "Left control";
			break;
		case CL_KEY_ESCAPE:
			quit = true;
			break;
		default:
			button << key.id << " (" << key.str << ")";
		}

		button << " pressed at " << key.mouse_pos.x << "," << key.mouse_pos.y << '\0';
		std::cout << button.str() << std::endl;
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

			// Set mode: 320x240 16 bpp
			CL_DisplayWindow window("ClanLib MouseSignal Test", 800, 600);

			// Make sure the display is black at startup:
			CL_Display::clear();

			// This connects to the mouse
			CL_Slot slot_mouse_up = CL_Mouse::sig_key_up().connect( this, &MouseSignalApp::on_button_press);

			// If you have more than one mouse connected, you can use one of the following
			// methods to specify which mouse to connect to:
//			CL_Slot slot_input_up = CL_Mouse::get_device(mouse_nr).sig_key_up().connect( this, &MouseSignalApp::on_button_press);
//			CL_Slot slot_input_up = window.get_ic()->get_mouse(mouse_nr).sig_key_up().connect( this, &MouseSignalApp::on_button_press);

			// Lets also connect to the keyboard for fun
			CL_Slot slot_keyboard_up = CL_Keyboard::sig_key_up().connect( this, &MouseSignalApp::on_button_press);

			// Connect the Window close event
			CL_Slot slot_quit = window.sig_window_close().connect(this, &MouseSignalApp::on_window_close);
			quit = false;

			// Loop until the user presses Escape
			while (!CL_Keyboard::get_keycode(CL_KEY_ESCAPE) && !quit)
			{
				// Clear display
				CL_Display::clear(CL_Color(0,0,127));

				// Flip front and backbuffer - this makes the changes visible
				CL_Display::flip();

				CL_System::sleep(10);

				// Update keyboard input and handle system events
				CL_System::keep_alive();
			}
		}
		catch (CL_Error err)
		{
			std::cout << "Exception caught: " << err.message.c_str() << std::endl;

			// Display console close message and wait for a key
			console.display_close_message();
		}

		return 0;
	}
} app;
