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

#include "treeitem_generic.h"
#include "component_generic.h"
#include "API/GUI/stylemanager.h"
#include "API/GUI/treenode.h"
#include "API/GUI/treeview.h"
#include "API/GUI/checkbox.h"
#include "API/Display/surface.h"

const static std::string blank;

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_TreeItem::CL_TreeItem(
	CL_TreeNode *node,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(parent, style), 
  impl(NULL)
{
	impl = new CL_TreeItem_Generic(this, node);
	get_style_manager()->connect_styles("treeitem", this);
	find_preferred_size();
	show(false);
}

CL_TreeItem::~CL_TreeItem()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

const std::string &CL_TreeItem::get_text(int column) const
{
	if((unsigned int)column >= impl->items.size())
		return blank;

	return impl->items[column].text;
}

CL_Component *CL_TreeItem::get_component(int column) const
{
	if((unsigned int)column >= impl->items.size())
		return NULL;

	return impl->items[column].component;
}

CL_TreeNode *CL_TreeItem::get_node() const
{
	return impl->node;
}

CL_Surface *CL_TreeItem::get_icon() const
{
	return impl->sur_icon;
}

int CL_TreeItem::get_text_margin(void) const
{
	return impl->text_margin;
}

int CL_TreeItem::get_custom_height() const
{
	return impl->custom_height;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_TreeItem::set_text(int column, const std::string &text)
{
	if(!text.empty())
	{
		if(impl->items.size() <= (unsigned int)column)
		{
			for(unsigned int i = impl->items.size(); i <= (unsigned int)column; ++i)
				impl->items.push_back(CL_TreeItem_Generic::TreeItemStruct());
		}

		CL_TreeItem_Generic::TreeItemStruct item;
		item.text = text;
		impl->items[column] = item;
	}
}

CL_CheckBox *CL_TreeItem::set_checkbox(int column, const std::string &text)
{
	CL_CheckBox *checkbox = new CL_CheckBox(this);
	checkbox->set_event_passing(false);
	set_component(column, checkbox);
	return checkbox;
}

void CL_TreeItem::set_component(int column, CL_Component *component)
{
	if(impl->items.size() <= (unsigned int)column)
	{
		for(unsigned int i = impl->items.size(); i <= (unsigned int)column; ++i)
			impl->items.push_back(CL_TreeItem_Generic::TreeItemStruct());
	}

	CL_TreeItem_Generic::TreeItemStruct item;
	item.component = component;
	impl->items[column] = item;
}

void CL_TreeItem::set_icon(CL_Surface *surface, bool delete_surface)
{
	if(impl->sur_icon && impl->delete_sur_icon)
		delete impl->sur_icon;
	
	impl->sur_icon = surface;
	impl->delete_sur_icon = delete_surface;

	find_preferred_size();
	get_node()->get_treeview()->sig_resize()(0,0);
}

void CL_TreeItem::set_text_margin(int margin)
{
	impl->text_margin = margin;
}

void CL_TreeItem::set_custom_height(int height)
{
	impl->custom_height = height;
	find_preferred_size();
	get_node()->get_treeview()->sig_resize()(0,0);
}

/////////////////////////////////////////////////////////////////////////////
// Signals:

CL_Signal_v0 &CL_TreeItem::sig_clicked()
{
	return impl->sig_clicked;
}
