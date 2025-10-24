/*
	Demonstration of the CL_Mouse polling interface
*/

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

class MouseApp : public CL_ClanApplication
{
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

			CL_DisplayWindow window("Mouse Example", 640, 480);

			// Create an animated sprite manually
			CL_SpriteDescription desc;
			desc.add_gridclipped_frames(CL_PNGProvider("mousecursor.png"), 0, 0, 20, 16, 5, 1, 0, 0, 0);
			CL_Sprite mouse_cursor(desc);
			
			CL_Mouse::set_cursor(mouse_cursor);
			
			// Loop until the user presses Escape
			while (CL_Keyboard::get_keycode(CL_KEY_ESCAPE) == false)
			{
				// Clear display
				CL_Display::clear(CL_Color(0, 0, 128));
			
				// Flip front and backbuffer - this makes the changes visible
				CL_Display::flip();
			
				// Update keyboard input and handle system events
				CL_System::keep_alive(10);

				if(CL_Mouse::get_keycode(CL_MOUSE_LEFT))
					std::cout << "Left pressed at " << CL_Mouse::get_x() << "," << CL_Mouse::get_y() << std::endl;
				if(CL_Mouse::get_keycode(CL_MOUSE_MIDDLE))
					std::cout << "Middle pressed" << std::endl;
				if(CL_Mouse::get_keycode(CL_MOUSE_RIGHT))
					std::cout << "Right pressed" << std::endl;
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
