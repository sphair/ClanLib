
#include "precomp.h"
#include "application.h"
#include "mainframe.h"
#include <iostream>

/////////////////////////////////////////////////////////////////////////////
// Application construction:

Application::Application()
{
	instance = this;
}

Application::~Application()
{
	instance = 0;
}

/////////////////////////////////////////////////////////////////////////////
// Application attributes:

Application *Application::instance = 0;

/////////////////////////////////////////////////////////////////////////////
// Application operations:

/////////////////////////////////////////////////////////////////////////////
// Application implementation:

Application app;

int Application::main(int argc, char **argv)
{
	// Create a console window for text-output if not available
	CL_ConsoleWindow console("Console");
	console.redirect_stdio();

	CL_SetupCore setup_core;
	CL_SetupDisplay setup_display;
	CL_SetupGL setup_gl;
	CL_SetupGUI setup_gui;
	CL_SetupNetwork setup_network;

	try
	{
		CL_DisplayWindow window("CTalk IRC Client", 800, 600, false, true);

		CL_ResourceManager resources((const std::string)"resources.xml");
#ifdef __APPLE__
		resources.add_resources(CL_ResourceManager((const std::string)"GUIStyleSilver/gui.xml"));
#else
		resources.add_resources(CL_ResourceManager((const std::string)"../../Resources/GUIStyleSilver/gui.xml"));
#endif

		CL_StyleManager_Silver style(&resources);
		CL_GUIManager gui(&style);
		MainFrame mainframe(&gui, &resources);
		gui.run(0);
	}
	catch (CL_Error error)
	{
		std::cout << error.message.c_str() << std::endl;

		// Display console close message and wait for a key
		console.display_close_message();
	}

	return 0;
}
