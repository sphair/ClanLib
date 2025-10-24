/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include "precomp.h"
#include "../API/GUI/treenode.h"
#include "../API/GUI/treeview.h"
#include "../API/Display/keys.h"
#include "treenode_generic.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_TreeNode_Generic::CL_TreeNode_Generic(CL_TreeNode *self, CL_TreeNode *parent, CL_TreeView *root)
{
	treenode = self;
	this->root = root;
	this->parent = parent;
	component = NULL;
	collapsed = false;
	selected = false;
	selectable = 2;
	collapsable = 2;
	userdata = NULL;
	root_node = false;
}

CL_TreeNode_Generic::~CL_TreeNode_Generic()
{
	destroy();
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_TreeNode_Generic::set_collapsed(bool new_collapsed)
{
	if(collapsed == new_collapsed || children.empty())
		return;
	collapsed = new_collapsed;

	if(collapsed)
	{
		show_nodes(false);
		root->sig_item_collapsed()(*treenode);
	}
	else
	{
		show_nodes(true);
		root->sig_item_expanded()(*treenode);
	}
}

void CL_TreeNode_Generic::show_nodes(bool show)
{
	std::list<CL_TreeNode *>::iterator it;
	for (it = children.begin(); it != children.end(); ++it)
	{
		(*it)->impl->component->enable(show);
		(*it)->impl->show_nodes(show);
	}
}

void CL_TreeNode_Generic::set_component(CL_Component *new_component)
{
	if(component)
	{
		component->sig_mouse_up().disconnect(slot_mouse_up);
		delete component;
	}

	component = new_component;
	slot_mouse_up = component->sig_mouse_up().connect(
		this, &CL_TreeNode_Generic::on_child_click);
}

void CL_TreeNode_Generic::destroy()
{
	if(component)
	{
		component->sig_mouse_up().disconnect(slot_mouse_up);
		delete component;
		component = NULL;
	}

	//let's schedule these for deletion ASAP, can't do it now because we might be in a
	//callback create by one of them (CL_FileDialog does this)

	std::list<CL_TreeNode *>::iterator it;
	for (it = children.begin(); it != children.end(); ++it)
	{
		root->add_node_to_delete_list(*it);
	}

	children.clear();
}

void CL_TreeNode_Generic::clear(bool do_signal)
{
	destroy();

	if(do_signal)
		root->sig_clear()();
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_TreeNode_Generic::on_child_click(const CL_InputEvent &key)
{
	if(key.id == CL_MOUSE_LEFT)
	{
		if(treenode->is_collapsable())
			treenode->set_collapsed(!collapsed);
		if(treenode->is_selectable())
			treenode->set_selected();
//		else
//			root->sig_item_clicked()(*treenode);
	}
}
