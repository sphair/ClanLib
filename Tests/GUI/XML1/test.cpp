#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

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
			gui.set_theme(theme);
			gui.set_css_document("theme.css");

			CL_GUITopLevelDescription window_desc;
			window_desc.set_allow_resize(true);
			window_desc.set_position(CL_RectPS(500, 600, 270, 140), false);
			CL_Window window(&gui, window_desc);
//			window.set_id_name("mainmenu");
			window.func_close().set(this, &App::on_close, &window);

			CL_GUILayoutCorners layout;
			window.set_layout(layout);

			window.create_components("dialog.xml");

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
		CL_OpenGLTarget display_target;

		// Start the Application
		App app;
		int retval = app.main(args);
		return retval;
	}
};

// Instantiate CL_ClanApplication, informing it where the Program is located
CL_ClanApplication app(&Program::main);

