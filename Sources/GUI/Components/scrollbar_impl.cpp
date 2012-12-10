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

#include "GUI/precomp.h"
#include "API/Core/Math/cl_math.h"
#include "API/Display/Window/keys.h"
#include "API/GUI/gui_message_pointer.h"
#include "API/GUI/Components/scrollbar.h"
#include "scrollbar_impl.h"
#include "../gui_css_strings.h"

namespace clan
{

ScrollBar_Impl::ScrollBar_Impl()
: vertical(false), scroll_min(0), scroll_max(1), line_step(1),
  page_step(10), position(0), mouse_down_mode(mouse_down_none),
  thumb_start_position(0), thumb_start_pixel_position(0),
  func_scroll_on_mouse_down(0)
{
}

void ScrollBar_Impl::on_process_message(std::shared_ptr<GUIMessage> &msg)
{
	if (!scrollbar->is_enabled())
		return;

	std::shared_ptr<GUIMessage_Input> input_msg = std::dynamic_pointer_cast<GUIMessage_Input>(msg);
	if (input_msg)
	{
		
		InputEvent &input_event = input_msg->input_event;
		if (input_event.type == InputEvent::pointer_moved)
			on_mouse_move(msg, input_event);
		else if (input_event.type == InputEvent::pressed && input_event.id == mouse_left)
			on_mouse_lbutton_down(msg, input_event);
		else if (input_event.type == InputEvent::released && input_event.id == mouse_left)
			on_mouse_lbutton_up(msg, input_event);
	}
	std::shared_ptr<GUIMessage_Pointer> pointer = std::dynamic_pointer_cast<GUIMessage_Pointer>(msg);
	if (pointer)
	{
		
		if (pointer->pointer_type == GUIMessage_Pointer::pointer_leave)
			on_mouse_leave();
	}
}

void ScrollBar_Impl::on_mouse_move(std::shared_ptr<GUIMessage> &msg, InputEvent &input_event)
{
	Point pos = input_event.mouse_pos;

	bool should_invalidate = false;
	should_invalidate |= scrollbar->set_pseudo_class(CssStr::hot, true);
	should_invalidate |= part_button_decrement.set_pseudo_class(CssStr::hot, rect_button_decrement.contains(pos));
	should_invalidate |= part_button_increment.set_pseudo_class(CssStr::hot, rect_button_increment.contains(pos));
	should_invalidate |= part_track_decrement.set_pseudo_class(CssStr::hot, rect_track_decrement.contains(pos));
	should_invalidate |= part_track_increment.set_pseudo_class(CssStr::hot, rect_track_increment.contains(pos));
	should_invalidate |= part_thumb.set_pseudo_class(CssStr::hot, rect_thumb.contains(pos));
	should_invalidate |= part_thumb_gripper.set_pseudo_class(CssStr::hot, rect_thumb.contains(pos));

	if (mouse_down_mode == mouse_down_thumb_drag)
	{
		Rect geom = scrollbar->get_window_geometry();

		int last_position = position;

		if (pos.x < -100 || pos.x > geom.get_width() + 100 || pos.y < -100 || pos.y > geom.get_height() + 100)
		{
			position = thumb_start_position;
		}
		else
		{
			int delta = vertical ? (pos.y - mouse_drag_start_pos.y) : (pos.x - mouse_drag_start_pos.x);
			int position_pixels = thumb_start_pixel_position + delta;

			int track_height = 0;
			if (vertical)
				track_height = rect_track_decrement.get_height()+rect_track_increment.get_height();
			else
				track_height = rect_track_decrement.get_width()+rect_track_increment.get_width();

			if (track_height != 0)
				position = scroll_min + position_pixels*(scroll_max-scroll_min)/track_height;
			else
				position = 0;

			if (position >= scroll_max)
				position = scroll_max-1;
			if (position < scroll_min)
				position = scroll_min;

		}

		if (position != last_position)
		{
			invoke_scroll_event(&func_scroll_thumb_track);

			if(update_part_positions())
				should_invalidate = true;
		}
	}

	if(should_invalidate)
		scrollbar->request_repaint();

	msg->consumed = true;
}

void ScrollBar_Impl::on_mouse_lbutton_down(std::shared_ptr<GUIMessage> &msg, InputEvent &input_event)
{
	Point pos = input_event.mouse_pos;
	mouse_drag_start_pos = pos;

	part_button_decrement.set_pseudo_class(CssStr::pressed, false);
	part_button_increment.set_pseudo_class(CssStr::pressed, false);
	part_track_decrement.set_pseudo_class(CssStr::pressed, false);
	part_track_increment.set_pseudo_class(CssStr::pressed, false);
	part_thumb.set_pseudo_class(CssStr::pressed, false);
	part_thumb_gripper.set_pseudo_class(CssStr::pressed, false);

	if (rect_button_decrement.contains(pos))
	{
		mouse_down_mode = mouse_down_button_decr;
		func_scroll_on_mouse_down = &func_scroll_line_decrement;
		part_button_decrement.set_pseudo_class(CssStr::pressed, true);

		int last_position = position;

		position -= line_step;
		last_step_size = -line_step;
		if (position >= scroll_max)
			position = scroll_max-1;
		if (position < scroll_min)
			position = scroll_min;

		if (last_position != position)
			invoke_scroll_event(&func_scroll_line_decrement);
	}
	else if (rect_button_increment.contains(pos))
	{
		mouse_down_mode = mouse_down_button_incr;
		part_button_increment.set_pseudo_class(CssStr::pressed, true);
		func_scroll_on_mouse_down = &func_scroll_line_increment;

		int last_position = position;

		position += line_step;
		last_step_size = line_step;
		if (position >= scroll_max)
			position = scroll_max-1;
		if (position < scroll_min)
			position = scroll_min;

		if (last_position != position)
			invoke_scroll_event(&func_scroll_line_increment);
	}
	else if (rect_thumb.contains(pos))
	{
		mouse_down_mode = mouse_down_thumb_drag;
		thumb_start_position = position;
		thumb_start_pixel_position = vertical ? (rect_thumb.top-rect_track_decrement.top) : (rect_thumb.left-rect_track_decrement.left);
		part_thumb.set_pseudo_class(CssStr::pressed, true);
		part_thumb_gripper.set_pseudo_class(CssStr::pressed, true);
	}
	else if (rect_track_decrement.contains(pos))
	{
		mouse_down_mode = mouse_down_track_decr;
		func_scroll_on_mouse_down = &func_scroll_page_decrement;
		part_track_decrement.set_pseudo_class(CssStr::pressed, true);

		int last_position = position;

		position -= page_step;
		last_step_size = -page_step;
		if (position >= scroll_max)
			position = scroll_max-1;
		if (position < scroll_min)
			position = scroll_min;

		if (last_position != position)
			invoke_scroll_event(&func_scroll_page_decrement);
	}
	else if (rect_track_increment.contains(pos))
	{
		mouse_down_mode = mouse_down_track_incr;
		func_scroll_on_mouse_down = &func_scroll_page_increment;
		part_track_increment.set_pseudo_class(CssStr::pressed, true);

		int last_position = position;

		position += page_step;
		last_step_size = page_step;
		if (position >= scroll_max)
			position = scroll_max-1;
		if (position < scroll_min)
			position = scroll_min;

		if (last_position != position)
			invoke_scroll_event(&func_scroll_page_increment);
	}

	mouse_down_timer.start(100,false);

	update_part_positions();

	scrollbar->request_repaint();
	scrollbar->capture_mouse(true);
	msg->consumed = true;
}

void ScrollBar_Impl::on_mouse_lbutton_up(std::shared_ptr<GUIMessage> &msg, InputEvent &input_event)
{
	part_button_decrement.set_pseudo_class(CssStr::pressed, false);
	part_button_increment.set_pseudo_class(CssStr::pressed, false);
	part_track_decrement.set_pseudo_class(CssStr::pressed, false);
	part_track_increment.set_pseudo_class(CssStr::pressed, false);
	part_thumb.set_pseudo_class(CssStr::pressed, false);
	part_thumb_gripper.set_pseudo_class(CssStr::pressed, false);

	if (mouse_down_mode == mouse_down_thumb_drag)
	{
		if (!func_scroll_thumb_release.is_null())
			func_scroll_thumb_release.invoke();
	}

	mouse_down_mode = mouse_down_none;
	mouse_down_timer.stop();

	scrollbar->request_repaint();
	scrollbar->capture_mouse(false);
	msg->consumed = true;
}

void ScrollBar_Impl::on_mouse_leave()
{
	scrollbar->set_pseudo_class(CssStr::hot, false);
	part_button_decrement.set_pseudo_class(CssStr::hot, false);
	part_button_increment.set_pseudo_class(CssStr::hot, false);
	part_track_decrement.set_pseudo_class(CssStr::hot, false);
	part_track_increment.set_pseudo_class(CssStr::hot, false);
	part_thumb.set_pseudo_class(CssStr::hot, false);
	part_thumb_gripper.set_pseudo_class(CssStr::hot, false);

	scrollbar->request_repaint();
}

void ScrollBar_Impl::on_resized()
{
	update_part_positions();
}

void ScrollBar_Impl::on_render(Canvas &canvas, const Rect &update_rect)
{
	Rect rect = scrollbar->get_geometry();
	part_button_decrement.render_box(canvas, rect_button_decrement, update_rect);
	part_track_decrement.render_box(canvas, rect_track_decrement, update_rect);
	part_thumb.render_box(canvas, rect_thumb, update_rect);
	part_thumb_gripper.render_box(canvas, rect_thumb, update_rect);
	part_track_increment.render_box(canvas, rect_track_increment, update_rect);
	part_button_increment.render_box(canvas, rect_button_increment, update_rect);
}

void ScrollBar_Impl::create_parts()
{
	part_button_decrement = GUIThemePart(scrollbar, "scrollbutton");
	part_button_increment = GUIThemePart(scrollbar, "scrollbutton");
	part_track_decrement = GUIThemePart(scrollbar, "scrolltrack");
	part_track_increment = GUIThemePart(scrollbar,"scrolltrack");
	part_thumb = GUIThemePart(scrollbar,"scrollthumb");
	part_thumb_gripper = GUIThemePart(scrollbar,"scrollthumbgripper");

	part_button_decrement.set_class(vertical ? "up" : "left");
	part_button_increment.set_class(vertical ? "down" : "right");
	part_track_decrement.set_class(vertical ? "up" : "left");
	part_track_increment.set_class(vertical ? "down" : "right");
	part_thumb.set_class(vertical ? "vertical" : "horizontal");
	part_thumb_gripper.set_class(vertical ? "vertical" : "horizontal");

	scrollbar->set_pseudo_class(CssStr::disabled, !scrollbar->is_enabled());
	part_button_decrement.set_pseudo_class(CssStr::disabled, !scrollbar->is_enabled());
	part_button_increment.set_pseudo_class(CssStr::disabled, !scrollbar->is_enabled());
	part_track_decrement.set_pseudo_class(CssStr::disabled, !scrollbar->is_enabled());
	part_track_increment.set_pseudo_class(CssStr::disabled, !scrollbar->is_enabled());
	part_thumb.set_pseudo_class(CssStr::disabled, !scrollbar->is_enabled());
	part_thumb_gripper.set_pseudo_class(CssStr::disabled, !scrollbar->is_enabled());

	scrollbar->set_pseudo_class(CssStr::normal, scrollbar->is_enabled());
	part_button_decrement.set_pseudo_class(CssStr::normal, scrollbar->is_enabled());
	part_button_increment.set_pseudo_class(CssStr::normal, scrollbar->is_enabled());
	part_track_decrement.set_pseudo_class(CssStr::normal, scrollbar->is_enabled());
	part_track_increment.set_pseudo_class(CssStr::normal, scrollbar->is_enabled());
	part_thumb.set_pseudo_class(CssStr::normal, scrollbar->is_enabled());
	part_thumb_gripper.set_pseudo_class(CssStr::normal, scrollbar->is_enabled());
}

// Calculates positions of all parts. Returns true if thumb position was changed compared to previously, false otherwise.
bool ScrollBar_Impl::update_part_positions()
{
	Rect rect(Point(0,0), scrollbar->get_geometry().get_size());
	Rect content_rect = scrollbar->get_content_box();

	int decr_height = part_button_decrement.get_css_height();
	int incr_height = part_button_increment.get_css_height();
	int total_height = vertical ? content_rect.get_height() : content_rect.get_width();
	int track_height = max(0, total_height - decr_height - incr_height);
	int thumb_height = calculate_thumb_size(track_height);

	int thumb_offset = decr_height + calculate_thumb_position(thumb_height, track_height);

	Rect previous_rect_thumb = rect_thumb;

	rect_button_decrement = create_rect(content_rect, 0, decr_height);
	rect_track_decrement = create_rect(content_rect, decr_height, thumb_offset);
	rect_thumb = create_rect(content_rect, thumb_offset, thumb_offset+thumb_height);
	rect_track_increment = create_rect(content_rect, thumb_offset+thumb_height, decr_height+track_height);
	rect_button_increment = create_rect(content_rect, decr_height+track_height, decr_height+track_height+incr_height);

	return (previous_rect_thumb != rect_thumb);
}

int ScrollBar_Impl::calculate_thumb_size(int track_size)
{
	int minimum_thumb_size = 20;
	int range = scroll_max-scroll_min;
	int length = range+page_step-1;
	int thumb_size = page_step*track_size/length;
	if (thumb_size < minimum_thumb_size)
		thumb_size = minimum_thumb_size;
	if (thumb_size > track_size)
		thumb_size = track_size;
	return thumb_size;
}

int ScrollBar_Impl::calculate_thumb_position(int thumb_size, int track_size)
{
	int relative_pos = position-scroll_min;
	int range = scroll_max-scroll_min-1;
	if (range != 0)
	{
		int available_area = max(0, track_size-thumb_size);
		return relative_pos*available_area/range;
	}
	else
	{
		return 0;
	}
}

Rect ScrollBar_Impl::create_rect(const Rect content_rect, int start, int end)
{
	if (vertical)
		return Rect(content_rect.left, content_rect.top+start, content_rect.right, content_rect.top+end);
	else
		return Rect(content_rect.left+start, content_rect.top, content_rect.left+end, content_rect.bottom);
}

void ScrollBar_Impl::on_timer_expired()
{
	if (mouse_down_mode == mouse_down_thumb_drag)
		return;

	mouse_down_timer.start(100, false);

	int last_position = position;
	position += last_step_size;
	if (position >= scroll_max)
		position = scroll_max-1;

	if (position < scroll_min)
		position = scroll_min;

	if (position != last_position)
	{
		invoke_scroll_event(func_scroll_on_mouse_down);

		if(update_part_positions())
			scrollbar->request_repaint();
	}
}

void ScrollBar_Impl::on_enablemode_changed()
{
	scrollbar->set_pseudo_class(CssStr::disabled, !scrollbar->is_enabled());
	part_button_decrement.set_pseudo_class(CssStr::disabled, !scrollbar->is_enabled());
	part_button_increment.set_pseudo_class(CssStr::disabled, !scrollbar->is_enabled());
	part_track_decrement.set_pseudo_class(CssStr::disabled, !scrollbar->is_enabled());
	part_track_increment.set_pseudo_class(CssStr::disabled, !scrollbar->is_enabled());
	part_thumb.set_pseudo_class(CssStr::disabled, !scrollbar->is_enabled());
	part_thumb_gripper.set_pseudo_class(CssStr::disabled, !scrollbar->is_enabled());

	scrollbar->set_pseudo_class(CssStr::normal, scrollbar->is_enabled());
	part_button_decrement.set_pseudo_class(CssStr::normal, scrollbar->is_enabled());
	part_button_increment.set_pseudo_class(CssStr::normal, scrollbar->is_enabled());
	part_track_decrement.set_pseudo_class(CssStr::normal, scrollbar->is_enabled());
	part_track_increment.set_pseudo_class(CssStr::normal, scrollbar->is_enabled());
	part_thumb.set_pseudo_class(CssStr::normal, scrollbar->is_enabled());
	part_thumb_gripper.set_pseudo_class(CssStr::normal, scrollbar->is_enabled());

	scrollbar->request_repaint();
}

void ScrollBar_Impl::invoke_scroll_event(Callback_v0 *event_ptr)
{
	if (position == scroll_max-1)
	{
		if (!func_scroll_max.is_null())
			func_scroll_max.invoke();
	}

	if (position == scroll_min)
	{
		if (!func_scroll_min.is_null())
			func_scroll_min.invoke();
	}

	if (!func_scroll.is_null())
		func_scroll.invoke();

	if (!event_ptr->is_null())
		event_ptr->invoke();
}

}
