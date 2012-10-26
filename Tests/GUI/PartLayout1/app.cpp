#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <ClanLib/csslayout.h>

class App
{
public:
	int main(const std::vector<CL_String> &args)
	{
		try
		{
			CL_DisplayWindow window("GUI Layout test", 1024, 768, false, true);

			CL_GUIManager gui(window, "../../../Resources/GUIThemeAero");

			gui.initialize_layout_manager("layout.xml", "layout.css");

			CL_GUITopLevelDescription window_desc1;
			window_desc1.set_title("Window 1");
			window_desc1.set_position(CL_Rect(100, 100, 300, 300), true);
			window_desc1.set_allow_resize(true);
			CL_Window window1(&gui, window_desc1);
			window1.func_close().set(this, &App::on_close, &window1);
			window1.set_draggable(true);

			window1.exec();
		}
		catch (CL_Exception e)
		{
			CL_ConsoleWindow console("Console");
			CL_Console::write_line(e.message);
			console.display_close_message();
		}

		return 0;
	}

	bool on_close(CL_Window *window)
	{
		window->exit_with_code(0);
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
