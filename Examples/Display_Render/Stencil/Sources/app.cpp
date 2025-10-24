/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
int App::start(const std::vector<CL_String> &args)
{
	CL_DisplayWindowDescription win_desc;
	win_desc.set_allow_resize(true);
	win_desc.set_stencil_size(8);
	// For simplicity this example does not use the depth components
	//win_desc.set_depth_size(16);
	win_desc.set_title("Stencil Example");
	win_desc.set_size(CL_Size( 900, 570 ), false);

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

	CL_GUIWindowManagerTexture wm(window);
	CL_GUIManager gui(wm, theme);
	
	CL_GraphicContext gc = window.get_gc();

	// Deleted automatically by the GUI
	Options *options = new Options(gui, CL_Rect(0, 0, gc.get_size()));

	CL_Image image_grid(gc, "../Blend/Resources/grid.png");
	CL_Image image_ball(gc, "../Blend/Resources/ball.png");
	grid_space = (float) (image_grid.get_width() - image_ball.get_width());

	setup_balls();

	CL_BufferControl buffer_control;
	CL_BufferControl default_buffer_control;

	options->request_repaint();

	CL_Font font(gc, "Tahoma", 24);

	unsigned int time_last = CL_System::get_time();

	while (!quit)
	{
		unsigned int time_now = CL_System::get_time();
		float time_diff = (float) (time_now - time_last);
		time_last = time_now;

		wm.process();
		wm.draw_windows(gc);

		int num_balls = options->num_balls;
		if (num_balls > max_balls)
			num_balls = max_balls;

		if (options->is_moveballs_set)
			move_balls(time_diff, num_balls);

		gc.clear_stencil(0);

		// Draw the grid
		const float grid_xpos = 10.0f;
		const float grid_ypos = 10.0f;
		image_grid.draw(gc, grid_xpos, grid_ypos);

		// Draw the circle onto the stencil
		if (options->is_circle_set)
		{
			buffer_control.enable_logic_op(false);
			buffer_control.enable_stencil_test(true);
			buffer_control.set_stencil_compare_func(cl_comparefunc_always, cl_comparefunc_always);
			buffer_control.set_stencil_fail(cl_stencil_incr_wrap, cl_stencil_incr_wrap);
			buffer_control.set_stencil_pass_depth_fail(cl_stencil_incr_wrap, cl_stencil_incr_wrap);
			buffer_control.set_stencil_pass_depth_pass(cl_stencil_incr_wrap, cl_stencil_incr_wrap);
			buffer_control.enable_color_write(false);
			buffer_control.enable_depth_write(false);
			buffer_control.enable_depth_test(false);
			gc.set_buffer_control(buffer_control);
			CL_Draw::circle(gc, grid_xpos + image_grid.get_width()/2, grid_ypos + image_grid.get_height()/2, 100, CL_Colorf::white);
		}

		buffer_control.enable_color_write(true);
		buffer_control.enable_logic_op(false);

		buffer_control.enable_stencil_test(true);
		buffer_control.set_stencil_compare_func(options->compare_function, options->compare_function);
		buffer_control.set_stencil_compare_reference(options->compare_reference, options->compare_reference);
		buffer_control.set_stencil_write_mask(255, 255);
		buffer_control.set_stencil_compare_mask(255, 255);

		buffer_control.set_stencil_fail(options->stencil_fail, options->stencil_fail);;

		// Note, depth testing disabled for this example
		buffer_control.set_stencil_pass_depth_fail(options->stencil_pass, options->stencil_pass);
		buffer_control.set_stencil_pass_depth_pass(options->stencil_pass, options->stencil_pass);

		buffer_control.enable_depth_write(false);
		buffer_control.enable_depth_test(false);

		gc.set_buffer_control(buffer_control);

		for (int cnt=0; cnt<num_balls; cnt++)
		{
			image_ball.draw(gc, grid_xpos + balls[cnt].xpos, grid_ypos + balls[cnt].ypos);
		}

		gc.set_buffer_control(default_buffer_control);

		CL_Image stencil_image = get_stencil(gc, 
			CL_Rect(grid_xpos, grid_ypos, image_grid.get_width(), image_grid.get_height()));

		const float stencil_image_xpos = 400.0f;
		const float stencil_image_ypos = 30.0f;
		const float stencil_image_scale = 0.5f;
		stencil_image.set_scale(stencil_image_scale, stencil_image_scale);
		stencil_image.draw(gc, stencil_image_xpos, stencil_image_ypos);
		CL_Draw::box(gc, CL_Rectf(stencil_image_xpos, stencil_image_ypos, CL_Sizef(stencil_image.get_width() * stencil_image_scale, stencil_image.get_height() * stencil_image_scale)), CL_Colorf::white);
		font.draw_text(gc, stencil_image_xpos, stencil_image_ypos - 4.0f, "Stencil", CL_Colorf::black);

		// Add a note to avoid confusion
		font.draw_text(gc, 10.0f, 500.0, "(This example does not use the stencil depth buffer comparison or the stencil bitmask)", CL_Colorf::black);

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

CL_Image App::get_stencil(CL_GraphicContext &gc, CL_Rect rect)
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

	clReadPixels(rect.left, gc.get_height()- rect.bottom, rect_width, rect_height, CL_STENCIL_INDEX, CL_UNSIGNED_BYTE, &buffer[0]);
	CL_PixelBuffer pbuf(rect_width, rect_height, cl_abgr8);
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
	return CL_Image(gc, pbuf, pbuf.get_size());
}
