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
			DisplayWindowDescription displaywindow_desc;
			displaywindow_desc.set_allow_resize(true);
			displaywindow_desc.set_title("Button WM Texture Test Application");
			displaywindow_desc.set_size(Size(640, 480), true);

			DisplayWindow displaywindow(displaywindow_desc);

//			GUIWindowManagerTexture wm(displaywindow);
			GUIWindowManagerSystem wm;

			GUIManager gui;
			gui.set_window_manager(wm);

//			ResourceManager resources("../../../Resources/GUIThemeLuna/resources.xml");
			ResourceManager resources("resources.xml");

			GUIThemeDefault theme;
			theme.set_resources(resources);
			gui.set_theme(theme);
//			gui.set_css_document("../../../Resources/GUIThemeLuna/theme.css");
			gui.set_css_document("theme.css");

			Window window(&gui, GUITopLevelDescription(Rect(200, 200, 540, 440), false));
			window.func_close().set(this, &App::on_close, &window);

			PushButton button(&window);
			button.set_geometry(Rect(10, 10, 160, 50));
			button.func_clicked().set(this, &App::on_button_clicked, &button);
			button.set_text("Button Normal");

			PushButton button_disabled(&window);
			button_disabled.set_geometry(Rect(10, 60, 160, 100));
			button_disabled.func_clicked().set(this, &App::on_button_clicked, &button_disabled);
			button_disabled.set_text("Button Disabled");
			button_disabled.set_enabled(false);

			PushButton button_ugly_flat(&window);
			button_ugly_flat.set_geometry(Rect(170, 110, 330, 150));
			button_ugly_flat.func_clicked().set(this, &App::on_button_clicked, &button_ugly_flat);
			button_ugly_flat.set_flat(true);
			button_ugly_flat.set_class_name("ugly");
			button_ugly_flat.set_text("Button Ugly Flat");

			PushButton button_ugly(&window);
			button_ugly.set_geometry(Rect(170, 10, 330, 50));
			button_ugly.func_clicked().set(this, &App::on_button_clicked, &button_ugly);
			button_ugly.set_flat(false);
			button_ugly.set_class_name("ugly");
			button_ugly.set_text("Button Ugly");

			PushButton button_ugly_disabled(&window);
			button_ugly_disabled.set_geometry(Rect(170, 60, 330, 100));
			button_ugly_disabled.func_clicked().set(this, &App::on_button_clicked, &button_ugly_disabled);
			button_ugly_disabled.set_class_name("ugly");
			button_ugly_disabled.set_text("Button Ugly Disabled");
			button_ugly_disabled.set_enabled(false);

			label = new Label(&window);
			label->set_geometry(Rect(10, 160, 330, 180));
			label->set_text("Click the buttons");

			gui.exec();
		}
		catch (Exception e)
		{
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

