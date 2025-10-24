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

class Spin : public CL_Window
{
public:
	Spin(CL_GUIManager &manager);

private:
	void on_value_changed(CL_Spin *spin);
	void on_checked_disable(CL_CheckBox *checkbox);
	void on_unchecked_disable(CL_CheckBox *checkbox);
	void on_checked_fp_mode(CL_CheckBox *checkbox);
	void on_unchecked_fp_mode(CL_CheckBox *checkbox);
	void on_apply_clicked(CL_PushButton *pushbutton);
	void on_value_enter_pressed(CL_LineEdit *lineedit);
	void on_min_enter_pressed(CL_LineEdit *lineedit);
	void on_max_enter_pressed(CL_LineEdit *lineedit);
	void on_step_enter_pressed(CL_LineEdit *lineedit);
	void on_decimal_places_enter_pressed(CL_LineEdit *lineedit);

private:
	CL_Spin *spin1;
	CL_Spin *spin2;

	CL_PushButton *pushbutton_apply;

	CL_LineEdit *lineedit_value;
	CL_LineEdit *lineedit_min;
	CL_LineEdit *lineedit_max;
	CL_LineEdit *lineedit_step;
	CL_LineEdit *lineedit_decimal_places;

	CL_Label *lineedit_label_value;
	CL_Label *lineedit_label_min;
	CL_Label *lineedit_label_max;
	CL_Label *lineedit_label_step;
	CL_Label *lineedit_label_decimal_places;

	CL_CheckBox *checkbox_disable;
	CL_CheckBox *checkbox_fp_mode;

	Info *info_value_changed;
};
