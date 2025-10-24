/*
    alpha test example
*/

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

class SurfaceApp : public CL_ClanApplication
{
private:

	bool quit;

public:
	void on_input_up(const CL_InputEvent &key)
	{
		if(key.id == CL_KEY_ESCAPE)
			quit = true;
	}


	virtual int main(int, char **)
	{
		quit = false;

		// Create a console window for text-output if not available
		CL_ConsoleWindow console("Console");
		console.redirect_stdio();

		try
		{
			CL_SetupCore::init();
			CL_SetupDisplay::init();
			CL_SetupGL::init();

			// Set mode: 640x480 16 bpp
			CL_DisplayWindow window("Alpha Blending Test", 800, 600);

			// Connect a keyboard handler to on_input_up()
			CL_Slot slot_input_up = window.get_ic()->get_keyboard().sig_key_up().connect(
			this, &SurfaceApp::on_input_up);

			// Create a gradient surface
			CL_Canvas *canvas = new CL_Canvas(CL_Size(100, 640));
			CL_PixelBuffer &pb = canvas->get_pixelbuffer();
			pb.lock();
			unsigned int* data = (unsigned int*) pb.get_data();
			for (int y=0; y<480; y++)
			{
				for (int x=0; x<100; x++)
				{
					data[x + 100 * y] = (int)CL_Color(
						255,
						0,
						0,
						y/480);
				}
			}
			pb.unlock();

			// Load surface from jpg file:
			CL_JPEGProvider jpeg("back.jpg");
			CL_Surface surface(&jpeg);

			int alpha = 0;

			// Loop indefinetly
			while (!quit)
			{
				// Draw surface
				surface.draw(0, 0, window.get_gc());

				// Change the alpha
				alpha += 1;
				if (alpha > 255)
					alpha -= 255;

				// Draw some green alpha lines
				for (int y = 10; y < 80; y += 10)
					CL_Display::draw_line(0, y, 640, y, CL_Color(0,255,0, 255 - alpha));

				// Draw a blue alpha box
				CL_Display::fill_rect(CL_Rect(30, 30, 290, 170), CL_Color(0,0,255,alpha));

				// Draw the red gradient
				canvas->draw(0,0);

				// Flip front and backbuffer. This makes the changes visible:
				CL_Display::flip();
			
				// Update keyboard input and handle system events:
				// Exits the loop if ClanLib requests shutdown - for instance if
				// someone closes the window.
				CL_System::keep_alive();
			}

			// De-Initialize the ClanLib components
			CL_SetupDisplay::deinit();
			CL_SetupCore::deinit();
		}
		catch(CL_Error error)
		{
			std::cout << "Exception caught : " << error.message.c_str() << std::endl;			

			// Display console close message and wait for a key
			console.display_close_message();
		}

		return 0;
	}
} app;
