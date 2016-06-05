/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
**    Magnus Norddahl
**    Mark Page
*/

#include "UI/precomp.h"
#include "API/UI/StandardViews/slider_view.h"
#include "API/UI/Style/style_property_parser.h"
#include "API/Display/2D/path.h"
#include "API/Display/System/timer.h"
#include "API/Display/2D/brush.h"
#include "API/UI/Events/pointer_event.h"
#include <algorithm>
#include "slider_view_impl.h"

namespace clan
{
	void SliderViewImpl::update_state()
	{
		bool target_hot = false;
		bool target_disabled = false;
		bool target_pressed = false;

		if (_state_disabled)
		{
			target_disabled = true;
		}
		else if (_state_pressed)
		{
			target_pressed = true;
		}
		else if (_state_hot)
		{
			target_hot = true;
		}

		slider->set_state_cascade("hot", target_hot);
		slider->set_state_cascade("pressed", target_pressed);
		slider->set_state_cascade("disabled", target_disabled);
	}

	void SliderViewImpl::update_pos(SliderView *view, int new_pos, int new_min, int new_max)
	{
		bool changed = new_min != _min_position || new_max != _max_position || new_pos != _position;
		if (changed)
		{
			_min_position = new_min;
			_max_position = new_max;
			_position = new_pos;
			view->set_needs_layout();
		}
	}

	void SliderViewImpl::on_focus_gained(FocusChangeEvent &e)
	{
	}

	void SliderViewImpl::on_focus_lost(FocusChangeEvent &e)
	{
		if (_state_disabled)
			return;
		mouse_down_mode = mouse_down_none;
		scroll_timer.stop();
	}

	void SliderViewImpl::on_activated(ActivationChangeEvent &e)
	{

	}

	void SliderViewImpl::on_deactivated(ActivationChangeEvent &e)
	{
		if (_state_disabled)
			return;

		mouse_down_mode = mouse_down_none;
		scroll_timer.stop();
	}

	void SliderViewImpl::on_pointer_track_press(PointerEvent &e)
	{
		if (_state_disabled || e.button() != PointerButton::left)
			return;
		if (e.target() == thumb)	// Thumb control handled elsewhere
			return;

		float mouse_pos;
		Rectf thumb_geometry(thumb->geometry().content_box());
		float thumb_position;
		if (slider->horizontal())
		{
			mouse_pos = e.pos(track.get()).x;
			thumb_position = thumb_geometry.left + thumb_geometry.get_width() / 2.0f;
			timer_target_position = _min_position + mouse_pos * ((_max_position - _min_position)) / (track->geometry().content_box().get_width());
		}
		else
		{
			mouse_pos = e.pos(track.get()).y;
			thumb_position = thumb_geometry.top + thumb_geometry.get_height() / 2.0f;
			timer_target_position = _min_position + mouse_pos * ((_max_position - _min_position)) / (track->geometry().content_box().get_height());
		}

		if (mouse_pos < thumb_position)
		{
			mouse_down_mode = mouse_down_track_decr;
			timer_step_size = -_page_step;
		}
		else
		{
			mouse_down_mode = mouse_down_track_incr;
			timer_step_size = _page_step;
		}

		scroll_timer_expired();
	}

	void SliderViewImpl::on_pointer_track_release(PointerEvent &e)
	{
		if (_state_disabled || e.button() != PointerButton::left)
			return;
		mouse_down_mode = mouse_down_none;
		scroll_timer.stop();
	}

	void SliderViewImpl::on_pointer_thumb_press(PointerEvent &e)
	{
		if (_state_disabled || e.button() != PointerButton::left)
			return;

		_state_pressed = true;
		update_state();
		mouse_down_mode = mouse_down_thumb_drag;
		thumb_move_start_position = _position;
		mouse_drag_start_pos = e.pos(track.get());
	}

	void SliderViewImpl::on_pointer_thumb_release(PointerEvent &e)
	{
		if (_state_disabled || e.button() != PointerButton::left)
			return;
		_state_pressed = false;
		update_state();
		mouse_down_mode = mouse_down_none;
		scroll_timer.stop();
	}

	void SliderViewImpl::on_pointer_move(PointerEvent &e)
	{
		if (_state_disabled)
			return;
		if (mouse_down_mode != mouse_down_thumb_drag)
		{
			return;
		}

		Pointf mouse_pos(e.pos(track.get()));
		Rectf track_geometry(track->geometry().content_box());

		int last_position = _position;

		if (mouse_pos.x < -100 || mouse_pos.x > track_geometry.get_width() + 100 || mouse_pos.y < -100 || mouse_pos.y > track_geometry.get_height() + 100)
		{
			_position = thumb_move_start_position;
		}
		else
		{
			if (slider->horizontal())
			{
				int delta = (mouse_pos.x - mouse_drag_start_pos.x);
				_position = thumb_move_start_position + (delta * (_max_position - _min_position)) / (track->geometry().content_box().get_width());
			}
			else
			{
				int delta = (mouse_pos.y - mouse_drag_start_pos.y);
				_position = thumb_move_start_position + (delta * (_max_position - _min_position)) / (track->geometry().content_box().get_height());
			}
		}
		if (_position > _max_position)
			_position = _max_position;
		if (_position < _min_position)
			_position = _min_position;

		if (_lock_to_ticks)
		{
			int remainder = (_position - _min_position) % _tick_count;
			_position = _position - remainder;
		}

		if (last_position != _position)
		{
			if (_func_value_changed)
				_func_value_changed();
			slider->set_needs_layout();
		}
	}
	
	void SliderViewImpl::scroll_timer_expired()
	{
		if (_state_disabled)
			return;
		if ((mouse_down_mode == mouse_down_none) || (mouse_down_mode == mouse_down_thumb_drag))
			return;

		int last_position = _position;
		_position += timer_step_size;

		if (mouse_down_mode == mouse_down_track_decr)
		{
			if (_position < timer_target_position)
				_position = timer_target_position;
		}
		if (mouse_down_mode == mouse_down_track_incr)
		{
			if (_position > timer_target_position)
				_position = timer_target_position;
		}

		if (_position > _max_position)
			_position = _max_position;
		if (_position < _min_position)
			_position = _min_position;

		if (last_position != _position)
		{
			if (_func_value_changed)
				_func_value_changed();
			slider->set_needs_layout();
		}
		scroll_timer.start(100, false);

	}

}
