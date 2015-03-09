/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
	// We support all display targets, in order listed here
#ifdef WIN32
	clan::D3DTarget::enable();
#endif
	clan::OpenGLTarget::enable();
	App::App() : quit(false)
{
}

// The start of the Application
int App::start(const std::vector<std::string> &args)
{
	clan::DisplayWindowDescription win_desc;
	win_desc.set_allow_resize(true);
	win_desc.set_title("MapMode Example");
	win_desc.set_size(clan::Size( 800, 480 ), false);

	clan::DisplayWindow window(win_desc);
    clan::SlotContainer cc;
	cc.connect(window.sig_window_close(), clan::bind_member(this, &App::on_window_close));
	cc.connect(window.get_ic().get_keyboard().sig_key_up(), clan::bind_member(this, &App::on_input_up));

	clan::Canvas canvas(window);

	// Deleted automatically by the GUI
	//Options *options = new Options(gui, clan::Rect(0, 0, canvas.get_size()));

	clan::Image image_grid(canvas, "../Blend/Resources/grid.png");
	clan::Image image_ball(canvas, "../Blend/Resources/ball.png");
	float grid_width = (float) image_grid.get_width();
	float grid_height = (float) image_grid.get_height();

	grid_space = (float) (image_grid.get_width() - image_ball.get_width());

	setup_balls();

	game_time.reset();
}

bool App::update()
{
	game_time.update();

	
	int num_balls = 3;	// options->num_balls;
	if (num_balls > max_balls)
		num_balls = max_balls;

//	if (options->is_moveballs_set)
		move_balls(game_time.get_time_elapsed(), num_balls);

//	canvas.set_map_mode(options->current_mapmode);

	const float grid_xpos = 10.0f;
	const float grid_ypos = 10.0f;

//	if (options->current_mapmode == clan::map_user_projection)
//	{
//		clan::Sizef area_size(grid_width + (grid_xpos * 2.0f), grid_height + (grid_ypos * 2.0f));
//		set_user_projection(canvas, area_size, options);
//	}

	// Draw the grid
	image_grid.draw(canvas, grid_xpos, grid_ypos);

	for (int cnt=0; cnt<num_balls; cnt++)
	{
		image_ball.draw(canvas, grid_xpos + balls[cnt].xpos, grid_ypos + balls[cnt].ypos);
	}

	canvas.set_transform(clan::Mat4f::identity());
	canvas.set_projection(clan::Mat4f::identity());
	canvas.set_map_mode(clan::map_2d_upper_left);
	canvas.get_gc().set_viewport(canvas.get_size());

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
		float ydisp = (balls[cnt].yspeed * time_diff) * 0.5f;

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

void App::set_user_projection(clan::Canvas &canvas, clan::Sizef &area_size, Options *options)
{
	canvas.get_gc().set_viewport(clan::Rectf(0, 0, area_size));

	float lens_zoom = 3.2f;
	float lens_near = 0.1f;
	float lens_far = 10000.0f;
	float lens_aspect = 1.0f;

	float fov = 2.0f * atan2(1.0f, lens_zoom);
	float aspect = 1.0f;

	aspect = ( area_size.width * lens_aspect) / area_size.height;

	fov = (fov * 180.0f) / clan::PI;
	clan::Mat4f projection_matrix = clan::Mat4f::perspective( fov, aspect, lens_near, lens_far, clan::handed_left, clan::clip_negative_positive_w);
	canvas.set_projection(projection_matrix);

	clan::Mat4f modelview_matrix = clan::Mat4f::identity();

	modelview_matrix.translate_self(-1.0f, 1.0, lens_zoom);
	//modelview_matrix = modelview_matrix * clan::Mat4f::rotate(clan::Angle((float) -options->grid_angle, clan::angle_degrees), 1.0f, 0.0f, 0.0f, false);
	modelview_matrix.scale_self(2.0f / area_size.width, -2.0f / area_size.height, 1.0f);

	canvas.set_transform(modelview_matrix);
}
