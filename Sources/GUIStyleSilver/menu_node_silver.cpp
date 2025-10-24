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

#include "API/Core/Resources/resource_manager.h"
#include "API/Core/Math/rect.h"
#include "API/Display/surface.h"
#include "API/Core/XML/dom_element.h"
#include "API/Display/display.h"
#include "API/GUIStyleSilver/stylemanager_silver.h"
#include "API/GUI/menu.h"
#include "API/GUIStyleSilver/menu_node_silver.h"

CL_MenuNode_Silver::CL_MenuNode_Silver(
	CL_MenuNode *node,
	CL_StyleManager_Silver *style)
:
	CL_ComponentStyle(node),
	node(node),
	style(style),
	resources(0),
	arrow(0)
{
	resources = style->get_resources();

	if (resources->exists("Menu/submenu_arrow"))
		arrow = new CL_Surface("Menu/submenu_arrow", resources);	
	
	slots.connect(node->sig_set_options(), this, &CL_MenuNode_Silver::on_set_options);
	slots.connect(node->sig_paint(), this, &CL_MenuNode_Silver::on_paint);
	slots.connect(node->sig_reposition(), this, &CL_MenuNode_Silver::on_reposition); // todo: sync names?
	slots.connect(node->sig_get_preferred_size(), this, &CL_MenuNode_Silver::on_get_preferred_size);
}

CL_MenuNode_Silver::~CL_MenuNode_Silver()
{
	delete arrow;
}

void CL_MenuNode_Silver::on_set_options(const CL_DomElement &options)
{
	// ?
}

void CL_MenuNode_Silver::on_paint()
{
	CL_Rect area(
		node->get_screen_x()+1,
		node->get_screen_y(),
		node->get_screen_x()+node->get_width()-1,
		node->get_screen_y()+node->get_height());

	// mouseover fill
	if( node->has_mouse_over() && node->is_enabled() && node->get_parent_menu()->is_enabled() )
		CL_Display::fill_rect( area, 
			CL_Gradient(
				CL_Color(255,255,255,128), CL_Color(254, 207, 147, 128),
				CL_Color(255,255,255,128), CL_Color(254, 207, 147, 128) ));

	if( arrow && node->has_submenu() )
	{		
		if( node->get_parent_menu()->is_vertical() ) // no arrow in horizontal menus
			arrow->draw( area.right-8, node->get_screen_y()+4 );
	}
}

void CL_MenuNode_Silver::on_reposition()
{
	CL_Rect menu_pos = node->get_parent_menu()->get_position();
	CL_Rect node_pos = node->get_position();
	CL_Menu *submenu = node->get_submenu();

	if( submenu )
	{
		if( node->get_parent_menu()->is_vertical() )
			submenu->reposition( menu_pos.right-2, menu_pos.top + node_pos.top+2 );
		else
		{
			submenu->reposition( menu_pos.left + node_pos.left, menu_pos.top+menu_pos.get_height() );
		}
	}
}

void CL_MenuNode_Silver::on_get_preferred_size(CL_Size &size)
{
	if( size.width < 220 )
		size.width = 220;
}
