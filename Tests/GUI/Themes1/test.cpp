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
			displaywindow_desc.set_title("Themes Test Application");
			displaywindow_desc.set_size(Size(640, 480), true);

			DisplayWindow displaywindow(displaywindow_desc);

			GUIManager gui;

//			ResourceManager resources("../../../Resources/GUIStyleMirage/resources.xml");
			ResourceManager resources("../../../Resources/GUIThemeLuna/resources.xml");

			GUIWindowManagerTexture wm(displaywindow);
			gui.set_window_manager(wm);

			GUIThemeDefault theme;
			theme.set_resources(resources);
			gui.set_theme(theme);
//			gui.set_css_document("../../../Resources/GUIStyleMirage/theme.css");
			gui.set_css_document("../../../Resources/GUIThemeLuna/theme.css");

			Window window1(&gui, GUITopLevelDescription(Rect(10, 10, 510, 410), false));

			Window window2(&gui, GUITopLevelDescription(Rect(50, 50, 540, 440), false));
			window2.func_close().set(this, &App::on_close, &window1);

			Rect client_area_pos = window2.get_client_area();

			PushButton button1(&window2);
			button1.set_geometry(Rect(0, 0, 160, 25).translate(client_area_pos));
			button1.set_text("Button 1");
			button1.set_default(true);
			button1.set_enabled(false); // workaround parts not being updated on set_default
			button1.set_enabled(true); // workaround parts not being updated on set_default

			PushButton button2(&window2);
			button2.set_geometry(Rect(0, 30, 160, 55).translate(client_area_pos));
			button2.set_text("Button 2");

			RadioButton radio1(&window2);
			radio1.set_geometry(Rect(0, 90, 160, 100).translate(client_area_pos));
			radio1.set_text("RadioButton Option 1");
			radio1.set_group_name("radio1");

			RadioButton radio2(&window2);
			radio2.set_geometry(Rect(0, 110, 160, 125).translate(client_area_pos));
			radio2.set_text("RadioButton Option 2");
			radio2.set_group_name("radio1");

			CheckBox checkbox1(&window2);
			checkbox1.set_geometry(Rect(0, 150, 160, 165).translate(client_area_pos));
			checkbox1.set_text("CheckBox 1");

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

