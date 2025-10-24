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

	try
	{
		CL_DisplayWindowDescription desc;
		desc.set_title("ClanLib Custom OpenGL Example");
		desc.set_size(CL_Size(640, 480), true);
		CL_DisplayWindow window(desc);

		// Connect the Window close event
		CL_Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close);

		// Connect a keyboard handler to on_key_up()
		CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

		// Get the graphic context
		CL_GraphicContext gc = window.get_gc();

		gc.set_map_mode(cl_user_projection);

		// Run until someone presses escape
		while (!quit)
		{

			CL_Mat4f perp = CL_Mat4f::perspective(45.0f, ((float) gc.get_width()) / ((float) gc.get_height()), 0.1f, 100000.0f);
			gc.set_projection(perp);
			gc.flush_batcher();

			// Note, use "CL_GraphicContext_GL1", if you use the GL1 target
			CL_GraphicContext_GL gl_gc(gc);
			gl_gc.set_active();

			// Note, use "cl1" insteal of "cl", if you use the GL1 target

			clMatrixMode(CL_MODELVIEW);							// Select The Modelview Matrix
			clClear(CL_COLOR_BUFFER_BIT | CL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
			clLoadIdentity();									// Reset The Current Modelview Matrix
			clTranslatef(-1.5f,0.0f,-6.0f);						// Move Left 1.5 Units And Into The Screen 6.0
			clBegin(CL_TRIANGLES);								// Drawing Using Triangles
				clColor3f(1.0f,0.0f,0.0f);						// Set The Color To Red
				clVertex3f( 0.0f, 1.0f, 0.0f);					// Top
				clColor3f(0.0f,1.0f,0.0f);						// Set The Color To Green
				clVertex3f(-1.0f,-1.0f, 0.0f);					// Bottom Left
				clColor3f(0.0f,0.0f,1.0f);						// Set The Color To Blue
				clVertex3f( 1.0f,-1.0f, 0.0f);					// Bottom Right
			clEnd();											// Finished Drawing The Triangle
			clTranslatef(3.0f,0.0f,0.0f);						// Move Right 3 Units
			clColor3f(0.5f,0.5f,1.0f);							// Set The Color To Blue One Time Only
			clBegin(CL_QUADS);									// Draw A Quad
				clVertex3f(-1.0f, 1.0f, 0.0f);					// Top Left
				clVertex3f( 1.0f, 1.0f, 0.0f);					// Top Right
				clVertex3f( 1.0f,-1.0f, 0.0f);					// Bottom Right
				clVertex3f(-1.0f,-1.0f, 0.0f);					// Bottom Left
			clEnd();											// Done Drawing The Quad


			// Flip the display, showing on the screen what we have drawed
			// since last call to flip()
			window.flip(1);

			// This call processes user input and other events
			CL_KeepAlive::process();
		}
	}
	catch(CL_Exception &exception)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console", 80, 160);
		CL_Console::write_line("Exception caught: " + exception.get_message_and_stack_trace());
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


