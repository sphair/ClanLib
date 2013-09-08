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
		GUIWindowManagerSystem wm;

		GUIManager gui;
		gui.set_window_manager(wm);

		gui.add_resources(clan::XMLResourceDocument("../../../Resources/GUIThemeAero/resources.xml"));
		gui.add_theme("../../../Resources/GUIThemeAero/theme.css");

		GUITopLevelDescription window_desc;
		window_desc.set_allow_resize(true);
		window_desc.set_size(Size(500, 600), false);
		Window window(&gui, window_desc);
		window.func_close().set(this, &App::on_close, &window);

		GUILayoutCorners layout;
		window.set_layout(layout);

		window.create_components("dialog.xml");

		gui.exec();

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

