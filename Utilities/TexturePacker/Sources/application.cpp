#include "precomp.h"
#include "main_window.h"

class Application
{
public:
	int main(const std::vector<CL_String> &args)
	{
		try
		{
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

			MainWindow mainwindow(&gui, &resources);
			gui.exec();
		}
		catch (CL_Exception &exception)
		{
			CL_ConsoleWindow console("Console", 160, 1000);
			CL_Console::write_line("Exception caught: " + exception.get_message_and_stack_trace());
			console.display_close_message();
		}

		return 0;
	}
};

class Program
{
public:
	static int main(const std::vector<CL_String> &args)
	{
		CL_SetupCore setup_core;
		CL_SetupDisplay setup_display;
//		CL_SetupGDI setup_gdi;
		CL_SetupGL setup_gl;

		Application app;
		return app.main(args);
	}
};

CL_ClanApplication app(&Program::main);

