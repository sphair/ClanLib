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

#include "options.h"

Options::Options(clan::GUIManager &gui, clan::Rect gui_position) : clan::GUIComponent(&gui, clan::GUITopLevelDescription("Options", gui_position, false))
{
	// Note, when changing these, remember to change the popup menu defaults
	current_mapmode = clan::map_2d_upper_left;
	num_balls = 3;
	grid_angle = 0;
	is_moveballs_set = true;

	int slider_xpos = 450;
	int slider_ypos = 100;
	int slider_gap = 24;
	slider_numballs = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_ypos += 8;
	slider_numballs->set_max(9);
	slider_numballs->set_position(num_balls);
	slider_numballs->func_value_changed().set(this, &Options::slider_numballs_changed);
	slider_ypos += 8;
	label_numballs = create_slider_label(slider_numballs);

	slider_grid_angle = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_ypos += 8;
	slider_grid_angle->set_min(-45);
	slider_grid_angle->set_max(90);
	slider_grid_angle->set_position(grid_angle);
	slider_grid_angle->func_value_changed().set(this, &Options::slider_grid_angle_changed);
	slider_ypos += 8;
	label_grid_angle = create_slider_label(slider_grid_angle);

	int checkbox_xpos = slider_xpos;
	int checkbox_ypos = slider_ypos;
	int checkbox_gap = 24;
	checkbox_moveballs = create_checkbox(checkbox_xpos, checkbox_ypos, "Moving Balls", is_moveballs_set);
	checkbox_moveballs->func_state_changed().set(this, &Options::checkbox_moveballs_changed);

	make_mapmode_menu(combo_mapmode_menu);
	combo_mapmode = create_mapmode_combo_box(450, 40, combo_mapmode_menu, 0);
	label_mapmode = create_combobox_label(combo_mapmode, "Map Mode");

	update_all_slider_text();

	func_render().set(this, &Options::on_render);
}

Options::~Options()
{

}

void Options::on_render(clan::Canvas &canvas, const clan::Rect &update_rect)
{
	clan::Rect rect = get_geometry();
	canvas.fill_rect(update_rect, clan::Colorf(0.6f, 0.6f, 0.2f, 1.0f));
}

float Options::get_value(clan::Slider *slider)
{
	float value = (float) slider->get_position();
	value /= (float) slider->get_max();
	return value;
}

clan::Slider *Options::create_slider(int xpos, int ypos)
{
	clan::Slider *component = new clan::Slider(this);
	component->set_geometry(clan::Rect(xpos, ypos, clan::Size(128, 17)));
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

clan::CheckBox *Options::create_checkbox(int xpos, int ypos, const char *name, bool state)
{
	clan::CheckBox *checkbox = new clan::CheckBox(this);
	checkbox->set_geometry(clan::Rect(xpos, ypos , clan::Size(300, 16)));
	checkbox->set_text(name);
	checkbox->set_checked(state);
	return checkbox;
}


void Options::on_mapmode_selected(int value, clan::ComboBox *combo)
{
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

clan::Label *Options::create_slider_label(clan::Slider *slider)
{
	clan::Label *component = new clan::Label(this);
	clan::Rect slider_geometry = slider->get_geometry();
	component->set_geometry(clan::Rect(slider_geometry.right + 4, slider_geometry.top - 2, clan::Size(256, 17)));
	component->set_text("##################");
	return component;
}

void Options::slider_numballs_changed()
{
	num_balls = slider_numballs->get_position();
	std::string text(clan::string_format("Number of Balls : %1", num_balls));
	label_numballs->set_text(text);
}

void Options::slider_grid_angle_changed()
{
	grid_angle = slider_grid_angle->get_position();
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
	is_moveballs_set = checkbox_moveballs->is_checked();
}

clan::ComboBox *Options::create_mapmode_combo_box(int xpos, int ypos, clan::PopupMenu &menu, int selected_item)
{
	clan::ComboBox *combo = new clan::ComboBox(this);
	combo->set_geometry(clan::Rect(xpos, ypos, clan::Size(180, 21)));
	combo->set_editable(false);
	combo->set_dropdown_height(128);
	combo->set_dropdown_minimum_width(64);
	combo->set_popup_menu(menu);
	combo->set_selected_item(selected_item);
	combo->func_item_selected().set(this, &Options::on_mapmode_selected, combo);

	return combo;
}

void Options::make_mapmode_menu(clan::PopupMenu &menu)
{
	menu.insert_item("2d Upper Left");
	menu.insert_item("2d Lower Left");
	menu.insert_item("User Projection");
}

clan::Label *Options::create_combobox_label(clan::ComboBox *combo, const char *text)
{
	clan::Label *component = new clan::Label(this);
	clan::Rect combo_geometry = combo->get_geometry();
	component->set_geometry(clan::Rect(combo_geometry.left, combo_geometry.top - 20, clan::Size(256, 17)));
	component->set_text(text);
	return component;
}
