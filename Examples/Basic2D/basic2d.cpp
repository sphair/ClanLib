#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>

#include <cmath>

#ifdef USE_GDI
#include <ClanLib/gdi.h>
#else
#include <ClanLib/gl.h>
#endif

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

#ifdef USE_GDI
		// Initilize the GDI drivers
		CL_SetupGDI setup_gdi;
#else
		// Initilize the OpenGL drivers
		CL_SetupGL setup_gl;
#endif
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
	CL_ConsoleWindow console("Console");

	try
	{
		// Set the window
		// This opens a 640 x 480 window, including the frame size
		// If you want more control over the window, pass CL_DisplayWindowDescription to CL_DisplayWindow
		// (This is useful to create a borderless window of a specific size)
		// If you require target specific control over the window, use the derived CL_OpenGLWindowDescription
		// (This contains the multisampling options)
		CL_DisplayWindow window("ClanLib Basic2D Example", 640, 480);

		// Connect the Window close event
		CL_Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close);

		// Connect a keyboard handler to on_key_up()
		CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

		// Get the graphic context
		CL_GraphicContext gc = window.get_gc();

		// Load a sprite from a targa-file
		CL_Sprite spr_logo(gc, "logo.tga");

		float sin_count = 0.0f;
		float ypos = 0.0f;
		float ydir = 3.0f;

		// Run until someone presses escape
		while (!quit)
		{
			// Clear the display in a dark blue nuance
			// The four arguments are red, green, blue and alpha
			gc.clear(CL_Colorf(0.0f,0.0f,0.2f));

			ypos += ydir;
			if (ypos+200.0f >= gc.get_height() || ypos < 0.0f)
			{
				ydir *= -1.0f;
			}
				
			CL_Draw::line(gc, 0, ypos-1.0f, (float) gc.get_width(), ypos-1.0f,CL_Colorf(0.5f, 0.0f, 0.0f));
			CL_Draw::line(gc, 0, ypos+198.0f, (float) gc.get_width(), ypos+198.0f, CL_Colorf(0.5f, 0.0f, 0.0f));

			// Show the logo image.
			// Use the get_width() and get_height() functions of both
			// CL_DisplayWindow and CL_Sprite, to show the image in the bottom right corner
			spr_logo.draw(gc, 
				(float) gc.get_width()-spr_logo.get_width(),
				(float) gc.get_height()-spr_logo.get_height());

			gc.push_cliprect(CL_Rect(0, (int)(ypos), gc.get_width(), (int)(ypos+198)));

			// Draw a rectangle in the center of the screen
			// going from (240, 140) -> (440, 340) _not_ including the 
			// pixels in the right-most column and bottom-most row (440, 340)
			CL_Draw::fill(gc, CL_Rectf(240.0f, 140.0f, 440.0f, 340.0f), CL_Colorf(1.0f, 1.0f, 1.0f));

			// Frame the rectangle with red lines
			CL_Draw::line(gc, 240.0f, 140.0f, 440.0f, 140.0f, CL_Colorf(1.0f, 0.0f, 0.0f));
			CL_Draw::line(gc, 240.0f, 340.0f, 440.0f, 340.0f, CL_Colorf(1.0f, 0.0f, 0.0f));
			CL_Draw::line(gc, 240.0f, 140.0f, 240.0f, 340.0f, CL_Colorf(1.0f, 0.0f, 0.0f));
			CL_Draw::line(gc, 440.0f, 140.0f, 440.0f, 340.0f, CL_Colorf(1.0f, 0.0f, 0.0f));

			// Show a few alpha-blending moving rectangles that moves in circles
			float x = cos(sin_count)*120.0f;
			float y = sin(sin_count)*120.0f;
			sin_count += 0.05f;
			CL_Draw::fill(gc, CL_Rectf( 320.0f + x -30.0f, 240.0f + y -30.0f, CL_Sizef(30.0f, 30.0f)), CL_Colorf(0.0f, 1.0f, 0.0, 0.5f));
			x = cos(sin_count+3.14159f)*120.0f;
			y = sin(sin_count+3.14159f)*120.0f;
			CL_Draw::fill(gc, CL_Rectf( 320.0f + x -30.0f, 240 + y -30.0f, CL_Sizef(30.0f, 30.0f)), CL_Colorf(1.0f, 1.0f, 0.0, 0.5f));

			gc.pop_cliprect();

			// Flip the display, showing on the screen what we have drawed
			// since last call to flip()
			window.flip(1);

			// This call processes user input and other events
			CL_DisplayMessageQueue::process();
		}
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


