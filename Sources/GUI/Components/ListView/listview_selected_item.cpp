/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewSelection_Impl class:

class CL_ListViewSelection_Impl
{
public:
	CL_ListViewSelection_Impl()
	{
	}

	CL_ListViewSelectedItem first;

	CL_ListViewSelectedItem last;
};

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewSelection_Impl class:

class CL_ListViewSelectedItem_Impl
{
public:
	CL_ListViewSelectedItem_Impl()
	{
	}

	CL_ListViewItem item;

	CL_SharedPtr<CL_ListViewSelectedItem_Impl> next;

	CL_WeakPtr<CL_ListViewSelectedItem_Impl> prev;
};

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewSelection Construction:

CL_ListViewSelection::CL_ListViewSelection()
: impl(new CL_ListViewSelection_Impl())
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewSelection Attributes:

CL_ListViewSelectedItem CL_ListViewSelection::get_first()
{
	return impl->first;
}

CL_ListViewSelectedItem CL_ListViewSelection::get_last()
{
	return impl->last;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewSelection Operations:

void CL_ListViewSelection::append(const CL_ListViewItem &item)
{
	if (impl->last.is_item())
	{
		CL_ListViewSelectedItem sel_item(item);
		sel_item.impl->prev = impl->last.impl;
		impl->last.impl->next = sel_item.impl;
		impl->last = sel_item;
	}
	else
	{
		impl->first = CL_ListViewSelectedItem(item);
		impl->last = CL_ListViewSelectedItem(item);
	}
}

void CL_ListViewSelection::remove(const CL_ListViewItem &item)
{
	CL_ListViewSelectedItem it = impl->first;
	while (it.is_item())
	{
		if (it.impl->item.impl == item.impl)
		{
			if (it.impl->prev)
			{
				it.impl->prev->next = it.impl->next;
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

void CL_ListViewSelection::clear()
{
	CL_ListViewSelectedItem it = impl->first;
	while (it.is_item())
	{
		it.get_item().impl->selected = false;
		it = it.get_next_sibling();
	}

	impl->first = CL_ListViewSelectedItem();
	impl->last = CL_ListViewSelectedItem();
}

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewSelection Implementation:






/////////////////////////////////////////////////////////////////////////////
// CL_ListViewSelectedItem Construction:

CL_ListViewSelectedItem::CL_ListViewSelectedItem() 
{
}

CL_ListViewSelectedItem::CL_ListViewSelectedItem(const CL_ListViewItem &item) 
: impl(new CL_ListViewSelectedItem_Impl())
{
	impl->item = item;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewSelectedItem Attributes:

void CL_ListViewSelectedItem::throw_if_null() const
{
	if (impl.is_null())
		throw CL_Exception("CL_ListViewSelectedItem is null");
}

bool CL_ListViewSelectedItem::is_item() const
{
	return !impl.is_null();
}

CL_ListViewItem CL_ListViewSelectedItem::get_item()
{
	return impl->item;
}

CL_ListViewSelectedItem CL_ListViewSelectedItem::prev()
{
	return CL_ListViewSelectedItem(impl->prev);
}

CL_ListViewSelectedItem CL_ListViewSelectedItem::get_next_sibling()
{
	return CL_ListViewSelectedItem(impl->next);
}

/////////////////////////////////////////////////////////////////////////////
// CL_ListViewSelectedItem Implementation:

CL_ListViewSelectedItem::CL_ListViewSelectedItem(const CL_SharedPtr<CL_ListViewSelectedItem_Impl> &impl)
: impl(impl)
{
}
