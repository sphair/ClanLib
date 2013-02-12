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

#include "precomp.h"

#include "options.h"

Options::Options(GUIManager &gui, Rect gui_position) : GUIComponent(&gui, GUITopLevelDescription("Options", gui_position, false))
{
	max_color_value = 1.0f;
	max_angle_value = 360.0f;
	max_shininess_value = 100.0f;

	light_specular_color = Colorf(0.5f, 0.5f, 0.5f, 1.0f);
	light_diffuse_color = Colorf(0.5f, 0.5f, 0.5f, 1.0f);
	light_ambient_color = Colorf(0.0f, 0.0f, 0.0f, 1.0f);

	material_emission_color = Colorf(0.0f, 0.0f, 0.0f, 1.0f);
	material_ambient_color = Colorf(1.0f, 1.0f, 1.0f, 1.0f);
	material_specular_color = Colorf(1.0f, 1.0f, 1.0f, 1.0f);
	material_shininess = 40.0f;

	light_direction_heading = Angle(45.0f, angle_degrees);
	light_direction_pitch = Angle(35.0f, angle_degrees);
	light_direction_bank = Angle(0.0f, angle_degrees);

	int slider_xpos = 8;
	int slider_ypos = 8;
	int slider_gap = 24;
	slider_ypos += 8;
	slider_material_emission_red = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_material_emission_red->func_value_changed().set(this, &Options::slider_material_emission_red_changed);
	slider_material_emission_green = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_material_emission_green->func_value_changed().set(this, &Options::slider_material_emission_green_changed);
	slider_material_emission_blue = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_material_emission_blue->func_value_changed().set(this, &Options::slider_material_emission_blue_changed);
	label_material_emission_red = create_slider_label(slider_material_emission_red);
	label_material_emission_green = create_slider_label(slider_material_emission_green);
	label_material_emission_blue = create_slider_label(slider_material_emission_blue);
	slider_ypos += 8;
	slider_material_ambient_red = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_material_ambient_red->func_value_changed().set(this, &Options::slider_material_ambient_red_changed);
	slider_material_ambient_green = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_material_ambient_green->func_value_changed().set(this, &Options::slider_material_ambient_green_changed);
	slider_material_ambient_blue = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_material_ambient_blue->func_value_changed().set(this, &Options::slider_material_ambient_blue_changed);
	label_material_ambient_red = create_slider_label(slider_material_ambient_red);
	label_material_ambient_green = create_slider_label(slider_material_ambient_green);
	label_material_ambient_blue = create_slider_label(slider_material_ambient_blue);
	slider_ypos += 8;
	slider_material_specular_red = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_material_specular_red->func_value_changed().set(this, &Options::slider_material_specular_red_changed);
	slider_material_specular_green = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_material_specular_green->func_value_changed().set(this, &Options::slider_material_specular_green_changed);
	slider_material_specular_blue = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_material_specular_blue->func_value_changed().set(this, &Options::slider_material_specular_blue_changed);
	label_material_specular_red = create_slider_label(slider_material_specular_red);
	label_material_specular_green = create_slider_label(slider_material_specular_green);
	label_material_specular_blue = create_slider_label(slider_material_specular_blue);
	slider_ypos += 8;
	slider_material_shininess = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_material_shininess->func_value_changed().set(this, &Options::slider_material_shininess_changed);
	label_material_shininess = create_slider_label(slider_material_shininess);

	slider_ypos += 32;
	slider_light_ambient_red = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_light_ambient_red->func_value_changed().set(this, &Options::slider_light_ambient_red_changed);
	slider_light_ambient_green = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_light_ambient_green->func_value_changed().set(this, &Options::slider_light_ambient_green_changed);
	slider_light_ambient_blue = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_light_ambient_blue->func_value_changed().set(this, &Options::slider_light_ambient_blue_changed);
	label_light_ambient_red = create_slider_label(slider_light_ambient_red);
	label_light_ambient_green = create_slider_label(slider_light_ambient_green);
	label_light_ambient_blue = create_slider_label(slider_light_ambient_blue);
	slider_ypos += 8;
	slider_light_diffuse_red = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_light_diffuse_red->func_value_changed().set(this, &Options::slider_light_diffuse_red_changed);
	slider_light_diffuse_green = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_light_diffuse_green->func_value_changed().set(this, &Options::slider_light_diffuse_green_changed);
	slider_light_diffuse_blue = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_light_diffuse_blue->func_value_changed().set(this, &Options::slider_light_diffuse_blue_changed);
	label_light_diffuse_red = create_slider_label(slider_light_diffuse_red);
	label_light_diffuse_green = create_slider_label(slider_light_diffuse_green);
	label_light_diffuse_blue = create_slider_label(slider_light_diffuse_blue);
	slider_ypos += 8;
	slider_light_specular_red = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_light_specular_red->func_value_changed().set(this, &Options::slider_light_specular_red_changed);
	slider_light_specular_green = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_light_specular_green->func_value_changed().set(this, &Options::slider_light_specular_green_changed);
	slider_light_specular_blue = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_light_specular_blue->func_value_changed().set(this, &Options::slider_light_specular_blue_changed);
	label_light_specular_red = create_slider_label(slider_light_specular_red);
	label_light_specular_green = create_slider_label(slider_light_specular_green);
	label_light_specular_blue = create_slider_label(slider_light_specular_blue);
	slider_ypos += 8;
	slider_light_direction_heading = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_light_direction_heading->func_value_changed().set(this, &Options::slider_light_direction_heading_changed);
	slider_light_direction_pitch = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_light_direction_pitch->func_value_changed().set(this, &Options::slider_light_direction_pitch_changed);
	label_light_direction_heading = create_slider_label(slider_light_direction_heading);
	label_light_direction_pitch = create_slider_label(slider_light_direction_pitch);

	set_all_sliders();

	func_render().set(this, &Options::on_render);
}

Options::~Options()
{

}

void Options::on_render(GraphicContext &gc, const Rect &update_rect)
{
	gc.enable_blending(false);
	Draw::fill(gc, update_rect, Colorf(0.6f, 0.6f, 0.8f, 0.0f));
	gc.enable_blending(true);

}

float Options::get_value(Slider *slider, float max_value)
{
	float value = (float) slider->get_position();
	value /= (float) slider->get_max();
	return value * max_value;
}

void Options::set_value(Slider *slider, float value, float max_value)
{
	value /= max_value;
	value *= (float) slider->get_max();
	slider->set_position(value);
}


Slider *Options::create_slider(int xpos, int ypos)
{
	Slider *component = new Slider(this);
	component->set_geometry(Rect(xpos, ypos, Size(128, 17)));
	component->set_vertical(false);
	component->set_horizontal(true);
	component->set_min(0);
	component->set_max(1000);
	component->set_tick_count(100);
	component->set_page_step(100);
	component->set_lock_to_ticks(false);
	component->set_position(component->get_max());
	return component;
}

CheckBox *Options::create_checkbox(int xpos, int ypos, const char *name, bool state)
{
	CheckBox *checkbox = new CheckBox(this);
	checkbox->set_geometry(Rect(xpos, ypos , Size(300, 16)));
	checkbox->set_text(name);
	checkbox->set_checked(state);
	return checkbox;
}

Label *Options::create_slider_label(Slider *slider)
{
	Label *component = new Label(this);
	Rect slider_geometry = slider->get_geometry();
	component->set_geometry(Rect(slider_geometry.right + 4, slider_geometry.top - 2, Size(256, 17)));
	component->set_text("##################");
	return component;
}

void Options::update_all_slider_text()
{
	slider_light_ambient_red_changed();
	slider_light_ambient_green_changed();
	slider_light_ambient_blue_changed();
	slider_light_specular_red_changed();
	slider_light_specular_green_changed();
	slider_light_specular_blue_changed();
	slider_light_diffuse_red_changed();
	slider_light_diffuse_green_changed();
	slider_light_diffuse_blue_changed();
	slider_light_direction_heading_changed();
	slider_light_direction_pitch_changed();

	slider_material_emission_red_changed();
	slider_material_emission_green_changed();
	slider_material_emission_blue_changed();
	slider_material_ambient_red_changed();
	slider_material_ambient_green_changed();
	slider_material_ambient_blue_changed();
	slider_material_specular_red_changed();
	slider_material_specular_green_changed();
	slider_material_specular_blue_changed();
	slider_material_shininess_changed();
}

Label *Options::create_combobox_label(ComboBox *combo, const char *text)
{
	Label *component = new Label(this);
	Rect combo_geometry = combo->get_geometry();
	component->set_geometry(Rect(combo_geometry.left, combo_geometry.top - 20, Size(256, 17)));
	component->set_text(text);
	return component;
}

void Options::slider_light_specular_red_changed()
{
	float value = get_value(slider_light_specular_red, max_color_value);
	std::string text(string_format("Light Specular Red : %1", value));
	label_light_specular_red->set_text(text);
	light_specular_color.r = value;
}

void Options::slider_light_specular_green_changed()
{
	float value = get_value(slider_light_specular_green, max_color_value);
	std::string text(string_format("Light Specular Green : %1", value));
	label_light_specular_green->set_text(text);
	light_specular_color.g = value;
}

void Options::slider_light_specular_blue_changed()
{
	float value = get_value(slider_light_specular_blue, max_color_value);
	std::string text(string_format("Light Specular Blue : %1", value));
	label_light_specular_blue->set_text(text);
	light_specular_color.b = value;
}

void Options::slider_light_diffuse_red_changed()
{
	float value = get_value(slider_light_diffuse_red, max_color_value);
	std::string text(string_format("Light Diffuse Red : %1", value));
	label_light_diffuse_red->set_text(text);
	light_diffuse_color.r = value;
}

void Options::slider_light_diffuse_green_changed()
{
	float value = get_value(slider_light_diffuse_green, max_color_value);
	std::string text(string_format("Light Diffuse Green : %1", value));
	label_light_diffuse_green->set_text(text);
	light_diffuse_color.g = value;
}

void Options::slider_light_diffuse_blue_changed()
{
	float value = get_value(slider_light_diffuse_blue, max_color_value);
	std::string text(string_format("Light Diffuse Blue : %1", value));
	label_light_diffuse_blue->set_text(text);
	light_diffuse_color.b = value;
}

void Options::slider_light_ambient_red_changed()
{
	float value = get_value(slider_light_ambient_red, max_color_value);
	std::string text(string_format("Light Ambient Red : %1", value));
	label_light_ambient_red->set_text(text);
	light_ambient_color.r = value;
}

void Options::slider_light_ambient_green_changed()
{
	float value = get_value(slider_light_ambient_green, max_color_value);
	std::string text(string_format("Light Ambient Green : %1", value));
	label_light_ambient_green->set_text(text);
	light_ambient_color.g = value;
}

void Options::slider_light_ambient_blue_changed()
{
	float value = get_value(slider_light_ambient_blue, max_color_value);
	std::string text(string_format("Light Ambient Blue : %1", value));
	label_light_ambient_blue->set_text(text);
	light_ambient_color.b = value;
}

void Options::slider_material_emission_red_changed()
{
	float value = get_value(slider_material_emission_red, max_color_value);
	std::string text(string_format("Material Emission Red : %1", value));
	label_material_emission_red->set_text(text);
	material_emission_color.r = value;
}

void Options::slider_material_emission_green_changed()
{
	float value = get_value(slider_material_emission_green, max_color_value);
	std::string text(string_format("Material Emission Green : %1", value));
	label_material_emission_green->set_text(text);
	material_emission_color.g = value;
}

void Options::slider_material_emission_blue_changed()
{
	float value = get_value(slider_material_emission_blue, max_color_value);
	std::string text(string_format("Material Emission Blue : %1", value));
	label_material_emission_blue->set_text(text);
	material_emission_color.b = value;
}
void Options::slider_material_ambient_red_changed()
{
	float value = get_value(slider_material_ambient_red, max_color_value);
	std::string text(string_format("Material Ambient Red : %1", value));
	label_material_ambient_red->set_text(text);
	material_ambient_color.r = value;
}

void Options::slider_material_ambient_green_changed()
{
	float value = get_value(slider_material_ambient_green, max_color_value);
	std::string text(string_format("Material Ambient Green : %1", value));
	label_material_ambient_green->set_text(text);
	material_ambient_color.g = value;
}

void Options::slider_material_ambient_blue_changed()
{
	float value = get_value(slider_material_ambient_blue, max_color_value);
	std::string text(string_format("Material Ambient Blue : %1", value));
	label_material_ambient_blue->set_text(text);
	material_ambient_color.b = value;
}
void Options::slider_material_specular_red_changed()
{
	float value = get_value(slider_material_specular_red, max_color_value);
	std::string text(string_format("Material Specular Red : %1", value));
	label_material_specular_red->set_text(text);
	material_specular_color.r = value;
}

void Options::slider_material_specular_green_changed()
{
	float value = get_value(slider_material_specular_green, max_color_value);
	std::string text(string_format("Material Specular Green : %1", value));
	label_material_specular_green->set_text(text);
	material_specular_color.g = value;
}

void Options::slider_material_specular_blue_changed()
{
	float value = get_value(slider_material_specular_blue, max_color_value);
	std::string text(string_format("Material Specular Blue : %1", value));
	label_material_specular_blue->set_text(text);
	material_specular_color.b = value;
}

void Options::slider_material_shininess_changed()
{
	float value = get_value(slider_material_shininess, max_shininess_value);
	std::string text(string_format("Material Shininess : %1", value));
	label_material_shininess->set_text(text);
	material_shininess = value;
}

void Options::slider_light_direction_heading_changed()
{
	float value = get_value(slider_light_direction_heading, max_angle_value);
	std::string text(string_format("Light Heading : %1", value));
	label_light_direction_heading->set_text(text);
	light_direction_heading.set_degrees(value);
}

void Options::slider_light_direction_pitch_changed()
{
	float value = get_value(slider_light_direction_pitch, max_angle_value);
	std::string text(string_format("Light Pitch : %1", value));
	label_light_direction_pitch->set_text(text);
	light_direction_pitch.set_degrees(value);
}

void Options::set_all_sliders()
{
	set_value(slider_light_specular_red, light_specular_color.r, max_color_value);
	set_value(slider_light_specular_green, light_specular_color.g, max_color_value);
	set_value(slider_light_specular_blue, light_specular_color.b, max_color_value);
	set_value(slider_light_diffuse_red, light_diffuse_color.r, max_color_value);
	set_value(slider_light_diffuse_green, light_diffuse_color.g, max_color_value);
	set_value(slider_light_diffuse_blue, light_diffuse_color.b, max_color_value);
	set_value(slider_light_ambient_red, light_ambient_color.r, max_color_value);
	set_value(slider_light_ambient_green, light_ambient_color.g, max_color_value);
	set_value(slider_light_ambient_blue, light_ambient_color.b, max_color_value);

	set_value(slider_material_emission_red, material_emission_color.r, max_color_value);
	set_value(slider_material_emission_green, material_emission_color.g, max_color_value);
	set_value(slider_material_emission_blue, material_emission_color.b, max_color_value);
	set_value(slider_material_ambient_red, material_ambient_color.r, max_color_value);
	set_value(slider_material_ambient_green, material_ambient_color.g, max_color_value);
	set_value(slider_material_ambient_blue, material_ambient_color.b, max_color_value);
	set_value(slider_material_specular_red, material_specular_color.r, max_color_value);
	set_value(slider_material_specular_green, material_specular_color.g, max_color_value);
	set_value(slider_material_specular_blue, material_specular_color.b, max_color_value);

	set_value(slider_material_shininess, material_shininess, max_shininess_value);

	set_value(slider_light_direction_heading, light_direction_heading.to_degrees(), max_angle_value);
	set_value(slider_light_direction_pitch, light_direction_pitch.to_degrees(), max_angle_value);

	update_all_slider_text();
}


