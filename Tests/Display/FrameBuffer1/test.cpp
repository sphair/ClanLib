
#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

class ExampleText
{
public:
	int start(const std::vector<CL_String> &args);
private:
	void on_keyboard_up(const CL_InputEvent &key, const CL_InputState &state);
	void on_window_close();
	
	bool quit;
	float last_fps;
	bool multi_framebuffer_flag;

};

int ExampleText::start(const std::vector<CL_String> &args)
{ 
	quit = false;
	multi_framebuffer_flag = true;

	try
	{
		// Set a videomode
		CL_DisplayWindowDescription desc;
		desc.set_allow_resize(false);
		desc.set_title("ClanLib FrameBuffer Speed Test");
		desc.set_size(CL_Size(800, 600), true);
		CL_DisplayWindow window(desc);

		// Connect the Window close event
		CL_Slot slot_quit = window.sig_window_close().connect(this, &ExampleText::on_window_close);

		// Connect a keyboard handler to on_key_up()
		CL_Slot slot_keyboard_up = window.get_ic().get_keyboard().sig_key_up().connect(this, &ExampleText::on_keyboard_up);

		CL_GraphicContext gc = window.get_gc();

		CL_FontDescription font_desc;
		font_desc.set_typeface_name("Tahoma");
		font_desc.set_anti_alias(true);
		font_desc.set_height(32);
		CL_Font font(gc, font_desc);

		CL_Texture texture(gc, 512, 512, cl_rgb8);
		texture.set_wrap_mode(cl_wrap_repeat, cl_wrap_repeat, cl_wrap_repeat);
		texture.set_min_filter(cl_filter_linear);
		texture.set_mag_filter(cl_filter_linear);

		// Create the framebuffer, and attach the texture into its color buffer
		CL_FrameBuffer fb;
		fb = CL_FrameBuffer(gc);
		fb.attach_color_buffer(0, texture);

		// Run until someone presses escape
		while (!quit)
		{
			static int total_time = 0, fps_count = 0, last_fps= 0;
			static int start_time = 0;

			if (start_time == 0)
			{
				start_time = CL_System::get_time();
			}

			gc.set_map_mode(CL_MapMode(cl_map_2d_upper_left));

			// Draw a nice blue gradient in the background
			CL_Draw::gradient_fill(gc, window.get_viewport(), CL_Gradient(CL_Colorf::lightblue, CL_Colorf::lightblue, CL_Colorf::darkblue, CL_Colorf::darkblue));

			CL_String text(cl_format("Frames per second = %1", last_fps));

			const int num_iterations = 64;

			if (multi_framebuffer_flag)
			{
				for (int cnt=0; cnt < num_iterations; cnt++)
				{
					gc.set_frame_buffer(fb);

					gc.clear(CL_Colorf(0.0f, 0.0f, 0.0f, 1.0f));
					font.draw_text(gc, 20, 20, text, CL_Colorf::white);

					gc.reset_frame_buffer();
				}
			}
			else
			{
				for (int cnt=0; cnt < num_iterations; cnt++)
				{
					fb.attach_color_buffer(0, texture);

					if (cnt==0)
					{
						gc.set_frame_buffer(fb);
					}

					gc.clear(CL_Colorf(0.0f, 0.0f, 0.0f, 1.0f));
					font.draw_text(gc, 20, 20, text, CL_Colorf::white);
				}
			}

			gc.reset_frame_buffer();

			gc.set_texture(0, texture);
			CL_Draw::texture(gc, CL_Rectf(100.0f, 100.0f, CL_Sizef(500.0f, 300.0f)), CL_Colorf(1.0f, 1.0f, 1.0f, 0.7f));
			gc.reset_texture(0);

			const char text_a[] = "Press \"A\" for multiple framebuffers, single textures";
			const char text_b[] = "Press \"B\" for single framebuffer, multiple textures";

			if (multi_framebuffer_flag)
			{
				font.draw_text(gc, 20, 40, text_a, CL_Colorf::white);
				font.draw_text(gc, 20, 80, text_b, CL_Colorf::grey);
			}
			else
			{
				font.draw_text(gc, 20, 40, text_a, CL_Colorf::grey);
				font.draw_text(gc, 20, 80, text_b, CL_Colorf::white);
			}

			fps_count++;
			int time = CL_System::get_time();
			total_time += time - start_time;
			start_time = time;
			if(total_time >= 1000)
			{
				last_fps = fps_count;
				total_time -= 1000;
				fps_count = 0;
			}

			// Flip the display, showing on the screen what we have drawn
			window.flip(0);

			// This call updates input and performs other "housekeeping" call this each frame
			CL_KeepAlive::process();
		}
	}

	catch(CL_Exception& exception)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");

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

void ExampleText::on_keyboard_up(const CL_InputEvent &key, const CL_InputState &state)
{
	if(key.id == CL_KEY_ESCAPE)
		quit = true;

	if(key.id == CL_KEY_A)
		multi_framebuffer_flag = true;

	if(key.id == CL_KEY_B)
		multi_framebuffer_flag = false;
}
	
void ExampleText::on_window_close()
{
	quit = true;
}

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
		ExampleText app;
		return app.start(args);
	}
};

// Instantiate CL_ClanApplication, informing it where the Program is located
CL_ClanApplication app(&Program::main);
