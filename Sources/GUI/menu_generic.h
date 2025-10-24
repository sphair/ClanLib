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

#ifndef header_menu_generic
#define header_menu_generic

class CL_Menu;
class CL_MenuNode;

class CL_Menu_Generic
{
//! Construction:
 public:
	CL_Menu_Generic(
		CL_Menu *self,
		bool vertical,
		CL_MenuNode *parent_node );
	
	virtual ~CL_Menu_Generic();

//! Attributes:
 public:
	CL_Menu *get_root_menu();
	
	std::list<CL_MenuNode*> children;
	CL_MenuNode *parent_node;

	bool vertical;
	bool menu_open;
	bool auto_resize;
	bool submenu_open;
	bool click_to_open;
	bool collapse_root;
	
	bool has_mouse_in_submenus();
	bool has_mouse_over();

	CL_Menu *get_menu( std::vector<std::string> path);
	CL_MenuNode *get_node( std::vector<std::string> path );
	CL_Component *get_item( std::vector<std::string> path );

//! Signals:
	CL_Signal_v2<int,int> sig_reposition;

//! Operations:
 public:
	void set_vertical(bool vertical);
	void add_node(CL_MenuNode *node);
	void remove_node(CL_MenuNode *node);
	void set_parent_node( CL_MenuNode *node );

	void open();
	void collapse();
	void collapse_submenus();
	
	int get_items_height();
	int get_items_width();

	CL_Menu *create_menu( std::vector<std::string> path, std::vector<std::string> labels );
	CL_MenuNode *create_node( std::vector<std::string> path, std::vector<std::string> labels );
	CL_MenuNode *create_item( std::vector<std::string> path, std::vector<std::string> labels );
	CL_MenuNode *create_toggle_item( std::vector<std::string> path, std::vector<std::string> labels );

//! Callbacks:
 private:
	void on_mouse_up(const CL_InputEvent &e);
	void on_menu_moved(int old_x, int old_y);
	
//! Implementation:
 private:
	CL_Menu *menu;
	
	CL_SlotContainer slots;
};

#endif
