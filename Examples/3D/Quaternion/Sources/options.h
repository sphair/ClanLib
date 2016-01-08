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

#pragma once

#include "../../../ThemeAero/Sources/theme.h"

class SliderOptionView : public View
{
public:
	SliderOptionView()
	{
		style()->set("margin: 2px 0; flex-direction: row;");

		slider->set_min_position(0);
		slider->set_max_position(1000);
		slider->set_tick_count(100);
		slider->set_page_step(100);
		slider->set_lock_to_ticks(false);
		slider->set_position(slider->max_position());

		slider->style()->set("margin: auto 5px auto 0; flex: 1 1");
		label->style()->set("margin: auto 0; flex: 1 1; font: 13px/1.5 'Segoe UI'");

		add_subview(slider);
		add_subview(label);
	}

	float get_value(float min_value, float max_value)
	{
		float value = (float)slider->position();
		value /= (float)slider->max_position();
		return (value * (max_value - min_value)) + min_value;
	}

	void set_value(float value, float min_value, float max_value)
	{
		value -= min_value;
		value /= (max_value - min_value);
		value *= (float)slider->max_position();
		slider->set_position((int)value);
	}

	std::shared_ptr<SliderView> slider = Theme::create_slider();
	std::shared_ptr<LabelView> label = std::make_shared<LabelView>();
};

class OptionColumnView : public View
{
public:
	OptionColumnView()
	{
		style()->set("flex-direction: column; flex: 1 1; margin: 7px;");
	}
};

class Options : public TextureWindow
{
public:
	Options(Canvas &canvas);

	Angle rotation_x;
	Angle rotation_y;
	Angle rotation_z;

	Angle target_x;
	Angle target_y;
	Angle target_z;

	Quaternionf quaternion;

	bool button_lerp_clicked;
	bool button_slerp_clicked;
	bool button_rotate_clicked;

	std::shared_ptr<ButtonView> button_lerp = Theme::create_button();
	std::shared_ptr<ButtonView> button_slerp = Theme::create_button();
	std::shared_ptr<ButtonView> button_rotate = Theme::create_button();

	float max_angle_value;

	void set_new_quaternion(const Quaternionf &new_quaternion);

private:
	void write_quaternion();
	void update_euler();
	void update_quaternion();
	void update_all_slider_text();
	void slider_rotation_x_changed();
	void slider_rotation_y_changed();
	void slider_rotation_z_changed();
	void slider_target_x_changed();
	void slider_target_y_changed();
	void slider_target_z_changed();
	void slider_quaternion_w_changed();
	void slider_quaternion_i_changed();
	void slider_quaternion_j_changed();
	void slider_quaternion_k_changed();
	void on_clicked_button_lerp(PointerEvent &event);
	void on_clicked_button_slerp(PointerEvent &event);
	void on_clicked_button_rotate(PointerEvent &event);
	void set_all_sliders();

private:
	std::shared_ptr<OptionColumnView> column1 = std::make_shared<OptionColumnView>();
	std::shared_ptr<OptionColumnView> column2 = std::make_shared<OptionColumnView>();
	std::shared_ptr<OptionColumnView> column3 = std::make_shared<OptionColumnView>();

	std::shared_ptr<SliderOptionView> rotation_x_view = std::make_shared<SliderOptionView>();
	std::shared_ptr<SliderOptionView> rotation_y_view = std::make_shared<SliderOptionView>();
	std::shared_ptr<SliderOptionView> rotation_z_view = std::make_shared<SliderOptionView>();

	std::shared_ptr<SliderOptionView> target_x_view = std::make_shared<SliderOptionView>();
	std::shared_ptr<SliderOptionView> target_y_view = std::make_shared<SliderOptionView>();
	std::shared_ptr<SliderOptionView> target_z_view = std::make_shared<SliderOptionView>();

	std::shared_ptr<SliderOptionView> quaternion_w_view = std::make_shared<SliderOptionView>();
	std::shared_ptr<SliderOptionView> quaternion_i_view = std::make_shared<SliderOptionView>();
	std::shared_ptr<SliderOptionView> quaternion_j_view = std::make_shared<SliderOptionView>();
	std::shared_ptr<SliderOptionView> quaternion_k_view = std::make_shared<SliderOptionView>();
};
