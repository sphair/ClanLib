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
#include "API/UI/TopLevel/view_tree.h"
#include "API/UI/ViewController/view_controller.h"
#include "API/UI/Events/event.h"
#include "API/UI/Events/focus_change_event.h"
#include "../View/view_impl.h"
#include "../View/positioned_layout.h"
#include <algorithm>

namespace clan
{
	class ViewTreeImpl
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

		View *focus_view = nullptr;
		std::shared_ptr<ViewController> root;
	};

	ViewTree::ViewTree() : impl(new ViewTreeImpl)
	{
	}

	ViewTree::~ViewTree()
	{
	}

	View *ViewTree::focus_view() const
	{
		return impl->focus_view;
	}

	const std::shared_ptr<ViewController> &ViewTree::view_controller() const
	{
		return impl->root;
	}

	void ViewTree::set_view_controller(std::shared_ptr<ViewController> controller)
	{
		impl->root = controller;
	}

	void ViewTree::set_focus_view(View *new_focus_view)
	{
		View *old_focus_view = impl->focus_view;

		if (old_focus_view == new_focus_view)
			return;

		if (old_focus_view)
		{
			FocusChangeEvent focus_loss(FocusChangeType::lost);
			View::dispatch_event(old_focus_view, &focus_loss, true);
		}

		impl->focus_view = new_focus_view;

		if (new_focus_view)
		{
			FocusChangeEvent focus_gain(FocusChangeType::gained);
			View::dispatch_event(new_focus_view, &focus_gain, true);
		}
	}

	void ViewTree::render(Canvas &canvas, const Rectf &margin_box)
	{
		View *view = impl->root->view.get();

		view->set_geometry(ViewGeometry::from_margin_box(view->style_cascade(), margin_box));

		if (view->needs_layout())
		{
			view->layout_subviews(canvas);
			PositionedLayout::layout_subviews(canvas, view);
		}
		view->impl->needs_layout = false;

		view->impl->render(view, canvas, ViewRenderLayer::background);
		view->impl->render(view, canvas, ViewRenderLayer::border);
		view->impl->render(view, canvas, ViewRenderLayer::content);
	}

	void ViewTree::dispatch_activation_change(ActivationChangeType type)
	{
		ViewTreeImpl::dispatch_activation_change(impl->root->view.get(), type);
	}
}
