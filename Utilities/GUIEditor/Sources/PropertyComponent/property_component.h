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

#include "EditState/property_component_edit_state.h"

class GuiEditorWindow;
class PropertyItem;

class PropertyComponent : public GUIComponent
{
public:
	PropertyComponent(GuiEditorWindow *main_window);
	~PropertyComponent();

	GuiEditorWindow *get_main_window() { return main_window; }

	void set_dialog_size(Size s) { }

	void clear();
	void add_property(PropertyItem *item);

	Rect get_header_resize_grabber() const;

	int hit_test(const Point &pos) const;
	void activate_item(int index);
	void deactivate();

private:
	bool on_input_pressed(const InputEvent &input_event);
	bool on_input_released(const InputEvent &input_event);
	bool on_input_doubleclick(const InputEvent &input_event);
	bool on_input_pointer_moved(const InputEvent &input_event);
	void on_render(Canvas &canvas, const Rect &update_rect);
	void on_selection_changed();
	void on_filter_message(std::shared_ptr<GUIMessage> &message);
	void update_scrollbar_size(Canvas &canvas);
	void update_layout();

	GuiEditorWindow *main_window;
	int name_column_width;
	std::vector<PropertyItem *> items;
	PropertyItem *active_item;
	GUIComponent *active_component;
	ScrollBar *scrollbar;

	GUIThemePart part_header;
	GUIThemePart part_header_name;
	GUIThemePart part_header_value;
	GUIThemePart part_row[2];
	GUIThemePart part_item_name;
	GUIThemePart part_item_value;

	PropertyComponentEditState edit_state;

	Rect content;
	Rect header;
	Rect body;
	Rect header_content;
	Rect header_name;
	Rect header_value;
	std::vector<Rect> value_boxes;

	SlotContainer slots;

	friend class PropertyComponentEditStateHeaderSizing;
};
