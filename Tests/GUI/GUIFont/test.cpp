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

			ResourceManager gui_resources("../../../Resources/GUIThemeAero/resources.xml");
			ResourceManager app_resources("resources.xml");

			GUIWindowManagerSystem wm;
			gui.set_window_manager(wm);

			GUIThemeDefault theme;
			theme.set_resources(gui_resources);
			gui.set_theme(theme); 
			gui.set_css_document("theme.css");

			DisplayWindowDescription win_desc;
			win_desc.set_allow_resize(true);
			win_desc.set_title("Button WM System Test Application");
			win_desc.set_position(Rect(200, 200, 940, 640), false);
			Window window(&gui, win_desc);
			window.func_close().set(this, &App::on_close, &window);

			GraphicContext gc = window.get_gc();

			Font_Sprite font_sprite(gc, "ClanFont", &app_resources);

			// Set the gui named font
			FontDescription clanlib_font_desc;
			clanlib_font_desc.set_typeface_name("ClanLib Font");
			gui.register_font(font_sprite, clanlib_font_desc);

			Label label(&window);
			label.set_geometry(Rect(10, 160, 330, 180));
			label.set_text("Click the button");
			main_label = &label;

			PushButton button(&window);
			button.set_geometry(Rect(10, 10, 400, 100));
			button.func_clicked().set(this, &App::on_button_clicked, &button);
			button.set_class_name("redtext");
			button.set_text("Button Normal");

			LineEdit lineedit1(&window);
			lineedit1.set_geometry(Rect(20, 300, 700, 380));
			lineedit1.set_class_name("greentext");
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

