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
#include "texture_view_impl.h"

namespace clan
{

	TextureView_Impl::TextureView_Impl(TextureView *view, Canvas &canvas) : window_view(view), canvas(canvas)
	{
		canvas_rect = canvas.get_size();
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
}
