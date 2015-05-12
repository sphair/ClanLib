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
#include "API/UI/View/root_view.h"
#include "API/UI/Events/event.h"
#include "view_impl.h"
#include "positioned_layout.h"
#include <algorithm>

namespace clan
{
	class RootViewImpl
	{
	public:
		static void dispatch_activation_change(View *view, ActivationChangeType type)
		{
			ActivationChangeEvent change(type);
			View::dispatch_event(view, &change, true);
			for (const auto &subview : view->subviews())
			{
				dispatch_activation_change(subview.get(), type);
			}
		}
	};

	void RootView::layout(Canvas &canvas)
	{
		if (needs_layout())
		{
			layout_subviews(canvas);
			PositionedLayout::layout_subviews(canvas, this);
		}
		impl->_needs_layout = false;
	}

	void RootView::render(Canvas &canvas)
	{
		impl->render(this, canvas, ViewRenderLayer::background);
		impl->render(this, canvas, ViewRenderLayer::border);
		impl->render(this, canvas, ViewRenderLayer::content);
	}

	void RootView::dispatch_activation_change(ActivationChangeType type)
	{
		RootViewImpl::dispatch_activation_change(this, type);
	}
}
