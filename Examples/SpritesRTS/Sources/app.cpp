#include <ClanLib/display.h>
#include <ClanLib/sound.h>
#include <ClanLib/gl.h>

#include "app.h"
#include "world.h"

Application app;

int Application::main(int argc, char** argv)
{
	#ifdef _DEBUG
	// Create a console window for text-output if not available
	CL_ConsoleWindow console("ClanLib RTS Demo", 80, 1000); // 1000 allows a y-scrollbar to be present
	console.redirect_stdio();
	#endif
	
	try
	{
		// Init stuff
		CL_SetupCore setup_core;
		CL_SetupDisplay setup_display;
		CL_SetupSound setup_sound;
		CL_SetupGL setup_gl;
		
		// Create a window
		CL_DisplayWindow window("ClanLib RTS Demo", 1024, 768, false);

		CL_SoundOutput output(44100);

		// Create world
		World world;

		// Connect the Window close event
		CL_Slot slot_quit = window.sig_window_close().connect(&world, &World::on_quit);

		// Run the main loop
		world.run();
	}
	catch (CL_Error e)
	{
		std::cout << e.message.c_str() << std::endl;

		#ifdef _DEBUG
		console.display_close_message();
		#endif
	}

	return 0;
}
