#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

#include "cl_writepng.h"

class ScreenShotApp : public CL_ClanApplication
{
public:
	CL_PixelBuffer take_screen_shot()
	{
		CL_PixelBuffer back_buffer = CL_Display::get_current_window()->get_back_buffer();
		back_buffer.lock();
		unsigned short width = back_buffer.get_width();
		unsigned short height = back_buffer.get_height();
		
		CL_PixelBuffer pbuf(width, height, width*3, CL_PixelFormat::rgb888);
		back_buffer.convert(pbuf);
		back_buffer.unlock();

		return pbuf;
	}
	
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
			
			// Set mode: 320x200 16 bpp
			CL_DisplayWindow window("screenshot example", 320, 200, false );

			std::cout << "Press space to take a screenshot" << std::endl;

			// Loop until the user hits escape:
			while (CL_Keyboard::get_keycode(CL_KEY_ESCAPE) == false)
			{
				// CL_Color contains a long list of static colours you can use,
				// like black, white, beige, cornflowerblue. Check reference
				// for full list.
				CL_Display::clear(CL_Color::black);
				
				// Let's draw something
				CL_Display::fill_rect(
					CL_Rect(1,1,200,200),
					CL_Gradient(
						CL_Color::cyan,
						CL_Color::red,
						CL_Color::blue,
						CL_Color::green));

				CL_Display::fill_rect(
					CL_Rect(1,1,50,50),
					CL_Color::cyan );

				CL_Display::fill_rect(
					CL_Rect(50,50,100,100),
					CL_Color::magenta );

				CL_Display::fill_rect(
					CL_Rect(150,150,200,200),
					CL_Color::yellow );
				
				// Make sure our updates are displayed
				CL_Display::flip();
				
				// Update keyboard input and handle system events:
				// Exits the loop if ClanLib requests shutdown - for instance if
				// someone closes the window.
				CL_System::keep_alive(10);

				// If someone pressed SPACE
				if(CL_Keyboard::get_keycode(CL_KEY_SPACE))
				{
					// Save a screenshot
					std::cout << "Saving png..." << std::endl;
					CL_PixelBuffer screenshot = take_screen_shot();

					WritePNG::write_png( screenshot, "screenshot.png" );
				}
			}
		}
		catch (CL_Error err)
		{
			std::cout << "Error: " << err.message.c_str() << std::endl;

			// Display console close message and wait for a key
			console.display_close_message();
		}

		return 0;
	}
} app;
