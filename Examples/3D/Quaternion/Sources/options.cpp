/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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

Options::Options(Canvas &canvas) : TextureWindow(canvas)
{
	root_view()->style()->set("background: rgb(153, 153, 204)");

	auto spacer1 = std::make_shared<View>();
	auto spacer2 = std::make_shared<View>();
	auto spacer3 = std::make_shared<View>();
	spacer1->style()->set("flex: 1 1");
	spacer2->style()->set("flex: 1 1");
	spacer3->style()->set("flex: 1 1");

	add_subview(column1);
	add_subview(column2);
	add_subview(column3);

	column1->add_subview(rotation_y_view);
	column1->add_subview(rotation_x_view);
	column1->add_subview(rotation_z_view);
	column1->add_subview(spacer1);
	column1->add_subview(button_lerp);

	column2->add_subview(quaternion_w_view);
	column2->add_subview(quaternion_i_view);
	column2->add_subview(quaternion_j_view);
	column2->add_subview(quaternion_k_view);
	column2->add_subview(spacer2);
	column2->add_subview(button_slerp);

	column3->add_subview(target_y_view);
	column3->add_subview(target_x_view);
	column3->add_subview(target_z_view);
	column3->add_subview(spacer3);
	column3->add_subview(button_rotate);

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

	rotation_y_view->slider->func_value_changed() = bind_member(this, &Options::slider_rotation_y_changed);
	rotation_x_view->slider->func_value_changed() = bind_member(this, &Options::slider_rotation_x_changed);
	rotation_z_view->slider->func_value_changed() = bind_member(this, &Options::slider_rotation_z_changed);

	quaternion_w_view->slider->func_value_changed() = bind_member(this, &Options::slider_quaternion_w_changed);
	quaternion_i_view->slider->func_value_changed() = bind_member(this, &Options::slider_quaternion_i_changed);
	quaternion_j_view->slider->func_value_changed() = bind_member(this, &Options::slider_quaternion_j_changed);
	quaternion_k_view->slider->func_value_changed() = bind_member(this, &Options::slider_quaternion_k_changed);

	target_y_view->slider->func_value_changed() = bind_member(this, &Options::slider_target_y_changed);
	target_x_view->slider->func_value_changed() = bind_member(this, &Options::slider_target_x_changed);
	target_z_view->slider->func_value_changed() = bind_member(this, &Options::slider_target_z_changed);

	button_lerp->label()->set_text("Linear Quaternion Interpolation (lerp)");
	button_slerp->label()->set_text("Spherical Linear Interpolation (slerp)");
	button_rotate->label()->set_text("Rotate : q.multiply(target)");

	button_lerp->slots.connect(button_lerp->sig_pointer_press(), bind_member(this, &Options::on_clicked_button_lerp));
	button_slerp->slots.connect(button_slerp->sig_pointer_press(), bind_member(this, &Options::on_clicked_button_slerp));
	button_rotate->slots.connect(button_rotate->sig_pointer_press(), bind_member(this, &Options::on_clicked_button_rotate));

	set_all_sliders();
}

void Options::update_all_slider_text()
{
	rotation_x_view->label->set_text(string_format("Pitch (x-axis) %1", rotation_x.to_degrees()));
	rotation_y_view->label->set_text(string_format("Heading (y-axis) %1", rotation_y.to_degrees()));
	rotation_z_view->label->set_text(string_format("Bank (z-axis) %1", rotation_z.to_degrees()));

	quaternion_w_view->label->set_text(string_format("Quaternion (w) %1", quaternion.w));
	quaternion_i_view->label->set_text(string_format("Quaternion (i) %1", quaternion.i));
	quaternion_j_view->label->set_text(string_format("Quaternion (j) %1", quaternion.j));
	quaternion_k_view->label->set_text(string_format("Quaternion (k) %1", quaternion.k));

	target_x_view->label->set_text(string_format("Target Pitch %1", target_x.to_degrees()));
	target_y_view->label->set_text(string_format("Target Heading %1", target_y.to_degrees()));
	target_z_view->label->set_text(string_format("Target Bank %1", target_z.to_degrees()));
}

void Options::slider_rotation_x_changed()
{
	rotation_x.set_degrees(rotation_x_view->get_value(0.0f, max_angle_value));
	update_quaternion();
}

void Options::slider_rotation_y_changed()
{
	rotation_y.set_degrees(rotation_y_view->get_value(0.0f, max_angle_value));
	update_quaternion();
}

void Options::slider_rotation_z_changed()
{
	rotation_z.set_degrees(rotation_z_view->get_value(0.0f, max_angle_value));
	update_quaternion();
}

void Options::slider_quaternion_w_changed()
{
	float value = quaternion_w_view->get_value(-1.0f, 1.0f);
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
	float value = quaternion_i_view->get_value(-1.0f, 1.0f);
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
	float value = quaternion_j_view->get_value(-1.0f, 1.0f);
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
	float value = quaternion_k_view->get_value(-1.0f, 1.0f);
	for (int cnt=0; cnt < 32; cnt++)	// Iterate, so value quaternion is transformed in a normalised way
	{
		quaternion.k = value;
		quaternion.normalize();
	}
	write_quaternion();
	update_euler();
}

void Options::on_clicked_button_lerp(PointerEvent &event)
{
	button_lerp_clicked = true;
}

void Options::on_clicked_button_slerp(PointerEvent &event)
{
	button_slerp_clicked = true;
}

void Options::on_clicked_button_rotate(PointerEvent &event)
{
	button_rotate_clicked = true;
}

void Options::slider_target_x_changed()
{
	target_x.set_degrees(target_x_view->get_value(0.0f, max_angle_value));
	update_all_slider_text();
}

void Options::slider_target_y_changed()
{
	target_y.set_degrees(target_y_view->get_value(0.0f, max_angle_value));
	update_all_slider_text();
}

void Options::slider_target_z_changed()
{
	target_z.set_degrees(target_z_view->get_value(0.0f, max_angle_value));
	update_all_slider_text();
}

void Options::set_all_sliders()
{
	rotation_x_view->set_value(rotation_x.to_degrees(), 0, max_angle_value);
	rotation_y_view->set_value(rotation_y.to_degrees(), 0, max_angle_value);
	rotation_z_view->set_value(rotation_z.to_degrees(), 0, max_angle_value);

	target_x_view->set_value(target_x.to_degrees(), 0, max_angle_value);
	target_y_view->set_value(target_y.to_degrees(), 0, max_angle_value);
	target_z_view->set_value(target_z.to_degrees(), 0, max_angle_value);

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
	quaternion_w_view->set_value(quaternion.w, -1.0f, 1.0f);
	quaternion_i_view->set_value(quaternion.i, -1.0f, 1.0f);
	quaternion_j_view->set_value(quaternion.j, -1.0f, 1.0f);
	quaternion_k_view->set_value(quaternion.k, -1.0f, 1.0f);
}

void Options::update_euler()
{
	Mat4f matrix = quaternion.to_matrix();

//	Vec3f euler = matrix.get_euler(order_YXZ);
	Vec3f euler = matrix.get_euler(order_ZXY);
	rotation_x.set_radians(euler.x);
	rotation_y.set_radians(euler.y);
	rotation_z.set_radians(euler.z);

	// Make 0 to 360 degrees
	rotation_x.normalize();
	rotation_y.normalize();
	rotation_z.normalize();

	rotation_x_view->set_value(rotation_x.to_degrees(), 0, max_angle_value);
	rotation_y_view->set_value(rotation_y.to_degrees(), 0, max_angle_value);
	rotation_z_view->set_value(rotation_z.to_degrees(), 0, max_angle_value);

	update_all_slider_text();
}

void Options::set_new_quaternion(const Quaternionf &new_quaternion)
{
	quaternion = new_quaternion;
	write_quaternion();
	update_euler();
}
