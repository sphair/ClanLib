
#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

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
			desc.set_title("ClanLib Draw To Texture Example");
			desc.set_size(CL_Size(1024, 256), true);
			CL_DisplayWindow window(desc);

			// erasing blend mode
			blend_erase.enable_blending(true);
			blend_erase.set_blend_function(
				cl_blend_zero, cl_blend_one_minus_src_alpha,
				cl_blend_zero, cl_blend_one_minus_src_alpha );

			// Connect the Window close event
			CL_Slot slot_quit = window.sig_window_close().connect(this, &ExampleCanvas::on_window_close);

			// Connect a keyboard handler to on_key_up()
			CL_Slot slot_keyboard_up = window.get_ic().get_keyboard().sig_key_up().connect(this, &ExampleCanvas::on_keyboard_up);

			// Connect a mouse handler to on_key_down()
			CL_Slot slot_mouse_down = window.get_ic().get_mouse().sig_key_up().connect(this, &ExampleCanvas::on_mouse_down);
			CL_Slot slot_mouse_move = window.get_ic().get_mouse().sig_pointer_move().connect(this, &ExampleCanvas::on_mouse_move);

			gc = window.get_gc();

			// Load the surfaces
			ground = CL_Texture(gc, "ground.png");
			ground_back = CL_Texture(gc, "ground.png");
			cutter = CL_Texture(gc, "circle1.png");

			// Create the framebuffer, and attach ground texture into its color buffer
			fb_ground = CL_FrameBuffer(gc);
			fb_ground.attach_color_buffer(0, ground);

			float back_pos = 0;
			scale = 1.0f;
			rect_cutter = CL_Rectf(cutter.get_size());

			// Run until someone presses escape
			while (!quit)
			{
				// Draw a nice blue gradient in the background
				CL_Draw::gradient_fill(gc, window.get_viewport(), CL_Gradient(CL_Colorf::lightblue, CL_Colorf::lightblue, CL_Colorf::darkblue, CL_Colorf::darkblue));

				// Draw the moving background
				back_pos+= 1.0f;
				gc.set_texture(0, ground_back);
				gc.push_translate(sinf(back_pos/100.0f)*100.0f, -20.0f);
				CL_Draw::texture(gc, CL_Rect(ground.get_size()), CL_Colorf(0.5f, 0.5f, 0.5f, 1.0f));
				gc.pop_modelview();

				// Draw the ground
				gc.set_texture(0, ground);
				CL_Draw::texture(gc, CL_Rect(ground.get_size()));

				// Draw mouse cursor
				gc.set_texture(0, cutter);
				gc.push_translate(mouse_pos.x - rect_cutter.get_center().x * scale, mouse_pos.y - rect_cutter.get_center().y * scale);
				gc.mult_scale(scale,scale);  
				CL_Draw::texture(gc, rect_cutter);
				gc.pop_modelview();

				gc.reset_texture(0);

				// Flip the display, showing on the screen what we have drawn
				window.flip(1);

				// This call updates input and performs other "housekeeping" call this each frame
				CL_KeepAlive::process();
			}

			// Cleanup
			ground = CL_Texture();
			cutter = CL_Texture();
			ground_back = CL_Texture();
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

private:
	void on_keyboard_up(const CL_InputEvent &key, const CL_InputState &state)
	{
		if(key.id == CL_KEY_ESCAPE)
			quit = true;
	}

	void on_mouse_move(const CL_InputEvent &key, const CL_InputState &state)
	{
		mouse_pos = key.mouse_pos;
	}

	void on_mouse_down(const CL_InputEvent &key, const CL_InputState &state)
	{
		if(key.id == CL_MOUSE_LEFT)
		{
			gc.set_frame_buffer(fb_ground);
  
			// Set the cutter surface to extract instead of replace colors
			gc.set_blend_mode(blend_erase);

			gc.set_texture(0, cutter); 

			gc.push_translate(mouse_pos.x - rect_cutter.get_center().x * scale, mouse_pos.y - rect_cutter.get_center().y * scale);
			gc.mult_scale(scale,scale);  
			CL_Draw::texture(gc, rect_cutter);
			gc.pop_modelview();

			gc.reset_frame_buffer();
			gc.reset_blend_mode();
			gc.reset_texture(0);
		}
		
		if(key.id == CL_MOUSE_WHEEL_UP)
		{
			scale = scale * 1.1f;
		}

		if(key.id == CL_MOUSE_WHEEL_DOWN)
		{
			scale = scale * 0.9f;
		}
	}

	void on_window_close()
	{
		quit = true;
	}

	CL_Texture ground;
	CL_Texture ground_back;
	CL_Texture cutter;

	CL_FrameBuffer fb_ground;

	CL_GraphicContext gc;

	CL_BlendMode blend_erase;

	CL_Rectf rect_cutter;

	CL_Point mouse_pos;

	float scale;

	bool mouse_down;

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
		ExampleCanvas app;
		return app.start(args);
	}
};

// Instantiate CL_ClanApplication, informing it where the Program is located
CL_ClanApplication app(&Program::main);
