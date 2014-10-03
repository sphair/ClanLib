/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

	WindowView_Impl::WindowView_Impl(const DisplayWindowDescription &desc) : window(desc)
	{
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
		sig_deactivated();
	}

	void WindowView_Impl::on_got_focus()
	{
		sig_activated();
	}

	void WindowView_Impl::on_resize(int, int)
	{
		sig_size_changed();
	}

	void WindowView_Impl::on_paint(const clan::Rect &box)
	{
		Canvas canvas(window);

		canvas.clear(clan::Colorf::transparent);

		sig_render(canvas);

		canvas.flush();
		window.flip();
	}

	void WindowView_Impl::on_window_close()
	{
		sig_close();
	}

	void WindowView_Impl::on_key_down(const clan::InputEvent &e)
	{
		KeyEventType type = KeyEventType::press;
		Key key = (Key)e.id;
		int repeat_count = e.repeat_count;
		const std::string text = e.str;
		const Pointf pointer_pos = Pointf(static_cast<float>(e.mouse_pos.x), static_cast<float>(e.mouse_pos.y));
		bool alt_down = e.alt;
		bool shift_down = e.shift;
		bool ctrl_down = e.ctrl;
		bool cmd_down = false;
		KeyEvent key_event(type, key, repeat_count, text, pointer_pos, alt_down, shift_down, ctrl_down, cmd_down);
		sig_key_event(key_event);
	}

	void WindowView_Impl::on_key_up(const clan::InputEvent &e)
	{
		KeyEventType type = KeyEventType::release;
		Key key = (Key)e.id;
		int repeat_count = e.repeat_count;
		const std::string text = e.str;
		const Pointf pointer_pos = Pointf(static_cast<float>(e.mouse_pos.x), static_cast<float>(e.mouse_pos.y));
		bool alt_down = e.alt;
		bool shift_down = e.shift;
		bool ctrl_down = e.ctrl;
		bool cmd_down = false;
		KeyEvent key_event(type, key, repeat_count, text, pointer_pos, alt_down, shift_down, ctrl_down, cmd_down);
		sig_key_event(key_event);
	}

	void WindowView_Impl::on_mouse_down(const clan::InputEvent &e)
	{
		PointerEventType type = PointerEventType::press;
		PointerButton button = decode_id(e.id);
		const Pointf pos = Pointf(static_cast<float>(e.mouse_pos.x), static_cast<float>(e.mouse_pos.y));
		bool alt_down = e.alt;
		bool shift_down = e.shift;
		bool ctrl_down = e.ctrl;
		bool cmd_down = false;
		PointerEvent pointer_event(type, button, pos, alt_down, shift_down, ctrl_down, cmd_down);
		sig_pointer_event(pointer_event);
	}

	void WindowView_Impl::on_mouse_dblclk(const clan::InputEvent &e)
	{
		PointerEventType type = PointerEventType::double_click;
		PointerButton button = decode_id(e.id);
		const Pointf pos = Pointf(static_cast<float>(e.mouse_pos.x), static_cast<float>(e.mouse_pos.y));
		bool alt_down = e.alt;
		bool shift_down = e.shift;
		bool ctrl_down = e.ctrl;
		bool cmd_down = false;
		PointerEvent pointer_event(type, button, pos, alt_down, shift_down, ctrl_down, cmd_down);
		sig_pointer_event(pointer_event);
	}

	void WindowView_Impl::on_mouse_up(const clan::InputEvent &e)
	{
		PointerEventType type = PointerEventType::release;
		PointerButton button = decode_id(e.id);
		const Pointf pos = Pointf(static_cast<float>(e.mouse_pos.x), static_cast<float>(e.mouse_pos.y));
		bool alt_down = e.alt;
		bool shift_down = e.shift;
		bool ctrl_down = e.ctrl;
		bool cmd_down = false;
		PointerEvent pointer_event(type, button, pos, alt_down, shift_down, ctrl_down, cmd_down);
		sig_pointer_event(pointer_event);
	}

	void WindowView_Impl::on_mouse_move(const clan::InputEvent &clan_event)
	{
		PointerEvent e(PointerEventType::move, PointerButton::none, Pointf((float)clan_event.mouse_pos.x, (float)clan_event.mouse_pos.y), clan_event.alt, clan_event.shift, clan_event.ctrl, false/*clan_event.cmd*/);
		sig_pointer_event(e);
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
