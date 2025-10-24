#include "precomp.h"
#include "application.h"
#include "main_window.h"

int Program::main(const std::vector<CL_String> &args)
{
	CL_SetupCore setup_core;
	CL_SetupDisplay setup_display;
	CL_SetupGL setup_gl;
	//CL_SetupGL1 setup_gl;
	//CL_SetupGDI setup_gdi;

	CL_ConsoleWindow console("Console", 140, 2000);
	CL_ConsoleLogger logger;

	try
	{
		Application app;
		app.run();
	}
	catch (CL_Exception e)
	{
#ifdef WIN32
		MessageBox(0, e.message.c_str(), TEXT("Unhandled Exception"), MB_OK|MB_ICONERROR);
#else
		CL_Console::write_line("Unhandled exception: %1", e.message);
#endif
		console.display_close_message();
		return 0;
	}

	return 1;
}

Application::Application()
{
	resources = CL_ResourceManager("../../Resources/GUIThemeAero/resources.xml");
	theme = CL_SharedPtr<CL_GUITheme>(new CL_GUIThemeDefault);
	theme->set_resources(resources);
	window_manager = CL_SharedPtr<CL_GUIWindowManagerSystem>(new CL_GUIWindowManagerSystem);
	gui = CL_SharedPtr<CL_GUIManager>(new CL_GUIManager);
	gui->set_window_manager(*window_manager);
	gui->set_theme(*theme);
	gui->set_css_document("../../Resources/GUIThemeAero/theme.css");
}

Application::~Application()
{
}

void Application::run()
{
	MainWindow main_window(this);
	gui->exec();
}

CL_ClanApplication app(&Program::main);
