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
		GUIManager gui;

		GUIWindowManagerSystem wm;
		gui.set_window_manager(wm);
		gui.add_resources(clan::XMLResourceDocument("../../../Resources/GUIThemeAero/resources.xml"));
		gui.add_theme("../../../Resources/GUIThemeAero/theme.css");
		DisplayWindowDescription win_desc;
		win_desc.set_allow_resize(true);
		win_desc.set_title("Button WM System Test Application");
		win_desc.set_position(Rect(200, 200, 540, 440), false);

		Window window(&gui, win_desc);
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

		label = new Label(&window);
		label->set_geometry(Rect(10, 160, 330, 180));
		label->set_text("Click the buttons");

		gui.exec();

		return 0;
	}

	void on_button_clicked(PushButton *button)
	{
		label->set_text("You clicked " + button->get_text());
		label->set_text_color(Colorf::red);
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

