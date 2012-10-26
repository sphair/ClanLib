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

#include <memory>

namespace clan
{

struct TopMenu
{
	std::string name;
	PopupMenu menu;
};

class MenuBar_Impl
{
public:
	MenuBar_Impl() : selected_index(-1), hot_index(-1)
	{
	}

	void on_process_message(GUIMessage &msg);
	void on_render(Canvas &canvas, const Rect &update_rect);
	void on_resized();
	void on_style_changed();
	void create_parts();

	void select_next();
	void select_previous();
	void select_item_at(const Point &mouse_pos);

	Point get_submenu_screen_pos();

	Rect get_menu_item_rect(int index);

	PopupMenu get_selected_menu();

	int get_selected_item_index();

	void clear_selection();

	MenuBar *menubar;
	GUIThemePart part_component;
	GUIThemePart part_item;

	std::vector<TopMenu> menus;
	int selected_index;
	int hot_index;

	int item_border_left;
	int item_padding_left;
	int item_padding_right;
	int item_border_right;

	std::unique_ptr<MenuModalLoop> menu_loop;
};

}
