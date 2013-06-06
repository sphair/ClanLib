/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
#include "GUI.h"

Panel3D::Panel3D(clan::GUIManager &manager) : 
	clan::GUIComponent(&manager, clan::GUITopLevelDescription("3D Controls", clan::Rect(128, 300, clan::Size(320, 448)), false), "window")
{
	clan::Rect client_area = get_geometry();

	int xoffset = client_area.left + 10;
	int yoffset = client_area.top + 10;

	int gap = 32;

	checkbox_animate = new clan::CheckBox(this);
	checkbox_animate->set_geometry(clan::Rect(xoffset, yoffset, clan::Size(128, 40)));
	checkbox_animate->set_text("Animated GUI");
	checkbox_animate->set_checked(true);

	yoffset += gap;
	set_control_group(xoffset, yoffset, slider_zoffset, spin_zoffset, label_zoffset, "Z Offset", 1.5f, 4.6f, 0.2f, 3.5f);

	yoffset += gap;
	set_control_group(xoffset, yoffset, slider_light_position_x, spin_light_position_x, label_light_position_x, "Light Position X", 0.0f, 1.0f, 0.1f, 0.5f);

	yoffset += gap;
	set_control_group(xoffset, yoffset, slider_light_position_y, spin_light_position_y, label_light_position_y, "Light Position Y", 0.0f, 1.0f, 0.1f, 0.5f);

	yoffset += gap;
	set_control_group(xoffset, yoffset, slider_light_position_z, spin_light_position_z, label_light_position_z, "Light Position Z", 0.0f, 1.0f, 0.1f, 0.0f);

	yoffset += gap;
	set_control_group(xoffset, yoffset, slider_light_specular, spin_light_specular, label_light_specular, "Light Specular", 0.0f, 1.0f, 0.1f, 0.8f);

	yoffset += gap;
	set_control_group(xoffset, yoffset, slider_light_diffuse, spin_light_diffuse, label_light_diffuse, "Light Diffuse", 0.0f, 1.0f, 0.1f, 0.8f);

	yoffset += gap;
	set_control_group(xoffset, yoffset, slider_material_shininess, spin_material_shininess, label_material_shininess, "Material Shininess", 0.0f, 1.0f, 0.1f, 0.4f);

	yoffset += gap;
	set_control_group(xoffset, yoffset, slider_material_emission, spin_material_emission, label_material_emission, "Material Emission", 0.0f, 1.0f, 0.1f, 0.2f);

	yoffset += gap;
	set_control_group(xoffset, yoffset, slider_material_specular, spin_material_specular, label_material_specular, "Material Specular", 0.0f, 1.0f, 0.1f, 0.0f);

	yoffset += gap;
	set_control_group(xoffset, yoffset, slider_material_transparency, spin_material_transparency, label_material_transparency, "Material Transparency", 0.0f, 1.0f, 0.1f, 0.0f);

}

void Panel3D::set_control_group(int xoffset, int yoffset, clan::Slider *&slider,
			clan::Spin *&spin, clan::Label *&label, const char *label_name, 
			float min_value, float max_value, float step_value, float initial_value)
{
	clan::Size label_size(100, 15);

	slider = new clan::Slider(this);
	slider->set_geometry(clan::Rect(xoffset, yoffset, clan::Size(100, 17)));
	slider->set_vertical(false);
	slider->set_horizontal(true);
	slider->set_min(0);
	slider->set_max(1000);
	slider->set_tick_count(2);
	slider->set_page_step(100);
	slider->set_position(50);
	slider->set_lock_to_ticks(false);
	slider->func_value_changed().set(this, &Panel3D::on_slider_changed, slider);

	spin = new clan::Spin(this);
	spin->set_geometry(clan::Rect(xoffset+115, yoffset, clan::Size(64, 21)));
	spin->set_number_of_decimal_places(2);
	spin->set_floating_point_mode(true);
	spin->set_value_float(initial_value);
	spin->set_step_size_float(step_value);
	spin->set_ranges_float(min_value, max_value);
	spin->func_value_changed().set(this, &Panel3D::on_spin_changed, spin);

	label = new clan::Label(this);
	label->set_geometry(clan::Rect(xoffset+185, yoffset, label_size));
	label->set_text(label_name);

	set_slider_to_spin(slider, spin);
}

void Panel3D::on_slider_changed(clan::Slider *slider)
{
	clan::Spin *spin;
	if (slider == slider_zoffset)
		spin = spin_zoffset;
	if (slider == slider_light_position_x)
		spin = spin_light_position_x;
	if (slider == slider_light_position_y)
		spin = spin_light_position_y;
	if (slider == slider_light_position_z)
		spin = spin_light_position_z;
	if (slider == slider_light_specular)
		spin = spin_light_specular;
	if (slider == slider_light_diffuse)
		spin = spin_light_diffuse;
	if (slider == slider_material_shininess)
		spin = spin_material_shininess;
	if (slider == slider_material_emission)
		spin = spin_material_emission;
	if (slider == slider_material_specular)
		spin = spin_material_specular;
	if (slider == slider_material_transparency)
		spin = spin_material_transparency;

	if (!spin)
		return;

	float position = (float) slider->get_position();

	position -= slider->get_min();
	float max = (float) (slider->get_max() + 1 - slider->get_min());

	float spin_min = spin->get_min_float();
	float spin_max = spin->get_max_float();
	float spin_range = spin_max - spin_min;

	float value = (position * spin_range) / max;
	value += spin_min;
	if (value < spin_min)
		value = spin_min;

	if (value > spin_max)
		value = spin_max;

	spin->set_value_float(value);
}


void Panel3D::set_slider_to_spin(clan::Slider *slider, clan::Spin *spin)
{
	float value = spin->get_value_float();
	float spin_min = spin->get_min_float();
	float spin_max = spin->get_max_float();
	float spin_range = spin_max - spin_min;

	int slider_max = slider->get_max();
	int slider_min = slider->get_min();

	float slider_range = (float) (slider_max + 1 - slider_min);

	int new_value = (int) (((value - spin_min) * slider_range) / spin_range);
	if (new_value < slider_min)
		new_value = slider_min;

	if (new_value > slider_max)
		new_value = slider_max;

	slider->set_position(new_value);

}

void Panel3D::on_spin_changed(clan::Spin *spin)
{
	clan::Slider *slider;
	if (spin == spin_zoffset)
		slider = slider_zoffset;

	if (spin == spin_light_position_x)
		slider = slider_light_position_x;
	if (spin == spin_light_position_y)
		slider = slider_light_position_y;
	if (spin == spin_light_position_z)
		slider = slider_light_position_z;
	if (spin == spin_light_specular)
		slider = slider_light_specular;
	if (spin == spin_light_diffuse)
		slider = slider_light_diffuse;
	if (spin == spin_material_shininess)
		slider = slider_material_shininess;
	if (spin == spin_material_emission)
		slider = slider_material_emission;
	if (spin == spin_material_specular)
		slider = slider_material_specular;
	if (spin == spin_material_transparency)
		slider = slider_material_transparency;

	if (!slider)
		return;

	set_slider_to_spin(slider, spin);
}

float Panel3D::get_zoffset()
{
	return spin_zoffset->get_value_float();
}

bool Panel3D::is_animated()
{
	return checkbox_animate->is_checked();
}

float Panel3D::get_light_position_x()
{
	return spin_light_position_x->get_value_float();
}

float Panel3D::get_light_position_y()
{
	return spin_light_position_y->get_value_float();
}

float Panel3D::get_light_position_z()
{
	return spin_light_position_z->get_value_float();
}

float Panel3D::get_light_specular()
{
	return spin_light_specular->get_value_float();
}

float Panel3D::get_light_diffuse()
{
	return spin_light_diffuse->get_value_float();
}

float Panel3D::get_material_shininess()
{
	return spin_material_shininess->get_value_float();
}

float Panel3D::get_material_emission()
{
	return spin_material_emission->get_value_float();
}

float Panel3D::get_material_specular()
{
	return spin_material_specular->get_value_float();
}

float Panel3D::get_material_transparency()
{
	return spin_material_transparency->get_value_float();
}
