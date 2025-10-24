/*
	Loads a surface from a pcx file.
*/

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

class SurfaceApp : public CL_ClanApplication
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

			CL_DisplayWindow window("Surface Example", 320, 400);

			CL_ResourceManager resources("resources.xml");

			CL_Surface *surface1 = load_image_from_file();
			CL_Surface *surface2 = load_image_from_resource(&resources);

			// Loop until the user hits escape:
			while (CL_Keyboard::get_keycode(CL_KEY_ESCAPE) == false)
			{
				// Draw surfaces:
				surface1->draw(0, 0);
				surface1->draw(0, 200);
				
				// Flip front and backbuffer. This makes the changes visible:
				CL_Display::flip();

				// Improve responsiveness:
				CL_System::sleep(10);
				
				// Update keyboard input and handle system events:
				CL_System::keep_alive();
			}

			delete surface1;
			delete surface2;
		}
		catch (CL_Error err)
		{
			std::cout << "Error: " << err.message.c_str() << std::endl;

			// Display console close message and wait for a key
			console.display_close_message();
		}

		return 0;
	}

	// Load image from file
	CL_Surface *load_image_from_file()
	{
		// Load surface from pcx file:
		return new CL_Surface("survivor.pcx");
	}

	// Load image from resource file
	CL_Surface *load_image_from_resource(CL_ResourceManager *resources)
	{
		// Load surface from resource:
		return new CL_Surface("survivor", resources);
	}
} app;
