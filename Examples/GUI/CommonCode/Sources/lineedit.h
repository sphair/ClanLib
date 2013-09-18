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

#pragma once

#include "info.h"


class LineEdit : public clan::Window
{
public:
	LineEdit(clan::GUIManager &manager);

private:
	bool on_close();
	void on_before_edit_changed(clan::InputEvent &input_event, clan::LineEdit *lineedit);
	void on_after_edit_changed(clan::InputEvent &input_event, clan::LineEdit *lineedit);
	void on_selection_changed(clan::LineEdit *lineedit);
	void on_focus_gained(clan::LineEdit *lineedit);
	void on_focus_lost(clan::LineEdit *lineedit);
	void on_enter_pressed(clan::LineEdit *lineedit);
	void on_checked_disable(clan::CheckBox *checkbox);
	void on_unchecked_disable(clan::CheckBox *checkbox);

	void on_checked_mask_hex(clan::CheckBox *checkbox);
	void on_unchecked_mask_hex(clan::CheckBox *checkbox);
	void on_checked_read_only(clan::CheckBox *checkbox);
	void on_unchecked_read_only(clan::CheckBox *checkbox);
	void on_checked_lowercase(clan::CheckBox *checkbox);
	void on_unchecked_lowercase(clan::CheckBox *checkbox);
	void on_checked_uppercase(clan::CheckBox *checkbox);
	void on_unchecked_uppercase(clan::CheckBox *checkbox);
	void on_checked_password_mode(clan::CheckBox *checkbox);
	void on_unchecked_password_mode(clan::CheckBox *checkbox);
	void on_checked_numeric_mode(clan::CheckBox *checkbox);
	void on_unchecked_numeric_mode(clan::CheckBox *checkbox);
	void on_checked_numeric_mode_decimals(clan::CheckBox *checkbox);
	void on_unchecked_numeric_mode_decimals(clan::CheckBox *checkbox);
	void on_settext_selected(int value, clan::ComboBox *combobox);
	void on_resize_clicked(clan::PushButton *pushbutton);

private:
	clan::LineEdit *lineedit1;

	clan::ComboBox *combo_settext;
	clan::PopupMenu menu_settext;

	clan::PushButton *pushbutton_resize;

	clan::CheckBox *checkbox_disable;
	clan::CheckBox *checkbox_mask_hex;
	clan::CheckBox *checkbox_read_only;
	clan::CheckBox *checkbox_lowercase;
	clan::CheckBox *checkbox_uppercase;
	clan::CheckBox *checkbox_password_mode;
	clan::CheckBox *checkbox_numeric_mode;
	clan::CheckBox *checkbox_numeric_mode_decimals;

	Info *info_before_edit_changed;
	Info *info_after_edit_changed;
	Info *info_selection_changed;
	Info *info_focus_gained;
	Info *info_focus_lost;
	Info *info_enter_pressed;
};
