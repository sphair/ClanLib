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

class Slider : public clan::Window
{
public:
	Slider(clan::GUIManager &manager);

private:
	bool on_close();
	void on_value_changed(clan::Slider *slider);
	void on_value_decremented(clan::Slider *slider);
	void on_value_incremented(clan::Slider *slider);
	void on_slider_moved(clan::Slider *slider);
	void on_checked_disable(clan::CheckBox *checkbox);
	void on_unchecked_disable(clan::CheckBox *checkbox);
	void on_checked_lock_to_ticks(clan::CheckBox *checkbox);
	void on_unchecked_lock_to_ticks(clan::CheckBox *checkbox);

	void on_min_enter_pressed(clan::LineEdit *lineedit);
	void on_max_enter_pressed(clan::LineEdit *lineedit);
	void on_tick_count_enter_pressed(clan::LineEdit *lineedit);
	void on_page_step_enter_pressed(clan::LineEdit *lineedit);
	void on_position_enter_pressed_horiz(clan::LineEdit *lineedit);
	void on_position_enter_pressed_vert(clan::LineEdit *lineedit);
	void on_apply_clicked(clan::PushButton *pushbutton);

private:
	clan::Slider *slider_vertical;
	clan::Slider *slider_horizontal;

	clan::PushButton *pushbutton_apply;

	clan::LineEdit *lineedit_min;
	clan::LineEdit *lineedit_max;
	clan::LineEdit *lineedit_tick_count;
	clan::LineEdit *lineedit_page_step;
	clan::LineEdit *lineedit_position_horiz;
	clan::LineEdit *lineedit_position_vert;

	clan::Label *lineedit_label_min;
	clan::Label *lineedit_label_max;
	clan::Label *lineedit_label_tick_count;
	clan::Label *lineedit_label_page_step;
	clan::Label *lineedit_label_position_horiz;
	clan::Label *lineedit_label_position_vert;

	clan::CheckBox *checkbox_disable;
	clan::CheckBox *checkbox_lock_to_ticks;

	Info *info_value_changed;
	Info *info_value_decremented;
	Info *info_value_incremented;
	Info *info_slider_moved;
};
