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

#include "API/display.h"
#include "API/gui.h"
#include "API/core.h"  // ?

#include "API/GUIStyleSilver/menu_silver.h"
#include "API/GUI/menu.h"
#include "API/GUI/menu_node.h"

CL_Menu_Silver::CL_Menu_Silver(
	CL_Menu *menu,
	CL_StyleManager_Silver *style)
:
	CL_ComponentStyle(menu),
	menu(menu),
	style(style),
	repositioned(true)
{
	slots.connect(menu->sig_set_options(), this, &CL_Menu_Silver::on_set_options);
	slots.connect(menu->sig_paint(), this, &CL_Menu_Silver::on_paint);
	slots.connect(menu->sig_reposition(), this, &CL_Menu_Silver::on_reposition);

	if( menu->get_parent_node() == 0 )
	{
		slots.connect(menu->get_parent()->sig_resize(), this, &CL_Menu_Silver::on_parent_resize);
		repositioned = false;
	}
}

CL_Menu_Silver::~CL_Menu_Silver()
{
}

void CL_Menu_Silver::on_set_options(const CL_DomElement &options)
{
	// ?
}

void CL_Menu_Silver::on_paint()
{
	// reposition
	if( !repositioned )
	{
		menu->reposition();
		repositioned = true;
	}

	CL_Rect area = menu->get_screen_rect();
	
	if( !menu->is_vertical() )
	{
		CL_Display::fill_rect(
			area,
			CL_Gradient(
				CL_Color::silver,CL_Color::silver,
				CL_Color::white, CL_Color::white )
		);

		CL_Display::fill_rect(
			CL_Rect( area.left, area.bottom-3, area.right, area.bottom ),
			CL_Gradient(
				CL_Color::white, CL_Color::white,
				CL_Color::silver, CL_Color::silver)
		);

		CL_Display::draw_rect( area, CL_Color(128,128,128,128) );

	}
	else
	{
		// full cover gradient
		CL_Display::fill_rect(
			area,	
			CL_Gradient(
				CL_Color::white, CL_Color::white,
				CL_Color::silver,CL_Color::silver ));

		// left side gradient 
		CL_Display::fill_rect(
			CL_Rect(area.left, area.top, area.left+3, area.bottom),	
			CL_Gradient(
				CL_Color::silver,CL_Color::silver,
				CL_Color::white, CL_Color::white ));
	
		// icon area gradient
		CL_Display::fill_rect(
			CL_Rect(area.left+3, area.top, area.left+3+17, area.bottom),	
			CL_Gradient(
				CL_Color::white, CL_Color::lightgray,
				CL_Color::white, CL_Color::lightgray ));


		CL_Display::draw_rect( area, CL_Color(0,0,0,128) );
	}
		
}

void CL_Menu_Silver::on_reposition(int pos_x, int pos_y)
{
	menu->raise(); // make menu be on top of other components.

	int items_width = menu->get_items_width() + 4;
	if( items_width < 90 )
		items_width = 90;

	int items_height = menu->get_items_height();
	if( items_height < 20 )
		items_height = 20;

	CL_Rect menu_rect(
		pos_x, pos_y,
		pos_x + items_width + 3,
		pos_y + items_height );
	
	if( menu->is_auto_resize() )
	{
		menu->set_position( menu_rect );
	}
	
	std::list<CL_MenuNode*> &children = menu->get_node_list();

	if( !children.empty() )
	{
		int X=2, Y=1;
		
		// calculate a new position for each menu node, and 
		// update the positions of all submenus.
		std::list<CL_MenuNode*>::iterator it;
		for( it = children.begin(); it != children.end(); ++it )
		{
			CL_Component *item = (*it)->get_data();
			const CL_Rect &ipos = item->get_position();

			(*it)->set_position(
				CL_Rect(
					X, Y,
					menu->is_vertical() ? X + items_width : X + item->get_width(),
					Y + ipos.top + item->get_height() + 4 ));

			if( menu->is_vertical() )
			{
				// don't resize elements not placed originally at (0,0) or (1,0)
				if( ipos.top == 0 && ipos.left == 0 )
					item->set_position(1,0);
			}
			else
				item->set_position(0,0);
			
			(*it)->reposition_submenus();
			
			if( menu->is_vertical() )
				Y += ipos.top + item->get_height() + 4;
			else
				X += ipos.left + item->get_width() + 4;
		}
	}	

	if( menu->is_auto_resize() )
	{
		CL_Rect child_rect = menu->get_children_rect();

		if( menu->is_vertical() )
			menu->set_width(child_rect.right);
		else
			menu->set_width(menu->get_parent()->get_width());

		menu->set_height(child_rect.bottom+2);
	}

}

void CL_Menu_Silver::on_parent_resize(int old_size_x, int old_size_y)
{
	if( menu->is_vertical() || !menu->is_auto_resize() )
		return;
	
	menu->set_width( menu->get_parent()->get_width() );
}
