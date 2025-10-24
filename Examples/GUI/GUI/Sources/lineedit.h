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

#pragma once

#include "info.h"

class GUI;

class LineEdit : public CL_Window
{
public:
	LineEdit(GUI *gui);

private:
	void on_before_edit_changed(CL_InputEvent &input_event, CL_LineEdit *lineedit);
	void on_after_edit_changed(CL_InputEvent &input_event, CL_LineEdit *lineedit);
	void on_selection_changed(CL_LineEdit *lineedit);
	void on_focus_gained(CL_LineEdit *lineedit);
	void on_focus_lost(CL_LineEdit *lineedit);
	void on_enter_pressed(CL_LineEdit *lineedit);
	void on_checked_disable(CL_CheckBox *checkbox);
	void on_unchecked_disable(CL_CheckBox *checkbox);

	void on_checked_mask_hex(CL_CheckBox *checkbox);
	void on_unchecked_mask_hex(CL_CheckBox *checkbox);
	void on_checked_read_only(CL_CheckBox *checkbox);
	void on_unchecked_read_only(CL_CheckBox *checkbox);
	void on_checked_lowercase(CL_CheckBox *checkbox);
	void on_unchecked_lowercase(CL_CheckBox *checkbox);
	void on_checked_uppercase(CL_CheckBox *checkbox);
	void on_unchecked_uppercase(CL_CheckBox *checkbox);
	void on_checked_password_mode(CL_CheckBox *checkbox);
	void on_unchecked_password_mode(CL_CheckBox *checkbox);
	void on_checked_numeric_mode(CL_CheckBox *checkbox);
	void on_unchecked_numeric_mode(CL_CheckBox *checkbox);
	void on_checked_numeric_mode_decimals(CL_CheckBox *checkbox);
	void on_unchecked_numeric_mode_decimals(CL_CheckBox *checkbox);
	void on_settext_selected(int value, CL_ComboBox *combobox);
	void on_resize_clicked(CL_PushButton *pushbutton);

private:
	GUI *gui;

	CL_LineEdit *lineedit1;

	CL_ComboBox *combo_settext;
	CL_PopupMenu menu_settext;

	CL_PushButton *pushbutton_resize;

	CL_CheckBox *checkbox_disable;
	CL_CheckBox *checkbox_mask_hex;
	CL_CheckBox *checkbox_read_only;
	CL_CheckBox *checkbox_lowercase;
	CL_CheckBox *checkbox_uppercase;
	CL_CheckBox *checkbox_password_mode;
	CL_CheckBox *checkbox_numeric_mode;
	CL_CheckBox *checkbox_numeric_mode_decimals;

	Info *info_before_edit_changed;
	Info *info_after_edit_changed;
	Info *info_selection_changed;
	Info *info_focus_gained;
	Info *info_focus_lost;
	Info *info_enter_pressed;
};
