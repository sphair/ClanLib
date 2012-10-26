#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
using namespace clan;

// This is the Application class (That is instantiated by the Program Class)
class App
{
public:
	int start(const std::vector<std::string> &args);

private:
	void on_input_up(const InputEvent &key, const InputState &state);
	void on_window_close();

private:
	bool quit;
};

// This is the Program class that is called by Application
class Program
{
public:
	static int main(const std::vector<std::string> &args)
	{
		// Initialize ClanLib base components
		SetupCore setup_core;

		// Initialize the ClanLib display component
		SetupDisplay setup_display;

		// Initilize the OpenGL drivers
		SetupGL setup_gl;

		// Start the Application
		App app;
		int retval = app.start(args);
		return retval;
	}
};

// Instantiate Application, informing it where the Program is located
Application app(&Program::main);

// The start of the Application
int App::start(const std::vector<std::string> &args)
{
	quit = false;

	// Create a console window for text-output if not available
	ConsoleWindow console("Console", 80, 200);

	try
	{
		// Set the window
		// This opens a 640 x 480 window, including the frame size
		// If you want more control over the window, pass DisplayWindowDescription to DisplayWindow
		// (This is useful to create a borderless window of a specific size)
		// If you require target specific control over the window, use the derived OpenGLWindowDescription
		// (This contains the multisampling options)
		OpenGLWindowDescription desc;
		desc.set_title("ClanLib 2D Test");
		desc.set_size(Size(800, 600), true);
//		desc.set_multisampling(4);
		DisplayWindow window(desc);

		// Connect the Window close event
		Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close);

		// Connect a keyboard handler to on_key_up()
		Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

		// Get the graphic context
		GraphicContext gc = window.get_gc();

		ResourceManager resources("resources.xml");

		Font small_font = Font(gc, "Tahoma", 12);

		int num_windows_created = 0;
		int start_time = System::get_time();

		OpenGLWindowDescription desc2;
		desc2.set_title("test window");
		desc2.set_size(Size(320, 200), true);

		DisplayWindow window2(desc2);

		// Run until someone presses escape
		while (!quit)
		{
			gc.clear(Colorf(0.0f,0.0f,0.2f));

			small_font.draw_text(gc, 420, 180, "Window Speed Test");
			small_font.draw_text(gc, 420, 200, string_format("num windows: %1", num_windows_created));

/*			DisplayWindow window2(desc2);
			window2 = DisplayWindow();*/

			window2.hide();
			window2.set_position(400+rand()%100, 400+rand()%100);
			window2.set_size(50+rand()%200, 50+rand()%200,true);
			window2.show();

			num_windows_created++;

			float wps = 1000 * (num_windows_created/float(System::get_time()-start_time));
			small_font.draw_text(gc, 420, 220, string_format("windows per second: %1", wps));

			window.flip(1);
			KeepAlive::process();
		}
		small_font = Font();
	}
	catch(Exception& exception)
	{
		Console::write_line("Exception caught:");
		Console::write_line(exception.message);

		// Display the stack trace (if available)
		std::vector<std::string> stacktrace = exception.get_stack_trace();
		int size = stacktrace.size();
		if (size > 0)
		{
			Console::write_line("Stack Trace:");
			for (int cnt=0; cnt < size; cnt++)
			{
				Console::write_line(stacktrace[cnt]);
			}
		}

		console.display_close_message();

		return -1;
	}
	return 0;
}

// A key was pressed
void App::on_input_up(const InputEvent &key, const InputState &state)
{
	if(key.id == KEY_ESCAPE)
	{
		quit = true;
	}
}

// The window was closed
void App::on_window_close()
{
	quit = true;
}


