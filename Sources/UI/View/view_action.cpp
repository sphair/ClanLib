/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
#include "API/UI/View/view_action.h"
#include "API/UI/View/view.h"
#include "API/UI/TopLevel/view_tree.h"
#include "API/UI/Events/event.h"
#include "API/UI/Events/activation_change_event.h"
#include "API/UI/Events/close_event.h"
#include "API/UI/Events/focus_change_event.h"
#include "API/UI/Events/key_event.h"
#include "API/UI/Events/pointer_event.h"
#include "API/UI/Events/resize_event.h"
#include "API/Core/Text/string_help.h"
#include "view_action_impl.h"
#include "view_impl.h"
#include <algorithm>

namespace clan
{
	ViewAction::ViewAction() : impl(new ViewActionImpl())
	{
	}

	ViewAction::~ViewAction()
	{
		remove_from_view();
	}

	View *ViewAction::view() const
	{
		return impl->view;
	}

	void ViewAction::remove_from_view()
	{
		if (impl->view)
		{
			impl->view->impl->_actions.erase(std::find(impl->view->impl->_actions.begin(), impl->view->impl->_actions.end(), shared_from_this()));
			if (impl->view->impl->_active_action == this)
				impl->view->impl->_active_action = nullptr;
		}
	}

	bool ViewAction::action_active() const
	{
		return impl->view && impl->view->impl->_active_action == this;
	}

	void ViewAction::begin_action()
	{
		if (impl->view)
		{
			impl->view->impl->_active_action = this;
		}
	}

	void ViewAction::end_action()
	{
		if (impl->view && impl->view->impl->_active_action == this)
		{
			impl->view->impl->_active_action = nullptr;
		}
	}
}
