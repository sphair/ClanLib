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
			resources.load("../../../Resources/GUIThemeLuna/resources.xml");
			
			GUIManager gui;

			GUIWindowManagerSystem wm;
			gui.set_window_manager(wm);

			GUIThemeDefault theme;
			theme.set_resources(resources);

			gui.set_theme(theme);
			gui.set_css_document("../../../Resources/GUIThemeLuna/theme.css");

			DisplayWindowDescription win_desc;
			win_desc.set_allow_resize(true);
			win_desc.set_title("Label test app."); 
			win_desc.set_position(Rect(200,200,600,700), false);
			Window root(&gui, win_desc);			
			root.func_close().set(this, &App::on_close, &root);
   
			Tab tab(&root);
			tab.set_geometry(Rect(Point(50,20),Size(280,360)));

			// tab 1
			TabPage *tab_page_1 = tab.add_page("foo");
			//tab_page_1->set_geometry(Rect(Point(0,0),Size(230,300)));
			//tab.add_page("foo", "foo", &tab_page_1);

			Label label_p1(tab_page_1);
			label_p1.set_geometry(Rect(Point(10,10),Size(130,20)));
			label_p1.set_text("tab page 1");

			PushButton btn_p1(tab_page_1);
			btn_p1.set_geometry(Rect(Point(10,30),Size(130,20)));
			btn_p1.set_text("Button Foo");

			// tab 2
			TabPage *tab_page_2 = tab.add_page("bar");
			//tab_page_2->set_geometry(Rect(Point(0,0),Size(230,300)));
			//tab.add_page("bar", "tab with really long name", &tab_page_2);
 
			Label label_p2(tab_page_2);
			label_p2.set_geometry(Rect(Point(10,10),Size(130,20)));
			label_p2.set_text("tab page 2");

			PushButton btn_p2(tab_page_2);
			btn_p2.set_geometry(Rect(Point(10,30),Size(80,26)));
			btn_p2.set_text("Button Bar");

			// tab 3
			TabPage *tab_page_3 = tab.add_page("p3");
			//tab_page_3->set_geometry(Rect(Point(0,0),Size(230,300)));
			//tab.add_page("p3", "p3", &tab_page_3);

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

