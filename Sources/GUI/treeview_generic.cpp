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
#include "treeview_generic.h"
#include "treenode_generic.h"
#include "API/GUI/gui_manager.h"
#include "API/Core/XML/dom_element.h"

const static std::string blank;

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_TreeView_Generic::CL_TreeView_Generic(CL_TreeView *self)
: show_root_decoration(true),
  show_header(true),
  treeview(self)
{
	// Create client area (which is the main treeview)
	CL_Component *client_area = new CL_Component(treeview);
	treeview->set_client_area(client_area);
	
	slot_paint_children = client_area->sig_paint_children().connect_virtual(
		this, &CL_TreeView_Generic::on_paint_children);
}

CL_TreeView_Generic::~CL_TreeView_Generic()
{
	delete treeview->get_client_area();
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

int CL_TreeView_Generic::get_column_count() const
{
	return columns.size();
}

int CL_TreeView_Generic::get_column_width(int index) const
{
	if(columns.size() == 0)
		return treeview->get_client_area()->get_width();

	if((unsigned int)index > columns.size())
		return 0;

	return columns[index].width;
}

const std::string &CL_TreeView_Generic::get_column_name(int index) const
{
	if((unsigned int)index > columns.size())
		return blank;

	return columns[index].name;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

int CL_TreeView_Generic::add_column(const std::string &name, int width)
{
	Column column;
	column.name = name;
	column.width = width;
	columns.push_back(column);
	return get_column_count() - 1;
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_TreeView_Generic::on_paint_children(CL_SlotParent_v0 &super)
{
//	CL_Point point(0, 0);
//	treeview->CL_TreeNode::impl->draw_nodes(point);
}
