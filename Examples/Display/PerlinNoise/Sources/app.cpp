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
#include <cstdlib>

clan::ApplicationInstance<App> clanapp;

App::App()
{
	// We support all display targets, in order listed here
	clan::OpenGLTarget::enable();

	clan::DisplayWindowDescription win_desc;
	win_desc.set_allow_resize(true);
	win_desc.set_title("Perlin Noise Example");
	win_desc.set_size(clan::Size(900, 520), false);

	window = clan::DisplayWindow(win_desc);
	sc.connect(window.sig_window_close(), clan::bind_member(this, &App::on_window_close));
	sc.connect(window.get_ic().get_keyboard().sig_key_up(), clan::bind_member(this, &App::on_input_up));

	canvas = clan::Canvas(window);
	clan::FileResourceDocument doc(clan::FileSystem("../../ThemeAero"));
	clan::ResourceManager resources = clan::FileResourceManager::create(doc);
	ui_thread = clan::UIThread(resources);

	options = std::make_shared<Options>(canvas);
	options->set_event_window(window);
	options->set_cursor_window(window);

	image_grid = clan::Image(canvas, "../../Display_Render/Blend/Resources/grid.png");
	image_grid.set_color(clan::Colorf(0.4f, 0.4f, 1.0f, 1.0f));
}

bool App::update()
{
	options->set_needs_render();
	options->set_viewport(canvas.get_size());
	options->set_background_color(clan::Colorf(0.6f, 0.6f, 0.2f, 1.0f));
	options->update();

	if (last_dimension != options->dimension)
	{
		changed_flag = true;
		last_dimension = options->dimension;
	}
	if (last_is_normals_set != options->is_normals_set)
	{
		changed_flag = true;
		last_is_normals_set = options->is_normals_set;
	}

	if (last_sized_format != options->sized_format)
	{
		changed_flag = true;
		last_sized_format = options->sized_format;
		noise.set_format(last_sized_format);
	}
	if (last_amplitude != options->amplitude)
	{
		changed_flag = true;
		last_amplitude = options->amplitude;
		noise.set_amplitude(last_amplitude);
	}
	if (last_width != options->width)
	{
		changed_flag = true;
		last_width = options->width;
		noise.set_size(last_width, last_height);
	}
	if (last_height != options->height)
	{
		changed_flag = true;
		last_height = options->height;
		noise.set_size(last_width, last_height);
	}
	if (last_octaves != options->octaves)
	{
		changed_flag = true;
		last_octaves = options->octaves;
		noise.set_octaves(last_octaves);
	}

	if (last_start_x != options->start_x)
	{
		changed_flag = true;
		last_start_x = options->start_x;
	}
	if (last_length_x != options->length_x)
	{
		changed_flag = true;
		last_length_x = options->length_x;
	}
	if (last_start_y != options->start_y)
	{
		changed_flag = true;
		last_start_y = options->start_y;
	}
	if (last_length_y != options->length_y)
	{
		changed_flag = true;
		last_length_y = options->length_y;
	}
	if (last_position_z != options->position_z)
	{
		changed_flag = true;
		last_position_z = options->position_z;
	}
	if (last_position_w != options->position_w)
	{
		changed_flag = true;
		last_position_w = options->position_w;
	}

	if (changed_flag)
	{
		changed_flag = false;
		clan::PixelBuffer pbuff;
		switch (last_dimension)
		{
			case perlin_1d:
				pbuff = noise.create_noise1d(last_start_x, last_start_x + last_length_x);
				break;
			case perlin_2d:
				pbuff = noise.create_noise2d(last_start_x, last_start_x + last_length_x, last_start_y, last_start_y + last_length_y);
				break;
			case perlin_3d:
				pbuff = noise.create_noise3d(last_start_x, last_start_x + last_length_x, last_start_y, last_start_y + last_length_y, last_position_z);
				break;
			case perlin_4d:
			default:
				pbuff = noise.create_noise4d(last_start_x, last_start_x + last_length_x, last_start_y, last_start_y + last_length_y, last_position_z, last_position_w);
				break;
		}

		if (last_is_normals_set)
			pbuff = convert_to_normalmap(pbuff);

		pbuff = pbuff.to_format(clan::tf_rgba8);	// Required for clanD3D
		noise_image = clan::Image(canvas, pbuff, pbuff.get_size());

	}

	image_grid.draw(canvas, 32, 32);
	noise_image.draw(canvas, 33, 33);

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

clan::PixelBuffer App::convert_to_normalmap(clan::PixelBuffer &input)
{
	if (input.get_format() != clan::tf_rgb8)
		return input;
	int width = input.get_width();
	int height = input.get_height();

	clan::PixelBuffer output(width, height, clan::tf_rgb8);

	int in_pitch = input.get_pitch();
	int out_pitch = output.get_pitch();
	int in_bpp = input.get_bytes_per_pixel();
	int out_bpp = output.get_bytes_per_pixel();

	int pixel_tl;
	int pixel_t;
	int pixel_tr;
	int pixel_r;
	int pixel_br;
	int pixel_b;
	int pixel_bl;
	int pixel_l;

	int xpos_left;
	int xpos_center;
	int xpos_right;
	int ypos_top;
	int ypos_center;
	int ypos_bottom;

	for (int ypos = 0; ypos < height; ypos++)
	{
		unsigned char *in_ptr = (unsigned char *) input.get_data();
		unsigned char *out_ptr = (unsigned char *) output.get_data();


		for (int xpos = 0; xpos < width; xpos++)
		{
			// Wrap the coordinates
			if (xpos != 0)
			{
				xpos_left = xpos - 1;
			}else
			{
				xpos_left = width - 1;
			}

			if (ypos != 0)
			{
				ypos_top = ypos - 1;
			}else
			{
				ypos_top = height - 1;
			}

			if (xpos != (width-1))
			{
				xpos_right = xpos + 1;
			}else
			{
				xpos_right = 0;
			}

			if (ypos != (height-1))
			{
				ypos_bottom = ypos + 1;
			}else
			{
				ypos_bottom = 0;
			}
			xpos_center = xpos;
			ypos_center = ypos;

			// Adjust the positions 
			xpos_left *= in_bpp;
			xpos_center *= in_bpp;
			xpos_right *= in_bpp;

			ypos_top *= in_pitch;
			ypos_center *= in_pitch;
			ypos_bottom *= in_pitch;

			// Get the pixels
			pixel_tl = in_ptr[ xpos_left + ypos_top ];
			pixel_l = in_ptr[ xpos_left + ypos_center ];
			pixel_bl = in_ptr[ xpos_left + ypos_bottom ];
			pixel_b = in_ptr[ xpos_center + ypos_bottom ];
			pixel_br = in_ptr[ xpos_right + ypos_bottom ];
			pixel_r = in_ptr[ xpos_right + ypos_center ];
			pixel_tr = in_ptr[ xpos_right + ypos_top ];
			pixel_t = in_ptr[ xpos_center + ypos_top ];
		
			// Compute dx using Sobel:
			//           -1 0 1 
			//           -2 0 2
			//           -1 0 1
			int dX = pixel_tr + 2 * pixel_r + pixel_br - pixel_tl - 2 * pixel_l - pixel_bl;

			// Compute dy using Sobel:
			//           -1 -2 -1 
			//            0  0  0
			//            1  2  1
			int dY = pixel_bl + 2 * pixel_b + pixel_br - pixel_tl - 2 * pixel_t - pixel_tr;
	
			const float strength = 1.0f * 256.0f;
			clan::Vec3f vector(dX, dY, strength);
			vector.normalize();

			// Stored as BGR
			out_ptr[(xpos * out_bpp + ypos * out_pitch) + 2] = (int) (vector.x * 127.0f + 127.0f);
			out_ptr[(xpos * out_bpp + ypos * out_pitch) + 1] = (int) (vector.y * 127.0f + 127.0f);
			out_ptr[(xpos * out_bpp + ypos * out_pitch) + 0] = (int) (vector.z * 127.0f + 127.0f);


		}
	}

	return output;

}
