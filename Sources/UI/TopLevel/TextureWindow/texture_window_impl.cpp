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
*/

#include "UI/precomp.h"
#include "API/UI/TopLevel/texture_window.h"
#include "API/UI/Events/key_event.h"
#include "API/UI/Events/pointer_event.h"
#include "API/UI/Events/close_event.h"
#include "API/UI/Events/activation_change_event.h"
#include "API/Display/Render/blend_state_description.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/2D/path.h"
#include "API/Display/2D/brush.h"
#include "texture_window_impl.h"

namespace clan
{

	TextureWindow_Impl::TextureWindow_Impl(TextureWindow *view, Canvas &canvas) : window_view(view), canvas(canvas)
	{
		canvas_rect = canvas.get_size();
		BlendStateDescription blend_desc;
		blend_desc.enable_blending(false);
		opaque_blend = BlendState(canvas, blend_desc);
	}

	void TextureWindow_Impl::set_window(const DisplayWindow &window, bool enable_automatic_events, const Mat4f &new_transform_mouse_matrix)
	{
		slots = SlotContainer();
		display_window = window;
		transform_mouse_matrix = new_transform_mouse_matrix;
		if (display_window && enable_automatic_events)
		{
			slots.connect(display_window.sig_lost_focus(), clan::bind_member(this, &TextureWindow_Impl::on_lost_focus));
			slots.connect(display_window.sig_got_focus(), clan::bind_member(this, &TextureWindow_Impl::on_got_focus));
			slots.connect(display_window.sig_window_close(), clan::bind_member(this, &TextureWindow_Impl::on_window_close));
			slots.connect(display_window.get_keyboard().sig_key_down(), clan::bind_member(this, &TextureWindow_Impl::transform_on_key_down));
			slots.connect(display_window.get_keyboard().sig_key_up(), clan::bind_member(this, &TextureWindow_Impl::transform_on_key_up));
			slots.connect(display_window.get_mouse().sig_key_down(), clan::bind_member(this, &TextureWindow_Impl::transform_on_mouse_down));
			slots.connect(display_window.get_mouse().sig_key_dblclk(), clan::bind_member(this, &TextureWindow_Impl::transform_on_mouse_dblclk));
			slots.connect(display_window.get_mouse().sig_key_up(), clan::bind_member(this, &TextureWindow_Impl::transform_on_mouse_up));
			slots.connect(display_window.get_mouse().sig_pointer_move(), clan::bind_member(this, &TextureWindow_Impl::transform_on_mouse_move));
		}
	}

	void TextureWindow_Impl::update()
	{
		if (needs_render || always_render)
		{
			ClipRectState cliprect_state(&canvas);
			canvas.set_cliprect(canvas_rect);

			if (clear_background_enable)
			{
				canvas.set_blend_state(opaque_blend);
				canvas.fill_rect(canvas_rect, background_color);
				canvas.reset_blend_state();
				//canvas.clear(background_color);	<--- On d3d, this clears the entire canvas - It does not recognise the cliprect
			}

			needs_render = false;
			window_view->render(canvas, canvas_rect);
		}
	}
	
	void TextureWindow_Impl::on_lost_focus()
	{
		release_capture();
		window_view->dispatch_activation_change(ActivationChangeType::deactivated);
	}

	void TextureWindow_Impl::on_got_focus()
	{
		release_capture();
		window_view->dispatch_activation_change(ActivationChangeType::activated);
	}

	void TextureWindow_Impl::on_window_close()
	{
		CloseEvent e;
		View::dispatch_event(window_view->root_view().get(), &e);
	}

	void TextureWindow_Impl::window_key_event(KeyEvent &e)
	{
		View *view = window_view->focus_view();
		if (view)
		{
			View::dispatch_event(view, &e);
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

	void TextureWindow_Impl::dispatch_hot_event(std::shared_ptr<View> &view, PointerEvent &e)
	{
		if (view != hot_view)
		{
			if (hot_view)
			{
				PointerEvent e_exit(PointerEventType::leave, PointerButton::none, e.pos(window_view->root_view()), e.alt_down(), e.shift_down(), e.ctrl_down(), e.cmd_down());
				View::dispatch_event(hot_view.get(), &e_exit, true);
			}

			hot_view = view;

			if (hot_view)
			{
				PointerEvent e_enter(PointerEventType::enter, PointerButton::none, e.pos(window_view->root_view()), e.alt_down(), e.shift_down(), e.ctrl_down(), e.cmd_down());
				View::dispatch_event(hot_view.get(), &e_enter, true);
			}
		}

		if (hot_view)
		{
			if (display_window)
				hot_view->update_cursor(display_window);
		}

		// Status changes are made fast redraw controls and we need to show it.
		needs_render = true;
	}

	void TextureWindow_Impl::release_capture()
	{
		if (captured_view)
		{
			if (display_window)
				display_window.capture_mouse(false);
			captured_view.reset();
			capture_down_counter = 0;
		}
	}

	std::shared_ptr<View> TextureWindow_Impl::get_capture_view(PointerEvent &e, std::shared_ptr<View> &view_above_cursor)
	{
		if (e.type() == PointerEventType::press || e.type() == PointerEventType::double_click)
		{
			capture_down_counter++;
			if (capture_down_counter == 1)
			{
				captured_view = view_above_cursor;
				if (captured_view)
				{
					if (display_window)
						display_window.capture_mouse(true);
				}
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

	void TextureWindow_Impl::window_pointer_event(PointerEvent &e_window)
	{
		PointerEvent e = e_window;
		Pointf pointer_pos = e.pos(window_view->root_view());
		pointer_pos.x -= window_view->root_view()->geometry().content_x;
		pointer_pos.y -= window_view->root_view()->geometry().content_y;
		e.set_pos(window_view->root_view().get(), pointer_pos);

		std::shared_ptr<View> view_above_cursor = window_view->root_view()->find_view_at(e.pos(window_view->root_view()));
		auto view = get_capture_view(e, view_above_cursor);
		if (!view)
			view = view_above_cursor;

		dispatch_hot_event(view, e);

		if (e.type() == PointerEventType::enter || e.type() == PointerEventType::leave)		// Ignore window enter/leave events
			return;

		if (view)
			View::dispatch_event(view.get(), &e);
		else
			View::dispatch_event(window_view->root_view().get(), &e);
	}

	void TextureWindow_Impl::on_key_down(const clan::InputEvent &e)
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

	void TextureWindow_Impl::on_key_up(const clan::InputEvent &e)
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

	void TextureWindow_Impl::on_mouse_down(const clan::InputEvent &e)
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

	void TextureWindow_Impl::on_mouse_dblclk(const clan::InputEvent &e)
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

	void TextureWindow_Impl::on_mouse_up(const clan::InputEvent &e)
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

	void TextureWindow_Impl::on_mouse_move(const clan::InputEvent &clan_event)
	{
		PointerEvent e(PointerEventType::move, PointerButton::none, clan_event.mouse_pos, clan_event.alt, clan_event.shift, clan_event.ctrl, false/*clan_event.cmd*/);
		window_pointer_event(e);
	}

	PointerButton TextureWindow_Impl::decode_id(clan::InputCode ic) const
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

	void TextureWindow_Impl::transform_on_key_down(const clan::InputEvent &e)
	{
		on_key_down(transform_input_event(e));
	}

	void TextureWindow_Impl::transform_on_key_up(const clan::InputEvent &e)
	{
		on_key_up(transform_input_event(e));
	}

	void TextureWindow_Impl::transform_on_mouse_down(const clan::InputEvent &e)
	{
		on_mouse_down(transform_input_event(e));
	}

	void TextureWindow_Impl::transform_on_mouse_dblclk(const clan::InputEvent &e)
	{
		on_mouse_dblclk(transform_input_event(e));
	}

	void TextureWindow_Impl::transform_on_mouse_up(const clan::InputEvent &e)
	{
		on_mouse_up(transform_input_event(e));
	}

	void TextureWindow_Impl::transform_on_mouse_move(const clan::InputEvent &clan_event)
	{
		on_mouse_move(transform_input_event(clan_event));
	}
}
