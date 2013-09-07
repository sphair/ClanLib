#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

#include "calendar.h"

class App
{
public:
	clan::Label *label;

	int main(const std::vector<std::string> &args)
	{
		clan::GUIManager gui;

		clan::GUIWindowManagerSystem wm;
		gui.set_window_manager(wm);

		gui.add_resources(clan::XMLResourceDocument("../../../Resources/GUIThemeAero/resources.xml"));
		gui.add_resources(clan::XMLResourceDocument("resources.xml"));

		gui.add_theme("../../../Resources/GUIThemeAero/theme.css");
		gui.add_theme("theme.css");

		clan::DisplayWindowDescription win_desc;
		win_desc.set_allow_resize(true);
		win_desc.set_title("Calandar Test Application");
		win_desc.set_position(clan::Rect(200, 200, 800, 840), false);
		clan::Window window(&gui, win_desc);
		window.func_close().set(this, &App::on_close, &window);

		label = new clan::Label(&window);
		label->set_geometry(clan::RectPS(11, 11, 150, 23));
		label->set_text("Date: none selected.");

		CalendarComponent calendar(&window);
		calendar.func_selection_changed().set(this, &App::on_date_selection_changed, &calendar);
		calendar.set_geometry(clan::Rect(clan::Point(11, 70), calendar.get_css_size()));

		gui.exec();

		return 0;
	}

	void on_date_selection_changed(CalendarComponent *cal)
	{
		clan::DateTime date = cal->get_selected_date();
		label->set_text(clan::string_format("Date: %1.%2.%3", date.get_day(), date.get_month(), date.get_year()));
	}

	bool on_close(clan::Window *win)
	{
		win->exit_with_code(0);
		return true;
	}
};

// This is the Program class that is called by clan::ClanApplication
class Program
{
public:
	static int main(const std::vector<std::string> &args)
	{
		clan::SetupCore setup_core;
		clan::SetupDisplay setup_display;
		clan::SetupGL setup_gl;

		// Start the Application
		App app;
		int retval = app.main(args);
		return retval;
	}
};

// Instantiate clan::Application, informing it where the Program is located
clan::Application app(&Program::main);
