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
		ConsoleWindow console("Console");

		try
		{
			ResourceManager resources;  
			resources.load("../../../Resources/GUIThemeAero/resources.xml");

			GUIManager gui;

			GUIWindowManagerSystem wm; 
			gui.set_window_manager(wm);

			GUIThemeDefault theme;
			theme.set_resources(resources);
			gui.set_theme(theme);
			gui.set_css_document("../../../Resources/GUIThemeAero/theme.css");
 
			DisplayWindowDescription win_desc;
			win_desc.set_allow_resize(true);
			win_desc.set_title("LineEdit test app.");
			win_desc.set_position(Rect(200,200,600,600), false);
			Window root(&gui, win_desc);
			root.func_close().set(this, &App::on_close, &root);

 			lineedit1 = new LineEdit(&root);
			lineedit1->set_geometry(Rect(20, 20, 200, 40));
 			lineedit1->set_text("Random text, random text"); 
			lineedit1->set_geometry(Rect(Point(100,100), Size(169, 21)));

			AcceleratorTable accel_table;
			AcceleratorKey key_ctrlP(KEY_P, KEY_CONTROL);
			key_ctrlP.func_pressed().set(this, &App::on_accelerator);
 
			AcceleratorKey key_menuD(KEY_D, KEY_MENU);
			key_menuD.func_pressed().set(this, &App::on_accelerator);

			accel_table.add_accelerator(key_menuD);
			accel_table.add_accelerator(key_ctrlP);

			gui.set_accelerator_table(accel_table);

			gui.exec();

			delete lineedit1; 
		}
		catch (Exception e)
		{
 			Console::write_line(e.message);
  			console.display_close_message();
		}
 
		return 0;
 	}

	LineEdit *lineedit1;

	bool on_close(Window *win)
	{
		win->exit_with_code(0);
		return true;
	}

	void on_timer()
	{
		Console::write_line("timer expired");
	}

	void on_accelerator(GUIMessage mgs, AcceleratorKey key)
	{
		if (key.get_id() == KEY_D)
		{
			lineedit1->set_text("alt-d pressed");
		}
		else if (key.get_id() == KEY_P)
		{
			lineedit1->set_text("ctrl-p pressed");
		}
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

