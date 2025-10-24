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
#include "API/gui.h"
#include "API/signals.h"
#include "menu_generic.h"
#include "API/GUI/menu.h"
#include "API/GUI/menu_node.h"
#include "API/Display/mouse.h"

CL_Menu_Generic::CL_Menu_Generic(
	CL_Menu *self,
	bool vertical,
	CL_MenuNode *parent_node)
:
	parent_node(parent_node),
	vertical(vertical),
	menu_open(true),
	auto_resize(true),
	submenu_open(false),
	click_to_open(false),
	collapse_root(false),
	menu(self)
{
	if( parent_node == 0 )
	{
		slots.connect( CL_Mouse::sig_key_up(), this, &CL_Menu_Generic::on_mouse_up);
		click_to_open = true;
	}

	slots.connect( menu->sig_move(), this, &CL_Menu_Generic::on_menu_moved );

	if( vertical ) collapse();
}

CL_Menu_Generic::~CL_Menu_Generic()
{
	// todo: delete?

	std::list<CL_MenuNode *>::iterator it;
	for( it = children.begin(); it != children.end(); ++it )
	{
		delete (*it);
	}
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

CL_Menu *CL_Menu_Generic::get_root_menu()
{
	CL_Menu *test_parent = menu;
	
	while( test_parent != NULL )
	{
		if( test_parent->get_parent_node() == NULL )
			return test_parent;
		else
			test_parent = test_parent->get_parent_node()->get_parent_menu();
	}

	return NULL;
}

int CL_Menu_Generic::get_items_width()
{
	int width = 0;

	std::list<CL_MenuNode*>::iterator it;
	for( it = children.begin(); it != children.end(); ++it )
	{
		const CL_Rect &pos = (*it)->get_data()->get_position();
		int child_width = (*it)->get_data()->get_width();

		if( vertical )
		{
			if( pos.left + child_width > width )
				width = pos.left + child_width;
		}
		else
		{
			width += child_width;
		}

	}
	
	return width;
}

int CL_Menu_Generic::get_items_height()
{
	int height = 0;
	
	if( !children.empty() )
	{
		std::list<CL_MenuNode*>::iterator it;
		for( it = children.begin(); it != children.end(); ++it )
		{
			const CL_Rect &pos = (*it)->get_data()->get_position();

			if( vertical )
			{
				height += pos.get_height();
			}
			else
			{
				if( pos.get_height() > height )
					height = pos.get_height();
			}
		}
	}
	
	return height;
}

bool CL_Menu_Generic::has_mouse_in_submenus()
{
	std::list<CL_MenuNode*>::iterator it;
	for( it = children.begin(); it != children.end(); ++it )
	{
		if( (*it)->has_mouse_over() )
			return true;
	
		if( (*it)->has_mouse_in_submenus() )
			return true;
	}

	return false;
}

bool CL_Menu_Generic::has_mouse_over()
{
	if( menu->CL_Component::has_mouse_over() )
		return true;
	
	std::list<CL_MenuNode*>::iterator it;
	for( it = children.begin(); it != children.end(); ++it )
	{
		if( (*it)->has_mouse_over() )
			return true;
	}
	
	return false;
}

CL_Menu *CL_Menu_Generic::get_menu( std::vector<std::string> path )
{
	bool final_step = false;
	std::string wanted = path.front();

	path.erase(path.begin());
	if( path.empty() )
		final_step = true;
	
	std::list<CL_MenuNode*>::iterator it;
	for( it = children.begin(); it != children.end(); ++it )
	{
		CL_MenuNode *node = (*it);
		
		if( node->has_submenu() )
		{
			if( node->get_name() == wanted )
			{
				if( final_step )
					return node->get_submenu();
				else
					return node->get_submenu()->impl->get_menu(path);
			}
		}
	}

	return 0;
}

CL_MenuNode *CL_Menu_Generic::get_node( std::vector<std::string> path )
{
	bool final_step = false;
	std::string wanted = path.front();
	
	path.erase(path.begin());
	if( path.empty() )
		final_step = true;

	std::list<CL_MenuNode*>::iterator it;
	for( it = children.begin(); it != children.end(); ++it )
	{
		CL_MenuNode *node = (*it);

		if( node->get_name() == wanted )
		{
			if( final_step )
			{
				return node;
			}
			else if( node->has_submenu() )
			{
				return node->get_submenu()->impl->get_node(path);
			}
		}
	}

	return 0;
}

CL_Component *CL_Menu_Generic::get_item( std::vector<std::string> path )
{
	return get_node(path)->get_data();
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_Menu_Generic::add_node(CL_MenuNode *node)
{
	node->set_parent_menu(menu);	
	children.push_back(node);
}

void CL_Menu_Generic::remove_node(CL_MenuNode *node)
{
	std::list<CL_MenuNode *>::iterator it;
	for( it = children.begin(); it != children.end(); ++it )
	{
		if( (*it) == node )
			it = children.erase(it);
	}
	
	CL_Menu *root = get_root_menu();
	CL_Rect pos = root->get_position();
	root->reposition(pos.left, pos.top);
}

void CL_Menu_Generic::open()
{
	menu->show(true);
	menu_open = true;
	if( menu->get_parent_node() )
		menu->get_parent_node()->get_parent_menu()->impl->submenu_open = true;
}

void CL_Menu_Generic::collapse()
{
	collapse_submenus();

	menu->show(false);
	menu_open = false;
}

void CL_Menu_Generic::collapse_submenus()
{
	std::list<CL_MenuNode*>::iterator it;
	for( it = children.begin(); it != children.end(); ++it )
	{
		(*it)->collapse();
	}

	submenu_open = false;
}	

CL_Menu *CL_Menu_Generic::create_menu(
	std::vector<std::string> path,
	std::vector<std::string> labels )
{
	if( path.empty() )
		return menu->get_root_menu();

	bool final_step = false;
	CL_MenuNode *wanted_node = 0;

	std::list<CL_MenuNode*>::iterator it;
	for( it = children.begin(); it != children.end(); ++it )
	{
		if( (*it)->get_name() == path.front() )
		{
			wanted_node = (*it);
		}
	}

	std::string wanted_name = path.front();
	std::string wanted_label = labels.front();

	path.erase(path.begin());
	labels.erase(labels.begin());

	if( path.empty() )
		final_step = true;

	if( wanted_node != 0 )
	{
		if( wanted_node->has_submenu() ) // node & submenu exsists
		{
			if( final_step )
				return wanted_node->get_submenu();
			else
				return wanted_node->get_submenu()->impl->create_menu(path, labels);
		}
		else 
		{
			// submenu doesn't exsist, create one.
			CL_Menu *submenu = new CL_Menu(wanted_node, menu->get_root_menu()->get_parent() );
			
			if( final_step ) return submenu;
			else return submenu->impl->create_menu(path, labels);
		}
	}
	else
	{
		// create node, item & submenu.

		// node
		wanted_node = new CL_MenuNode(menu);
		wanted_node->set_name(wanted_name);

		// item (submenu label)
		CL_MenuItem *item = new CL_MenuItem(wanted_label, wanted_node);

		if( wanted_node->get_parent_menu()->get_parent_node() == 0 )
		{
			item->set_use_icon(false);
			item->find_preferred_size();
		}

		// submenu
		CL_Menu *submenu = new CL_Menu(wanted_node, menu->get_root_menu()->get_parent());
		
		if( final_step ) return submenu;
		else return submenu->impl->create_menu(path,labels);
	}

	return 0;
}


CL_MenuNode *CL_Menu_Generic::create_node(
	std::vector<std::string> path,
	std::vector<std::string> labels )
{
	CL_MenuNode *node = menu->get_root_menu()->impl->get_node(path);

	if( node )
		return node;

	std::string node_name = path.back();
	path.pop_back();
	labels.pop_back();

	CL_Menu *parent_menu = create_menu(path, labels);

	node = new CL_MenuNode(parent_menu);
	node->set_name(node_name);

	return node;
}

CL_MenuNode *CL_Menu_Generic::create_item(
	std::vector<std::string> path,
	std::vector<std::string> labels)
{
	std::string item_name = labels.back();
	CL_MenuNode *parent_node = create_node(path, labels);
	// The newly created item is added to the parent_node in the constructor
	new CL_MenuItem(item_name, parent_node );

	return parent_node;
}

CL_MenuNode *CL_Menu_Generic::create_toggle_item(
	std::vector<std::string> path,
	std::vector<std::string> labels)
{
	CL_MenuNode *node = create_item(path,labels);
	CL_MenuItem *item = static_cast<CL_MenuItem*>(node->get_data());

	item->set_toggling(true);
	node->set_close_on_click(false);

	return node;
}


/////////////////////////////////////////////////////////////////////////////
// Callbacks:

// on_mouse_up() is connected only to the root menu. It will collapse
// the menus if a click outside the menus takes place.
void CL_Menu_Generic::on_mouse_up(const CL_InputEvent&)
{
	if( submenu_open && !menu->has_mouse_in_submenus() )
	{
		collapse_submenus();
		click_to_open = true;
	}

	if( parent_node	== 0 && collapse_root
		&& !menu->has_mouse_over() && !menu->has_mouse_in_submenus()  )
		collapse();
}

void CL_Menu_Generic::on_menu_moved(int old_x, int old_y)
{
	menu->reposition();
}
