#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

class App
{
public:
	CL_Label *main_label;

	int main(const std::vector<CL_String> &args)
	{
		try
		{
			CL_GUIManager gui;

			CL_ResourceManager gui_resources("../../../Resources/GUIThemeAero/resources.xml");
			CL_ResourceManager app_resources("resources.xml");

			CL_GUIWindowManagerSystem wm;
			gui.set_window_manager(wm);

			CL_GUIThemeDefault theme;
			theme.set_resources(gui_resources);
			gui.set_theme(theme); 
			gui.set_css_document("theme.css");

			CL_DisplayWindowDescription win_desc;
			win_desc.set_allow_resize(true);
			win_desc.set_title("Button WM System Test Application");
			win_desc.set_position(CL_Rect(200, 200, 940, 640), false);
			CL_Window window(&gui, win_desc);
			window.func_close().set(this, &App::on_close, &window);

			CL_GraphicContext gc = window.get_gc();

			CL_Font_Sprite font_sprite(gc, "ClanFont", &app_resources);

			// Set the gui named font
			CL_FontDescription clanlib_font_desc;
			clanlib_font_desc.set_typeface_name("ClanLib Font");
			gui.register_font(font_sprite, clanlib_font_desc);

			CL_Label label(&window);
			label.set_geometry(CL_Rect(10, 160, 330, 180));
			label.set_text("Click the button");
			main_label = &label;

			CL_PushButton button(&window);
			button.set_geometry(CL_Rect(10, 10, 400, 100));
			button.func_clicked().set(this, &App::on_button_clicked, &button);
			button.set_class_name("redtext");
			button.set_text("Button Normal");

			CL_LineEdit lineedit1(&window);
			lineedit1.set_geometry(CL_Rect(20, 300, 700, 380));
			lineedit1.set_class_name("greentext");
			lineedit1.set_text("Select, Click and Edit me :)"); 

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

private:
	void on_button_clicked(CL_PushButton *button)
	{
		main_label->set_text("You clicked " + button->get_text());
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

