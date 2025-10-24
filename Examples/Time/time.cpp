#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

class ExampleTime : public CL_ClanApplication
{
public:
	virtual int main(int argc, char **argv)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();
		
		try
		{
			// Initialize ClanLib base components
			CL_SetupCore setup_core;

			// Initialize the ClanLib display component
			CL_SetupDisplay setup_display;

			// Initilize the OpenGL drivers
			CL_SetupGL setup_gl;

			// Set a videomode
			CL_DisplayWindow window("ClanLib Time Example", 640, 480);

			CL_ResourceManager resources("resources.xml", false);
			CL_Font font("font_gray", &resources);

			// Class to give us the framerate
			CL_FramerateCounter framerate;

			// A timer that emits a signal every 2 seconds
			CL_Timer timer(2000);
			CL_Slot slot_timer = timer.sig_timer().connect(this, &ExampleTime::on_timer);
			timer.enable();

			// Call callback once to initialize variable
			on_timer();

			// Run until someone presses escape
			while (!CL_Keyboard::get_keycode(CL_KEY_ESCAPE))
			{
				// Clear the display in a dark blue nuance
				// The four arguments are red, green, blue and alpha
				// All color nuances in ClanLib are measured in the interval 0->255
				CL_Display::clear(CL_Color(0,0,56));

				font.draw(10,10, "FPS: " + CL_String::from_int(framerate.get_fps()));

				font.draw(10,30, CL_String::from_int(time_since_system_boot) + " ms since system boot (updating every 2 secs)");

				// Flip the display, showing on the screen what we have drawed
				// since last call to flip_display()
				CL_Display::flip();

				// This call updates input and performs other "housekeeping"
				// call this each frame
				CL_System::keep_alive();
			}
		}
		catch(CL_Error error)
		{
			std::cout << "Exception caught : " << error.message.c_str() << std::endl;			

			// Display console close message and wait for a key
			console.display_close_message();

			return -1;
		}

		return 0;
	}

	void on_timer()
	{
		time_since_system_boot = CL_System::get_time();
	}

	int time_since_system_boot;
} my_app;
