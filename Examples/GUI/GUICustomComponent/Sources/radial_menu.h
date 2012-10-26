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
*/

#pragma once

#include <ClanLib/gui.h>
#include <ClanLib/display.h>
#include <ClanLib/core.h>
using namespace clan;

class RadialMenu : public GUIComponent
{
public:
	RadialMenu(GUIComponent *parent);
	virtual ~RadialMenu();

// Attributes
public:
	Size get_preferred_size() const;

	std::string get_item(int index) const;

// Operations:
public:
	void show(Point &position);

	void clear_items();

	void add_item(const std::string &text);

// Signals and callbacks:
public:
	Callback_v1<int> func_selected;

// Implementation:
private:
	struct RadialMenuItem
	{
		RadialMenuItem(const std::string &text) : text(text) {}
		std::string text;
	};

	void create_parts();

	void on_render(Canvas &canvas, const Rect &update_rect);
	void on_style_changed();
	void on_resized();
	void on_process_message(GUIMessage &msg);
	void on_mouse_move(InputEvent &input_event);
	void on_mouse_lbutton_up(InputEvent &input_event);

	GUIThemePart part_component;

	Sprite image_pointer;

	Font normal_font;
	Colorf normal_text_color;
	float normal_font_height;
	float normal_text_distance;

	Font selected_font;
	Colorf selected_text_color;
	float selected_font_height;
	float selected_text_distance;

	int selected_index;
	Angle selected_angle;
	Angle max_angle;

	int center_x, center_y;

	std::vector<RadialMenuItem> items;
};
