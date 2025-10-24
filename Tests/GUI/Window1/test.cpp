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
		try
		{
			CL_DisplayWindowDescription displaywindow_desc;
			displaywindow_desc.set_allow_resize(true);
			displaywindow_desc.set_title("Window Texture Test Application");
			displaywindow_desc.set_size(CL_Size(640, 480), true);

			CL_DisplayWindow displaywindow(displaywindow_desc);

			CL_ResourceManager resources("../../../Resources/GUIThemeAero/resources.xml"); 

			CL_GUIThemeDefault theme;
			theme.set_resources(resources);

  			CL_GUIWindowManagerTexture wm(displaywindow);
  			//CL_GUIWindowManagerSystem wm;
 
			CL_GUIManager gui;
			gui.set_window_manager(wm);
			gui.set_theme(theme);
			gui.set_css_document("../../../Resources/GUIThemeAero/theme.css");

			CL_GUITopLevelDescription window_desc1;
			window_desc1.set_title("Window 1");
			window_desc1.set_position(CL_Rect(10, 10, 630, 100), false);
			CL_Window window1(&gui, window_desc1);

			CL_GUITopLevelDescription window_desc2;
			window_desc2.set_title("Window 2");
			window_desc2.set_position(CL_Rect(10, 160, 630, 470), false);
			CL_Window window2(&gui, window_desc2);

			CL_PushButton resize(&window2);
			resize.set_geometry(CL_Rect(window2.get_client_area().get_top_left(), CL_Size(100, 25)));
			resize.set_text("Resize window");
			resize.func_clicked().set(this, &App::on_button_resize, &window2);

			CL_Slot slot = displaywindow.sig_window_close().connect(this, &App::on_close, &gui);

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

	void on_close(CL_GUIManager *guimanager)
	{
		guimanager->exit_with_code(0);
	}

	void on_button_resize(CL_Window *window)
	{
		CL_Rect r = window->get_geometry();
		window->set_geometry(CL_Rect(r.left, r.top, r.right - 20, r.bottom - 20));
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

