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
#include "API/GUI/listbox.h"
#include "API/GUI/stylemanager.h"
#include "API/Core/XML/dom_element.h"
#include "listbox_generic.h"
#include "component_generic.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_ListBox::CL_ListBox(CL_Component *parent, CL_StyleManager *style)
: CL_Component(parent, style), impl(0)
{
	impl = new CL_ListBox_Generic(this);
	get_style_manager()->connect_styles("listbox", this);
	find_preferred_size();
}

CL_ListBox::CL_ListBox(
	const CL_Rect &pos,
	CL_Component *parent,
	CL_StyleManager *style)
: CL_Component(pos, parent, style), impl(0)
{
	impl = new CL_ListBox_Generic(this);
	get_style_manager()->connect_styles("listbox", this);
}

CL_ListBox::~CL_ListBox()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

int CL_ListBox::get_count() const
{
	return impl->items.size();
}

std::vector<CL_ListItem *> &CL_ListBox::get_items() const
{
	return impl->items;
}


std::vector<std::string> CL_ListBox::get_selected_items() const
{
	return impl->get_selected_items();
}

const std::string &CL_ListBox::get_current_text() const
{
	return impl->get_current_text();
}

CL_ListItem *CL_ListBox::get_item(int index) const
{
	return impl->get_item(index);
}

int CL_ListBox::get_item(const CL_Point &pt)
{
	return impl->get_item(pt);
}

const std::string &CL_ListBox::get_text(int index) const
{
	return impl->get_text(index);
}

int CL_ListBox::get_current_item() const
{
	return impl->get_current_item();
}

bool CL_ListBox::is_selected(int index) const
{
	if(index < 0 || index > (int)impl->items.size())
		return false;
		
	return impl->items[index]->selected;
}

bool CL_ListBox::is_multi_selection() const
{
	return impl->multi_selection;
}

int CL_ListBox::get_item_height() const
{
	return impl->item_height;
}

int CL_ListBox::get_top_item() const
{
	return impl->scroll_offset;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

int CL_ListBox::insert_item(CL_ListItem *item, int index, bool delete_item)
{
	return impl->insert_item(item, index, delete_item);
}

int CL_ListBox::insert_item(const std::string &text, int index)
{
	return impl->insert_item(text, index);
}

void CL_ListBox::remove_item(int index)
{
	impl->remove_item(index);
}

void CL_ListBox::change_item(CL_ListItem *item, int index)
{
	impl->change_item(item, index);
}

void CL_ListBox::change_item(const std::string &text, int index)
{
	impl->change_item(text, index);
}

void CL_ListBox::set_current_item(int index)
{
	impl->set_current_item(index);
}

void CL_ListBox::set_selected(int index, bool select)
{
	impl->set_selected(index, select);
}

void CL_ListBox::clear_selection()
{
	impl->clear_selection();
}

void CL_ListBox::select_all(bool select)
{
	impl->select_all(select);
}

void CL_ListBox::invert_selection()
{
	impl->invert_selection();
}

void CL_ListBox::set_multi_selection(bool enable)
{
	impl->set_multi_selection(enable);
}

void CL_ListBox::set_item_height(int height)
{
	impl->set_item_height(height);
}

void CL_ListBox::set_top_item(int index)
{
	impl->set_top_item(index);
}

void CL_ListBox::sort(bool ascending)
{
	impl->sort(ascending);
}

void CL_ListBox::clear()
{
	impl->clear();
}

/////////////////////////////////////////////////////////////////////////////
// Signals:

CL_Signal_v1<int> &CL_ListBox::sig_highlighted()
{
	return impl->sig_highlighted;
}

CL_Signal_v1<int> &CL_ListBox::sig_activated()
{
	return impl->sig_activated;
}

CL_Signal_v0 &CL_ListBox::sig_selection_changed()
{
	return impl->sig_selection_changed;
}

CL_Signal_v1<int> &CL_ListBox::sig_item_added()
{
	return impl->sig_item_added;
}

CL_Signal_v1<int> &CL_ListBox::sig_item_removed()
{
	return impl->sig_item_removed;
}

CL_Signal_v0 &CL_ListBox::sig_clear()
{
	return impl->sig_clear;
}
