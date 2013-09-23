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
**    Mark Page
*/

#include "precomp.h"

#include "option_list.h"

OptionList::OptionList()
{

}

OptionList::~OptionList()
{

}

void OptionList::setup(clan::GUIComponent *base_component, const std::string &name, int xpos, int ypos, const std::vector<std::string> &list)
{

	checkbox = new clan::CheckBox(base_component);
	int width = 120;
	int height = 16;
	checkbox->set_geometry(clan::Rect(xpos, ypos , clan::Size(width, height)));
	xpos += width;
	checkbox->set_text(name);
	checkbox->set_checked(false);
	checkbox->func_state_changed().set(this, &OptionList::checkbox_changed);

	for (unsigned int cnt = 0; cnt<list.size(); cnt++)
	{
		popupmenu.insert_item(list[cnt]);
	}

	combobox = new clan::ComboBox(base_component);
	width = 100;
	combobox->set_geometry(clan::Rect(xpos, ypos - 3, clan::Size(width, 21)));
	xpos += width;
	combobox->set_editable(false);
	combobox->set_dropdown_height(128);
	combobox->set_dropdown_minimum_width(32);
	combobox->set_popup_menu(popupmenu);
	combobox->set_selected_item(0);
	combobox->func_item_selected().set(this, &OptionList::combobox_changed);

	combobox_changed(0);
	checkbox_changed();
}

void OptionList::checkbox_changed()
{
	css_enabled = checkbox->is_checked();
	combobox->set_enabled(css_enabled);
}

void OptionList::combobox_changed(int index)
{
	int item = combobox->get_selected_item();
	clan::PopupMenuItem menu = popupmenu.get_item(item);
	css_item = menu.get_text();
}

