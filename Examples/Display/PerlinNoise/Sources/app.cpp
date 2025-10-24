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
#include <cstdlib>

App::App() : quit(false)
{
}

// The start of the Application
int App::start(const std::vector<CL_String> &args)
{
	CL_DisplayWindowDescription win_desc;
	win_desc.set_allow_resize(true);
	win_desc.set_title("Perlin Noise Example");
	win_desc.set_size(CL_Size( 800, 520 ), false);

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

	CL_Image image_grid(gc, "../../Display_Render/Blend/Resources/grid.png");
	image_grid.set_color(CL_Colorf(0.4f, 0.4f, 1.0f, 1.0f));

	CL_PerlinNoise noise;

	CL_Image noise_image;

	CL_TextureFormat last_sized_format = cl_rgb8;
	float last_amplitude = 0.0f;
	int last_width = 0;
	int last_height = 0;
	int last_octaves = 0 ;
	float last_start_x = 0.0f;
	float last_length_x = 0.0f;
	float last_start_y = 0.0f;
	float last_length_y = 0.0f;
	float last_position_z = 0.0f;
	float last_position_w = 0.0f;
	PerlinDimension last_dimension = perlin_2d;

	while (!quit)
	{

		wm.process();
		wm.draw_windows(gc);

		bool changed_flag = false;
		if (last_dimension != options->dimension)
		{
			changed_flag = true;
			last_dimension = options->dimension;
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
			CL_PixelBuffer pbuff;
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
			noise_image = CL_Image(gc, pbuff, pbuff.get_size());

		}

		image_grid.draw(gc, 32, 32);
		noise_image.draw(gc, 33, 33);

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

