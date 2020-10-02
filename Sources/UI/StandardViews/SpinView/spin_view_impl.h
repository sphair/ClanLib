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
**    Harry Storbacka
**    Magnus Norddahl
**    Kenneth Gangstoe
**    Mark Page
*/

#pragma once

namespace clan
{

	class SpinView_Impl
	{
	public:

		void on_pointer_decrement_press(PointerEvent &e);
		void on_pointer_decrement_release(PointerEvent &e);
		void on_pointer_increment_press(PointerEvent &e);
		void on_pointer_increment_release(PointerEvent &e);

		void on_focus_gained(FocusChangeEvent &e);
		void on_focus_lost(FocusChangeEvent &e);
		void on_activated(ActivationChangeEvent &e);
		void on_deactivated(ActivationChangeEvent &e);

		void timer_expired();

		enum MouseDownMode
		{
			mouse_down_none,
			mouse_down_button_decr,
			mouse_down_button_incr,
		} mouse_down_mode = mouse_down_none;

		SpinView *spin = nullptr;

		std::shared_ptr<View> button_decrement;
		std::shared_ptr<View> button_increment;

		Signal<void()> sig_scroll;
		Timer timer;
		bool _state_disabled = false;
		bool _state_increment_hot = false;
		bool _state_increment_pressed = false;
		bool _state_decrement_hot = false;
		bool _state_decrement_pressed = false;

		double _value = 0.0;
		double _minimum = 0.0;
		double _maximum = 0.0;
		double _step_size = 0.0;
		int _decimal_places = 0;

		void update_increment_state();
		void update_decrement_state();

		void set_standard_state(View *view, bool state_disabled, bool state_hot, bool state_pressed);

		std::function<void()> _func_value_changed;


	};

}
