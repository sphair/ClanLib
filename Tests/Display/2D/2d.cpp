#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>

#define USE_SWRENDER

#ifdef USE_SWRENDER
#include <ClanLib/swrender.h>
#else
#include <ClanLib/gl.h>
#endif

#define GL_MULTISAMPLE                    0x809D


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
#ifdef USE_SWRENDER
		CL_SetupSWRender setup_swrender;
#else
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
	CL_ConsoleWindow console("Console", 80, 200);

	try
	{
		// Set the window
		// This opens a 640 x 480 window, including the frame size
		// If you want more control over the window, pass CL_DisplayWindowDescription to CL_DisplayWindow
		// (This is useful to create a borderless window of a specific size)
		// If you require target specific control over the window, use the derived CL_OpenGLWindowDescription
		// (This contains the multisampling options)
#ifdef USE_SWRENDER
		CL_DisplayWindowDescription desc;
#else
		CL_OpenGLWindowDescription desc;
//		desc.set_multisampling(4);
#endif
		desc.set_title("ClanLib 2D Test");
		desc.set_size(CL_Size(800, 600), true);
		CL_DisplayWindow window(desc);

		// Connect the Window close event
		CL_Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close);

		// Connect a keyboard handler to on_key_up()
		CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

		// Get the graphic context
		CL_GraphicContext gc = window.get_gc();

		CL_Texture texture_image(gc, "tux.png");
		texture_image.set_wrap_mode(cl_wrap_repeat, cl_wrap_repeat, cl_wrap_repeat);
		texture_image.set_min_filter(cl_filter_linear);
		texture_image.set_mag_filter(cl_filter_linear);

		CL_ResourceManager resources("resources.xml");
		CL_Sprite sprite(gc, "test", &resources);
		//sprite.set_linear_filter(true);

		CL_Font small_font = CL_Font(gc, "Tahoma", 12);

		float test_base_angle = 0.0f;
		float test_angle = 0.0f;
		float test_angle_pitch = 0.0f;
		float test_angle_yaw = 0.0f;
		float test_scale = 1.0f;
		bool test_scale_dir = false;

		// Run until someone presses escape
		while (!quit)
		{
			gc.clear(CL_Colorf(0.0f,0.0f,0.2f));

			gc.set_map_mode(CL_MapMode(cl_map_2d_upper_left));

	// CL_Draw::point()
			for (int xcnt=0; xcnt<8; xcnt++)
			{
				for (int ycnt=0; ycnt<6; ycnt++)
				{
					CL_Draw::point(gc, xcnt*2, ycnt*2, CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f));
				}
			}
			small_font.draw_text(gc, 32, 10, "8*6 Points (0 + 2x), (0 + 2y)");

	// CL_Draw::line()
			for (int xcnt=0; xcnt<4; xcnt++)
			{
				for (int ycnt=0; ycnt<3; ycnt++)
				{
					const int offset_y = 16;
					const int line_length = 6;
					const int spacing = 8;
					CL_Draw::line(gc, xcnt*spacing, (ycnt*spacing) + offset_y, line_length + (xcnt*spacing), (line_length + (ycnt*spacing)) + offset_y, CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f));
				}
			}
			small_font.draw_text(gc, 48, 30, "4*3 Lines (0 + 8x), (32 + 8y), (6 + 8x), (38 + 8y)");

	// CL_Draw::box()
			for (int xcnt=0; xcnt<4; xcnt++)
			{
				for (int ycnt=0; ycnt<3; ycnt++)
				{
					const int offset_y = 48;
					const int line_length = 6;
					const int spacing = 8;
					CL_Draw::box(gc, xcnt*spacing, (ycnt*spacing) + offset_y, line_length + (xcnt*spacing), (line_length + (ycnt*spacing)) + offset_y, CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f));
				}
			}
			small_font.draw_text(gc, 48, 66, "4*3 Box (0 + 8x), (32 + 8y), (6 + 8x), (38 + 8y)");

	// CL_Draw::fill()
			for (int xcnt=0; xcnt<4; xcnt++)
			{
				for (int ycnt=0; ycnt<3; ycnt++)
				{
					const int offset_y = 80;
					const int line_length = 6;
					const int spacing = 8;
					CL_Draw::fill(gc, xcnt*spacing, (ycnt*spacing) + offset_y, line_length + (xcnt*spacing), (line_length + (ycnt*spacing)) + offset_y, CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f));
				}
			}
			small_font.draw_text(gc, 48, 90, "4*3 Fill (0 + 8x), (32 + 8y), (6 + 8x), (38 + 8y)");

	// CL_Draw::gradient_fill()
			CL_Gradient gradient;
			gradient.top_left = CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f);
			gradient.top_right = CL_Colorf(1.0f, 0.0f, 0.0f, 1.0f);
			gradient.bottom_left = CL_Colorf(0.0f, 0.0f, 1.0f, 1.0f);
			gradient.bottom_right = CL_Colorf(0.0f, 1.0f, 0.0f, 1.0f);
			for (int xcnt=0; xcnt<4; xcnt++)
			{
				for (int ycnt=0; ycnt<3; ycnt++)
				{
					const int offset_y = 110;
					const int line_length = 6;
					const int spacing = 8;
					CL_Draw::gradient_fill(gc, xcnt*spacing, (ycnt*spacing) + offset_y, line_length + (xcnt*spacing), (line_length + (ycnt*spacing)) + offset_y, gradient);
				}
			}
			small_font.draw_text(gc, 48, 115, "4*3 GradientFill (0 + 8x), (32 + 8y), (6 + 8x), (38 + 8y)");
			small_font.draw_text(gc, 48, 125, "top left = white. top right = red");
			small_font.draw_text(gc, 48, 135, "bottom left = blue. bottom right = green");

	// CL_Draw::circle()
			{
				const int offset_y = 140;
				int radius = 5;
				CL_Draw::circle(gc, radius, offset_y + radius, radius, CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f));

				const int offset_x = 16;
				radius = 16;
				CL_Draw::circle(gc, offset_x + radius, offset_y + radius, radius, CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f));
			}
			small_font.draw_text(gc, 54, 149, "Circle (5, 145) Radius = 5");
			small_font.draw_text(gc, 54, 159, "Circle (32, 156) Radius = 16");

	// CL_Draw::gradient_circle()
			{
				CL_Gradient gradient;
				gradient.top_left = CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f);
				gradient.top_right = CL_Colorf(0.0f, 0.0f, 0.0f, 1.0f);
				gradient.bottom_left = CL_Colorf(0.0f, 0.0f, 0.0f, 1.0f);
				gradient.bottom_right = CL_Colorf(1.0f, 0.0f, 0.0f, 1.0f);

				const int offset_y = 180;
				float radius = 17.0;
				float xpos = radius;
				float ypos = offset_y + radius;
				CL_Draw::gradient_circle(gc, CL_Pointf( xpos, ypos ), CL_Pointf(radius/2.0, 0.0f), radius, gradient);

				const int offset_x = 40;
				radius = 17.0;
				xpos = offset_x + radius;
				ypos = offset_y + radius;
				CL_Draw::gradient_circle(gc, CL_Pointf( xpos, ypos), CL_Pointf(0.0f, radius/2.0), radius, gradient);
			}

			small_font.draw_text(gc, 80, 189, "Gradient Circle (16, 196) Radius = 17. Gradient right");
			small_font.draw_text(gc, 80, 199, "Gradient Circle (56, 196) Radius = 17. Gradient up");
			small_font.draw_text(gc, 80, 209, "centre = white, outer = red");

	// CL_Draw::triangle()
			{
				const float offset_y = 220.0f;
				const float size = 12.0f;
				CL_Draw::triangle(gc, CL_Pointf(0.0f, offset_y), CL_Pointf(0.0f, offset_y + size), CL_Pointf(size, offset_y + size), CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f));

				float offset_x = 16.0f;
				CL_Draw::triangle(gc, CL_Pointf(offset_x + 0.0f, offset_y + size), CL_Pointf(offset_x + size, offset_y + size), CL_Pointf(offset_x + 0.0f, offset_y), CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f));

				offset_x = 32.0f;
				CL_Draw::triangle(gc, CL_Pointf(offset_x + size, offset_y + size), CL_Pointf(offset_x + 0.0f, offset_y), CL_Pointf(offset_x + 0.0f, offset_y + size), CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f));
			}
			small_font.draw_text(gc, 48, 229, "3 Triangles (12 pixel size) (Left vertical edge).");
			small_font.draw_text(gc, 48, 239, "Top Left: (0,220)  (16,220)  (32,220)");

	// CL_Draw::texture()
			gc.set_texture(0, texture_image);
			{
				float offset_x = 0.0f;
				float offset_y = 250.0f;
				CL_Rectf src_rect(offset_x, offset_y, CL_Sizef(31, 47));
				CL_Rectf texture_coords(0.0, 0.0, 1.0f, 1.0f);
				CL_Colorf color(1.0f, 1.0f, 1.0f, 1.0f);
				CL_Draw::texture(gc, src_rect, color, texture_coords);

				offset_x = 33.0f;
				src_rect = CL_Rectf(offset_x, offset_y, CL_Sizef(31, 47));
				texture_coords = CL_Rectf(0.25f, 0.25f, 0.75f, 0.75f);
				color = CL_Colorf(1.0f, 0.0f, 0.0f, 1.0f);
				CL_Draw::texture(gc, src_rect, color, texture_coords);
			}
			gc.reset_texture(0);
			small_font.draw_text(gc, 76, 260, "Texture (0, 250) size=(31,47)");
			small_font.draw_text(gc, 76, 275, "Texture (33, 250) size=(31,47) (red, magnify*2)");

	// CL_RoundedRect
			{
				CL_RoundedRect roundedrect(CL_Sizef(64.0f, 32.0f), 15.0f);

				float offset_x = 0.0f;
				float offset_y = 300.0f;
				CL_Origin origin = origin_top_left;
				roundedrect.draw(gc, CL_Pointf(offset_x, offset_y), CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f), origin);

				offset_y = 340.0f;
				roundedrect.fill(gc, CL_Pointf(offset_x, offset_y), CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f), origin);

				offset_y = 380.0f;
				CL_Gradient gradient;
				gradient.top_left = CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f);
				gradient.top_right = CL_Colorf(1.0f, 0.0f, 0.0f, 1.0f);
				gradient.bottom_left = CL_Colorf(0.0f, 0.0f, 1.0f, 1.0f);
				gradient.bottom_right = CL_Colorf(0.0f, 1.0f, 0.0f, 1.0f);
				roundedrect.fill(gc, CL_Pointf(offset_x, offset_y), gradient, origin);

				offset_y = 420.0f;
				roundedrect.set_control_point_bl(CL_Pointf(0.4f, 0.8f));
				roundedrect.set_rounding_bottom_left(CL_Sizef(0.2f, 0.6f));

				roundedrect.set_control_point_tl(CL_Pointf(0.2f, 0.4f));
				roundedrect.set_rounding_top_left(CL_Sizef(0.4f, 0.2f));

				roundedrect.set_control_point_tr(CL_Pointf(0.6f, 0.2f));
				roundedrect.set_rounding_top_right(CL_Sizef(0.8f, 0.4f));
				roundedrect.set_control_point_br(CL_Pointf(0.6f, 0.8f));
				roundedrect.set_rounding_bottom_right(CL_Sizef(0.8f, 0.6f));

				roundedrect.fill(gc, CL_Pointf(offset_x, offset_y), gradient, origin);

			}
			small_font.draw_text(gc, 76, 310, "RoundedRect - draw (0, 300) size=(64,32)");
			small_font.draw_text(gc, 76, 325, "(RoundedRect - draw gradient - is not implemented)");
			small_font.draw_text(gc, 76, 350, "RoundedRect - fill (0, 340) size=(64,32)");
			small_font.draw_text(gc, 76, 390, "RoundedRect - fill gradient (0, 380) size=(64,32)");
			small_font.draw_text(gc, 76, 400, "top left = white. top right = red");
			small_font.draw_text(gc, 76, 410, "bottom left = blue. bottom right = green");
			small_font.draw_text(gc, 76, 430, "RoundedRect - fill gradient (0, 420) size=(64,32)");
			small_font.draw_text(gc, 76, 440, "Controling control / rounding points");

	// CL_Sprite
			{
				test_base_angle+=5.0f;
				if (test_base_angle >= 360.0f)
				{
					test_base_angle = 0.0f;
				}
#ifndef USE_SWRENDER
				clEnable(GL_MULTISAMPLE);
#endif
				sprite.set_base_angle(CL_Angle(test_base_angle, cl_degrees));
				sprite.draw(gc, 350, 20);
				sprite.set_base_angle(CL_Angle(0, cl_degrees));

#ifndef USE_SWRENDER
				clDisable(GL_MULTISAMPLE);
#endif
			}
			small_font.draw_text(gc, 370, 20, "Sprite - Base angle");
			small_font.draw_text(gc, 370, 35, "Multisampling enabled");

			{
				test_angle+=5.0f;
				if (test_angle >= 360.0f)
				{
					test_angle = 0.0f;
				}

				sprite.set_angle(CL_Angle(test_angle, cl_degrees));
				sprite.draw(gc, 350, 60);
				sprite.set_angle(CL_Angle(0, cl_degrees));
			}
			small_font.draw_text(gc, 370, 60, "Sprite - Angle");

			{
				test_angle_pitch+=5.0f;
				if (test_angle_pitch >= 360.0f)
				{
					test_angle_pitch = 0.0f;
				}

				sprite.set_angle_pitch(CL_Angle(test_angle_pitch, cl_degrees));
				sprite.draw(gc, 350, 100);
				sprite.set_angle_pitch(CL_Angle(0, cl_degrees));
			}
			small_font.draw_text(gc, 370, 100, "Sprite - Angle Pitch");

			{
				test_angle_yaw+=5.0f;
				if (test_angle_yaw >= 360.0f)
				{
					test_angle_yaw = 0.0f;
				}

				sprite.set_angle_yaw(CL_Angle(test_angle_yaw, cl_degrees));
				sprite.draw(gc, 350, 140);
				sprite.set_angle_yaw(CL_Angle(0, cl_degrees));
			}
			small_font.draw_text(gc, 370, 140, "Sprite - Angle Yaw");

			{
				if (test_scale_dir)
				{
					test_scale += 0.1f;
					if (test_scale >= 2.0f)
					{
						test_scale = 2.0f;
						test_scale_dir = false;
					}
				}else
				{
					test_scale -= 0.1f;
					if (test_scale <= -2.0f)
					{
						test_scale = -2.0f;
						test_scale_dir = true;
					}
				}

				sprite.set_scale(test_scale, 1.0f);
				sprite.draw(gc, 350, 180);
				sprite.set_scale(1.0f, test_scale);
				sprite.draw(gc, 390, 180);
				sprite.set_scale(1.0f, 1.0f);
			}
			small_font.draw_text(gc, 420, 180, "Sprite - Set Scale (x), (y)");

			// Flip the display, showing on the screen what we have drawed
			// since last call to flip()
			window.flip(1);

			// This call processes user input and other events
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


