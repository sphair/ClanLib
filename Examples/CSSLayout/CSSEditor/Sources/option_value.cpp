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

#include "option_value.h"

OptionValue::OptionValue()
{

}

OptionValue::~OptionValue()
{

}

void OptionValue::setup(clan::GUIComponent *base_component, const std::string &name, int xpos, int ypos)
{

	checkbox = new clan::CheckBox(base_component);
	int width = 100;
	int height = 16;
	checkbox->set_geometry(clan::Rect(xpos, ypos , clan::Size(width, height)));
	xpos += width;
	checkbox->set_text(name);
	checkbox->set_checked(false);
	checkbox->func_state_changed().set(this, &OptionValue::checkbox_changed);

	slider = new clan::Slider(base_component);
	width = 150;
	slider->set_geometry(clan::Rect(xpos, ypos, clan::Size(width, 17)));
	xpos += width + 4;
	slider->set_vertical(false);
	slider->set_horizontal(true);
	slider->set_min(0);
	slider->set_max(1000);
	slider->set_tick_count(100);
	slider->set_page_step(100);
	slider->set_lock_to_ticks(false);
	slider->set_position(0);
	slider->func_value_changed().set(this, &OptionValue::slider_changed);

	spin = new clan::Spin(base_component);
	width = 64;
	spin->set_geometry(clan::Rect(xpos, ypos - 3, clan::Size(width, 21)));
	xpos += width + 4;
	spin->set_ranges_float(0.0f, 200.0f);
	spin->func_value_changed().set(this, &OptionValue::spin_changed);

	popupmenu.insert_item("px");
	popupmenu.insert_item("em");
	popupmenu.insert_item("pt");
	popupmenu.insert_item("pc");
	popupmenu.insert_item("mm");
	popupmenu.insert_item("cm");
	popupmenu.insert_item("in");
	popupmenu.insert_item("ex");
	combobox = new clan::ComboBox(base_component);
	width = 48;
	combobox->set_geometry(clan::Rect(xpos, ypos - 3, clan::Size(width, 21)));
	xpos += width;
	combobox->set_editable(false);
	combobox->set_dropdown_height(128);
	combobox->set_dropdown_minimum_width(32);
	combobox->set_popup_menu(popupmenu);
	combobox->set_selected_item(0);
	combobox->func_item_selected().set(this, &OptionValue::combobox_changed);

	combobox_changed(0);
	update_spin();
	checkbox_changed();
}

void OptionValue::update_spin()
{
	slider_changed();

}

float OptionValue::get_slider_value(float max_value)
{
	float value = (float) slider->get_position();
	value /= (float) slider->get_max();
	return value * max_value;
}

void OptionValue::set_slider_value(float value, float max_value)
{
	value /= max_value;
	value *= (float) slider->get_max();
	slider->set_position(value);
}


void OptionValue::slider_changed()
{
	css_value = get_slider_value(spin->get_max_float());
	spin->set_value_float(css_value);
}

void OptionValue::spin_changed()
{
	css_value = spin->get_value_float();
	set_slider_value(css_value, spin->get_max_float());
}

void OptionValue::checkbox_changed()
{
	css_enabled = checkbox->is_checked();
	slider->set_enabled(css_enabled);
	spin->set_enabled(css_enabled);
	combobox->set_enabled(css_enabled);
}

void OptionValue::combobox_changed(int index)
{
	int item = combobox->get_selected_item();
	clan::PopupMenuItem menu = popupmenu.get_item(item);
	css_type_length = menu.get_text();
}

