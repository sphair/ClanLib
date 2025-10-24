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
*/

#pragma once

#include <ClanLib/gui.h>
#include <ClanLib/display.h>
#include <ClanLib/core.h>

class RadialMenu : public CL_GUIComponent
{
public:
	RadialMenu(CL_GUIComponent *parent);
	virtual ~RadialMenu();

// Attributes
public:
	CL_Size get_preferred_size() const;

	CL_String get_item(int index) const;

// Operations:
public:
	void show(CL_Point &position);

	void clear_items();

	void add_item(const CL_String &text);

// Signals and callbacks:
public:
	CL_Callback_v1<int> func_selected;

// Implementation:
private:
	struct RadialMenuItem
	{
		RadialMenuItem(const CL_String &text) : text(text) {}
		CL_String text;
	};

	void create_parts();

	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	void on_style_changed();
	void on_resized();
	void on_process_message(CL_GUIMessage &msg);
	void on_mouse_move(CL_InputEvent &input_event);
	void on_mouse_lbutton_up(CL_InputEvent &input_event);

	CL_GUIThemePart part_component;

	CL_Sprite image_pointer;

	CL_Font normal_font;
	CL_Colorf normal_text_color;
	float normal_font_height;
	float normal_text_distance;

	CL_Font selected_font;
	CL_Colorf selected_text_color;
	float selected_font_height;
	float selected_text_distance;

	int selected_index;
	CL_Angle selected_angle;
	CL_Angle max_angle;

	int center_x, center_y;

	std::vector<RadialMenuItem> items;
};
