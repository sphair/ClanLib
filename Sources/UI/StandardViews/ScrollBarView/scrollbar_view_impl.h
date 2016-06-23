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
**    Artem Khomenko
*/

#pragma once

#include <algorithm>

namespace clan
{
	enum class ScrollBarButtonDirection
	{
		left,
		right,
		up,
		down
	};

	class ScrollBarButtonView : public View
	{
	public:
		ScrollBarButtonView(bool render_button_arrows);

		void render_content(Canvas &canvas) override;
		void set_direction(ScrollBarButtonDirection new_dir);

	private:
		ScrollBarButtonDirection direction = ScrollBarButtonDirection::left;
		bool _render_button_arrows = true;
	};

	class ScrollBarViewImpl
	{
	public:

		void on_pointer_track_press(PointerEvent &e);
		void on_pointer_track_release(PointerEvent &e);
		void on_pointer_thumb_press(PointerEvent &e);
		void on_pointer_thumb_release(PointerEvent &e);
		void on_pointer_decrement_press(PointerEvent &e);
		void on_pointer_decrement_release(PointerEvent &e);
		void on_pointer_increment_press(PointerEvent &e);
		void on_pointer_increment_release(PointerEvent &e);

		void on_pointer_move(PointerEvent &e);
		void on_focus_gained(FocusChangeEvent &e);
		void on_focus_lost(FocusChangeEvent &e);
		void on_activated(ActivationChangeEvent &e);
		void on_deactivated(ActivationChangeEvent &e);

		void scroll_timer_expired();

		// Pixel length of the entire scrollbar track
		double track_length() const
		{
			auto track_geometry = track->geometry();
			return scrollbar->vertical() ? track_geometry.content_box().get_height() : track_geometry.content_box().get_width();
		}

		// Pixel length of thumb
		double thumb_length() const
		{
			double track = track_length();
			double content_size = max_pos - min_pos + page_step;
			if (content_size * track == 0.0)
				return 0.0;
			double length = page_step / content_size * track;
			return std::min(std::max(length, 16.0), track);
		}

		// Pixel position of thumb
		double thumb_pos() const
		{
			if (min_pos == max_pos)
				return 0.0;
			double t = (pos - min_pos) / (max_pos - min_pos);
			return t * (track_length() - thumb_length());
		}

		// How many units to move per pixel when dragging the thumb
		double thumb_units_per_pixel() const
		{
			double available_units = max_pos - min_pos;
			double available_pixels = track_length() - thumb_length();
			return available_pixels != 0.0 ? available_units / available_pixels : 0.0;
		}

		enum MouseDownMode
		{
			mouse_down_none,
			mouse_down_button_decr,
			mouse_down_button_incr,
			mouse_down_track_decr,
			mouse_down_track_incr,
			mouse_down_thumb_drag
		} mouse_down_mode = mouse_down_none;

		ScrollBarView *scrollbar = nullptr;

		std::shared_ptr<ScrollBarButtonView> button_decrement;
		std::shared_ptr<ScrollBarButtonView> button_increment;
		std::shared_ptr<View> track;
		std::shared_ptr<View> thumb;
		std::shared_ptr<View> thumb_grip;
		double min_pos = 0.0;
		double max_pos = 100.0;
		double pos = 0.0;
		double line_step = 1.0;
		double page_step = 25.0;
		double timer_step_size = 0.0;
		double timer_target_position = 0.0;
		double thumb_move_start_position = 0.0;
		Pointf mouse_drag_start_pos;
		Signal<void()> sig_scroll;
		Timer scroll_timer;
		bool _state_disabled = false;
		bool _state_thumb_hot = false;
		bool _state_thumb_pressed = false;
		bool _state_track_hot = false;
		bool _state_track_pressed = false;
		bool _state_increment_hot = false;
		bool _state_increment_pressed = false;
		bool _state_decrement_hot = false;
		bool _state_decrement_pressed = false;

		void update_thumb_state();
		void update_track_state();
		void update_increment_state();
		void update_decrement_state();

		void set_standard_state(View *view, bool state_disabled, bool state_hot, bool state_pressed);

		void update_pos(ScrollBarView *view, double new_pos, double new_min, double new_max);

	private:
		// For scroll timer - between first and second need a big delay, then the small one.
		bool isFirstTimerExpired = true;
	};

}
