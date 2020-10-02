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
	// If you change these, you also need to change the default selected combobox item
	blendfunc[0] = clan::BlendFunc::src_alpha;
	blendfunc[1] = clan::BlendFunc::one_minus_src_alpha;
	blendfunc[2] = clan::BlendFunc::src_alpha;
	blendfunc[3] = clan::BlendFunc::one_minus_src_alpha;
	blendequation[0] = clan::BlendEquation::add;
	blendequation[1] = clan::BlendEquation::add;
	logic_operation = clan::LogicOp::set;
	logic_operation_enabled = false;

	primary_color = clan::Colorf::white;
	blend_color = clan::Colorf::white;
	num_balls = 4;
	is_premult_alpha_set = false;
	is_moveballs_set = true;
	is_blending_set = true;

	int slider_xpos = 400;
	int slider_label_xpos = slider_xpos + 200;
	int slider_ypos = 300;
	int slider_gap = 32;
	slider_numballs = create_slider(slider_xpos, slider_ypos);
	slider_numballs->set_max_position(9);
	slider_numballs->set_position(4);
	slider_numballs->func_value_changed() = bind_member(this, &Options::slider_numballs_changed);
	label_numballs = create_slider_label(slider_label_xpos, slider_ypos);
	slider_ypos += slider_gap;
	slider_primary_red = create_slider(slider_xpos, slider_ypos);
	slider_primary_red->func_value_changed() = bind_member(this, &Options::slider_primary_red_changed);
	label_primary_red = create_slider_label(slider_label_xpos, slider_ypos);
	slider_ypos += slider_gap;
	slider_primary_green = create_slider(slider_xpos, slider_ypos);
	slider_primary_green->func_value_changed() = bind_member(this, &Options::slider_primary_green_changed);
	label_primary_green = create_slider_label(slider_label_xpos, slider_ypos);
	slider_ypos += slider_gap;
	slider_primary_blue = create_slider(slider_xpos, slider_ypos);
	slider_primary_blue->func_value_changed() = bind_member(this, &Options::slider_primary_blue_changed);
	label_primary_blue = create_slider_label(slider_label_xpos, slider_ypos);
	slider_ypos += slider_gap;
	slider_primary_alpha = create_slider(slider_xpos, slider_ypos);
	slider_primary_alpha->func_value_changed() = bind_member(this, &Options::slider_primary_alpha_changed);
	label_primary_alpha = create_slider_label(slider_label_xpos, slider_ypos);
	slider_ypos += slider_gap;
	slider_blend_red = create_slider(slider_xpos, slider_ypos);
	slider_blend_red->func_value_changed() = bind_member(this, &Options::slider_blend_red_changed);
	label_blend_red = create_slider_label(slider_label_xpos, slider_ypos);
	slider_ypos += slider_gap;
	slider_blend_green = create_slider(slider_xpos, slider_ypos);
	slider_blend_green->func_value_changed() = bind_member(this, &Options::slider_blend_green_changed);
	label_blend_green = create_slider_label(slider_label_xpos, slider_ypos);
	slider_ypos += slider_gap;
	slider_blend_blue = create_slider(slider_xpos, slider_ypos);
	slider_blend_blue->func_value_changed() = bind_member(this, &Options::slider_blend_blue_changed);
	label_blend_blue = create_slider_label(slider_label_xpos, slider_ypos);
	slider_ypos += slider_gap;
	slider_blend_alpha = create_slider(slider_xpos, slider_ypos);
	slider_blend_alpha->func_value_changed() = bind_member(this, &Options::slider_blend_alpha_changed);
	label_blend_alpha = create_slider_label(slider_label_xpos, slider_ypos);
	slider_ypos += slider_gap;

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

	listbox_logic = create_listbox(900, 40, "LOGIC OPERATION");
	listbox_logic->set_items<std::string>(
		{ 
			"Disabled", "clear", "and", "and_reverse", "copy", "and_inverted", "noop", "xor", "or", "nor", "equiv", "invert", "or_reverse", "copy_inverted", "or_inverted", "nand", "set" 
		}, Theme::create_listbox_label);
	listbox_logic->func_selection_changed() = clan::bind_member(this, &Options::on_logic_selected);
	listbox_logic->set_selected_item(0);

	std::shared_ptr<clan::ListBoxView> listbox;
	listbox = create_listbox(400, 40, "COLOR EQUATION");
	insert_equation(listbox);
	listbox->func_selection_changed() = [=](){on_equation_selected(listbox, 0); };
	listbox->set_selected_item(0);

	listbox = create_listbox(650, 40, "ALPHA EQUATION");
	insert_equation(listbox);
	listbox->func_selection_changed() = [=](){on_equation_selected(listbox, 1); };
	listbox->set_selected_item(0);

	listbox = create_listbox(400, 180, "SOURCE FUNCTION");
	insert_blend(listbox);
	listbox->func_selection_changed() = [=](){on_blend_selected(listbox, 0); };
	listbox->set_selected_item(6);

	listbox = create_listbox(650, 180, "DEST FUNCTION");
	insert_blend(listbox);
	listbox->func_selection_changed() = [=](){on_blend_selected(listbox, 1); };
	listbox->set_selected_item(7);

	listbox = create_listbox(900, 180, "SOURCE ALPHA FUNCTION");
	insert_blend(listbox);
	listbox->func_selection_changed() = [=](){on_blend_selected(listbox, 2); };
	listbox->set_selected_item(6);

	listbox = create_listbox(1150, 180, "DEST ALPHA FUNCTION");
	insert_blend(listbox);
	listbox->func_selection_changed() = [=](){on_blend_selected(listbox, 3); };
	listbox->set_selected_item(7);

	update_all_slider_text();

}

void Options::insert_equation(std::shared_ptr<clan::ListBoxView> &listbox)
{
	listbox->set_items<std::string>(
	{
		"Add", "Subtract", "Reverse Subtract", "Min", "Max"
	}, Theme::create_listbox_label);

}

void Options::insert_blend(std::shared_ptr<clan::ListBoxView> &listbox)
{
	listbox->set_items<std::string>(
	{
		"Zero", "One", "Dest Color", "Src Color", "One Minus Dest Color", "One Minus Src Color", "Src Alpha",
		"One Minus Src Alpha", "Dest Alpha", "One Minus Dest Alpha", "Src Alpha Saturate", "Constant Color",
		"One Minus Constant Color", "Constant Alpha", "One Minus Constant Alpha"
	}, Theme::create_listbox_label);

}

std::shared_ptr<clan::ListBoxView> Options::create_listbox(int xpos, int ypos, const std::string &title )
{
	auto listbox = Theme::create_listbox();
	add_child(listbox);
	listbox->style()->set("position: absolute; left:%1px; top:%2px; width:%3px; height:%4px;", xpos, ypos, 200, 100);
	auto label_logic = create_slider_label(xpos, ypos - 30);
	label_logic->set_text(title);
	return listbox;
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

void Options::set_value(std::shared_ptr<clan::SliderView> slider, float value, float max_value)
{
	value /= max_value;
	value *= (float)slider->max_position();
	slider->set_position(value);
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

void Options::on_equation_selected(std::shared_ptr<clan::ListBoxView> listbox, int offset)
{
	int value = listbox->selected_item();

	switch (value)
	{
		case 0:
			blendequation[offset] = clan::BlendEquation::add;
			break;
		case 1:
			blendequation[offset] = clan::BlendEquation::subtract;
			break;
		case 2:
			blendequation[offset] = clan::BlendEquation::reverse_subtract;
			break;
		case 3:
			blendequation[offset] = clan::BlendEquation::min;
			break;
		case 4:
			blendequation[offset] = clan::BlendEquation::max;
			break;
	}
}

void Options::on_logic_selected()
{
	int value = listbox_logic->selected_item();
	logic_operation_enabled = (value==0) ? false : true;

	switch (value)
	{
		case 1:
			logic_operation = clan::LogicOp::clear;
			break;
		case 2:
			logic_operation = clan::LogicOp::and;
			break;
		case 3:
			logic_operation = clan::LogicOp::and_reverse;
			break;
		case 4:
			logic_operation = clan::LogicOp::copy;
			break;
		case 5:
			logic_operation = clan::LogicOp::and_inverted;
			break;
		case 6:
			logic_operation = clan::LogicOp::noop;
			break;
		case 7:
			logic_operation = clan::LogicOp::xor;
			break;
		case 8:
			logic_operation = clan::LogicOp::or;
			break;
		case 9:
			logic_operation = clan::LogicOp::nor;
			break;
		case 10:
			logic_operation = clan::LogicOp::equiv;
			break;
		case 11:
			logic_operation = clan::LogicOp::invert;
			break;
		case 12:
			logic_operation = clan::LogicOp::or_reverse;
			break;
		case 13:
			logic_operation = clan::LogicOp::copy_inverted;
			break;
		case 14:
			logic_operation = clan::LogicOp::or_inverted;
			break;
		case 15:
			logic_operation = clan::LogicOp::nand;
			break;
		case 16:
			logic_operation = clan::LogicOp::set;
			break;
	}
}

void Options::on_blend_selected(std::shared_ptr<clan::ListBoxView> listbox, int offset)
{
	int value = listbox->selected_item();

	switch (value)
	{
		case 0:
			blendfunc[offset] = clan::BlendFunc::zero;
			break;
		case 1:
			blendfunc[offset] = clan::BlendFunc::one;
			break;
		case 2:
			blendfunc[offset] = clan::BlendFunc::dest_color;
			break;
		case 3:
			blendfunc[offset] = clan::BlendFunc::src_color;
			break;
		case 4:
			blendfunc[offset] = clan::BlendFunc::one_minus_dest_color;
			break;
		case 5:
			blendfunc[offset] = clan::BlendFunc::one_minus_src_color;
			break;
		case 6:
			blendfunc[offset] = clan::BlendFunc::src_alpha;
			break;
		case 7:
			blendfunc[offset] = clan::BlendFunc::one_minus_src_alpha;
			break;
		case 8:
			blendfunc[offset] = clan::BlendFunc::dest_alpha;
			break;
		case 9:
			blendfunc[offset] = clan::BlendFunc::one_minus_dest_alpha;
			break;
		case 10:
			blendfunc[offset] = clan::BlendFunc::src_alpha_saturate;
			break;
		case 11:
			blendfunc[offset] = clan::BlendFunc::constant_color;
			break;
		case 12:
			blendfunc[offset] = clan::BlendFunc::one_minus_constant_color;
			break;
		case 13:
			blendfunc[offset] = clan::BlendFunc::constant_alpha;
			break;
		case 14:
			blendfunc[offset] = clan::BlendFunc::one_minus_constant_alpha;
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
	is_moveballs_set = checkbox_moveballs->checked();
}

void Options::checkbox_blending_changed()
{
	is_blending_set = checkbox_blending->checked();
}

void Options::checkbox_premult_alpha_changed()
{
	is_premult_alpha_set = checkbox_premult_alpha->checked();
}

