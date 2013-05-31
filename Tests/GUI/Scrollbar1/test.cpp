#include <ClanLib/core.h> 
#include <ClanLib/gui.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
using namespace clan;

#ifndef WIN32
// For rand()
#include <stdlib.h>
#endif

class App
{
public:
	ScrollBar *scrollbar1;
	ScrollBar *scrollbar2;
	ScrollBar *scrollbar3;

	int main(const std::vector<std::string> &args)
	{
		ConsoleWindow console("Console");

		try
		{
			DisplayCache resources;  
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
			win_desc.set_title("The Amazing Scrollbar");
			win_desc.set_position(Rect(200,200,600,600), false);
			Window root(&gui, win_desc);
			root.func_close().set(this, &App::on_close, &root);

			PushButton button1(&root);
			button1.set_geometry(Rect(20, 20, 200, 60));
			button1.func_clicked().set(this, &App::on_button_clicked, &button1);
			button1.set_text("Click me!");

			scrollbar1 = new ScrollBar(&root);
			scrollbar1->set_geometry(Rect(20, 70, 20 + 17, 70 + 200));
			scrollbar1->set_vertical();
			scrollbar1->set_min(0);
			scrollbar1->set_max(100);
			scrollbar1->set_line_step(1);
			scrollbar1->set_page_step(40);
			scrollbar1->set_position(50);

			scrollbar2 = new ScrollBar(&root);
			scrollbar2->set_geometry(Rect(50, 70, 50 + 17, 70 + 200));
			scrollbar2->set_vertical();
			scrollbar2->set_min(0);
			scrollbar2->set_max(100);
			scrollbar2->set_line_step(1);
			scrollbar2->set_page_step(40);
			scrollbar2->set_position(50);
			scrollbar2->set_enabled(false);

			scrollbar3 = new ScrollBar(&root);
			scrollbar3->set_geometry(Rect(100, 70, 100 + 200, 70 + 17));
			scrollbar3->set_horizontal();
			scrollbar3->set_min(0);
			scrollbar3->set_max(100);
			scrollbar3->set_line_step(1);
			scrollbar3->set_page_step(40);
			scrollbar3->set_position(50);

			gui.exec();
		}
		catch (Exception e)
		{
 			Console::write_line(e.message);
  			console.display_close_message();
		}
 
		return 0;
 	}
 
	void on_button_clicked(PushButton *button)
	{
		scrollbar1->set_position(rand() % 100);
		scrollbar1->request_repaint();
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

