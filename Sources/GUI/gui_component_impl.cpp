/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
**    Harry Storbacka
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_component.h"
#include "gui_component_impl.h"
#include "gui_manager_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_GUIComponent_Impl Construction:

CL_GUIComponent_Impl::CL_GUIComponent_Impl(const CL_SharedPtr<CL_GUIManager_Impl> &init_gui_manager, CL_GUIComponent *parent_or_owner, bool toplevel)
: gui_manager(init_gui_manager), parent(0), prev_sibling(0), next_sibling(0), first_child(0), last_child(0), layout(0),
  focus_policy(CL_GUIComponent::focus_refuse), allow_resize(false), clip_children(false), enabled(true),
  visible(true), activated(false), click_through(false), is_tab_order_controller(false), 
  component_tab_index(-1), tab_order_controller_current_index(-1), tab_order_controller_last_index(-1),
  default_handler(false), cancel_handler(false), constant_repaint(false)
{
	gui_manager_impl = gui_manager.get();

	if (!toplevel)
		parent = parent_or_owner;
}

CL_GUIComponent_Impl *CL_GUIComponent_Impl::create_from_parent(CL_GUIComponent *parent)
{
	if (parent)
		return new CL_GUIComponent_Impl(parent->get_gui_manager().impl, parent, false);
	else
		throw CL_Exception(cl_text("Cannot create child component with no parent!"));
}

CL_GUIComponent_Impl *CL_GUIComponent_Impl::create_from_manager(CL_GUIManager *manager)
{
	if (manager)
		return new CL_GUIComponent_Impl(manager->impl, 0, true);
	else
		throw CL_Exception(cl_text("Cannot create component with a null manager!"));
}

CL_GUIComponent_Impl *CL_GUIComponent_Impl::create_from_owner(CL_GUIComponent *owner)
{
	if (owner)
		return new CL_GUIComponent_Impl(owner->get_gui_manager().impl, owner, true);
	else
		throw CL_Exception(cl_text("Cannot create child component with a null owner!"));
}

CL_GUIComponent_Impl::~CL_GUIComponent_Impl()
{
	while (last_child)
		delete last_child;

	if (prev_sibling)
		prev_sibling->impl->next_sibling = next_sibling;
	if (next_sibling)
		next_sibling->impl->prev_sibling = prev_sibling;
	
	if (prev_sibling == 0 && parent)
		parent->impl->first_child = next_sibling;
	if (next_sibling == 0 && parent)
		parent->impl->last_child = prev_sibling;

	gui_manager_impl->remove_component(this);

	delete layout;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GUIComponent_Impl Attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_GUIComponent_Impl Operations:

/////////////////////////////////////////////////////////////////////////////
// CL_GUIComponent_Impl Implementation:
