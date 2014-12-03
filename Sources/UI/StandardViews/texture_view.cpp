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
#include "API/UI/StandardViews/texture_view.h"
#include "API/UI/Events/key_event.h"
#include "API/UI/Events/pointer_event.h"
#include "API/UI/Events/close_event.h"
#include "API/UI/Events/activation_change_event.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Window/input_context.h"
#include "API/Display/2D/canvas.h"
#include "UI/View/positioned_layout.h"
#include "texture_view_impl.h"

namespace clan
{
	TextureView::TextureView(Canvas &canvas) : impl(std::make_shared<TextureView_Impl>(this, canvas))
	{
	}

	void TextureView::set_needs_render()
	{
		impl->needs_render = true;
	}

	bool TextureView::local_root()
	{
		return true;
	}

	void TextureView::update()
	{
		if (impl->needs_render)
		{
			impl->canvas.clear(clan::Colorf::transparent);

			impl->needs_render = false;
			set_geometry(BoxGeometry::from_margin_box(box_style, impl->canvas.get_size()));
			layout(impl->canvas);
			render(impl->canvas);
		}
	}


	void TextureView::layout_local()
	{
		Rectf containing_box = superview()->geometry().content;
		BoxGeometry geometry = PositionedLayout::get_geometry(impl->canvas, this, containing_box);
		set_geometry(BoxGeometry::from_margin_box(box_style, impl->canvas.get_size()));
		layout(impl->canvas);

		Pointf screen_pos = superview()->to_screen_pos(geometry.margin_box().get_top_left());
		Sizef screen_size = geometry.margin_box().get_size();
		//impl->window.set_position(Rectf(screen_pos, screen_size), false);
	}

	Pointf TextureView::to_screen_pos(const Pointf &pos)
	{
		//Point client_pos = geometry().content_box().get_top_left() + pos;
		//return impl->window.client_to_screen(client_pos);
		return pos;
	}

	Pointf TextureView::from_screen_pos(const Pointf &pos)
	{
		//Point client_pos = impl->window.screen_to_client(Point(pos));
		//return Pointf(client_pos) - geometry().content_box().get_top_left();
		return pos;
	}
}
