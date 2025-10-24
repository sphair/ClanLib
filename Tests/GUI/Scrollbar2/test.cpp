#include <ClanLib/core.h> 
#include <ClanLib/gui.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/swrender.h>

class App
{
public:
	std::vector<CL_ScrollBar *> scrollbars;

	int main(const std::vector<CL_String> &args)
	{
		CL_ConsoleWindow console("Console");
		CL_ConsoleLogger logger;

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
			win_desc.set_title("The Performing Scrollbar");
			win_desc.set_position(CL_Rect(200,200,805,540), false);
			CL_Window root(&gui, win_desc);
			root.func_close().set(this, &App::on_close, &root);
			root.set_constant_repaint(true);

			int x = 0;
			int y = 0;
			for(int i = 0; i < 30; ++i)
			{
				CL_ScrollBar *scrollbar1 = new CL_ScrollBar(&root);
				scrollbar1->set_geometry(CL_Rect(x, y, x + 17, y + 200));
				scrollbar1->set_vertical();
				scrollbar1->set_min(0);
				scrollbar1->set_max(100);
				scrollbar1->set_line_step(1);
				scrollbar1->set_page_step(40);
				scrollbar1->set_position(50);

				if(i % 2 == 0)
					scrollbar1->set_enabled(false);

				x += 20;
			}

			for(int i = 0; i < 15; ++i)
			{
				if(i == 0)
				{
					x = 0;
					y = 203;
				}
				if(i == 5)
				{
					x = 203;
					y = 203;
				}
				if(i == 10)
				{
					x = 406;
					y = 203;
				}

				CL_ScrollBar *scrollbar1 = new CL_ScrollBar(&root);
				scrollbar1->set_geometry(CL_Rect(x, y, x + 200, y + 17));
				scrollbar1->set_horizontal();
				scrollbar1->set_min(0);
				scrollbar1->set_max(100);
				scrollbar1->set_line_step(1);
				scrollbar1->set_page_step(40);
				scrollbar1->set_position(50);

				if(i % 2 == 0)
					scrollbar1->set_enabled(false);

				y += 20;
			}

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
//		CL_SetupGL setup_gl;
		CL_SetupSWRender setup_swrender;

		// Start the Application
		App app;
		return app.main(args);
	}
};

// Instantiate CL_ClanApplication, informing it where the Program is located
CL_ClanApplication app(&Program::main);
