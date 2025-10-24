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

#include "precomp.h"
#include "menu_node_generic.h"
#include "menu_generic.h"
#include "API/GUI/menu.h"
#include "API/GUI/menu_node.h"
#include "API/signals.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_MenuNode_Generic::CL_MenuNode_Generic(
	CL_MenuNode *self,
	CL_Menu *parent_menu)
:
	data(0),
	submenu(0),
	parent_menu(parent_menu),
	close_on_click(true),
	node(self),
	close_lock(false)
{
	slots.connect( node->sig_mouse_enter(), this, &CL_MenuNode_Generic::on_mouse_enter);
	slots.connect( node->sig_mouse_down(), this, &CL_MenuNode_Generic::on_mouse_down);
	slots.connect( node->sig_mouse_up(), this, &CL_MenuNode_Generic::on_mouse_up);
	slots.connect( node->sig_child_add(), this, &CL_MenuNode_Generic::on_child_add);
}

CL_MenuNode_Generic::~CL_MenuNode_Generic()
{
	delete submenu; //SETH: uncommented this to fix mem leak.. why was it commented out?
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

bool CL_MenuNode_Generic::has_mouse_over()
{
	if( node->CL_Component::has_mouse_over() )
		return true;

	if( data )
	{
		if( data->has_mouse_over() ) return true;
		if( child_has_mouse_over(data) ) return true;
	}
	
	return false;
}

bool CL_MenuNode_Generic::has_mouse_in_submenus()
{
	if( !submenu )
		return false;

	if( submenu->has_mouse_over() )
		return true;

	return submenu->has_mouse_in_submenus();
}

bool CL_MenuNode_Generic::has_open_submenu()
{
	if( submenu )
		return submenu->is_visible(false);

	return false;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_MenuNode_Generic::collapse()
{
	if( !submenu )
		return;

	submenu->collapse();
}

void CL_MenuNode_Generic::open_submenu()
{
	if( !submenu )
		return;

	submenu->open();
}

void CL_MenuNode_Generic::set_submenu(CL_Menu *menu)
{
	submenu = menu;
	submenu->set_parent_node(node);

//	parent_menu->get_root_menu()->reposition();
}

void CL_MenuNode_Generic::set_parent_menu(CL_Menu *menu)
{
	parent_menu = menu;

//	parent_menu->get_root_menu()->reposition();
}


/////////////////////////////////////////////////////////////////////////////
// Callbacks:

void CL_MenuNode_Generic::on_child_add(CL_Component *child)
{
	this->data = child;
	
	node->set_width(child->get_width());
	node->set_height(child->get_height());
	
	slots.connect( child->sig_mouse_enter(), this, &CL_MenuNode_Generic::on_mouse_enter );

	node->find_preferred_size();
}

void CL_MenuNode_Generic::on_mouse_down(const CL_InputEvent &e)
{
	if( !parent_menu->is_vertical() ) 
		parent_menu->set_click_to_open(false);
	
	// click to open a submenu in a horizontal menu.
	if( !node->get_parent_menu()->is_vertical() )
	{
		if( node->is_enabled(false) && submenu != 0 )
		{
			if( !submenu->is_open() )
			{
				open_submenu();
				close_lock = true;
			}
		}
	}
}

void CL_MenuNode_Generic::on_mouse_up(const CL_InputEvent &e)
{
	if( close_lock )
	{
		close_lock = false;
		return;
	}

	CL_Menu *root = parent_menu->get_root_menu();

	// click to close a submenu in a horizontal menu.
	if( !node->get_parent_menu()->is_vertical() )
	{
		root->collapse_submenus();
		root->set_click_to_open(true);
	}

	if( root->is_root_collapsing() )
		root->collapse();

	if( close_on_click && submenu == 0 && node->is_enabled(false) )
	{
		CL_Menu *root = parent_menu->get_root_menu();
		root->collapse_submenus();
		root->set_click_to_open(true);
	}

	sig_clicked();	
}

void CL_MenuNode_Generic::on_mouse_enter()
{
	parent_menu->collapse_submenus();

	if( !node->is_enabled(false) )
		return;

	if( parent_menu->is_vertical() )
		open_submenu();
	else if( !parent_menu->is_click_to_open() ) // horizontal
		open_submenu();
}

bool CL_MenuNode_Generic::child_has_mouse_over(const CL_Component *comp) const
{
	std::list<CL_Component*>::const_iterator it;
	const std::list<CL_Component*> &children = comp->get_children();
	
	for( it = children.begin(); it != children.end(); ++it )
	{
		if( (*it)->has_mouse_over() ) return true;
		if( child_has_mouse_over(*it) ) return true;
	}
	
	return false;	
}
