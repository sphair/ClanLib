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
		ConsoleWindow console("Console");

		try
		{
			DisplayCache resources("../../../Resources/GUIThemeLuna/resources.xml");

			GUIManager gui;

			GUIWindowManagerSystem wm;
			gui.set_window_manager(wm);

			GUIThemeDefault theme;
			theme.set_resources(resources);
			gui.set_theme(theme);
			gui.set_css_document("theme.css");

			DisplayWindowDescription win_desc;
			win_desc.set_allow_resize(true);
			win_desc.set_title("Slider test app."); 
			win_desc.set_position(Rect(200,200,600,600), false);
			Window root(&gui, win_desc);			
			root.func_close().set(this, &App::on_close, &root); 
     
			Slider slider1(&root);
			slider1.set_geometry(Rect(20, 20, 200, 40));
			slider1.set_ranges(0, 10, 10, 1);
   
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

		// Start the Application
		App app;
		int retval = app.main(args);
		return retval;
	}
};

// Instantiate Application, informing it where the Program is located
Application app(&Program::main);

