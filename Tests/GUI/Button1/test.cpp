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
			gui.set_css_document("theme.css");

			CL_DisplayWindowDescription win_desc;
			win_desc.set_allow_resize(true);
			win_desc.set_title("Button WM System Test Application");
			win_desc.set_position(CL_Rect(200, 200, 540, 440), false);
			CL_Window window(&gui, win_desc);
			window.func_close().set(this, &App::on_close, &window);

			CL_PushButton button(&window);
			button.set_geometry(CL_Rect(10, 10, 160, 50));
			button.func_clicked().set(this, &App::on_button_clicked, &button);
			button.set_text("Button Normal");

			CL_PushButton button_disabled(&window);
			button_disabled.set_geometry(CL_Rect(10, 60, 160, 100));
			button_disabled.func_clicked().set(this, &App::on_button_clicked, &button_disabled);
			button_disabled.set_text("Button Disabled");
			button_disabled.set_enabled(false);

			CL_PushButton button_ugly_flat(&window);
			button_ugly_flat.set_geometry(CL_Rect(170, 110, 330, 150));
			button_ugly_flat.func_clicked().set(this, &App::on_button_clicked, &button_ugly_flat);
			button_ugly_flat.set_flat(true);
			button_ugly_flat.set_class_name("ugly");
			button_ugly_flat.set_text("Button Ugly Flat");

			CL_PushButton button_ugly(&window);
			button_ugly.set_geometry(CL_Rect(170, 10, 330, 50));
			button_ugly.func_clicked().set(this, &App::on_button_clicked, &button_ugly);
			button_ugly.set_flat(false);
			button_ugly.set_class_name("ugly");
			button_ugly.set_text("Button Ugly");

			CL_PushButton button_ugly_disabled(&window);
			button_ugly_disabled.set_geometry(CL_Rect(170, 60, 330, 100));
			button_ugly_disabled.func_clicked().set(this, &App::on_button_clicked, &button_ugly_disabled);
			button_ugly_disabled.set_class_name("ugly");
			button_ugly_disabled.set_text("Button Ugly Disabled");
			button_ugly_disabled.set_enabled(false);

			label = new CL_Label(&window);
			label->set_geometry(CL_Rect(10, 160, 330, 180));
			label->set_text("Click the buttons");

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

