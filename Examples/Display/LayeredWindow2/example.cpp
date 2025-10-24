/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Mark Page
*/

#include <ClanLib/core.h>
#include <ClanLib/application.h>
#include <ClanLib/display.h>
#include <ClanLib/gl.h>

#include <cmath>
#include <cstdlib>

#include "framerate_counter.h"

class TuxBall
{
public:
	TuxBall(const CL_Sprite &image, const CL_Rect &boundary);

	void move(std::vector<TuxBall> &tuxballs, float time_diff);
	void draw(CL_GraphicContext &gc);

private:
	float xpos;
	float ypos;
	float xdir;
	float ydir;
	float scale;
	CL_Sprite sprite;
	CL_Rect boundary;
};

// This is the Application class (That is instantiated by the Program Class)
class App
{
public:
	int start(const std::vector<CL_String> &args);

private:
	void on_mouse_down(const CL_InputEvent &key, const CL_InputState &state);
	void on_window_close(CL_DisplayWindow *window);
	void on_lost_focus();
	void on_input_up(const CL_InputEvent &key, const CL_InputState &state);
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
		CL_SetupGL setup_gl;

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

	try
	{
		// (See README.TXT for more documentation)

		// Define the whole size
		CL_Size entire_window_size(650, 650);
		const int window_inner_offset = 128;
		CL_Point window_initial_position(64, 64);

		// Set the window description
		CL_DisplayWindowDescription desc_window;
		desc_window.set_title("Layered Example");
		desc_window.set_allow_resize(false);
		desc_window.set_decorations(false);

		// Open the main window
		desc_window.set_layered(false);
		desc_window.set_position(CL_Rect(window_initial_position.x + window_inner_offset, window_initial_position.y + window_inner_offset, CL_Size(entire_window_size.width - (window_inner_offset*2), entire_window_size.height - (window_inner_offset*2))), false);
		CL_DisplayWindow window_center(desc_window);

		// Create the border layered windows
		desc_window.set_layered(true);
		desc_window.set_title("");
		desc_window.set_owner_window(window_center);
		desc_window.set_position(CL_Rect(window_initial_position.x, window_initial_position.y, CL_Size(entire_window_size.width, window_inner_offset)), false);
		CL_DisplayWindow window_top(desc_window);
		desc_window.set_position(CL_Rect(window_initial_position.x, window_initial_position.y + entire_window_size.height - window_inner_offset, CL_Size(entire_window_size.width, window_inner_offset)), false);
		CL_DisplayWindow window_bottom(desc_window);
		desc_window.set_position(CL_Rect(window_initial_position.x, window_initial_position.y + window_inner_offset , CL_Size(window_inner_offset, entire_window_size.height - (window_inner_offset*2))), false);
		CL_DisplayWindow window_left(desc_window);
		desc_window.set_position(CL_Rect(window_initial_position.x + entire_window_size.width - window_inner_offset, window_initial_position.y + window_inner_offset , CL_Size(window_inner_offset, entire_window_size.height - (window_inner_offset*2))), false);
		CL_DisplayWindow window_right(desc_window);

		// Setup the slots
		CL_Slot slot_quit = window_center.sig_window_close().connect(this, &App::on_window_close, &window_center);
		CL_Slot slot_mouse_down = (window_center.get_ic().get_mouse()).sig_key_down().connect(this, &App::on_mouse_down);
		CL_Slot slot_mouse_dblclk = (window_center.get_ic().get_mouse()).sig_key_dblclk().connect(this, &App::on_mouse_down);
		CL_Slot slot_input_up = (window_center.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

		// Get the graphic context
		CL_GraphicContext gc_center = window_center.get_gc();
		CL_GraphicContext gc_top = window_top.get_gc();
		CL_GraphicContext gc_right = window_right.get_gc();
		CL_GraphicContext gc_bottom = window_bottom.get_gc();
		CL_GraphicContext gc_left = window_left.get_gc();

		// Get the graphics
		CL_FontDescription font_desc;
		font_desc.set_typeface_name("tahoma");
		font_desc.set_height(24);
		CL_Font font(gc_center, font_desc);

		CL_Sprite tux(gc_center, "../LayeredWindow/round_tux.png");
		CL_Image rock(gc_center, "../LayeredWindow/rock.png");

		// Translate the window matrix to position the graphics at the correct position
		gc_center.set_translate(-window_inner_offset, -window_inner_offset);
		gc_top.set_translate(0.0f, 0.0f);
		gc_left.set_translate(0, -window_inner_offset);
		gc_right.set_translate(-entire_window_size.width + window_inner_offset, -window_inner_offset);
		gc_bottom.set_translate(0, -entire_window_size.height + window_inner_offset);

		// Scale the window matrix, so the rock fills it
		CL_Mat4f matrix = CL_Mat4f::scale( (float) entire_window_size.width / rock.get_width(), (float) entire_window_size.height / rock.get_height(), 1.0f);
		gc_top.mult_modelview(matrix);
		gc_right.mult_modelview(matrix);
		gc_bottom.mult_modelview(matrix);
		gc_left.mult_modelview(matrix);
		gc_center.mult_modelview(matrix);

		// Prepare the static image in the layered window
		gc_top.clear(CL_Colorf(0.0f,0.0f,0.0f, 0.0f));
		gc_right.clear(CL_Colorf(0.0f,0.0f,0.0f, 0.0f));
		gc_bottom.clear(CL_Colorf(0.0f,0.0f,0.0f, 0.0f));
		gc_left.clear(CL_Colorf(0.0f,0.0f,0.0f, 0.0f));
		rock.draw(gc_top, 0.0f, 0.0f);
		rock.draw(gc_right, 0.0f, 0.0f);
		rock.draw(gc_bottom, 0.0f, 0.0f);
		rock.draw(gc_left, 0.0f, 0.0f);

		// Draw the layered windows
		window_top.flip();
		window_right.flip();
		window_bottom.flip();
		window_left.flip();

		// Setup the demo
		CL_Rect boundary(120, 120, CL_Size(360, 360));
		const int num_tuxballs = 256;
		std::vector<TuxBall> tuxballs;
		tuxballs.reserve(num_tuxballs);
		for (int cnt=0; cnt<num_tuxballs; cnt++)
		{
			tuxballs.push_back(TuxBall(tux, boundary));
		}

		FramerateCounter framerate_counter;

		unsigned int time_last = CL_System::get_time();

		// Run until someone presses escape
		while (!quit)
		{
			framerate_counter.frame_shown();

			unsigned time_now = CL_System::get_time();
			float time_diff = (float) (time_now - time_last);
			time_last = time_now;

			gc_center.clear(CL_Colorf(0.0f,0.0f,0.0f, 1.0f));
			rock.draw(gc_center, 0.0f, 0.0f);

			// Move tux
			for (int cnt=0; cnt<num_tuxballs; cnt++)
			{
				tuxballs[cnt].move(tuxballs, time_diff);
			}

			// Draw tux
			for (int cnt=0; cnt<num_tuxballs; cnt++)
			{
				tuxballs[cnt].draw(gc_center);
			}

			CL_String fps(cl_format("%1 fps", framerate_counter.get_framerate()));
			font.draw_text(gc_center, 150-2, 150-2, fps, CL_Colorf(0.0f, 0.0f, 0.0f, 1.0f));
			font.draw_text(gc_center, 150, 150, fps, CL_Colorf(1.0f, 1.0f, 1.0f, 1.0f));

			window_center.flip(0);

			// This call processes user input and other events
			CL_KeepAlive::process();
		}
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

void App::on_mouse_down(const CL_InputEvent &key, const CL_InputState &state)
{
	quit = true;
}

void App::on_window_close(CL_DisplayWindow *window)
{
	quit = true;
}

void App::on_input_up(const CL_InputEvent &key, const CL_InputState &state)
{
	if(key.id == CL_KEY_ESCAPE)
	{
		quit = true;
	}
}

TuxBall::TuxBall(const CL_Sprite &image, const CL_Rect &new_boundary) : boundary(new_boundary), sprite(image)
{
	scale =  ((float) ((rand() % 512)) + 256) / 4096.0f;
	float scaled_width = scale * (float) sprite.get_width();
	float scaled_height = scale * (float) sprite.get_height();

	boundary.right -= scaled_width;
	boundary.bottom -= scaled_height;

	xpos = (float) ((rand() % (boundary.get_width())) + boundary.left);
	ypos = (float) ((rand() % (boundary.get_height())) + boundary.top );

	xdir =  ((float) ((rand() % 512)) + 256) / 256.0f;
	ydir =  ((float) ((rand() % 512)) + 256) / 256.0f;


}

void TuxBall::move(std::vector<TuxBall> &tuxballs, float time_diff)
{
	xpos += xdir*(time_diff/50.0f);
	ypos += ydir*(time_diff/50.0f);

	if ((int) xpos < boundary.left)
	{
		xdir = -xdir;
		xpos = (float) boundary.left;
	}

	if ((int) ypos < boundary.top)
	{
		ydir = -ydir;
		ypos = (float) boundary.top;
	}

	if ((int) xpos > boundary.right)
	{
		xdir = -xdir;
		xpos = (float) boundary.right;
	}

	if ((int) ypos > boundary.bottom)
	{
		ydir = -ydir;
		ypos = (float) boundary.bottom;
	}
}

void TuxBall::draw(CL_GraphicContext &gc)
{
	sprite.set_scale(scale, scale);
	sprite.set_alpha(0.5f);
	sprite.draw(gc, xpos, ypos);
}



