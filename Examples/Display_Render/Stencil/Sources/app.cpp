/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
#include <cstdlib>

App::App() : quit(false)
{
}

// The start of the Application
int App::start(const std::vector<std::string> &args)
{
	clan::DisplayWindowDescription win_desc;
	win_desc.set_allow_resize(true);
	win_desc.set_stencil_size(8);
	// For simplicity this example does not use the depth components
	//win_desc.set_depth_size(16);
	win_desc.set_title("Stencil Example");
	win_desc.set_size(Size( 900, 570 ), false);

	clan::DisplayWindow window(win_desc);
	clan::Slot slot_quit = window.sig_window_close().connect(this, &App::on_window_close);
	clan::Slot slot_input_up = (window.get_ic().get_keyboard()).sig_key_up().connect(this, &App::on_input_up);

	std::string theme;
	if (clan::FileHelp::file_exists("../../../Resources/GUIThemeAero/theme.css"))
		theme = "../../../Resources/GUIThemeAero";
	else if (clan::FileHelp::file_exists("../../../Resources/GUIThemeBasic/theme.css"))
		theme = "../../../Resources/GUIThemeBasic";
	else
		throw Exception("No themes found");

	clan::GUIWindowManagerTexture wm(window);
	clan::GUIManager gui(wm, theme);
	
	clan::Canvas canvas = window.get_gc();

	// Deleted automatically by the GUI
	Options *options = new Options(gui, clan::Rect(0, 0, canvas.get_size()));

	clan::Image image_grid(canvas, "../Blend/Resources/grid.png");
	clan::Image image_ball(canvas, "../Blend/Resources/ball.png");
	grid_space = (float) (image_grid.get_width() - image_ball.get_width());

	setup_balls();

	options->request_repaint();

	clan::Font font(canvas, "Tahoma", 24);

	unsigned int time_last = clan::System::get_time();

	while (!quit)
	{
		unsigned int time_now = clan::System::get_time();
		float time_diff = (float) (time_now - time_last);
		time_last = time_now;

		wm.process();
		wm.draw_windows(canvas);

		int num_balls = options->num_balls;
		if (num_balls > max_balls)
			num_balls = max_balls;

		if (options->is_moveballs_set)
			move_balls(time_diff, num_balls);

		canvas.clear_stencil(0);

		// Draw the grid
		const float grid_xpos = 10.0f;
		const float grid_ypos = 10.0f;
		image_grid.draw(canvas, grid_xpos, grid_ypos);

		// Draw the circle onto the stencil
		if (options->is_circle_set)
		{
			canvas.enable_logic_op(false);
			canvas.enable_stencil_test(true);

			canvas.set_stencil_compare_front(cl_comparefunc_always);
			canvas.set_stencil_compare_back(cl_comparefunc_always);
			canvas.set_stencil_op_front(cl_stencil_incr_wrap, cl_stencil_incr_wrap, cl_stencil_incr_wrap);
			canvas.set_stencil_op_back(cl_stencil_incr_wrap, cl_stencil_incr_wrap, cl_stencil_incr_wrap);
			canvas.enable_color_write(false);
			canvas.enable_depth_write(false);
			canvas.enable_depth_test(false);
			Draw::circle(canvas, grid_xpos + image_grid.get_width()/2, grid_ypos + image_grid.get_height()/2, 100, clan::Colorf::white);
		}

		canvas.enable_color_write(true);
		canvas.enable_logic_op(false);

		canvas.enable_stencil_test(true);
		canvas.set_stencil_compare_front(options->compare_function, options->compare_reference);
		canvas.set_stencil_compare_back(options->compare_function, options->compare_reference);
		canvas.set_stencil_op_front(options->stencil_fail, options->stencil_pass, options->stencil_pass);
		canvas.set_stencil_op_back(options->stencil_fail, options->stencil_pass, options->stencil_pass);

		// Note, depth testing disabled for this example
		canvas.enable_depth_write(false);
		canvas.enable_depth_test(false);
		
		for (int cnt=0; cnt<num_balls; cnt++)
		{
			image_ball.draw(canvas, grid_xpos + balls[cnt].xpos, grid_ypos + balls[cnt].ypos);
		}

		canvas.reset_buffer_control();

		clan::Image stencil_image = get_stencil(canvas, 
			clan::Rect(grid_xpos, grid_ypos, image_grid.get_width(), image_grid.get_height()));

		const float stencil_image_xpos = 400.0f;
		const float stencil_image_ypos = 30.0f;
		const float stencil_image_scale = 0.5f;
		stencil_image.set_scale(stencil_image_scale, stencil_image_scale);
		stencil_image.draw(canvas, stencil_image_xpos, stencil_image_ypos);
		canvas.draw_box(clan::Rectf(stencil_image_xpos, stencil_image_ypos, clan::Sizef(stencil_image.get_width() * stencil_image_scale, stencil_image.get_height() * stencil_image_scale)), Colorf::white);
		font.draw_text(canvas, stencil_image_xpos, stencil_image_ypos - 4.0f, "Stencil", clan::Colorf::black);

		// Add a note to avoid confusion
		font.draw_text(canvas, 10.0f, 500.0, "(This example does not use the stencil depth buffer comparison or the stencil bitmask)", clan::Colorf::black);

		canvas.flip(1);

		clan::KeepAlive::process();
	}

	return 0;
}

// A key was pressed
void App::on_input_up(const clan::InputEvent &key)
{
	if(key.id == clan::keycode_escape)
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

clan::Image App::get_stencil(clan::Canvas &canvas, clan::Rect rect)
{

	// For an unknown reason, stencil reads should be a multiple of 32
	rect.left =  32 * ((rect.left + 31) / 32);
	rect.top =  32 * ((rect.top + 31) / 32);
	rect.right =  32 * ((rect.right + 31) / 32);
	rect.bottom =  32 * ((rect.bottom + 31) / 32);

	int rect_width = rect.get_width();
	int rect_height  = rect.get_height();

	std::vector<unsigned char> buffer;
	buffer.resize(rect_width * rect_height);

	clan::glReadPixels(rect.left, canvas.get_height()- rect.bottom, rect_width, rect_height, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &buffer[0]);
	clan::PixelBuffer pbuf(rect_width, rect_height, clan::tf_abgr8);
	unsigned int *pdata = (unsigned int *) pbuf.get_data();
	unsigned char *rdata = &buffer[0];
	for (int ycnt=0; ycnt < rect_height; ycnt++)
	{
		for (int xcnt=0; xcnt < rect_width; xcnt++)
		{
			int value = *(rdata++);
			if (value == 0)
			{
				*(pdata++) = 0xFF005500;
			}
			else
			{
				value = value * 16;
				value = 0xFF000000 | value | (value << 8) | (value << 16);
				*(pdata++) = value;
			}
		}
	}
	pbuf.flip_vertical();
	return clan::Image(canvas, pbuf, pbuf.get_size());
}
