#include <ClanLib/core.h> 
#include <ClanLib/gui.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

class App
{
public:
	int main(const std::vector<CL_String> &args)
	{
		CL_ConsoleWindow console("Console");

		try
		{
			CL_ResourceManager resources;  
			resources.load("../../../Resources/GUIThemeAero/resources.xml");

			CL_GUIManager gui;

			CL_GUIWindowManagerSystem wm; 
			gui.set_window_manager(wm);

			CL_GUIThemeDefault theme;
			theme.set_resources(resources);
			gui.set_theme(theme);
			gui.set_css_document("../../../Resources/GUIThemeAero/theme.css");
 
			CL_DisplayWindowDescription win_desc;
			win_desc.set_allow_resize(true);
			win_desc.set_title("LineEdit test app.");
			win_desc.set_position(CL_Rect(200,200,600,600), false);
			CL_Window root(&gui, win_desc);
			root.func_close().set(this, &App::on_close, &root);

 			lineedit1 = new CL_LineEdit(&root);
			lineedit1->set_geometry(CL_Rect(20, 20, 200, 40));
 			lineedit1->set_text("Random text, random text"); 
			lineedit1->set_geometry(CL_Rect(CL_Point(100,100), CL_Size(169, 21)));

			CL_AcceleratorTable accel_table;
			CL_AcceleratorKey key_ctrlP(CL_KEY_P, CL_KEY_CONTROL);
			key_ctrlP.func_pressed().set(this, &App::on_accelerator);
 
			CL_AcceleratorKey key_menuD(CL_KEY_D, CL_KEY_MENU);
			key_menuD.func_pressed().set(this, &App::on_accelerator);

			accel_table.add_accelerator(key_menuD);
			accel_table.add_accelerator(key_ctrlP);

			gui.set_accelerator_table(accel_table);

			gui.exec();

			delete lineedit1; 
		}
		catch (CL_Exception e)
		{
 			CL_Console::write_line(e.message);
  			console.display_close_message();
		}
 
		return 0;
 	}

	CL_LineEdit *lineedit1;

	bool on_close(CL_Window *win)
	{
		win->exit_with_code(0);
		return true;
	}

	void on_timer()
	{
		CL_Console::write_line("timer expired");
	}

	void on_accelerator(CL_GUIMessage mgs, CL_AcceleratorKey key)
	{
		if (key.get_id() == CL_KEY_D)
		{
			lineedit1->set_text("alt-d pressed");
		}
		else if (key.get_id() == CL_KEY_P)
		{
			lineedit1->set_text("ctrl-p pressed");
		}
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

