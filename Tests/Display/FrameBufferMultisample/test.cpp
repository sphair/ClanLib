
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
		desc.set_title("ClanLib FrameBuffer Multisampling and blitting test.");
		desc.set_size(CL_Size(800, 600), true);
		CL_DisplayWindow window(desc);

		// Connect the Window close event
		CL_Slot slot_quit = window.sig_window_close().connect(this, &ExampleText::on_window_close);

		// Connect a keyboard handler to on_key_up()
		CL_Slot slot_keyboard_up = window.get_ic().get_keyboard().sig_key_up().connect(this, &ExampleText::on_keyboard_up);

		CL_GraphicContext gc = window.get_gc();

		CL_Texture texture(gc, 512, 512, cl_rgb8);
		texture.set_wrap_mode(cl_wrap_repeat, cl_wrap_repeat, cl_wrap_repeat);
		texture.set_min_filter(cl_filter_nearest);
		texture.set_mag_filter(cl_filter_nearest);

		CLenum error = clGetError();

		gc.reset_texture(0);
		gc.reset_texture(1);

		// Renderbuffer with 8x multisampling.
		CL_RenderBuffer rb(gc, 512, 512, cl_rgba, 8);

		// Create the frame buffer and attach the renderbuffer as color buffer 0.
		CL_FrameBuffer fb_rb(gc);		
		fb_rb.attach_color_buffer(0, rb);
		gc.set_frame_buffer(fb_rb);

		// Clear the renderbuffer and draw a triangle.
		gc.clear(CL_Colorf::darkkhaki);
		CL_Draw::triangle(gc, CL_Pointf(10,10), CL_Pointf(500,50), CL_Pointf(10,100), CL_Colorf::darkblue);
		fb_rb.detach_color_buffer(0, rb);
		gc.reset_frame_buffer();

		// Blit multisampled renderbuffer to rgba texture.
		fb_rb.set_bind_target(cl_framebuffer_read);
		fb_rb.attach_color_buffer(0, rb);

		CL_FrameBuffer fb_tex(gc);
		fb_tex.attach_color_buffer(0, texture);
		gc.set_frame_buffer(fb_tex, fb_rb); // fb_tex for writing, fb_rb for reading.

		clBlitFramebuffer(0,0,512,512, 0,0,512,512, CL_COLOR_BUFFER_BIT, CL_NEAREST);

		fb_rb.detach_color_buffer(0, rb);
		fb_tex.detach_color_buffer(0, texture);
		gc.reset_frame_buffer();

		// Run until someone presses escape
		gc.set_texture(0, texture);

		while (!window.get_ic().get_keyboard().get_keycode(CL_KEY_ESCAPE))
		{
			gc.clear(CL_Colorf::gray);
			CL_Draw::texture(gc, CL_Rectf(0,0,512,512));

			window.flip(0);
			CL_KeepAlive::process();
		}
	}

	catch (CL_Exception& exception)
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
