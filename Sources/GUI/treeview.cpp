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

#ifdef _MSC_VER
#pragma warning (disable:4355)
#endif

#include "API/GUI/stylemanager.h"
#include "treeview_generic.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_TreeView::CL_TreeView(
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), CL_TreeNode(NULL, this), impl(NULL)
{
	impl = new CL_TreeView_Generic(this);
	get_style_manager()->connect_styles("treeview", this);
	find_preferred_size();
}

CL_TreeView::CL_TreeView(
	const CL_Rect &pos,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(pos, parent, style), CL_TreeNode(NULL, this), impl(NULL)
{
	impl = new CL_TreeView_Generic(this);
	get_style_manager()->connect_styles("treeview", this);
}

CL_TreeView::~CL_TreeView()
{
	clear();
	clear_delete_list();
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

int CL_TreeView::get_column_count() const
{
	return impl->get_column_count();
}

int CL_TreeView::get_column_width(int index) const
{
	return impl->get_column_width(index);
}

const std::string &CL_TreeView::get_column_name(int index) const
{
	return impl->get_column_name(index);
}

bool CL_TreeView::is_root_decoration_visible() const
{
	return impl->show_root_decoration;
}

bool CL_TreeView::is_header_visible() const
{
	return impl->show_header;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

int CL_TreeView::add_column(const std::string &name, int width)
{
	int index = impl->add_column(name, width);
	impl->sig_column_added(index);
	return index;
}

void CL_TreeView::show_root_decoration(bool enable)
{
	impl->show_root_decoration = enable;
}

void CL_TreeView::show_header(bool enable)
{
	impl->show_header = enable;
	sig_resize()(get_width(), get_height());
}

void CL_TreeView::add_node_to_delete_list(CL_TreeNode *pNode)
{
	//we'll delete it when we can
	node_delete_list.push_back(pNode);
}

void CL_TreeView::clear_delete_list()
{
	std::list<CL_TreeNode *>::iterator it;
	for (it = node_delete_list.begin(); it != node_delete_list.end(); ++it)
	{
		delete (*it);
	}

	node_delete_list.clear();
}


/////////////////////////////////////////////////////////////////////////////
// Signals:

CL_Signal_v1<const CL_TreeNode &> &CL_TreeView::sig_selection_changed()
{
	return impl->sig_selection_changed;
}

CL_Signal_v1<const CL_TreeNode &> &CL_TreeView::sig_item_clicked()
{
	return impl->sig_item_clicked;
}

CL_Signal_v2<CL_TreeNode *, CL_Point &> &CL_TreeView::sig_paint_node()
{
	return impl->sig_paint_node;
}

CL_Signal_v1<const CL_TreeNode &> &CL_TreeView::sig_item_added()
{
	return impl->sig_item_added;
}

CL_Signal_v1<const CL_TreeNode &> &CL_TreeView::sig_item_removed()
{
	return impl->sig_item_removed;
}

CL_Signal_v1<const CL_TreeNode &> &CL_TreeView::sig_item_expanded()
{
	return impl->sig_item_expanded;
}

CL_Signal_v1<const CL_TreeNode &> &CL_TreeView::sig_item_collapsed()
{
	return impl->sig_item_collapsed;
}

CL_Signal_v1<int> &CL_TreeView::sig_column_added()
{
	return impl->sig_column_added;
}

CL_Signal_v1<int> &CL_TreeView::sig_column_removed()
{
	return impl->sig_column_removed;
}

CL_Signal_v0 &CL_TreeView::sig_clear()
{
	return impl->sig_clear;
}
