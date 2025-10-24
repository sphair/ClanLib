
#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

class ExampleCanvas : public CL_ClanApplication
{
public:
	virtual int main(int argc, char **argv)
	{
		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();
		
		quit = false;

		try
		{
			// Initialize ClanLib base components
			CL_SetupCore setup_core;

			// Initialize the ClanLib display component
			CL_SetupDisplay setup_display;

			// Initilize the OpenGL drivers
			CL_SetupGL setup_gl;

			// Set a videomode - 640x480
			CL_DisplayWindow window("ClanLib Canvas Example", 1024, 256);

			// Connect the Window close event
			CL_Slot slot_quit = window.sig_window_close().connect(this, &ExampleCanvas::on_window_close);

			// Connect a keyboard handler to on_key_up()
			CL_Slot slot_keyboard_up = CL_Keyboard::sig_key_up().connect(this, &ExampleCanvas::on_keyboard_up);

			// Connect a mouse handler to on_key_down()
			CL_Slot slot_mouse_down = CL_Mouse::sig_key_up().connect(this, &ExampleCanvas::on_mouse_down);

			// Load the surfaces
			ground = CL_Surface("ground.png");
			ground_back = CL_Surface("ground.png");
			cutter = CL_Surface("circle1.png");
			cutter.set_alignment(origin_center);

			canvas_ground = new CL_Canvas(ground);

			int width = CL_Display::get_width();
			int height = CL_Display::get_height();
			
			scale = 1.0f;
			int back_pos = 0;

			// Run until someone presses escape
			while (!quit)
			{
				// Draw a nice blue gradient in the background
				CL_Display::fill_rect(
					CL_Rect(0, 0, width, height), 
					CL_Gradient(CL_Color::aliceblue, CL_Color::aliceblue, CL_Color::cornflowerblue, CL_Color::cornflowerblue));
				
				// Draw the moving background
				back_pos++;
				ground_back.draw(int(sinf(back_pos / 100.0f) * 100.0f), 0);

				// Draw the ground
				ground.draw(0, 0);

				// Draw mousecursor
				cutter.draw(CL_Mouse::get_x(), CL_Mouse::get_y());

				// Flip the display, showing on the screen what we have drawed
				// since last call to flip_display()
				CL_Display::flip();

				// This call updates input and performs other "housekeeping"
				// call this each frame
				CL_System::keep_alive(10);
			}

			delete canvas_ground;

			ground = CL_Surface();
			cutter = CL_Surface();
			ground_back = CL_Surface();
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

private:
	void on_keyboard_up(const CL_InputEvent &key)
	{
		if(key.id == CL_KEY_ESCAPE)
			quit = true;
	}

	void on_mouse_down(const CL_InputEvent &key)
	{
		if(key.id == CL_MOUSE_LEFT)
		{
			// Set the cutter surface to extract instead of replace colours
			cutter.set_blend_func(blend_zero, blend_one_minus_src_alpha);
			cutter.draw(CL_Mouse::get_x(), CL_Mouse::get_y(), canvas_ground->get_gc());

			// Set cutter back to normal (for showing the mousecursor)
			cutter.set_blend_func(blend_src_alpha, blend_one_minus_src_alpha);
		}

		if(key.id == CL_MOUSE_WHEEL_UP)
		{
			scale = scale * 1.1f;
			cutter.set_scale(scale, scale);
		}

		if(key.id == CL_MOUSE_WHEEL_DOWN)
		{
			scale = scale * 0.9f;
			cutter.set_scale(scale, scale);
		}
	}

	void on_window_close()
	{
		quit = true;
	}

	bool quit;
	float scale;

	CL_Surface ground;
	CL_Surface cutter;
	CL_Surface ground_back;
	CL_Canvas *canvas_ground;
} my_app;
