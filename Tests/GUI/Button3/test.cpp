#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
using namespace clan;
//#include <ClanLib/d3d.h>
#include "windows_theme.h"

class App
{
public:
	Label *label;

	int main(const std::vector<std::string> &args)
	{
		ConsoleWindow console("Console");

		try
		{
			GUIWindowManagerSystem wm;

			GUIManager gui;
			gui.set_window_manager(wm);

			DisplayCache resources("resources.xml");
			DisplayCache resources2("../../../Resources/GUIThemeLuna/resources.xml");
			resources.add_resources(resources2);

			GUIThemeDefault theme;
			theme.set_resources(resources);

			GUITheme windows_theme( new WindowsTheme(&theme) );
			gui.set_theme(windows_theme);
			gui.set_css_document("theme.css");

			Window window(&gui, GUITopLevelDescription(Rect(200, 200, 640, 480), false));
			window.set_id_name("mainmenu");
			window.func_close().set(this, &App::on_close, &window);

			PushButton button(&window);
			button.set_id_name("SuperFancyButton");
			button.set_geometry(RectPS(20, 20, 313, 39));
			button.set_text("Hello Test");

			PushButton button2(&window);
			button2.set_id_name("SuperFancyButton");
			button2.set_geometry(RectPS(20, 80, 313, 39));

			PushButton button3(&window);
			button3.set_id_name("SuperFancyButton");
			button3.set_geometry(RectPS(20, 140, 313, 39));

			gui.exec();
		}
		catch (Exception e)
		{
			Console::write_line(e.message);
			console.display_close_message();
		}

		return 0;
	}

	bool on_close(Window *win)
	{
		win->exit_with_code(0);
		return true;
	}
};

// This is the Program class that is called by Application
class Program
{
public:
	static int main(const std::vector<std::string> &args)
	{
		SetupCore setup_core;
		SetupDisplay setup_display;
		SetupGL setup_gl;
		//SetupD3D setup_d3d;

		// Start the Application
		App app;
		int retval = app.main(args);
		return retval;
	}
};

// Instantiate Application, informing it where the Program is located
Application app(&Program::main);

