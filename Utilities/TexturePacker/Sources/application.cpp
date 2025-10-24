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

			CL_String resource_filename(cl_text("../../Resources/GUIThemeAero/resources.xml"));
			CL_String theme_filename(cl_text("theme.css"));
			if (!CL_FileHelp::file_exists(resource_filename))
			{
				resource_filename = cl_text("../../Resources/GUIThemeBasic/resources.xml");
				theme_filename = cl_text("theme_basic.css");
			}

			CL_ResourceManager resources(resource_filename);
			local_resources.add_resources(resources);

			CL_GUIThemeDefault theme;
			theme.set_resources(local_resources);

			CL_GUIWindowManagerSystem wm;

			CL_GUIManager gui;
			gui.set_window_manager(wm);
			gui.set_theme(theme);
			gui.set_css_document(theme_filename);

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

