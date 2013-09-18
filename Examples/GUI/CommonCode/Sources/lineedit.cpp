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
#include "lineedit.h"

LineEdit::LineEdit(clan::GUIManager &manager) :
	clan::Window(&manager, clan::GUITopLevelDescription("Line Edit", clan::Rect(512 + 24, 8, clan::Size(256, 256)), false))
{
	func_close().set(this, &LineEdit::on_close);


	clan::Rect client_area = get_client_area();

	lineedit1 = new clan::LineEdit(this);
	lineedit1->set_geometry(clan::Rect(client_area.left + 11, client_area.top + 10, clan::Size(128, 21)));
	lineedit1->set_text("Line Edit");

	lineedit1->func_before_edit_changed().set(this, &LineEdit::on_before_edit_changed, lineedit1);
	lineedit1->func_after_edit_changed().set(this, &LineEdit::on_after_edit_changed, lineedit1);
	lineedit1->func_selection_changed().set(this, &LineEdit::on_selection_changed, lineedit1);
	lineedit1->func_focus_gained().set(this, &LineEdit::on_focus_gained, lineedit1);
	lineedit1->func_focus_lost().set(this, &LineEdit::on_focus_lost, lineedit1);
	lineedit1->func_enter_pressed().set(this, &LineEdit::on_enter_pressed, lineedit1);

	int xoffset = client_area.left + 96;
	int yoffset = client_area.top + 40;
	const int gap = 16;

	info_before_edit_changed = new Info(this);
	info_before_edit_changed->set(xoffset, yoffset, "Before Edit Changed");
	yoffset += gap;
	info_after_edit_changed = new Info(this);
	info_after_edit_changed->set(xoffset, yoffset, "After Edit Changed");
	yoffset += gap;
	info_selection_changed = new Info(this);
	info_selection_changed->set(xoffset, yoffset, "Selection Changed");
	yoffset += gap;
	info_focus_gained = new Info(this);
	info_focus_gained->set(xoffset, yoffset, "Focus Gained");
	yoffset += gap;
	info_focus_lost = new Info(this);
	info_focus_lost->set(xoffset, yoffset, "Focus Lost");
	yoffset += gap;
	info_enter_pressed = new Info(this);
	info_enter_pressed->set(xoffset, yoffset, "Enter Pressed");
	yoffset += gap;

	xoffset = client_area.left + 1;
	yoffset = client_area.top + 195 - gap*8;
	clan::Size label_size(50, 15);

	checkbox_mask_hex = new clan::CheckBox(this);
	checkbox_mask_hex->set_geometry(clan::Rect(xoffset, yoffset, clan::Size(100, 15)));
	checkbox_mask_hex->func_checked().set(this, &LineEdit::on_checked_mask_hex, checkbox_mask_hex);
	checkbox_mask_hex->func_unchecked().set(this, &LineEdit::on_unchecked_mask_hex, checkbox_mask_hex);
	checkbox_mask_hex->set_text("Hex Mask");

	yoffset+=gap;
	checkbox_read_only = new clan::CheckBox(this);
	checkbox_read_only->set_geometry(clan::Rect(xoffset, yoffset, clan::Size(100, 15)));
	checkbox_read_only->func_checked().set(this, &LineEdit::on_checked_read_only, checkbox_read_only);
	checkbox_read_only->func_unchecked().set(this, &LineEdit::on_unchecked_read_only, checkbox_read_only);
	checkbox_read_only->set_text("Read Only");

	yoffset+=gap;
	checkbox_lowercase = new clan::CheckBox(this);
	checkbox_lowercase->set_geometry(clan::Rect(xoffset, yoffset, clan::Size(100, 15)));
	checkbox_lowercase->func_checked().set(this, &LineEdit::on_checked_lowercase, checkbox_lowercase);
	checkbox_lowercase->func_unchecked().set(this, &LineEdit::on_unchecked_lowercase, checkbox_lowercase);
	checkbox_lowercase->set_text("Lowercase");

	yoffset+=gap;
	checkbox_uppercase = new clan::CheckBox(this);
	checkbox_uppercase->set_geometry(clan::Rect(xoffset, yoffset, clan::Size(100, 15)));
	checkbox_uppercase->func_checked().set(this, &LineEdit::on_checked_uppercase, checkbox_uppercase);
	checkbox_uppercase->func_unchecked().set(this, &LineEdit::on_unchecked_uppercase, checkbox_uppercase);
	checkbox_uppercase->set_text("Uppercase");

	yoffset+=gap;
	checkbox_password_mode = new clan::CheckBox(this);
	checkbox_password_mode->set_geometry(clan::Rect(xoffset, yoffset, clan::Size(100, 15)));
	checkbox_password_mode->func_checked().set(this, &LineEdit::on_checked_password_mode, checkbox_password_mode);
	checkbox_password_mode->func_unchecked().set(this, &LineEdit::on_unchecked_password_mode, checkbox_password_mode);
	checkbox_password_mode->set_text("Password Mode");

	yoffset+=gap;
	checkbox_numeric_mode = new clan::CheckBox(this);
	checkbox_numeric_mode->set_geometry(clan::Rect(xoffset, yoffset, clan::Size(100, 15)));
	checkbox_numeric_mode->func_checked().set(this, &LineEdit::on_checked_numeric_mode, checkbox_numeric_mode);
	checkbox_numeric_mode->func_unchecked().set(this, &LineEdit::on_unchecked_numeric_mode, checkbox_numeric_mode);
	checkbox_numeric_mode->set_text("Numeric Mode");

	yoffset+=gap;
	checkbox_numeric_mode_decimals = new clan::CheckBox(this);
	checkbox_numeric_mode_decimals->set_geometry(clan::Rect(xoffset, yoffset, clan::Size(100, 15)));
	checkbox_numeric_mode_decimals->func_checked().set(this, &LineEdit::on_checked_numeric_mode_decimals, checkbox_numeric_mode_decimals);
	checkbox_numeric_mode_decimals->func_unchecked().set(this, &LineEdit::on_unchecked_numeric_mode_decimals, checkbox_numeric_mode_decimals);
	checkbox_numeric_mode_decimals->set_text("Numeric Mode Decimals");
	checkbox_numeric_mode_decimals->set_enabled(false);

	yoffset+=gap;
	pushbutton_resize = new clan::PushButton(this);
	pushbutton_resize->set_geometry(clan::Rect(xoffset, yoffset, clan::Size(64, 20)));
	pushbutton_resize->set_text("Resize");
	pushbutton_resize->func_clicked().set(this, &LineEdit::on_resize_clicked, pushbutton_resize);

	yoffset+=gap+4;
	checkbox_disable = new clan::CheckBox(this);
	checkbox_disable->set_geometry(clan::Rect(xoffset, yoffset, clan::Size(100, 15)));
	checkbox_disable->func_checked().set(this, &LineEdit::on_checked_disable, checkbox_disable);
	checkbox_disable->func_unchecked().set(this, &LineEdit::on_unchecked_disable, checkbox_disable);
	checkbox_disable->set_text("Disable");

	xoffset = client_area.left + 101;
	yoffset = client_area.top + 190;
	menu_settext.insert_item("(Settings)");
	menu_settext.insert_item("Set Align: Left");
	menu_settext.insert_item("Set Align: Centre");
	menu_settext.insert_item("Set Align: Right");
	menu_settext.insert_item("Set Text: Hello World");
	menu_settext.insert_item("Set Integer: 1234");
	menu_settext.insert_item("Set Float: 123.456");
	menu_settext.insert_item("Select: All");
	menu_settext.insert_item("Select: 2nd to 5th char");
	menu_settext.insert_item("Select: None");

	combo_settext = new clan::ComboBox(this);
	combo_settext->set_geometry(clan::Rect(xoffset, yoffset, clan::Size(130, 20)));
	combo_settext->set_editable(false);
	combo_settext->set_dropdown_height(128);
	combo_settext->set_dropdown_minimum_width(64);
	combo_settext->set_popup_menu(menu_settext);
	combo_settext->set_selected_item(0);
	combo_settext->func_item_selected().set(this, &LineEdit::on_settext_selected, combo_settext);
}

void LineEdit::on_before_edit_changed(clan::InputEvent &input_event, clan::LineEdit *lineedit)
{
	info_before_edit_changed->activate();
}

void LineEdit::on_after_edit_changed(clan::InputEvent &input_event, clan::LineEdit *lineedit)
{
	info_after_edit_changed->activate();
}

void LineEdit::on_selection_changed(clan::LineEdit *lineedit)
{
	info_selection_changed->activate();
}

void LineEdit::on_focus_gained(clan::LineEdit *lineedit)
{
	info_focus_gained->activate();
}

void LineEdit::on_focus_lost(clan::LineEdit *lineedit)
{
	info_focus_lost->activate();
}

void LineEdit::on_enter_pressed(clan::LineEdit *lineedit)
{
	info_enter_pressed->activate();
}

void LineEdit::on_checked_disable(clan::CheckBox *checkbox)
{
	lineedit1->set_enabled(false);
}

void LineEdit::on_unchecked_disable(clan::CheckBox *checkbox)
{
	lineedit1->set_enabled(true);
}

void LineEdit::on_checked_read_only(clan::CheckBox *checkbox)
{
	lineedit1->set_read_only(true);
}

void LineEdit::on_unchecked_read_only(clan::CheckBox *checkbox)
{
	lineedit1->set_read_only(false);
}

void LineEdit::on_checked_lowercase(clan::CheckBox *checkbox)
{
	lineedit1->set_lowercase(true);
	checkbox_uppercase->set_enabled(false);
}

void LineEdit::on_unchecked_lowercase(clan::CheckBox *checkbox)
{
	lineedit1->set_lowercase(false);
	checkbox_uppercase->set_enabled(true);
}

void LineEdit::on_checked_uppercase(clan::CheckBox *checkbox)
{
	lineedit1->set_uppercase(true);
	checkbox_lowercase->set_enabled(false);
}

void LineEdit::on_unchecked_uppercase(clan::CheckBox *checkbox)
{
	lineedit1->set_uppercase(false);
	checkbox_lowercase->set_enabled(true);
}

void LineEdit::on_checked_password_mode(clan::CheckBox *checkbox)
{
	lineedit1->set_password_mode(true);
}

void LineEdit::on_unchecked_password_mode(clan::CheckBox *checkbox)
{
	lineedit1->set_password_mode(false);
}

void LineEdit::on_checked_numeric_mode(clan::CheckBox *checkbox)
{
	lineedit1->set_numeric_mode(true, checkbox_numeric_mode_decimals->is_checked());
	checkbox_numeric_mode_decimals->set_enabled(true);
}

void LineEdit::on_unchecked_numeric_mode(clan::CheckBox *checkbox)
{
	lineedit1->set_numeric_mode(false);
	checkbox_numeric_mode_decimals->set_enabled(false);
}

void LineEdit::on_checked_numeric_mode_decimals(clan::CheckBox *checkbox)
{
	lineedit1->set_numeric_mode(true, true);
}

void LineEdit::on_unchecked_numeric_mode_decimals(clan::CheckBox *checkbox)
{
	lineedit1->set_numeric_mode(true, false);
}

void LineEdit::on_settext_selected(int value, clan::ComboBox *combobox)
{
	std::string str;

	switch (value)
	{
		case 1:
			lineedit1->set_alignment(clan::LineEdit::align_left);
			break;
		case 2:
			lineedit1->set_alignment(clan::LineEdit::align_center);
			break;
		case 3:
			lineedit1->set_alignment(clan::LineEdit::align_right);
			break;
		case 4:
			lineedit1->set_text("Hello World");
			break;
		case 5:
			lineedit1->set_text(1234);
			break;
		case 6:
			lineedit1->set_text(123.456f);
			break;
		case 7:
			str = lineedit1->get_text();
			lineedit1->set_selection(0, str.length());
			break;
		case 8:
			lineedit1->set_selection(2, 3);
			break;
		case 9:
			lineedit1->clear_selection();
			break;
	}
}

void LineEdit::on_checked_mask_hex(clan::CheckBox *checkbox)
{
	lineedit1->set_input_mask("1234567890acbdefABCDEF");
}

void LineEdit::on_unchecked_mask_hex(clan::CheckBox *checkbox)
{
	std::string empty_mask;
	lineedit1->set_input_mask(empty_mask);
}

void LineEdit::on_resize_clicked(clan::PushButton *pushbutton)
{
	lineedit1->resize_to_fit(200);
}

bool LineEdit::on_close()
{
	exit_with_code(0);
	return true;
}
