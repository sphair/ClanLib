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

//! clanGUI="Controls"
//! header=gui.h

#ifndef header_menu_node
#define header_menu_node

#ifdef CL_API_DLL
#ifdef CL_GUI_EXPORT
#define CL_API_GUI __declspec(dllexport)
#else
#define CL_API_GUI __declspec(dllimport)
#endif
#else
#define CL_API_GUI
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "../GUI/component.h"

class CL_Menu;
class CL_MenuNode_Generic;

//: MenuNode component
//- !group=GUI/Controls!
//- !header=gui.h!
class CL_API_GUI CL_MenuNode : public CL_Component
{
//! Construction:
public:
	//: CL_MenuNode Constructor
	CL_MenuNode(
		CL_Menu *parent_menu,
		CL_StyleManager *style = NULL);

	//: CL_MenuNode destructor
	virtual ~CL_MenuNode();

//! Attributes:
public:
	//: Returns a pointer to the CL_Component which is this MenuNodes MenuItem.
	CL_Component *get_data();
	
	//: Returns true if the mouse is in the submenus of this node.
	bool has_mouse_in_submenus();
	
	//: Returns true if the mouse is on this component or it's MenuItem
	//: This overrides CL_Component's has_mouse_over()
	bool has_mouse_over();
	
	//: Returns true if the node has a submenu.
	bool has_submenu();

	//: Returns true if the node has an open submenu.
	bool has_open_submenu();
	
	//: Returns the nodes submenu, or NULL if the node has no submenu.
	CL_Menu *get_submenu();
	
	//: Returns the nodes parent menu.
	CL_Menu *get_parent_menu();

	//: Returns the name of the menu node. 
	//: A menu nodes name forms a part of the path used by the CL_Menu::get_<menu/node/item>(std::string path) functions.
	std::string get_name();

//! Operations:
public:
	//: Set the nodes submenu.
	void set_submenu(CL_Menu *menu);

	//: Set the nodes parent menu.
	void set_parent_menu(CL_Menu *menu);

	//: Set whether the menus should collapse when an node is clicked
	void set_close_on_click(bool close);
	
	//: Close the nodes submenu.
	void collapse();

	//: Open the nodes submenu.
	void open_submenu();

	//: Start the repositioning of the nodes submenus.
	void reposition_submenus();

	//: Set the name of the node. This forms a part of the path used in adressing menu components.
	void set_name(std::string);

//! Signals:
public:
	//: Emitted when repositioning of the menus is requested.
	CL_Signal_v0 &sig_reposition();

	//: Emitted when a menu node is clicked.
	CL_Signal_v0 &sig_clicked();

//! Implementation:
private:
	CL_MenuNode(const CL_MenuNode &copy) : CL_Component(NULL, NULL) { return; } // disallow copy construction.

	CL_MenuNode_Generic *impl;
};

#endif
