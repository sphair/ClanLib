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

class ScrollBar : public CL_Window
{
public:
	ScrollBar(CL_GUIManager &manager);

private:
	void on_scroll(CL_ScrollBar *scrollbar);
	void on_scroll_min(CL_ScrollBar *scrollbar);
	void on_scroll_max(CL_ScrollBar *scrollbar);
	void on_scroll_line_increment(CL_ScrollBar *scrollbar);
	void on_scroll_line_decrement(CL_ScrollBar *scrollbar);
	void on_scroll_page_increment(CL_ScrollBar *scrollbar);
	void on_scroll_page_decrement(CL_ScrollBar *scrollbar);
	void on_scroll_thumb_release(CL_ScrollBar *scrollbar);
	void on_scroll_thumb_track(CL_ScrollBar *scrollbar);
	void on_min_enter_pressed(CL_LineEdit *lineedit);
	void on_max_enter_pressed(CL_LineEdit *lineedit);
	void on_line_step_enter_pressed(CL_LineEdit *lineedit);
	void on_page_step_enter_pressed(CL_LineEdit *lineedit);
	void on_position_enter_pressed_horiz(CL_LineEdit *lineedit);
	void on_position_enter_pressed_vert(CL_LineEdit *lineedit);
	void on_checked_disable(CL_CheckBox *checkbox);
	void on_unchecked_disable(CL_CheckBox *checkbox);
	void on_apply_clicked(CL_PushButton *pushbutton);

private:
	CL_ScrollBar *scrollbar_vertical;
	CL_ScrollBar *scrollbar_horizontal;
	CL_PushButton *pushbutton_apply;

	CL_LineEdit *lineedit_min;
	CL_LineEdit *lineedit_max;
	CL_LineEdit *lineedit_line_step;
	CL_LineEdit *lineedit_page_step;
	CL_LineEdit *lineedit_position_horiz;
	CL_LineEdit *lineedit_position_vert;

	CL_Label *lineedit_label_min;
	CL_Label *lineedit_label_max;
	CL_Label *lineedit_label_line_step;
	CL_Label *lineedit_label_page_step;
	CL_Label *lineedit_label_position_horiz;
	CL_Label *lineedit_label_position_vert;

	CL_CheckBox *checkbox_disable;

	Info *info_scroll;
	Info *info_scroll_min;
	Info *info_scroll_max;
	Info *info_scroll_line_decrement;
	Info *info_scroll_line_increment;
	Info *info_scroll_page_decrement;
	Info *info_scroll_page_increment;
	Info *info_scroll_thumb_release;
	Info *info_scroll_thumb_track;
};
