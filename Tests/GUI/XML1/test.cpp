#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
using namespace clan;

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

			ResourceManager resources("resources.xml");
			ResourceManager resources2("../../../Resources/GUIThemeLuna/resources.xml");
			resources.add_resources(resources2);

			GUIThemeDefault theme;
			theme.set_resources(resources);
			gui.set_theme(theme);
			gui.set_css_document("theme.css");

			GUITopLevelDescription window_desc;
			window_desc.set_allow_resize(true);
			window_desc.set_position(RectPS(500, 600, 270, 140), false);
			Window window(&gui, window_desc);
//			window.set_id_name("mainmenu");
			window.func_close().set(this, &App::on_close, &window);

			GUILayoutCorners layout;
			window.set_layout(layout);

			window.create_components("dialog.xml");

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
		OpenGLTarget display_target;

		// Start the Application
		App app;
		int retval = app.main(args);
		return retval;
	}
};

// Instantiate Application, informing it where the Program is located
Application app(&Program::main);

