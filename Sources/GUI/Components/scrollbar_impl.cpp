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
#include "API/Core/Text/string_help.h"
#include "scrollbar_impl.h"
#include "../gui_css_strings.h"
#include "API/CSSLayout/css_box_properties.h"

namespace clan
{

ScrollBar_Impl::ScrollBar_Impl()
: vertical(false), scroll_min(0), scroll_max(1), line_step(1),
  page_step(10), position(0), mouse_down_mode(mouse_down_none),
  thumb_start_position(0), thumb_start_pixel_position(0),
  func_scroll_on_mouse_down(0),  thumb_left(256), thumb_size(256)
{
}

void ScrollBar_Impl::on_part_button_decrement(std::shared_ptr<GUIMessage> &msg)
{
	if (!scrollbar->is_enabled())
		return;
	std::shared_ptr<GUIMessage_Input> input_msg = std::dynamic_pointer_cast<GUIMessage_Input>(msg);
	if (input_msg)
	{
		if (input_msg->input_event.type == InputEvent::pressed && input_msg->input_event.id == mouse_left)
		{
			mouse_drag_start_pos = input_msg->input_event.mouse_pos;;
			mouse_down_mode = mouse_down_button_decr;
			func_scroll_on_mouse_down = &func_scroll_line_decrement;
			part_button_decrement->set_pseudo_class(CssStr::pressed, true);

			int last_position = position;

			position -= line_step;
			last_step_size = -line_step;
			if (position >= scroll_max)
				position = scroll_max-1;
			if (position < scroll_min)
				position = scroll_min;

			if (last_position != position)
				invoke_scroll_event(&func_scroll_line_decrement);

			update_part_positions();

		}
		else if (input_msg->input_event.type == InputEvent::released && input_msg->input_event.id == mouse_left)
		{
			part_button_decrement->set_pseudo_class(CssStr::pressed, false);
			mouse_released();
		}
	}
}

void ScrollBar_Impl::on_part_track_decrement(std::shared_ptr<GUIMessage> &msg)
{
	if (!scrollbar->is_enabled())
		return;
	std::shared_ptr<GUIMessage_Input> input_msg = std::dynamic_pointer_cast<GUIMessage_Input>(msg);
	if (input_msg)
	{
		if (input_msg->input_event.type == InputEvent::pressed && input_msg->input_event.id == mouse_left)
		{
			mouse_drag_start_pos = input_msg->input_event.mouse_pos;;
			mouse_down_mode = mouse_down_track_decr;
			func_scroll_on_mouse_down = &func_scroll_page_decrement;
			part_track_decrement->set_pseudo_class(CssStr::pressed, true);
			part_track_increment->set_pseudo_class(CssStr::pressed, true);

			int last_position = position;

			position -= page_step;
			last_step_size = -page_step;
			if (position >= scroll_max)
				position = scroll_max-1;
			if (position < scroll_min)
				position = scroll_min;

			if (last_position != position)
				invoke_scroll_event(&func_scroll_page_decrement);

			update_part_positions();
		}
		else if (input_msg->input_event.type == InputEvent::released && input_msg->input_event.id == mouse_left)
		{
			part_track_increment->set_pseudo_class(CssStr::pressed, false);
			part_track_decrement->set_pseudo_class(CssStr::pressed, false);
			mouse_released();
		}
	}
}

void ScrollBar_Impl::on_part_thumb(std::shared_ptr<GUIMessage> &msg)
{
	if (!scrollbar->is_enabled())
		return;
	std::shared_ptr<GUIMessage_Input> input_msg = std::dynamic_pointer_cast<GUIMessage_Input>(msg);
	if (input_msg)
	{
		if (input_msg->input_event.type == InputEvent::pressed && input_msg->input_event.id == mouse_left)
		{
			mouse_drag_start_pos = input_msg->input_event.mouse_pos;;
			mouse_down_mode = mouse_down_thumb_drag;
			thumb_start_position = position;
			Rect rect_thumb = part_thumb->get_geometry();
			Rect rect_track_decrement = part_track_decrement->get_geometry();
			thumb_start_pixel_position = vertical ? (rect_thumb.top-rect_track_decrement.top) : (rect_thumb.left-rect_track_decrement.left);
			part_thumb->set_pseudo_class(CssStr::pressed, true);
			part_thumb_gripper.set_pseudo_class(CssStr::pressed, true);

			mouse_down_timer.start(100,false);
			update_part_positions();
			part_thumb->capture_mouse(true);

		}
		else if (input_msg->input_event.type == InputEvent::released && input_msg->input_event.id == mouse_left)
		{
			part_thumb->set_pseudo_class(CssStr::pressed, false);
			part_thumb_gripper.set_pseudo_class(CssStr::pressed, false);
			mouse_released();
		}
		else if (input_msg->input_event.type == InputEvent::pointer_moved)
			mouse_moved(input_msg->input_event.mouse_pos);

	}
}

void ScrollBar_Impl::on_part_track_increment(std::shared_ptr<GUIMessage> &msg)
{
	if (!scrollbar->is_enabled())
		return;
	std::shared_ptr<GUIMessage_Input> input_msg = std::dynamic_pointer_cast<GUIMessage_Input>(msg);
	if (input_msg)
	{
		if (input_msg->input_event.type == InputEvent::pressed && input_msg->input_event.id == mouse_left)
		{
			mouse_drag_start_pos = input_msg->input_event.mouse_pos;;
			mouse_down_mode = mouse_down_track_incr;
			func_scroll_on_mouse_down = &func_scroll_page_increment;
			part_track_increment->set_pseudo_class(CssStr::pressed, true);
			part_track_decrement->set_pseudo_class(CssStr::pressed, true);

			int last_position = position;

			position += page_step;
			last_step_size = page_step;
			if (position >= scroll_max)
				position = scroll_max-1;
			if (position < scroll_min)
				position = scroll_min;

			if (last_position != position)
				invoke_scroll_event(&func_scroll_page_increment);

			update_part_positions();

		}
		else if (input_msg->input_event.type == InputEvent::released && input_msg->input_event.id == mouse_left)
		{
			part_track_increment->set_pseudo_class(CssStr::pressed, false);
			part_track_decrement->set_pseudo_class(CssStr::pressed, false);
			mouse_released();
		}

	}
}

void ScrollBar_Impl::on_part_button_increment(std::shared_ptr<GUIMessage> &msg)
{
	if (!scrollbar->is_enabled())
		return;
	std::shared_ptr<GUIMessage_Input> input_msg = std::dynamic_pointer_cast<GUIMessage_Input>(msg);
	if (input_msg)
	{
		if (input_msg->input_event.type == InputEvent::pressed && input_msg->input_event.id == mouse_left)
		{
			mouse_drag_start_pos = input_msg->input_event.mouse_pos;
			mouse_down_mode = mouse_down_button_incr;
			part_button_increment->set_pseudo_class(CssStr::pressed, true);
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

			update_part_positions();

		}
		else if (input_msg->input_event.type == InputEvent::released && input_msg->input_event.id == mouse_left)
		{
			part_button_increment->set_pseudo_class(CssStr::pressed, false);
			mouse_released();
		}

	}
}

void ScrollBar_Impl::mouse_moved(const Point &pos)
{
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

			Rect rect_track_decrement = part_track_decrement->get_geometry();
			Rect rect_track_increment = part_track_increment->get_geometry();

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

			update_part_positions();
		}
	}

}

void ScrollBar_Impl::mouse_released()
{
	mouse_down_mode = mouse_down_none;
	mouse_down_timer.stop();
	part_thumb->capture_mouse(false);
}

void ScrollBar_Impl::update_part_positions()
{
	Rect rect_part_track_decrement = part_track_decrement->get_geometry();
	Rect rect_part_thumb = part_thumb->get_geometry();
	Rect rect_part_track_increment = part_track_increment->get_geometry();
	Rect rect_part_button_decrement = part_button_decrement->get_geometry();

	int track_size = rect_part_track_decrement.get_width() + rect_part_track_increment.get_width();
	thumb_size = calculate_thumb_size(track_size);

	int position = calculate_thumb_position(thumb_size, track_size);
	Rect new_rect;
	new_rect.left = rect_part_button_decrement.right + 1 + position;
	new_rect.right = new_rect.left + thumb_size;
	new_rect.top = rect_part_thumb.top;
	new_rect.bottom = rect_part_thumb.bottom;
	part_thumb->set_geometry( new_rect );

	token_width.type = CSSToken::type_dimension;
	token_width.value = StringHelp::int_to_text(thumb_size);
	token_width.dimension = "px";

	token_left.type = CSSToken::type_dimension;
	thumb_left = position + 1 - (part_button_increment->get_geometry().get_width() + track_size);
	token_left.value = StringHelp::int_to_text( thumb_left );
	token_left.dimension = "px";

	int thumb_middle = new_rect.get_center().x;

	new_rect = rect_part_track_decrement;
	new_rect.right = thumb_middle - 1; 
	part_track_decrement->set_geometry( new_rect );

	new_rect = rect_part_track_increment;
	new_rect.left = thumb_middle;
	part_track_increment->set_geometry( new_rect );

	scrollbar->request_repaint();
}

void ScrollBar_Impl::create_parts()
{
	part_button_decrement = new GUIComponent(scrollbar, vertical ? "scrollbutton_up" : "scrollbutton_left");
	part_track_decrement = new GUIComponent(scrollbar, vertical ? "scrolltrack_up" : "scrolltrack_left");
	part_track_increment = new GUIComponent(scrollbar, vertical ? "scrolltrack_down" : "scrolltrack_right");
	part_button_increment = new GUIComponent(scrollbar, vertical ? "scrollbutton_down" : "scrollbutton_right");
	part_thumb = new GUIComponent(scrollbar, vertical ? "scrollthumb_vertical" : "scrollthumb_horizontal");
	part_thumb_gripper = GUIThemePart(part_thumb, vertical ? "scrollthumbgripper_vertical" : "scrollthumbgripper_horizontal");

	part_button_decrement->set_double_click_enabled(false);
	part_track_decrement->set_double_click_enabled(false);
	part_track_increment->set_double_click_enabled(false);
	part_button_increment->set_double_click_enabled(false);
	part_thumb->set_double_click_enabled(false);

	on_enablemode_changed();

	part_button_decrement->func_process_message().set(this, &ScrollBar_Impl::on_part_button_decrement);
	part_track_decrement->func_process_message().set(this, &ScrollBar_Impl::on_part_track_decrement);
	part_thumb->func_process_message().set(this, &ScrollBar_Impl::on_part_thumb);
	part_track_increment->func_process_message().set(this, &ScrollBar_Impl::on_part_track_increment);
	part_button_increment->func_process_message().set(this, &ScrollBar_Impl::on_part_button_increment);
	part_thumb->func_render().set(this, &ScrollBar_Impl::on_part_thumb_render);

	scrollbar->func_enablemode_changed().set(this, &ScrollBar_Impl::on_enablemode_changed);

	part_thumb->func_apply_properties().set(this, &ScrollBar_Impl::on_apply_properties);

	mouse_down_timer.func_expired().set(this, &ScrollBar_Impl::on_timer_expired);

}

void ScrollBar_Impl::on_part_thumb_render(Canvas &canvas, const Rect &update_rect)
{
	Rect thumb_rect = part_thumb->get_geometry();
	int gripper_width = part_thumb_gripper.get_css_properties().width.length.value;
	Rect gripper_rect((thumb_rect.get_width() - gripper_width)/2, thumb_rect.top, Size(gripper_width, thumb_rect.get_height())) ;
	part_thumb_gripper.render_box(canvas, gripper_rect, update_rect);
}

void ScrollBar_Impl::on_apply_properties(CSSBoxProperties &properties)
{
	properties.width.type = CSSBoxWidth::type_length;
	properties.width.length.value = thumb_size;
	properties.width.length.type = CSSBoxLength::type_px;

	properties.left.type = CSSBoxLeft::type_length;
	properties.left.length.value = thumb_left;
	properties.left.length.type = CSSBoxLength::type_px;
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
	}
}

void ScrollBar_Impl::on_enablemode_changed()
{
	scrollbar->set_pseudo_class(CssStr::disabled, !scrollbar->is_enabled());
	part_button_decrement->set_pseudo_class(CssStr::disabled, !scrollbar->is_enabled());
	part_button_increment->set_pseudo_class(CssStr::disabled, !scrollbar->is_enabled());
	part_track_decrement->set_pseudo_class(CssStr::disabled, !scrollbar->is_enabled());
	part_track_increment->set_pseudo_class(CssStr::disabled, !scrollbar->is_enabled());
	part_thumb->set_pseudo_class(CssStr::disabled, !scrollbar->is_enabled());
	part_thumb_gripper.set_pseudo_class(CssStr::disabled, !scrollbar->is_enabled());

	scrollbar->set_pseudo_class(CssStr::normal, scrollbar->is_enabled());
	part_button_decrement->set_pseudo_class(CssStr::normal, scrollbar->is_enabled());
	part_button_increment->set_pseudo_class(CssStr::normal, scrollbar->is_enabled());
	part_track_decrement->set_pseudo_class(CssStr::normal, scrollbar->is_enabled());
	part_track_increment->set_pseudo_class(CssStr::normal, scrollbar->is_enabled());
	part_thumb->set_pseudo_class(CssStr::normal, scrollbar->is_enabled());
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
