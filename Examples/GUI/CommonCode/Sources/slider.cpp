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
#include "slider.h"

Slider::Slider(clan::GUIManager &manager) : 
	clan::Window(&manager, clan::GUITopLevelDescription("Slider", clan::Rect(256 + 16, 8, clan::Size(256, 256)), false))
{

	func_close().set(this, &Slider::on_close);

	clan::Rect client_area = get_client_area();

	slider_vertical = new clan::Slider(this);
	slider_vertical->set_geometry(clan::Rect(client_area.left + 1, client_area.top + 10, clan::Size(17, 200)));
	slider_vertical->set_vertical(true);
	slider_vertical->set_horizontal(false);
	slider_vertical->set_min(0);
	slider_vertical->set_max(100);
	slider_vertical->set_tick_count(10);
	slider_vertical->set_page_step(40);
	slider_vertical->set_position(50);
	slider_vertical->set_lock_to_ticks(false);
	slider_vertical->func_value_changed().set(this, &Slider::on_value_changed, slider_vertical);
	slider_vertical->func_value_decremented().set(this, &Slider::on_value_decremented, slider_vertical);
	slider_vertical->func_value_incremented().set(this, &Slider::on_value_incremented, slider_vertical);
	slider_vertical->func_slider_moved().set(this, &Slider::on_slider_moved, slider_vertical);

	slider_horizontal = new clan::Slider(this);
	slider_horizontal->set_geometry(clan::Rect(client_area.left + 36, client_area.top + 10, clan::Size(200, 17)));
	slider_horizontal->set_vertical(false);
	slider_horizontal->set_horizontal(true);
	slider_horizontal->set_min(0);
	slider_horizontal->set_max(100);
	slider_horizontal->set_tick_count(10);
	slider_horizontal->set_page_step(40);
	slider_horizontal->set_position(50);
	slider_horizontal->set_lock_to_ticks(false);
	slider_horizontal->func_value_changed().set(this, &Slider::on_value_changed, slider_horizontal);
	slider_horizontal->func_value_decremented().set(this, &Slider::on_value_decremented, slider_horizontal);
	slider_horizontal->func_value_incremented().set(this, &Slider::on_value_incremented, slider_horizontal);
	slider_horizontal->func_slider_moved().set(this, &Slider::on_slider_moved, slider_horizontal);

	clan::Size lineedit_size(42, 20);
	clan::Size label_size(50, 15);
	int lineedit_xpos = client_area.left + 146;
	int label_xpos = client_area.left + 190;

	int lineedit_ypos = client_area.top + 40;
	const int lineedit_gap = 25;

	lineedit_min = new clan::LineEdit(this);
	lineedit_min->set_geometry(clan::Rect(lineedit_xpos, lineedit_ypos, lineedit_size));
	lineedit_min->set_text("0");
	lineedit_min->set_numeric_mode(true);
	lineedit_min->func_enter_pressed().set(this, &Slider::on_min_enter_pressed, lineedit_min);

	lineedit_label_min = new clan::Label(this);
	lineedit_label_min->set_geometry(clan::Rect(label_xpos, lineedit_ypos, label_size));
	lineedit_label_min->set_text("Min");
	lineedit_ypos += lineedit_gap;

	lineedit_max = new clan::LineEdit(this);
	lineedit_max->set_geometry(clan::Rect(lineedit_xpos, lineedit_ypos, lineedit_size));
	lineedit_max->set_text("100");
	lineedit_max->set_numeric_mode(true);
	lineedit_max->func_enter_pressed().set(this, &Slider::on_max_enter_pressed, lineedit_max);

	lineedit_label_max = new clan::Label(this);
	lineedit_label_max->set_geometry(clan::Rect(label_xpos, lineedit_ypos, label_size));
	lineedit_label_max->set_text("Max");
	lineedit_ypos += lineedit_gap;

	lineedit_tick_count = new clan::LineEdit(this);
	lineedit_tick_count->set_geometry(clan::Rect(lineedit_xpos, lineedit_ypos, lineedit_size));
	lineedit_tick_count->set_text("10");
	lineedit_tick_count->func_enter_pressed().set(this, &Slider::on_tick_count_enter_pressed, lineedit_tick_count);

	lineedit_label_tick_count = new clan::Label(this);
	lineedit_label_tick_count->set_geometry(clan::Rect(label_xpos, lineedit_ypos, label_size));
	lineedit_label_tick_count->set_text("Tick Count");
	lineedit_ypos += lineedit_gap;

	lineedit_page_step = new clan::LineEdit(this);
	lineedit_page_step->set_geometry(clan::Rect(lineedit_xpos, lineedit_ypos, lineedit_size));
	lineedit_page_step->set_text("40");
	lineedit_page_step->func_enter_pressed().set(this, &Slider::on_page_step_enter_pressed, lineedit_page_step);

	lineedit_label_page_step = new clan::Label(this);
	lineedit_label_page_step->set_geometry(clan::Rect(label_xpos, lineedit_ypos, label_size));
	lineedit_label_page_step->set_text("Page Step");
	lineedit_ypos += lineedit_gap;

	lineedit_position_vert = new clan::LineEdit(this);
	lineedit_position_vert->set_geometry(clan::Rect(lineedit_xpos, lineedit_ypos, lineedit_size));
	lineedit_position_vert->set_text("50");
	lineedit_position_vert->func_enter_pressed().set(this, &Slider::on_position_enter_pressed_vert, lineedit_position_vert);

	lineedit_label_position_vert = new clan::Label(this);
	lineedit_label_position_vert->set_geometry(clan::Rect(label_xpos, lineedit_ypos, label_size));
	lineedit_label_position_vert->set_text("Vertical");
	lineedit_ypos += lineedit_gap;

	lineedit_position_horiz = new clan::LineEdit(this);
	lineedit_position_horiz->set_geometry(clan::Rect(lineedit_xpos, lineedit_ypos, lineedit_size));
	lineedit_position_horiz->set_text("50");
	lineedit_position_horiz->func_enter_pressed().set(this, &Slider::on_position_enter_pressed_horiz, lineedit_position_horiz);

	lineedit_label_position_horiz = new clan::Label(this);
	lineedit_label_position_horiz->set_geometry(clan::Rect(label_xpos, lineedit_ypos, label_size));
	lineedit_label_position_horiz->set_text("Horizonal");
	lineedit_ypos += lineedit_gap;

	pushbutton_apply = new clan::PushButton(this);
	pushbutton_apply->set_geometry(clan::Rect(lineedit_xpos, lineedit_ypos, clan::Size(48, 20)));
	pushbutton_apply->set_text("Apply");
	pushbutton_apply->func_clicked().set(this, &Slider::on_apply_clicked, pushbutton_apply);

	int xoffset = client_area.left + 21;
	int yoffset = client_area.top + 35;
	const int gap = 16;

	info_value_changed = new Info(this);
	info_value_changed->set(xoffset, yoffset, "Value Changed");
	yoffset += gap;

	info_value_decremented = new Info(this);
	info_value_decremented->set(xoffset, yoffset, "Value Decr.");
	yoffset += gap;

	info_value_incremented = new Info(this);
	info_value_incremented->set(xoffset, yoffset, "Value Incr.");
	yoffset += gap;

	info_slider_moved = new Info(this);
	info_slider_moved->set(xoffset, yoffset, "Slider Moved");
	yoffset += gap;

	xoffset = client_area.left + 36;
	yoffset = client_area.top + 195 - gap;

	checkbox_lock_to_ticks = new clan::CheckBox(this);
	checkbox_lock_to_ticks->set_geometry(clan::Rect(xoffset, yoffset, clan::Size(100, 15)));
	checkbox_lock_to_ticks->func_checked().set(this, &Slider::on_checked_lock_to_ticks, checkbox_lock_to_ticks);
	checkbox_lock_to_ticks->func_unchecked().set(this, &Slider::on_unchecked_lock_to_ticks, checkbox_lock_to_ticks);
	checkbox_lock_to_ticks->set_text("Lock to Ticks");

	yoffset += gap;

	checkbox_disable = new clan::CheckBox(this);
	checkbox_disable->set_geometry(clan::Rect(xoffset, yoffset, clan::Size(100, 15)));
	checkbox_disable->func_checked().set(this, &Slider::on_checked_disable, checkbox_disable);
	checkbox_disable->func_unchecked().set(this, &Slider::on_unchecked_disable, checkbox_disable);
	checkbox_disable->set_text("Disable");
}

void Slider::on_value_changed(clan::Slider *slider)
{
	info_value_changed->activate();

	int position = slider->get_position();

	if (slider == slider_vertical)
	{
		lineedit_position_vert->set_text(position);
	}
	else
	{
		lineedit_position_horiz->set_text(position);
	}
}

void Slider::on_value_decremented(clan::Slider *slider)
{
	info_value_decremented->activate();
}

void Slider::on_value_incremented(clan::Slider *slider)
{
	info_value_incremented->activate();
}

void Slider::on_slider_moved(clan::Slider *slider)
{
	info_slider_moved->activate();
}

void Slider::on_checked_disable(clan::CheckBox *checkbox)
{
	slider_vertical->set_enabled(false);
	slider_horizontal->set_enabled(false);
}

void Slider::on_unchecked_disable(clan::CheckBox *checkbox)
{
	slider_vertical->set_enabled(true);
	slider_horizontal->set_enabled(true);
}

void Slider::on_checked_lock_to_ticks(clan::CheckBox *checkbox)
{
	slider_vertical->set_lock_to_ticks(true);
	slider_horizontal->set_lock_to_ticks(true);
}

void Slider::on_unchecked_lock_to_ticks(clan::CheckBox *checkbox)
{
	slider_vertical->set_lock_to_ticks(false);
	slider_horizontal->set_lock_to_ticks(false);
}

void Slider::on_min_enter_pressed(clan::LineEdit *lineedit)
{
	int value = clan::StringHelp::text_to_int(lineedit->get_text());
	slider_vertical->set_min(value);
	slider_horizontal->set_min(value);
}

void Slider::on_max_enter_pressed(clan::LineEdit *lineedit)
{
	int value = clan::StringHelp::text_to_int(lineedit->get_text());
	slider_vertical->set_max(value);
	slider_horizontal->set_max(value);
}

void Slider::on_tick_count_enter_pressed(clan::LineEdit *lineedit)
{
	int value = clan::StringHelp::text_to_int(lineedit->get_text());
	slider_vertical->set_tick_count(value);
	slider_horizontal->set_tick_count(value);
}

void Slider::on_page_step_enter_pressed(clan::LineEdit *lineedit)
{
	int value = clan::StringHelp::text_to_int(lineedit->get_text());
	slider_vertical->set_page_step(value);
	slider_horizontal->set_page_step(value);
}

void Slider::on_position_enter_pressed_horiz(clan::LineEdit *lineedit)
{
	int value = clan::StringHelp::text_to_int(lineedit->get_text());
	slider_horizontal->set_position(value);
}

void Slider::on_position_enter_pressed_vert(clan::LineEdit *lineedit)
{
	int value = clan::StringHelp::text_to_int(lineedit->get_text());
	slider_vertical->set_position(value);
}

void Slider::on_apply_clicked(clan::PushButton *pushbutton)
{
	int value = clan::StringHelp::text_to_int(lineedit_position_vert->get_text());
	slider_vertical->set_position(value);

	value = clan::StringHelp::text_to_int(lineedit_position_horiz->get_text());
	slider_horizontal->set_position(value);

	value = clan::StringHelp::text_to_int(lineedit_tick_count->get_text());
	slider_vertical->set_tick_count(value);
	slider_horizontal->set_tick_count(value);

	value = clan::StringHelp::text_to_int(lineedit_page_step->get_text());
	slider_vertical->set_page_step(value);
	slider_horizontal->set_page_step(value);

	value = clan::StringHelp::text_to_int(lineedit_max->get_text());
	slider_vertical->set_max(value);
	slider_horizontal->set_max(value);

	value = clan::StringHelp::text_to_int(lineedit_min->get_text());
	slider_vertical->set_min(value);
	slider_horizontal->set_min(value);
}

bool Slider::on_close()
{
	exit_with_code(0);
	return true;
}
