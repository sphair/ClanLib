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

#include "precomp.h"
#include "scrollbar.h"
#include "GUI.h"

ScrollBar::ScrollBar(GUI *gui) : 
	CL_Window(&gui->get_gui_manager(), CL_GUITopLevelDescription("Scrollbar", CL_Rect(8, 8, CL_Size(256, 256)), false)),
	gui(gui)
{
	set_draggable(true);

	CL_Rect client_area = get_client_area();

	scrollbar_vertical = new CL_ScrollBar(this);
	scrollbar_vertical->set_geometry(CL_Rect(client_area.left + 1, client_area.top + 10, CL_Size(17, 200)));
	scrollbar_vertical->set_vertical();
	scrollbar_vertical->set_min(0);
	scrollbar_vertical->set_max(100);
	scrollbar_vertical->set_line_step(1);
	scrollbar_vertical->set_page_step(40);
	scrollbar_vertical->set_position(50);
	scrollbar_vertical->func_scroll().set(this, &ScrollBar::on_scroll, scrollbar_vertical);
	scrollbar_vertical->func_scroll_min().set(this, &ScrollBar::on_scroll_min, scrollbar_vertical);
	scrollbar_vertical->func_scroll_max().set(this, &ScrollBar::on_scroll_max, scrollbar_vertical);
	scrollbar_vertical->func_scroll_line_decrement().set(this, &ScrollBar::on_scroll_line_decrement, scrollbar_vertical);
	scrollbar_vertical->func_scroll_line_increment().set(this, &ScrollBar::on_scroll_line_increment, scrollbar_vertical);
	scrollbar_vertical->func_scroll_page_decrement().set(this, &ScrollBar::on_scroll_page_decrement, scrollbar_vertical);
	scrollbar_vertical->func_scroll_page_increment().set(this, &ScrollBar::on_scroll_page_increment, scrollbar_vertical);
	scrollbar_vertical->func_scroll_thumb_release().set(this, &ScrollBar::on_scroll_thumb_release, scrollbar_vertical);
	scrollbar_vertical->func_scroll_thumb_track().set(this, &ScrollBar::on_scroll_thumb_track, scrollbar_vertical);

	scrollbar_horizontal = new CL_ScrollBar(this);
	scrollbar_horizontal->set_geometry(CL_Rect(client_area.left + 26, client_area.top + 10, CL_Size(200, 17)));
	scrollbar_horizontal->set_horizontal();
	scrollbar_horizontal->set_min(0);
	scrollbar_horizontal->set_max(100);
	scrollbar_horizontal->set_line_step(1);
	scrollbar_horizontal->set_page_step(40);
	scrollbar_horizontal->set_position(50);
	scrollbar_horizontal->func_scroll().set(this, &ScrollBar::on_scroll, scrollbar_horizontal);
	scrollbar_horizontal->func_scroll_min().set(this, &ScrollBar::on_scroll_min, scrollbar_horizontal);
	scrollbar_horizontal->func_scroll_max().set(this, &ScrollBar::on_scroll_max, scrollbar_horizontal);
	scrollbar_horizontal->func_scroll_line_decrement().set(this, &ScrollBar::on_scroll_line_decrement, scrollbar_horizontal);
	scrollbar_horizontal->func_scroll_line_increment().set(this, &ScrollBar::on_scroll_line_increment, scrollbar_horizontal);
	scrollbar_horizontal->func_scroll_page_decrement().set(this, &ScrollBar::on_scroll_page_decrement, scrollbar_horizontal);
	scrollbar_horizontal->func_scroll_page_increment().set(this, &ScrollBar::on_scroll_page_increment, scrollbar_horizontal);
	scrollbar_horizontal->func_scroll_thumb_release().set(this, &ScrollBar::on_scroll_thumb_release, scrollbar_horizontal);
	scrollbar_horizontal->func_scroll_thumb_track().set(this, &ScrollBar::on_scroll_thumb_track, scrollbar_horizontal);

	CL_Size lineedit_size(42, 20);
	CL_Size label_size(50, 15);
	int lineedit_xpos = client_area.left + 146;
	int label_xpos = client_area.left + 190;

	int lineedit_ypos = client_area.top + 40;
	const int lineedit_gap = 25;

	lineedit_min = new CL_LineEdit(this);
	lineedit_min->set_geometry(CL_Rect(lineedit_xpos, lineedit_ypos, lineedit_size));
	lineedit_min->set_text("0");
	lineedit_min->set_numeric_mode(true);
	lineedit_min->func_enter_pressed().set(this, &ScrollBar::on_min_enter_pressed, lineedit_min);

	lineedit_label_min = new CL_Label(this);
	lineedit_label_min->set_geometry(CL_Rect(label_xpos, lineedit_ypos, label_size));
	lineedit_label_min->set_text("Min");
	lineedit_ypos += lineedit_gap;

	lineedit_max = new CL_LineEdit(this);
	lineedit_max->set_geometry(CL_Rect(lineedit_xpos, lineedit_ypos, lineedit_size));
	lineedit_max->set_text("100");
	lineedit_max->set_numeric_mode(true);
	lineedit_max->func_enter_pressed().set(this, &ScrollBar::on_max_enter_pressed, lineedit_max);

	lineedit_label_max = new CL_Label(this);
	lineedit_label_max->set_geometry(CL_Rect(label_xpos, lineedit_ypos, label_size));
	lineedit_label_max->set_text("Max");
	lineedit_ypos += lineedit_gap;

	lineedit_line_step = new CL_LineEdit(this);
	lineedit_line_step->set_geometry(CL_Rect(lineedit_xpos, lineedit_ypos, lineedit_size));
	lineedit_line_step->set_text("1");
	lineedit_line_step->func_enter_pressed().set(this, &ScrollBar::on_line_step_enter_pressed, lineedit_line_step);

	lineedit_label_line_step = new CL_Label(this);
	lineedit_label_line_step->set_geometry(CL_Rect(label_xpos, lineedit_ypos, label_size));
	lineedit_label_line_step->set_text("Line Step");
	lineedit_ypos += lineedit_gap;

	lineedit_page_step = new CL_LineEdit(this);
	lineedit_page_step->set_geometry(CL_Rect(lineedit_xpos, lineedit_ypos, lineedit_size));
	lineedit_page_step->set_text("40");
	lineedit_page_step->func_enter_pressed().set(this, &ScrollBar::on_page_step_enter_pressed, lineedit_page_step);

	lineedit_label_page_step = new CL_Label(this);
	lineedit_label_page_step->set_geometry(CL_Rect(label_xpos, lineedit_ypos, label_size));
	lineedit_label_page_step->set_text("Page Step");
	lineedit_ypos += lineedit_gap;

	lineedit_position_vert = new CL_LineEdit(this);
	lineedit_position_vert->set_geometry(CL_Rect(lineedit_xpos, lineedit_ypos, lineedit_size));
	lineedit_position_vert->set_text("50");
	lineedit_position_vert->func_enter_pressed().set(this, &ScrollBar::on_position_enter_pressed_vert, lineedit_position_vert);

	lineedit_label_position_vert = new CL_Label(this);
	lineedit_label_position_vert->set_geometry(CL_Rect(label_xpos, lineedit_ypos, label_size));
	lineedit_label_position_vert->set_text("Vertical");
	lineedit_ypos += lineedit_gap;

	lineedit_position_horiz = new CL_LineEdit(this);
	lineedit_position_horiz->set_geometry(CL_Rect(lineedit_xpos, lineedit_ypos, lineedit_size));
	lineedit_position_horiz->set_text("50");
	lineedit_position_horiz->func_enter_pressed().set(this, &ScrollBar::on_position_enter_pressed_horiz, lineedit_position_horiz);

	lineedit_label_position_horiz = new CL_Label(this);
	lineedit_label_position_horiz->set_geometry(CL_Rect(label_xpos, lineedit_ypos, label_size));
	lineedit_label_position_horiz->set_text("Horizonal");
	lineedit_ypos += lineedit_gap;

	pushbutton_apply = new CL_PushButton(this);
	pushbutton_apply->set_geometry(CL_Rect(lineedit_xpos, lineedit_ypos, CL_Size(48, 20)));
	pushbutton_apply->set_text("Apply");
	pushbutton_apply->func_clicked().set(this, &ScrollBar::on_apply_clicked, pushbutton_apply);

	int xoffset = client_area.left + 21;
	int yoffset = client_area.top + 35;
	const int gap = 16;

	info_scroll = new Info(gui, this);
	info_scroll->set(xoffset, yoffset, "Scroll");
	yoffset += gap;
	info_scroll_min = new Info(gui, this);
	info_scroll_min->set(xoffset, yoffset, "Scroll Min");
	yoffset += gap;
	info_scroll_max = new Info(gui, this);
	info_scroll_max->set(xoffset, yoffset, "Scroll Max");
	yoffset += gap;
	info_scroll_line_decrement = new Info(gui, this);
	info_scroll_line_decrement->set(xoffset, yoffset, "Line Decrement");
	yoffset += gap;
	info_scroll_line_increment = new Info(gui, this);
	info_scroll_line_increment->set(xoffset, yoffset, "Line Increment");
	yoffset += gap;
	info_scroll_page_decrement = new Info(gui, this);
	info_scroll_page_decrement->set(xoffset, yoffset, "Page Decrement");
	yoffset += gap;
	info_scroll_page_increment = new Info(gui, this);
	info_scroll_page_increment->set(xoffset, yoffset, "Page Increment");
	yoffset += gap;
	info_scroll_thumb_release = new Info(gui, this);
	info_scroll_thumb_release->set(xoffset, yoffset, "Thumb Release");
	yoffset += gap;
	info_scroll_thumb_track = new Info(gui, this);
	info_scroll_thumb_track->set(xoffset, yoffset, "Thumb Track");
	yoffset += gap;

	xoffset = client_area.left + 36;
	yoffset = client_area.top + 195;

	checkbox_disable = new CL_CheckBox(this);
	checkbox_disable->set_geometry(CL_Rect(xoffset, yoffset, CL_Size(100, 15)));
	checkbox_disable->func_checked().set(this, &ScrollBar::on_checked_disable, checkbox_disable);
	checkbox_disable->func_unchecked().set(this, &ScrollBar::on_unchecked_disable, checkbox_disable);
	checkbox_disable->set_text("Disable");
}

void ScrollBar::on_scroll(CL_ScrollBar *scrollbar)
{
	info_scroll->activate();

	int position = scrollbar->get_position();

	if (scrollbar == scrollbar_vertical)
	{
		lineedit_position_vert->set_text(position);
	}
	else
	{
		lineedit_position_horiz->set_text(position);
	}
}

void ScrollBar::on_scroll_min(CL_ScrollBar *scrollbar)
{
	info_scroll_min->activate();
}

void ScrollBar::on_scroll_max(CL_ScrollBar *scrollbar)
{
	info_scroll_max->activate();
}

void ScrollBar::on_scroll_line_decrement(CL_ScrollBar *scrollbar)
{
	info_scroll_line_decrement->activate();
}

void ScrollBar::on_scroll_line_increment(CL_ScrollBar *scrollbar)
{
	info_scroll_line_increment->activate();
}

void ScrollBar::on_scroll_page_decrement(CL_ScrollBar *scrollbar)
{
	info_scroll_page_decrement->activate();
}

void ScrollBar::on_scroll_page_increment(CL_ScrollBar *scrollbar)
{
	info_scroll_page_increment->activate();
}

void ScrollBar::on_scroll_thumb_release(CL_ScrollBar *scrollbar)
{
	info_scroll_thumb_release->activate();
}

void ScrollBar::on_scroll_thumb_track(CL_ScrollBar *scrollbar)
{
	info_scroll_thumb_track->activate();
}

void ScrollBar::on_min_enter_pressed(CL_LineEdit *lineedit)
{
	int value = CL_StringHelp::text_to_int(lineedit->get_text());
	scrollbar_vertical->set_min(value);
	scrollbar_horizontal->set_min(value);
}

void ScrollBar::on_max_enter_pressed(CL_LineEdit *lineedit)
{
	int value = CL_StringHelp::text_to_int(lineedit->get_text());
	scrollbar_vertical->set_max(value);
	scrollbar_horizontal->set_max(value);
}

void ScrollBar::on_line_step_enter_pressed(CL_LineEdit *lineedit)
{
	int value = CL_StringHelp::text_to_int(lineedit->get_text());
	scrollbar_vertical->set_line_step(value);
	scrollbar_horizontal->set_line_step(value);
}

void ScrollBar::on_page_step_enter_pressed(CL_LineEdit *lineedit)
{
	int value = CL_StringHelp::text_to_int(lineedit->get_text());
	scrollbar_vertical->set_page_step(value);
	scrollbar_horizontal->set_page_step(value);
}

void ScrollBar::on_position_enter_pressed_horiz(CL_LineEdit *lineedit)
{
	int value = CL_StringHelp::text_to_int(lineedit->get_text());
	scrollbar_horizontal->set_position(value);
}

void ScrollBar::on_position_enter_pressed_vert(CL_LineEdit *lineedit)
{
	int value = CL_StringHelp::text_to_int(lineedit->get_text());
	scrollbar_vertical->set_position(value);
}

void ScrollBar::on_checked_disable(CL_CheckBox *checkbox)
{
	scrollbar_vertical->set_enabled(false);
	scrollbar_horizontal->set_enabled(false);
}

void ScrollBar::on_unchecked_disable(CL_CheckBox *checkbox)
{
	scrollbar_vertical->set_enabled(true);
	scrollbar_horizontal->set_enabled(true);
}

void ScrollBar::on_apply_clicked(CL_PushButton *pushbutton)
{
	int value = CL_StringHelp::text_to_int(lineedit_position_vert->get_text());
	scrollbar_vertical->set_position(value);

	value = CL_StringHelp::text_to_int(lineedit_position_horiz->get_text());
	scrollbar_horizontal->set_position(value);

	value = CL_StringHelp::text_to_int(lineedit_page_step->get_text());
	scrollbar_vertical->set_page_step(value);
	scrollbar_horizontal->set_page_step(value);

	value = CL_StringHelp::text_to_int(lineedit_line_step->get_text());
	scrollbar_vertical->set_line_step(value);
	scrollbar_horizontal->set_line_step(value);

	value = CL_StringHelp::text_to_int(lineedit_max->get_text());
	scrollbar_vertical->set_max(value);
	scrollbar_horizontal->set_max(value);

	value = CL_StringHelp::text_to_int(lineedit_min->get_text());
	scrollbar_vertical->set_min(value);
	scrollbar_horizontal->set_min(value);
}
