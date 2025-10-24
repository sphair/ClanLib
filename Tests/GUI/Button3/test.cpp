#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
//#include <ClanLib/swrender.h>
#include "windows_theme.h"

class App
{
public:
	CL_Label *label;

	int main(const std::vector<CL_String> &args)
	{
		CL_ConsoleWindow console("Console");

		try
		{
			CL_GUIWindowManagerSystem wm;

			CL_GUIManager gui;
			gui.set_window_manager(wm);

			CL_ResourceManager resources("resources.xml");
			CL_ResourceManager resources2("../../../Resources/GUIThemeLuna/resources.xml");
			resources.add_resources(resources2);

			CL_GUIThemeDefault theme;
			theme.set_resources(resources);

			CL_GUITheme windows_theme( new WindowsTheme(&theme) );
			gui.set_theme(windows_theme);
			gui.set_css_document("theme.css");

			CL_Window window(&gui, CL_GUITopLevelDescription(CL_Rect(200, 200, 640, 480), false));
			window.set_id_name("mainmenu");
			window.func_close().set(this, &App::on_close, &window);

			CL_PushButton button(&window);
			button.set_id_name("SuperFancyButton");
			button.set_geometry(CL_RectPS(20, 20, 313, 39));
			button.set_text("Hello Test");

			CL_PushButton button2(&window);
			button2.set_id_name("SuperFancyButton");
			button2.set_geometry(CL_RectPS(20, 80, 313, 39));

			CL_PushButton button3(&window);
			button3.set_id_name("SuperFancyButton");
			button3.set_geometry(CL_RectPS(20, 140, 313, 39));

			gui.exec();
		}
		catch (CL_Exception e)
		{
			CL_Console::write_line(e.message);
			console.display_close_message();
		}

		return 0;
	}

	bool on_close(CL_Window *win)
	{
		win->exit_with_code(0);
		return true;
	}
};

// This is the Program class that is called by CL_ClanApplication
class Program
{
public:
	static int main(const std::vector<CL_String> &args)
	{
		CL_SetupCore setup_core;
		CL_SetupDisplay setup_display;
		CL_SetupGL setup_gl;
		//CL_SetupSWRender setup_swrender;

		// Start the Application
		App app;
		int retval = app.main(args);
		return retval;
	}
};

// Instantiate CL_ClanApplication, informing it where the Program is located
CL_ClanApplication app(&Program::main);

