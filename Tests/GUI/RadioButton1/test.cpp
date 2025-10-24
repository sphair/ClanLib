#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

class App
{
public:
	int main(const std::vector<CL_String> &args)
	{
		CL_ConsoleWindow console("Console");

		try
		{
			CL_ResourceManager resources;
			resources.load("../../../Resources/GUIThemeLuna/resources.xml");

			CL_GUIManager gui;

			CL_GUIWindowManagerSystem wm;
			gui.set_window_manager(wm);

			CL_GUIThemeDefault theme;
			theme.set_resources(resources);
			gui.set_theme(theme);
			gui.set_css_document("../../../Resources/GUIThemeLuna/theme.css");

			CL_DisplayWindowDescription win_desc;
			win_desc.set_allow_resize(true);
			win_desc.set_title("RadioButton test app.");
			win_desc.set_position(CL_Rect(200,200,600,600), false);
			CL_Window root(&gui, win_desc);
			root.func_close().set(this, &App::on_close, &root);

			CL_RadioButton radio1(&root);
			radio1.set_geometry(CL_Rect(20, 20, 200, 30));
			radio1.set_id_name("A1");
			radio1.set_text("group A 1");
			radio1.set_group_name("group A");

			CL_RadioButton radio2(&root);
			radio2.set_geometry(CL_Rect(20, 40, 200, 50));
			radio2.set_id_name("A2");
			radio2.set_text("group A 2");
			radio2.set_group_name("group A");

			CL_RadioButton radio3(&root);
			radio3.set_geometry(CL_Rect(20, 160, 200, 170));
			radio3.set_id_name("B1");
			radio3.set_text("group B 1");
			radio3.set_group_name("group B");

			CL_RadioButton radio4(&root);
			radio4.set_geometry(CL_Rect(20, 180, 200, 190));
			radio4.set_id_name("B2");
			radio4.set_text("group B 2");
			radio4.set_group_name("group B");

			gui.exec();
		}
		catch (CL_Exception e)
		{
 			CL_Console::write_line(e.message);
			console.display_close_message();
		}

		return 0;
	}

	void on_close(CL_Window *win)
	{
		win->exit_with_code(0);
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

		// Start the Application
		App app;
		int retval = app.main(args);
		return retval;
	}
};

// Instantiate CL_ClanApplication, informing it where the Program is located
CL_ClanApplication app(&Program::main);

