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
			win_desc.set_position(CL_Rect(200,200,600,700), false);
			CL_Window root(&gui, win_desc);			
			root.func_close().set(this, &App::on_close, &root);
   
			CL_Tab tab(&root);
			tab.set_geometry(CL_Rect(CL_Point(50,20),CL_Size(280,360)));

			// tab 1
			CL_TabPage *tab_page_1 = tab.add_page("foo");
			//tab_page_1->set_geometry(CL_Rect(CL_Point(0,0),CL_Size(230,300)));
			//tab.add_page("foo", "foo", &tab_page_1);

			CL_Label label_p1(tab_page_1);
			label_p1.set_geometry(CL_Rect(CL_Point(10,10),CL_Size(130,20)));
			label_p1.set_text("tab page 1");

			CL_PushButton btn_p1(tab_page_1);
			btn_p1.set_geometry(CL_Rect(CL_Point(10,30),CL_Size(130,20)));
			btn_p1.set_text("Button Foo");

			// tab 2
			CL_TabPage *tab_page_2 = tab.add_page("bar");
			//tab_page_2->set_geometry(CL_Rect(CL_Point(0,0),CL_Size(230,300)));
			//tab.add_page("bar", "tab with really long name", &tab_page_2);
 
			CL_Label label_p2(tab_page_2);
			label_p2.set_geometry(CL_Rect(CL_Point(10,10),CL_Size(130,20)));
			label_p2.set_text("tab page 2");

			CL_PushButton btn_p2(tab_page_2);
			btn_p2.set_geometry(CL_Rect(CL_Point(10,30),CL_Size(80,26)));
			btn_p2.set_text("Button Bar");

			// tab 3
			CL_TabPage *tab_page_3 = tab.add_page("p3");
			//tab_page_3->set_geometry(CL_Rect(CL_Point(0,0),CL_Size(230,300)));
			//tab.add_page("p3", "p3", &tab_page_3);

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

