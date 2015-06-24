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
#include "API/Display/Window/display_window_description.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/input_context.h"
#include "API/Display/2D/canvas.h"
#include "UI/View/positioned_layout.h"
#include "window_view_impl.h"

namespace clan
{
	WindowView::WindowView(const DisplayWindowDescription &desc) : impl(std::make_shared<WindowView_Impl>(this, desc))
	{
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

	Canvas WindowView::get_root_canvas() const
	{
		return impl->canvas;
	}

	bool WindowView::root_hidden() const
	{
		return !impl->window.is_visible();
	}

	void WindowView::set_root_hidden(bool value)
	{
		if (hidden() != value)
		{
			set_needs_layout();
			set_needs_render();
			show(value ? WindowShowType::hide : WindowShowType::show);
		}
	}

	void WindowView::set_root_needs_render()
	{
		impl->window.request_repaint(impl->window.get_viewport());
	}

	void WindowView::layout_root()
	{
		Canvas canvas(impl->window);
		Rectf containing_box = superview()->geometry().content_box();
		ViewGeometry geometry = PositionedLayout::get_geometry(canvas, this, containing_box);
		set_geometry(ViewGeometry::from_margin_box(style_cascade(), impl->window.get_viewport()));
		layout(canvas);

		Pointf screen_pos = superview()->to_screen_pos(geometry.margin_box().get_top_left());
		Sizef screen_size = geometry.margin_box().get_size();
		impl->window.set_position(Rectf(screen_pos, screen_size), false);
	}

	Pointf WindowView::root_to_screen_pos(const Pointf &pos)
	{
		Pointf client_pos(geometry().content_box().get_top_left() + pos);
		return Pointf(impl->window.client_to_screen(client_pos));
	}

	Pointf WindowView::root_from_screen_pos(const Pointf &pos)
	{
		Pointf client_pos = impl->window.screen_to_client(Pointf(pos));
		return Pointf(client_pos) - geometry().content_box().get_top_left();
	}

	void WindowView::root_present_popup(const Pointf &pos, const std::shared_ptr<View> &popup)
	{
		Pointf screen_pos = to_screen_pos(pos);

		DisplayWindowDescription desc;
		desc.set_type(WindowType::popup);
		desc.set_visible(false);
		desc.set_topmost(true);
		desc.set_no_activate(true);
		desc.show_caption(false);

		auto popup_window = std::make_shared<WindowView>(desc);
		popup_window->impl->popup_owner = this;
		popup_window->add_subview(popup);
		popup_window->style()->set("flex-direction: row");

		impl->popups[popup_window.get()] = popup_window;

		Canvas canvas = popup_window->get_canvas();
		float width = popup_window->calculate_preferred_width(canvas);
		float height = popup_window->calculate_preferred_height(canvas, width);
		popup_window->get_display_window().set_position(Rectf(screen_pos.x, screen_pos.y, screen_pos.x + width, screen_pos.y + height), false);

		popup_window->show(WindowShowType::show_no_activate);
	}

	void WindowView::root_dismiss_popup()
	{
		if (impl->popup_owner)
		{
			WindowView *popup_owner = impl->popup_owner;
			auto it = popup_owner->impl->popups.find(this);
			if (it != popup_owner->impl->popups.end())
				popup_owner->impl->popups.erase(it);
		}
	}
}
