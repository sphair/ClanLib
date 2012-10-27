/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

#ifdef INCLUDE_COMPONENTS

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// ListViewItem Construction:

ListViewItem::ListViewItem()
{
}

ListViewItem::ListViewItem(const std::shared_ptr<ListViewItem_Impl> &impl)
: impl(impl)
{
}

ListViewItem::~ListViewItem()
{
}

/////////////////////////////////////////////////////////////////////////////
// ListViewItem Attributes:

void ListViewItem::throw_if_null() const
{
	if (!impl)
		throw Exception("ListViewItem is null");
}

bool ListViewItem::is_item() const
{
	return impl ? true : false;
}

ListViewColumnData ListViewItem::get_column(const std::string &column_id)
{
	std::vector<ListViewColumnData>::iterator it;
	for (it = impl->column_data.begin(); it != impl->column_data.end(); ++it)
	{
		if ((*it).get_column_id() == column_id)
			return (*it);
	}

	return ListViewColumnData();
}

ListViewItem ListViewItem::get_parent()
{
	return ListViewItem(impl->parent.lock());
}

ListViewItem ListViewItem::get_next_sibling()
{
	if (impl)
		return ListViewItem(impl->next_sibling);

	return ListViewItem();
}

ListViewItem ListViewItem::get_prev_sibling()
{
	if (impl)
		return ListViewItem(impl->prev_sibling.lock());

	return ListViewItem();
}

ListViewItem ListViewItem::get_first_child()
{
	if (impl)
		return ListViewItem(impl->first_child);
	return ListViewItem();
}

ListViewItem ListViewItem::get_last_child()
{
	if (impl)
		return ListViewItem(impl->last_child.lock());
	return ListViewItem();
}

bool ListViewItem::is_open() const
{
	return impl->open;
}

bool ListViewItem::is_editable() const
{
	return impl->editable;
}

bool ListViewItem::is_selected() const
{
	return impl->selected;
}

bool ListViewItem::has_children() const
{
	return impl->first_child ? true : false;;
}

int ListViewItem::get_parent_count()
{
	int num_parents = 0;
	ListViewItem i = get_parent();
	while (i.is_item())
	{
		num_parents++;
		i = i.get_parent();
	}

	return num_parents;
}

int ListViewItem::get_child_count(bool recursive, bool recurse_only_into_open_items)
{
	int num_children = 0;
	ListViewItem i = get_first_child();
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

int ListViewItem::get_id() const
{
	return impl->id;
}

int ListViewItem::get_icon() const
{
	return impl->icon;
}

std::vector<int> ListViewItem::get_overlay_icons()
{
	return impl->overlay_icons;
}

std::shared_ptr<ListViewItemUserData> ListViewItem::get_userdata() const
{
	return impl->userdata;
}

bool ListViewItem::operator==(ListViewItem &other) const
{
	return impl.get() == other.impl.get();
}

bool ListViewItem::operator!=(ListViewItem &other) const
{
	return impl.get() != other.impl.get();
}


/////////////////////////////////////////////////////////////////////////////
// ListViewItem Operations:

ListViewItem ListViewItem::append_child(ListViewItem &item)
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

	return ListViewItem();
}

ListViewItem ListViewItem::remove()
{
	ListViewItem prev = get_prev_sibling();
	ListViewItem next = get_next_sibling();
	ListViewItem parent = get_parent();
	
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
			next.impl->prev_sibling = std::shared_ptr<ListViewItem_Impl>();
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

	ListViewItem document_item = get_document_item();

	if (!document_item.impl->func_item_deleted.is_null())
		document_item.impl->func_item_deleted.invoke(*this);
	
	return *this;
}

void ListViewItem::remove_children()
{
	std::shared_ptr<ListViewItem_Impl> null_impl;
	impl->first_child = null_impl;
	impl->last_child = null_impl;

	if (!impl->func_item_deleted.is_null())
		impl->func_item_deleted.invoke(ListViewItem());
}

void ListViewItem::set_column_text(const std::string &column_id, const std::string &text)
{
	ListViewItem document_item = get_document_item();

	std::vector<ListViewColumnData>::iterator it;
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

	ListViewColumnData cd;
	cd.set_column_id(column_id);
	cd.set_text(text);
	impl->column_data.push_back(cd);

	if (!document_item.impl->func_item_modified.is_null())
		document_item.impl->func_item_modified.invoke(*this);
}

void ListViewItem::set_open(bool open)
{
	impl->open = open;
}

void ListViewItem::set_editable(bool editable)
{
	impl->editable = editable;
}

void ListViewItem::set_icon(int icon_index)
{
	impl->icon = icon_index;
}

void ListViewItem::add_overlay_icon(int icon_index)
{
	std::vector<int>::iterator it;
	for (it = impl->overlay_icons.begin(); it != impl->overlay_icons.end(); ++it)
	{
		if ((*it) == icon_index)
			return;
	}

	impl->overlay_icons.push_back(icon_index);
}

void ListViewItem::remove_overlay_icon(int icon_index)
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

void ListViewItem::set_userdata(std::shared_ptr<ListViewItemUserData> ptr)
{
	impl->userdata = ptr;
}

Callback_v1<Rect> &ListViewItem::func_render_icon()
{
	return impl->func_render_icon;
}

void ListViewItem::set_id(int id)
{
	impl->id = id;
}

ListViewItem ListViewItem::get_document_item()
{
	ListViewItem p = *this;
	while (!p.get_parent().is_null())
	{
		p = p.get_parent();
	}

	return p;
}

/////////////////////////////////////////////////////////////////////////////
// ListViewItem Implementation:

ListViewItem_Impl *ListViewItem_Impl::get_root_parent()
{
	ListViewItem_Impl *item = this;

	while (item)
	{
		if (item->parent.expired())
			return item;
		item = item->parent.lock().get();
	}

	return 0;
}

}

#endif
