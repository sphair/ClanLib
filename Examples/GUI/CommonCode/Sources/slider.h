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

class Slider : public CL_Window
{
public:
	Slider(CL_GUIManager &manager);

private:
	void on_value_changed(CL_Slider *slider);
	void on_value_decremented(CL_Slider *slider);
	void on_value_incremented(CL_Slider *slider);
	void on_slider_moved(CL_Slider *slider);
	void on_checked_disable(CL_CheckBox *checkbox);
	void on_unchecked_disable(CL_CheckBox *checkbox);
	void on_checked_lock_to_ticks(CL_CheckBox *checkbox);
	void on_unchecked_lock_to_ticks(CL_CheckBox *checkbox);

	void on_min_enter_pressed(CL_LineEdit *lineedit);
	void on_max_enter_pressed(CL_LineEdit *lineedit);
	void on_tick_count_enter_pressed(CL_LineEdit *lineedit);
	void on_page_step_enter_pressed(CL_LineEdit *lineedit);
	void on_position_enter_pressed_horiz(CL_LineEdit *lineedit);
	void on_position_enter_pressed_vert(CL_LineEdit *lineedit);
	void on_apply_clicked(CL_PushButton *pushbutton);

private:
	CL_Slider *slider_vertical;
	CL_Slider *slider_horizontal;

	CL_PushButton *pushbutton_apply;

	CL_LineEdit *lineedit_min;
	CL_LineEdit *lineedit_max;
	CL_LineEdit *lineedit_tick_count;
	CL_LineEdit *lineedit_page_step;
	CL_LineEdit *lineedit_position_horiz;
	CL_LineEdit *lineedit_position_vert;

	CL_Label *lineedit_label_min;
	CL_Label *lineedit_label_max;
	CL_Label *lineedit_label_tick_count;
	CL_Label *lineedit_label_page_step;
	CL_Label *lineedit_label_position_horiz;
	CL_Label *lineedit_label_position_vert;

	CL_CheckBox *checkbox_disable;
	CL_CheckBox *checkbox_lock_to_ticks;

	Info *info_value_changed;
	Info *info_value_decremented;
	Info *info_value_incremented;
	Info *info_slider_moved;
};
