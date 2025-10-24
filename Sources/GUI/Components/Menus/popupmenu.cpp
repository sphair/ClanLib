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
#include "API/GUI/gui_manager.h"
#include "API/GUI/Components/popupmenu.h"
#include "API/Core/Text/string_help.h"
#include "popupmenu_impl.h"
#include "popupmenu_window.h"
#include "menu_modal_loop.h"

/////////////////////////////////////////////////////////////////////////////
// CL_PopupMenu Construction:

CL_PopupMenu::CL_PopupMenu()
: impl(new CL_PopupMenu_Impl)
{
}

CL_PopupMenu::~CL_PopupMenu()
{
}

CL_PopupMenu CL_PopupMenu::create_null_object()
{
	CL_PopupMenu pmenu;
	pmenu.impl = CL_SharedPtr<CL_PopupMenu_Impl>();
	return pmenu;
}

/////////////////////////////////////////////////////////////////////////////
// CL_PopupMenu Attributes:

void CL_PopupMenu::throw_if_null() const
{
	if (!impl)
		throw CL_Exception("CL_PopupMenu is null");
}


CL_PopupMenuItem CL_PopupMenu::get_item(int id)
{
	std::vector<CL_PopupMenuItem>::size_type index, size;
	size = impl->items.size();
	for (index = 0; index < size; index++)
	{
		if (impl->items[index].get_id() == id)
			return impl->items[index];
	}
	return CL_PopupMenuItem();
}

CL_PopupMenuItem CL_PopupMenu::get_item_at(int index) 
{
	if (index < 0 || index >= (int)impl->items.size())
		return CL_PopupMenuItem();
	else
		return impl->items[index];
}

int CL_PopupMenu::get_item_count() const
{
	return (int) impl->items.size();
}

CL_Size CL_PopupMenu::get_preferred_size() const
{
	return CL_Size(100,100); // todo
}

CL_String CL_PopupMenu::get_class_name() const
{
	return impl->class_name;
}

/////////////////////////////////////////////////////////////////////////////
// CL_PopupMenu Events:

CL_Callback_v0 &CL_PopupMenu::func_close()
{
	return impl->func_close;
}

/////////////////////////////////////////////////////////////////////////////
// CL_PopupMenu Operations:

void CL_PopupMenu::start(CL_GUIComponent *parent, const CL_Point &pos)
{
	if (get_item_count() == 0)
		return;

	//CL_MenuModalLoop *menu_ptr = new CL_MenuModalLoop(parent->get_gui_manager());
	//menu_ptr->start(parent, *this, pos);

	if(!impl.get()->menu_ptr.get())
		impl.get()->menu_ptr.reset(new CL_MenuModalLoop(parent->get_gui_manager()));

	impl.get()->menu_ptr.get()->start(parent, *this, pos);
}

CL_PopupMenuItem CL_PopupMenu::insert_item(const CL_StringRef &text, int id, int index)
{
	return insert_item_accel(text, "", id, index);
}

CL_PopupMenuItem CL_PopupMenu::insert_item_accel(const CL_StringRef &text, const CL_StringRef &accel_text, int id, int index)
{
	if (id == -1)
		id = impl->next_id++;

	CL_PopupMenuItem item(id);
	item.set_text(text);
	item.set_accelerator_text(accel_text);

	if (index < 0 || index >= (int)impl->items.size())
		impl->items.push_back(item);
	else
		impl->items.insert(impl->items.begin() + index, item);
	return item;
}

CL_PopupMenuItem CL_PopupMenu::insert_separator(int index)
{
	CL_PopupMenuItem item(-2);
	item.set_separator(true);

	if (index < 0 || index >= (int)impl->items.size())
		impl->items.push_back(item);
	else
		impl->items.insert(impl->items.begin() + index, item);

	return item;
}

void CL_PopupMenu::set_maximum_height(int height_pixels)
{
	impl->maximum_height = height_pixels;
}

void CL_PopupMenu::set_minimum_width(int width_pixels)
{
	impl->minimum_width = width_pixels;
}

int CL_PopupMenu::get_minimum_width() const
{
	return impl->minimum_width;
}

int CL_PopupMenu::find_item(const CL_StringRef &text, bool case_sensitive)
{
	int size = get_item_count();
	int index = 0;
	int best_match_index = -1;
	CL_String::size_type highest_match = 0;
	CL_StringRef::size_type text_len = text.length();
	CL_String text_upper = CL_StringHelp::text_to_upper(text);

	while (index < size)
	{
		CL_PopupMenuItem item = get_item_at(index);
		CL_String str = CL_StringHelp::text_to_upper(item.get_text());
		CL_String::size_type str_len = str.length();

		CL_String::size_type char_index = 0;
		while (char_index < str_len && char_index < text_len && text_len <= str_len)
		{
			if (str[char_index] == text_upper[char_index])
			{
				char_index++;
			}
			else
			{
				char_index = 0;
				break;
			}
		}
		if (char_index != 0 && char_index > highest_match)
		{
			best_match_index = index;
			highest_match = char_index;
		}

		index++;
	}

	return best_match_index;
}

void CL_PopupMenu::set_class_name(const CL_StringRef &class_name)
{
	impl->class_name = class_name;
}

void CL_PopupMenu::clear()
{
	impl->items.clear();
	impl->next_id = 0;
}

/////////////////////////////////////////////////////////////////////////////
// CL_PopupMenu Implementation:
