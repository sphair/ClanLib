#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
using namespace clan;

//#define USE_D3D

#ifdef USE_D3D
#include <ClanLib/d3d.h>
#else
#include <ClanLib/gl.h>
#endif

#define GL_MULTISAMPLE                    0x809D

// This is the Application class (That is instantiated by the Program Class)
class App
{
public:
	int start(const std::vector<std::string> &args);

private:
	void on_input_up(const InputEvent &key);
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
#ifdef USE_D3D
		SetupD3D setup_d3d;
#else
		SetupGL setup_gl;
#endif
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
#ifdef USE_D3D
		DisplayWindowDescription desc;
#else
		OpenGLWindowDescription desc;
//		desc.set_multisampling(4);
#endif
		desc.set_title("ClanLib 2D Test");
		desc.set_size(Size(800, 600), true);
		DisplayWindow window(desc);

		// Connect the Window close event
		Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close);

		// Connect a keyboard handler to on_key_up()
		Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

		// Create the Canvas
		Canvas canvas(window);

		//Set the reference to GraphicContext
		GraphicContext &gc = canvas.get_gc();

		Texture2D texture_image(gc, "tux.png");
		texture_image.set_wrap_mode(wrap_repeat, wrap_repeat);
		texture_image.set_min_filter(filter_linear);
		texture_image.set_mag_filter(filter_linear);

		Image image(gc,texture_image,texture_image.get_size());

		ResourceManager resources("resources.xml");
		Sprite sprite(gc, "test", &resources);
		//sprite.set_linear_filter(true);

		Font small_font = Font(gc, "Tahoma", 12);

		float test_base_angle = 0.0f;
		float test_angle = 0.0f;
		float test_angle_pitch = 0.0f;
		float test_angle_yaw = 0.0f;
		float test_scale = 1.0f;
		bool test_scale_dir = false;

		// Run until someone presses escape
		while (!quit)
		{
			canvas.clear(Colorf(0.0f,0.0f,0.2f));

			canvas.set_map_mode(MapMode(map_2d_upper_left));

	// Canvas::point()
			for (int xcnt=0; xcnt<8; xcnt++)
			{
				for (int ycnt=0; ycnt<6; ycnt++)
				{
					canvas.point(xcnt*2, ycnt*2, Colorf(1.0f, 1.0f, 1.0f, 1.0f));
				}
			}
			small_font.draw_text(canvas, 32, 10, "8*6 Points (0 + 2x), (0 + 2y)");

	// Canvas::line()
			for (int xcnt=0; xcnt<4; xcnt++)
			{
				for (int ycnt=0; ycnt<3; ycnt++)
				{
					const int offset_y = 16;
					const int line_length = 6;
					const int spacing = 8;
					canvas.line(xcnt*spacing, (ycnt*spacing) + offset_y, line_length + (xcnt*spacing), (line_length + (ycnt*spacing)) + offset_y, Colorf(1.0f, 1.0f, 1.0f, 1.0f));
				}
			}
			small_font.draw_text(canvas, 48, 30, "4*3 Lines (0 + 8x), (32 + 8y), (6 + 8x), (38 + 8y)");

	// Canvas::box()
			for (int xcnt=0; xcnt<4; xcnt++)
			{
				for (int ycnt=0; ycnt<3; ycnt++)
				{
					const int offset_y = 48;
					const int line_length = 6;
					const int spacing = 8;
					canvas.box(xcnt*spacing, (ycnt*spacing) + offset_y, line_length + (xcnt*spacing), (line_length + (ycnt*spacing)) + offset_y, Colorf(1.0f, 1.0f, 1.0f, 1.0f));
				}
			}
			small_font.draw_text(canvas, 48, 66, "4*3 Box (0 + 8x), (32 + 8y), (6 + 8x), (38 + 8y)");

	// Canvas::fill()
			for (int xcnt=0; xcnt<4; xcnt++)
			{
				for (int ycnt=0; ycnt<3; ycnt++)
				{
					const int offset_y = 80;
					const int line_length = 6;
					const int spacing = 8;
					canvas.fill(xcnt*spacing, (ycnt*spacing) + offset_y, line_length + (xcnt*spacing), (line_length + (ycnt*spacing)) + offset_y, Colorf(1.0f, 1.0f, 1.0f, 1.0f));
				}
			}
			small_font.draw_text(canvas, 48, 90, "4*3 Fill (0 + 8x), (32 + 8y), (6 + 8x), (38 + 8y)");
		
	// Canvas::gradient_fill()
			Gradient gradient;
			gradient.top_left = Colorf(1.0f, 1.0f, 1.0f, 1.0f);
			gradient.top_right = Colorf(1.0f, 0.0f, 0.0f, 1.0f);
			gradient.bottom_left = Colorf(0.0f, 0.0f, 1.0f, 1.0f);
			gradient.bottom_right = Colorf(0.0f, 1.0f, 0.0f, 1.0f);
			for (int xcnt=0; xcnt<4; xcnt++)
			{
				for (int ycnt=0; ycnt<3; ycnt++)
				{
					const int offset_y = 110;
					const int line_length = 6;
					const int spacing = 8;
					canvas.gradient_fill(xcnt*spacing, (ycnt*spacing) + offset_y, line_length + (xcnt*spacing), (line_length + (ycnt*spacing)) + offset_y, gradient);
				}
			}
			small_font.draw_text(canvas, 48, 115, "4*3 GradientFill (0 + 8x), (32 + 8y), (6 + 8x), (38 + 8y)");
			small_font.draw_text(canvas, 48, 125, "top left = white. top right = red");
			small_font.draw_text(canvas, 48, 135, "bottom left = blue. bottom right = green");

	// Canvas::circle()
			{
				const int offset_y = 140;
				int radius = 5;
				canvas.circle(radius, offset_y + radius, radius, Colorf(1.0f, 1.0f, 1.0f, 1.0f));

				const int offset_x = 16;
				radius = 16;
				canvas.circle(offset_x + radius, offset_y + radius, radius, Colorf(1.0f, 1.0f, 1.0f, 1.0f));
			}
			small_font.draw_text(canvas, 54, 149, "Circle (5, 145) Radius = 5");
			small_font.draw_text(canvas, 54, 159, "Circle (32, 156) Radius = 16");

	// Canvas::gradient_circle()
			{
				Gradient gradient;
				gradient.top_left = Colorf(1.0f, 1.0f, 1.0f, 1.0f);
				gradient.top_right = Colorf(0.0f, 0.0f, 0.0f, 1.0f);
				gradient.bottom_left = Colorf(0.0f, 0.0f, 0.0f, 1.0f);
				gradient.bottom_right = Colorf(1.0f, 0.0f, 0.0f, 1.0f);

				const int offset_y = 180;
				float radius = 17.0;
				float xpos = radius;
				float ypos = offset_y + radius;
				canvas.gradient_circle(Pointf( xpos, ypos ), Pointf(radius/2.0, 0.0f), radius, gradient);

				const int offset_x = 40;
				radius = 17.0;
				xpos = offset_x + radius;
				ypos = offset_y + radius;
				canvas.gradient_circle(Pointf( xpos, ypos), Pointf(0.0f, radius/2.0), radius, gradient);
			}

			small_font.draw_text(canvas, 80, 189, "Gradient Circle (16, 196) Radius = 17. Gradient right");
			small_font.draw_text(canvas, 80, 199, "Gradient Circle (56, 196) Radius = 17. Gradient up");
			small_font.draw_text(canvas, 80, 209, "centre = white, outer = red");
	
	// Canvas::triangle()
			{
				const float offset_y = 220.0f;
				const float size = 12.0f;
				canvas.triangle(Pointf(0.0f, offset_y), Pointf(0.0f, offset_y + size), Pointf(size, offset_y + size), Colorf(1.0f, 1.0f, 1.0f, 1.0f));

				float offset_x = 16.0f;
				canvas.triangle(Pointf(offset_x + 0.0f, offset_y + size), Pointf(offset_x + size, offset_y + size), Pointf(offset_x + 0.0f, offset_y), Colorf(1.0f, 1.0f, 1.0f, 1.0f));

				offset_x = 32.0f;
				canvas.triangle(Pointf(offset_x + size, offset_y + size), Pointf(offset_x + 0.0f, offset_y), Pointf(offset_x + 0.0f, offset_y + size), Colorf(1.0f, 1.0f, 1.0f, 1.0f));
			}
			small_font.draw_text(canvas, 48, 229, "3 Triangles (12 pixel size) (Left vertical edge).");
			small_font.draw_text(canvas, 48, 239, "Top Left: (0,220)  (16,220)  (32,220)");

	// Canvas::image()

			Size image_size = image.get_size();
			float offset_x = 0.0f;
			float offset_y = 250.0f;
			Rectf dest_rect= Rectf(offset_x, offset_y, Sizef(31, 47));
			Rectf src_rect= Rectf(0.0*image_size.width, 
								  0.0*image_size.width,
								  1.0f*image_size.height,
								  1.0f*image_size.height);
			Colorf color(1.0f, 1.0f, 1.0f, 1.0f);
				
			image.set_color(color);
			image.draw(canvas,src_rect,dest_rect);

			offset_x = 33.0f;

			dest_rect = Rectf(offset_x, offset_y, Sizef(31, 47));
			src_rect = Rectf(0.25f*image_size.width,
							 0.25f*image_size.width,	
							 0.75f*image_size.height, 
							 0.75f*image_size.height);
			color = Colorf(1.0f, 0.0f, 0.0f, 1.0f);

			image.set_color(color);
			image.draw(canvas,src_rect,dest_rect);
			
			small_font.draw_text(canvas, 76, 260, "Image (0, 250) size=(31,47)");
			small_font.draw_text(canvas, 76, 275, "Image (33, 250) size=(31,47) (red, magnify*2)");

	// RoundedRect
			{
				RoundedRect roundedrect(Sizef(64.0f, 32.0f), 15.0f);

				float offset_x = 0.0f;
				float offset_y = 300.0f;
				Origin origin = origin_top_left;
				roundedrect.draw(canvas, Pointf(offset_x, offset_y), Colorf(1.0f, 1.0f, 1.0f, 1.0f), origin);

				offset_y = 340.0f;
				roundedrect.fill(canvas, Pointf(offset_x, offset_y), Colorf(1.0f, 1.0f, 1.0f, 1.0f), origin);

				offset_y = 380.0f;
				Gradient gradient;
				gradient.top_left = Colorf(1.0f, 1.0f, 1.0f, 1.0f);
				gradient.top_right = Colorf(1.0f, 0.0f, 0.0f, 1.0f);
				gradient.bottom_left = Colorf(0.0f, 0.0f, 1.0f, 1.0f);
				gradient.bottom_right = Colorf(0.0f, 1.0f, 0.0f, 1.0f);
				roundedrect.fill(canvas, Pointf(offset_x, offset_y), gradient, origin);

				offset_y = 420.0f;
				roundedrect.set_control_point_bl(Pointf(0.4f, 0.8f));
				roundedrect.set_rounding_bottom_left(Sizef(0.2f, 0.6f));

				roundedrect.set_control_point_tl(Pointf(0.2f, 0.4f));
				roundedrect.set_rounding_top_left(Sizef(0.4f, 0.2f));

				roundedrect.set_control_point_tr(Pointf(0.6f, 0.2f));
				roundedrect.set_rounding_top_right(Sizef(0.8f, 0.4f));
				roundedrect.set_control_point_br(Pointf(0.6f, 0.8f));
				roundedrect.set_rounding_bottom_right(Sizef(0.8f, 0.6f));

				roundedrect.fill(canvas, Pointf(offset_x, offset_y), gradient, origin);

			}

			small_font.draw_text(canvas, 76, 310, "RoundedRect - draw (0, 300) size=(64,32)");
			small_font.draw_text(canvas, 76, 325, "(RoundedRect - draw gradient - is not implemented)");
			small_font.draw_text(canvas, 76, 350, "RoundedRect - fill (0, 340) size=(64,32)");
			small_font.draw_text(canvas, 76, 390, "RoundedRect - fill gradient (0, 380) size=(64,32)");
			small_font.draw_text(canvas, 76, 400, "top left = white. top right = red");
			small_font.draw_text(canvas, 76, 410, "bottom left = blue. bottom right = green");
			small_font.draw_text(canvas, 76, 430, "RoundedRect - fill gradient (0, 420) size=(64,32)");
			small_font.draw_text(canvas, 76, 440, "Controling control / rounding points");

	// Sprite
			{
				test_base_angle+=5.0f;
				if (test_base_angle >= 360.0f)
				{
					test_base_angle = 0.0f;
				}
#ifndef USE_D3D
				glEnable(GL_MULTISAMPLE);
#endif
				sprite.set_base_angle(Angle(test_base_angle, angle_degrees));
				sprite.draw(canvas, 350, 20);
				sprite.set_base_angle(Angle(0, angle_degrees));

#ifndef USE_D3D
				glDisable(GL_MULTISAMPLE);
#endif
			}
			small_font.draw_text(canvas, 370, 20, "Sprite - Base angle");
			small_font.draw_text(canvas, 370, 35, "Multisampling enabled");

			{
				test_angle+=5.0f;
				if (test_angle >= 360.0f)
				{
					test_angle = 0.0f;
				}

				sprite.set_angle(Angle(test_angle, angle_degrees));
				sprite.draw(canvas, 350, 60);
				sprite.set_angle(Angle(0, angle_degrees));
			}
			small_font.draw_text(canvas, 370, 60, "Sprite - Angle");
			
				//Sprite yaw and pitch is not supported in ClanLib 3.0.
				//The same effect can be achieved with the scaling feature.

			/*
			{
				test_angle_pitch+=5.0f;
				if (test_angle_pitch >= 360.0f)
				{
					test_angle_pitch = 0.0f;
				}

				sprite.set_angle_pitch(Angle(test_angle_pitch, angle_degrees));
				sprite.draw(canvas, 350, 100);
				sprite.set_angle_pitch(Angle(0, angle_degrees));
			}
			small_font.draw_text(canvas, 370, 100, "Sprite - Angle Pitch");
			
			{
				test_angle_yaw+=5.0f;
				if (test_angle_yaw >= 360.0f)
				{
					test_angle_yaw = 0.0f;
				}

				sprite.set_angle_yaw(Angle(test_angle_yaw, angle_degrees));
				sprite.draw(canvas, 350, 140);
				sprite.set_angle_yaw(Angle(0, angle_degrees));
			}
			small_font.draw_text(canvas, 370, 140, "Sprite - Angle Yaw");
			*/

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
			small_font.draw_text(canvas, 420, 80, "Font - draw_text before scaling Sprite");
			small_font.draw_text(canvas, 420, 100, "Sprite - Set Scale (x), (y)");

				sprite.set_scale(test_scale, 1.0f);
				sprite.draw(canvas, 350, 100);
				sprite.set_scale(1.0f, test_scale);
				sprite.draw(canvas, 390, 100);
				sprite.set_scale(1.0f, 1.0f);
			}

			small_font.draw_text(canvas, 420, 120, "Font - draw_text after scaling Sprite");

			// Flip the display, showing on the screen what we have drawed
			// since last call to flip()
			window.flip(1);

			// This call processes user input and other events
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
void App::on_input_up(const InputEvent &key)
{
	if(key.id == keycode_escape)
	{
		quit = true;
	}
}

// The window was closed
void App::on_window_close()
{
	quit = true;
}


