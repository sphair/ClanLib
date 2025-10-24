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

#include "API/Display/font.h"
#include "API/Display/display.h"
#include "API/Display/keys.h"
#include "API/GUI/scrollbar.h"
#include "API/GUI/listitem.h"
#include "API/GUIStyleSilver/listbox_silver.h"
#include "API/GUIStyleSilver/stylemanager_silver.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_ListBox_Silver::CL_ListBox_Silver(
	CL_ListBox *listbox,
	CL_StyleManager_Silver *style)
:
	CL_ComponentStyle(listbox),
	listbox(listbox),
	style(style)
{
	resources = style->get_resources();
	font = new CL_Font("ListBox/font", resources);
	font->set_color( CL_Color::black );

	// Create a vertical scrollbar
	scrollbar = new CL_ScrollBar(false, listbox);
	visible_scrollbar = true;

	int font_height = font->get_height();
	listbox->set_item_height(font_height);

	CL_Component *client_area = listbox->get_client_area();

	slots.connect(listbox->sig_set_options(), this, &CL_ListBox_Silver::on_set_options);
	slots.connect(listbox->sig_paint(), this, &CL_ListBox_Silver::on_paint_background);
	slots.connect(client_area->sig_mouse_down(), this, &CL_ListBox_Silver::on_mouse_down);
	slots.connect(client_area->sig_paint(), this, &CL_ListBox_Silver::on_paint_listbox);
	slots.connect(listbox->sig_resize(), this, &CL_ListBox_Silver::on_resize);
	slots.connect(listbox->sig_item_added(), this, &CL_ListBox_Silver::on_item_added);
	slots.connect(listbox->sig_item_removed(), this, &CL_ListBox_Silver::on_item_removed);
	slots.connect(listbox->sig_clear(), this, &CL_ListBox_Silver::on_clear);
	slots.connect(scrollbar->sig_value_changed(), this, &CL_ListBox_Silver::on_scroll_change);

	on_resize(0,0);
}

CL_ListBox_Silver::~CL_ListBox_Silver()
{
	delete font;
	delete scrollbar;
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_ListBox_Silver::on_item_added(int index)
{
	update_scrollbar(true);
}

void CL_ListBox_Silver::on_item_removed(int index)
{
	update_scrollbar(true);
}

void CL_ListBox_Silver::on_clear()
{
	update_scrollbar(true);
}

void CL_ListBox_Silver::on_set_options(const CL_DomElement &options)
{
}

void CL_ListBox_Silver::on_scroll_change(int new_offset)
{
	listbox->set_top_item(new_offset);
}

void CL_ListBox_Silver::on_mouse_down(const CL_InputEvent &key)
{
	if(key.id == CL_MOUSE_WHEEL_UP)
	{
		listbox->set_top_item(listbox->get_top_item() - 5);
		update_scrollbar(true);
		return;
	}
	if(key.id == CL_MOUSE_WHEEL_DOWN)
	{
		listbox->set_top_item(listbox->get_top_item() + 5);
		update_scrollbar(true);
		return;
	}
}

void CL_ListBox_Silver::on_resize(int old_width, int old_height)
{
	// Position the listbox item-box
	CL_Rect rect(4, 4, 4 + listbox->get_width() - 8 , 4 + listbox->get_height() - 8);
	listbox->get_client_area()->set_position(rect);

	update_scrollbar(false);

	// Scrollbar was added, shrink clientarea
	if(visible_scrollbar)
	{
		listbox->get_client_area()->set_width(listbox->get_client_area()->get_width() - 19);

		// Position the scrollbar
		rect = CL_Rect(listbox->get_width() - 19, 3, listbox->get_width() - 3, listbox->get_height() - 3);
		scrollbar->set_position(rect);
	}
}

void CL_ListBox_Silver::on_paint_background()
{
	// Outline
	CL_Display::draw_rect(listbox->get_screen_rect(), CL_Color(128, 142, 159));

	// Background
	CL_Rect rect = listbox->get_screen_rect();
	rect.left++;
	rect.top++;
	rect.right--;
	rect.bottom--;
	CL_Display::fill_rect(rect, CL_Color::white);
}

void CL_ListBox_Silver::on_paint_listbox()
{
	int width = listbox->get_client_area()->get_width();

	int pos = 0;

	int font_height = font->get_height();

	std::vector<CL_ListItem *> items = listbox->get_items();
	std::vector<CL_ListItem *>::iterator it;
	int offset = listbox->get_top_item();
	int end = offset + (listbox->get_height () - 2) / font_height;
	for (it = items.begin(); it != items.end(); it++)
	{
		if(pos < offset)
		{
			++pos;
			continue;
		}
		else if (pos >= end)
		{
			break;
		}

		if((*it)->selected)
		{
			CL_Display::fill_rect(
				CL_Rect(
					listbox->get_screen_x() + 1,
					listbox->get_screen_y() + 1 +(pos - offset) * font_height,
					listbox->get_screen_x() - 2 + width,
					listbox->get_screen_y() + 1 +(pos + 1 - offset) * font_height),
				CL_Gradient(
					CL_Color(254, 207, 147),
					CL_Color::white,
					CL_Color(254, 207, 147),
					CL_Color::white));
		}

		

		CL_Rect rect (CL_Point (listbox->get_screen_x() + 3,
			                    listbox->get_screen_y() + 1 +(pos - offset) * font_height),
					  CL_Size (listbox->get_width () - 6,
							   font_height));

		if (visible_scrollbar)
		{
			rect.right -= scrollbar->get_width ();
		}

		font->draw (rect,		
					(*it)->str);

		pos++;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Implementation:

void CL_ListBox_Silver::update_scrollbar(bool resize)
{	
	int max_visible_items = (listbox->get_client_area()->get_height() - 4) / listbox->get_item_height();

	if(listbox->get_count() > max_visible_items)
	{
		if(visible_scrollbar == false)
		{
			visible_scrollbar = true;
			listbox->add_child(scrollbar, false);

			if(resize)
				on_resize(0,0);
		}

		scrollbar->set_max_value(listbox->get_count() - max_visible_items);
		scrollbar->set_value(listbox->get_top_item());
	}
	else
	{
		if(visible_scrollbar)
		{
			visible_scrollbar = false;
			scrollbar->set_value(0);
			listbox->remove_child(scrollbar);

			if(resize)
				on_resize(0,0);
		}
	}
}
