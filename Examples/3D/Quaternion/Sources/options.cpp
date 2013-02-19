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
	button_lerp_clicked = false;
	button_slerp_clicked = false;
	button_rotate_clicked = false;

	max_angle_value = 360.0f;

	rotation_x = Angle(0.0f, angle_degrees);
	rotation_y = Angle(0.0f, angle_degrees);
	rotation_z = Angle(0.0f, angle_degrees);

	target_x = Angle(0.0f, angle_degrees);
	target_y = Angle(0.0f, angle_degrees);
	target_z = Angle(0.0f, angle_degrees);

	int slider_xpos = 8;
	int slider_ypos = 8;
	int slider_gap = 24;
	slider_rotation_y = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_rotation_y->func_value_changed().set(this, &Options::slider_rotation_y_changed);
	slider_rotation_x = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_rotation_x->func_value_changed().set(this, &Options::slider_rotation_x_changed);
	slider_rotation_z = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_rotation_z->func_value_changed().set(this, &Options::slider_rotation_z_changed);
	label_rotation_y = create_slider_label(slider_rotation_y);
	label_rotation_x = create_slider_label(slider_rotation_x);
	label_rotation_z = create_slider_label(slider_rotation_z);

	slider_xpos = 300;
	slider_ypos = 8;

	slider_quaternion_w = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_quaternion_w->func_value_changed().set(this, &Options::slider_quaternion_w_changed);
	label_quaternion_w = create_slider_label(slider_quaternion_w);
	slider_quaternion_i = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_quaternion_i->func_value_changed().set(this, &Options::slider_quaternion_i_changed);
	label_quaternion_i = create_slider_label(slider_quaternion_i);
	slider_quaternion_j = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_quaternion_j->func_value_changed().set(this, &Options::slider_quaternion_j_changed);
	label_quaternion_j = create_slider_label(slider_quaternion_j);
	slider_quaternion_k = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_quaternion_k->func_value_changed().set(this, &Options::slider_quaternion_k_changed);
	label_quaternion_k = create_slider_label(slider_quaternion_k);

	slider_xpos = 600;
	slider_ypos = 8;
	slider_target_y = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_target_y->func_value_changed().set(this, &Options::slider_target_y_changed);
	slider_target_x = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_target_x->func_value_changed().set(this, &Options::slider_target_x_changed);
	slider_target_z = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_target_z->func_value_changed().set(this, &Options::slider_target_z_changed);
	label_target_y = create_slider_label(slider_target_y);
	label_target_x = create_slider_label(slider_target_x);
	label_target_z = create_slider_label(slider_target_z);

	slider_xpos = 8;
	slider_ypos = 120;

	button_lerp = new PushButton(this);
	button_lerp->set_geometry(Rect(slider_xpos, slider_ypos, Size(256, 30)));
	button_lerp->set_text("Linear Quaternion Interpolation (lerp)");
	button_lerp->func_clicked().set(this, &Options::on_clicked_button_lerp);
	slider_xpos += 300;

	button_slerp = new PushButton(this);
	button_slerp->set_geometry(Rect(slider_xpos, slider_ypos, Size(256, 30)));
	button_slerp->set_text("Spherical Linear Interpolation (slerp)");
	button_slerp->func_clicked().set(this, &Options::on_clicked_button_slerp);
	slider_xpos += 300;

	button_rotate = new PushButton(this);
	button_rotate->set_geometry(Rect(slider_xpos, slider_ypos, Size(256, 30)));
	button_rotate->set_text("Rotate : q.multiply(target)");
	button_rotate->func_clicked().set(this, &Options::on_clicked_button_rotate);

	set_all_sliders();

	func_render().set(this, &Options::on_render);
}

Options::~Options()
{

}

void Options::on_render(Canvas &canvas, const Rect &update_rect)
{
	canvas.fill_rect(update_rect, Colorf(0.6f, 0.6f, 0.8f, 1.0f));
}

float Options::get_value(Slider *slider, float min_value, float max_value)
{
	float value = (float) slider->get_position();
	value /= (float) slider->get_max();
	return (value * (max_value - min_value)) + min_value;
}

void Options::set_value(Slider *slider, float value, float min_value, float max_value)
{
	value -= min_value;
	value /= (max_value - min_value);
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
	label_rotation_x->set_text(std::string(string_format("Pitch (x-axis) %1", rotation_x.to_degrees())));
	label_rotation_y->set_text(std::string(string_format("Heading (y-axis) %1", rotation_y.to_degrees())));
	label_rotation_z->set_text(std::string(string_format("Bank (z-axis) %1", rotation_z.to_degrees())));

	label_quaternion_w->set_text(std::string(string_format("Quaternion (w) %1", quaternion.w)));
	label_quaternion_i->set_text(std::string(string_format("Quaternion (i) %1", quaternion.i)));
	label_quaternion_j->set_text(std::string(string_format("Quaternion (j) %1", quaternion.j)));
	label_quaternion_k->set_text(std::string(string_format("Quaternion (k) %1", quaternion.k)));

	label_target_x->set_text(std::string(string_format("Target Pitch %1", target_x.to_degrees())));
	label_target_y->set_text(std::string(string_format("Target Heading %1", target_y.to_degrees())));
	label_target_z->set_text(std::string(string_format("Target Bank %1", target_z.to_degrees())));

}

Label *Options::create_combobox_label(ComboBox *combo, const char *text)
{
	Label *component = new Label(this);
	Rect combo_geometry = combo->get_geometry();
	component->set_geometry(Rect(combo_geometry.left, combo_geometry.top - 20, Size(256, 17)));
	component->set_text(text);
	return component;
}


void Options::slider_rotation_x_changed()
{
	rotation_x.set_degrees(get_value(slider_rotation_x, 0.0f, max_angle_value));
	update_quaternion();
}

void Options::slider_rotation_y_changed()
{
	rotation_y.set_degrees(get_value(slider_rotation_y, 0.0f, max_angle_value));
	update_quaternion();
}

void Options::slider_rotation_z_changed()
{
	rotation_z.set_degrees(get_value(slider_rotation_z, 0.0f, max_angle_value));
	update_quaternion();
}

void Options::slider_quaternion_w_changed()
{
	float value = get_value(slider_quaternion_w, -1.0f, 1.0f);
	for (int cnt=0; cnt < 32; cnt++)	// Iterate, so value quaternion is transformed in a normalised way
	{
		quaternion.w = value;
		quaternion.normalize();
	}
	write_quaternion();

	update_euler();
}

void Options::slider_quaternion_i_changed()
{
	float value = get_value(slider_quaternion_i, -1.0f, 1.0f);
	for (int cnt=0; cnt < 32; cnt++)	// Iterate, so value quaternion is transformed in a normalised way
	{
		quaternion.i = value;
		quaternion.normalize();
	}
	write_quaternion();

	update_euler();
}

void Options::slider_quaternion_j_changed()
{
	float value = get_value(slider_quaternion_j, -1.0f, 1.0f);
	for (int cnt=0; cnt < 32; cnt++)	// Iterate, so value quaternion is transformed in a normalised way
	{
		quaternion.j = value;
		quaternion.normalize();
	}
	write_quaternion();

	update_euler();
}

void Options::slider_quaternion_k_changed()
{
	float value = get_value(slider_quaternion_k, -1.0f, 1.0f);
	for (int cnt=0; cnt < 32; cnt++)	// Iterate, so value quaternion is transformed in a normalised way
	{
		quaternion.k = value;
		quaternion.normalize();
	}
	write_quaternion();
	update_euler();
}

void Options::on_clicked_button_lerp()
{
	button_lerp_clicked = true;
}

void Options::on_clicked_button_slerp()
{
	button_slerp_clicked = true;
}

void Options::on_clicked_button_rotate()
{
	button_rotate_clicked = true;
}


void Options::slider_target_x_changed()
{
	target_x.set_degrees(get_value(slider_target_x, 0.0f, max_angle_value));
	update_all_slider_text();
}

void Options::slider_target_y_changed()
{
	target_y.set_degrees(get_value(slider_target_y, 0.0f, max_angle_value));
	update_all_slider_text();
}

void Options::slider_target_z_changed()
{
	target_z.set_degrees(get_value(slider_target_z, 0.0f, max_angle_value));
	update_all_slider_text();
}

void Options::set_all_sliders()
{
	set_value(slider_rotation_x, rotation_x.to_degrees(), 0, max_angle_value);
	set_value(slider_rotation_y, rotation_y.to_degrees(), 0, max_angle_value);
	set_value(slider_rotation_z, rotation_z.to_degrees(), 0, max_angle_value);

	set_value(slider_target_x, target_x.to_degrees(), 0, max_angle_value);
	set_value(slider_target_y, target_y.to_degrees(), 0, max_angle_value);
	set_value(slider_target_z, target_z.to_degrees(), 0, max_angle_value);

	update_quaternion();

	update_all_slider_text();
}

void Options::update_quaternion()
{
	quaternion = Quaternionf(rotation_x, rotation_y, rotation_z, order_YXZ);
	write_quaternion();
	update_all_slider_text();

}

void Options::write_quaternion()
{
	set_value(slider_quaternion_w, quaternion.w, -1.0f, 1.0f);
	set_value(slider_quaternion_i, quaternion.i, -1.0f, 1.0f);
	set_value(slider_quaternion_j, quaternion.j, -1.0f, 1.0f);
	set_value(slider_quaternion_k, quaternion.k, -1.0f, 1.0f);
}

void Options::update_euler()
{
	Mat4f matrix = quaternion.to_matrix();

	Vec3f euler = matrix.get_euler(order_YXZ);
	rotation_x.set_radians(euler.x);
	rotation_y.set_radians(euler.y);
	rotation_z.set_radians(euler.z);

	// Make 0 to 360 degrees
	rotation_x.normalize();
	rotation_y.normalize();
	rotation_z.normalize();

	set_value(slider_rotation_x, rotation_x.to_degrees(), 0, max_angle_value);
	set_value(slider_rotation_y, rotation_y.to_degrees(), 0, max_angle_value);
	set_value(slider_rotation_z, rotation_z.to_degrees(), 0, max_angle_value);

	update_all_slider_text();
}

void Options::set_new_quaternion(const Quaternionf &new_quaternion)
{
	quaternion = new_quaternion;
	write_quaternion();
	update_euler();
}

