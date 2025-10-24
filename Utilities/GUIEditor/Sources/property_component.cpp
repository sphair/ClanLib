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

#include "precomp.h"
#include "property_component.h"
#include "property_item.h"
#include "holder_component.h"
#include "grid_component.h"
#include "custom_component.h"
#include "selection.h"
#include "main_window.h"
#include "property_item_position.h"
#include "property_item_text.h"
#include "property_item_group_name.h"
#include "property_item_class_name.h"
#include "property_item_id_name.h"
#include "property_item_min.h"
#include "property_item_max.h"
#include "property_item_tick_count.h"
#include "property_item_step_size.h"
#include "property_item_page_label.h"
#include "property_item_type_name.h"

PropertyComponent::PropertyComponent(MainWindow *main_window)
: CL_GUIComponent(main_window), main_window(main_window), name_column_width(75), active_item(0), active_component(0), scrollbar(0)
{
	set_type_name("property-component");
	set_clip_children(true);
	part_background = CL_GUIThemePart(this);
	part_header = CL_GUIThemePart(this, "header");
	part_header_name = CL_GUIThemePart(this, "header name");
	part_header_value = CL_GUIThemePart(this, "header value");
	part_row[0] = CL_GUIThemePart(this, "row");
	part_row[1] = CL_GUIThemePart(this, "row");
	part_row[1].set_state("alternate", true);
	part_item_name = CL_GUIThemePart(this, "item name");
	part_item_value = CL_GUIThemePart(this, "item value");

	func_input_pressed().set(this, &PropertyComponent::on_input_pressed);
	func_input_released().set(this, &PropertyComponent::on_input_released);
	func_input_doubleclick().set(this, &PropertyComponent::on_input_doubleclick);
	func_input_pointer_moved().set(this, &PropertyComponent::on_input_pointer_moved);
	func_render().set(this, &PropertyComponent::on_render);

	scrollbar = new CL_ScrollBar(this);
	scrollbar->set_vertical();
	scrollbar->set_visible(false);

	edit_state.set_component(this);

	slots.connect(main_window->get_selection()->sig_selection_changed(), this, &PropertyComponent::on_selection_changed);
	slots.connect(get_gui_manager().sig_filter_message(), this, &PropertyComponent::on_filter_message);
}

PropertyComponent::~PropertyComponent()
{
	clear();
}

void PropertyComponent::clear()
{
	deactivate();
	for (size_t i = 0; i < items.size(); i++)
		delete items[i];
	items.clear();
	request_repaint();
}

void PropertyComponent::add_property(PropertyItem *item)
{
	item->property_component = this;
	items.push_back(item);
	request_repaint();
}

void PropertyComponent::on_selection_changed()
{
	clear();
	std::vector<HolderComponent *> selection = main_window->get_selection()->get_selection();
	if (!selection.empty())
	{
		HolderComponent *item = selection[0];
		CL_GUIComponent *comp = item->get_component();

		// add_property(new PropertyItemHeader("Properties"));

		CL_String type = comp->get_type_name();

		if (type != "grid")
		{
			CL_Size comp_size = comp->get_geometry().get_size();
			bool enable_width = true;
			bool enable_height = true;

			HolderComponent *holder = dynamic_cast<HolderComponent*>(comp->get_parent_component());
			if (holder)
			{
				add_property(new PropertyItemPosition(PropertyItemPosition::type_x1));
				add_property(new PropertyItemPosition(PropertyItemPosition::type_y1));
				add_property(new PropertyItemPosition(PropertyItemPosition::type_x2));
				add_property(new PropertyItemPosition(PropertyItemPosition::type_y2));
/*
				CL_String equ_x = holder->get_position_equation_x();
				CL_String equ_y = holder->get_position_equation_y();

				if (equ_x.empty())
					add_property(new PropertyItemPosition("x1", CL_StringHelp::int_to_text(holder->get_geometry().left)));
				else
					add_property(new PropertyItemPosition("x1", equ_x));

				if (equ_y.empty())
					add_property(new PropertyItemPosition("y1", CL_StringHelp::int_to_text(holder->get_geometry().top)));
				else
					add_property(new PropertyItemPosition("y1", equ_y));


				CL_String equ_x2 = holder->get_position_equation_x2();
				CL_String equ_y2 = holder->get_position_equation_y2();

				add_property(new PropertyItemLineEdit("x2", equ_x2));
				add_property(new PropertyItemLineEdit("y2", equ_y2));

				if (!equ_x2.empty())
					enable_width = false;
				if (!equ_y2.empty())
					enable_height = false;
*/
			}

			add_property(new PropertyItemPosition(PropertyItemPosition::type_width));
			add_property(new PropertyItemPosition(PropertyItemPosition::type_height));
			//add_property(new PropertyItemLineEdit("Width", CL_StringHelp::int_to_text(comp_size.width)));
			//add_property(new PropertyItemLineEdit("Height", CL_StringHelp::int_to_text(comp_size.height)));
			// le_w->set_enabled(enable_width);
			// le_h->set_enabled(enable_height);
		}

		add_property(new PropertyItemText());

		if (type == "button")
		{
			CL_PushButton *co = dynamic_cast<CL_PushButton*>(comp);
			//add_property(new PropertyItemLineEdit("Text", co->get_text()));
		}
		else if (type == "lineedit")
		{
			CL_LineEdit *co = dynamic_cast<CL_LineEdit*>(comp);
			//add_property(new PropertyItemLineEdit("Text",co->get_text()));
		}
		else if (type == "checkbox")
		{
			CL_CheckBox *co = dynamic_cast<CL_CheckBox*>(comp);
			//add_property(new PropertyItemLineEdit("Text", co->get_text()));
		}
		else if (type == "radiobutton")
		{
			CL_RadioButton *co = dynamic_cast<CL_RadioButton*>(comp);
			//add_property(new PropertyItemLineEdit("Text", co->get_text()));
			//add_property(new PropertyItemLineEdit("Group Name", co->get_group_name()));
			add_property(new PropertyItemGroupName());
		}
		else if (type == "label")
		{
			CL_Label *co = dynamic_cast<CL_Label*>(comp);
			//add_property(new PropertyItemLineEdit("Text", co->get_text()));
		}
		else if (type == "statusbar")
		{
			CL_StatusBar *co = dynamic_cast<CL_StatusBar*>(comp);
		}
		else if (type == "frame")
		{
			CL_Frame *co = dynamic_cast<CL_Frame*>(comp);
			//add_property(new PropertyItemLineEdit("Text", co->get_header_text()));
		}
		else if (type == "slider")
		{
			CL_Slider *co = dynamic_cast<CL_Slider*>(comp);
			add_property(new PropertyItemMin());
			add_property(new PropertyItemMax());
			add_property(new PropertyItemTickCount());
			add_property(new PropertyItemStepSize());
			//add_property(new PropertyItemLineEdit("Min", CL_StringHelp::int_to_text(co->get_min())));
			//add_property(new PropertyItemLineEdit("Max", CL_StringHelp::int_to_text(co->get_max())));
			//add_property(new PropertyItemLineEdit("Tick Count", CL_StringHelp::int_to_text(co->get_tick_count())));
			//add_property(new PropertyItemLineEdit("Step Size", CL_StringHelp::int_to_text(co->get_page_step())));
		}
		else if (type == "grid")
		{
			GridComponent *co = dynamic_cast<GridComponent*>(comp);
			CL_Size s = co->get_dialog_size();
			//add_property(new PropertyItemLineEdit("Width", CL_StringHelp::int_to_text(s.width)));
			//add_property(new PropertyItemLineEdit("Height", CL_StringHelp::int_to_text(s.height)));
		}
		else if (type == "menubar")
		{
		}
		else if (type == "spin")
		{
		}
		else if (type == "statusbar")
		{
		}
		else if (type == "combobox")
		{
		}
		else if (type == "listview")
		{
		}
		else if (type == "tab")
		{
			CL_Tab *tab = dynamic_cast<CL_Tab*>(comp);
			int current_page = tab->get_current_page_index();
			CL_TabPage *tab_page = tab->get_page(current_page);
			add_property(new PropertyItemPageLabel());
			//add_property(new PropertyItemLineEdit("Page label", tab_page->get_label()));
		}
		else
		{
			add_property(new PropertyItemTypeName());
			//add_property(new PropertyItemLineEdit("Type", comp->get_type_name()));
		}

		add_property(new PropertyItemIdName());
		add_property(new PropertyItemClassName());
		//add_property(new PropertyItemLineEdit("#id", comp->get_id_name()));
		//add_property(new PropertyItemLineEdit(".class", comp->get_class_name()));

		add_property(new PropertyItemAnchor());
/*
		// Layout
		if (type != "grid")
		{
			add_anchor_radios();
		}

		// Other non-lineedit controls go here...
		if (type == "tab")
		{
			CL_Rect child_rect = get_child_rect(container);

			CL_PushButton *add_tab = new CL_PushButton(container);
			add_tab->set_geometry(CL_RectPS(child_rect.left, child_rect.bottom+10, 80,24));
			add_tab->set_text("Add tab");

			CL_PushButton *del_tab = new CL_PushButton(container);
			del_tab->set_geometry(CL_RectPS(child_rect.left, child_rect.bottom+10+32, 80,24));
			del_tab->set_text("Delete tab");

			CL_Tab *tab = dynamic_cast<CL_Tab*>(comp);
			add_tab->func_clicked().set(this, &PropertyComponent::on_add_tab, tab);
			del_tab->func_clicked().set(this, &PropertyComponent::on_delete_tab, tab);
		}

		// Callback functions:
		add_property(new PropertyItemHeader(type + " Callbacks:"));
		std::vector<CallbackData> callbacks = callback_info.get_callbacks(type);
		std::vector<CallbackData>::iterator it_cb;
		for (it_cb = callbacks.begin(); it_cb != callbacks.end(); ++it_cb)
		{
			CallbackData &cb = (*it_cb);
			add_property(new PropertyItemLineEdit(cb.get_callback_name(), cb.get_handler_function_name()));
		}

		add_header("Component Callbacks:");
		callbacks = callback_info.get_callbacks("gui_component");
		for (it_cb = callbacks.begin(); it_cb != callbacks.end(); ++it_cb)
		{
			CallbackData &cb = (*it_cb);
			add_property(new PropertyItemLineEdit(cb.get_callback_name(), cb.get_handler_function_name()));
		}
*/

		for (size_t i = 0; i < items.size(); i++)
			items[i]->selection_changed(selection);
		request_repaint();
	}
}

bool PropertyComponent::on_input_pressed(const CL_InputEvent &input_event)
{
	return edit_state.on_input_pressed(input_event);
}

bool PropertyComponent::on_input_released(const CL_InputEvent &input_event)
{
	return edit_state.on_input_released(input_event);
}

bool PropertyComponent::on_input_doubleclick(const CL_InputEvent &input_event)
{
	return edit_state.on_input_doubleclick(input_event);
}

bool PropertyComponent::on_input_pointer_moved(const CL_InputEvent &input_event)
{
	return edit_state.on_input_pointer_moved(input_event);
}

void PropertyComponent::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	update_layout();
	update_scrollbar_size(gc);

	part_background.render_box(gc, get_size(), update_rect);
	part_header.render_box(gc, header, update_rect);
	part_header_name.render_box(gc, header_name, update_rect);
	part_header_value.render_box(gc, header_value, update_rect);
	part_header_name.render_text(gc, "Property", part_header_name.get_content_box(header_name), update_rect);
	part_header_value.render_text(gc, "Value", part_header_value.get_content_box(header_value), update_rect);

	value_boxes.clear();
	int row_width = content.get_width(); // to do: account for shrink box
	int y = body.top;
	for (size_t i = scrollbar->get_position(); i < items.size() && y < body.bottom; i++)
	{
		int row_height = items[i]->get_inactive_height(gc, part_item_value, row_width-name_column_width);
		if (row_height < part_item_name.get_preferred_height())
			row_height = part_item_name.get_preferred_height();
		CL_Rect row(body.left, y, body.right, y+row_height);
		CL_Rect row_content = part_row[i%2].get_content_box(row);
		CL_Rect name_box(row_content.left, row_content.top, row_content.left+name_column_width, row_content.bottom);
		CL_Rect value_box(row_content.left+name_column_width, row_content.top, row_content.right, row_content.bottom);
		part_row[i%2].render_box(gc, row, update_rect);
		part_item_name.render_box(gc, name_box, update_rect);
		part_item_name.render_text(gc, items[i]->name, part_item_name.get_content_box(name_box), update_rect);
		part_item_value.render_box(gc, value_box, update_rect);
		value_boxes.push_back(value_box);

		if (items[i] != active_item || active_component == 0)
			items[i]->render_inactive(gc, part_item_value, value_box, update_rect);
		else
			active_component->set_geometry(value_box);
		y = row.bottom;
	}
}

void PropertyComponent::deactivate()
{
	if (active_item)
	{
		active_item->deactivate(active_component);
		std::vector<HolderComponent *> selection = main_window->get_selection()->get_selection();
		for (size_t i = 0; i < selection.size(); i++)
		{
			try
			{
				active_item->apply_changes(selection[i]);
			}
			catch (CL_Exception &)
			{
				// to do: display a message box here perhaps?
			}
		}

		active_item = 0;
		active_component = 0;
		request_repaint();
		main_window->get_grid_component()->request_repaint();
	}
}

int PropertyComponent::hit_test(const CL_Point &pos) const
{
	for (size_t i = 0; i < value_boxes.size(); i++)
	{
		if (value_boxes[i].contains(pos))
			return i;
	}
	return -1;
}

void PropertyComponent::activate_item(int index)
{
	if (items[index] != active_item)
	{
		deactivate();
		active_item = items[index];
		active_component = items[index]->activate();
		if(active_component)
			active_component->set_focus();
		request_repaint();
	}
}

void PropertyComponent::on_filter_message(CL_GUIMessage &message)
{
	if (active_component)
	{
		if (message.get_type() == CL_GUIMessage_Input::get_type_name())
		{
			CL_GUIMessage_Input input_message = message;
			CL_InputEvent e = input_message.get_event();
			if (e.type == CL_InputEvent::pressed && e.device.get_type() == CL_InputDevice::pointer)
			{
				if (active_component != message.get_target() && !active_component->is_ancestor_of(message.get_target()))
					deactivate();
			}
		}
	}
}

void PropertyComponent::update_layout()
{
	CL_Rect geometry = get_size();
	content = part_background.get_content_box(geometry);
	header = CL_Rect(content.left, content.top, content.right, content.top+part_header.get_preferred_height());
	body = CL_Rect(content.left, header.bottom, content.right, content.bottom);
	header_content = part_header.get_content_box(header);
	header_name = CL_Rect(header_content.left, header_content.top, header_content.left+name_column_width, header_content.bottom);
	header_value = CL_Rect(header_name.right, header_content.top, header_content.right, header_content.bottom);
}

void PropertyComponent::update_scrollbar_size(CL_GraphicContext &gc)
{
	int row_width = content.get_width(); // to do: account for shrink box
	int y = content.bottom;
	size_t i;
	for (i = items.size(); i > 0 && y > content.top; i--)
		y -= items[i-1]->get_inactive_height(gc, part_item_value, row_width-name_column_width);

	int page_step = items.size() - i;
	if (page_step > 0)
	{
		scrollbar->set_ranges(0, items.size()/*-page_step+1*/, 1, page_step);
		scrollbar->set_visible(true);
	}
	else
	{
		scrollbar->set_ranges(0, 1, 1, 1);
		scrollbar->set_visible(false);
	}
}

CL_Rect PropertyComponent::get_header_resize_grabber() const
{
	return CL_Rect(header_name.right-3, header.top, header_name.right+2, header.bottom);
}

#ifdef OLD_CODE

/////////////////////////////////////////////////////////////////////////////
// PropertyComponent Construction:

PropertyComponent::PropertyComponent(MainWindow *main_window)
: CL_GUIComponent(main_window), main_window(main_window), selected_component(0),
  container(0)
{
	func_process_message().set(this, &PropertyComponent::on_process_message);
	func_render().set(this, &PropertyComponent::on_render);
	func_resized().set(this, &PropertyComponent::on_resized);

	set_clip_children(true);

	container = new CL_GUIComponent(this);
	slots.connect(main_window->get_selection()->sig_selection_changed(), this, &PropertyComponent::on_selection_changed);
}

/////////////////////////////////////////////////////////////////////////////
// PropertyComponent Attributes:

/////////////////////////////////////////////////////////////////////////////
// PropertyComponent Operations:

void PropertyComponent::clear()
{
	delete container;
	container = 0;
}

void PropertyComponent::on_selection_changed()
{
	std::vector<HolderComponent*> selected_holders = main_window->get_selection()->get_selection();

	if (selected_holders.empty())
	{
		update_property_list(0);
		selected_component = 0;
		selected_holder = 0;
		request_repaint();
		return;
	}

	selected_holder = selected_holders.back();
	callback_info = selected_holder->get_callback_info();
	selected_component = selected_holder->get_first_child();
	update_property_list(selected_component);
	request_repaint();
}

/////////////////////////////////////////////////////////////////////////////
// PropertyComponent Implementation:

void PropertyComponent::on_process_message(CL_GUIMessage &msg)
{
/*	if (msg.is_type(CL_GUIMessage_Input::get_type_name()))
	{
		CL_GUIMessage_Input input_msg = msg;
		CL_InputEvent e = input_msg.get_event();

		if (e.type == CL_InputEvent::pressed && e.id == CL_MOUSE_LEFT)
		{
			if (boundary_outer.contains(e.mouse_pos) && !boundary_inner.is_inside(e.mouse_pos))
			{
				capture_mouse(true);
				moving_boundary = true;
			}
		}
	}
*/
}

void PropertyComponent::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	CL_Rect g = get_geometry().get_size();
}

void PropertyComponent::on_resized()
{
	if (container)
		container->set_geometry(get_geometry().get_size());
}

void PropertyComponent::update_property_list(CL_GUIComponent *comp)
{
	delete container;
	container = 0;

	if (comp == 0)
	{
		return;
	}

	container = new CL_GUIComponent(this);
	container->set_geometry(get_geometry().get_size());

	add_header("Properties");

	CL_String type = comp->get_type_name();

	if (type != "grid")
	{
		CL_Size comp_size = comp->get_geometry().get_size();
		bool enable_width = true;
		bool enable_height = true;

		HolderComponent *holder = dynamic_cast<HolderComponent*>(comp->get_parent_component());
		if (holder)
		{
			CL_String equ_x = holder->get_position_equation_x();
			CL_String equ_y = holder->get_position_equation_y();

			if (equ_x.empty())
				add_field("x1", CL_StringHelp::int_to_text(holder->get_geometry().left));
			else
				add_field("x1", equ_x);

			if (equ_y.empty())
				add_field("y1", CL_StringHelp::int_to_text(holder->get_geometry().top));
			else
				add_field("y1", equ_y);


			CL_String equ_x2 = holder->get_position_equation_x2();
			CL_String equ_y2 = holder->get_position_equation_y2();

			add_field("x2", equ_x2);
			add_field("y2", equ_y2);

			if (!equ_x2.empty())
				enable_width = false;
			if (!equ_y2.empty())
				enable_height = false;
		}

		CL_LineEdit *le_w = add_field("Width", CL_StringHelp::int_to_text(comp_size.width));
		CL_LineEdit *le_h = add_field("Height", CL_StringHelp::int_to_text(comp_size.height));
		le_w->set_enabled(enable_width);
		le_h->set_enabled(enable_height);
	}

	if (type == "button")
	{
		CL_PushButton *co = dynamic_cast<CL_PushButton*>(comp);
		add_field("Text", co->get_text());
	}
	else if (type == "lineedit")
	{
		CL_LineEdit *co = dynamic_cast<CL_LineEdit*>(comp);
		add_field("Text", co->get_text());
	}
	else if (type == "checkbox")
	{
		CL_CheckBox *co = dynamic_cast<CL_CheckBox*>(comp);
		add_field("Text", co->get_text());
	}
	else if (type == "radiobutton")
	{
		CL_RadioButton *co = dynamic_cast<CL_RadioButton*>(comp);
		add_field("Text", co->get_text());
		add_field("Group Name", co->get_group_name());
	}
	else if (type == "label")
	{
		CL_Label *co = dynamic_cast<CL_Label*>(comp);
		add_field("Text", co->get_text());
	}
	else if (type == "statusbar")
	{
		CL_StatusBar *co = dynamic_cast<CL_StatusBar*>(comp);
	}
	else if (type == "frame")
	{
		CL_Frame *co = dynamic_cast<CL_Frame*>(comp);
		add_field("Text", co->get_header_text());
	}
	else if (type == "slider")
	{
		CL_Slider *co = dynamic_cast<CL_Slider*>(comp);
		add_field("Min", CL_StringHelp::int_to_text(co->get_min()));
		add_field("Max", CL_StringHelp::int_to_text(co->get_max()));
		add_field("Tick Count", CL_StringHelp::int_to_text(co->get_tick_count()));
		add_field("Step Size", CL_StringHelp::int_to_text(co->get_page_step()));
	}
	else if (type == "grid")
	{
		GridComponent *co = dynamic_cast<GridComponent*>(comp);
		CL_Size s = co->get_dialog_size();
		add_field("Width", CL_StringHelp::int_to_text(s.width));
		add_field("Height", CL_StringHelp::int_to_text(s.height));
	}
	else if (type == "menubar")
	{
	}
	else if (type == "spin")
	{
	}
	else if (type == "statusbar")
	{
	}
	else if (type == "combobox")
	{
	}
	else if (type == "listview")
	{
	}
	else if (type == "tab")
	{
		CL_Tab *tab = dynamic_cast<CL_Tab*>(comp);
		int current_page = tab->get_current_page_index();
		CL_TabPage *tab_page = tab->get_page(current_page);
		add_field("Page label", tab_page->get_label());
	}
	else
	{
		add_field("Type", comp->get_type_name());
	}

	add_field("#id", comp->get_id_name());
	add_field(".class", comp->get_class_name());

	// Layout
	if (type != "grid")
	{
		add_anchor_radios();
	}

	// Other non-lineedit controls go here...
	if (type == "tab")
	{
		CL_Rect child_rect = get_child_rect(container);

		CL_PushButton *add_tab = new CL_PushButton(container);
		add_tab->set_geometry(CL_RectPS(child_rect.left, child_rect.bottom+10, 80,24));
		add_tab->set_text("Add tab");

		CL_PushButton *del_tab = new CL_PushButton(container);
		del_tab->set_geometry(CL_RectPS(child_rect.left, child_rect.bottom+10+32, 80,24));
		del_tab->set_text("Delete tab");

		CL_Tab *tab = dynamic_cast<CL_Tab*>(comp);
		add_tab->func_clicked().set(this, &PropertyComponent::on_add_tab, tab);
		del_tab->func_clicked().set(this, &PropertyComponent::on_delete_tab, tab);
	}

	// Callback functions:
	add_header(type + " Callbacks:");
	std::vector<CallbackData> callbacks = callback_info.get_callbacks(type);
	std::vector<CallbackData>::iterator it_cb;
	for (it_cb = callbacks.begin(); it_cb != callbacks.end(); ++it_cb)
	{
		CallbackData &cb = (*it_cb);
		add_field(cb.get_callback_name(), cb.get_handler_function_name());
	}

	add_header("Component Callbacks:");
	callbacks = callback_info.get_callbacks("gui_component");
	for (it_cb = callbacks.begin(); it_cb != callbacks.end(); ++it_cb)
	{
		CallbackData &cb = (*it_cb);
		add_field(cb.get_callback_name(), cb.get_handler_function_name());
	}

}

void PropertyComponent::on_property_changed(CL_InputEvent event)
{
	CL_GUIComponent *comp = selected_component;
	if (comp == 0)
	{
		return;
	}

	if (comp->get_type_name() == "button")
	{
		CL_PushButton *co = dynamic_cast<CL_PushButton*>(comp);
		co->set_text(properties["Text"]->get_text());
	}
	else if (comp->get_type_name() == "radiobutton")
	{
		CL_RadioButton *co = dynamic_cast<CL_RadioButton*>(comp);
		co->set_text(properties["Text"]->get_text());
		co->set_group_name(properties["Group Name"]->get_text());
	}
	else if (comp->get_type_name() == "checkbox")
	{
		CL_CheckBox *co = dynamic_cast<CL_CheckBox*>(comp);
		co->set_text(properties["Text"]->get_text());
	}
	else if (comp->get_type_name() == "label")
	{
		CL_Label *co = dynamic_cast<CL_Label*>(comp);
		co->set_text(properties["Text"]->get_text());
	}
	else if (comp->get_type_name() == "frame")
	{
		CL_Frame *co = dynamic_cast<CL_Frame*>(comp);
		co->set_header_text(properties["Text"]->get_text());
	}
	else if (comp->get_type_name() == "statusbar")
	{
		CL_StatusBar *co = dynamic_cast<CL_StatusBar*>(comp);
	}
	else if (comp->get_type_name() == "menubar")
	{
		CL_MenuBar *co = dynamic_cast<CL_MenuBar*>(comp);
	}
	else if (comp->get_type_name() == "spin")
	{
		CL_Spin *co = dynamic_cast<CL_Spin*>(comp);
	}
	else if (comp->get_type_name() == "combobox")
	{
		CL_ComboBox *co = dynamic_cast<CL_ComboBox*>(comp);
	}
	else if (comp->get_type_name() == "lineedit")
	{
		CL_LineEdit *co = dynamic_cast<CL_LineEdit*>(comp);
		co->set_text(properties["Text"]->get_text());
	}
	else if (comp->get_type_name() == "slider")
	{
		CL_Slider *co = dynamic_cast<CL_Slider*>(comp);
		co->set_min(CL_StringHelp::text_to_int(properties["Min"]->get_text()));
		co->set_max(CL_StringHelp::text_to_int(properties["Max"]->get_text()));
		co->set_tick_count(CL_StringHelp::text_to_int(properties["Tick Count"]->get_text()));
		co->set_page_step(CL_StringHelp::text_to_int(properties["Step Size"]->get_text()));
	}
	else if (comp->get_type_name() == "tab")
	{
		CL_Tab *co = dynamic_cast<CL_Tab*>(comp);
		int index = co->get_current_page_index();
		CL_String new_label = properties["Page label"]->get_text();
		co->set_label(index, new_label);
	}
	else if (comp->get_type_name() == "grid")
	{
		GridComponent *co = dynamic_cast<GridComponent*>(comp);
		CL_Size s;
		s.width = CL_StringHelp::text_to_int(properties["Width"]->get_text());
		s.height = CL_StringHelp::text_to_int(properties["Height"]->get_text());
		co->set_boundary_size(s);
	}
	else // custom component
	{
		CustomComponent *co = dynamic_cast<CustomComponent*>(comp);
		if (co != 0)
		{
			co->set_type_name(properties["Type"]->get_text());
		}
	}

	if (comp->get_type_name() != "grid")
	{
		CL_Rect g = comp->get_geometry();

		CL_String equ_x = properties["x1"]->get_text();
		CL_String equ_y = properties["y1"]->get_text();

		CL_String equ_x2 = properties["x2"]->get_text();
		CL_String equ_y2 = properties["y2"]->get_text();

		CL_Size parent_size(0,0);
		HolderComponent *holder = dynamic_cast<HolderComponent*>(comp->get_parent_component());
		if (holder)
		{
			CL_GUIComponent *parent_of_holder = holder->get_parent_component();
			if (parent_of_holder) 
			{
				if (parent_of_holder->get_type_name() == "grid")
					parent_size = dialog_size;
				else
					parent_size = parent_of_holder->get_size();
			}
		}

		CL_Vec2i new_pos = parse_position_equations(equ_x, equ_y, comp->get_size(), parent_size);

		int new_width = comp->get_width();
		if (!equ_x2.empty())
		{
			int x2 = parse_position_equation(equ_x2, comp->get_size(), parent_size);
			new_width = x2-new_pos.x;
			properties["Width"]->set_enabled(false);
			properties["Width"]->set_text(new_width);
		}
		else
		{
			new_width = CL_StringHelp::text_to_int(properties["Width"]->get_text());
			properties["Width"]->set_enabled(true);
		}

		int new_height = comp->get_height();
		if (!equ_y2.empty())
		{
			int y2 = parse_position_equation(equ_y2, comp->get_size(), parent_size);
			new_height = y2-new_pos.y;
			properties["Height"]->set_enabled(false);
			properties["Height"]->set_text(new_height);
		}
		else
		{
			new_height = CL_StringHelp::text_to_int(properties["Height"]->get_text());
			properties["Height"]->set_enabled(true);
		}


		if (holder)
		{
			//holder->set_position_equations(equ_x, equ_y);
			//holder->set_position_equations2(equ_x2, equ_y2);
			CL_Rect hg = holder->get_geometry();
			holder->set_geometry(CL_RectPS(new_pos.x, new_pos.y, new_width, new_height));
			main_window->get_grid_component()->request_repaint();
		}

		comp->set_geometry(CL_RectPS(g.left, g.top, new_width, new_height));
	}

	comp->set_id_name(properties["#id"]->get_text());
	comp->set_class_name(properties[".class"]->get_text());

	// Callbacks
	std::vector<CallbackData> callbacks = callback_info.get_callbacks(comp->get_type_name());
	std::vector<CallbackData>::iterator it_cb;
	for (it_cb = callbacks.begin(); it_cb != callbacks.end(); ++it_cb)
	{
		CallbackData &cb = (*it_cb);
		cb.set_handler_function_name(properties[cb.get_callback_name()]->get_text());
	}

	callbacks = callback_info.get_callbacks("gui_component");
	for (it_cb = callbacks.begin(); it_cb != callbacks.end(); ++it_cb)
	{
		CallbackData &cb = (*it_cb);
		cb.set_handler_function_name(properties[cb.get_callback_name()]->get_text());
	}
}

CL_LineEdit *PropertyComponent::add_field(const CL_StringRef &property_name, const CL_StringRef &value) 
{
	CL_Rect rect = get_child_rect(container);
	int top = rect.bottom-1;
	CL_Size s = get_geometry().get_size();

	CL_Label *label = new CL_Label(container);
	label->set_class_name("properties");
	label->set_text(property_name);
	CL_Rect label_rect = CL_RectPS(4,top, label->get_preferred_width(),label->get_preferred_height());
	label->set_geometry(label_rect);

	CL_LineEdit *le = new CL_LineEdit(container);
	le->set_class_name("properties");
	le->set_geometry(CL_Rect(label_rect.right,top, s.width-8, top+le->get_preferred_height()));
	le->set_text(value);
	le->func_after_edit_changed().set(this, &PropertyComponent::on_property_changed);
	properties[property_name] = le;
	return le;
}

void PropertyComponent::add_anchor_radios() 
{
	CL_Rect g = get_geometry().get_size();
	CL_Rect rect = get_child_rect(container);
	int top = rect.bottom + 4;

	CL_Frame *frame = new CL_Frame(container);
	frame->set_geometry(CL_Rect(5,top, g.right-8, top+100));
	frame->set_header_text("Layout:");

	int left = 12;

	CL_Label *label_tl = new CL_Label(container);
	label_tl->set_text("Top Left:");
	label_tl->set_geometry(CL_RectPS(left+10,top+16, 80,23));

	CL_Label *label_br = new CL_Label(container);
	label_br->set_text("Bottom Right:");
	label_br->set_geometry(CL_RectPS(left+100,top+16, 80,23));

	CL_RadioButton *tl = new CL_RadioButton(container);
	CL_RadioButton *tr = new CL_RadioButton(container);
	CL_RadioButton *bl = new CL_RadioButton(container);
	CL_RadioButton *br = new CL_RadioButton(container);
	CL_RadioButton *scale = new CL_RadioButton(container);
	tl->set_id_name("tl"); tr->set_id_name("tr"); bl->set_id_name("bl"); br->set_id_name("br");
	scale->set_id_name("%");

	CL_Size s = get_geometry().get_size();
	top = rect.bottom+4+32;
	tl->set_geometry(CL_RectPS(        10+left, top,    20, 23));
 	tr->set_geometry(CL_RectPS(     10+50+left, top,    20, 23));
	bl->set_geometry(CL_RectPS(        10+left, top+32, 20, 23));
	br->set_geometry(CL_RectPS(     10+50+left, top+32, 20, 23));
	scale->set_geometry(CL_RectPS(  10+25+left, top+16, 20, 23));

	tl->set_group_name("TL");
	tr->set_group_name("TL");
	bl->set_group_name("TL");
	br->set_group_name("TL");
	scale->set_group_name("TL");

	CL_RadioButton *br_tl = new CL_RadioButton(container);
	CL_RadioButton *br_tr = new CL_RadioButton(container);
	CL_RadioButton *br_bl = new CL_RadioButton(container);
	CL_RadioButton *br_br = new CL_RadioButton(container);
	CL_RadioButton *br_scale = new CL_RadioButton(container);
	br_tl->set_id_name("tl"); br_tr->set_id_name("tr"); br_bl->set_id_name("bl"); br_br->set_id_name("br");
	br_scale->set_id_name("%");

	left += 100;
	br_tl->set_geometry(CL_RectPS(        left, top,    20, 23));
	br_tr->set_geometry(CL_RectPS(     left+50, top,    20, 23));
	br_bl->set_geometry(CL_RectPS(        left, top+32, 20, 23));
	br_br->set_geometry(CL_RectPS(     left+50, top+32, 20, 23));
	br_scale->set_geometry(CL_RectPS(  left+25, top+16, 20, 23));

	br_tl->set_group_name("BR");
	br_tr->set_group_name("BR");
	br_bl->set_group_name("BR");
	br_br->set_group_name("BR");
	br_scale->set_group_name("BR");

	scale->func_group_selection_changed().set(this, &PropertyComponent::on_anchoring_changed);
	br_scale->func_group_selection_changed().set(this, &PropertyComponent::on_anchoring_changed);

	CL_ComponentAnchorPoint cap_tl = selected_holder->get_anchor_tl();
	CL_ComponentAnchorPoint cap_br = selected_holder->get_anchor_br();

	switch (cap_tl)
	{
	case cl_anchor_top_left: tl->set_selected(true); break;
	case cl_anchor_top_right: tr->set_selected(true); break;
	case cl_anchor_bottom_left: bl->set_selected(true); break;
	case cl_anchor_bottom_right: br->set_selected(true); break;
	case cl_anchor_relative: scale->set_selected(true); break;
	default:break;
	}

	switch (cap_br)
	{
	case cl_anchor_top_left: br_tl->set_selected(true); break;
	case cl_anchor_top_right: br_tr->set_selected(true); break;
	case cl_anchor_bottom_left: br_bl->set_selected(true); break;
	case cl_anchor_bottom_right: br_br->set_selected(true); break;
	case cl_anchor_relative: br_scale->set_selected(true); break;
	default:break;
	}
}

CL_Rect PropertyComponent::get_child_rect(CL_GUIComponent *parent) 
{
	CL_GUIComponent *child = parent->get_first_child();

	if (child == 0)
		return CL_Rect(0,0,0,0);
	
	CL_Rect bounding_rect = child->get_geometry();
	child = child->get_next_sibling();

	while (child)
	{
		bounding_rect.bounding_rect(child->get_geometry());
		child = child->get_next_sibling();
	}

	return bounding_rect;
}

void PropertyComponent::on_anchoring_changed(CL_RadioButton *rb)
{
	CL_String id = rb->get_id_name();

	if (rb->get_group_name() == "TL")
	{
		if (id == "tl")
		{
			selected_holder->set_anchor_tl(cl_anchor_top_left);
		}
		else if (id == "tr")
		{
			selected_holder->set_anchor_tl(cl_anchor_top_right);
		}
		else if (id == "bl")
		{
			selected_holder->set_anchor_tl(cl_anchor_bottom_left);
		}
		else if (id == "br")
		{
			selected_holder->set_anchor_tl(cl_anchor_bottom_right);
		}
		else if (id == "%")
		{
			selected_holder->set_anchor_tl(cl_anchor_relative);
		}
	}
	else
	{
		if (id == "tl")
		{
			selected_holder->set_anchor_br(cl_anchor_top_left);
		}
		else if (id == "tr")
		{
			selected_holder->set_anchor_br(cl_anchor_top_right);
		}
		else if (id == "bl")
		{
			selected_holder->set_anchor_br(cl_anchor_bottom_left);
		}
		else if (id == "br")
		{
			selected_holder->set_anchor_br(cl_anchor_bottom_right);
		}
		else if (id == "%")
		{
			selected_holder->set_anchor_br(cl_anchor_relative);
		}
	}
}

void PropertyComponent::on_add_tab(CL_Tab *tab)
{
	int num_pages = tab->get_page_count();
	tab->add_page(cl_format("Page %1", num_pages+1));
}

void PropertyComponent::on_delete_tab(CL_Tab *tab)
{
}

void PropertyComponent::add_header(const CL_StringRef &header)
{
	CL_Label *label_properties_header = new CL_Label(container);
	label_properties_header->set_class_name("properties_header");
	label_properties_header->set_text(header);

	CL_Rect child_rect = get_child_rect(container);
	int y_offset = 10;
	label_properties_header->set_geometry(CL_Rect(4,y_offset+child_rect.bottom,container->get_geometry().get_width()-8, y_offset+child_rect.bottom+label_properties_header->get_preferred_height()));
}


CL_Vec2i PropertyComponent::parse_position_equations(const CL_String &equ_x, const CL_String &equ_y, const CL_Size &component_size, const CL_Size &parent_size)
{
	CL_Vec2i pos;
	pos.x = parse_position_equation(equ_x, component_size, parent_size);
	pos.y = parse_position_equation(equ_y, component_size, parent_size);
	return pos;
}

int PropertyComponent::parse_position_equation(const CL_String &equation, const CL_Size &component_size, const CL_Size &parent_size)
{
	std::vector<CL_String> tokens = split_text_char(equation, "+-");
	std::vector<CL_String>::size_type i;
	int sum = 0;
	bool add_next = true;
	for (i = 0; i < tokens.size(); i++)
	{
		int num = 0;
		if (tokens[i] == "+")
		{
			add_next = true;
			continue;
		}
		else if (tokens[i] == "-")
		{
			add_next = false;
			continue;
		}
		else if (tokens[i] == "pw")
		{
			num = parent_size.width;
		}
		else if (tokens[i] == "ph")
		{
			num = parent_size.height;
		}
		else if (tokens[i] == "w")
		{
			num = component_size.width;
		}
		else if (tokens[i] == "h")
		{
			num = component_size.height;
		}
		else
		{
			num = CL_StringHelp::text_to_int(tokens[i]);
		}

		if (add_next)
			sum += num;
		else
			sum -= num;
	}

	return sum;
}

std::vector<CL_String> PropertyComponent::split_text_char(
	const CL_StringRef &text,
	const CL_StringRef &split_string,
	bool skip_empty)
{
	std::vector<CL_String> result;
	CL_String::size_type end_pos = 0, begin_pos = 0;
	while (true)
	{
		end_pos = text.find_first_of(split_string, begin_pos);
		if (end_pos == CL_String::npos)
		{
			if (begin_pos != text.length())
				result.push_back(text.substr(begin_pos));
			break;
		}
		else
		{
			if (!skip_empty || begin_pos != end_pos)
			{
				result.push_back(text.substr(begin_pos, end_pos-begin_pos));
				result.push_back(text.substr(end_pos, 1));
			}

			begin_pos = end_pos+1;
		}
	}
	return result;
}

void PropertyComponent::set_dialog_size(const CL_Size &size)
{
	dialog_size = size;
}
#endif
