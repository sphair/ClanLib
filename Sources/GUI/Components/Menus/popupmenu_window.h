/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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

#include "API/GUI/gui_component_description.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/Components/popupmenu.h"
#include "API/Display/Font/font.h"

namespace clan
{

class GUIManager;

class PopupMenuWindow : public GUIComponent
{
// Construction:
public:
	PopupMenuWindow(const PopupMenu &menu, const Point &position, GUIComponent *owner);
	~PopupMenuWindow();

// Attributes:
public:
	PopupMenuItem get_selected_item();

	int get_selected_item_index();

	// Returns the position of the submenu of the currently selected item.
	Point get_submenu_screen_position();

	virtual Size get_preferred_size() const;

// Operations:
public:
	void select_previous();

	void select_next();

	void select_item_at(Point mouse_pos);

	// Toggle checkable items and change selection in a radio button group.
	void do_selected_item_special_action();

	void set_item_pressed(bool pressed);

// Implementation:
private:
	static GUITopLevelDescription create_toplevel_description();

	void create_parts();

	void on_render(Canvas &canvas, const Rect &update_rect);
	bool on_close();

	Rect get_item_rect(int index);

	Size calc_desired_size();

	PopupMenu menu;
	int selected;
	bool item_pressed;
	int icon_column_width;
	Size icon_size;
	Size check_size;

	//FIXME: GUIThemePart part_component;
	//FIXME: GUIThemePart part_item_row;
	//FIXME: GUIThemePart part_item_icon;
	//FIXME: GUIThemePart part_item_label;
	//FIXME: GUIThemePart part_item_check;
	//FIXME: GUIThemePart part_item_accel_label;
	//FIXME: GUIThemePart part_separator;
	//FIXME: GUIThemePart part_submenu_arrow;
	//FIXME: GUIThemePart part_menubar_joiner;
	//GUIThemePartProperty prop_icon_column_width;
};

}
