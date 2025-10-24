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
			resources.load("../../../Resources/GUIThemeLuna/resources.xml");

			CL_GUIManager gui;

			CL_GUIWindowManagerSystem wm;
			gui.set_window_manager(wm);

			CL_GUIThemeDefault theme;
			theme.set_resources(resources);

			gui.set_theme(theme);
			gui.set_css_document("../../../Resources/GUIThemeLuna/theme.css");

			CL_DisplayWindowDescription win_desc;
			win_desc.set_allow_resize(true);
			win_desc.set_title("Label test app."); 
			win_desc.set_position(CL_Rect(400,400,800,650), false);
			CL_Window root(&gui, win_desc);			
			root.func_close().set(this, &App::on_close, &root); 
   
			label = new CL_Label(&root);
			label->set_geometry(CL_Rect(20, 20, 200, 40));
			label->set_text("foobar");

			CL_Timer timer;
			timer.func_expired().set(this, &App::on_timer);
			timer.start(200);

			CL_AcceleratorTable at;
			gui.exec(at);
		}
		catch (CL_Exception e)
		{
 			CL_Console::write_line(e.message);
			console.display_close_message();
		}

		return 0;
	}

	void on_timer()
	{
		static int counter = 0;
		label->set_text(cl_format("foobar %1", counter));
		counter++;
	}

	void on_close(CL_Window *win)
	{
		win->exit_with_code(0);
	}

	CL_Label *label;
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

