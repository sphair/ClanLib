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

#pragma once

namespace clan
{
	class SliderViewImpl
	{
	public:

		void on_pointer_track_press(PointerEvent &e);
		void on_pointer_track_release(PointerEvent &e);
		void on_pointer_thumb_press(PointerEvent &e);
		void on_pointer_thumb_release(PointerEvent &e);

		void on_pointer_move(PointerEvent &e);
		void on_focus_gained(FocusChangeEvent &e);
		void on_focus_lost(FocusChangeEvent &e);
		void on_activated(ActivationChangeEvent &e);
		void on_deactivated(ActivationChangeEvent &e);

		void scroll_timer_expired();

		enum MouseDownMode
		{
			mouse_down_none,
			mouse_down_track_decr,
			mouse_down_track_incr,
			mouse_down_thumb_drag
		} mouse_down_mode = mouse_down_none;

		SliderView *slider = nullptr;

		std::shared_ptr<View> track;
		std::shared_ptr<View> thumb;
		Pointf mouse_drag_start_pos;
		std::function<void()> _func_value_changed;
		Timer scroll_timer;

		int _min_position = 0;
		int _max_position = 100;
		int _position = 50;
		bool _vertical = false;
		int _tick_count = 1;
		int _page_step = 1;
		bool _lock_to_ticks = false;

		int timer_step_size = 0;
		int timer_target_position = 0;
		int thumb_move_start_position = 0;

		bool _state_disabled = false;
		bool _state_hot = false;
		bool _state_pressed = false;

		void update_pos(SliderView *view, int new_pos, int new_min, int new_max);
		void update_state();

	};

}
