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

#include "UI/precomp.h"
#include "API/UI/StandardViews/window_view.h"
#include "API/UI/Events/key_event.h"
#include "API/UI/Events/pointer_event.h"
#include "API/UI/Events/close_event.h"
#include "API/UI/Events/activation_change_event.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/input_context.h"
#include "API/Display/2D/canvas.h"
#include "window_view_impl.h"

namespace clan
{
	WindowView_Impl::WindowView_Impl(WindowView *view, const DisplayWindowDescription &desc) : window_view(view), window(desc)
	{
		canvas = Canvas(window);

		slots.connect(window.sig_lost_focus(), clan::bind_member(this, &WindowView_Impl::on_lost_focus));
		slots.connect(window.sig_got_focus(), clan::bind_member(this, &WindowView_Impl::on_got_focus));
		slots.connect(window.sig_resize(), clan::bind_member(this, &WindowView_Impl::on_resize));
		slots.connect(window.sig_paint(), clan::bind_member(this, &WindowView_Impl::on_paint));
		slots.connect(window.sig_window_close(), clan::bind_member(this, &WindowView_Impl::on_window_close));
		slots.connect(window.get_ic().get_keyboard().sig_key_down(), clan::bind_member(this, &WindowView_Impl::on_key_down));
		slots.connect(window.get_ic().get_keyboard().sig_key_up(), clan::bind_member(this, &WindowView_Impl::on_key_up));
		slots.connect(window.get_ic().get_mouse().sig_key_down(), clan::bind_member(this, &WindowView_Impl::on_mouse_down));
		slots.connect(window.get_ic().get_mouse().sig_key_dblclk(), clan::bind_member(this, &WindowView_Impl::on_mouse_dblclk));
		slots.connect(window.get_ic().get_mouse().sig_key_up(), clan::bind_member(this, &WindowView_Impl::on_mouse_up));
		slots.connect(window.get_ic().get_mouse().sig_pointer_move(), clan::bind_member(this, &WindowView_Impl::on_mouse_move));
	}

	void WindowView_Impl::on_lost_focus()
	{
		release_capture();

		ActivationChangeEvent e(ActivationChangeType::deactivated);
		window_view->dispatch_event(&e);
	}

	void WindowView_Impl::on_got_focus()
	{
		release_capture();
		ActivationChangeEvent e(ActivationChangeType::activated);
		window_view->dispatch_event(&e);
	}

	void WindowView_Impl::on_resize(float, float)
	{
		window.request_repaint(window.get_viewport());
	}

	void WindowView_Impl::on_paint(const clan::Rect &box)
	{
		canvas.clear(clan::Colorf::transparent);

		window_view->set_geometry(ViewGeometry::from_margin_box(window_view->style_cascade(), window.get_viewport()));
		window_view->layout(canvas);
		window_view->render(canvas);

		canvas.flush();
		window.flip();
	}

	void WindowView_Impl::on_window_close()
	{
		CloseEvent e;
		window_view->dispatch_event(&e);
	}

	void WindowView_Impl::window_key_event(KeyEvent &e)
	{
		View *view = window_view->focus_view();
		if (view)
		{
			view->dispatch_event(&e);
		}

		if (!e.default_prevented() && e.type() == KeyEventType::press && e.shift_down() && e.key() == Key::tab)
		{
			window_view->prev_focus();
		}
		else if (!e.default_prevented() && e.type() == KeyEventType::press && e.key() == Key::tab)
		{
			window_view->next_focus();
		}
	}

	void WindowView_Impl::dispatch_hot_event(std::shared_ptr<View> &view, PointerEvent &e)
	{
		if (view != hot_view)
		{
			if (hot_view)
			{
				PointerEvent e_exit(PointerEventType::leave, PointerButton::none, e.pos(window_view->root_view()), e.alt_down(), e.shift_down(), e.ctrl_down(), e.cmd_down());
				hot_view->dispatch_event(&e_exit, true);
			}

			hot_view = view;

			if (hot_view)
			{
				PointerEvent e_enter(PointerEventType::enter, PointerButton::none, e.pos(window_view->root_view()), e.alt_down(), e.shift_down(), e.ctrl_down(), e.cmd_down());
				hot_view->dispatch_event(&e_enter, true);
			}
		}

		if (hot_view)
			hot_view->update_cursor(window);

	}

	void WindowView_Impl::release_capture()
	{
		if (captured_view)
		{
			window.capture_mouse(false);
			captured_view.reset();
			capture_down_counter = 0;
		}
	}

	std::shared_ptr<View> WindowView_Impl::get_capture_view(PointerEvent &e, std::shared_ptr<View> &view_above_cursor)
	{
		if (e.type() == PointerEventType::press || e.type() == PointerEventType::double_click)
		{
			capture_down_counter++;
			if (capture_down_counter == 1)
			{
				captured_view = view_above_cursor;
				if (captured_view)
					window.capture_mouse(true);
			}
		}

		std::shared_ptr<View> view = captured_view;

		if (e.type() == PointerEventType::release)
		{
			if (capture_down_counter > 0)
			{
				capture_down_counter--;
				if (capture_down_counter == 0)
				{
					if (captured_view)
						release_capture();
				}
			}
		}
		return view;
	}

	void WindowView_Impl::window_pointer_event(PointerEvent &e)
	{
		std::shared_ptr<View> view_above_cursor = window_view->find_view_at(e.pos(window_view));
		auto view = get_capture_view(e, view_above_cursor);
		if (!view)
			view = view_above_cursor;

		dispatch_hot_event(view, e);

		if (e.type() == PointerEventType::enter || e.type() == PointerEventType::leave)		// Ignore window enter/leave events
			return;

		if (view)
			view->dispatch_event(&e);
		else
			window_view->dispatch_event(&e);
	}

	Pointf WindowView_Impl::to_root_pos(const Pointf &client_pos) const
	{
		return window_view->to_root_pos(client_pos - window_view->geometry().content_pos());
	}

	void WindowView_Impl::on_key_down(const clan::InputEvent &e)
	{
		KeyEventType type = KeyEventType::press;
		Key key = decode_ic(e.id);
		int repeat_count = e.repeat_count;
		const std::string text = e.str;
		const Pointf pointer_pos = to_root_pos(e.mouse_pos);
		bool alt_down = e.alt;
		bool shift_down = e.shift;
		bool ctrl_down = e.ctrl;
		bool cmd_down = false;
		KeyEvent key_event(type, key, repeat_count, text, pointer_pos, alt_down, shift_down, ctrl_down, cmd_down);
		window_key_event(key_event);
	}

	void WindowView_Impl::on_key_up(const clan::InputEvent &e)
	{
		KeyEventType type = KeyEventType::release;
		Key key = decode_ic(e.id);
		int repeat_count = e.repeat_count;
		const std::string text = e.str;
		const Pointf pointer_pos = to_root_pos(e.mouse_pos);
		bool alt_down = e.alt;
		bool shift_down = e.shift;
		bool ctrl_down = e.ctrl;
		bool cmd_down = false;
		KeyEvent key_event(type, key, repeat_count, text, pointer_pos, alt_down, shift_down, ctrl_down, cmd_down);
		window_key_event(key_event);
	}

	void WindowView_Impl::on_mouse_down(const clan::InputEvent &e)
	{
		PointerEventType type = PointerEventType::press;
		PointerButton button = decode_id(e.id);
		const Pointf pos = to_root_pos(e.mouse_pos);
		bool alt_down = e.alt;
		bool shift_down = e.shift;
		bool ctrl_down = e.ctrl;
		bool cmd_down = false;
		PointerEvent pointer_event(type, button, pos, alt_down, shift_down, ctrl_down, cmd_down);
		window_pointer_event(pointer_event);
	}

	void WindowView_Impl::on_mouse_dblclk(const clan::InputEvent &e)
	{
		PointerEventType type = PointerEventType::double_click;
		PointerButton button = decode_id(e.id);
		const Pointf pos = to_root_pos(e.mouse_pos);
		bool alt_down = e.alt;
		bool shift_down = e.shift;
		bool ctrl_down = e.ctrl;
		bool cmd_down = false;
		PointerEvent pointer_event(type, button, pos, alt_down, shift_down, ctrl_down, cmd_down);
		window_pointer_event(pointer_event);
	}

	void WindowView_Impl::on_mouse_up(const clan::InputEvent &e)
	{
		PointerEventType type = PointerEventType::release;
		PointerButton button = decode_id(e.id);
		const Pointf pos = to_root_pos(e.mouse_pos);
		bool alt_down = e.alt;
		bool shift_down = e.shift;
		bool ctrl_down = e.ctrl;
		bool cmd_down = false;
		PointerEvent pointer_event(type, button, pos, alt_down, shift_down, ctrl_down, cmd_down);
		window_pointer_event(pointer_event);
	}

	void WindowView_Impl::on_mouse_move(const clan::InputEvent &clan_event)
	{
		PointerEvent e(PointerEventType::move, PointerButton::none, to_root_pos(clan_event.mouse_pos), clan_event.alt, clan_event.shift, clan_event.ctrl, false/*clan_event.cmd*/);
		window_pointer_event(e);
	}

	PointerButton WindowView_Impl::decode_id(clan::InputCode ic) const
	{
		switch (ic)
		{
		default:
			return PointerButton::none;
		case clan::InputCode::mouse_left:
			return PointerButton::left;
		case clan::InputCode::mouse_right:
			return PointerButton::right;
		case clan::InputCode::mouse_middle:
			return PointerButton::middle;
		case clan::InputCode::mouse_wheel_up:
			return PointerButton::wheel_up;
		case clan::InputCode::mouse_wheel_down:
			return PointerButton::wheel_down;
		case clan::InputCode::mouse_xbutton1:
			return PointerButton::xbutton1;
		case clan::InputCode::mouse_xbutton2:
			return PointerButton::xbutton2;
			//case clan::InputCode::mouse_xbutton3:
			// return PointerButton::xbutton3;
			//case clan::InputCode::mouse_xbutton4:
			// return PointerButton::xbutton4;
			//case clan::InputCode::mouse_xbutton5:
			// return PointerButton::xbutton5;
			//case clan::InputCode::mouse_xbutton6:
			// return PointerButton::xbutton6;
		}
	}
}
