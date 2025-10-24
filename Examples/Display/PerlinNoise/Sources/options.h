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

#pragma once

enum PerlinDimension
{
	perlin_1d,
	perlin_2d,
	perlin_3d,
	perlin_4d
};

class Options : public CL_GUIComponent
{
public:
	Options(CL_GUIManager &gui, CL_Rect gui_position);
	virtual ~Options();

	CL_TextureFormat sized_format;
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

private:
	void checkbox_moveballs_changed();
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

	void set_value(CL_Slider *slider, float value, float max_value);
	CL_Label *create_slider_label(CL_Slider *slider);
	void on_format_selected(int value, CL_ComboBox *combo);
	void on_dimension_selected(int value, CL_ComboBox *combo);
	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	CL_Slider *create_slider(int xpos, int ypos);
	float get_value(CL_Slider *slider, float max_value);

	CL_ComboBox *create_dimension_combo_box(int xpos, int ypos, CL_PopupMenu &menu, int selected_item);
	void make_dimension_menu(CL_PopupMenu &menu);
	CL_ComboBox *create_format_combo_box(int xpos, int ypos, CL_PopupMenu &menu, int selected_item);
	void make_format_menu(CL_PopupMenu &menu);
	CL_Label *create_combobox_label(CL_ComboBox *combo, const char *text);
private:
	CL_Slider *slider_width;
	CL_Label *label_width;
	CL_Slider *slider_height;
	CL_Label *label_height;
	CL_Slider *slider_start_x;
	CL_Label *label_start_x;
	CL_Slider *slider_start_y;
	CL_Label *label_start_y;
	CL_Slider *slider_length_x;
	CL_Label *label_length_x;
	CL_Slider *slider_length_y;
	CL_Label *label_length_y;
	CL_Slider *slider_position_z;
	CL_Label *label_position_z;
	CL_Slider *slider_position_w;
	CL_Label *label_position_w;
	CL_Slider *slider_octaves;
	CL_Label *label_octaves;
	CL_Slider *slider_amplitude;
	CL_Label *label_amplitude;

	CL_Label *label_format;
	CL_ComboBox *combo_format;
	CL_PopupMenu combo_format_menu;

	CL_Label *label_dimension;
	CL_ComboBox *combo_dimension;
	CL_PopupMenu combo_dimension_menu;

};

