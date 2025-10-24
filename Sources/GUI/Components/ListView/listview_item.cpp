/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
**    Harry Storbacka
**    Magnus Norddahl
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_message_input.h"
#include "API/GUI/gui_message_pointer.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_component_description.h"
#include "API/Core/System/timer.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "API/GUI/Components/listview_item.h"
#include "API/GUI/Components/listview_column_data.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Font/font.h"
#include "API/Display/2D/sprite.h"
#include "API/Display/Window/keys.h"
#include "listview_item_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewItem Construction:

CL_ListViewItem::CL_ListViewItem()
{
}

CL_ListViewItem::CL_ListViewItem(const CL_SharedPtr<CL_ListViewItem_Impl> &impl)
: impl(impl)
{
}

CL_ListViewItem::~CL_ListViewItem()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewItem Attributes:

void CL_ListViewItem::throw_if_null() const
{
	if (!impl)
		throw CL_Exception("CL_ListViewItem is null");
}

bool CL_ListViewItem::is_item() const
{
	return impl ? true : false;
}

CL_ListViewColumnData CL_ListViewItem::get_column(const CL_StringRef &column_id)
{
	std::vector<CL_ListViewColumnData>::iterator it;
	for (it = impl->column_data.begin(); it != impl->column_data.end(); ++it)
	{
		if ((*it).get_column_id() == column_id)
			return (*it);
	}

	return CL_ListViewColumnData();
}

CL_ListViewItem CL_ListViewItem::get_parent()
{
	return CL_ListViewItem(impl->parent.lock());
}

CL_ListViewItem CL_ListViewItem::get_next_sibling()
{
	if (impl)
		return CL_ListViewItem(impl->next_sibling);

	return CL_ListViewItem();
}

CL_ListViewItem CL_ListViewItem::get_prev_sibling()
{
	if (impl)
		return CL_ListViewItem(impl->prev_sibling.lock());

	return CL_ListViewItem();
}

CL_ListViewItem CL_ListViewItem::get_first_child()
{
	if (impl)
		return CL_ListViewItem(impl->first_child);
	return CL_ListViewItem();
}

CL_ListViewItem CL_ListViewItem::get_last_child()
{
	if (impl)
		return CL_ListViewItem(impl->last_child.lock());
	return CL_ListViewItem();
}

bool CL_ListViewItem::is_open() const
{
	return impl->open;
}

bool CL_ListViewItem::is_editable() const
{
	return impl->editable;
}

bool CL_ListViewItem::is_selected() const
{
	return impl->selected;
}

bool CL_ListViewItem::has_children() const
{
	return impl->first_child ? true : false;;
}

int CL_ListViewItem::get_parent_count()
{
	int num_parents = 0;
	CL_ListViewItem i = get_parent();
	while (i.is_item())
	{
		num_parents++;
		i = i.get_parent();
	}

	return num_parents;
}

int CL_ListViewItem::get_child_count(bool recursive, bool recurse_only_into_open_items)
{
	int num_children = 0;
	CL_ListViewItem i = get_first_child();
	while (i.is_item())
	{
		num_children++;
		if (recursive && i.has_children())
		{
			if (recurse_only_into_open_items)
			{
				if (i.is_open())
					num_children += i.get_child_count(recursive, recurse_only_into_open_items);
			}
			else
			{
				num_children += i.get_child_count(recursive, recurse_only_into_open_items);
			}
		}
		i = i.get_next_sibling();
	}

	return num_children;
}

int CL_ListViewItem::get_id() const
{
	return impl->id;
}

int CL_ListViewItem::get_icon() const
{
	return impl->icon;
}

std::vector<int> CL_ListViewItem::get_overlay_icons()
{
	return impl->overlay_icons;
}

CL_SharedPtr<CL_ListViewItemUserData> CL_ListViewItem::get_userdata() const
{
	return impl->userdata;
}

bool CL_ListViewItem::operator==(CL_ListViewItem &other) const
{
	return impl.get() == other.impl.get();
}

bool CL_ListViewItem::operator!=(CL_ListViewItem &other) const
{
	return impl.get() != other.impl.get();
}


/////////////////////////////////////////////////////////////////////////////
// CL_ListViewItem Operations:

CL_ListViewItem CL_ListViewItem::append_child(CL_ListViewItem &item)
{
	if (impl)
	{
		item.impl->parent = impl;

		if (!impl->last_child.expired())
		{
			impl->last_child.lock()->next_sibling = item.impl;
			item.impl->prev_sibling = impl->last_child;
			impl->last_child = item.impl;		
		}
		else
		{
			impl->first_child = item.impl;		
			impl->last_child = item.impl;		
		}

		if (!impl->get_root_parent()->func_item_added.is_null())
			impl->get_root_parent()->func_item_added.invoke();

		return item;
	}

	return CL_ListViewItem();
}

CL_ListViewItem CL_ListViewItem::remove()
{
	CL_ListViewItem prev = get_prev_sibling();
	CL_ListViewItem next = get_next_sibling();
	CL_ListViewItem parent = get_parent();
	
	if (prev.is_item())
	{
		prev.impl->next_sibling = next.impl;

		if (next.is_item())
			next.impl->prev_sibling = prev.impl;
		else // no next item, update last_child of parent to point at prev.impl
			parent.impl->last_child = prev.impl;
	}
	else if (prev.is_null())
	{
		// delete first item by settin parents first item to next.impl
		parent.impl->first_child = next.impl;
	
		if (!next.is_null())
		{
			next.impl->prev_sibling = CL_SharedPtr<CL_ListViewItem_Impl>();
		}

		if (parent.impl->first_child)
		{
			if (!parent.impl->first_child->next_sibling)
				parent.impl->last_child = parent.impl->first_child;
		}
		else
		{
			// first_child = last_child = 0
			parent.impl->last_child = parent.impl->first_child;
		}
	}

	CL_ListViewItem document_item = get_document_item();

	if (!document_item.impl->func_item_deleted.is_null())
		document_item.impl->func_item_deleted.invoke(*this);
	
	return *this;
}

void CL_ListViewItem::remove_children()
{
	CL_SharedPtr<CL_ListViewItem_Impl> null_impl;
	impl->first_child = null_impl;
	impl->last_child = null_impl;

	if (!impl->func_item_deleted.is_null())
		impl->func_item_deleted.invoke(CL_ListViewItem());
}

void CL_ListViewItem::set_column_text(const CL_StringRef &column_id, const CL_StringRef &text)
{
	CL_ListViewItem document_item = get_document_item();

	std::vector<CL_ListViewColumnData>::iterator it;
	for (it = impl->column_data.begin(); it!=impl->column_data.end(); ++it)
	{
		if ((*it).get_column_id() == column_id)
		{
			(*it).set_text(text);
			if (!document_item.impl->func_item_modified.is_null())
				document_item.impl->func_item_modified.invoke(*this);
			return;
		}
	}

	CL_ListViewColumnData cd;
	cd.set_column_id(column_id);
	cd.set_text(text);
	impl->column_data.push_back(cd);

	if (!document_item.impl->func_item_modified.is_null())
		document_item.impl->func_item_modified.invoke(*this);
}

void CL_ListViewItem::set_open(bool open)
{
	impl->open = open;
}

void CL_ListViewItem::set_editable(bool editable)
{
	impl->editable = editable;
}

void CL_ListViewItem::set_icon(int icon_index)
{
	impl->icon = icon_index;
}

void CL_ListViewItem::add_overlay_icon(int icon_index)
{
	std::vector<int>::iterator it;
	for (it = impl->overlay_icons.begin(); it != impl->overlay_icons.end(); ++it)
	{
		if ((*it) == icon_index)
			return;
	}

	impl->overlay_icons.push_back(icon_index);
}

void CL_ListViewItem::remove_overlay_icon(int icon_index)
{
	std::vector<int>::iterator it;
	for (it = impl->overlay_icons.begin(); it != impl->overlay_icons.end(); ++it)
	{
		if ((*it) == icon_index)
		{
			it = impl->overlay_icons.erase(it);
			break;
		}
	}
}

void CL_ListViewItem::set_userdata(CL_SharedPtr<CL_ListViewItemUserData> ptr)
{
	impl->userdata = ptr;
}

CL_Callback_v1<CL_Rect> &CL_ListViewItem::func_render_icon()
{
	return impl->func_render_icon;
}

void CL_ListViewItem::set_id(int id)
{
	impl->id = id;
}

CL_ListViewItem CL_ListViewItem::get_document_item()
{
	CL_ListViewItem p = *this;
	while (!p.get_parent().is_null())
	{
		p = p.get_parent();
	}

	return p;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewItem Implementation:

CL_ListViewItem_Impl *CL_ListViewItem_Impl::get_root_parent()
{
	CL_ListViewItem_Impl *item = this;

	while (item)
	{
		if (item->parent.expired())
			return item;
		item = item->parent.lock().get();
	}

	return 0;
}

