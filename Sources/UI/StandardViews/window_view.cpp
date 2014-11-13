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
	WindowView::WindowView(const DisplayWindowDescription &desc) : impl(std::make_shared<WindowView_Impl>(desc))
	{
		slots.connect(impl->sig_size_changed, this, &WindowView::on_window_size_changed);
		slots.connect(impl->sig_render, this, &WindowView::on_window_render);
		slots.connect(impl->sig_key_event, this, &WindowView::on_window_key_event);
		slots.connect(impl->sig_pointer_event, this, &WindowView::on_window_pointer_event);
		slots.connect(impl->sig_close, this, &WindowView::on_window_close);
		slots.connect(impl->sig_activated, this, &WindowView::on_window_activated);
		slots.connect(impl->sig_deactivated, this, &WindowView::on_window_deactivated);
	}

	DisplayWindow WindowView::get_display_window()
	{
		return impl->window;
	}

	void WindowView::show(WindowShowType type)
	{
		switch (type)
		{
		case WindowShowType::hide:
			impl->window.hide();
			break;
		case WindowShowType::show:
		case WindowShowType::normal:
		case WindowShowType::restore:
		case WindowShowType::show_default:
			impl->window.show();
			break;
		case WindowShowType::show_no_activate:
			impl->window.show(false);
			break;
		case WindowShowType::maximized:
			impl->window.maximize();
			impl->window.show();
			break;
		case WindowShowType::maximize:
			impl->window.show();
			impl->window.maximize();
			break;
		case WindowShowType::minimized:
			impl->window.minimize();
			impl->window.show();
			break;
		case WindowShowType::minimize:
			impl->window.show();
			impl->window.minimize();
			break;
		case WindowShowType::minimize_no_activate:
			impl->window.show(false);
			impl->window.minimize();
			break;
		}
	}

	void WindowView::hide()
	{
		impl->window.hide();
	}

	void WindowView::set_needs_render()
	{
		impl->window.request_repaint(impl->window.get_viewport());
	}

	void WindowView::on_window_size_changed()
	{
		impl->window.request_repaint(impl->window.get_viewport());
	}

	void WindowView::on_window_render(Canvas &canvas)
	{
		set_geometry(BoxGeometry::from_margin_box(box_style, impl->window.get_viewport()));
		layout(canvas);
		render(canvas);
	}

	void WindowView::on_window_key_event(KeyEvent &e)
	{
		View *view = focus_view();
		if (view)
		{
			view->dispatch_event(&e);
		}

		if (!e.default_prevented() && e.type() == KeyEventType::press && e.shift_down() && e.key() == Key::tab)
		{
			root_view()->prev_focus();
		}
		else if (!e.default_prevented() && e.type() == KeyEventType::press && e.key() == Key::tab)
		{
			root_view()->next_focus();
		}
	}

	void WindowView::on_window_pointer_event(PointerEvent &e_window)
	{
		PointerEvent e = e_window;
		e.set_pos(e.pos() - geometry().content.get_top_left());

		std::shared_ptr<View> view_above_cursor = find_view_at(e.pos());

		if (view_above_cursor != impl->hot_view)
		{
			if (impl->hot_view)
			{
				PointerEvent e_exit(PointerEventType::leave, PointerButton::none, e.pos(), e.alt_down(), e.shift_down(), e.ctrl_down(), e.cmd_down());
				impl->hot_view->dispatch_event(&e_exit, true);
			}

			impl->hot_view = view_above_cursor;

			if (impl->hot_view)
			{
				PointerEvent e_enter(PointerEventType::enter, PointerButton::none, e.pos(), e.alt_down(), e.shift_down(), e.ctrl_down(), e.cmd_down());
				impl->hot_view->dispatch_event(&e_enter, true);
			}
		}

		if (e.type() == PointerEventType::enter || e.type() == PointerEventType::leave)
			return;

		if (e.type() == PointerEventType::press || e.type() == PointerEventType::double_click)
		{
			// To do: use flags for each mouse key rather than a counter - it is safer in case a release event is never sent
			impl->capture_down_counter++;
			if (impl->capture_down_counter == 1)
				impl->captured_view = view_above_cursor;
		}

		std::shared_ptr<View> view = impl->captured_view ? impl->captured_view : view_above_cursor;
		if (view)
			view->dispatch_event(&e);
		else
			dispatch_event(&e);

		if (e.type() == PointerEventType::release)
		{
			impl->capture_down_counter--;
			if (impl->capture_down_counter == 0)
				impl->captured_view.reset();
		}
	}

	void WindowView::on_window_close()
	{
		CloseEvent e;
		dispatch_event(&e);
	}

	void WindowView::on_window_activated()
	{
		ActivationChangeEvent e(ActivationChangeType::activated);
		dispatch_event(&e);
	}

	void WindowView::on_window_deactivated()
	{
		ActivationChangeEvent e(ActivationChangeType::deactivated);
		dispatch_event(&e);
	}

}
