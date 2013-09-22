/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

class GuiEditorWindow;
class PropertyItem;

class PropertyComponent : public clan::GUIComponent
{
public:
	PropertyComponent(GuiEditorWindow *main_window);
	~PropertyComponent();

	GuiEditorWindow *get_main_window() { return main_window; }

	void set_dialog_size(clan::Size s) { }

	void clear();
	void add_property(PropertyItem *item);

	clan::Rect get_header_resize_grabber() const;

	int hit_test(const clan::Point &pos) const;
	void activate_item(int index);
	void deactivate();

private:
	bool on_input_pressed(const clan::InputEvent &input_event);
	bool on_input_released(const clan::InputEvent &input_event);
	bool on_input_doubleclick(const clan::InputEvent &input_event);
	bool on_input_pointer_moved(const clan::InputEvent &input_event);
	void on_render(clan::Canvas &canvas, const clan::Rect &update_rect);
	void on_selection_changed();
	void on_filter_message(std::shared_ptr<clan::GUIMessage> &message);
	void update_scrollbar_size(clan::Canvas &canvas);
	void update_layout();

	GuiEditorWindow *main_window;
	int name_column_width;
	std::vector<PropertyItem *> items;
	PropertyItem *active_item;
	clan::GUIComponent *active_component;
	clan::ScrollBar *scrollbar;

	
	clan::GUIThemePart part_header;
	clan::GUIThemePart part_header_name;
	clan::GUIThemePart part_header_value;
	clan::GUIThemePart part_row[2];
	clan::GUIThemePart part_item_name;
	clan::GUIThemePart part_item_value;

	PropertyComponentEditState edit_state;

	clan::Rect content;
	clan::Rect header;
	clan::Rect body;
	clan::Rect header_content;
	clan::Rect header_name;
	clan::Rect header_value;
	std::vector<clan::Rect> value_boxes;

	clan::SlotContainer slots;

	friend class PropertyComponentEditStateHeaderSizing;
};
