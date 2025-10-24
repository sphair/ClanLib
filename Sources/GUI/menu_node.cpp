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
#include "API/GUI/stylemanager.h"
#include "API/GUI/menu_node.h"
#include "API/GUI/menu.h"
#include "menu_node_generic.h"

/////////////////////////////////////////////////////////////////////////////
// Construction:

CL_MenuNode::CL_MenuNode(
	CL_Menu *parent_menu,
	CL_StyleManager *style)
:
	CL_Component( parent_menu, style ),
	impl(0)
{
	impl = new CL_MenuNode_Generic(this, parent_menu);
	parent_menu->add_node(this);
	get_style_manager()->connect_styles("menu_node", this);
	
}

CL_MenuNode::~CL_MenuNode()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes:

bool CL_MenuNode::has_submenu()
{
	if( impl->submenu )
		return true;

	return false;
}

bool CL_MenuNode::has_open_submenu()
{
	return impl->has_open_submenu();
}

bool CL_MenuNode::has_mouse_over()
{
	return impl->has_mouse_over();
}

bool CL_MenuNode::has_mouse_in_submenus()
{
	return impl->has_mouse_in_submenus();
}

CL_Component *CL_MenuNode::get_data()
{
	return impl->data;
}

CL_Menu *CL_MenuNode::get_parent_menu()
{
	return impl->parent_menu;
}

CL_Menu *CL_MenuNode::get_submenu()
{
	return impl->submenu;
}

std::string CL_MenuNode::get_name()
{
	return impl->name;
}

/////////////////////////////////////////////////////////////////////////////
// Operations:

void CL_MenuNode::collapse()
{
	impl->collapse();
}

void CL_MenuNode::open_submenu()
{
	impl->open_submenu();
}

void CL_MenuNode::set_submenu(CL_Menu *menu)
{
	impl->set_submenu(menu);
}

void CL_MenuNode::set_name(std::string name)
{
	impl->name = name;
}

void CL_MenuNode::set_parent_menu(CL_Menu *menu)
{
	impl->set_parent_menu(menu);
}

void CL_MenuNode::set_close_on_click(bool close)
{
	impl->close_on_click = close;
}

void CL_MenuNode::reposition_submenus()
{
	impl->sig_reposition();
}

/////////////////////////////////////////////////////////////////////////////
// Signals:

CL_Signal_v0 &CL_MenuNode::sig_reposition()
{
	return impl->sig_reposition;
}

CL_Signal_v0 &CL_MenuNode::sig_clicked()
{
	return impl->sig_clicked;
}
