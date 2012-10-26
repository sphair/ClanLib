#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
using namespace clan;

class App
{
public:
	int main(const std::vector<std::string> &args)
	{
		try
		{
			DisplayWindowDescription displaywindow_desc;
			displaywindow_desc.set_allow_resize(true);
			displaywindow_desc.set_title("Window Texture Test Application");
			displaywindow_desc.set_size(Size(640, 480), true);

			DisplayWindow displaywindow(displaywindow_desc);

			ResourceManager resources("../../../Resources/GUIThemeAero/resources.xml"); 

			GUIThemeDefault theme;
			theme.set_resources(resources);

  			GUIWindowManagerTexture wm(displaywindow);
  			//GUIWindowManagerSystem wm;
 
			GUIManager gui;
			gui.set_window_manager(wm);
			gui.set_theme(theme);
			gui.set_css_document("../../../Resources/GUIThemeAero/theme.css");

			GUITopLevelDescription window_desc1;
			window_desc1.set_title("Window 1");
			window_desc1.set_position(Rect(10, 10, 630, 100), false);
			Window window1(&gui, window_desc1);

			GUITopLevelDescription window_desc2;
			window_desc2.set_title("Window 2");
			window_desc2.set_position(Rect(10, 160, 630, 470), false);
			Window window2(&gui, window_desc2);

			PushButton resize(&window2);
			resize.set_geometry(Rect(window2.get_client_area().get_top_left(), Size(100, 25)));
			resize.set_text("Resize window");
			resize.func_clicked().set(this, &App::on_button_resize, &window2);

			Slot slot = displaywindow.sig_window_close().connect(this, &App::on_close, &gui);

			gui.exec();
		}
		catch (Exception e)
		{
			ConsoleWindow console("Console");
 			Console::write_line(e.message);
			console.display_close_message();
		}

		return 0;
	}

	void on_close(GUIManager *guimanager)
	{
		guimanager->exit_with_code(0);
	}

	void on_button_resize(Window *window)
	{
		Rect r = window->get_geometry();
		window->set_geometry(Rect(r.left, r.top, r.right - 20, r.bottom - 20));
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

		// Start the Application
		App app;
		int retval = app.main(args);
		return retval;
	}
};

// Instantiate Application, informing it where the Program is located
Application app(&Program::main);

