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
			resources.load("resources.xml");
			resources.load("../../../Resources/GUIThemeLuna/resources.xml");

			CL_GUIManager gui;

			CL_GUIWindowManagerSystem wm;
			gui.set_window_manager(wm);

			CL_GUIThemeDefault theme;
			theme.set_resources(resources);

			gui.set_theme(theme);
			gui.set_css_document("theme.css");

			CL_DisplayWindowDescription win_desc;
			win_desc.set_allow_resize(true);
			win_desc.set_title("Label test app."); 
			win_desc.set_position(CL_Rect(400,400,800,650), false);
			CL_Window root(&gui, win_desc);			
			root.func_close().set(this, &App::on_close, &root); 
   
			CL_Label label1(&root);
			label1.set_geometry(CL_Rect(20, 20, 200, 40));
			label1.set_class_name("yellow");
			label1.set_text("Yellow label ÅÄygj");

			CL_Label label2(&root);
			label2.set_geometry(CL_Rect(20, 60, 300, 100));

			CL_LineEdit input_name(&label2);
			CL_LineEdit input_password(&label2);
			input_name.set_geometry(CL_Size(100, 16));
			input_password.set_geometry(CL_Size(100, 16));
			input_name.set_visible(true, false);
			input_password.set_visible(true, false);

			CL_Font font(label1.get_gc(), "Verdana", -13);
			CL_SpanLayout span;
			span.add_text("Name: ", font, CL_Colorf::black);
			span.add_component(&input_name, 4);
			span.add_text("\nPassword: ", font, CL_Colorf::black);
			span.add_component(&input_password, 4);

			label2.set_span(span);
			label2.set_alignment(CL_Label::align_right);

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

