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

#include "API/GUI/treeview.h"
#include "API/GUI/treeitem.h"
#include "API/GUI/treenode.h"
#include "treenode_generic.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_TreeNode::CL_TreeNode(CL_TreeNode *parent, CL_TreeView *root)
{
	impl = new CL_TreeNode_Generic(this, parent, root);
}

CL_TreeNode::~CL_TreeNode()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

void *CL_TreeNode::get_userdata() const
{
	return impl->userdata;
}

CL_Component *CL_TreeNode::get_component() const
{
	return impl->component;
}

bool CL_TreeNode::is_selectable() const
{
	switch(impl->selectable)
	{
	case 0:
		return false;
		break;
	case 1:
		return true;
		break;
	default:
		if(impl->parent)
            return impl->parent->is_selectable();
	}
	return true;
}

bool CL_TreeNode::is_collapsable() const
{
	switch(impl->collapsable)
	{
	case 0:
		return false;
		break;
	case 1:
		return true;
		break;
	default:
		if(impl->parent)
            return impl->parent->is_collapsable();
	}
	return true;
}

bool CL_TreeNode::is_selected() const
{
	return impl->selected;
}

bool CL_TreeNode::is_collapsed() const
{
	return impl->collapsed;
}

bool CL_TreeNode::has_children() const
{
	return (impl->children.empty() == false);
}

bool CL_TreeNode::is_root() const
{
	return impl->root_node;
}

CL_TreeNode *CL_TreeNode::get_current_item() const
{
	// Do a depth-first search for current selected item.
	std::list<CL_TreeNode *>::const_iterator it;
	for (it = impl->children.begin(); it != impl->children.end(); ++it)
	{
		if((*it)->impl->selected)
			return (*it);
		else 
		{
			CL_TreeNode *node = (*it)->get_current_item();
			if(node)
				return node;
		}
	}
	return NULL;
}

CL_TreeNode *CL_TreeNode::get_parent() const
{
	return impl->parent;
}

int CL_TreeNode::get_depth() const
{
	if(impl->parent == 0)
		return 0;
	else
		return impl->parent->get_depth() + 1;
}

CL_TreeView *CL_TreeNode::get_treeview() const
{
	return impl->root;
}

int CL_TreeNode::get_placement_offset() const
{
	return impl->placement_offset;
}

int CL_TreeNode::get_items_height() const
{
	int height = 0;
	if(impl->component && impl->component->is_enabled(false))
		height = impl->component->get_height();

	std::list<CL_TreeNode *>::iterator it;
	for (it = impl->children.begin(); it != impl->children.end(); ++it)
		height += (*it)->get_items_height();

	return height;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

CL_TreeItem *CL_TreeNode::insert_item(
	const std::string &label1, 
	const std::string &label2, 
	const std::string &label3, 
	const std::string &label4, 
	const std::string &label5, 
	const std::string &label6, 
	const std::string &label7, 
	const std::string &label8) 
{
	CL_TreeNode *node = new CL_TreeNode(this, impl->root);
	impl->children.push_back(node);

	if(impl->root == this)
		node->impl->root_node = true;

	CL_TreeItem *treeitem = new CL_TreeItem(node, impl->root->get_client_area());
	treeitem->set_text(0, label1);
	treeitem->set_text(1, label2);
	treeitem->set_text(2, label3);
	treeitem->set_text(3, label4);
	treeitem->set_text(4, label5);
	treeitem->set_text(5, label6);
	treeitem->set_text(6, label7);
	treeitem->set_text(7, label8);
	treeitem->find_preferred_size();

	node->set_component(treeitem);

	impl->root->sig_item_added()(*node);

	return treeitem;
}

CL_TreeNode *CL_TreeNode::insert_item(
	CL_Component *component1, 
	CL_Component *component2, 
	CL_Component *component3, 
	CL_Component *component4, 
	CL_Component *component5, 
	CL_Component *component6, 
	CL_Component *component7, 
	CL_Component *component8)
{
	CL_TreeNode *node = new CL_TreeNode(this, impl->root);
	impl->children.push_back(node);

	if(impl->root == this)
		node->impl->root_node = true;

	impl->root->get_client_area()->add_child(component1);

	node->set_component(component1);

	impl->root->sig_item_added()(*node);

	return node;
}

bool CL_TreeNode::remove_item(CL_TreeNode *node)
{
	std::list<CL_TreeNode *>::iterator it;

	// First check all children
	for(it = impl->children.begin(); it != impl->children.end(); ++it)
	{
		if((*it) == node)
		{
			impl->root->sig_item_removed()(*node);
			delete (*it);
			impl->children.erase(it);
			return true;
		}
	}

	// Then search all children
	for(it = impl->children.begin(); it != impl->children.end(); ++it)
		if((*it)->remove_item(node))
			return true;

	return false;
}

void CL_TreeNode::set_component(CL_Component *component)
{
	impl->set_component(component);
}

void CL_TreeNode::set_collapsed(bool collapsed)
{
	impl->set_collapsed(collapsed);
}
	
void CL_TreeNode::set_selected(bool select)
{
	impl->root->clear_selection();
	impl->selected = select;
	impl->root->sig_selection_changed()(*this);
//	impl->root->sig_item_clicked()(*this);
}

void CL_TreeNode::set_selectable(bool enable)
{
	impl->selectable = enable;
}

void CL_TreeNode::set_collapsable(bool enable)
{
	impl->collapsable = enable;
}

void CL_TreeNode::set_selected(CL_TreeNode *node, bool select)
{
	node->set_selected(select);
}

void CL_TreeNode::clear_selection()
{
	impl->selected = false;

	std::list<CL_TreeNode *>::iterator it;
	for (it = impl->children.begin(); it != impl->children.end(); ++it)
		(*it)->clear_selection();
}

void CL_TreeNode::clear()
{
	impl->clear(true);
}

void CL_TreeNode::set_userdata(void *data)
{
	impl->userdata = data;
}

void CL_TreeNode::set_placement_offset(int offset)
{
	impl->placement_offset = offset;
}

void CL_TreeNode::draw_nodes(CL_Point &point)
{
	std::list<CL_TreeNode *>::iterator it;

	// Draw all children
	for (it = impl->children.begin(); it != impl->children.end(); ++it)
	{
		int x = point.x;

		impl->root->sig_paint_node()(*it, point);
		if((*it)->is_collapsed() == false)
			(*it)->draw_nodes(point);

		point.x = x;
	}
}
