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

	Canvas WindowView::get_canvas() const
	{
		return impl->canvas;
	}

	bool WindowView::hidden() const
	{
		return !impl->window.is_visible();
	}

	void WindowView::set_hidden(bool value)
	{
		if (hidden() != value)
		{
			set_needs_layout();
			set_needs_render();
			show(value ? WindowShowType::hide : WindowShowType::show);
		}
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

	bool WindowView::local_root()
	{
		return true;
	}

	void WindowView::layout_local()
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

	Pointf WindowView::to_screen_pos(const Pointf &pos)
	{
		Pointf client_pos(geometry().content_box().get_top_left() + pos);
		return Pointf(impl->window.client_to_screen(client_pos));
	}

	Pointf WindowView::from_screen_pos(const Pointf &pos)
	{
		Pointf client_pos = impl->window.screen_to_client(Pointf(pos));
		return Pointf(client_pos) - geometry().content_box().get_top_left();
	}
}
