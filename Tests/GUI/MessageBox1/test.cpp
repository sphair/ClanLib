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

			CL_GUIWindowManagerSystem wm;
			gui.set_window_manager(wm);

			CL_GUIThemeDefault theme;
			theme.set_resources(resources);

			gui.set_theme(theme);
 			gui.set_css_document("../../../Resources/GUIThemeAero/theme.css");

			CL_DisplayWindowDescription win_desc;
			win_desc.set_allow_resize(true);
			win_desc.set_title("Label test app."); 
			win_desc.set_position(CL_Rect(400,400,800,650), false);
 
			mainwin = new CL_Window(&gui, win_desc);			
			mainwin->func_close().set(this, &App::on_close, mainwin); 
   
			CL_PushButton button(mainwin);
			button.set_geometry(CL_RectPS(11, 11, 73, 23));
			button.func_clicked().set(this, &App::on_button_clicked);

			gui.exec();
		}
		catch (CL_Exception e)
		{
 			CL_Console::write_line(e.message);
			console.display_close_message();
		}

		return 0;
	}

	bool on_close(CL_Window *win)
	{
		win->exit_with_code(0);
		return true;
	}

	void on_button_clicked()
	{
		CL_MessageBoxResult res;

		res = cl_message_box(&gui,
			"MSDN quote of the day!", 
			"WS_CLIPCHILDREN excludes the area occupied by child windows when drawing occurs within the parent window.",
			cl_mb_buttons_ok, 
			cl_mb_icon_error);

		res = cl_message_box(mainwin,
			"MSDN quote of the day!", 
			"WS_CLIPCHILDREN excludes the area occupied by child windows when drawing occurs within the parent window.",
			cl_mb_buttons_ok, 
			cl_mb_icon_error);

		res = cl_message_box(mainwin,
			"Warning!", 
			"Don't do that again or I'll tell your mama!",
			cl_mb_buttons_ok_cancel,
			cl_mb_icon_warning);

		res = cl_message_box(mainwin,
			"Question!", 
			"Would you like to fries with that?",
			cl_mb_buttons_yes_no,
			cl_mb_icon_question);

		res = cl_message_box(mainwin,
			"Very informative!", 
			"Oh joy!",
			cl_mb_buttons_yes_no_cancel,
			cl_mb_icon_question);

	}

	CL_GUIManager gui;
	CL_Window *mainwin;
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

