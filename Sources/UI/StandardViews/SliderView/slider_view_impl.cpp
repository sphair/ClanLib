/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
**    Artem Khomenko
*/

#include "UI/precomp.h"
#include "API/UI/StandardViews/slider_view.h"
#include "API/UI/Style/style_property_parser.h"
#include "API/Display/2D/path.h"
#include "API/Display/System/timer.h"
#include "API/Display/2D/brush.h"
#include "API/UI/Events/pointer_event.h"
#include <algorithm>
#include "API/UI/TopLevel/view_tree.h"
#include "API/Display/Window/display_window.h"
#include "API/Display/Window/input_event.h"
#include "slider_view_impl.h"

namespace clan
{
	// Delays for timer
	const int cLongDelay = 300;
	const int cShortDelay = 30;

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

		// Draw changes.
		slider->draw_without_layout();
	}

	void SliderViewImpl::update_pos(SliderView *view, int new_pos, int new_min, int new_max)
	{
		bool changed = new_min != _min_position || new_max != _max_position || new_pos != _position;
		if (changed)
		{
			_min_position = new_min;
			_max_position = new_max;
			_position = new_pos;

			// Update the view.
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

		float mouse_pos;
		const ViewGeometry &thumb_geometry(thumb->geometry());
		float thumb_position;
		if (slider->horizontal())
		{
			mouse_pos = e.pos(track.get()).x;
			thumb_position = thumb_geometry.content_x + thumb_geometry.content_width * 0.5f;
			timer_target_position = _min_position + mouse_pos * thumb_units_per_pixel();
		}
		else
		{
			mouse_pos = e.pos(track.get()).y;
			thumb_position = thumb_geometry.content_y + thumb_geometry.content_height * 0.5f;
			timer_target_position = _min_position + mouse_pos * thumb_units_per_pixel();
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

		// For scroll timer - between first and second there is need a big delay, then small.
		isFirstTimerExpired = true;
		scroll_timer_expired();
	}

	void SliderViewImpl::on_pointer_track_release(PointerEvent &e)
	{
		if (_state_disabled || e.button() != PointerButton::left)
			return;
		mouse_down_mode = mouse_down_none;
		scroll_timer.stop();

		// Mouse position relative to track.
		//Pointf mouse_pos = e.pos(slider->track());

		// Thumb position relative to track.
		//Rectf thumb_geometry(thumb->geometry().content_box());

		// Check and set hot state by long and winding road - need to reach TopLevelWindow_Impl::dispatch_hot_event().
		auto tree = slider->view_tree();
		DisplayWindow pDispWindow = tree->display_window();
		if (pDispWindow) {
			InputDevice &pInputDevice = pDispWindow.get_mouse();
			if (pInputDevice) {

				// Root view.
				auto rootView = tree->root_view();

				// Mouse position relative to root.
				Pointf mouse_pos = e.pos(rootView);

				// Change relative to screen.
				mouse_pos = rootView->to_screen_pos(mouse_pos);

				// Change relative to client.
				mouse_pos = pDispWindow.screen_to_client(mouse_pos);

				// Prepare event to be emitted:
				InputEvent moveEvent;
				moveEvent.type = InputEvent::pointer_moved;
				moveEvent.mouse_pos = mouse_pos;

				// Send message.
				pInputDevice.sig_pointer_move()(moveEvent);
			}
		}
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

		e.stop_propagation(); // prevent track press reacting to this event
	}

	void SliderViewImpl::on_pointer_thumb_release(PointerEvent &e)
	{
		if (_state_disabled || e.button() != PointerButton::left)
			return;
		_state_pressed = false;
		update_state();
		mouse_down_mode = mouse_down_none;

		e.stop_propagation(); // prevent track press reacting to this event
	}

	void SliderViewImpl::on_pointer_move(PointerEvent &e)
	{
		if (_state_disabled || mouse_down_mode != mouse_down_thumb_drag)
			return;

		Pointf mouse_pos(e.pos(track.get()));
		const ViewGeometry &track_geometry(track->geometry());

		int last_position = _position;

		if (mouse_pos.x < -100 || mouse_pos.x > track_geometry.content_width + 100 || mouse_pos.y < -100 || mouse_pos.y > track_geometry.content_height + 100)
		{
			// If the mouse strayed very far away, back to the starting position the thumb.
			_position = thumb_move_start_position;
		}
		else
		{
			int delta = slider->horizontal() ? mouse_pos.x - mouse_drag_start_pos.x : mouse_pos.y - mouse_drag_start_pos.y;
			_position = thumb_move_start_position + delta * thumb_units_per_pixel();
		}

		// Set position to near integer in _lock_to_ticks if needs.
		if (_lock_to_ticks)
			_position = (int)roundf((float)_position / _tick_count) * _tick_count;

		if (_position > _max_position)
			_position = _max_position;
		else if (_position < _min_position)
			_position = _min_position;

		if (last_position != _position)
		{
			// Notify the user.
			if (_func_value_changed)
				_func_value_changed();

			// Update the view.
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

		if (_position < _min_position)
			_position = _min_position;
		else if (_position > _max_position)
			_position = _max_position;

		if (last_position != _position)
		{
			// Notify the user.
			if (_func_value_changed)
				_func_value_changed();

			// Update the view.
			slider->set_needs_layout();
		}

		// Run timer again only if the goal is not reached.
		// TODO FIX  warning: '&&' within '||'[-Wlogical - op - parentheses]
		if (mouse_down_mode == mouse_down_track_decr && _position > timer_target_position
			|| mouse_down_mode == mouse_down_track_incr && _position + _page_step < timer_target_position
			)
			scroll_timer.start(isFirstTimerExpired ? cLongDelay : cShortDelay, false);

		// Switch to a short delay.
		isFirstTimerExpired = false;
	}

}
