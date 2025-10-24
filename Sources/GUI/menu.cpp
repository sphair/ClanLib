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
#include "API/core.h" // todo: remove/narrow?
#include "API/GUI/menu.h"
#include "API/GUI/menu_node.h"
#include "menu_generic.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_Menu::CL_Menu(
	const CL_Point &pos,
	CL_Component *parent,
	CL_StyleManager *style,
	bool vertical)
:
	CL_Component( CL_Rect(pos.x, pos.y, 0, 0), parent, style),
	impl(0)
{
	impl = new CL_Menu_Generic(this, vertical, 0);
	get_style_manager()->connect_styles("menu", this);
}

CL_Menu::CL_Menu(
	CL_Component *parent,
	CL_StyleManager *style,
	bool vertical)
:
	CL_Component( CL_Rect(0, 0, 0, 0), parent, style),
	impl(0)
{
	impl = new CL_Menu_Generic(this, vertical, 0);
	get_style_manager()->connect_styles("menu", this);
}

CL_Menu::CL_Menu(
	const CL_Rect &rect,
	CL_Component *parent,
	CL_StyleManager *style,
	bool vertical)
:
	CL_Component( rect, parent, style),
	impl(0)
{
	impl = new CL_Menu_Generic(this, vertical, 0);
	impl->auto_resize = false;
	get_style_manager()->connect_styles("menu", this);
}

CL_Menu::CL_Menu(
	CL_MenuNode *parent_node,
	CL_Component *parent,
	CL_StyleManager *style)
:
	CL_Component( parent, style),
	impl(0)
{
	impl = new CL_Menu_Generic(this, true, parent_node);
	parent_node->set_submenu(this);
	get_style_manager()->connect_styles("menu", this);
}

CL_Menu::~CL_Menu()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

bool CL_Menu::is_open()
{
	return impl->menu_open;
}

bool CL_Menu::is_vertical()
{
	return impl->vertical;
}

bool CL_Menu::is_root_collapsing()
{
	return impl->collapse_root;
}

bool CL_Menu::is_auto_resize()
{
	return impl->auto_resize;
}

CL_Menu *CL_Menu::get_root_menu()
{
	return impl->get_root_menu();
}

CL_MenuNode *CL_Menu::get_parent_node()
{
	return impl->parent_node;
}

std::list<CL_MenuNode*> &CL_Menu::get_node_list()
{
	// todo: CL_Component::get_children() does the same?
	return impl->children;
}

int CL_Menu::get_items_width()
{
	return impl->get_items_width();
}

int CL_Menu::get_items_height()
{
	return impl->get_items_height();
}

bool CL_Menu::has_mouse_over()
{
	return impl->has_mouse_over();
}

bool CL_Menu::has_mouse_in_submenus()
{
	return impl->has_mouse_in_submenus();
}

bool CL_Menu::is_click_to_open()
{
	return impl->click_to_open;
}

CL_Menu *CL_Menu::get_menu(const std::string &name)
{
	return impl->get_menu(CL_String::tokenize(name, "/"));
}

CL_MenuNode *CL_Menu::get_node(const std::string &name)
{
	return impl->get_node(CL_String::tokenize(name, "/"));
}

CL_Component *CL_Menu::get_item(const std::string &name)
{
	return impl->get_item(CL_String::tokenize(name, "/"));
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_Menu::add_node(CL_MenuNode *node)
{
	impl->add_node(node);
}

void CL_Menu::remove_node(CL_MenuNode *node)
{
	impl->remove_node(node);
}

void CL_Menu::open()
{
	impl->open();
}

void CL_Menu::collapse()
{
	impl->collapse();
}

void CL_Menu::collapse_submenus()
{
	impl->collapse_submenus();
}

void CL_Menu::set_vertical(bool vertical)
{
	impl->vertical = vertical; 
}

void CL_Menu::set_root_collapsing(bool collapse)
{
	impl->collapse_root = collapse;
}

void CL_Menu::set_auto_resize(bool value)
{
	impl->auto_resize = value;
}

void CL_Menu::reposition()
{
	CL_Rect area = get_position(); 

	impl->sig_reposition(area.left, area.top);
}

void CL_Menu::reposition(int x, int y)
{
	impl->sig_reposition(x,y);
}

void CL_Menu::set_parent_node(CL_MenuNode *parent_node)
{
	impl->parent_node = parent_node;
}

void CL_Menu::set_click_to_open(bool click_to_open)
{
	impl->click_to_open = click_to_open;
}

CL_Menu *CL_Menu::create_menu(
	const std::string &path,
	const std::string &labels)
{
	std::vector<std::string> tokens = CL_String::tokenize(path, "/");

	if( labels.empty() )
		return impl->create_menu(tokens, tokens);

	return impl->create_menu(tokens, CL_String::tokenize(labels, "/"));
}

CL_MenuNode *CL_Menu::create_node(
	const std::string &path,
	const std::string &labels)
{
	std::vector<std::string> tokens = CL_String::tokenize(path, "/");

	if( labels.empty() )
		return impl->create_node(tokens, tokens);

	return impl->create_node(tokens, CL_String::tokenize(labels, "/"));
}

CL_MenuNode *CL_Menu::create_item(
	const std::string &path,
	const std::string &labels )
{
	std::vector<std::string> tokens = CL_String::tokenize(path, "/");

	if( labels.empty() )
		return impl->create_item(tokens, tokens);

	return impl->create_item(tokens, CL_String::tokenize(labels, "/"));
}

CL_MenuNode *CL_Menu::create_toggle_item(
	const std::string &path,
	const std::string &labels )
{
	std::vector<std::string> tokens = CL_String::tokenize(path, "/");

	if( labels.empty() )
		return impl->create_toggle_item(tokens, tokens);

	return impl->create_toggle_item(tokens, CL_String::tokenize(labels, "/"));
}


/////////////////////////////////////////////////////////////////////////////
// Signals:

CL_Signal_v2<int,int> &CL_Menu::sig_reposition()
{
	return impl->sig_reposition;
}
