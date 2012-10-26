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

#pragma once

class Options : public GUIComponent
{
public:
	Options(GUIManager &gui, Rect gui_position);
	virtual ~Options();

	enum LightType
	{
		light_distant = 0,
		light_spot = 1
	};
	LightType selected_light;

	Colorf distant_specular_color;
	Colorf spot_specular_color;
	Colorf distant_diffuse_color;
	Colorf spot_diffuse_color;

	Angle spot_direction_heading;
	Angle spot_direction_pitch;
	Angle spot_direction_bank;

	Angle distant_direction_heading;
	Angle distant_direction_pitch;
	Angle distant_direction_bank;

	float spot_light_cutoff;
	float spot_light_exponent;
	Vec3f spot_light_position;

	float max_color_value;
	float max_angle_value;
	float max_exponent_value;
	float max_position_value;

private:

	void update_all_slider_text();
	Label *create_slider_label(Slider *slider);
	void on_light_selected(int value, ComboBox *combo);
	CheckBox *create_checkbox(int xpos, int ypos, const char *name, bool state);
	void on_render(GraphicContext &gc, const Rect &update_rect);
	Slider *create_slider(int xpos, int ypos);
	float get_value(Slider *slider, float max_value);
	void set_value(Slider *slider, float value, float max_value);
	ComboBox *create_light_combo_box(int xpos, int ypos, PopupMenu &menu, int selected_item);
	void make_light_menu(PopupMenu &menu);
	Label *create_combobox_label(ComboBox *combo, const char *text);
	Colorf &get_selected_specular();
	Colorf &get_selected_diffuse();
	void slider_specular_red_changed();
	void slider_specular_green_changed();
	void slider_specular_blue_changed();
	void slider_diffuse_red_changed();
	void slider_diffuse_green_changed();
	void slider_diffuse_blue_changed();
	void slider_direction_heading_changed();
	void slider_direction_pitch_changed();
	void slider_direction_bank_changed();
	void slider_light_xpos_changed();
	void slider_light_ypos_changed();
	void slider_light_zpos_changed();
	void slider_cutoff_changed();
	void slider_exponent_changed();
	void set_all_sliders();

private:

	Slider *slider_specular_red;
	Slider *slider_specular_green;
	Slider *slider_specular_blue;
	Label *label_specular_red;
	Label *label_specular_green;
	Label *label_specular_blue;

	Slider *slider_diffuse_red;
	Slider *slider_diffuse_green;
	Slider *slider_diffuse_blue;
	Label *label_diffuse_red;
	Label *label_diffuse_green;
	Label *label_diffuse_blue;

	Slider *slider_direction_heading;
	Slider *slider_direction_pitch;
	Slider *slider_direction_bank;
	Label *label_direction_heading;
	Label *label_direction_pitch;
	Label *label_direction_bank;

	Slider *slider_light_xpos;
	Slider *slider_light_ypos;
	Slider *slider_light_zpos;
	Slider *slider_cutoff;
	Slider *slider_exponent;
	Label *label_light_xpos;
	Label *label_light_ypos;
	Label *label_light_zpos;
	Label *label_cutoff;
	Label *label_exponent;

	Label *label_light;
	ComboBox *combo_light;
	PopupMenu combo_light_menu;

};

