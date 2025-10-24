
#include "precomp.h"
#include "application.h"
#include "mainframe.h"
#include "crash_reporter.h"
#include "detect_hang.h"
#include <iostream>
#include <ClanLib/gdi.h>

// This is the Program class that is called by CL_ClanApplication
class Program
{
public:
	static int main(const std::vector<CL_String> &args)
	{
		// Initialize ClanLib base components
		CL_SetupCore setup_core;

		#ifndef _DEBUG
		CL_String appdata_dir = CL_Directory::get_appdata("ClanLib", "Carambola", "1.0");
		CrashReporter crash_reporter(appdata_dir);
		#endif

		DetectHang detect_hang;

		// Initialize the ClanLib display component
		CL_SetupDisplay setup_display;

		// Initilize the OpenGL drivers
		//CL_SetupGL setup_gl;
		CL_SetupGDI setup_gdi;

		// Initialize the network
		CL_SetupNetwork setup_network;

		// Start the Application
		Application app;
		int retval = app.main(args);
		return retval;
	}
};

// Instantiate CL_ClanApplication, informing it where the Program is located
CL_ClanApplication app(&Program::main);

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

int Application::main(const std::vector<CL_String> &args)
{
	// In release mode we prefer to crash with a dump file
#ifdef _DEBUG
	try
	{
#endif
		CL_ResourceManager local_resources("resources.xml");
		CL_ResourceManager resources("../../Resources/GUIThemeAero/resources.xml");
		local_resources.add_resources(resources);
		CL_GUIThemeDefault theme;
		theme.set_resources(local_resources);
		CL_GUIWindowManagerSystem wm;
		CL_GUIManager gui;
		gui.set_window_manager(wm);
		gui.set_theme(theme);
		gui.set_css_document("theme.css");

		MainFrame mainframe(&gui, &resources);
		gui.exec();
#ifdef _DEBUG
	}
	catch(CL_Exception &exception)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console", 80, 160);
		CL_Console::write_line("Exception caught: " + exception.get_message_and_stack_trace());
		console.display_close_message();

		return -1;
	}
#endif

	return 0;
}
