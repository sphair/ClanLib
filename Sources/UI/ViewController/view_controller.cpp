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
#include "API/UI/ViewController/view_controller.h"
#include "API/UI/View/view.h"
#include "API/UI/TopLevel/view_tree.h"
#include "view_controller_impl.h"
#include <algorithm>

namespace clan
{
	ViewController::ViewController() : view(std::make_shared<View>()), impl(new ViewControllerImpl())
	{
	}

	ViewController::~ViewController()
	{
	}

	ViewController *ViewController::parent_controller() const
	{
		return impl->parent_controller;
	}

	const std::vector<std::shared_ptr<ViewController>> &ViewController::child_controllers() const
	{
		return impl->child_controllers;
	}

	void ViewController::add_child_controller(const std::shared_ptr<ViewController> &controller)
	{
		if (controller)
		{
			impl->child_controllers.push_back(controller);
			controller->impl->parent_controller = this;

			child_controller_added(controller);
		}
	}

	void ViewController::remove_from_parent_controller()
	{
		ViewController *parent = impl->parent_controller;
		if (parent)
		{
			std::shared_ptr<ViewController> controller_ptr = shared_from_this();

			auto it = std::find_if(parent->impl->child_controllers.begin(), parent->impl->child_controllers.end(), [&](const std::shared_ptr<ViewController> &controller) { return controller.get() == this; });
			if (it != parent->impl->child_controllers.end())
				parent->impl->child_controllers.erase(it);
			impl->parent_controller = nullptr;

			child_controller_removed(controller_ptr);
		}
	}

	void ViewController::dismiss_popup()
	{
		view->view_tree()->dismiss_popup();
	}

	void ViewController::dismiss_modal()
	{
		view->view_tree()->dismiss_modal();
	}

	void ViewController::child_controller_added(const std::shared_ptr<ViewController> &view_controller)
	{
		if (view && view_controller->view)
		{
			view->add_subview(view_controller->view);
		}
	}

	void ViewController::child_controller_removed(const std::shared_ptr<ViewController> &view_controller)
	{
		if (view_controller->view)
		{
			view_controller->view->remove_from_super();
		}
	}
}
