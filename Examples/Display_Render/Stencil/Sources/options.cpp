/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
	compare_function = clan::CompareFunction::greater;
	stencil_pass = clan::StencilOp::keep;
	stencil_fail = clan::StencilOp::incr;

	num_balls = 9;
	is_moveballs_set = true;
	is_circle_set = true;
	compare_reference = 1;

	int slider_xpos = 400;
	int slider_label_xpos = slider_xpos + 200;
	int slider_ypos = 300;
	int slider_gap = 24;
	slider_numballs = create_slider(slider_xpos, slider_ypos);
	slider_numballs->set_max_position(9);
	slider_numballs->set_position(num_balls);
	slider_numballs->func_value_changed() = bind_member(this, &Options::slider_numballs_changed);
	label_numballs = create_slider_label(slider_label_xpos, slider_ypos);
	slider_ypos += slider_gap;

	slider_compare_functionreference = create_slider(slider_xpos, slider_ypos);
	slider_compare_functionreference->set_max_position(16);
	slider_compare_functionreference->set_position(compare_reference);
	slider_compare_functionreference->func_value_changed() = bind_member(this, &Options::slider_compare_reference_changed);
	label_compare_reference = create_slider_label(slider_label_xpos, slider_ypos);
	slider_ypos += slider_gap;

	int checkbox_xpos = slider_xpos;
	int checkbox_ypos = slider_ypos;
	int checkbox_gap = 24;
	checkbox_moveballs = create_checkbox(checkbox_xpos, checkbox_ypos, "Moving Balls", is_moveballs_set);
	checkbox_moveballs->func_state_changed() = bind_member(this, &Options::checkbox_moveballs_changed);
	checkbox_ypos += checkbox_gap + 8;
	checkbox_circle = create_checkbox(checkbox_xpos, checkbox_ypos, "Draw Stencil Circle (Without selected stencil options)", is_circle_set);
	checkbox_circle->func_state_changed() = bind_member(this, &Options::checkbox_circle_changed);
	checkbox_ypos += checkbox_gap + 8;

	listbox1 = create_listbox(600, 40, "Compare Function");
	insert_compare(listbox1);
	listbox1->func_selection_changed() = [this](){on_equation_compare(listbox1); };
	listbox1->set_selected_item(3);

	listbox2 = create_listbox(600, 180, "Pass Operation");
	insert_passfail(listbox2);
	listbox2->func_selection_changed() = [this](){on_equation_passfail(listbox2, true); };
	listbox2->set_selected_item(0);

	listbox3 = create_listbox(850, 180, "Fail Operation");
	insert_passfail(listbox3);
	listbox3->func_selection_changed() = [this](){on_equation_passfail(listbox3, false); };
	listbox3->set_selected_item(3);


	update_all_slider_text();
}

Options::~Options()
{

}

std::shared_ptr<clan::ListBoxView> Options::create_listbox(int xpos, int ypos, const std::string &title)
{
	auto listbox = Theme::create_listbox();
	add_child(listbox);
	listbox->style()->set("position: absolute; left:%1px; top:%2px; width:%3px; height:%4px;", xpos, ypos, 200, 100);
	auto label_logic = create_slider_label(xpos, ypos - 30);
	label_logic->set_text(title);
	return listbox;
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

void Options::on_equation_compare(std::shared_ptr<clan::ListBoxView> listbox)
{
	int value = listbox->selected_item();
	switch (value)
	{
		case 0:
			compare_function = clan::CompareFunction::lequal;
			break;
		case 1:
			compare_function = clan::CompareFunction::gequal;
			break;
		case 2:
			compare_function = clan::CompareFunction::less;
			break;
		case 3:
			compare_function = clan::CompareFunction::greater;
			break;
		case 4:
			compare_function = clan::CompareFunction::equal;
			break;
		case 5:
			compare_function = clan::CompareFunction::notequal;
			break;
		case 6:
			compare_function = clan::CompareFunction::always;
			break;
		case 7:
			compare_function = clan::CompareFunction::never;
			break;
	}
}

void Options::on_equation_passfail(std::shared_ptr<clan::ListBoxView> listbox, bool is_pass)
{
	int value = listbox->selected_item();
	clan::StencilOp selected;
	switch (value)
	{
		case 0:
			selected = clan::StencilOp::keep;
			break;
		case 1:
			selected = clan::StencilOp::zero;
			break;
		case 2:
			selected = clan::StencilOp::replace;
			break;
		case 3:
			selected = clan::StencilOp::incr;
			break;
		case 4:
			selected = clan::StencilOp::decr;
			break;
		case 5:
			selected = clan::StencilOp::invert;
			break;
		case 6:
			selected = clan::StencilOp::incr_wrap;
			break;
		case 7:
			selected = clan::StencilOp::decr_wrap;
			break;
		default:
			throw clan::Exception("ERROR");
	}
	if (is_pass)
	{
		stencil_pass = selected;
	}
	else
	{
		stencil_fail = selected;
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

void Options::slider_compare_reference_changed()
{
	compare_reference = slider_compare_functionreference->position();
	std::string text(clan::string_format("Stencil Reference : %1", compare_reference));
	label_compare_reference->set_text(text);
}

void Options::update_all_slider_text()
{
	slider_numballs_changed();
	slider_compare_reference_changed();
}

void Options::checkbox_moveballs_changed()
{
	is_moveballs_set = checkbox_moveballs->checked();
}

void Options::checkbox_circle_changed()
{
	is_circle_set = checkbox_circle->checked();
}

void Options::insert_compare(std::shared_ptr<clan::ListBoxView> &listbox)
{
	listbox->set_items<std::string>(
	{
		"lequal", "gequal", "less", "greater", "equal", "notequal", "always", "never"
	}, Theme::create_listbox_label);

}

void Options::insert_passfail(std::shared_ptr<clan::ListBoxView> &listbox)
{
	listbox->set_items<std::string>(
	{
		"keep", "zero", "replace", "incr", "decr", "invert", "incr_wrap", "decr_wrap"
	}, Theme::create_listbox_label);

}
