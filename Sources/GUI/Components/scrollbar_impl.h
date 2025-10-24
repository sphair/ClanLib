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
**    Harry Storbacka
**    Magnus Norddahl
**    Kenneth Gangstoe
*/

#pragma once

#include "API/GUI/gui_message.h"
#include "API/GUI/gui_message_input.h"
#include "API/GUI/gui_theme_part.h"
#include "API/Display/Window/input_event.h"
#include "API/Core/System/timer.h"
#include "API/Display/Render/graphic_context.h"

class CL_ScrollBar;

class CL_ScrollBar_Impl
{
public:
	CL_ScrollBar_Impl();

	void on_process_message(CL_GUIMessage &msg);
	void on_mouse_move(CL_GUIMessage_Input &input, CL_InputEvent &input_event);
	void on_mouse_lbutton_down(CL_GUIMessage_Input &input, CL_InputEvent &input_event);
	void on_mouse_lbutton_up(CL_GUIMessage_Input &input, CL_InputEvent &input_event);
	void on_mouse_leave();
	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	void on_enablemode_changed();
	void on_timer_expired();
	void on_resized();

	void create_parts();
	bool update_part_positions();
	int calculate_thumb_size(int track_size);
	int calculate_thumb_position(int thumb_size, int track_size);
	CL_Rect create_rect(const CL_Rect content_rect, int start, int end);

private:
	void invoke_scroll_event(CL_Callback_v0 *event_ptr);

public:
	bool vertical;
	int scroll_min, scroll_max, line_step, page_step, position;

	enum MouseDownMode
	{
		mouse_down_none,
		mouse_down_button_decr,
		mouse_down_button_incr,
		mouse_down_track_decr,
		mouse_down_track_incr,
		mouse_down_thumb_drag
	} mouse_down_mode;

	int thumb_start_position;
	CL_Point mouse_drag_start_pos;
	int thumb_start_pixel_position;

	CL_Timer mouse_down_timer;
	int last_step_size;

	CL_Rect rect_button_decrement;
	CL_Rect rect_track_decrement;
	CL_Rect rect_thumb;
	CL_Rect rect_track_increment;
	CL_Rect rect_button_increment;

	CL_GUIThemePart part_component;
	CL_GUIThemePart part_button_decrement;
	CL_GUIThemePart part_button_increment;
	CL_GUIThemePart part_track_decrement;
	CL_GUIThemePart part_track_increment;
	CL_GUIThemePart part_thumb;
	CL_GUIThemePart part_thumb_gripper;

	CL_ScrollBar *scrollbar;

	CL_Callback_v0 func_scroll;
	CL_Callback_v0 func_scroll_min;
	CL_Callback_v0 func_scroll_max;
	CL_Callback_v0 func_scroll_line_decrement;
	CL_Callback_v0 func_scroll_line_increment;
	CL_Callback_v0 func_scroll_page_decrement;
	CL_Callback_v0 func_scroll_page_increment;
	CL_Callback_v0 func_scroll_thumb_release;
	CL_Callback_v0 func_scroll_thumb_track;
	CL_Callback_v0 func_scroll_end;
	CL_Callback_v0 *func_scroll_on_mouse_down;

};
