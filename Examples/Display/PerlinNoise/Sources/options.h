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

#include "../../../ThemeAero/Sources/theme.h"

enum PerlinDimension
{
	perlin_1d,
	perlin_2d,
	perlin_3d,
	perlin_4d
};

class Options : public clan::TextureWindow
{
public:
	Options(clan::Canvas &canvas);
	virtual ~Options();

	clan::TextureFormat sized_format;
	float amplitude;
	int width;
	int height;
	int octaves;
	float start_x;
	float length_x;
	float start_y;
	float length_y;
	float position_z;
	float position_w;
	PerlinDimension dimension;
	bool is_normals_set;

private:
	void on_format_selected(std::shared_ptr<clan::ListBoxView> listbox);
	void on_dimension_selected(std::shared_ptr<clan::ListBoxView> listbox);

	std::shared_ptr<clan::ListBoxView> create_listbox(int xpos, int ypos, const std::string &title);
	std::shared_ptr<clan::CheckBoxView> create_checkbox(int xpos, int ypos, const std::string &name, bool state);
	std::shared_ptr<clan::SliderView> create_slider(int xpos, int ypos);
	std::shared_ptr<clan::LabelView> create_slider_label(int xpos, int ypos);
	void set_value(std::shared_ptr<clan::SliderView> slider, float value, float max_value);
	float get_value(std::shared_ptr<clan::SliderView> slider, float max_value);

	void checkbox_normals_changed();
	void update_all_slider_text();
	void slider_amplitude_changed();
	void slider_width_changed();
	void slider_height_changed();
	void slider_octaves_changed();
	void slider_start_x_changed();
	void slider_length_x_changed();
	void slider_start_y_changed();
	void slider_length_y_changed();
	void slider_position_z_changed();
	void slider_position_w_changed();
	void checkbox_normal_changed();

private:
	std::shared_ptr<clan::SliderView> slider_width;
	std::shared_ptr<clan::LabelView> label_width;
	std::shared_ptr<clan::SliderView> slider_height;
	std::shared_ptr<clan::LabelView> label_height;
	std::shared_ptr<clan::SliderView> slider_start_x;
	std::shared_ptr<clan::LabelView> label_start_x;
	std::shared_ptr<clan::SliderView> slider_start_y;
	std::shared_ptr<clan::LabelView> label_start_y;
	std::shared_ptr<clan::SliderView> slider_length_x;
	std::shared_ptr<clan::LabelView> label_length_x;
	std::shared_ptr<clan::SliderView> slider_length_y;
	std::shared_ptr<clan::LabelView> label_length_y;
	std::shared_ptr<clan::SliderView> slider_position_z;
	std::shared_ptr<clan::LabelView> label_position_z;
	std::shared_ptr<clan::SliderView> slider_position_w;
	std::shared_ptr<clan::LabelView> label_position_w;
	std::shared_ptr<clan::SliderView> slider_octaves;
	std::shared_ptr<clan::LabelView> label_octaves;
	std::shared_ptr<clan::SliderView> slider_amplitude;
	std::shared_ptr<clan::LabelView> label_amplitude;

	std::shared_ptr<clan::LabelView> label_format;
	std::shared_ptr<clan::LabelView> label_dimension;

	std::shared_ptr<clan::CheckBoxView> checkbox_normals;
	std::shared_ptr<clan::ListBoxView> listbox1;
	std::shared_ptr<clan::ListBoxView> listbox2;

};

