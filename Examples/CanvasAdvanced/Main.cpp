#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

#include <cstdlib>

#define	LIGHTS_COLOR		true		// Add color to our lights?
#define USE_SCALE			true		// Set to false to see the difference with fullscreen vs. scaled mode.
#define CANVAS_SCALE_X		4.0f		// This would be scaled by aspect ratio.  For example 4:3
#define CANVAS_SCALE_Y		3.0f

#include "LightContainer.h"

class ExampleCanvas
{
public:
	int start(const std::vector<CL_String> &args)
	{ 
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		
		quit = false;

		try
		{
			// Set a videomode - 1024x256
			CL_DisplayWindowDescription desc;
			desc.set_allow_resize(false);
			desc.set_title("ClanLib - Advanced Canvas");
			desc.set_size(CL_Size(1024, 768), true);
			desc.set_fullscreen(false);
			CL_DisplayWindow window(desc);
			gc = window.get_gc();

			// Seed randoms
			srand(CL_System::get_time());

			// Connect Slots
			CL_Slot slot_quit = window.sig_window_close().connect(this, &ExampleCanvas::on_window_close);
			CL_Slot slot_keyboard_up = window.get_ic().get_keyboard().sig_key_up().connect(this, &ExampleCanvas::on_keyboard_up);
			CL_Slot slot_mouse_up = window.get_ic().get_mouse().sig_key_up().connect(this, &ExampleCanvas::on_mouse_up);
			CL_Slot	slot_mouse_move = window.get_ic().get_mouse().sig_pointer_move().connect(this, &ExampleCanvas::on_mouse_move);

			// Load the surfaces
			// -- This is our ground texture.
			background = CL_Image(gc, "data/background.png");

			// -- This is the light mask that draws over everything (and is attached to our framebuffer).
			if(USE_SCALE)
				light_mask = CL_Texture(gc, "data/scale_mask.png");
			else
				light_mask = CL_Texture(gc, "data/full_mask.png");

			// Create the framebuffer, and attach ground texture into its color buffer
			fb_lightmask = CL_FrameBuffer(gc);
			fb_lightmask.attach_color_buffer(0, light_mask);

			// Just a bunch of variables for keeping time and tracking FPS
			unsigned int current_time = 0;
			unsigned int last_time = CL_System::get_time();
			float micro_second = 0;
			float fps_ticker = 0;
			int real_fps = 0;
			int frames = 0;

			// Some day/night cycle variables.
			float daylight = 0.50f;
			bool daylight_forward = false;

			// Setup our mouse pointer light.
			mouse_light = new Light(gc);
			mouse_light->set_scale(3.0f, 3.0f);
			mouse_light->set_color(CL_Colorf((float)get_random(0,100)/100,(float)get_random(0,100)/100,(float)get_random(0,100)/100,0.20f));
			lights.add(mouse_light);

			// Run until someone presses escape
			while (!quit)
			{
				// Manage our time/fps
				current_time = CL_System::get_time();
				if (last_time - current_time == 0)
					micro_second = 0;
				else 
					micro_second = (float(current_time) - float(last_time)) / 1000.0f;
				last_time = current_time;
				fps_ticker += micro_second;

				if(fps_ticker > 1.00f)
				{
					// a second passed.
					real_fps = int(frames / fps_ticker);
					CL_Console::write_line("FPS: %1",real_fps);
					fps_ticker = 0;
					frames = 0;
				}

				// Manage our day/night cycle
				if(daylight_forward)
				{
					daylight += micro_second * 0.10f;	// Add a smaller amount of our delta time to slow the transition a little.
					if(daylight >= 1.0f)
						daylight_forward = false;
				}
				else
				{
					daylight -= micro_second * 0.10f;
					if(daylight <= 0.30f)
						daylight_forward = true;
				}

				// Updates
				lights.update(micro_second);

				// ** Draw Regular Game Objects/Images ** \\

				// Draw background
				background.draw(gc, 0, 0);

				// Draw your normal game objects around here...
	
				// Draw colored lights.
				if(LIGHTS_COLOR)
					lights.draw(gc);

				// ** Clip Light Mask ** \\

				// Set the working framebuffer
				gc.set_frame_buffer(fb_lightmask);

				// Clear it from last frame.
				// -- Here is where day/night can be accomplished, but interpolating the color/alpha values.
				gc.clear(CL_Colorf(0.0f, 0.0f, 0.0f, daylight));
				
				// Draw the Light cutouts
				lights.draw_clips(gc);

				// We're done making our changes to the texture, so reset the buffer.
				gc.reset_frame_buffer();
			
				// Draw the lightmask texture
				// The color of the texture here can influence your output.  I chose to keep it simple
				// by staying white with 100% alpha.
				gc.set_texture(0, light_mask);
				if(USE_SCALE)
					gc.mult_scale(CANVAS_SCALE_X,CANVAS_SCALE_Y);
				CL_Draw::texture(gc, CL_Rect(light_mask.get_size()), CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f));
				gc.pop_modelview();

				// Reset
				gc.reset_texture(0);
				
				// Flip the display, showing on the screen what we have drawn (no v-sync)
				window.flip(0);
				frames++;

				// This call updates input and performs other "housekeeping"
				CL_DisplayMessageQueue::process();
			}

			// Cleanup
			lights.clear();
			background = CL_Image();
			light_mask = CL_Texture();
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

private:
	void on_mouse_move(const CL_InputEvent &key, const CL_InputState &state)
	{
		// Update our mouse light to reflect the current mouse position.
		mouse_light->set_translation((float)key.mouse_pos.x, (float)key.mouse_pos.y);
	};

	void on_keyboard_up(const CL_InputEvent &key, const CL_InputState &state)
	{
		if(key.id == CL_KEY_ESCAPE)
			quit = true;
	}

	void on_mouse_up(const CL_InputEvent &key, const CL_InputState &state)
	{
		// Here we create a random light.
		int r = get_random(0,100);
		int g = get_random(0,100);
		int b = get_random(0,100);
		int scale = get_random(100,500);

		Light *light = new Light(gc);
		light->set_translation((float)get_random(50,900), (float)get_random(50,700));
		light->set_scale((float)scale/100,(float)scale/100);
		light->set_color(CL_Colorf( (float)r/100, (float)g/100, (float)b/100, 0.20f));
		light->set_life(5000);	// Die in 5 seconds.

		lights.add(light);
	}

	void on_window_close()
	{
		quit = true;
	}

	int get_random(int low, int high)
	{ 
		return low+int(rand() * (high - low) / (RAND_MAX+1.0)); 
	}

	LightContainer		lights;				// Our container for lights.
	Light				*mouse_light;		// The light that follows our mouse around.
	CL_Image			background;			// This is a simple image, so we use the simple image class.
	CL_Texture			light_mask;			// We require more low-level functions for our lights, so this should be a texture.
	CL_FrameBuffer		fb_lightmask;		// The FrameBuffer.
	CL_GraphicContext	gc;					// Graphic Context

	bool				quit;				
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
		ExampleCanvas app;
		return app.start(args);
	}
};

// Instantiate CL_ClanApplication, informing it where the Program is located
CL_ClanApplication app(&Program::main);
