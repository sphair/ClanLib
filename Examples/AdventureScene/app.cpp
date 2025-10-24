#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

#include "scene.h"
#include "player.h"

class AdventureSceneApp : public CL_ClanApplication
{
public:
	virtual int main(int argc, char **argv)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();

		try
		{
			// Initialize ClanLib base components
			CL_SetupCore setup_core;

			// Initialize the ClanLib display component
			CL_SetupDisplay setup_display;
			
			// Initilize the OpenGL drivers
			CL_SetupGL setup_gl;

			// Set a videomode
			CL_DisplayWindow window("ClanLib Adventure Scene", 320, 240);

			// Connect the Window close event
			CL_Slot slot_quit = window.sig_window_close().connect(this, &AdventureSceneApp::on_window_close);
			quit = false;

			// Dump some instructions
			std::cout << "Press 1 to skip drawing background" << std::endl;
			std::cout << "Press 2 to draw mask" << std::endl;
			std::cout << "Press 3 to draw walk" << std::endl;
			std::cout << "Press 4 to skip drawing objects" << std::endl << std::endl;
			std::cout << "Use arrows to control player" << std::endl;
			
			// Create a scene
			Scene scene("resources/ex-bkg.tga", "resources/ex-mask.tga", "resources/ex-wasc.tga");

			// Create a player (will be deleted by scene)
			new Player(&scene);
			
			// Run until someone presses escape
			while (!CL_Keyboard::get_keycode(CL_KEY_ESCAPE) && !quit)
			{
				// Draw the whole scene with all objects
				scene.draw();
				
				// Flip the display, showing on the screen what we have drawed
				// since last call to flip_display()
				CL_Display::flip();

				// This call updates input and performs other "housekeeping"
				// call this each frame
				CL_System::keep_alive(5);
			}
		}
		catch(CL_Error error)
		{
			std::cout << "Exception caught : " << error.message.c_str() << std::endl;			

			// Display console close message and wait for a key
			console.display_close_message();

			return -1;
		}

		return 0;
	}

private:
	bool quit;

	void on_window_close()
	{
		quit = true;
	}
} my_app;
