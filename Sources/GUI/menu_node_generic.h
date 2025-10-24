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

#ifndef header_menu_node_generic
#define header_menu_node_generic

#include "API/gui.h"


class CL_MenuNode;
class CL_Menu;
class CL_Menu_Generic;

class CL_MenuNode_Generic
{
 public:
	// construction
	CL_MenuNode_Generic( CL_MenuNode *self, CL_Menu *parent_menu);
	virtual ~CL_MenuNode_Generic();

	// attributes
	CL_Component *data;
	CL_Menu *submenu;
	CL_Menu *parent_menu;
	std::string name;
	bool close_on_click;

	bool mouse_in_submenus();
	bool has_mouse_over();
	bool has_mouse_in_submenus();
	bool has_open_submenu();

	bool child_has_mouse_over(const CL_Component *comp) const;

	// operations
	void set_submenu(CL_Menu *menu);
	void set_parent_menu(CL_Menu *menu);
	void open_submenu();
	void collapse();
	void collapse_submenus();
	void reposition();

	// callbacks
	void on_mouse_down(const CL_InputEvent &e);
	void on_mouse_up(const CL_InputEvent &e);
	void on_mouse_enter();
	void on_child_add(CL_Component *child);

	// signals:
	CL_Signal_v0 sig_reposition;
	CL_Signal_v0 sig_clicked;

 private:
	// implementation
	CL_MenuNode *node;
	bool close_lock;

	CL_SlotContainer slots;

	friend class CL_Menu;
};

#endif
