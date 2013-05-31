#include <ClanLib/core.h> 
#include <ClanLib/gui.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
using namespace clan;

class App
{
public:
	ProgressBar *progress1;
	ProgressBar *progress2;
	ProgressBar *progress3;

	int main(const std::vector<std::string> &args)
	{
		ConsoleWindow console("Console");

		try 
		{
			DisplayCache resources("../../../Resources/GUIThemeLuna/resources.xml");

			GUIManager gui; 
 
			GUIWindowManagerSystem wm;
			gui.set_window_manager(wm);

			GUIThemeDefault theme;
			theme.set_resources(resources);
			gui.set_theme(theme);
			gui.set_css_document("theme.css");
 
			DisplayWindowDescription win_desc;
			win_desc.set_allow_resize(true);
			win_desc.set_title("The Amazing ProgressBar");
			win_desc.set_position(Rect(200,200,600,600), false);
			Window root(&gui, win_desc);
			root.func_close().set(this, &App::on_close, &root);

			PushButton button1(&root);
			button1.set_geometry(Rect(20, 20, 200, 60));
			button1.func_clicked().set(this, &App::on_button_clicked, &button1);
			button1.set_text("Click me!");

			progress1 = new ProgressBar(&root);
			progress1->set_geometry(Rect(20, 70, 200, 88));
			progress1->set_min(0);
			progress1->set_max(100);
			progress1->set_step_size(1);
			progress1->set_position(0);

			progress2 = new ProgressBar(&root);
			progress2->set_geometry(Rect(20, 90, 200, 108));
			progress2->set_min(0);
			progress2->set_max(100);
			progress2->set_step_size(1);
			progress2->set_position(100);

			progress3 = new ProgressBar(&root);
			progress3->set_geometry(Rect(20, 110, 200, 128));
			progress3->set_min(0);
			progress3->set_max(100);
			progress3->set_step_size(10);
			progress3->set_position(0);

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
		int new_value = rand() % 100;

		progress1->set_position(new_value);
		progress1->request_repaint();

		progress2->set_position(100 - new_value);
		progress2->request_repaint();

		progress3->step_position();
		progress3->request_repaint();
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


