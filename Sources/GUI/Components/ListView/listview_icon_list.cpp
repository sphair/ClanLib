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
*/

#include "GUI/precomp.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_component_description.h"
#include "API/GUI/Components/listview_icon_list.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/Window/display_window.h"
#include <map>

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// ListViewIconList_Impl Class:

class ListViewIconList_Impl
{
public:
	ListViewIconList_Impl() 
	: color(Colorf::transparent), selected_color(Colorf::transparent),
	  color_overlay(Colorf::transparent), selected_color_overlay(Colorf::transparent)
	{
	}

	std::map<int, ListViewIcon> icons;

	Colorf color, selected_color;
	Colorf color_overlay, selected_color_overlay;
};

/////////////////////////////////////////////////////////////////////////////
// ListViewIconList Construction:

ListViewIconList::ListViewIconList()
: impl(new ListViewIconList_Impl)
{
}

ListViewIconList::~ListViewIconList()
{
}

/////////////////////////////////////////////////////////////////////////////
// ListViewIconList Attributes:

ListViewIcon ListViewIconList::get_icon(int index)
{
	std::map<int, ListViewIcon>::const_iterator it;
	it = impl->icons.find(index);
	if (it != impl->icons.end())
		return impl->icons[index];

	return ListViewIcon();
}

Colorf ListViewIconList::get_color()
{
	return impl->color;
}

Colorf ListViewIconList::get_selected_color()
{
	return impl->selected_color;
}

Colorf ListViewIconList::get_color_overlay()
{
	return impl->color_overlay;
}

Colorf ListViewIconList::get_selected_color_overlay()
{
	return impl->selected_color_overlay;
}

/////////////////////////////////////////////////////////////////////////////
// ListViewIconList Operations:

void ListViewIconList::clear()
{
	impl->icons.clear();
}

void ListViewIconList::erase(int index)
{
	std::map<int, ListViewIcon>::const_iterator it;
	it = impl->icons.find(index);
	if (it != impl->icons.end())
		impl->icons.erase(index);
}

void ListViewIconList::set_icon(int index, const ListViewIcon &icon)
{
	impl->icons[index] = icon;	
}

void ListViewIconList::set_color(const Colorf &color)
{
	impl->color = color;
}

void ListViewIconList::set_selected_color(const Colorf &color)
{
	impl->selected_color = color;
}

void ListViewIconList::set_color_overlay(const Colorf &color)
{
	impl->color_overlay = color;
}

void ListViewIconList::set_selected_color_overlay(const Colorf &color)
{
	impl->selected_color_overlay = color;
}

/////////////////////////////////////////////////////////////////////////////
// ListViewIconList Implementation:

}
