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
			resources.load("../../../Resources/GUIThemeAero/resources.xml");

			CL_GUIManager gui;

			CL_GUIWindowManagerSystem wm;
			gui.set_window_manager(wm);

			CL_GUIThemeDefault theme;
			theme.set_resources(resources);
			gui.set_theme(theme);
			gui.set_css_document("../../../Resources/GUIThemeAero/theme.css");

			CL_DisplayWindowDescription win_desc;
			win_desc.set_allow_resize(true);
			win_desc.set_title("RadioButton test app.");
			win_desc.set_position(CL_Rect(200,200,600,600), false);
			CL_Window root(&gui, win_desc);
			root.func_close().set(this, &App::on_close, &root);

			CL_RadioButton radioA1(&root);
			radioA1.set_geometry(CL_RectPS(20, 20, 200, 17));
			radioA1.set_text("Group A - 1");
			radioA1.set_group_name("Group A");
			radioA1.set_selected(true);
			radioA1.set_focus(true);

			CL_RadioButton radioA2(&root);
			radioA2.set_geometry(CL_RectPS(20, 40, 200, 17));
 			radioA2.set_text("Group A - 2");
			radioA2.set_group_name("Group A");

			CL_RadioButton radioA3(&root);
			radioA3.set_geometry(CL_RectPS(20, 60, 200, 17));
			radioA3.set_text("Group A - 3");
			radioA3.set_group_name("Group A");


			CL_RadioButton radioB1(&root);
			radioB1.set_geometry(CL_RectPS(20, 160, 200, 17));
			radioB1.set_text("Group B - 1");
			radioB1.set_group_name("Group B");
 
			CL_RadioButton radioB2(&root);
			radioB2.set_geometry(CL_RectPS(20, 180, 200, 17));
			radioB2.set_text("Group B - 2");
			radioB2.set_group_name("Group B");


			CL_RadioButton radioC1(&root);
			radioC1.set_geometry(CL_RectPS(120, 160, 200, 17));
			radioC1.set_text("Group C - 1");
			radioC1.set_group_name("Group C");

			CL_RadioButton radioC2(&root);
			radioC2.set_geometry(CL_RectPS(120, 180, 200, 17));
			radioC2.set_text("Group C - 2");
			radioC2.set_group_name("Group C");

			CL_RadioButton radioC3(&root);
			radioC3.set_geometry(CL_RectPS(120, 200, 200, 17));
			radioC3.set_text("Group C - 3");
			radioC3.set_group_name("Group C");

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

		// Start the Application
		App app;
		int retval = app.main(args);
		return retval;
	}
};

// Instantiate CL_ClanApplication, informing it where the Program is located
CL_ClanApplication app(&Program::main);

