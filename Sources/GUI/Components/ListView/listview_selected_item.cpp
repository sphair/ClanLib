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
#include "API/GUI/Components/listview_selected_item.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// ListViewSelection_Impl class:

class ListViewSelection_Impl
{
public:
	ListViewSelection_Impl()
	{
	}

	ListViewSelectedItem first;

	ListViewSelectedItem last;
};

/////////////////////////////////////////////////////////////////////////////
// ListViewSelection_Impl class:

class ListViewSelectedItem_Impl
{
public:
	ListViewSelectedItem_Impl()
	{
	}

	ListViewItem item;

	std::shared_ptr<ListViewSelectedItem_Impl> next;

	std::weak_ptr<ListViewSelectedItem_Impl> prev;
};

/////////////////////////////////////////////////////////////////////////////
// ListViewSelection Construction:

ListViewSelection::ListViewSelection()
: impl(new ListViewSelection_Impl())
{
}

/////////////////////////////////////////////////////////////////////////////
// ListViewSelection Attributes:

ListViewSelectedItem ListViewSelection::get_first()
{
	return impl->first;
}

ListViewSelectedItem ListViewSelection::get_last()
{
	return impl->last;
}

/////////////////////////////////////////////////////////////////////////////
// ListViewSelection Operations:

void ListViewSelection::append(const ListViewItem &item)
{
	if (impl->last.is_item())
	{
		ListViewSelectedItem sel_item(item);
		sel_item.impl->prev = impl->last.impl;
		impl->last.impl->next = sel_item.impl;
		impl->last = sel_item;
	}
	else
	{
		impl->first = ListViewSelectedItem(item);
		impl->last = ListViewSelectedItem(item);
	}
}

void ListViewSelection::remove(const ListViewItem &item)
{
	ListViewSelectedItem it = impl->first;
	while (it.is_item())
	{
		if (it.impl->item.impl == item.impl)
		{
			if (!it.impl->prev.expired())
			{
				it.impl->prev.lock()->next = it.impl->next;
			}
			if (it.impl->next)
			{
				it.impl->next->prev = it.impl->prev;
			}
			break;
		}
		it = it.get_next_sibling();
	}
}

void ListViewSelection::clear()
{
	ListViewSelectedItem it = impl->first;
	while (it.is_item())
	{
		it.get_item().impl->selected = false;
		it = it.get_next_sibling();
	}

	impl->first = ListViewSelectedItem();
	impl->last = ListViewSelectedItem();
}

/////////////////////////////////////////////////////////////////////////////
// ListViewSelection Implementation:






/////////////////////////////////////////////////////////////////////////////
// ListViewSelectedItem Construction:

ListViewSelectedItem::ListViewSelectedItem() 
{
}

ListViewSelectedItem::ListViewSelectedItem(const ListViewItem &item) 
: impl(new ListViewSelectedItem_Impl())
{
	impl->item = item;
}

/////////////////////////////////////////////////////////////////////////////
// ListViewSelectedItem Attributes:

void ListViewSelectedItem::throw_if_null() const
{
	if (!impl)
		throw Exception("ListViewSelectedItem is null");
}

bool ListViewSelectedItem::is_item() const
{
	return impl ? true : false;
}

ListViewItem ListViewSelectedItem::get_item()
{
	return impl->item;
}

ListViewSelectedItem ListViewSelectedItem::prev()
{
	return ListViewSelectedItem(impl->prev.lock());
}

ListViewSelectedItem ListViewSelectedItem::get_next_sibling()
{
	return ListViewSelectedItem(impl->next);
}

/////////////////////////////////////////////////////////////////////////////
// ListViewSelectedItem Implementation:

ListViewSelectedItem::ListViewSelectedItem(const std::shared_ptr<ListViewSelectedItem_Impl> &impl)
: impl(impl)
{
}

}
