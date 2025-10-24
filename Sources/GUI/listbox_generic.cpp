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

#include <algorithm>

#include "listbox_generic.h"
#include "API/GUI/listbox.h"
#include "API/GUI/listitem.h"
#include "API/GUI/component.h"
#include "API/GUI/stylemanager.h"
#include "API/Core/XML/dom_element.h"
#include "API/Display/keys.h"
#include "API/Core/System/error.h"

const static std::string blank;

inline bool list_item_ptr_less(CL_ListItem const * one, CL_ListItem const * two)
{
	return *one < *two;
}

inline bool list_item_ptr_greater(CL_ListItem const * one, CL_ListItem const * two)
{
	return *two < *one;
}

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_ListBox_Generic::CL_ListBox_Generic(CL_ListBox *self)
: listbox(self)
{
	scroll_offset = 0;
	multi_selection = false;

	// Create client area (which is the main listbox)
	CL_Component *client_area = new CL_Component(listbox);
	listbox->set_client_area(client_area);

	slots.connect(client_area->sig_mouse_dblclk(), this, &CL_ListBox_Generic::on_mouse_down);
	slots.connect(listbox->sig_set_options(), this, &CL_ListBox_Generic::on_set_options);
	slots.connect(client_area->sig_mouse_down(), this, &CL_ListBox_Generic::on_mouse_down);
	slots.connect(client_area->sig_key_up(), this, &CL_ListBox_Generic::on_key_up);
}

CL_ListBox_Generic::~CL_ListBox_Generic()
{
	std::vector<CL_ListItem *>::iterator it;
	for(it = items.begin(); it != items.end(); ++it)
		if((*it)->delete_item)
			delete (*it);

	delete listbox->get_client_area();
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

std::vector<std::string> CL_ListBox_Generic::get_selected_items() const
{
	std::vector<std::string> retval;
	std::vector<CL_ListItem *>::const_iterator it;
	
	for(it = items.begin(); it != items.end(); ++it)
		if((*it)->selected)
			retval.push_back((*it)->str);
	
	return retval;
}

const std::string &CL_ListBox_Generic::get_current_text() const
{
	int pos = 0;
	std::vector<CL_ListItem *>::const_iterator it;
	for(it = items.begin(); it != items.end(); ++it, ++pos)
		if((*it)->selected)
			return (*it)->str;

	return blank;
}

CL_ListItem *CL_ListBox_Generic::get_item(int index) const
{
	int pos = 0;
	std::vector<CL_ListItem *>::const_iterator it;
	for(it = items.begin(); it != items.end(); it++, pos++)
		if(pos == index)
			return (*it);

	return 0;
}

int CL_ListBox_Generic::get_item(const CL_Point &pt)
{
	int index = (int)(pt.y / item_height + scroll_offset);

	if(index < 0 || index >= (int)items.size()) return -1;

	//also check the x

	if (!listbox->get_children_rect().is_inside(pt)) return -1;
	return index;
}


const std::string &CL_ListBox_Generic::get_text(int index) const
{
	const CL_ListItem *item = get_item(index);
	if(item)
		return item->str;
	else
		return blank;
}

int CL_ListBox_Generic::get_current_item() const
{
	int pos = 0;
	std::vector<CL_ListItem *>::const_iterator it;
	for(it = items.begin(); it != items.end(); ++it, ++pos)
		if((*it)->selected)
			return pos;

	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

int CL_ListBox_Generic::insert_item(CL_ListItem *item, int index, bool delete_item)
{
	item->delete_item = delete_item;

	if(index < 0)
		items.push_back(item);
	else
		throw CL_Error("CL_ListBox::insert_item() using index is not implemented");

	sig_item_added(items.size() - 1);
	return items.size() - 1;
}

int CL_ListBox_Generic::insert_item(const std::string &text, int index)
{
	if(index < 0)
	{
		CL_ListItem *item = new CL_ListItem;
		item->str = text;
		item->delete_item = true;
		items.push_back(item);
	}
	else
		throw CL_Error("CL_ListBox::insert_item() using index is not implemented");

	sig_item_added(items.size() - 1);
	return items.size() - 1;
}

void CL_ListBox_Generic::remove_item(int index)
{
	if(index < 0 || index > (int)items.size())
		return;

	int pos = 0;
	std::vector<CL_ListItem *>::iterator it;
	for (it = items.begin(); it != items.end(); ++it, ++pos)
	{
		if(pos == index)
		{
			if((*it)->delete_item)
				delete (*it);
			items.erase(it);
			break;
		}
	}

	int max_visible_items = listbox->get_client_area()->get_height() / item_height;
	if((int)items.size() - scroll_offset < max_visible_items)
		scroll_offset = 0;

	sig_item_removed(index);
}

void CL_ListBox_Generic::change_item(CL_ListItem *item, int index)
{
	if(index < 0 || index >= (int)items.size())
		return;

	delete items[index];
	items[index] = item;
}

void CL_ListBox_Generic::change_item(const std::string &text, int index)
{
	items[index]->str = text;
}

void CL_ListBox_Generic::set_current_item(int index)
{
	if(index < 0 || index >= (int)items.size())
		return;

	std::vector<CL_ListItem *>::iterator it;
	for(it = items.begin(); it != items.end(); ++it)
		if((*it)->selected)
			(*it)->selected = false;
	items[index]->selected = true;
	
	sig_highlighted(index);
	sig_selection_changed();
}

void CL_ListBox_Generic::clear_selection()
{
	std::vector<CL_ListItem *>::iterator it;
	for(it = items.begin(); it != items.end(); ++it)
		(*it)->selected = false;
}

void CL_ListBox_Generic::sort(bool ascending)
{
	if (ascending)
		std::sort(items.begin(), items.end(), list_item_ptr_less);
	else
		std::sort(items.begin(), items.end(), list_item_ptr_greater);
}

void CL_ListBox_Generic::clear()
{
	std::vector<CL_ListItem *>::iterator it;
	for(it = items.begin(); it != items.end(); ++it)
		if((*it)->delete_item)
			delete (*it);

	items.clear();
	sig_clear();
}

void CL_ListBox_Generic::set_item_height(int new_item_height)
{
	item_height = new_item_height;
}

void CL_ListBox_Generic::set_top_item(int index)
{
	int max_visible_items = listbox->get_client_area()->get_height() / item_height;
	int maximum_offset = items.size() - max_visible_items;

	if (index < 0)
		index = 0;
	if (maximum_offset < 0)
		maximum_offset = 0;
	
	if (index >= maximum_offset)
		scroll_offset = maximum_offset;
	else
		scroll_offset = index;
}

void CL_ListBox_Generic::set_selected(int index, bool select)
{
	if(index < 0 || index >= (int)items.size())
		return;

	items[index]->selected = select;
}

void CL_ListBox_Generic::select_all(bool select)
{
	std::vector<CL_ListItem *>::iterator it;
	for(it = items.begin(); it != items.end(); ++it)
		(*it)->selected = select;
}

void CL_ListBox_Generic::invert_selection()
{
	std::vector<CL_ListItem *>::iterator it;
	for(it = items.begin(); it != items.end(); ++it)
		(*it)->selected = !((*it)->selected);
}

void CL_ListBox_Generic::set_multi_selection(bool enable)
{
	multi_selection = enable;
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_ListBox_Generic::on_set_options(const CL_DomElement &options)
{
	CL_DomNode node = options.get_first_child();
	while (!node.is_null())
	{
		if (node.is_element())
		{
			CL_DomElement element = node.to_element();
			if (element.get_tag_name() == "item")
			{
				CL_ListItem *item = new CL_ListItem;
				item->str = element.get_attribute("value");
				items.push_back(item);
			}
		}
		node = node.get_next_sibling();
	}
}

void CL_ListBox_Generic::on_mouse_down(const CL_InputEvent &key)
{
	if(items.size() == 0)
		return;

	if(key.id == CL_MOUSE_LEFT)
	{
		int index = (int)(key.mouse_pos.y / item_height + scroll_offset);

		if(index < 0 || index >= (int)items.size())
			return;

		if(multi_selection)
		{
			if (key.repeat_count != 2)
			items[index]->selected = !items[index]->selected;
		}
		else
		{
			std::vector<CL_ListItem *>::iterator it;
			for(it = items.begin(); it != items.end(); ++it)
				if((*it)->selected)
					(*it)->selected = false;
			items[index]->selected = true;
		}

		sig_highlighted(index);
		sig_selection_changed();
		if(key.repeat_count == 2)
			sig_activated(get_current_item());
	}
}

void CL_ListBox_Generic::on_key_up(const CL_InputEvent &key)
{
	if (key.id == CL_KEY_SPACE || key.id == CL_KEY_RETURN)
		sig_activated(get_current_item());
}
