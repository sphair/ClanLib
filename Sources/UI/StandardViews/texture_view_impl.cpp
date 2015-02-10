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
#include "API/UI/StandardViews/texture_view.h"
#include "API/UI/Events/key_event.h"
#include "API/UI/Events/pointer_event.h"
#include "API/UI/Events/close_event.h"
#include "API/UI/Events/activation_change_event.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/input_context.h"
#include "API/Display/2D/canvas.h"
#include "texture_view_impl.h"

namespace clan
{

	TextureView_Impl::TextureView_Impl(TextureView *view, Canvas &canvas) : window_view(view), canvas(canvas)
	{
		canvas_rect = canvas.get_size();
	}

	void TextureView_Impl::set_event_window(const DisplayWindow &new_event_window, const Mat4f &new_transform_mouse_matrix)
	{
		slots = SlotContainer();
		event_window = new_event_window;
		transform_mouse_matrix = new_transform_mouse_matrix;
		if (!event_window.is_null())
		{
			slots.connect(event_window.sig_lost_focus(), clan::bind_member(this, &TextureView_Impl::on_lost_focus));
			slots.connect(event_window.sig_got_focus(), clan::bind_member(this, &TextureView_Impl::on_got_focus));
			slots.connect(event_window.sig_window_close(), clan::bind_member(this, &TextureView_Impl::on_window_close));
			slots.connect(event_window.get_ic().get_keyboard().sig_key_down(), clan::bind_member(this, &TextureView_Impl::transform_on_key_down));
			slots.connect(event_window.get_ic().get_keyboard().sig_key_up(), clan::bind_member(this, &TextureView_Impl::transform_on_key_up));
			slots.connect(event_window.get_ic().get_mouse().sig_key_down(), clan::bind_member(this, &TextureView_Impl::transform_on_mouse_down));
			slots.connect(event_window.get_ic().get_mouse().sig_key_dblclk(), clan::bind_member(this, &TextureView_Impl::transform_on_mouse_dblclk));
			slots.connect(event_window.get_ic().get_mouse().sig_key_up(), clan::bind_member(this, &TextureView_Impl::transform_on_mouse_up));
			slots.connect(event_window.get_ic().get_mouse().sig_pointer_move(), clan::bind_member(this, &TextureView_Impl::transform_on_mouse_move));
		}
	}

	void TextureView_Impl::update()
	{
		if (needs_render)
		{
			canvas.set_cliprect(canvas_rect);
			canvas.clear(clan::Colorf::transparent);

			needs_render = false;
			window_view->set_geometry(BoxGeometry::from_margin_box(window_view->box_style, canvas_rect));
			window_view->layout(canvas);
			window_view->render(canvas);
			canvas.reset_cliprect();
		}
	}

	void TextureView_Impl::on_lost_focus()
	{
		capture_down_counter = 0;
		captured_view.reset();

		ActivationChangeEvent e(ActivationChangeType::deactivated);
		window_view->dispatch_event(&e);
	}

	void TextureView_Impl::on_got_focus()
	{
		ActivationChangeEvent e(ActivationChangeType::activated);
		window_view->dispatch_event(&e);
	}

	void TextureView_Impl::on_window_close()
	{
		CloseEvent e;
		window_view->dispatch_event(&e);
	}

	void TextureView_Impl::window_key_event(KeyEvent &e)
	{
		View *view = window_view->focus_view();
		if (view)
		{
			view->dispatch_event(&e);
		}

		if (!e.default_prevented() && e.type() == KeyEventType::press && e.shift_down() && e.key() == Key::tab)
		{
			window_view->root_view()->prev_focus();
		}
		else if (!e.default_prevented() && e.type() == KeyEventType::press && e.key() == Key::tab)
		{
			window_view->root_view()->next_focus();
		}
	}

	void TextureView_Impl::window_pointer_event(PointerEvent &e_window)
	{
		PointerEvent e = e_window;
		e.set_pos(window_view, e.pos(window_view) - window_view->geometry().content.get_top_left());

		std::shared_ptr<View> view_above_cursor = window_view->find_view_at(e.pos(window_view));

		if (view_above_cursor != hot_view)
		{
			if (hot_view)
			{
				PointerEvent e_exit(PointerEventType::leave, PointerButton::none, e.pos(window_view->root_view()), e.alt_down(), e.shift_down(), e.ctrl_down(), e.cmd_down());
				hot_view->dispatch_event(&e_exit, true);
			}

			hot_view = view_above_cursor;

			if (hot_view)
			{
				PointerEvent e_enter(PointerEventType::enter, PointerButton::none, e.pos(window_view->root_view()), e.alt_down(), e.shift_down(), e.ctrl_down(), e.cmd_down());
				hot_view->dispatch_event(&e_enter, true);

				if (!cursor_window.is_null())
					hot_view->update_cursor(cursor_window);
			}
		}

		if (e.type() == PointerEventType::enter || e.type() == PointerEventType::leave)
			return;

		if (e.type() == PointerEventType::press || e.type() == PointerEventType::double_click)
		{
			// To do: use flags for each mouse key rather than a counter - it is safer in case a release event is never sent
			capture_down_counter++;
			if (capture_down_counter == 1)
				captured_view = view_above_cursor;
		}

		std::shared_ptr<View> view = captured_view ? captured_view : view_above_cursor;
		if (view)
			view->dispatch_event(&e);
		else
			window_view->dispatch_event(&e);

		if (e.type() == PointerEventType::release)
		{
			if (capture_down_counter > 0)
			{
				capture_down_counter--;
				if (capture_down_counter == 0)
					captured_view.reset();
			}
		}

	}

	void TextureView_Impl::on_key_down(const clan::InputEvent &e)
	{
		KeyEventType type = KeyEventType::press;
		Key key = decode_ic(e.id);
		int repeat_count = e.repeat_count;
		const std::string text = e.str;
		const Pointf pointer_pos = e.mouse_pos;
		bool alt_down = e.alt;
		bool shift_down = e.shift;
		bool ctrl_down = e.ctrl;
		bool cmd_down = false;
		KeyEvent key_event(type, key, repeat_count, text, pointer_pos, alt_down, shift_down, ctrl_down, cmd_down);
		window_key_event(key_event);
	}

	void TextureView_Impl::on_key_up(const clan::InputEvent &e)
	{
		KeyEventType type = KeyEventType::release;
		Key key = decode_ic(e.id);
		int repeat_count = e.repeat_count;
		const std::string text = e.str;
		const Pointf pointer_pos = e.mouse_pos;
		bool alt_down = e.alt;
		bool shift_down = e.shift;
		bool ctrl_down = e.ctrl;
		bool cmd_down = false;
		KeyEvent key_event(type, key, repeat_count, text, pointer_pos, alt_down, shift_down, ctrl_down, cmd_down);
		window_key_event(key_event);
	}

	void TextureView_Impl::on_mouse_down(const clan::InputEvent &e)
	{
		PointerEventType type = PointerEventType::press;
		PointerButton button = decode_id(e.id);
		const Pointf pos = e.mouse_pos;
		bool alt_down = e.alt;
		bool shift_down = e.shift;
		bool ctrl_down = e.ctrl;
		bool cmd_down = false;
		PointerEvent pointer_event(type, button, pos, alt_down, shift_down, ctrl_down, cmd_down);
		window_pointer_event(pointer_event);
	}

	void TextureView_Impl::on_mouse_dblclk(const clan::InputEvent &e)
	{
		PointerEventType type = PointerEventType::double_click;
		PointerButton button = decode_id(e.id);
		const Pointf pos = e.mouse_pos;
		bool alt_down = e.alt;
		bool shift_down = e.shift;
		bool ctrl_down = e.ctrl;
		bool cmd_down = false;
		PointerEvent pointer_event(type, button, pos, alt_down, shift_down, ctrl_down, cmd_down);
		window_pointer_event(pointer_event);
	}

	void TextureView_Impl::on_mouse_up(const clan::InputEvent &e)
	{
		PointerEventType type = PointerEventType::release;
		PointerButton button = decode_id(e.id);
		const Pointf pos = e.mouse_pos;
		bool alt_down = e.alt;
		bool shift_down = e.shift;
		bool ctrl_down = e.ctrl;
		bool cmd_down = false;
		PointerEvent pointer_event(type, button, pos, alt_down, shift_down, ctrl_down, cmd_down);
		window_pointer_event(pointer_event);
	}

	void TextureView_Impl::on_mouse_move(const clan::InputEvent &clan_event)
	{
		PointerEvent e(PointerEventType::move, PointerButton::none, clan_event.mouse_pos, clan_event.alt, clan_event.shift, clan_event.ctrl, false/*clan_event.cmd*/);
		window_pointer_event(e);
	}

	PointerButton TextureView_Impl::decode_id(clan::InputCode ic) const
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
		}
	}

	void TextureView_Impl::transform_on_key_down(const clan::InputEvent &e)
	{
		on_key_down(transform_input_event(e));
	}

	void TextureView_Impl::transform_on_key_up(const clan::InputEvent &e)
	{
		on_key_up(transform_input_event(e));
	}

	void TextureView_Impl::transform_on_mouse_down(const clan::InputEvent &e)
	{
		on_mouse_down(transform_input_event(e));
	}

	void TextureView_Impl::transform_on_mouse_dblclk(const clan::InputEvent &e)
	{
		on_mouse_dblclk(transform_input_event(e));
	}

	void TextureView_Impl::transform_on_mouse_up(const clan::InputEvent &e)
	{
		on_mouse_up(transform_input_event(e));
	}

	void TextureView_Impl::transform_on_mouse_move(const clan::InputEvent &clan_event)
	{
		on_mouse_move(transform_input_event(clan_event));
	}
}
