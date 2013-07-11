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

#include "precomp.h"
#include "property_component.h"
#include "property_item.h"
#include "GridComponent/grid_object.h"
#include "GridComponent/grid_component.h"
#include "ComponentTypes/custom_component.h"
#include "Selection/selection.h"
#include "MainWindow/gui_editor_window.h"
#include "ComponentTypes/PropertyItems/property_item_position.h"
#include "ComponentTypes/PropertyItems/property_item_text.h"
#include "ComponentTypes/PropertyItems/property_item_group_name.h"
#include "ComponentTypes/PropertyItems/property_item_class_name.h"
#include "ComponentTypes/PropertyItems/property_item_id_name.h"
#include "ComponentTypes/PropertyItems/property_item_min.h"
#include "ComponentTypes/PropertyItems/property_item_max.h"
#include "ComponentTypes/PropertyItems/property_item_tick_count.h"
#include "ComponentTypes/PropertyItems/property_item_step_size.h"
#include "ComponentTypes/PropertyItems/property_item_page_label.h"
#include "ComponentTypes/PropertyItems/property_item_type_name.h"
#include "ComponentTypes/PropertyItems/property_item_anchor.h"
#include "ComponentTypes/PropertyItems/property_item_enabled.h"

PropertyComponent::PropertyComponent(GuiEditorWindow *main_window)
: GUIComponent(main_window), main_window(main_window), name_column_width(75), active_item(0), active_component(0), scrollbar(0)
{
	set_tag_name("property-component");
	set_clip_children(true);
	part_header = GUIThemePart(this, "header");
	part_header_name = GUIThemePart(this, "header name");
	part_header_value = GUIThemePart(this, "header value");
	part_row[0] = GUIThemePart(this, "row");
	part_row[1] = GUIThemePart(this, "row");
	part_row[1].set_pseudo_class("alternate", true);
	part_item_name = GUIThemePart(this, "item name");
	part_item_value = GUIThemePart(this, "item value");

	func_input_pressed().set(this, &PropertyComponent::on_input_pressed);
	func_input_released().set(this, &PropertyComponent::on_input_released);
	func_input_doubleclick().set(this, &PropertyComponent::on_input_doubleclick);
	func_input_pointer_moved().set(this, &PropertyComponent::on_input_pointer_moved);
	func_render().set(this, &PropertyComponent::on_render);

	scrollbar = new ScrollBar(this);
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
	std::vector<GridObject *> selection = main_window->get_selection()->get_selection();
	if (!selection.empty())
	{
		GridObject *item = selection[0];
		GUIComponent *comp = item->get_component();

		// add_property(new PropertyItemHeader("Properties"));

		std::string type = comp->get_tag_name();

		if (type != "grid")
		{
			Size comp_size = comp->get_geometry().get_size();
			bool enable_width = true;
			bool enable_height = true;

			GridObject *object = dynamic_cast<GridObject*>(comp->get_parent_component());
			if (object)
			{
				add_property(new PropertyItemPosition(PropertyItemPosition::type_x1));
				add_property(new PropertyItemPosition(PropertyItemPosition::type_y1));
				add_property(new PropertyItemPosition(PropertyItemPosition::type_x2));
				add_property(new PropertyItemPosition(PropertyItemPosition::type_y2));
/*
				std::string equ_x = object->get_position_equation_x();
				std::string equ_y = object->get_position_equation_y();

				if (equ_x.empty())
					add_property(new PropertyItemPosition("x1", StringHelp::int_to_text(object->get_geometry().left)));
				else
					add_property(new PropertyItemPosition("x1", equ_x));

				if (equ_y.empty())
					add_property(new PropertyItemPosition("y1", StringHelp::int_to_text(object->get_geometry().top)));
				else
					add_property(new PropertyItemPosition("y1", equ_y));


				std::string equ_x2 = object->get_position_equation_x2();
				std::string equ_y2 = object->get_position_equation_y2();

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
			//add_property(new PropertyItemLineEdit("Width", StringHelp::int_to_text(comp_size.width)));
			//add_property(new PropertyItemLineEdit("Height", StringHelp::int_to_text(comp_size.height)));
			// le_w->set_enabled(enable_width);
			// le_h->set_enabled(enable_height);
		}

		add_property(new PropertyItemText());

		if (type == "button")
		{
			PushButton *co = dynamic_cast<PushButton*>(comp);
			//add_property(new PropertyItemLineEdit("Text", co->get_text()));
		}
		else if (type == "lineedit")
		{
			LineEdit *co = dynamic_cast<LineEdit*>(comp);
			//add_property(new PropertyItemLineEdit("Text",co->get_text()));
		}
		else if (type == "textedit")
		{
			TextEdit *co = dynamic_cast<TextEdit*>(comp);
			//add_property(new PropertyItemLineEdit("Text",co->get_text()));
		}
		else if (type == "checkbox")
		{
			CheckBox *co = dynamic_cast<CheckBox*>(comp);
			//add_property(new PropertyItemLineEdit("Text", co->get_text()));
		}
		else if (type == "radiobutton")
		{
			RadioButton *co = dynamic_cast<RadioButton*>(comp);
			//add_property(new PropertyItemLineEdit("Text", co->get_text()));
			//add_property(new PropertyItemLineEdit("Group Name", co->get_group_name()));
			add_property(new PropertyItemGroupName());
		}
		else if (type == "label")
		{
			Label *co = dynamic_cast<Label*>(comp);
			//add_property(new PropertyItemLineEdit("Text", co->get_text()));
		}
		else if (type == "statusbar")
		{
			StatusBar *co = dynamic_cast<StatusBar*>(comp);
		}
		else if (type == "frame")
		{
			Frame *co = dynamic_cast<Frame*>(comp);
			//add_property(new PropertyItemLineEdit("Text", co->get_header_text()));
		}
		else if (type == "slider")
		{
			Slider *co = dynamic_cast<Slider*>(comp);
			add_property(new PropertyItemMin());
			add_property(new PropertyItemMax());
			add_property(new PropertyItemTickCount());
			add_property(new PropertyItemStepSize());
			//add_property(new PropertyItemLineEdit("Min", StringHelp::int_to_text(co->get_min())));
			//add_property(new PropertyItemLineEdit("Max", StringHelp::int_to_text(co->get_max())));
			//add_property(new PropertyItemLineEdit("Tick Count", StringHelp::int_to_text(co->get_tick_count())));
			//add_property(new PropertyItemLineEdit("Step Size", StringHelp::int_to_text(co->get_page_step())));
		}
		else if (type == "grid")
		{
			GridComponent *co = dynamic_cast<GridComponent*>(comp);
			Size s = co->get_dialog_size();
			//add_property(new PropertyItemLineEdit("Width", StringHelp::int_to_text(s.width)));
			//add_property(new PropertyItemLineEdit("Height", StringHelp::int_to_text(s.height)));
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
		else if (type == "imageview")
		{
		}
		else if (type == "tab")
		{
			Tab *tab = dynamic_cast<Tab*>(comp);
			int current_page = tab->get_current_page_index();
			TabPage *tab_page = tab->get_page(current_page);
			add_property(new PropertyItemPageLabel());
			//add_property(new PropertyItemLineEdit("Page label", tab_page->get_label()));
		}
		else
		{
			add_property(new PropertyItemTypeName());
			//add_property(new PropertyItemLineEdit("Type", comp->get_tag_name()));
		}

		add_property(new PropertyItemEnabled());
		add_property(new PropertyItemIdName());
		add_property(new PropertyItemClassName());
		//add_property(new PropertyItemLineEdit("#id", comp->get_id()));
		//add_property(new PropertyItemLineEdit(".class", comp->get_class()));

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
			Rect child_rect = get_child_rect(container);

			PushButton *add_tab = new PushButton(container);
			add_tab->set_geometry(RectPS(child_rect.left, child_rect.bottom+10, 80,24));
			add_tab->set_text("Add tab");

			PushButton *del_tab = new PushButton(container);
			del_tab->set_geometry(RectPS(child_rect.left, child_rect.bottom+10+32, 80,24));
			del_tab->set_text("Delete tab");

			Tab *tab = dynamic_cast<Tab*>(comp);
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

bool PropertyComponent::on_input_pressed(const InputEvent &input_event)
{
	return edit_state.on_input_pressed(input_event);
}

bool PropertyComponent::on_input_released(const InputEvent &input_event)
{
	return edit_state.on_input_released(input_event);
}

bool PropertyComponent::on_input_doubleclick(const InputEvent &input_event)
{
	return edit_state.on_input_doubleclick(input_event);
}

bool PropertyComponent::on_input_pointer_moved(const InputEvent &input_event)
{
	return edit_state.on_input_pointer_moved(input_event);
}

void PropertyComponent::on_render(Canvas &canvas, const Rect &update_rect)
{
	update_layout();
	update_scrollbar_size(canvas);

	part_header.render_box(canvas, header);
	part_header_name.render_box(canvas, header_name);
	part_header_value.render_box(canvas, header_value);
	part_header_name.render_text(canvas, "Property", part_header_name.get_content_box(header_name));
	part_header_value.render_text(canvas, "Value", part_header_value.get_content_box(header_value));

	value_boxes.clear();
	int row_width = content.get_width(); // to do: account for shrink box
	int y = body.top;
	for (size_t i = scrollbar->get_position(); i < items.size() && y < body.bottom; i++)
	{
		int row_height = items[i]->get_inactive_height(canvas, part_item_value, row_width-name_column_width);
		if (row_height < part_item_name.get_css_height())
			row_height = part_item_name.get_css_height();
		Rect row(body.left, y, body.right, y+row_height);
		Rect row_content = part_row[i%2].get_content_box(row);
		Rect name_box(row_content.left, row_content.top, row_content.left+name_column_width, row_content.bottom);
		Rect value_box(row_content.left+name_column_width, row_content.top, row_content.right, row_content.bottom);
		part_row[i%2].render_box(canvas, row);
		part_item_name.render_box(canvas, name_box);
		part_item_name.render_text(canvas, items[i]->name, part_item_name.get_content_box(name_box));
		part_item_value.render_box(canvas, value_box);
		value_boxes.push_back(value_box);

		if (items[i] != active_item || active_component == 0)
			items[i]->render_inactive(canvas, part_item_value, value_box);
		//else
		//	active_component->set_geometry(value_box);
		y = row.bottom;
	}
}

void PropertyComponent::deactivate()
{
	if (active_item)
	{
		active_item->deactivate(active_component);
		std::vector<GridObject *> selection = main_window->get_selection()->get_selection();
		for (size_t i = 0; i < selection.size(); i++)
		{
			try
			{
				active_item->apply_changes(selection[i]);

				for (size_t i = 0; i < items.size(); i++)
					items[i]->selection_changed(selection);
			}
			catch (Exception &)
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

int PropertyComponent::hit_test(const Point &pos) const
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
		if (active_component)
		{
			Point pos = component_to_screen_coords(value_boxes[index].get_top_left());
			active_component->set_geometry(Rect(pos, value_boxes[index].get_size()));
			active_component->set_visible(true);
			active_component->set_focus();
		}
		request_repaint();
	}
}

void PropertyComponent::on_filter_message(std::shared_ptr<GUIMessage> &message)
{
	if (active_component)
	{
		std::shared_ptr<GUIMessage_Pointer> pointer = std::dynamic_pointer_cast<GUIMessage_Pointer>(message);
		if (pointer)
		{
			if (active_component != message->target && !active_component->is_ancestor_of(message->target))
				deactivate();
		}
	}
}

void PropertyComponent::update_layout()
{
	Rect geometry = get_size();
	content = get_content_box();
	header = Rect(content.left, content.top, content.right, content.top+part_header.get_css_height());
	body = Rect(content.left, header.bottom, content.right, content.bottom);
	header_content = part_header.get_content_box(header);
	header_name = Rect(header_content.left, header_content.top, header_content.left+name_column_width, header_content.bottom);
	header_value = Rect(header_name.right, header_content.top, header_content.right, header_content.bottom);
}

void PropertyComponent::update_scrollbar_size(Canvas &canvas)
{
	int row_width = content.get_width(); // to do: account for shrink box
	int y = content.bottom;
	size_t i;
	for (i = items.size(); i > 0 && y > content.top; i--)
		y -= items[i-1]->get_inactive_height(canvas, part_item_value, row_width-name_column_width);

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

Rect PropertyComponent::get_header_resize_grabber() const
{
	return Rect(header_name.right-3, header.top, header_name.right+2, header.bottom);
}
