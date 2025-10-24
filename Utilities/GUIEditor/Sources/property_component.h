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

#include "property_component_edit_state.h"

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

#ifdef OLD_CODE

#include "callback_info.h"

class HolderComponent;

class PropertyComponent : public CL_GUIComponent
{
//! Construction:
public:
	PropertyComponent(MainWindow *main_window);

//! Attributes:
public:

//! Operations:
public:
	void clear();
	void set_dialog_size(const CL_Size &size);

//! Events:
public:
	void on_selection_changed();

//! Implementation:
private:
	void on_process_message(CL_GUIMessage &msg);
	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);
	void on_resized();
	void on_property_changed(CL_InputEvent event);
	void on_anchoring_changed(CL_RadioButton *rb);
	void on_add_tab(CL_Tab*);
	void on_delete_tab(CL_Tab*);

	void update_property_list(CL_GUIComponent *comp);
	CL_LineEdit *add_field(const CL_StringRef &property_name, const CL_StringRef &value);
	void add_header(const CL_StringRef &header);
	void add_anchor_radios();

	CL_Rect get_child_rect(CL_GUIComponent *parent);
	std::vector<CL_String> split_text_char( const CL_StringRef &text, const CL_StringRef &split_string, bool skip_empty=true);
	int parse_position_equation(const CL_String &equation, const CL_Size &component_size, const CL_Size &parent_size);
	CL_Vec2i parse_position_equations(const CL_String &equ_x, const CL_String &equ_y, const CL_Size &component_size, const CL_Size &parent_size);
	MainWindow *main_window;
	HolderComponent *selected_holder;
	CL_GUIComponent *selected_component;
	CL_GUIComponent *container;
	CL_Size dialog_size;

	std::map<CL_String, CL_LineEdit*> properties;
	CL_SlotContainer slots;
	CallbackInfo callback_info;
};

#endif
