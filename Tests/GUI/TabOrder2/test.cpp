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
		try
		{
			GUIManager gui;

			DisplayCache resources("../../../Resources/GUIThemeLuna/resources.xml");

			GUIWindowManagerSystem wm;
			gui.set_window_manager(wm);

			GUIThemeDefault theme;
			theme.set_resources(resources);
			gui.set_theme(theme); 
			gui.set_css_document("../../../Resources/GUIThemeLuna/theme.css");

			DisplayWindowDescription win_desc;
			win_desc.set_allow_resize(true);
			win_desc.set_title("Button WM System Test Application");
			win_desc.set_position(RectPS(200, 200, 640, 520), false);
			Window window(&gui, win_desc);
			window.func_close().set(this, &App::on_close, &window);
			
			LineEdit lineedit(&window);
			lineedit.set_geometry(RectPS(300,300,100,21));

			PushButton button(&window);
			button.set_geometry(RectPS(640-10-80-10-80, 480-10-21, 80, 21));
			button.set_text("Ok");
			button.set_default(true);
			
			PushButton button2(&window);
			button2.set_geometry(RectPS(640-10-80, 480-10-21, 80, 21));
			button2.set_text("Cancel");
			
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

	void on_button_clicked(PushButton *button)
	{
		label->set_text("You clicked " + button->get_text());
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

