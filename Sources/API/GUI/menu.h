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

#ifndef header_menu
#define header_menu

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

#include "component.h"
#include "../Core/Math/point.h"

class CL_MenuNode;
class CL_Menu_Generic;
class CL_MenuItem;

//: Menu component
//- !group=GUI/Controls!
//- !header=gui.h!
class CL_API_GUI CL_Menu : public CL_Component
{
//! Construction:
public:
	//: CL_Menu Constructor
	CL_Menu(
		const CL_Point &pos,
		CL_Component *parent,
		CL_StyleManager *style = NULL,
		bool vertical=false);

	//: CL_Menu Constructor
	//: This places the menu at (0, 0).
	CL_Menu(
		CL_Component *parent,
		CL_StyleManager *style = NULL,
		bool vertical=false);

	//: CL_Menu Constructor
	CL_Menu(
		const CL_Rect &rect,
		CL_Component *parent,
		CL_StyleManager *style = NULL,
		bool vertical=false);

	//: CL_Menu Constructor
	CL_Menu(
		CL_MenuNode *parent_node,
		CL_Component *parent,
		CL_StyleManager *style = NULL );
	
	//: CL_Menu Destructor
	virtual ~CL_Menu();

//! Attributes:
public:
	//: Returns true if the menu is open
	bool is_open();

	//: Returns true if the menu is vertical, false if horizontal
	bool is_vertical();

	//: Returns true if the root menu collapses when a click outside the menus takes place.
	//: Ignored by non-root menus.
	bool is_root_collapsing();

	//: Returns true if menu resized automatically when its parent is resized.
	bool is_auto_resize();

	//: Returns true if the mouse cursor is on top of the menu
	bool has_mouse_over();

	//: Returns true if the mouse cursor is in a open submenu of the menu
	bool has_mouse_in_submenus();

	//: Returns the parent node of this menu.
	CL_MenuNode *get_parent_node();
	
	//: Returns the top level parent of the menu tree.
	CL_Menu *get_root_menu();

	//: Returns a list containing the CL_MenuNode's in this menu.
	std::list<CL_MenuNode*> &get_node_list();

	//: Returns the total height of items in a menu
	int get_items_height();

	//: Returns the total width of items in a menu
	int get_items_width();

	//: Returns true if the menus submenus need to be clicked to open.
	//: By default they open on mouse enter for vertical menus and
	//: on click with horizontal menus when it doesn't have focus.
	bool is_click_to_open();

	//: Returns a pointer to the menu in the path given, or 0 if no such menu exsists.
	//param std::string name : name of the menu to get, Ex: "View/Zoom"
	CL_Menu *get_menu(const std::string &name);

	//: Returns a pointer to the menu node in the path given, or 0 if no such menu exsists.
	//param std::string name : name of the menu node to get, Ex: "View/Zoom/1:1"
	CL_MenuNode *get_node(const std::string &name);

	//: Returns a pointer to the menu item (CL_Component*) in the path given, or 0 if no such menu item exsists.
	//param std::string name : name of the menu item to get, Ex: "View/Zoom/1:1"
	CL_Component *get_item(const std::string &name);

//! Signals:
public:
	//: This signal is emitted when a repositioning of the menus is requsted.
	//param int x : x position of topleft of the menu
	//param int y : y position of topleft of the menu
	CL_Signal_v2<int,int> &sig_reposition();
	
//! Operations:
public:
	//: Calculates a position and size for the menu, using code in the GUI theme.
	void reposition();

	//: Calculates a position and size for the menu, using code in the GUI theme.
	//param int x : x position of topleft of the menu
	//param int y : y position of topleft of the menu	
	void reposition(int,int);

	//: Makes the menu resize automatically when its parent is resized
	void set_auto_resize(bool value);

	//: Makes the menu vertical (true) or horizontal (false).
	void set_vertical(bool vertical);

	//: Set weather the root menu should collapse when a click outside the menus takes place.
	//: Ignored by non-root menus.
	void set_root_collapsing(bool collapse);

	//: Add a CL_MenuNode to the menu.
	void add_node(CL_MenuNode *node);
	
	//: Remove a CL_MenuNode from the menu.
	void remove_node(CL_MenuNode *node);

	//: Set the parent node of this menu.
	void set_parent_node(CL_MenuNode *);
	
	//: Open the menu.
	void open();

	//: Close the menu.
	void collapse();

	//: Close all submenus of the menu.
	void collapse_submenus();

	//: Set the opening mode of submenus to open on click (true) or open on mouse over (false)
	void set_click_to_open(bool click);

	//: Convenience function for easy creation of menus.
	//: Creates needed menus, items and nodes automatically
	//param std::string path : path and name of the menu node to create. Ex. "File/Open"
	//param std::string labels : labels to give menu items. Ex. "Fil/Aben". If empty, the path is used for labels.
	CL_Menu *create_menu( const std::string &path, const std::string &labels=std::string());
	
	//: Convenience function for easy creation of menus.
	//: Creates needed menus, items and nodes automatically
	//param std::string path : path of the menu node to create. Ex. "File/Open"
	//param std::string labels : labels to give menu items. Ex. "Fil/Aben". If empty, the path is used for labels.
	CL_MenuNode *create_node( const std::string &path, const std::string &labels=std::string());

	//: Convenience function for easy creation of menus.
	//: Creates needed menus, items and nodes automatically
	//: Returns the parent node of the created item.
	//param std::string path : path the menu item to create. Ex. "File/Open"
	//param std::string labels : labels of the menu items to create. Ex. "Fil/Aben"
	CL_MenuNode *create_item( const std::string &path, const std::string &labels=std::string());

	//: Convenience function for easy creation of menus.
	//: Creates a toggling menu item in the specified path.
	//: Returns the parent node of the created item.
	//param std::string path : path the menu item to create. Ex. "File/Open"
	//param std::string labels : labels of the menu items to create. Ex. "Fil/Aben"
	CL_MenuNode *create_toggle_item( const std::string &path, const std::string &labels=std::string());

//! Implementation:
private:
	CL_Menu(const CL_Menu &copy) : CL_Component(0,0) { return; } // disallow copy construction.
	
	CL_Menu_Generic *impl;

	friend class CL_Menu_Generic;
	friend class CL_MenuNode_Generic;
};

#endif
