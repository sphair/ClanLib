/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// PopupMenu Construction:

PopupMenu::PopupMenu()
: impl(new PopupMenu_Impl)
{
}

PopupMenu::~PopupMenu()
{
}

PopupMenu PopupMenu::create_null_object()
{
	PopupMenu pmenu;
	pmenu.impl = std::shared_ptr<PopupMenu_Impl>();
	return pmenu;
}

/////////////////////////////////////////////////////////////////////////////
// PopupMenu Attributes:

void PopupMenu::throw_if_null() const
{
	if (!impl)
		throw Exception("PopupMenu is null");
}


PopupMenuItem PopupMenu::get_item(int id)
{
	std::vector<PopupMenuItem>::size_type index, size;
	size = impl->items.size();
	for (index = 0; index < size; index++)
	{
		if (impl->items[index].get_id() == id)
			return impl->items[index];
	}
	return PopupMenuItem();
}

PopupMenuItem PopupMenu::get_item_at(int index) 
{
	if (index < 0 || index >= (int)impl->items.size())
		return PopupMenuItem();
	else
		return impl->items[index];
}

int PopupMenu::get_item_count() const
{
	return (int) impl->items.size();
}

Size PopupMenu::get_css_size() const
{
	return Size(100,100); // todo
}

std::string PopupMenu::get_class() const
{
	return impl->class_name;
}

/////////////////////////////////////////////////////////////////////////////
// PopupMenu Events:

std::function<void()> &PopupMenu::func_close()
{
	return impl->func_close;
}

/////////////////////////////////////////////////////////////////////////////
// PopupMenu Operations:

void PopupMenu::start(GUIComponent *parent, const Point &pos)
{
	if (get_item_count() == 0)
		return;

	//MenuModalLoop *menu_ptr = new MenuModalLoop(parent->get_gui_manager());
	//menu_ptr->start(parent, *this, pos);

	if(!impl.get()->menu_ptr.get())
		impl.get()->menu_ptr.reset(new MenuModalLoop(parent->get_gui_manager()));

	impl.get()->menu_ptr.get()->start(parent, *this, pos);
}

PopupMenuItem PopupMenu::insert_item(const std::string &text, int id, int index)
{
	return insert_item_accel(text, "", id, index);
}

PopupMenuItem PopupMenu::insert_item_accel(const std::string &text, const std::string &accel_text, int id, int index)
{
	if (id == -1)
		id = impl->next_id++;

	PopupMenuItem item(id);
	item.set_text(text);
	item.set_accelerator_text(accel_text);

	if (index < 0 || index >= (int)impl->items.size())
		impl->items.push_back(item);
	else
		impl->items.insert(impl->items.begin() + index, item);
	return item;
}

PopupMenuItem PopupMenu::insert_separator(int index)
{
	PopupMenuItem item(-2);
	item.set_separator(true);

	if (index < 0 || index >= (int)impl->items.size())
		impl->items.push_back(item);
	else
		impl->items.insert(impl->items.begin() + index, item);

	return item;
}

void PopupMenu::set_maximum_height(int height_pixels)
{
	impl->maximum_height = height_pixels;
}

void PopupMenu::set_minimum_width(int width_pixels)
{
	impl->minimum_width = width_pixels;
}

int PopupMenu::get_minimum_width() const
{
	return impl->minimum_width;
}

int PopupMenu::find_item(const std::string &text, bool case_sensitive)
{
	int size = get_item_count();
	int index = 0;
	int best_match_index = -1;
	std::string::size_type highest_match = 0;
	std::string::size_type text_len = text.length();
	std::string text_upper = StringHelp::text_to_upper(text);

	while (index < size)
	{
		PopupMenuItem item = get_item_at(index);
		std::string str = StringHelp::text_to_upper(item.get_text());
		std::string::size_type str_len = str.length();

		std::string::size_type char_index = 0;
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

void PopupMenu::set_class_name(const std::string &class_name)
{
	impl->class_name = class_name;
}

void PopupMenu::clear()
{
	impl->items.clear();
	impl->next_id = 0;
}

/////////////////////////////////////////////////////////////////////////////
// PopupMenu Implementation:

}
