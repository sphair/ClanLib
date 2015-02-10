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

#pragma once

enum PerlinDimension
{
	perlin_1d,
	perlin_2d,
	perlin_3d,
	perlin_4d
};
/*
class Options : public clan::GUIComponent
{
public:
	Options(clan::GUIManager &gui, clan::Rect gui_position);
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
	void set_value(clan::Slider *slider, float value, float max_value);
	clan::Label *create_slider_label(clan::Slider *slider);
	void on_format_selected(int value, clan::ComboBox *combo);
	void on_dimension_selected(int value, clan::ComboBox *combo);
	void on_render(clan::Canvas &canvas, const clan::Rect &update_rect);
	clan::Slider *create_slider(int xpos, int ypos);
	float get_value(clan::Slider *slider, float max_value);

	clan::CheckBox *create_checkbox(int xpos, int ypos, const char *name, bool state);
	clan::ComboBox *create_dimension_combo_box(int xpos, int ypos, clan::PopupMenu &menu, int selected_item);
	void make_dimension_menu(clan::PopupMenu &menu);
	clan::ComboBox *create_format_combo_box(int xpos, int ypos, clan::PopupMenu &menu, int selected_item);
	void make_format_menu(clan::PopupMenu &menu);
	clan::Label *create_combobox_label(clan::ComboBox *combo, const char *text);
private:
	clan::Slider *slider_width;
	clan::Label *label_width;
	clan::Slider *slider_height;
	clan::Label *label_height;
	clan::Slider *slider_start_x;
	clan::Label *label_start_x;
	clan::Slider *slider_start_y;
	clan::Label *label_start_y;
	clan::Slider *slider_length_x;
	clan::Label *label_length_x;
	clan::Slider *slider_length_y;
	clan::Label *label_length_y;
	clan::Slider *slider_position_z;
	clan::Label *label_position_z;
	clan::Slider *slider_position_w;
	clan::Label *label_position_w;
	clan::Slider *slider_octaves;
	clan::Label *label_octaves;
	clan::Slider *slider_amplitude;
	clan::Label *label_amplitude;

	clan::Label *label_format;
	clan::ComboBox *combo_format;
	clan::PopupMenu combo_format_menu;

	clan::Label *label_dimension;
	clan::ComboBox *combo_dimension;
	clan::PopupMenu combo_dimension_menu;

	clan::CheckBox *checkbox_normals;

};

*/
