/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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
public:
	CL_PopupMenuWindow(const CL_PopupMenu &menu, const CL_Point &position, CL_GUIComponent *owner, CL_GUIManager *manager);
	~CL_PopupMenuWindow();

	CL_PopupMenuWindow *get_owner_popup() const { return owner_popup; }
	CL_PopupMenuWindow *get_child_popup() const { return child_popup; }
	virtual CL_Size get_preferred_size() const;
	CL_Rect get_item_rect(int index);
	void set_clicked_item(CL_PopupMenuItem &item);
	CL_PopupMenuItem get_clicked_item() { return clicked_item; }
	CL_PopupMenuWindow *get_root_popup();

private:
	void create_parts();
	void on_process_message(CL_GUIMessage &msg);
	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	void set_owner_popup(CL_PopupMenuWindow *popup);

	CL_Size calc_desired_size();
	static CL_GUITopLevelDescription create_toplevel_description();

	CL_PopupMenu menu;
	int selected;
	bool keyboard_selected;
	bool mouse_pressed;

	CL_GUIThemePart part_component;
	CL_GUIThemePart part_item_row;
	CL_GUIThemePart part_item_icon;
	CL_GUIThemePart part_item_label;
	CL_GUIThemePart part_item_accel_label;
	CL_GUIThemePart part_separator;
	CL_GUIThemePart part_submenu_arrow;
	CL_GUIThemePart part_menubar_joiner;
	CL_GUIThemePartProperty prop_text_color;
	CL_GUIThemePartProperty prop_text_color_disabled;

	CL_PopupMenuWindow *child_popup;
	CL_PopupMenuWindow *owner_popup;
	CL_Font font;
	CL_Colorf text_color;
	CL_Colorf text_color_disabled;
	CL_PopupMenuItem clicked_item;
};
