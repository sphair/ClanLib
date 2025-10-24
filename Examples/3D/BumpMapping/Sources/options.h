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

class Options : public CL_GUIComponent
{
public:
	Options(CL_GUIManager &gui, CL_Rect gui_position);
	virtual ~Options();

	CL_Colorf light_ambient_color;
	CL_Colorf light_specular_color;
	CL_Colorf light_diffuse_color;

	CL_Colorf material_emission_color;
	CL_Colorf material_ambient_color;
	CL_Colorf material_specular_color;
	float material_shininess;

	CL_Angle light_direction_heading;
	CL_Angle light_direction_pitch;
	CL_Angle light_direction_bank;

	float max_color_value;
	float max_angle_value;
	float max_shininess_value;

private:

	void update_all_slider_text();
	CL_Label *create_slider_label(CL_Slider *slider);
	CL_CheckBox *create_checkbox(int xpos, int ypos, const char *name, bool state);
	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	CL_Slider *create_slider(int xpos, int ypos);
	float get_value(CL_Slider *slider, float max_value);
	void set_value(CL_Slider *slider, float value, float max_value);
	CL_Label *create_combobox_label(CL_ComboBox *combo, const char *text);
	void slider_light_specular_red_changed();
	void slider_light_specular_green_changed();
	void slider_light_specular_blue_changed();
	void slider_light_diffuse_red_changed();
	void slider_light_diffuse_green_changed();
	void slider_light_diffuse_blue_changed();
	void slider_light_direction_heading_changed();
	void slider_light_direction_pitch_changed();
	void slider_light_ambient_red_changed();
	void slider_light_ambient_green_changed();
	void slider_light_ambient_blue_changed();

	void slider_material_emission_red_changed();
	void slider_material_emission_green_changed();
	void slider_material_emission_blue_changed();
	void slider_material_ambient_red_changed();
	void slider_material_ambient_green_changed();
	void slider_material_ambient_blue_changed();
	void slider_material_specular_red_changed();
	void slider_material_specular_green_changed();
	void slider_material_specular_blue_changed();
	void slider_material_shininess_changed();

	void set_all_sliders();

private:

	CL_Slider *slider_light_specular_red;
	CL_Slider *slider_light_specular_green;
	CL_Slider *slider_light_specular_blue;
	CL_Label *label_light_specular_red;
	CL_Label *label_light_specular_green;
	CL_Label *label_light_specular_blue;

	CL_Slider *slider_light_diffuse_red;
	CL_Slider *slider_light_diffuse_green;
	CL_Slider *slider_light_diffuse_blue;
	CL_Label *label_light_diffuse_red;
	CL_Label *label_light_diffuse_green;
	CL_Label *label_light_diffuse_blue;

	CL_Slider *slider_light_ambient_red;
	CL_Slider *slider_light_ambient_green;
	CL_Slider *slider_light_ambient_blue;
	CL_Label *label_light_ambient_red;
	CL_Label *label_light_ambient_green;
	CL_Label *label_light_ambient_blue;

	CL_Slider *slider_light_direction_heading;
	CL_Slider *slider_light_direction_pitch;
	CL_Label *label_light_direction_heading;
	CL_Label *label_light_direction_pitch;

	CL_Slider *slider_material_emission_red;
	CL_Slider *slider_material_emission_green;
	CL_Slider *slider_material_emission_blue;
	CL_Label *label_material_emission_red;
	CL_Label *label_material_emission_green;
	CL_Label *label_material_emission_blue;

	CL_Slider *slider_material_ambient_red;
	CL_Slider *slider_material_ambient_green;
	CL_Slider *slider_material_ambient_blue;
	CL_Label *label_material_ambient_red;
	CL_Label *label_material_ambient_green;
	CL_Label *label_material_ambient_blue;

	CL_Slider *slider_material_specular_red;
	CL_Slider *slider_material_specular_green;
	CL_Slider *slider_material_specular_blue;
	CL_Label *label_material_specular_red;
	CL_Label *label_material_specular_green;
	CL_Label *label_material_specular_blue;

	CL_Slider *slider_material_shininess;
	CL_Label *label_material_shininess;

};

