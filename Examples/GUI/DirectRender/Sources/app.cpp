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

#include "precomp.h"
#include "app.h"
#include "options.h"
#include "gui_window_manager_direct.h"

#include <cstdlib>

App::App() : quit(false)
{
}

// The start of the Application
int App::start(const std::vector<CL_String> &args)
{
	CL_DisplayWindowDescription win_desc;
	win_desc.set_allow_resize(true);
	win_desc.set_title("Direct Render Example");
	win_desc.set_size(CL_Size( 700, 700 ), false);

	CL_DisplayWindow window(win_desc);
	CL_Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close);
	CL_Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

	CL_String theme;
	if (CL_FileHelp::file_exists("../../../Resources/GUIThemeAero/theme.css"))
		theme = "../../../Resources/GUIThemeAero";
	else if (CL_FileHelp::file_exists("../../../Resources/GUIThemeBasic/theme.css"))
		theme = "../../../Resources/GUIThemeBasic";
	else
		throw CL_Exception("No themes found");

	GUIWindowManagerDirect wm(window);
	CL_GUIManager gui(wm, theme);
	
	CL_GraphicContext gc = window.get_gc();

	// Deleted automatically by the GUI
	Options *options = new Options(gui, CL_Rect(0, 0, gc.get_size()));

	CL_Image image_ball(gc, "../../Display_Render/Blend/Resources/ball.png");
	image_ball.set_alignment(origin_center);
	grid_space = (float) 600.0f;

	setup_balls();

	options->request_repaint();

	CL_Font font(gc, "Tahoma", 20);

	unsigned int time_last = CL_System::get_time();

	while (!quit)
	{
		unsigned int time_now = CL_System::get_time();
		float time_diff = (float) (time_now - time_last);
		time_last = time_now;

		gc.clear(CL_Colorf(0.8f,0.5f,0.5f, 1.0f));

		float grid_xpos = ( ((float) gc.get_width()) - grid_space) / 2.0f;
		float grid_ypos = ( ((float) gc.get_height()) - grid_space) / 2.0f;;

		image_ball.set_color(options->primary_color);

		int num_balls = options->num_balls;
		if (num_balls > max_balls)
			num_balls = max_balls;

		if (options->is_moveballs_set)
			move_balls(time_diff, num_balls);

		for (int cnt=0; cnt<num_balls; cnt++)
		{
			image_ball.draw(gc, grid_xpos + balls[cnt].xpos, grid_ypos + balls[cnt].ypos);
		}

		gui.render_windows();

		window.flip(1);

		CL_KeepAlive::process();
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

void App::setup_balls()
{
	for (int cnt=0; cnt<max_balls; cnt++)
	{
		balls[cnt].xpos = (float) (rand() % (int) grid_space);
		balls[cnt].ypos = (float) (rand() % (int) grid_space);
		balls[cnt].xspeed = (float) ((rand() & 0xFF)+0x7F);
		balls[cnt].yspeed = (float) ((rand() & 0xFF)+0x7F);
		balls[cnt].xdir = ((rand() & 1) == 0);
		balls[cnt].ydir = ((rand() & 1) == 0);
	}
}

void App::move_balls(float time_diff, int num_balls)
{
	for (int cnt=0; cnt<num_balls; cnt++)
	{
		float xdisp = (balls[cnt].xspeed * time_diff) / 5000.0f;
		float ydisp = (balls[cnt].xspeed * time_diff) / 5000.0f;

		if (balls[cnt].xdir)
		{
			balls[cnt].xpos += xdisp;
			if (balls[cnt].xpos >= (float) grid_space)
			{
				balls[cnt].xpos = grid_space;
				balls[cnt].xdir = false;
			}
		}
		else
		{
			balls[cnt].xpos -= xdisp;
			if (balls[cnt].xpos <= 0.0f)
			{
				balls[cnt].xpos = 0.0f;
				balls[cnt].xdir = true;
			}
		}

		if (balls[cnt].ydir)
		{
			balls[cnt].ypos += ydisp;
			if (balls[cnt].ypos >= (float) grid_space)
			{
				balls[cnt].ypos = grid_space;
				balls[cnt].ydir = false;
			}
		}
		else
		{
			balls[cnt].ypos -= ydisp;
			if (balls[cnt].ypos <= 0.0f)
			{
				balls[cnt].ypos = 0.0f;
				balls[cnt].ydir = true;
			}
		}
	}
}
