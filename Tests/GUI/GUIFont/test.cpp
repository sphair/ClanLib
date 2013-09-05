#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
using namespace clan;

class App
{
public:
	Label *main_label;

	int main(const std::vector<std::string> &args)
	{
		try
		{
			GUIManager gui;

			clan::ResourceManager app_resources = clan::XMLResourceManager::create(clan::XMLResourceDocument("resources.xml"));

			GUIWindowManagerSystem wm;
			gui.set_window_manager(wm);

			gui.add_resources(clan::XMLResourceDocument("../../../Resources/GUIThemeAero/resources.xml"));

			gui.add_theme("../../../Resources/GUIThemeAero/theme.css");
			gui.add_theme("../../../Tests/GUI/GUIFont/theme.css");

			DisplayWindowDescription win_desc;
			win_desc.set_allow_resize(true);
			win_desc.set_title("Button WM System Test Application");
			win_desc.set_position(Rect(200, 200, 940, 640), false);
			Window window(&gui, win_desc);
			window.func_close().set(this, &App::on_close, &window);

			Canvas canvas = window.get_canvas();

			clan::FontDescription font_desc("ClanFont");
			Font_Sprite font_sprite(Font_Sprite::resource(canvas, font_desc, app_resources));

			Label label(&window);
			label.set_geometry(Rect(10, 160, 330, 180));
			label.set_text("Click the button");
			main_label = &label;

			PushButton button(&window);
			button.set_geometry(Rect(10, 10, 400, 100));
			button.func_clicked().set(this, &App::on_button_clicked, &button);
			button.set_class("redtext", true);
			button.set_text("Button Normal");

			LineEdit lineedit1(&window);
			lineedit1.set_geometry(Rect(20, 300, 700, 380));
			lineedit1.set_class("greentext", true);
			lineedit1.set_text("Select, Click and Edit me :)"); 

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

private:
	void on_button_clicked(PushButton *button)
	{
		main_label->set_text("You clicked " + button->get_text());
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

