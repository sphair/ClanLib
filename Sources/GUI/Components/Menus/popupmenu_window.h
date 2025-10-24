/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
**    Harry Storbacka
**    Magnus Norddahl
*/

#pragma once


#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_theme_part_property.h"
#include "API/GUI/gui_component_description.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/Components/popupmenu.h"
#include "API/Display/Font/font.h"

class CL_GUIManager;

class CL_PopupMenuWindow : public CL_GUIComponent
{
// Construction:
public:
	CL_PopupMenuWindow(const CL_PopupMenu &menu, const CL_Point &position, CL_GUIComponent *owner);
	~CL_PopupMenuWindow();

// Attributes:
public:
	CL_PopupMenuItem get_selected_item();

	int get_selected_item_index();

	// Returns the position of the submenu of the currently selected item.
	CL_Point get_submenu_screen_position();

	virtual CL_Size get_preferred_size() const;

// Operations:
public:
	void select_previous();

	void select_next();

	void select_item_at(CL_Point mouse_pos);

	// Toggle checkable items and change selection in a radio button group.
	void do_selected_item_special_action();

	void set_item_pressed(bool pressed);

// Implementation:
private:
	static CL_GUITopLevelDescription create_toplevel_description();

	void create_parts();

	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	bool on_close();

	CL_Rect get_item_rect(int index);

	CL_Size calc_desired_size();

	CL_PopupMenu menu;
	int selected;
	bool item_pressed;
	int icon_column_width;
	CL_Size icon_size;
	CL_Size check_size;

	CL_GUIThemePart part_component;
	CL_GUIThemePart part_item_row;
	CL_GUIThemePart part_item_icon;
	CL_GUIThemePart part_item_label;
	CL_GUIThemePart part_item_check;
	CL_GUIThemePart part_item_accel_label;
	CL_GUIThemePart part_separator;
	CL_GUIThemePart part_submenu_arrow;
	CL_GUIThemePart part_menubar_joiner;
	CL_GUIThemePartProperty prop_icon_column_width;
};
