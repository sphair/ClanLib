/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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

clan::ApplicationInstance<App> clanapp;

App::App()
{
	clan::OpenGLTarget::set_current();

	clan::DisplayWindowDescription win_desc;
	win_desc.set_allow_resize(true);
	win_desc.set_stencil_size(8);
	// For simplicity this example does not use the depth components
	//win_desc.set_depth_size(16);
	win_desc.set_title("Stencil Example");
	win_desc.set_size(clan::Size( 1200, 570 ), false);

	window = clan::DisplayWindow(win_desc);
	sc.connect(window.sig_window_close(), clan::bind_member(this, &App::on_window_close));
	sc.connect(window.get_keyboard().sig_key_up(), clan::bind_member(this, &App::on_input_up));
	canvas = clan::Canvas(window);

	clan::FileResourceDocument doc(clan::FileSystem("../../ThemeAero"));
	clan::ResourceManager resources = clan::FileResourceManager::create(doc);
	ui_thread = clan::UIThread(resources);

	options = std::make_shared<Options>(canvas);
	options->set_always_render();
	options->set_window(window);

	image_grid = clan::Image(canvas, "../Blend/Resources/grid.png");
	image_ball = clan::Image(canvas, "../Blend/Resources/ball.png");
	grid_space = (float) (image_grid.get_width() - image_ball.get_width());

	setup_balls();

	font = clan::Font("Tahoma", 20);

	clan::BlendStateDescription blend_desc;
	blend_desc.enable_color_write(false, false, false, false);
	blend_state_no_color_write = clan::BlendState(canvas, blend_desc);

	game_time.reset();
}

bool App::update()
{
	game_time.update();
	
	options->set_viewport(canvas.get_size());
	options->set_background_color(clan::Colorf(0.6f, 0.6f, 0.2f, 1.0f));
	options->update();

	int num_balls = options->num_balls;
	if (num_balls > max_balls)
		num_balls = max_balls;

	if (options->is_moveballs_set)
		move_balls(game_time.get_time_elapsed(), num_balls);

	canvas.clear_stencil(0);

	// Draw the grid
	const float grid_xpos = 10.0f;
	const float grid_ypos = 10.0f;
	image_grid.draw(canvas, grid_xpos, grid_ypos);

	clan::DepthStencilStateDescription stencil_desc;

	// Draw the circle onto the stencil
	if (options->is_circle_set)
	{
		stencil_desc.enable_stencil_test(true);

		stencil_desc.set_stencil_compare_front(clan::CompareFunction::always, 255, 255);
		stencil_desc.set_stencil_compare_back(clan::CompareFunction::always, 255, 255);
		stencil_desc.set_stencil_op_front(clan::StencilOp::incr_wrap, clan::StencilOp::incr_wrap, clan::StencilOp::incr_wrap);
		stencil_desc.set_stencil_op_back(clan::StencilOp::incr_wrap, clan::StencilOp::incr_wrap, clan::StencilOp::incr_wrap);
		stencil_desc.enable_depth_write(false);
		stencil_desc.enable_depth_test(false);

		clan::DepthStencilState stencil_state(canvas, stencil_desc);
		canvas.set_depth_stencil_state(stencil_state);
		canvas.set_blend_state(blend_state_no_color_write);

		canvas.fill_circle(grid_xpos + image_grid.get_width()/2, grid_ypos + image_grid.get_height()/2, 100, clan::Colorf::white);
		canvas.reset_blend_state();

	}

	stencil_desc.enable_stencil_test(true);
	stencil_desc.set_stencil_compare_front(options->compare_function, options->compare_reference, 255);
	stencil_desc.set_stencil_compare_back(options->compare_function, options->compare_reference, 255);
	stencil_desc.set_stencil_op_front(options->stencil_fail, options->stencil_pass, options->stencil_pass);
	stencil_desc.set_stencil_op_back(options->stencil_fail, options->stencil_pass, options->stencil_pass);

	// Note, depth testing disabled for this example
	stencil_desc.enable_depth_write(false);
	stencil_desc.enable_depth_test(false);
		
	//clan::BlendState blend_state(canvas, blend_desc);
	clan::DepthStencilState stencil_state(canvas, stencil_desc);
	canvas.set_depth_stencil_state(stencil_state);

	for (int cnt=0; cnt<num_balls; cnt++)
	{
		image_ball.draw(canvas, grid_xpos + balls[cnt].xpos, grid_ypos + balls[cnt].ypos);
	}

	canvas.reset_depth_stencil_state();

	clan::Image stencil_image = get_stencil(canvas, 
		clan::Rect(grid_xpos, grid_ypos, image_grid.get_width(), image_grid.get_height()));

	const float stencil_image_xpos = 400.0f;
	const float stencil_image_ypos = 30.0f;
	const float stencil_image_scale = 0.5f;
	stencil_image.set_scale(stencil_image_scale, stencil_image_scale);
	stencil_image.draw(canvas, stencil_image_xpos, stencil_image_ypos);
	canvas.draw_box(clan::Rectf(stencil_image_xpos, stencil_image_ypos, clan::Sizef(stencil_image.get_width() * stencil_image_scale, stencil_image.get_height() * stencil_image_scale)), clan::Colorf::white);
	font.draw_text(canvas, stencil_image_xpos, stencil_image_ypos - 4.0f, "Stencil", clan::Colorf::black);

	// Add a note to avoid confusion
	font.draw_text(canvas, 10.0f, 500.0, "(This example does not use the stencil depth buffer comparison or the stencil bitmask)", clan::Colorf::black);

	window.flip(1);

	return !quit;
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
		float xdisp = (balls[cnt].xspeed * time_diff) * 0.5f;
		float ydisp = (balls[cnt].xspeed * time_diff) * 0.5f;

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
	canvas.flush();

	// For an unknown reason, stencil reads should be a multiple of 32
	rect.left =  32 * ((rect.left + 31) / 32);
	rect.top =  32 * ((rect.top + 31) / 32);
	rect.right =  32 * ((rect.right + 31) / 32);
	rect.bottom =  32 * ((rect.bottom + 31) / 32);

	int rect_width = rect.get_width();
	int rect_height  = rect.get_height();

	std::vector<unsigned char> buffer;
	buffer.resize(rect_width * rect_height);


	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ROW_LENGTH, rect_width);
	glPixelStorei(GL_PACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_PACK_SKIP_ROWS, 0);
	glReadBuffer(GL_BACK);
	if (glClampColor)
	{
#ifdef GL_CLAMP_READ_COLOR
		glClampColor(GL_CLAMP_READ_COLOR, GL_FALSE);
#else
		glClampColor(clan::GL_CLAMP_READ_COLOR, GL_FALSE);
#endif
	}

	glReadPixels(rect.left, canvas.get_height()- rect.bottom, rect_width, rect_height, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, &buffer[0]);
	clan::PixelBuffer pbuf(rect_width, rect_height, clan::TextureFormat::rgba8);
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
