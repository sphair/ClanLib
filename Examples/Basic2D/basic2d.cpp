#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#define __Apple__
#ifdef USE_SDL
#include <ClanLib/sdl.h>
#else
#include <ClanLib/gl.h>
#endif

class ExampleBasic2D : public CL_ClanApplication
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
#ifdef USE_SDL
			CL_SetupSDL setup_sdl;
#else
			CL_SetupGL setup_gl;
#endif
			// Set a videomode - 640x480
			CL_DisplayWindow window("ClanLib Basic2D Example", 640, 480);

			// Connect the Window close event
			CL_Slot slot_quit = window.sig_window_close().connect(this, &ExampleBasic2D::on_window_close);

			// Connect a keyboard handler to on_key_up()
			CL_Slot slot_input_up = (window.get_ic()->get_keyboard()).sig_key_up().connect(this, &ExampleBasic2D::on_input_up);

			// Load a surface from a targa-file
			CL_Surface sur_logo("logo.tga");

			double sin_count = 0.0f;
			float ypos = 0.0f;
			float ydir = 3.0f;

			// Run until someone presses escape
			while (!quit)
			{
				// Clear the display in a dark blue nuance
				// The four arguments are red, green, blue and alpha
				// All color nuances in ClanLib are measured in the interval 0->255
				CL_Display::clear(CL_Color(0,0,56));

				ypos += ydir;
				if (ypos+200 >= CL_Display::get_height() || ypos < 0)
				{
					ydir *= -1;
				}
				
				CL_Display::draw_line(0, (int)(ypos-1), CL_Display::get_width(), (int)(ypos-1),CL_Color(127, 0, 0));
				CL_Display::draw_line(0, (int)(ypos+198), CL_Display::get_width(), (int)(ypos+198), CL_Color( 127, 0, 0));

				// Show the logo surface.
				// Use the get_width() and get_height() functions of both
				// CL_Display and CL_Surface, to show the surface in the bottom right corner
				sur_logo.draw(
					CL_Display::get_width()-sur_logo.get_width(),
					CL_Display::get_height()-sur_logo.get_height(),
					window.get_gc());

				CL_Display::push_cliprect(CL_Rect(0, (int)(ypos), CL_Display::get_width(), (int)(ypos+198)));

				// Draw a rectangle in the center of the screen
				// going from (240, 140) -> (440, 340) _not_ including the 
				// pixels in the right-most column and bottom-most row (440, 340)
				CL_Display::fill_rect(CL_Rect(240, 140, 440, 340), CL_Color(255,255,255,255));

				// Frame the rectangle with red lines
				CL_Display::draw_line(240, 140, 440, 140, CL_Color(255, 0, 0));
				CL_Display::draw_line(240, 340, 440, 340, CL_Color(255, 0, 0));
				CL_Display::draw_line(240, 140, 240, 340, CL_Color(255, 0, 0));
				CL_Display::draw_line(440, 140, 440, 340, CL_Color(255, 0, 0));

				// Show a few alpha-blending moving rectangles that moves in circles
				float x = float(cos(sin_count)*120);
				float y = float(sin(sin_count)*120);
				sin_count += 0.05f;
				CL_Display::fill_rect(CL_Rect((int)(320+x-30), (int)(240+y-30), (int)(320+x+30), (int)(240+y+30)), CL_Color(0, 255, 0, 127));
				x = float(cos(sin_count+3.14159)*120);
				y = float(sin(sin_count+3.14159)*120);
				CL_Display::fill_rect(CL_Rect((int)(320+x-30),(int)(240+y-30),(int)(320+x+30),(int)(240+y+30)), CL_Color(255, 255, 0, 127));

				CL_Display::pop_cliprect();

				// Flip the display, showing on the screen what we have drawed
				// since last call to flip_display()
				CL_Display::flip();

				// This call updates input and performs other "housekeeping"
				// call this each frame
				CL_System::keep_alive(10);
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

private:
	bool quit;

	void on_input_up(const CL_InputEvent &key)
	{
		if(key.id == CL_KEY_ESCAPE)
		{
			quit = true;
		}
		else if (key.id == CL_KEY_F12)
		{
                  std::string filename = "screenshot.jpg";
                  std::cout << "Saving screenshot to: " << filename << std::endl;
                  CL_ProviderFactory::save(CL_Display::get_current_window()->get_gc()->get_pixeldata(CL_Rect(100, 200, 500, 300)), 
														 filename);
		}
	}

	void on_window_close()
	{
		quit = true;
	}
} my_app;
