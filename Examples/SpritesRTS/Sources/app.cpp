#include <ClanLib/display.h>
#include <ClanLib/sound.h>
#include <ClanLib/gl.h>

#include "app.h"
#include "world.h"

// This is the Program class that is called by CL_ClanApplication
class Program
{
public:
	static int main(const std::vector<CL_String> &args)
	{
		// Initialize ClanLib base components
		CL_SetupCore setup_core;

		// Initialize the ClanLib display component
		CL_SetupDisplay setup_display;

		// Initilize the OpenGL drivers
		CL_SetupGL setup_gl;

		CL_SetupSound setup_sound;

		// Start the Application
		Application app;
		int retval = app.main(args);
		return retval;
	}
};

// Instantiate CL_ClanApplication, informing it where the Program is located
CL_ClanApplication app(&Program::main);

int Application::main(const std::vector<CL_String> &args)
{
	try
	{		
		// Create a window
		CL_DisplayWindowDescription desc;
		desc.set_title("ClanLib RTS Demo");
		desc.set_size(CL_Size(1024, 768), true);	// Use this resolution (as caption is disabled)
		CL_DisplayWindow window(desc);

		CL_GraphicContext gc = window.get_gc();

		CL_SoundOutput output(44100);

		// Create world
		World world(window);

		// Run the main loop
		world.run();
	}
	catch(CL_Exception &exception)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console", 80, 160);
		CL_Console::write_line("Exception caught: " + exception.get_message_and_stack_trace());
		console.display_close_message();

		return -1;
	}

	return 0;
}
