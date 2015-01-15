/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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

/*

Options::Options(clan::GUIManager &gui, clan::Rect gui_position) : clan::GUIComponent(&gui, clan::GUITopLevelDescription("Options", gui_position, false))
{
	// If you change these, you also need to change the default selected combobox item
	blendfunc[0] = clan::blend_src_alpha;
	blendfunc[1] = clan::blend_one_minus_src_alpha;
	blendfunc[2] = clan::blend_src_alpha;
	blendfunc[3] = clan::blend_one_minus_src_alpha;
	blendequation[0] = clan::equation_add;
	blendequation[1] = clan::equation_add;
	logic_operation = clan::logic_set;
	logic_operation_enabled = false;

	primary_color = clan::Colorf::white;
	blend_color = clan::Colorf::white;
	num_balls = 4;
	is_premult_alpha_set = false;
	is_moveballs_set = true;
	is_blending_set = true;

	int slider_xpos = 400;
	int slider_ypos = 200;
	int slider_gap = 24;
	slider_numballs = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_ypos += 8;
	slider_numballs->set_max(9);
	slider_numballs->set_position(4);
	slider_numballs->func_value_changed() = bind_member(this, &Options::slider_numballs_changed);
	slider_primary_red = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_primary_red->func_value_changed() = bind_member(this, &Options::slider_primary_red_changed);
	slider_primary_green = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_primary_green->func_value_changed() = bind_member(this, &Options::slider_primary_green_changed);
	slider_primary_blue = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_primary_blue->func_value_changed() = bind_member(this, &Options::slider_primary_blue_changed);
	slider_primary_alpha = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_primary_alpha->func_value_changed() = bind_member(this, &Options::slider_primary_alpha_changed);
	slider_ypos += 8;
	slider_blend_red = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_blend_red->func_value_changed() = bind_member(this, &Options::slider_blend_red_changed);
	slider_blend_green = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_blend_green->func_value_changed() = bind_member(this, &Options::slider_blend_green_changed);
	slider_blend_blue = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_blend_blue->func_value_changed() = bind_member(this, &Options::slider_blend_blue_changed);
	slider_blend_alpha = create_slider(slider_xpos, slider_ypos); slider_ypos += slider_gap;
	slider_blend_alpha->func_value_changed() = bind_member(this, &Options::slider_blend_alpha_changed);
	slider_ypos += 8;
	label_numballs = create_slider_label(slider_numballs);
	label_primary_red = create_slider_label(slider_primary_red);
	label_primary_green = create_slider_label(slider_primary_green);
	label_primary_blue = create_slider_label(slider_primary_blue);
	label_primary_alpha = create_slider_label(slider_primary_alpha);
	label_blend_red = create_slider_label(slider_blend_red);
	label_blend_green = create_slider_label(slider_blend_green);
	label_blend_blue = create_slider_label(slider_blend_blue);
	label_blend_alpha = create_slider_label(slider_blend_alpha);

	int checkbox_xpos = slider_xpos;
	int checkbox_ypos = slider_ypos;
	int checkbox_gap = 24;
	checkbox_moveballs = create_checkbox(checkbox_xpos, checkbox_ypos, "Moving Balls", is_moveballs_set);
	checkbox_moveballs->func_state_changed() = bind_member(this, &Options::checkbox_moveballs_changed);
	checkbox_blending = create_checkbox(checkbox_xpos + 150, checkbox_ypos, "Enable Blending", is_blending_set);
	checkbox_blending->func_state_changed() = bind_member(this, &Options::checkbox_blending_changed);
	checkbox_premult_alpha = create_checkbox(checkbox_xpos + 300, checkbox_ypos, "Use Premultipied Alpha", is_premult_alpha_set);
	checkbox_premult_alpha->func_state_changed() = bind_member(this, &Options::checkbox_premult_alpha_changed);
	checkbox_ypos += checkbox_gap + 8;

	make_equation_menu(combo_color_equation_menu);
	make_equation_menu(combo_alpha_equation_menu);
	combo_color_equation = create_equation_combo_box(400, 40, combo_color_equation_menu, 0);
	combo_alpha_equation = create_equation_combo_box(550, 40, combo_alpha_equation_menu, 0);
	label_color = create_combobox_label(combo_color_equation, "COLOR EQUATION");
	label_alpha = create_combobox_label(combo_alpha_equation, "ALPHA EQUATION");

	make_logic_menu(combo_logic_menu);
	combo_logic = create_logic_combo_box(700, 40, combo_logic_menu, 0);
	label_logic = create_combobox_label(combo_logic, "LOGIC OPERATION");

	make_blend_menu(combo_src_blend_menu);
	make_blend_menu(combo_dest_blend_menu);
	make_blend_menu(combo_src_alpha_blend_menu);
	make_blend_menu(combo_dest_alpha_blend_menu);
	combo_src_blend = create_blend_combo_box(410, 100, combo_src_blend_menu, 6);
	label_src = create_combobox_label(combo_src_blend, "SOURCE FUNCTION");
	combo_dest_blend = create_blend_combo_box(610, 100, combo_dest_blend_menu, 7);
	label_dest = create_combobox_label(combo_dest_blend, "DEST FUNCTION");
	combo_src_alpha_blend = create_blend_combo_box(410, 160, combo_src_alpha_blend_menu, 6);
	label_src_alpha = create_combobox_label(combo_src_alpha_blend, "SOURCE ALPHA FUNCTION");
	combo_dest_alpha_blend = create_blend_combo_box(610, 160, combo_dest_alpha_blend_menu, 7);
	label_dest_alpha = create_combobox_label(combo_dest_alpha_blend, "DEST ALPHA FUNCTION");

	update_all_slider_text();

	func_render() = bind_member(this, &Options::on_render);
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
	component->set_geometry(clan::Rect(xpos, ypos, clan::Size(256, 17)));
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
	checkbox->set_geometry(clan::Rect(xpos, ypos , clan::Size(140, 16)));
	checkbox->set_text(name);
	checkbox->set_checked(state);
	return checkbox;
}

void Options::on_equation_selected(int value, clan::ComboBox *combo)
{
	int offset = 0;
	if (combo == combo_alpha_equation)
		offset = 1;

	switch (value)
	{
		case 0:
			blendequation[offset] = clan::equation_add;
			break;
		case 1:
			blendequation[offset] = clan::equation_subtract;
			break;
		case 2:
			blendequation[offset] = clan::equation_reverse_subtract;
			break;
		case 3:
			blendequation[offset] = clan::equation_min;
			break;
		case 4:
			blendequation[offset] = clan::equation_max;
			break;
	}
}

void Options::on_logic_selected(int value, clan::ComboBox *combo)
{
	logic_operation_enabled = (value==0) ? false : true;

	switch (value)
	{
		case 1:
			logic_operation = clan::logic_clear;
			break;
		case 2:
			logic_operation = clan::logic_and;
			break;
		case 3:
			logic_operation = clan::logic_and_reverse;
			break;
		case 4:
			logic_operation = clan::logic_copy;
			break;
		case 5:
			logic_operation = clan::logic_and_inverted;
			break;
		case 6:
			logic_operation = clan::logic_noop;
			break;
		case 7:
			logic_operation = clan::logic_xor;
			break;
		case 8:
			logic_operation = clan::logic_or;
			break;
		case 9:
			logic_operation = clan::logic_nor;
			break;
		case 10:
			logic_operation = clan::logic_equiv;
			break;
		case 11:
			logic_operation = clan::logic_invert;
			break;
		case 12:
			logic_operation = clan::logic_or_reverse;
			break;
		case 13:
			logic_operation = clan::logic_copy_inverted;
			break;
		case 14:
			logic_operation = clan::logic_or_inverted;
			break;
		case 15:
			logic_operation = clan::logic_nand;
			break;
		case 16:
			logic_operation = clan::logic_set;
			break;
	}
}

void Options::on_blend_selected(int value, clan::ComboBox *combo)
{
	int offset = -1;

	if (combo == combo_src_blend)
		offset = 0;
	if (combo == combo_dest_blend)
		offset = 1;
	if (combo == combo_src_alpha_blend)
		offset = 2;
	if (combo == combo_dest_alpha_blend)
		offset = 3;

	if (offset < 0)
		return;

	switch (value)
	{
		case 0:
			blendfunc[offset] = clan::blend_zero;
			break;
		case 1:
			blendfunc[offset] = clan::blend_one;
			break;
		case 2:
			blendfunc[offset] = clan::blend_dest_color;
			break;
		case 3:
			blendfunc[offset] = clan::blend_src_color;
			break;
		case 4:
			blendfunc[offset] = clan::blend_one_minus_dest_color;
			break;
		case 5:
			blendfunc[offset] = clan::blend_one_minus_src_color;
			break;
		case 6:
			blendfunc[offset] = clan::blend_src_alpha;
			break;
		case 7:
			blendfunc[offset] = clan::blend_one_minus_src_alpha;
			break;
		case 8:
			blendfunc[offset] = clan::blend_dest_alpha;
			break;
		case 9:
			blendfunc[offset] = clan::blend_one_minus_dest_alpha;
			break;
		case 10:
			blendfunc[offset] = clan::blend_src_alpha_saturate;
			break;
		case 11:
			blendfunc[offset] = clan::blend_constant_color;
			break;
		case 12:
			blendfunc[offset] = clan::blend_one_minus_constant_color;
			break;
		case 13:
			blendfunc[offset] = clan::blend_constant_alpha;
			break;
		case 14:
			blendfunc[offset] = clan::blend_one_minus_constant_alpha;
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

void Options::slider_primary_red_changed()
{
	primary_color.r = get_value(slider_primary_red);
	std::string text(clan::string_format("Primary Color Red : %1", primary_color.r));
	label_primary_red->set_text(text);
}

void Options::slider_primary_green_changed()
{
	primary_color.g = get_value(slider_primary_green);
	std::string text(clan::string_format("Primary Color green : %1", primary_color.g));
	label_primary_green->set_text(text);
}

void Options::slider_primary_blue_changed()
{
	primary_color.b = get_value(slider_primary_blue);
	std::string text(clan::string_format("Primary Color blue : %1", primary_color.b));
	label_primary_blue->set_text(text);
}

void Options::slider_primary_alpha_changed()
{
	primary_color.a = get_value(slider_primary_alpha);
	std::string text(clan::string_format("Primary Color alpha : %1", primary_color.a));
	label_primary_alpha->set_text(text);
}

void Options::slider_blend_red_changed()
{
	blend_color.r = get_value(slider_blend_red);
	std::string text(clan::string_format("Blend Color Red : %1", blend_color.r));
	label_blend_red->set_text(text);
}

void Options::slider_blend_green_changed()
{
	blend_color.g = get_value(slider_blend_green);
	std::string text(clan::string_format("Blend Color green : %1", blend_color.g));
	label_blend_green->set_text(text);
}

void Options::slider_blend_blue_changed()
{
	blend_color.b = get_value(slider_blend_blue);
	std::string text(clan::string_format("Blend Color blue : %1", blend_color.b));
	label_blend_blue->set_text(text);
}

void Options::slider_blend_alpha_changed()
{
	blend_color.a = get_value(slider_blend_alpha);
	std::string text(clan::string_format("Blend Color alpha : %1", blend_color.a));
	label_blend_alpha->set_text(text);
}

void Options::update_all_slider_text()
{
	slider_numballs_changed();
	slider_primary_red_changed();
	slider_primary_green_changed();
	slider_primary_blue_changed();
	slider_primary_alpha_changed();
	slider_blend_red_changed();
	slider_blend_green_changed();
	slider_blend_blue_changed();
	slider_blend_alpha_changed();
}

void Options::checkbox_moveballs_changed()
{
	is_moveballs_set = checkbox_moveballs->is_checked();
}

void Options::checkbox_blending_changed()
{
	is_blending_set = checkbox_blending->is_checked();
}

void Options::checkbox_premult_alpha_changed()
{
	is_premult_alpha_set = checkbox_premult_alpha->is_checked();
}

clan::ComboBox *Options::create_equation_combo_box(int xpos, int ypos, clan::PopupMenu &menu, int selected_item)
{
	clan::ComboBox *combo = new clan::ComboBox(this);
	combo->set_geometry(clan::Rect(xpos, ypos, clan::Size(140, 21)));
	combo->set_editable(false);
	combo->set_dropdown_height(128);
	combo->set_dropdown_minimum_width(64);
	combo->set_popup_menu(menu);
	combo->set_selected_item(selected_item);
	combo->func_item_selected() = [=](int value){on_equation_selected(value, combo); };

	return combo;
}

clan::ComboBox *Options::create_blend_combo_box(int xpos, int ypos, clan::PopupMenu &menu, int selected_item)
{
	clan::ComboBox *combo = new clan::ComboBox(this);
	combo->set_geometry(clan::Rect(xpos, ypos, clan::Size(180, 21)));
	combo->set_editable(false);
	combo->set_dropdown_height(128);
	combo->set_dropdown_minimum_width(64);
	combo->set_popup_menu(menu);
	combo->set_selected_item(selected_item);
	combo->func_item_selected() = [=](int value){on_blend_selected(value, combo); };

	return combo;
}

clan::ComboBox *Options::create_logic_combo_box(int xpos, int ypos, clan::PopupMenu &menu, int selected_item)
{
	clan::ComboBox *combo = new clan::ComboBox(this);
	combo->set_geometry(clan::Rect(xpos, ypos, clan::Size(180, 21)));
	combo->set_editable(false);
	combo->set_dropdown_height(128);
	combo->set_dropdown_minimum_width(64);
	combo->set_popup_menu(menu);
	combo->set_selected_item(selected_item);
	combo->func_item_selected() = [=](int value){on_logic_selected(value, combo); };

	return combo;
}

void Options::make_equation_menu(clan::PopupMenu &menu)
{
	menu.insert_item("Add");
	menu.insert_item("Subtract");
	menu.insert_item("Reverse Subtract");
	menu.insert_item("Min");
	menu.insert_item("Max");
}

void Options::make_blend_menu(clan::PopupMenu &menu)
{
	menu.insert_item("Zero");
	menu.insert_item("One");
	menu.insert_item("Dest Color");
	menu.insert_item("Src Color");
	menu.insert_item("One Minus Dest Color");
	menu.insert_item("One Minus Src Color");
	menu.insert_item("Src Alpha");
	menu.insert_item("One Minus Src Alpha");
	menu.insert_item("Dest Alpha");
	menu.insert_item("One Minus Dest Alpha");
	menu.insert_item("Src Alpha Saturate");
	menu.insert_item("Constant Color");
	menu.insert_item("One Minus Constant Color");
	menu.insert_item("Constant Alpha");
	menu.insert_item("One Minus Constant Alpha");
}

void Options::make_logic_menu(clan::PopupMenu &menu)
{
	menu.insert_item("Disabled");
	menu.insert_item("clear");
	menu.insert_item("and"); 
	menu.insert_item("and_reverse");
	menu.insert_item("copy");
	menu.insert_item("and_inverted");
	menu.insert_item("noop");
	menu.insert_item("xor");
	menu.insert_item("or");
	menu.insert_item("nor");
	menu.insert_item("equiv");
	menu.insert_item("invert");
	menu.insert_item("or_reverse");
	menu.insert_item("copy_inverted");
	menu.insert_item("or_inverted");
	menu.insert_item("nand");
	menu.insert_item("set");
}

clan::Label *Options::create_combobox_label(clan::ComboBox *combo, const char *text)
{
	clan::Label *component = new clan::Label(this);
	clan::Rect combo_geometry = combo->get_geometry();
	component->set_geometry(clan::Rect(combo_geometry.left, combo_geometry.top - 20, clan::Size(256, 17)));
	component->set_text(text);
	return component;
}

*/
