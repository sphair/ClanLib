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

class ProgressBar : public CL_Window
{
public:
	ProgressBar(GUI *gui);

private:
	void on_checked_marquee_mode(CL_CheckBox *checkbox);
	void on_unchecked_marquee_mode(CL_CheckBox *checkbox);

	void on_min_enter_pressed(CL_LineEdit *lineedit);
	void on_max_enter_pressed(CL_LineEdit *lineedit);
	void on_step_size_enter_pressed(CL_LineEdit *lineedit);
	void on_position_enter_pressed(CL_LineEdit *lineedit);
	void on_marquee_speed_enter_pressed(CL_LineEdit *lineedit);
	void on_apply_clicked(CL_PushButton *pushbutton);
	void on_step_position_clicked(CL_PushButton *pushbutton);

private:
	GUI *gui;

	CL_ProgressBar *progressbar1;

	CL_PushButton *pushbutton_apply;
	CL_PushButton *pushbutton_step_position;

	CL_LineEdit *lineedit_min;
	CL_LineEdit *lineedit_max;
	CL_LineEdit *lineedit_step_size;
	CL_LineEdit *lineedit_position;
	CL_LineEdit *lineedit_marquee_speed;

	CL_Label *lineedit_label_min;
	CL_Label *lineedit_label_max;
	CL_Label *lineedit_label_step_size;
	CL_Label *lineedit_label_position;
	CL_Label *lineedit_label_marquee_speed;

	CL_CheckBox *checkbox_marquee_mode;

};
