/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

#include "API/Core/System/uniqueptr.h"
#include <memory>

struct CL_TopMenu
{
	CL_String name;
	CL_PopupMenu menu;
};

class CL_MenuBar_Impl
{
public:
	CL_MenuBar_Impl() : selected_index(-1), hot_index(-1)
	{
	}

	void on_process_message(CL_GUIMessage &msg);
	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	void on_resized();
	void on_style_changed();
	void create_parts();

	void select_next();
	void select_previous();
	void select_item_at(const CL_Point &mouse_pos);

	CL_Point get_submenu_screen_pos();

	CL_Rect get_menu_item_rect(int index);

	CL_PopupMenu get_selected_menu();

	int get_selected_item_index();

	void clear_selection();

	CL_MenuBar *menubar;
	CL_GUIThemePart part_component;
	CL_GUIThemePart part_item;

	std::vector<CL_TopMenu> menus;
	int selected_index;
	int hot_index;

	int item_border_left;
	int item_padding_left;
	int item_padding_right;
	int item_border_right;

	CL_UniquePtr<CL_MenuModalLoop> menu_loop;
};


