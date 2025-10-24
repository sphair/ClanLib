/*
	Init display example.
*/

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

#define SCREEN_X 640
#define SCREEN_Y 480


class InitDisplayApp : public CL_ClanApplication
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

			CL_DisplayWindow display("Init Display example - F and W for fullscreen/windowed mode", SCREEN_X, SCREEN_Y);

			// Loop until the user hits escape:
			while (CL_Keyboard::get_keycode(CL_KEY_ESCAPE) == false)
			{

				
				//*********************************************************************************
				// This part is so you can press F and W to toggle fullscreen and windowed mode

				if (CL_Keyboard::get_keycode(CL_KEY_F))
				{
					display.set_fullscreen(SCREEN_X,SCREEN_Y);
				}
			
				if (CL_Keyboard::get_keycode(CL_KEY_W))
				{
					display.set_windowed();
					display.set_size(SCREEN_X,SCREEN_Y);
				}
				//*********************************************************************************


				// Clear display with random colors:
				CL_Display::clear(CL_Color(
					rand() % 255,  // red component
					rand() % 255,  // green component
					rand() % 255)); // blue component

				
				//Draw a few filled rectangles
				CL_Display::fill_rect(CL_Rect(240, 140, 440, 340), CL_Color::green);
				CL_Display::fill_rect(CL_Rect(340, 170, 510, 200), CL_Color::red);

				// Flip front and backbuffer. This makes the changes visible:
				CL_Display::flip();
			
				// Update keyboard input and handle system events:
				CL_System::keep_alive();
			}
		}
		catch (CL_Error err)
		{
			std::cout << "Error occoured: " << err.message.c_str() << std::endl;

			// Display console close message and wait for a key
			console.display_close_message();
		}

		return 0;
	}
} app;
