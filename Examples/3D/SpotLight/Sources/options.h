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

#pragma once

class Options : public CL_GUIComponent
{
public:
	Options(CL_GUIManager &gui, CL_Rect gui_position);
	virtual ~Options();

	enum LightType
	{
		light_distant = 0,
		light_spot = 1
	};
	LightType selected_light;

	CL_Colorf distant_specular_color;
	CL_Colorf spot_specular_color;
	CL_Colorf distant_diffuse_color;
	CL_Colorf spot_diffuse_color;

	CL_Angle spot_direction_heading;
	CL_Angle spot_direction_pitch;
	CL_Angle spot_direction_bank;

	CL_Angle distant_direction_heading;
	CL_Angle distant_direction_pitch;
	CL_Angle distant_direction_bank;

	float spot_light_cutoff;
	float spot_light_exponent;
	CL_Vec3f spot_light_position;

	float max_color_value;
	float max_angle_value;
	float max_exponent_value;
	float max_position_value;

private:

	void update_all_slider_text();
	CL_Label *create_slider_label(CL_Slider *slider);
	void on_light_selected(int value, CL_ComboBox *combo);
	CL_CheckBox *create_checkbox(int xpos, int ypos, const char *name, bool state);
	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	CL_Slider *create_slider(int xpos, int ypos);
	float get_value(CL_Slider *slider, float max_value);
	void set_value(CL_Slider *slider, float value, float max_value);
	CL_ComboBox *create_light_combo_box(int xpos, int ypos, CL_PopupMenu &menu, int selected_item);
	void make_light_menu(CL_PopupMenu &menu);
	CL_Label *create_combobox_label(CL_ComboBox *combo, const char *text);
	CL_Colorf &get_selected_specular();
	CL_Colorf &get_selected_diffuse();
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

	CL_Slider *slider_specular_red;
	CL_Slider *slider_specular_green;
	CL_Slider *slider_specular_blue;
	CL_Label *label_specular_red;
	CL_Label *label_specular_green;
	CL_Label *label_specular_blue;

	CL_Slider *slider_diffuse_red;
	CL_Slider *slider_diffuse_green;
	CL_Slider *slider_diffuse_blue;
	CL_Label *label_diffuse_red;
	CL_Label *label_diffuse_green;
	CL_Label *label_diffuse_blue;

	CL_Slider *slider_direction_heading;
	CL_Slider *slider_direction_pitch;
	CL_Slider *slider_direction_bank;
	CL_Label *label_direction_heading;
	CL_Label *label_direction_pitch;
	CL_Label *label_direction_bank;

	CL_Slider *slider_light_xpos;
	CL_Slider *slider_light_ypos;
	CL_Slider *slider_light_zpos;
	CL_Slider *slider_cutoff;
	CL_Slider *slider_exponent;
	CL_Label *label_light_xpos;
	CL_Label *label_light_ypos;
	CL_Label *label_light_zpos;
	CL_Label *label_cutoff;
	CL_Label *label_exponent;

	CL_Label *label_light;
	CL_ComboBox *combo_light;
	CL_PopupMenu combo_light_menu;

};

