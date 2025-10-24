#include <ClanLib/application.h>
#include <ClanLib/core.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>
#include <GL/gl.h>

#define PIXELSIZE 3

/////////////////////////////////////////////////////////////////////////////
// Application class

class Application : public CL_ClanApplication
{
public:
	int main(int argc, char** argv)
	{
		CL_ConsoleWindow debug_console("Debug console");
		debug_console.redirect_stdio();
	
		try
		{
			CL_SetupCore::init();
			CL_SetupDisplay::init();
			CL_SetupGL::init();

			CL_DisplayWindowDescription desc;
			desc.set_title("ClanLib DrawPixels");
			desc.set_size(CL_Size(1024, 768));
			desc.set_bpp(32);
			desc.set_fullscreen(true);
			CL_DisplayWindow window(desc);

			flatten_render_pipeline();

			width = CL_Display::get_width();
			height = CL_Display::get_height();

			image = (unsigned char*)malloc(width * height * PIXELSIZE);

			fill_buffer();

			while (!CL_Keyboard::get_keycode(CL_KEY_ESCAPE))
			{
				update();
				draw();

				window.flip();
				CL_System::keep_alive();
			}

			CL_SetupGL::deinit();
			CL_SetupDisplay::deinit();
			CL_SetupCore::deinit();
		}
		catch (CL_Error error)
		{
			std::cout << "Uncaught error: " << error.message.c_str() << std::endl;
		}

		debug_console.wait_for_key();
		return 0;
	}

	void flatten_render_pipeline()
	{
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_DITHER);
		glDisable(GL_FOG);
		glDisable(GL_LIGHTING);
		glDisable(GL_LOGIC_OP);
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_TEXTURE_1D);
		glDisable(GL_TEXTURE_2D);
		glPixelTransferi(GL_MAP_COLOR, GL_FALSE);
		glPixelTransferi(GL_RED_SCALE, 1);
		glPixelTransferi(GL_RED_BIAS, 0);
		glPixelTransferi(GL_GREEN_SCALE, 1);
		glPixelTransferi(GL_GREEN_BIAS, 0);
		glPixelTransferi(GL_BLUE_SCALE, 1);
		glPixelTransferi(GL_BLUE_BIAS, 0);
		glPixelTransferi(GL_ALPHA_SCALE, 1);
		glPixelTransferi(GL_ALPHA_BIAS, 0);

		#ifdef GL_EXT_convolution
			glDisable(GL_CONVOLUTION_1D_EXT);
			glDisable(GL_CONVOLUTION_2D_EXT);
			glDisable(GL_SEPARABLE_2D_EXT);
		#endif

		#ifdef GL_EXT_histogram
			glDisable(GL_HISTOGRAM_EXT);
			glDisable(GL_MINMAX_EXT);
		#endif

		#ifdef GL_EXT_texture3D
			glDisable(GL_TEXTURE_3D_EXT);
		#endif

		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	}

	void update(void)
	{
		// Calculate time since last update
		static int start_time = CL_System::get_time();
		int cur_time = CL_System::get_time();
		int delta_time = cur_time - start_time;
		start_time = cur_time;
		calc_fps(delta_time);

//		fill_buffer();
	}

	void fill_buffer()
	{
		// Fill screen with some lame pattern
		static char current_colour = 0;
		current_colour++;
		for(int i=0;i<width*height*PIXELSIZE;i++)
			image[i] = current_colour++;
	}

	void calc_fps(int frame_time)
	{
		static int fps_counter = 0;
		static int total_time = 0;

		total_time += frame_time;
		if(total_time >= 1000)// One second has passed
		{
			std::cout << fps_counter << std::endl;
			fps_counter = total_time = 0;
		}
		else
			fps_counter++;// Less than one second, increase fps
	}

	void draw(void)
	{
		glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)image);
	}

	unsigned char* image;
	int width;
	int height;
} app;