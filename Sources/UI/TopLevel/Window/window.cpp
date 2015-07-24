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
#include "API/UI/TopLevel/window.h"
#include "API/UI/ViewController/view_controller.h"
#include "API/UI/Events/key_event.h"
#include "API/UI/Events/pointer_event.h"
#include "API/UI/Events/close_event.h"
#include "API/UI/Events/activation_change_event.h"
#include "API/Display/Window/display_window_description.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/input_context.h"
#include "API/Display/2D/canvas.h"
#include "UI/View/positioned_layout.h"
#include "window_impl.h"

namespace clan
{
	Window::Window(const DisplayWindowDescription &desc) : impl(std::make_shared<Window_Impl>(this, desc))
	{
	}

	DisplayWindow Window::get_display_window()
	{
		return impl->window;
	}

	void Window::show(WindowShowType type)
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

	void Window::hide()
	{
		impl->window.hide();
	}

	Canvas Window::get_canvas() const
	{
		return impl->canvas;
	}

	void Window::set_needs_render()
	{
		impl->window.request_repaint(impl->window.get_viewport());
	}

	Pointf Window::client_to_screen_pos(const Pointf &client_pos)
	{
		return Pointf(impl->window.client_to_screen(client_pos));
	}

	Pointf Window::screen_to_client_pos(const Pointf &pos)
	{
		return impl->window.screen_to_client(Pointf(pos));
	}

	void Window::present_popup(const Pointf &pos, const std::shared_ptr<ViewController> &popup)
	{
		Pointf screen_pos = client_to_screen_pos(pos);

		DisplayWindowDescription desc;
		desc.set_type(WindowType::popup);
		desc.set_visible(false);
		desc.set_topmost(true);
		desc.set_no_activate(true);
		desc.show_caption(false);

		auto popup_window = std::make_shared<Window>(desc);
		popup_window->impl->popup_owner = this;
		popup_window->set_view_controller(popup);

		impl->popups[popup_window.get()] = popup_window;

		Canvas canvas = popup_window->get_canvas();
		float width = popup_window->view_controller()->view->calculate_preferred_width(canvas);
		float height = popup_window->view_controller()->view->calculate_preferred_height(canvas, width);
		Rectf content_box(screen_pos.x, screen_pos.y, screen_pos.x + width, screen_pos.y + height);
		Rectf margin_box = ViewGeometry::from_content_box(popup_window->view_controller()->view->style_cascade(), content_box).margin_box();
		popup_window->get_display_window().set_position(margin_box, false);

		popup_window->show(WindowShowType::show_no_activate);
	}

	void Window::dismiss_popup()
	{
		if (impl->popup_owner)
		{
			Window *popup_owner = impl->popup_owner;
			auto it = popup_owner->impl->popups.find(this);
			if (it != popup_owner->impl->popups.end())
				popup_owner->impl->popups.erase(it);
		}
	}

	void Window::present_modal(const std::string &title, const std::shared_ptr<ViewController> &modal)
	{
		Pointf screen_pos = client_to_screen_pos(view_controller()->view->geometry().content_box().get_center());

		DisplayWindowDescription desc;
		desc.set_type(WindowType::normal);
		desc.set_visible(false);
		desc.set_owner_window(get_display_window());
		desc.set_title(title);
		desc.show_minimize_button(false);
		desc.show_maximize_button(false);
		// desc.show_sysmenu(false); // to do: fix that this also hides the close button

		auto modal_window = std::make_shared<Window>(desc);
		modal_window->impl->modal_owner = this;
		modal_window->set_view_controller(modal);

		impl->modal = modal_window;

		modal_window->impl->slots.connect(modal_window->get_display_window().sig_window_close(), [this]() { impl->modal.reset(); });

		Canvas canvas = modal_window->get_canvas();
		float width = modal_window->view_controller()->view->calculate_preferred_width(canvas);
		float height = modal_window->view_controller()->view->calculate_preferred_height(canvas, width);
		Rectf content_box(screen_pos.x - width * 0.5f, screen_pos.y - height * 0.5f, screen_pos.x + width * 0.5f, screen_pos.y + height * 0.5f);
		Rectf margin_box = ViewGeometry::from_content_box(modal_window->view_controller()->view->style_cascade(), content_box).margin_box();
		modal_window->get_display_window().set_position(margin_box, true);

		modal_window->show(WindowShowType::show);
	}

	void Window::dismiss_modal()
	{
		if (impl->modal_owner)
		{
			Window *modal_owner = impl->modal_owner;
			modal_owner->impl->modal.reset();
		}
	}
}
