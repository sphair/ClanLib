#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

#include "calendar.h"

class App
{
public:
	CL_Label *label;

	int main(const std::vector<CL_String> &args)
	{
		try
		{
			CL_GUIManager gui;

			CL_ResourceManager resources("resources.xml");
			CL_ResourceManager resources2("../../../Resources/GUIThemeLuna/resources.xml");
			resources.add_resources(resources2);

			CL_GUIWindowManagerSystem wm;
			gui.set_window_manager(wm);

			CL_GUIThemeDefault theme;
			theme.set_resources(resources);
			gui.set_theme(theme); 
			gui.set_css_document("theme.css");

			CL_DisplayWindowDescription win_desc;
			win_desc.set_allow_resize(true);
			win_desc.set_title("Button WM System Test Application");
			win_desc.set_position(CL_Rect(200, 200, 800, 840), false);
			CL_Window window(&gui, win_desc);
			window.func_close().set(this, &App::on_close, &window);

			label = new CL_Label(&window);
			label->set_geometry(CL_RectPS(11, 11, 150, 23));
			label->set_text("Date: none selected.");

			CL_CalendarComponent calendar(&window);
			calendar.func_selection_changed().set(this, &App::on_date_selection_changed, &calendar);
			calendar.set_geometry(CL_Rect(
				CL_Point(11, 70),
				calendar.get_preferred_size()));

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

	void on_date_selection_changed(CL_CalendarComponent *cal)
	{
		CL_DateTime date = cal->get_selected_date();
		label->set_text(cl_format("Date: %1.%2.%3", date.get_day(), date.get_month(), date.get_year()));
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
