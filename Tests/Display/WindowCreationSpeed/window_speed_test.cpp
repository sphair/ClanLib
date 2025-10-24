#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

// This is the Application class (That is instantiated by the Program Class)
class App
{
public:
	int start(const std::vector<CL_String> &args);

private:
	void on_input_up(const CL_InputEvent &key, const CL_InputState &state);
	void on_window_close();

private:
	bool quit;
};

// This is the Program class that is called by CL_ClanApplication
class Program
{
public:
	static int main(const std::vector<CL_String> &args)
	{
		// Initialize ClanLib base components
		CL_SetupCore setup_core;

		// Initialize the ClanLib display component
		CL_SetupDisplay setup_display;

		// Initilize the OpenGL drivers
		CL_SetupGL setup_gl;

		// Start the Application
		App app;
		int retval = app.start(args);
		return retval;
	}
};

// Instantiate CL_ClanApplication, informing it where the Program is located
CL_ClanApplication app(&Program::main);

// The start of the Application
int App::start(const std::vector<CL_String> &args)
{
	quit = false;

	// Create a console window for text-output if not available
	CL_ConsoleWindow console("Console", 80, 200);

	try
	{
		// Set the window
		// This opens a 640 x 480 window, including the frame size
		// If you want more control over the window, pass CL_DisplayWindowDescription to CL_DisplayWindow
		// (This is useful to create a borderless window of a specific size)
		// If you require target specific control over the window, use the derived CL_OpenGLWindowDescription
		// (This contains the multisampling options)
		CL_OpenGLWindowDescription desc;
		desc.set_title("ClanLib 2D Test");
		desc.set_size(CL_Size(800, 600), true);
//		desc.set_multisampling(4);
		CL_DisplayWindow window(desc);

		// Connect the Window close event
		CL_Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close);

		// Connect a keyboard handler to on_key_up()
		CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

		// Get the graphic context
		CL_GraphicContext gc = window.get_gc();

		CL_ResourceManager resources("resources.xml");

		CL_Font small_font = CL_Font(gc, "Tahoma", 12);

		int num_windows_created = 0;
		int start_time = CL_System::get_time();

		CL_OpenGLWindowDescription desc2;
		desc2.set_title("test window");
		desc2.set_size(CL_Size(320, 200), true);

		CL_DisplayWindow window2(desc2);

		// Run until someone presses escape
		while (!quit)
		{
			gc.clear(CL_Colorf(0.0f,0.0f,0.2f));

			small_font.draw_text(gc, 420, 180, "Window Speed Test");
			small_font.draw_text(gc, 420, 200, cl_format("num windows: %1", num_windows_created));

/*			CL_DisplayWindow window2(desc2);
			window2 = CL_DisplayWindow();*/

			window2.hide();
			window2.set_position(400+rand()%100, 400+rand()%100);
			window2.set_size(50+rand()%200, 50+rand()%200,true);
			window2.show();

			num_windows_created++;

			float wps = 1000 * (num_windows_created/float(CL_System::get_time()-start_time));
			small_font.draw_text(gc, 420, 220, cl_format("windows per second: %1", wps));

			window.flip(1);
			CL_KeepAlive::process();
		}
		small_font = CL_Font();
	}
	catch(CL_Exception& exception)
	{
		CL_Console::write_line("Exception caught:");
		CL_Console::write_line(exception.message);

		// Display the stack trace (if available)
		std::vector<CL_String> stacktrace = exception.get_stack_trace();
		int size = stacktrace.size();
		if (size > 0)
		{
			CL_Console::write_line("Stack Trace:");
			for (int cnt=0; cnt < size; cnt++)
			{
				CL_Console::write_line(stacktrace[cnt]);
			}
		}

		console.display_close_message();

		return -1;
	}
	return 0;
}

// A key was pressed
void App::on_input_up(const CL_InputEvent &key, const CL_InputState &state)
{
	if(key.id == CL_KEY_ESCAPE)
	{
		quit = true;
	}
}

// The window was closed
void App::on_window_close()
{
	quit = true;
}


