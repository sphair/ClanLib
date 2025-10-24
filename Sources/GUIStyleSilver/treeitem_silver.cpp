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

#include "API/Display/display.h"
#include "API/Display/font.h"
#include "API/Display/surface.h"
#include "API/GUI/treenode.h"
#include "API/GUI/treeview.h"
#include "API/GUIStyleSilver/treeitem_silver.h"
#include "API/GUIStyleSilver/stylemanager_silver.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_TreeItem_Silver::CL_TreeItem_Silver(
	CL_TreeItem *treeview_item,
	CL_StyleManager_Silver *style)
:
	CL_ComponentStyle(treeview_item),
	item(treeview_item),
	style(style)
{
	CL_ResourceManager *resources = style->get_resources();
	font = new CL_Font("TreeViewItem/font", resources);
	font->set_color( CL_Color::black );
		
	slot_paint = item->sig_paint().connect(
		this, &CL_TreeItem_Silver::on_paint);
	slot_get_preferred_size = item->sig_get_preferred_size().connect(
		this, &CL_TreeItem_Silver::on_get_preferred_size);
}

CL_TreeItem_Silver::~CL_TreeItem_Silver()
{
	delete font;
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_TreeItem_Silver::on_get_preferred_size(CL_Size &size)
{
	size.width = item->get_node()->get_treeview()->get_client_area()->get_width();
	size.height = item->get_custom_height();

	if(size.height == 0)
	{
		if(item->get_icon())
			size.height = item->get_icon()->get_height() + 2;
		else
			size.height = font->get_height(item->get_text()) + 2;
	}
}

void CL_TreeItem_Silver::on_paint()
{
	CL_Rect rect = item->get_screen_rect();
	
	int height = rect.get_height();
	int font_height = font->get_height(item->get_text(0));
	
	if(item->has_mouse_over())
		CL_Display::fill_rect(rect, CL_Color(232, 236, 241));
	if(item->get_node()->is_selected())
		CL_Display::fill_rect(rect, CL_Color(184, 195, 209, 150));

	CL_TreeView *treeview = item->get_node()->get_treeview();
	int columns = treeview->get_column_count();
	if(columns == 0)
		columns = 1;

	// Draw columns
	for(int x = 0, i = 0; i < columns; ++i)
	{
		int dx = 0;
		if(i == 0)
		{
			dx += item->get_text_margin();

			CL_Surface *icon = item->get_icon();
			if(icon)
			{
				icon->draw(
					rect.left + x + 1,
					rect.top + (height - icon->get_height()) / 2);
				dx += icon->get_width();
			}
		}

		CL_Component *comp = item->get_component(i);
		if(comp)
		{
			comp->set_position(CL_Rect(x + dx + 4, 0, x + dx + 4 + treeview->get_column_width(i), height));
			comp->paint();
		}
		else
		{
			font->draw(
				rect.left + x + dx + 4,
				rect.top + (height - font_height) / 2,
				item->get_text(i));
		}

		x += treeview->get_column_width(i);
		if(i == 0)
			x -= item->get_node()->get_placement_offset();
	}
}
