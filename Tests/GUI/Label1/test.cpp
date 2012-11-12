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
			resources.load("resources.xml");
			resources.load("../../../Resources/GUIThemeLuna/resources.xml");

			GUIManager gui;

			GUIWindowManagerSystem wm;
			gui.set_window_manager(wm);

			//GUIThemeDefault theme;
			//theme.set_resources(resources);

			//gui.set_theme(theme);
			gui.set_css_document("theme.css");

			DisplayWindowDescription win_desc;
			win_desc.set_allow_resize(true);
			win_desc.set_title("Label test app."); 
			win_desc.set_position(Rect(400,400,800,650), false);
			Window root(&gui, win_desc);			
			root.func_close().set(this, &App::on_close, &root); 
   
			Label label1(&root);
			label1.set_geometry(Rect(20, 20, 200, 40));
			//label1.set_class_name("yellow");
			label1.set_text("Yellow label ÅÄygj");
/*
			Label label2(&root);
			label2.set_geometry(Rect(20, 60, 300, 100));

			LineEdit input_name(&label2);
			LineEdit input_password(&label2);
			input_name.set_geometry(Size(100, 16));
			input_password.set_geometry(Size(100, 16));
			input_name.set_visible(true, false);
			input_password.set_visible(true, false);

			Font font(label1.get_gc(), "Verdana", -13);
			SpanLayout span;
			span.add_text("Name: ", font, Colorf::black);
			span.add_component(&input_name, 4);
			span.add_text("\nPassword: ", font, Colorf::black);
			span.add_component(&input_password, 4);

			label2.set_span(span);
			label2.set_alignment(Label::align_right);
*/
			gui.exec();
		}
		catch (Exception e)
		{
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

