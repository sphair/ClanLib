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
#include "spin.h"

Spin::Spin(clan::GUIManager &manager) :
	clan::Window(&manager, clan::GUITopLevelDescription("Spin", clan::Rect(256*3 + 32, 256 + 16, clan::Size(256, 180)), false))
{

	func_close().set(this, &Spin::on_close);

	clan::Rect client_area = get_client_area();

	spin1 = new clan::Spin(this);
	spin1->set_geometry(clan::Rect(client_area.left + 11, client_area.top + 10, clan::Size(128, 21)));
	spin1->set_number_of_decimal_places(2);
	spin1->set_value(500);
	spin1->set_step_size(50);
	spin1->set_ranges(200, 2000);
	spin1->set_floating_point_mode(false);
	spin1->func_value_changed().set(this, &Spin::on_value_changed, spin1);

	int xoffset = client_area.left + 16;
	int yoffset = client_area.top + 40;
	const int gap = 16;
	clan::Size label_size(50, 15);

	info_value_changed = new Info(this);
	info_value_changed->set(xoffset, yoffset, "Value Changed");
	yoffset += gap;

	clan::Size lineedit_size(48, 20);
	int lineedit_xpos = client_area.left + 6;
	int label_xpos = client_area.left + 56;
	int lineedit_ypos = client_area.top + 60;
	const int lineedit_gap = 25;

	lineedit_value = new clan::LineEdit(this);
	lineedit_value->set_geometry(clan::Rect(lineedit_xpos, lineedit_ypos, lineedit_size));
	lineedit_value->set_text("500");
	lineedit_value->set_numeric_mode(true);
	lineedit_value->func_enter_pressed().set(this, &Spin::on_value_enter_pressed, lineedit_value);

	lineedit_label_value = new clan::Label(this);
	lineedit_label_value->set_geometry(clan::Rect(label_xpos, lineedit_ypos, label_size));
	lineedit_label_value->set_text("Value");
	lineedit_ypos += lineedit_gap;

	lineedit_decimal_places = new clan::LineEdit(this);
	lineedit_decimal_places->set_geometry(clan::Rect(lineedit_xpos, lineedit_ypos, lineedit_size));
	lineedit_decimal_places->set_text("2");
	lineedit_decimal_places->set_numeric_mode(true, true);
	lineedit_decimal_places->func_enter_pressed().set(this, &Spin::on_decimal_places_enter_pressed, lineedit_decimal_places);

	lineedit_label_decimal_places = new clan::Label(this);
	lineedit_label_decimal_places->set_geometry(clan::Rect(label_xpos, lineedit_ypos, label_size));
	lineedit_label_decimal_places->set_text("Decimal Places");
	lineedit_ypos += lineedit_gap;
	lineedit_decimal_places->set_enabled(false);

	lineedit_xpos = client_area.left + 146;
	label_xpos = client_area.left + 196;
	lineedit_ypos = client_area.top + 10;

	lineedit_min = new clan::LineEdit(this);
	lineedit_min->set_geometry(clan::Rect(lineedit_xpos, lineedit_ypos, lineedit_size));
	lineedit_min->set_text("200");
	lineedit_min->set_numeric_mode(true, true);
	lineedit_min->func_enter_pressed().set(this, &Spin::on_min_enter_pressed, lineedit_min);

	lineedit_label_min = new clan::Label(this);
	lineedit_label_min->set_geometry(clan::Rect(label_xpos, lineedit_ypos, label_size));
	lineedit_label_min->set_text("Min");
	lineedit_ypos += lineedit_gap;

	lineedit_max = new clan::LineEdit(this);
	lineedit_max->set_geometry(clan::Rect(lineedit_xpos, lineedit_ypos, lineedit_size));
	lineedit_max->set_text("2000");
	lineedit_max->set_numeric_mode(true, true);
	lineedit_max->func_enter_pressed().set(this, &Spin::on_max_enter_pressed, lineedit_max);

	lineedit_label_max = new clan::Label(this);
	lineedit_label_max->set_geometry(clan::Rect(label_xpos, lineedit_ypos, label_size));
	lineedit_label_max->set_text("Max");
	lineedit_ypos += lineedit_gap;

	lineedit_step = new clan::LineEdit(this);
	lineedit_step->set_geometry(clan::Rect(lineedit_xpos, lineedit_ypos, lineedit_size));
	lineedit_step->set_text("50");
	lineedit_step->set_numeric_mode(true, true);
	lineedit_step->func_enter_pressed().set(this, &Spin::on_step_enter_pressed, lineedit_step);

	lineedit_label_step = new clan::Label(this);
	lineedit_label_step->set_geometry(clan::Rect(label_xpos, lineedit_ypos, label_size));
	lineedit_label_step->set_text("Step");
	lineedit_ypos += lineedit_gap;

	pushbutton_apply = new clan::PushButton(this);
	pushbutton_apply->set_geometry(clan::Rect(lineedit_xpos, lineedit_ypos, clan::Size(48, 20)));
	pushbutton_apply->set_text("Apply");
	pushbutton_apply->func_clicked().set(this, &Spin::on_apply_clicked, pushbutton_apply);

	xoffset = client_area.left + 1;
	yoffset = client_area.top + 126 - gap;

	checkbox_fp_mode = new clan::CheckBox(this);
	checkbox_fp_mode->set_geometry(clan::Rect(xoffset, yoffset, clan::Size(100, 15)));
	checkbox_fp_mode->func_checked().set(this, &Spin::on_checked_fp_mode, checkbox_fp_mode);
	checkbox_fp_mode->func_unchecked().set(this, &Spin::on_unchecked_fp_mode, checkbox_fp_mode);
	checkbox_fp_mode->set_text("Floating Point Mode");

	yoffset += gap;

	checkbox_disable = new clan::CheckBox(this);
	checkbox_disable->set_geometry(clan::Rect(xoffset, yoffset, clan::Size(100, 15)));
	checkbox_disable->func_checked().set(this, &Spin::on_checked_disable, checkbox_disable);
	checkbox_disable->func_unchecked().set(this, &Spin::on_unchecked_disable, checkbox_disable);
	checkbox_disable->set_text("Disable");
}

void Spin::on_value_changed(clan::Spin *spin)
{
	if (spin1->get_floating_point_mode())
	{
		lineedit_value->set_text( spin1->get_value_float(), 2 );
	}
	else
	{
		lineedit_value->set_text( spin1->get_value() );
	}
	info_value_changed->activate();
}

void Spin::on_checked_disable(clan::CheckBox *checkbox)
{
	spin1->set_enabled(false);
}

void Spin::on_unchecked_disable(clan::CheckBox *checkbox)
{
	spin1->set_enabled(true);
}

void Spin::on_checked_fp_mode(clan::CheckBox *checkbox)
{
	spin1->set_floating_point_mode(true);
	lineedit_decimal_places->set_enabled(true);
	on_apply_clicked(pushbutton_apply);
}

void Spin::on_unchecked_fp_mode(clan::CheckBox *checkbox)
{
	spin1->set_floating_point_mode(false);
	lineedit_decimal_places->set_enabled(false);
	on_apply_clicked(pushbutton_apply);
}

void Spin::on_value_enter_pressed(clan::LineEdit *lineedit)
{
	if (spin1->get_floating_point_mode())
	{
		float value = clan::StringHelp::text_to_float(lineedit->get_text());
		spin1->set_value_float(value);
	}
	else
	{
		int value = clan::StringHelp::text_to_int(lineedit->get_text());
		spin1->set_value(value);
	}
}

void Spin::on_min_enter_pressed(clan::LineEdit *lineedit)
{
	if (spin1->get_floating_point_mode())
	{
		float min = clan::StringHelp::text_to_float(lineedit->get_text());
		spin1->set_ranges_float(min, spin1->get_max_float());
	}
	else
	{
		int min = clan::StringHelp::text_to_int(lineedit->get_text());
		spin1->set_ranges(min, spin1->get_max());
	}
}

void Spin::on_max_enter_pressed(clan::LineEdit *lineedit)
{
	if (spin1->get_floating_point_mode())
	{
		float max = clan::StringHelp::text_to_float(lineedit->get_text());
		spin1->set_ranges_float(spin1->get_min_float(), max);
	}
	else
	{
		int max = clan::StringHelp::text_to_int(lineedit->get_text());
		spin1->set_ranges(spin1->get_min(), max);
	}
}

void Spin::on_step_enter_pressed(clan::LineEdit *lineedit)
{
	if (spin1->get_floating_point_mode())
	{
		float step = clan::StringHelp::text_to_float(lineedit->get_text());
		spin1->set_step_size_float(step);
	}
	else
	{
		int step = clan::StringHelp::text_to_int(lineedit->get_text());
		spin1->set_step_size(step);
	}
}

void Spin::on_decimal_places_enter_pressed(clan::LineEdit *lineedit)
{
	int decimal_places = clan::StringHelp::text_to_int(lineedit->get_text());
	spin1->set_number_of_decimal_places(decimal_places);
}

void Spin::on_apply_clicked(clan::PushButton *pushbutton)
{
	on_value_enter_pressed(lineedit_value);
	on_min_enter_pressed(lineedit_min);
	on_max_enter_pressed(lineedit_max);
	on_step_enter_pressed(lineedit_step);
	on_decimal_places_enter_pressed(lineedit_decimal_places);
}

bool Spin::on_close()
{
	exit_with_code(0);
	return true;
}
