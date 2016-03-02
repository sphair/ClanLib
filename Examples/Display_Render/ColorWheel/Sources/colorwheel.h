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

class ColorWheelRadioRow : public clan::View
{
public:
	ColorWheelRadioRow()
	{
		add_child(radiobutton_HSV);
		add_child(label_HSV);
		add_child(radiobutton_HSL);
		add_child(label_HSL);

		style()->set("margin: 3px 0");

		radiobutton_HSV->style()->set("margin-right: 2px");
		radiobutton_HSL->style()->set("margin-right: 2px; margin-left: 15px");

		label_HSV->style()->set("font: 16px Tahoma; -clan-font-rendering: anti-alias; color: white");
		label_HSL->style()->set("font: 16px Tahoma; -clan-font-rendering: anti-alias; color: white");

		label_HSV->set_text("HSV");
		label_HSL->set_text("HSL");
	}

	std::shared_ptr<clan::RadioButtonView> radiobutton_HSV = Theme::create_radiobutton();
	std::shared_ptr<clan::LabelView> label_HSV = std::make_shared<clan::LabelView>();
	std::shared_ptr<clan::RadioButtonView> radiobutton_HSL = Theme::create_radiobutton();
	std::shared_ptr<clan::LabelView> label_HSL = std::make_shared<clan::LabelView>();
};

class ColorWheelSliderRow : public clan::View
{
public:
	ColorWheelSliderRow(const std::string &name)
	{
		add_child(slider);
		add_child(label);

		style()->set("margin: 3px 0");

		slider->style()->set("width: 200px; height: 17px; margin-right: 5px");
		slider->set_horizontal();
		slider->set_min_position(0);
		slider->set_max_position(1000);
		slider->set_tick_count(100);
		slider->set_page_step(100);
		slider->set_position(0);
		slider->set_lock_to_ticks(false);

		label->set_text(name);
	}

	std::shared_ptr<clan::SliderView> slider = Theme::create_slider();
	std::shared_ptr<clan::LabelView> label = Theme::create_label();
};

class ColorWheelOverlay : public clan::View
{
public:
	ColorWheelOverlay()
	{
		style()->set("position: absolute; left: 0; top: 0; right: 0");
		style()->set("flex-direction: column");
		add_child(radio_row);
		add_child(saturation_outer);
		add_child(saturation_inner);
		add_child(value_outer);
		add_child(value_inner);
	}

	std::shared_ptr<ColorWheelRadioRow> radio_row = std::make_shared<ColorWheelRadioRow>();
	std::shared_ptr<ColorWheelSliderRow> saturation_outer = std::make_shared<ColorWheelSliderRow>("Saturation Outer");
	std::shared_ptr<ColorWheelSliderRow> saturation_inner = std::make_shared<ColorWheelSliderRow>("Saturation Inner");
	std::shared_ptr<ColorWheelSliderRow> value_outer = std::make_shared<ColorWheelSliderRow>("Value Outer");
	std::shared_ptr<ColorWheelSliderRow> value_inner = std::make_shared<ColorWheelSliderRow>("Value Inner");
};

class ColorWheel : public clan::View
{
public:
	ColorWheel();

	std::shared_ptr<ColorWheelOverlay> overlay = std::make_shared<ColorWheelOverlay>();

	float saturation_outer = 1.0f;
	float saturation_inner = 0.0f;
	float value_outer = 1.0f;
	float value_inner = 0.0f;
	bool is_hsl = false;

private:
	void draw(clan::Canvas &canvas, const clan::Pointf &center, float radius);
	void render_content(clan::Canvas &canvas) override;
	void get_options();
	float get_value(const std::shared_ptr<clan::SliderView> &slider);
	void option_changed();
	void update_labels();
};
