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

	Colorf light_ambient_color;
	Colorf light_specular_color;
	Colorf light_diffuse_color;

	Colorf material_emission_color;
	Colorf material_ambient_color;
	Colorf material_specular_color;
	float material_shininess;

	Angle light_direction_heading;
	Angle light_direction_pitch;
	Angle light_direction_bank;

	float max_color_value;
	float max_angle_value;
	float max_shininess_value;

private:

	void update_all_slider_text();
	Label *create_slider_label(Slider *slider);
	CheckBox *create_checkbox(int xpos, int ypos, const char *name, bool state);
	void on_render(GraphicContext &gc, const Rect &update_rect);
	Slider *create_slider(int xpos, int ypos);
	float get_value(Slider *slider, float max_value);
	void set_value(Slider *slider, float value, float max_value);
	Label *create_combobox_label(ComboBox *combo, const char *text);
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

	Slider *slider_light_specular_red;
	Slider *slider_light_specular_green;
	Slider *slider_light_specular_blue;
	Label *label_light_specular_red;
	Label *label_light_specular_green;
	Label *label_light_specular_blue;

	Slider *slider_light_diffuse_red;
	Slider *slider_light_diffuse_green;
	Slider *slider_light_diffuse_blue;
	Label *label_light_diffuse_red;
	Label *label_light_diffuse_green;
	Label *label_light_diffuse_blue;

	Slider *slider_light_ambient_red;
	Slider *slider_light_ambient_green;
	Slider *slider_light_ambient_blue;
	Label *label_light_ambient_red;
	Label *label_light_ambient_green;
	Label *label_light_ambient_blue;

	Slider *slider_light_direction_heading;
	Slider *slider_light_direction_pitch;
	Label *label_light_direction_heading;
	Label *label_light_direction_pitch;

	Slider *slider_material_emission_red;
	Slider *slider_material_emission_green;
	Slider *slider_material_emission_blue;
	Label *label_material_emission_red;
	Label *label_material_emission_green;
	Label *label_material_emission_blue;

	Slider *slider_material_ambient_red;
	Slider *slider_material_ambient_green;
	Slider *slider_material_ambient_blue;
	Label *label_material_ambient_red;
	Label *label_material_ambient_green;
	Label *label_material_ambient_blue;

	Slider *slider_material_specular_red;
	Slider *slider_material_specular_green;
	Slider *slider_material_specular_blue;
	Label *label_material_specular_red;
	Label *label_material_specular_green;
	Label *label_material_specular_blue;

	Slider *slider_material_shininess;
	Label *label_material_shininess;

};

