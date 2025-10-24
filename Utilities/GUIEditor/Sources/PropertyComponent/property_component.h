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

#include "EditState/property_component_edit_state.h"

class MainWindow;
class PropertyItem;

class PropertyComponent : public CL_GUIComponent
{
public:
	PropertyComponent(MainWindow *main_window);
	~PropertyComponent();

	MainWindow *get_main_window() { return main_window; }

	void set_dialog_size(CL_Size s) { }

	void clear();
	void add_property(PropertyItem *item);

	CL_Rect get_header_resize_grabber() const;

	int hit_test(const CL_Point &pos) const;
	void activate_item(int index);
	void deactivate();

private:
	bool on_input_pressed(const CL_InputEvent &input_event);
	bool on_input_released(const CL_InputEvent &input_event);
	bool on_input_doubleclick(const CL_InputEvent &input_event);
	bool on_input_pointer_moved(const CL_InputEvent &input_event);
	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	void on_selection_changed();
	void on_filter_message(CL_GUIMessage &message);
	void update_scrollbar_size(CL_GraphicContext &gc);
	void update_layout();

	MainWindow *main_window;
	int name_column_width;
	std::vector<PropertyItem *> items;
	PropertyItem *active_item;
	CL_GUIComponent *active_component;
	CL_ScrollBar *scrollbar;

	CL_GUIThemePart part_background;
	CL_GUIThemePart part_header;
	CL_GUIThemePart part_header_name;
	CL_GUIThemePart part_header_value;
	CL_GUIThemePart part_row[2];
	CL_GUIThemePart part_item_name;
	CL_GUIThemePart part_item_value;

	PropertyComponentEditState edit_state;

	CL_Rect content;
	CL_Rect header;
	CL_Rect body;
	CL_Rect header_content;
	CL_Rect header_name;
	CL_Rect header_value;
	std::vector<CL_Rect> value_boxes;

	CL_SlotContainer slots;

	friend class PropertyComponentEditStateHeaderSizing;
};
