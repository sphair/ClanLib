#include <ClanLib/core.h>
#include <ClanLib/gui.h>
#include <ClanLib/guistylesilver.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

// The main application
class App : public CL_ClanApplication
{
public:
	int main(int argc, char** argv)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();

		try
		{
			CL_SetupCore setup_core;
			CL_SetupDisplay setup_display;
			CL_SetupGL setup_gl;
			CL_SetupGUI setup_gui;

			CL_DisplayWindow display("GUI Simple example", 640, 480);

			// Connect the Window close event
			CL_Slot slot_quit = display.sig_window_close().connect(this, &App::on_window_close);
			
			// Create the GUI using the default style
			CL_ResourceManager gui_resources("../../Resources/GUIStyleSilver/gui.xml");
			CL_StyleManager_Silver style(&gui_resources);
			CL_GUIManager guimanager(&style);
			gui = &guimanager;

			CL_Window window(CL_Rect(10, 10, 300, 300), "Window", &guimanager);
			
			CL_Slot slot_paint = guimanager.sig_paint().connect(this, &App::on_paint);

			guimanager.run();
		}
		catch (CL_Error e)
		{
 			std::cout << e.message.c_str() << std::endl;

			// Display console close message and wait for a key
			console.display_close_message();
		}

		return 0;
	}
	
private:
	void on_window_close()
	{
		gui->quit();
	}
	
	void on_paint()
	{
		CL_Display::clear();
	}
	
	CL_GUIManager *gui;
} app;
