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
#include "API/Display/surface.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/GUI/component_style.h"
#include "API/GUI/menu_item.h"
#include "API/GUIStyleSilver/menu_item_silver.h"
#include "API/GUIStyleSilver/stylemanager_silver.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_MenuItem_Silver::CL_MenuItem_Silver(
	CL_MenuItem *menu_item,
	CL_StyleManager_Silver *style)
:
	CL_ComponentStyle(menu_item),
	menu_item(menu_item),
	sur_icon(0),
	sur_checked(0),
	sur_unchecked(0),
	delete_sur_icon(false)
{
	resources = style->get_resources();
	
	font = new CL_Font("MenuItem/font", resources);
	font_disabled = new CL_Font("MenuItem/font_disabled", resources);

	font->set_color( CL_Color::black );
	font_disabled->set_color( CL_Color::gray );

	slot_paint = menu_item->sig_paint().connect(
		this, &CL_MenuItem_Silver::on_paint);
	slot_get_preferred_size = menu_item->sig_get_preferred_size().connect(
		this, &CL_MenuItem_Silver::on_get_preferred_size);
}

CL_MenuItem_Silver::~CL_MenuItem_Silver()
{
	delete font;
	delete font_disabled;
	delete sur_checked;
	delete sur_unchecked;

	if( delete_sur_icon )
		delete sur_icon;
}

/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_MenuItem_Silver::on_get_preferred_size(CL_Size &size)
{
	int icon_width = 0;
	int height = 16;

	if( menu_item->use_icon() )
		icon_width = 16;
	
	if( font->get_height(menu_item->get_text()) > height )
		height = font->get_height(menu_item->get_text())+1;

	size.width = icon_width + 4 + font->get_width(menu_item->get_text());
	size.height = height;
}

void CL_MenuItem_Silver::on_paint()
{
	if( menu_item->is_toggling() )
	{
		if( sur_checked == 0 && sur_unchecked == 0 )
		{
			if (resources->exists("MenuItem/checked"))
				sur_checked = new CL_Surface("MenuItem/checked", resources);
			if (resources->exists("MenuItem/unchecked"))
				sur_unchecked = new CL_Surface("MenuItem/unchecked", resources);
		}

		if( menu_item->is_selected() )
		{
			if( sur_checked )
				sur_checked->draw(menu_item->get_screen_x(),menu_item->get_screen_y());
		}
		else	
		{
			if( sur_unchecked )
				sur_unchecked->draw(menu_item->get_screen_x(),menu_item->get_screen_y());
		}
	}
	else if( sur_icon )
		sur_icon->draw(menu_item->get_screen_x(),menu_item->get_screen_y());

	const char *text = menu_item->get_text().c_str();

	// check if menu node is enabled
	bool is_enabled = menu_item->get_parent()->is_enabled();

	// check if parent menu is enabled
	if( menu_item->get_parent()->get_parent()->is_enabled() == false )
		is_enabled = false;

	if( menu_item->use_icon() )
	{
		if( is_enabled )
			font->draw(menu_item->get_screen_x()+20, menu_item->get_screen_y()+2, text);
		else
			font_disabled->draw(menu_item->get_screen_x()+18, menu_item->get_screen_y()+2, text);
	}
	else
	{
		if( is_enabled )
			font->draw(menu_item->get_screen_x()+2, menu_item->get_screen_y()+2, text);
		else
			font_disabled->draw(menu_item->get_screen_x()+2, menu_item->get_screen_y()+2, text);
	}
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_MenuItem_Silver::set_icon(CL_Surface *sur, bool del_surface)
{
	if( sur_icon && delete_sur_icon )
		delete sur_icon;
	
	sur_icon = sur;
	delete_sur_icon = del_surface;
}


