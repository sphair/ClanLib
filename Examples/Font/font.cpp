/*
	Font Example
*/

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

class FontApp : public CL_ClanApplication
{
public:
	int main(int, char **)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console",80,1000);
		console.redirect_stdio();

		try
		{
			CL_SetupCore setup_core;
			CL_SetupDisplay setup_display;
			CL_SetupGL setup_gl;

			{
				CL_DisplayWindow window("ClanLib Font Example", 640, 480);

				CL_ResourceManager resources("font.xml");
            			
				// Load some fonts from the resource file
				CL_Font font1("Font1", &resources);
				CL_Font font2("Font2", &resources);
				
				// Connect the Window close event
				CL_Slot slot_quit = window.sig_window_close().connect(this, &FontApp::on_window_close);
				quit = false;

				while (!CL_Keyboard::get_keycode(CL_KEY_ESCAPE) && !quit)
				{
					CL_Display::clear(CL_Color::red);

					font1.draw(25, 25, "ClanLib: Phear the Power!");
					
					font2.draw(3, 155, "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz∆Êÿ¯≈Â0123456789[]()!#$&%/\\=-+~'`\";.,:;*?");
					
					CL_Display::flip();
					CL_System::keep_alive(15);
				}
			}
		}
		catch (CL_Error err)
		{
			std::cout << "Exception caught: " << err.message.c_str() << std::endl;

			// Display console close message and wait for a key
			console.display_close_message();

			return -1;
		}
		
		return 0;
	}

private:
	void on_window_close()
	{
		quit = true;
	}

	bool quit;
} app;
