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

#include "option_rgb.h"

OptionRGB::OptionRGB()
{

}

OptionRGB::~OptionRGB()
{

}

void OptionRGB::setup(clan::GUIComponent *base_component, const std::string &name, int xpos, int ypos)
{

	checkbox = new clan::CheckBox(base_component);
	int width = 100;
	int height = 16;
	checkbox->set_geometry(clan::Rect(xpos, ypos , clan::Size(width, height)));
	xpos += width;
	checkbox->set_text(name);
	checkbox->set_checked(false);
	checkbox->func_state_changed() = bind_member(this, &OptionRGB::checkbox_changed);

	int slider_width = 64;
	slider_red = create_slider(base_component, clan::Rect(xpos, ypos, clan::Size(slider_width, 17)));
	xpos += slider_width + 4;
	slider_red->func_value_changed() = bind_member(this, &OptionRGB::slider_red_changed);

	int spin_width = 40;

	spin_red = new clan::Spin(base_component);
	spin_red->set_geometry(clan::Rect(xpos, ypos - 3, clan::Size(spin_width, 21)));
	xpos += spin_width + 4;
	spin_red->set_ranges(0, 255);
	spin_red->func_value_changed() = bind_member(this, &OptionRGB::spin_red_changed);

	slider_green = create_slider(base_component, clan::Rect(xpos, ypos, clan::Size(slider_width, 17)));
	xpos += slider_width + 4;
	slider_green->func_value_changed() = bind_member(this, &OptionRGB::slider_green_changed);

	spin_green = new clan::Spin(base_component);
	spin_green->set_geometry(clan::Rect(xpos, ypos - 3, clan::Size(spin_width, 21)));
	xpos += spin_width + 4;
	spin_green->set_ranges(0, 255);
	spin_green->func_value_changed() = bind_member(this, &OptionRGB::spin_green_changed);

	slider_blue = create_slider(base_component, clan::Rect(xpos, ypos, clan::Size(slider_width, 17)));
	xpos += slider_width + 4;
	slider_blue->func_value_changed() = bind_member(this, &OptionRGB::slider_blue_changed);

	spin_blue = new clan::Spin(base_component);
	spin_blue->set_geometry(clan::Rect(xpos, ypos - 3, clan::Size(spin_width, 21)));
	xpos += spin_width + 4;
	spin_blue->set_ranges(0, 255);
	spin_blue->func_value_changed() = bind_member(this, &OptionRGB::spin_blue_changed);

	slider_red_changed();
	slider_green_changed();
	slider_blue_changed();

	checkbox_changed();
}

float OptionRGB::get_slider_value(clan::Slider *slider, float max_value)
{
	float value = (float) slider->get_position();
	value /= (float) slider->get_max();
	return value * max_value;
}

void OptionRGB::set_slider_value(clan::Slider *slider, float value, float max_value)
{
	value /= max_value;
	value *= (float) slider->get_max();
	slider->set_position(value);
}


void OptionRGB::slider_red_changed()
{
	css_red = get_slider_value(slider_red, spin_red->get_max());
	spin_red->set_value(css_red);
}

void OptionRGB::spin_red_changed()
{
	css_red = spin_red->get_value();
	set_slider_value(slider_red, css_red, spin_red->get_max());
}

void OptionRGB::slider_green_changed()
{
	css_green = get_slider_value(slider_green, spin_green->get_max());
	spin_green->set_value(css_green);
}

void OptionRGB::spin_green_changed()
{
	css_green = spin_green->get_value();
	set_slider_value(slider_green, css_green, spin_green->get_max());
}

void OptionRGB::slider_blue_changed()
{
	css_blue = get_slider_value(slider_blue, spin_blue->get_max());
	spin_blue->set_value(css_blue);
}

void OptionRGB::spin_blue_changed()
{
	css_blue = spin_blue->get_value();
	set_slider_value(slider_blue, css_blue, spin_blue->get_max_float());
}

void OptionRGB::checkbox_changed()
{
	css_enabled = checkbox->is_checked();
	slider_red->set_enabled(css_enabled);
	spin_red->set_enabled(css_enabled);
	slider_green->set_enabled(css_enabled);
	spin_green->set_enabled(css_enabled);
	slider_blue->set_enabled(css_enabled);
	spin_blue->set_enabled(css_enabled);

}

clan::Slider *OptionRGB::create_slider(clan::GUIComponent *base_component, const clan::Rect &rect)
{
	clan::Slider *slider = new clan::Slider(base_component);
	slider->set_geometry(rect);
	slider->set_vertical(false);
	slider->set_horizontal(true);
	slider->set_min(0);
	slider->set_max(1000);
	slider->set_tick_count(100);
	slider->set_page_step(100);
	slider->set_lock_to_ticks(false);
	slider->set_position(0);
	return slider;
}
