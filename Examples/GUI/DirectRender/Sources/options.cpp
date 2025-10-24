/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

Options::Options(CL_GUIManager &gui, CL_Rect gui_position) : CL_GUIComponent(&gui, CL_GUITopLevelDescription("Options", gui_position, false))
{

	primary_color = CL_Colorf::white;
	num_balls = 4;
	is_moveballs_set = true;

	int slider_xpos = 200;
	int slider_ypos = 200;
	int slider_gap = 24;
	slider_numballs = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_ypos += 8;
	slider_numballs->set_max(50);
	slider_numballs->set_position(50);
	slider_numballs->func_value_changed().set(this, &Options::slider_numballs_changed);
	slider_primary_red = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_primary_red->func_value_changed().set(this, &Options::slider_primary_red_changed);
	slider_primary_green = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_primary_green->func_value_changed().set(this, &Options::slider_primary_green_changed);
	slider_primary_blue = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_primary_blue->func_value_changed().set(this, &Options::slider_primary_blue_changed);
	slider_primary_alpha = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_primary_alpha->func_value_changed().set(this, &Options::slider_primary_alpha_changed);
	slider_ypos += 8;
	label_numballs = create_slider_label(slider_numballs);
	label_primary_red = create_slider_label(slider_primary_red);
	label_primary_green = create_slider_label(slider_primary_green);
	label_primary_blue = create_slider_label(slider_primary_blue);
	label_primary_alpha = create_slider_label(slider_primary_alpha);

	int checkbox_xpos = slider_xpos;
	int checkbox_ypos = slider_ypos;
	int checkbox_gap = 24;
	checkbox_moveballs = create_checkbox(checkbox_xpos, checkbox_ypos, "Moving Balls", is_moveballs_set);
	checkbox_moveballs->func_state_changed().set(this, &Options::checkbox_moveballs_changed);
	checkbox_ypos += checkbox_gap + 8;

	make_logic_menu(combo_logic_menu);
	combo_logic = create_logic_combo_box(200, 40, combo_logic_menu, 0);
	label_logic = create_combobox_label(combo_logic, "Example ComboBox");

	update_all_slider_text();

	func_render().set(this, &Options::on_render);
}

Options::~Options()
{
}

void Options::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
}

float Options::get_value(CL_Slider *slider)
{
	float value = (float) slider->get_position();
	value /= (float) slider->get_max();
	return value;
}

CL_Slider *Options::create_slider(int xpos, int ypos)
{
	CL_Slider *component = new CL_Slider(this);
	component->set_geometry(CL_Rect(xpos, ypos, CL_Size(256, 17)));
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

CL_CheckBox *Options::create_checkbox(int xpos, int ypos, const char *name, bool state)
{
	CL_CheckBox *checkbox = new CL_CheckBox(this);
	checkbox->set_geometry(CL_Rect(xpos, ypos , CL_Size(140, 16)));
	checkbox->set_text(name);
	checkbox->set_checked(state);
	return checkbox;
}

void Options::on_logic_selected(int value, CL_ComboBox *combo)
{
}

CL_Label *Options::create_slider_label(CL_Slider *slider)
{
	CL_Label *component = new CL_Label(this);
	CL_Rect slider_geometry = slider->get_geometry();
	component->set_geometry(CL_Rect(slider_geometry.right + 4, slider_geometry.top - 2, CL_Size(256, 17)));
	component->set_text("##################");
	return component;
}

void Options::slider_numballs_changed()
{
	num_balls = slider_numballs->get_position();
	CL_String text(cl_format("Number of Balls : %1", num_balls));
	label_numballs->set_text(text);
}

void Options::slider_primary_red_changed()
{
	primary_color.r = get_value(slider_primary_red);
	CL_String text(cl_format("Primary Color Red : %1", primary_color.r));
	label_primary_red->set_text(text);
}

void Options::slider_primary_green_changed()
{
	primary_color.g = get_value(slider_primary_green);
	CL_String text(cl_format("Primary Color green : %1", primary_color.g));
	label_primary_green->set_text(text);
}

void Options::slider_primary_blue_changed()
{
	primary_color.b = get_value(slider_primary_blue);
	CL_String text(cl_format("Primary Color blue : %1", primary_color.b));
	label_primary_blue->set_text(text);
}

void Options::slider_primary_alpha_changed()
{
	primary_color.a = get_value(slider_primary_alpha);
	CL_String text(cl_format("Primary Color alpha : %1", primary_color.a));
	label_primary_alpha->set_text(text);
}

void Options::update_all_slider_text()
{
	slider_numballs_changed();
	slider_primary_red_changed();
	slider_primary_green_changed();
	slider_primary_blue_changed();
	slider_primary_alpha_changed();
}

void Options::checkbox_moveballs_changed()
{
	is_moveballs_set = checkbox_moveballs->is_checked();
}

CL_ComboBox *Options::create_logic_combo_box(int xpos, int ypos, CL_PopupMenu &menu, int selected_item)
{
	CL_ComboBox *combo = new CL_ComboBox(this);
	combo->set_geometry(CL_Rect(xpos, ypos, CL_Size(180, 21)));
	combo->set_editable(false);
	combo->set_dropdown_height(128);
	combo->set_dropdown_minimum_width(64);
	combo->set_popup_menu(menu);
	combo->set_selected_item(selected_item);
	combo->func_item_selected().set(this, &Options::on_logic_selected, combo);

	return combo;
}

void Options::make_logic_menu(CL_PopupMenu &menu)
{
	menu.insert_item("and"); 
	menu.insert_item("xor");
	menu.insert_item("or");
	menu.insert_item("nor");
}

CL_Label *Options::create_combobox_label(CL_ComboBox *combo, const char *text)
{
	CL_Label *component = new CL_Label(this);
	CL_Rect combo_geometry = combo->get_geometry();
	component->set_geometry(CL_Rect(combo_geometry.left, combo_geometry.top - 20, CL_Size(256, 17)));
	component->set_text(text);
	return component;
}
