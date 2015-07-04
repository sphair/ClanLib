/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
*/

#pragma once

#include "API/Display/Window/display_window.h"

namespace clan
{
	class KeyEvent;
	class PointerEvent;

	class WindowView_Impl
	{
	public:
		WindowView_Impl(WindowView *view, const DisplayWindowDescription &desc);

		WindowView *window_view;
		DisplayWindow window;
		Canvas canvas;
		SlotContainer slots;

		std::shared_ptr<View> captured_view;
		int capture_down_counter = 0;

		std::shared_ptr<View> hot_view;

		WindowView *popup_owner = 0;
		std::map<View *, std::shared_ptr<WindowView>> popups;

		WindowView *modal_owner = 0;
		std::shared_ptr<WindowView> modal;

	private:
		Pointf to_root_pos(const Pointf &client_pos) const;

		void dispatch_hot_event(std::shared_ptr<View> &view, PointerEvent &e);
		std::shared_ptr<View> get_capture_view(PointerEvent &e, std::shared_ptr<View> &view_above_cursor);

		void window_key_event(KeyEvent &e);
		void window_pointer_event(PointerEvent &e);
		void release_capture();

		void on_window_close();
		void on_lost_focus();
		void on_got_focus();
		void on_resize(float, float);
		void on_paint(const clan::Rect &);
		void on_key_down(const clan::InputEvent &);
		void on_key_up(const clan::InputEvent &);
		void on_mouse_down(const clan::InputEvent &);
		void on_mouse_dblclk(const clan::InputEvent &);
		void on_mouse_up(const clan::InputEvent &);
		void on_mouse_move(const clan::InputEvent &);
		PointerButton decode_id(clan::InputCode ic) const;

	};
}
