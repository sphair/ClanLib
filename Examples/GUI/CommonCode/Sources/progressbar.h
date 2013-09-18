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

class ProgressBar : public clan::Window
{
public:
	ProgressBar(clan::GUIManager &manager);

private:
	bool on_close();
	void on_checked_marquee_mode(clan::CheckBox *checkbox);
	void on_unchecked_marquee_mode(clan::CheckBox *checkbox);

	void on_min_enter_pressed(clan::LineEdit *lineedit);
	void on_max_enter_pressed(clan::LineEdit *lineedit);
	void on_step_size_enter_pressed(clan::LineEdit *lineedit);
	void on_position_enter_pressed(clan::LineEdit *lineedit);
	void on_marquee_speed_enter_pressed(clan::LineEdit *lineedit);
	void on_apply_clicked(clan::PushButton *pushbutton);
	void on_step_position_clicked(clan::PushButton *pushbutton);

private:
	clan::ProgressBar *progressbar1;

	clan::PushButton *pushbutton_apply;
	clan::PushButton *pushbutton_step_position;

	clan::LineEdit *lineedit_min;
	clan::LineEdit *lineedit_max;
	clan::LineEdit *lineedit_step_size;
	clan::LineEdit *lineedit_position;
	clan::LineEdit *lineedit_marquee_speed;

	clan::Label *lineedit_label_min;
	clan::Label *lineedit_label_max;
	clan::Label *lineedit_label_step_size;
	clan::Label *lineedit_label_position;
	clan::Label *lineedit_label_marquee_speed;

	clan::CheckBox *checkbox_marquee_mode;

};
