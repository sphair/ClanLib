/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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

#pragma once

#include "options.h"

class Options;

class App : public clan::Application
{
public:
	App();
	bool update() override;

private:
	void on_input_up(const clan::InputEvent &key);
	void on_window_close();
	void set_user_projection(clan::Canvas &canvas, clan::Sizef &area_size, Options *options);
	clan::PixelBuffer convert_to_normalmap(clan::PixelBuffer &input);
private:
	clan::DisplayWindow window;
	clan::Canvas canvas;
	clan::PerlinNoise noise;
	clan::Image image_grid;
	clan::Image noise_image;
	clan::UIThread ui_thread;
	std::shared_ptr<Options> options;

	clan::TextureFormat last_sized_format = clan::TextureFormat::rgb8;
	float last_amplitude = 1.0f;
	int last_width = 256;
	int last_height = 256;
	int last_octaves = 1;
	float last_start_x = 0.0f;
	float last_length_x = 32.0f;
	float last_start_y = 0.0f;
	float last_length_y = 32.0f;
	float last_position_z = 0.0f;
	float last_position_w = 0.0f;
	PerlinDimension last_dimension = perlin_2d;
	bool last_is_normals_set = false;
	bool changed_flag = true;

	bool quit = false;
	clan::SlotContainer sc;
};
