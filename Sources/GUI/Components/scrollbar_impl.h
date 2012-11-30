/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
#include "API/Display/Window/input_event.h"
#include "API/Core/System/timer.h"
#include "API/Display/Render/graphic_context.h"
#include "API/CSSLayout/css_property.h"
#include "API/CSSLayout/css_token.h"

namespace clan
{

class ScrollBar;

class ScrollBar_Impl
{
public:
	ScrollBar_Impl();

	void on_part_button_decrement(std::shared_ptr<GUIMessage> &msg);
	void on_part_track_decrement(std::shared_ptr<GUIMessage> &msg);
	void on_part_thumb(std::shared_ptr<GUIMessage> &msg);
	void on_part_track_increment(std::shared_ptr<GUIMessage> &msg);
	void on_part_button_increment(std::shared_ptr<GUIMessage> &msg);

	void on_enablemode_changed();
	void on_timer_expired();

	void create_parts();
	int calculate_thumb_size(int track_size);
	int calculate_thumb_position(int thumb_size, int track_size);
	Rect create_rect(const Rect content_rect, int start, int end);
	void update_part_positions();

private:
	void invoke_scroll_event(Callback_v0 *event_ptr);
	void mouse_released();
	void mouse_moved(const Point &pos);
	void on_css_property_list(CSSPropertyList &properties);
	void on_default_properties(CSSBoxProperties &properties);

public:
	int thumb_left;
	int thumb_size;

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
	Point mouse_drag_start_pos;
	int thumb_start_pixel_position;

	Timer mouse_down_timer;
	int last_step_size;

	GUIComponent *part_button_decrement;
	GUIComponent *part_button_increment;
	GUIComponent *part_track_decrement;
	GUIComponent *part_track_increment;
	GUIComponent *part_thumb;
	GUIComponent *part_thumb_gripper;

	ScrollBar *scrollbar;

	Callback_v0 func_scroll;
	Callback_v0 func_scroll_min;
	Callback_v0 func_scroll_max;
	Callback_v0 func_scroll_line_decrement;
	Callback_v0 func_scroll_line_increment;
	Callback_v0 func_scroll_page_decrement;
	Callback_v0 func_scroll_page_increment;
	Callback_v0 func_scroll_thumb_release;
	Callback_v0 func_scroll_thumb_track;
	Callback_v0 func_scroll_end;
	Callback_v0 *func_scroll_on_mouse_down;

	CSSToken token_width;
	CSSToken token_left;

};

}
