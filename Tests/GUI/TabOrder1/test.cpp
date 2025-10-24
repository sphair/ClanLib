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
		try
		{
			CL_GUIManager gui;

			CL_ResourceManager resources("../../../Resources/GUIThemeLuna/resources.xml");

			CL_GUIWindowManagerSystem wm;
			gui.set_window_manager(wm);

			CL_GUIThemeDefault theme;
			theme.set_resources(resources);
			gui.set_theme(theme); 
			//gui.set_css_document("theme.css");
			gui.set_css_document("../../../Resources/GUIThemeLuna/theme.css");

			CL_DisplayWindowDescription win_desc;
			win_desc.set_allow_resize(true);
			win_desc.set_title("Button WM System Test Application");
			win_desc.set_position(CL_RectPS(200, 200, 320, 240), false);
			CL_Window window(&gui, win_desc);
			window.func_close().set(this, &App::on_close, &window);
	
			CL_PushButton button(&window);
			button.set_geometry(CL_RectPS(10, 10, 160, 40));
			button.set_text("Button 1");

			CL_PushButton button2(&window);
			button2.set_geometry(CL_RectPS(10, 50, 160, 40));
			button2.set_text("Button 2");
			button2.set_enabled(false);

			CL_PushButton button3(&window);
			button3.set_geometry(CL_RectPS(10, 90, 160, 40));
			button3.set_text("Button 3");
 
			CL_CheckBox check1(&window);
			check1.set_geometry(CL_RectPS(10, 130, 160, 21));
			check1.set_text("Check Box 1");

			CL_CheckBox check2(&window);
			check2.set_geometry(CL_RectPS(10, 160, 160, 21));
			check2.set_text("Check Box 2");
			check2.set_enabled(false);

			CL_CheckBox check3(&window);
			check3.set_geometry(CL_RectPS(10, 190, 160, 21));
			check3.set_text("Check Box 3");

			gui.exec();
		}
		catch (CL_Exception e)
		{
			CL_ConsoleWindow console("Console");
			CL_Console::write_line(e.message);
			console.display_close_message();
		}

		return 0;
	}

	void on_button_clicked(CL_PushButton *button)
	{
		label->set_text("You clicked " + button->get_text());
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

