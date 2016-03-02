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

Options::Options(clan::Canvas &canvas) : clan::TextureWindow(canvas)
{
	// Note, when changing these, remember to change the popup menu defaults
	current_mapmode = clan::map_2d_upper_left;
	num_balls = 3;
	grid_angle = 0;
	is_moveballs_set = true;

	int slider_xpos = 450;
	int slider_label_xpos = slider_xpos + 200;
	int slider_ypos = 100;
	int slider_gap = 24;
	slider_numballs = create_slider(slider_xpos, slider_ypos);
	slider_numballs->set_max_position(9);
	slider_numballs->set_position(num_balls);
	slider_numballs->func_value_changed() = bind_member(this, &Options::slider_numballs_changed);
	label_numballs = create_slider_label(slider_label_xpos, slider_ypos);
	slider_ypos += slider_gap;

	slider_grid_angle = create_slider(slider_xpos, slider_ypos);
	slider_grid_angle->set_min_position(-45);
	slider_grid_angle->set_max_position(90);
	slider_grid_angle->set_position(grid_angle);
	slider_grid_angle->func_value_changed() = bind_member(this, &Options::slider_grid_angle_changed);
	label_grid_angle = create_slider_label(slider_label_xpos, slider_ypos);
	slider_ypos += slider_gap;

	int checkbox_xpos = slider_xpos;
	int checkbox_ypos = slider_ypos;
	int checkbox_gap = 24;
	checkbox_moveballs = create_checkbox(checkbox_xpos, checkbox_ypos, "Moving Balls", is_moveballs_set);
	checkbox_moveballs->func_state_changed() = bind_member(this, &Options::checkbox_moveballs_changed);

	listbox_mapmode = Theme::create_listbox();
	listbox_mapmode->style()->set("position: absolute; left:%1px; top:%2px; width:%3px; height:auto;", checkbox_xpos, 10, 180);

	listbox_mapmode->set_items<std::string>( { "2d Upper Left", "2d Lower Left", "User Projection" }, Theme::create_listbox_label);

	listbox_mapmode->func_selection_changed() = bind_member(this, &Options::on_mapmode_selected);
	listbox_mapmode->set_selected_item(0);
	add_child(listbox_mapmode);

	label_mapmode = Theme::create_label(true);
	add_child(label_mapmode);
	label_mapmode->style()->set("position: absolute; left:%1px; top:%2px", slider_label_xpos, 10);
	label_mapmode->set_text("Map Mode");

	update_all_slider_text();
}

Options::~Options()
{

}

float Options::get_value(std::shared_ptr<clan::SliderView> slider)
{
	float value = (float)slider->position();
	value /= (float)slider->max_position();
	return value;
}

std::shared_ptr<clan::SliderView> Options::create_slider(int xpos, int ypos)
{
	std::shared_ptr<clan::SliderView> component = Theme::create_slider();
	add_child(component);

	component->style()->set("position: absolute; left:%1px; top:%2px; width:%3px; height:auto;", xpos, ypos, 192);
	component->set_horizontal();
	component->set_min_position(0);
	component->set_max_position(1000);
	component->set_tick_count(100);
	component->set_page_step(100);
	component->set_lock_to_ticks(false);
	component->set_position(component->max_position());

	return component;

}

std::shared_ptr<clan::CheckBoxView> Options::create_checkbox(int xpos, int ypos, const std::string &name, bool state)
{
	std::shared_ptr<clan::CheckBoxView> checkbox = Theme::create_checkbox();
	add_child(checkbox);
	checkbox->style()->set("position: absolute; left:%1px; top:%2px", xpos, ypos);
	checkbox->set_check(state);

	auto label = Theme::create_label(true);
	label->set_text(name);
	label->style()->set("position: absolute; left:%1px; top:%2px", xpos + 16, ypos - 3);
	add_child(label);

	return checkbox;
}

void Options::on_mapmode_selected()
{
	int value = listbox_mapmode->selected_item();
	switch (value)
	{
		case 0:
			current_mapmode = clan::map_2d_upper_left;
			break;
		case 1:
			current_mapmode = clan::map_2d_lower_left;
			break;
		case 2:
			current_mapmode = clan::map_user_projection;
			break;
	}
}

std::shared_ptr<clan::LabelView> Options::create_slider_label(int xpos, int ypos)
{
	std::shared_ptr<clan::LabelView> component = Theme::create_label(true);
	add_child(component);
	component->style()->set("position: absolute; left:%1px; top:%2px", xpos, ypos);
	component->set_text("##################");
	return component;
}

void Options::slider_numballs_changed()
{
	num_balls = slider_numballs->position();
	std::string text(clan::string_format("Number of Balls : %1", num_balls));
	label_numballs->set_text(text);
}

void Options::slider_grid_angle_changed()
{
	grid_angle = slider_grid_angle->position();
	std::string text(clan::string_format("Grid Rotation : %1 (user projection)", grid_angle));
	label_grid_angle->set_text(text);
}

void Options::update_all_slider_text()
{
	slider_numballs_changed();
	slider_grid_angle_changed();
}

void Options::checkbox_moveballs_changed()
{
	is_moveballs_set = checkbox_moveballs->checked();
}

/*
clan::ComboBox *Options::create_mapmode_combo_box(int xpos, int ypos, clan::PopupMenu &menu, int selected_item)
{
	clan::ComboBox *combo = new clan::ComboBox(this);
	combo->set_geometry(clan::Rect(xpos, ypos, clan::Size(180, 21)));
	combo->set_editable(false);
	combo->set_dropdown_height(128);
	combo->set_dropdown_minimum_width(64);
	combo->set_popup_menu(menu);
	combo->set_selected_item(selected_item);
	combo->func_item_selected() = bind_member(this, &Options::on_mapmode_selected);

	return combo;
}

void Options::make_mapmode_menu(clan::PopupMenu &menu)
{
	menu.insert_item("2d Upper Left");
	menu.insert_item("2d Lower Left");
	menu.insert_item("User Projection");
}

std::shared_ptr<clan::LabelView> Options::create_combobox_label(clan::ComboBox *combo, const char *text)
{
	std::shared_ptr<clan::LabelView> component = new clan::Label(this);
	clan::Rect combo_geometry = combo->get_geometry();
	component->set_geometry(clan::Rect(combo_geometry.left, combo_geometry.top - 20, clan::Size(256, 17)));
	component->set_text(text);
	return component;
}
*/
