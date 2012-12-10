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

#include "GUI/precomp.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_message_input.h"
#include "API/GUI/gui_message_pointer.h"
#include "API/GUI/gui_theme_part.h"
#include "API/GUI/gui_component_description.h"
#include "API/Core/System/timer.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "API/GUI/Components/listview.h"
#include "API/GUI/Components/listview_icon_list.h"
#include "API/GUI/Components/listview_header.h"
#include "API/GUI/Components/listview_column_data.h"
#include "API/GUI/Components/listview_selected_item.h"
#include "API/GUI/Components/scrollbar.h"
#include "API/GUI/Components/lineedit.h"
#include "API/Display/Window/input_event.h"
#include "API/GUI/gui_message_focus_change.h"
#include "API/Display/Font/font.h"
#include "API/Display/Window/keys.h"
#include "listview_item_impl.h"
#include "listview_shown_item.h"
#include "listview_layout.h"
#include "listview_renderer.h"
#include "listview_layout_details.h"
#include "listview_layout_icons.h"
#include "listview_layout_thumbnails.h"
#include "listview_impl.h"
#include "../../gui_css_strings.h"
#include "API/Display/2D/canvas.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// ListView Construction:

ListView::ListView(GUIComponent *parent)
: GUIComponent(parent, CssStr::ListView::type_name), impl(new ListView_Impl)
{
	set_focus_policy(focus_local);
	impl->listview = this;

	impl->renderer = new ListViewRenderer(this);
	impl->renderer->create_parts();
	impl->renderer->set_icon_list(impl->icon_list);

	impl->create_components();

	func_process_message().set(impl.get(), &ListView_Impl::on_process_message);
	func_render().set(impl.get(), &ListView_Impl::on_render);
	func_resized().set(impl.get(), &ListView_Impl::on_resized);

	impl->document_item.impl->func_item_added.set(impl.get(), &ListView_Impl::on_item_added);
	impl->document_item.impl->func_item_modified.set(impl.get(), &ListView_Impl::on_item_modified);
	impl->document_item.impl->func_item_deleted.set(impl.get(), &ListView_Impl::on_item_deleted);
	impl->create_parts();

	set_display_mode(listview_mode_details);

//	impl->update_part_positions();
}

ListView::~ListView()
{
}

/////////////////////////////////////////////////////////////////////////////
// ListView Attributes:

int ListView::get_scroll_position()
{
	return impl->scrollbar->get_position();
}

int ListView::get_scroll_max_position()
{
	return impl->scrollbar->get_max();
}

ListView *ListView::get_named_item(GUIComponent *reference_component, const std::string &id)
{
	ListView *object = NULL;
	if (reference_component)
		object = dynamic_cast<ListView*>(reference_component->get_named_item(id));

	if (!object)
		throw Exception(string_format("Cannot find ListView named item: %1", id));

	return object;
}

ListViewItem ListView::get_document_item() const
{
	return impl->document_item;
}

bool ListView::get_select_whole_row() const
{
	return impl->select_whole_row;
}

ListViewHeader *ListView::get_header() const
{
	return impl->header;
}

ListViewIconList ListView::get_icon_list() const
{
	return impl->icon_list;
}

ListViewSelection ListView::get_selected_items() const
{
	return impl->selection;
}

ListViewItem ListView::get_selected_item()
{
	if (impl->selection.get_first().is_item())
	{
		return impl->selection.get_first().get_item();
	}

	return ListViewItem();
}

ListViewDisplayMode ListView::get_display_mode() const
{
	return impl->display_mode;
}

/////////////////////////////////////////////////////////////////////////////
// ListView Operations:

void ListView::set_scroll_position(int pos)
{
	impl->scrollbar->set_position(pos);
	impl->on_scroll();
}

ListViewItem ListView::create_item()
{
	impl->cancel_edit();

	std::shared_ptr<ListViewItem_Impl> item_impl(new ListViewItem_Impl());
	ListViewItem new_item = ListViewItem(item_impl);
	return new_item;
}

void ListView::set_select_whole_row(bool value)
{
	impl->cancel_edit();

	impl->select_whole_row = value;
	request_repaint();
}

void ListView::set_multi_select(bool value)
{
	impl->cancel_edit();

	impl->multiple_selection = value;
}

void ListView::set_display_mode(ListViewDisplayMode mode)
{
	impl->cancel_edit();

	impl->display_mode = mode;
	impl->header->set_display_mode(mode);

	if (impl->layout)
		delete impl->layout;

	if (mode == listview_mode_details)
	{
		impl->layout = new ListViewLayoutDetails(this);
	}
	else if (mode == listview_mode_icons)
	{
		impl->layout = new ListViewLayoutIcons(this);
	}
	else if (mode == listview_mode_thumbnails)
	{
		impl->layout = new ListViewLayoutThumbnails(this);
	}
	else if (mode == listview_mode_tiles)
	{
		throw Exception("ListView doesn't support listview_mode_tiles yet");
	}
	else if (mode == listview_mode_list)
	{
		throw Exception("ListView doesn't support listview_mode_list yet");
	}

	impl->layout->set_listview_header(impl->header);
	impl->layout->set_view_rect(impl->rect_columns_content);
	impl->layout->set_root_item(impl->document_item);
	impl->layout->create_parts();
	impl->layout->invalidate();

	impl->renderer->set_display_mode(mode);
	impl->layout->set_show_detail_icon(impl->show_detail_icon);
	impl->layout->set_show_detail_opener(impl->show_detail_opener);
}

void ListView::show_detail_icon(bool enable)
{
	impl->show_detail_icon = enable;
	impl->layout->set_show_detail_icon(impl->show_detail_icon);
}

void ListView::show_detail_opener(bool enable)
{
	impl->show_detail_opener = enable;
	impl->layout->set_show_detail_opener(impl->show_detail_opener);
}

void ListView::clear()
{
	impl->cancel_edit();

	clear_selection();
	impl->document_item.remove_children();
	request_repaint();
}

void ListView::clear_selection()
{
	impl->cancel_edit();

	impl->selection.clear();
	request_repaint();
}

void ListView::set_selected(ListViewItem &item, bool selected)
{
	impl->cancel_edit();

	if (!impl->multiple_selection)
		impl->selection.clear();

	item.impl->selected = selected;

	if (selected == true)
		impl->selection.append(item);
	else
		impl->selection.remove(item);

	if (!impl->func_selection_changed.is_null())
		impl->func_selection_changed.invoke(impl->selection);

	request_repaint();
}

ListViewItem ListView::find(const std::string &col_id, const std::string &str, bool recursive)
{
	ListViewItem it = impl->document_item.get_first_child();
	return impl->find(it, col_id, str, recursive);
}

ListViewItem ListView::find(std::shared_ptr<ListViewItemUserData> userdata, bool recursive)
{
	ListViewItem it = impl->document_item.get_first_child();
	return impl->find(it, userdata, recursive);
}

ListViewItem ListView::find(int id, bool recursive)
{
	ListViewItem it = impl->document_item.get_first_child();
	return impl->find(it, id, recursive);
}

/////////////////////////////////////////////////////////////////////////////
// ListView Events:

Callback_v1<ListViewSelection> &ListView::func_selection_changed()
{
	impl->cancel_edit();

	return impl->func_selection_changed;
}

Callback_v1<const ListViewItem &> &ListView::func_item_doubleclick()
{
	return impl->func_item_doubleclick;
}

Callback_2<bool, ListViewItem, std::string &> &ListView::func_item_edited()
{
	return impl->func_item_edited;
}

Callback_v1<InputEvent&> &ListView::func_key_pressed()
{
	return impl->func_key_pressed;
}

Callback_v1<InputEvent&> &ListView::func_key_released()
{
	return impl->func_key_released;
}

Callback_v1<Point> & ListView::func_mouse_right_up()
{
	return impl->func_mouse_right_up;
}

Callback_v1<const ListViewItem &> &ListView::func_item_opened()
{
	return impl->func_item_opened;
}

Callback_v1<const ListViewItem &> &ListView::func_item_closed()
{
	return impl->func_item_closed;
}

Callback_v0 &ListView::func_begin_drag()
{
	return impl->func_begin_drag;
}

/////////////////////////////////////////////////////////////////////////////
// ListView Implementation:

void ListView_Impl::on_process_message(std::shared_ptr<GUIMessage> &msg)
{
	std::shared_ptr<GUIMessage_Input> input_msg = std::dynamic_pointer_cast<GUIMessage_Input>(msg);
	if (input_msg)
	{
		
		InputEvent &input_event = input_msg->input_event;

		if (input_event.type == InputEvent::pointer_moved)
		{
			on_mouse_move(msg, input_event);
		}
		else if (input_event.type == InputEvent::pressed && input_event.id == mouse_left)
		{
			on_mouse_lbutton_down(msg, input_event);
			msg->consumed = true;
		}
		else if (input_event.type == InputEvent::pressed && input_event.id == mouse_wheel_up)
		{
			scrollbar->set_position(scrollbar->get_position()-scrollbar->get_line_step());
			on_scroll();
			msg->consumed = true;
		}
		else if (input_event.type == InputEvent::pressed && input_event.id == mouse_wheel_down)
		{
			scrollbar->set_position(scrollbar->get_position() + scrollbar->get_line_step());
			on_scroll();
			msg->consumed = true;
		}
		else if (input_event.type == InputEvent::released && input_event.id == mouse_left)
		{
			on_mouse_lbutton_up(msg, input_event);
			msg->consumed = true;
		}
		else if (input_event.type == InputEvent::doubleclick && input_event.id == mouse_left)
		{
			on_mouse_lbutton_doubleclick(msg, input_event);
			msg->consumed = true;
		}
		else if (input_event.type == InputEvent::pressed && input_event.id == mouse_right)
		{
			on_mouse_rbutton_down(msg, input_event);
			msg->consumed = true;
		}
		else if (input_event.type == InputEvent::released && input_event.id == mouse_right)
		{
			on_mouse_rbutton_up(msg, input_event);
			msg->consumed = true;
		}
		else if (input_event.type == InputEvent::pressed && input_event.device.get_type() == InputDevice::keyboard)
		{
			bool consumed = on_keyboard_pressed(input_event);
			if (consumed)
				msg->consumed = true;
		}
		else if (input_event.type == InputEvent::released && input_event.device.get_type() == InputDevice::keyboard)
		{
			bool consumed = on_keyboard_released(input_event);
			if (consumed)
				msg->consumed = true;
		}
	}
	std::shared_ptr<GUIMessage_Pointer> pointer = std::dynamic_pointer_cast<GUIMessage_Pointer>(msg);
	if (pointer)
	{
		
		if (pointer->pointer_type == GUIMessage_Pointer::pointer_leave)
			on_mouse_leave();
		else if (pointer->pointer_type == GUIMessage_Pointer::pointer_enter)
			on_mouse_enter();
	}
	std::shared_ptr<GUIMessage_FocusChange> focus_change_msg = std::dynamic_pointer_cast<GUIMessage_FocusChange>(msg);
	if (focus_change_msg)
	{
		if (focus_change_msg->focus_type == GUIMessage_FocusChange::gained_focus)
		{
			// ..
		}
		else
		{
			if (just_launched_lineedit)
				just_launched_lineedit = false;
			else
				cancel_edit();
		}
		listview->request_repaint();
	}
}

bool ListView_Impl::on_keyboard_pressed(InputEvent &event)
{
	bool event_consumed = false;

	if (document_item.get_child_count() == 0)
		return event_consumed;

	if (event.id == keycode_left || event.id == keycode_right || event.id == keycode_up || event.id == keycode_down)
	{
		event_consumed = true;

		// Ensure we have a selected item.
		if (selection.get_first().is_null())
		{
			ListViewItem item = document_item.get_first_child();
			listview->set_selected(item, true);
		}

		ListViewSelectedItem sel = selection.get_last();
		ListViewItem sel_item = sel.get_item();

		ListViewItem new_selected_item;

		if (event.id == keycode_left)
		{
			if (display_mode == listview_mode_details)
			{
				if (sel_item.has_children() && sel_item.is_open())
				{
					sel_item.set_open(false);
					layout->invalidate();
					update_scrollbar();
					new_selected_item = sel_item;
					if (!func_item_closed.is_null())
						func_item_closed.invoke(sel_item);
				}
				else
				{
					new_selected_item = layout->get_neighbour(sel_item, ListViewLayout::neighbour_left);
				}
			}
			else
			{
				new_selected_item = layout->get_neighbour(sel_item, ListViewLayout::neighbour_left);
			}
		}
		else if (event.id == keycode_right)
		{
			if (display_mode == listview_mode_details)
			{
				if (sel_item.has_children() && !sel_item.is_open())
				{
					sel_item.set_open(true);
					layout->invalidate();
					update_scrollbar();
					new_selected_item = sel_item;
					if (!func_item_opened.is_null())
						func_item_opened.invoke(sel_item);
				}
				else
				{
					new_selected_item = layout->get_neighbour(sel_item, ListViewLayout::neighbour_right);
				}
			}
			else
			{
				new_selected_item = layout->get_neighbour(sel_item, ListViewLayout::neighbour_right);
			}
		}
		else if (event.id == keycode_up)
		{
			new_selected_item = layout->get_neighbour(sel_item, ListViewLayout::neighbour_up);
		}
		else if (event.id == keycode_down)
		{
			new_selected_item = layout->get_neighbour(sel_item, ListViewLayout::neighbour_down);
		}

		if (!event.ctrl)
			listview->set_selected(new_selected_item);
		else
			selection.append(new_selected_item);

        if (!layout->is_visible(new_selected_item))
		{
			if (event.id == keycode_up || event.id == keycode_left)
			{
				int scroll_pos = scrollbar->get_position();
				scroll_pos -= layout->get_row_height();
				scrollbar->set_position(scroll_pos);
				on_scroll();
			}
			else
			{
				int scroll_pos = scrollbar->get_position();
				scroll_pos += layout->get_row_height();
				scrollbar->set_position(scroll_pos);
				on_scroll();
			}
		}
	}
	else if (event.id == keycode_f2)
	{
		std::vector<ListViewShownItem> &items = layout->get_shown_items();
		std::vector<ListViewShownItem>::iterator it;
		for (it = items.begin(); it != items.end(); ++it)
		{
			ListViewShownItem &si = (*it);
			if (si.is_null())
				continue;
			if (si.item.is_null())
				continue;

			if (si.item.is_selected())
				edit_item(*it);
		}

	}
	else if (event.type == InputEvent::pressed && event.id == keycode_home)
	{
		scrollbar->set_position(scrollbar->get_min());
		on_scroll();
		std::vector<ListViewShownItem> show_items = layout->get_shown_items();
		if (!show_items.empty())
			listview->set_selected(show_items[0].item, true);
	}
	else if (event.type == InputEvent::pressed && event.id == keycode_end)
	{
		scrollbar->set_position(scrollbar->get_max());
		on_scroll();
		std::vector<ListViewShownItem> show_items = layout->get_shown_items();
		if (!show_items.empty())
			listview->set_selected(show_items.back().item, true);
	}
	else if (event.type == InputEvent::pressed && event.id == keycode_prior)
	{
		// PageUp - selects the first item of the visible items if another item is selected.
		// If first item already selected, do a page up.

		std::vector<ListViewShownItem> show_items = layout->get_shown_items();
		if (show_items.size() > 1)
		{
			if (selection.get_first().get_item() != show_items[0].item)
				listview->set_selected(show_items[0].item, true);
			else
			{
				scrollbar->set_position(scrollbar->get_position()-scrollbar->get_page_step());
				on_scroll();
				std::vector<ListViewShownItem> show_items = layout->get_shown_items();
				if (!show_items.empty())
					listview->set_selected(show_items[0].item, true);
			}
		}
	}
	else if (event.type == InputEvent::pressed && event.id == keycode_next)
	{
		// PageDown - selects the last item of the visible items if last item is selected.
		// If last item is already selected, do a page down.
		
		std::vector<ListViewShownItem> show_items = layout->get_shown_items();
		if (show_items.size() > 1)
		{
			if (selection.get_first().get_item() != show_items.back().item)
				listview->set_selected(show_items.back().item, true);
			else
			{
				scrollbar->set_position(scrollbar->get_position() + scrollbar->get_page_step());
				on_scroll();
				std::vector<ListViewShownItem> show_items = layout->get_shown_items();
				if (!show_items.empty())
					listview->set_selected(show_items.back().item, true);
			}
		}
	}
	else
	{
		if (!func_key_pressed.is_null())
			func_key_pressed.invoke(event);
	}

	return event_consumed;
}

bool ListView_Impl::on_keyboard_released(InputEvent &event)
{
	bool event_consumed = false;

	if (event.id == keycode_left)
	{
	}
	else if (event.id == keycode_right)
	{
	}
	else if (event.id == keycode_up)
	{
	}
	else if (event.id == keycode_down)
	{
	}
	else
	{
		if (!func_key_released.is_null())
			func_key_released.invoke(event);
	}

	return event_consumed;
}

void ListView_Impl::on_mouse_lbutton_down(std::shared_ptr<GUIMessage> &msg, InputEvent &input_event)
{
	on_mouse_button_down(msg, input_event);
}

void ListView_Impl::on_mouse_button_down(std::shared_ptr<GUIMessage> &msg, InputEvent &input_event)
{
	listview->set_focus();

	Point pos = input_event.mouse_pos;
	pos_mouse_drag_start = pos;

	lineedit->set_visible(false);
	listview->set_focus();
	if (edited_item.is_item())
	{
		edited_item.impl->selected = true;
		listview->request_repaint();
		edited_item = ListViewItem();
	}

	ListViewShownItem si = layout->get_item_at(pos); 

	if (si.valid == false)
		return;

	// Check if opener rect is clicked.
	if (si.rect_opener.contains(pos))
	{
		si.item.set_open(!si.item.is_open());
		layout->invalidate();
		listview->request_repaint();
		update_scrollbar();
		if (si.item.is_open())
		{
			if (!func_item_opened.is_null())
				func_item_opened.invoke(si.item);
		}
		else
		{
			if (!func_item_closed.is_null())
				func_item_closed.invoke(si.item);
		}
	}

	// Check if icon or text is clicked.
	if (si.rect_text[0].contains(pos) || si.rect_icon.contains(pos))
	{
		if (si.item.is_selected() && si.item.is_editable())
		{
			edit_timer.start(500, false);
		}
		else
		{
			if (multiple_selection && !si.item.is_selected())
			{
				si.item.impl->selected = true;
				selection.append(si.item);
				if (!func_selection_changed.is_null())
					func_selection_changed.invoke(selection);
			}
			else
			{
				if (!si.item.is_selected())
				{
					selection.clear();
					si.item.impl->selected = true;
					selection.append(si.item);
					if (!func_selection_changed.is_null())
						func_selection_changed.invoke(selection);
				}
			}

			if (!si.item.is_open())
			{
				si.item.set_open(true);
				layout->invalidate();
				update_scrollbar();

				if (!func_item_opened.is_null())
					func_item_opened.invoke(si.item);
			}
		}
		listview->request_repaint();
		drag_or_edit_started = true;
	}

	listview->capture_mouse(true);
}

void ListView_Impl::on_drag_or_edit_timeout()
{
	ListViewShownItem si = layout->get_item_at(pos_mouse_drag_start); 
	if (si.valid == false)
		return;
	drag_or_edit_started = false;
	edit_item(si);
}

void ListView_Impl::on_mouse_lbutton_up(std::shared_ptr<GUIMessage> &msg, InputEvent &input_event)
{
	Point pos = input_event.mouse_pos;
	drag_or_edit_started = false;
	listview->capture_mouse(false);
}

void ListView_Impl::on_mouse_lbutton_doubleclick(std::shared_ptr<GUIMessage> &msg, InputEvent &input_event)
{
	if(!selection.get_first().is_null())
		if (!func_item_doubleclick.is_null())
			func_item_doubleclick.invoke(selection.get_first().get_item());
}

void ListView_Impl::on_mouse_rbutton_down(std::shared_ptr<GUIMessage> &msg, InputEvent &input_event)
{
	cancel_edit();
	on_mouse_button_down(msg, input_event); 
}

void ListView_Impl::on_mouse_rbutton_up(std::shared_ptr<GUIMessage> &msg, InputEvent &input_event)
{
	drag_or_edit_started = false;
	if (!func_mouse_right_up.is_null())
		func_mouse_right_up.invoke(input_event.mouse_pos);
}

void ListView_Impl::on_mouse_move(std::shared_ptr<GUIMessage> &msg, InputEvent &input_event)
{
	if (drag_or_edit_started && input_event.mouse_pos.distance(pos_mouse_drag_start) > 5)
	{
		drag_or_edit_started = false;
		edit_timer.stop();
		if (!func_begin_drag.is_null())
			func_begin_drag.invoke();
		return;
	}

	std::vector<ListViewShownItem> &items = layout->get_shown_items();	

	std::vector<ListViewShownItem>::iterator it;
	for (it = items.begin(); it != items.end(); ++it)
	{
		ListViewShownItem &si = (*it);
		bool mouse_over = false;

		// mouse over the first text rectangle?
		if (!si.rect_text.empty())
		{
			Rect &rect = si.rect_text[0];
			if (rect.contains(input_event.mouse_pos))
				mouse_over = true;
		}

		if (si.rect_icon.contains(input_event.mouse_pos))
			mouse_over = true;

		if (si.mouse_over != mouse_over)
		{
			si.mouse_over = mouse_over;
			listview->request_repaint();
		}
	}
}

void ListView_Impl::on_mouse_enter()
{
	listview->set_pseudo_class(CssStr::hot, true);
	listview->request_repaint();
}

void ListView_Impl::on_mouse_leave()
{
	listview->set_pseudo_class(CssStr::hot, false);
	listview->request_repaint();
}

void ListView_Impl::on_column_added(ListViewColumnHeader col)
{
	cancel_edit();
	update_part_positions();
}

void ListView_Impl::on_resized()
{
	cancel_edit();
	update_part_positions();
	layout->invalidate();
}

void ListView_Impl::on_render(Canvas &canvas, const Rect &update_rect)
{
	Rect rect = listview->get_geometry();
	part_columns_bg.render_box(canvas, rect_columns, update_rect);

	listview->push_cliprect(canvas, rect_columns_content);

	std::vector<ListViewShownItem> &items = layout->get_shown_items();
	std::vector<ListViewColumn> &columns = layout->get_columns();
	std::vector<ListViewRow> &rows = layout->get_rows();
	renderer->render(canvas, columns, rows, items, update_rect);

	listview->pop_cliprect(canvas);
}

void ListView_Impl::create_components()
{
	lineedit = new LineEdit(listview);
	lineedit->set_visible(false);
	lineedit->func_before_edit_changed().set(this, &ListView_Impl::on_before_edit_item);
	lineedit->func_after_edit_changed().set(this, &ListView_Impl::on_after_edit_item);
	lineedit->func_focus_lost().set(this, &ListView_Impl::on_lineedit_focus_lost);

	header = new ListViewHeader(listview); // geometry updated later.
	header->func_column_added().set(this, &ListView_Impl::on_column_added);
	header->func_column_size_changed().set(this, &ListView_Impl::on_column_added);
	header->set_display_mode(display_mode);

	scrollbar = new ScrollBar(listview);
	scrollbar->func_scroll().set(this, &ListView_Impl::on_scroll);
	scrollbar->set_visible(false);
	scrollbar->set_vertical();

	renderer->set_listview_header(header);
}

void ListView_Impl::create_parts()
{
	part_mouse_selection = GUIThemePart(listview, "mouse_selection");
	part_columns_bg = GUIThemePart(listview, "columns_bg");

	update_part_positions();
}

void ListView_Impl::update_part_positions()
{
	cancel_edit();

	Rect rect(listview->get_geometry().get_size());
	rect_content = listview->get_content_box();

	header->update_geometry(rect_content);

	rect_columns.left = rect_content.left;
	rect_columns.top = rect_content.top + header->get_geometry().get_height();
	rect_columns.right = rect_content.right;
	rect_columns.bottom = rect_content.bottom;

	rect_columns_content = part_columns_bg.get_content_box(rect_columns);

	if (layout)
	{
		std::vector<ListViewColumn> &columns = layout->get_columns();
		std::vector<ListViewColumn>::iterator it;
		for (it = columns.begin(); it != columns.end(); ++it)
		{
			ListViewColumn &col = (*it);
			col.rect.bottom = rect_columns_content.bottom;
		}

		layout->set_view_rect(rect_columns_content);
	}

	update_scrollbar();
}

void ListView_Impl::update_scrollbar()
{
	cancel_edit();

	if (!layout)
	{
		return;
	}

	Rect rect(
		rect_content.get_width()-scrollbar->get_content_box().get_width(),
		rect_content.top, 
		rect_content.get_width(),
		rect_content.get_height());

	scrollbar->set_geometry(rect);

	int total_height = layout->get_total_size().height;
	bool visible = total_height > rect_columns_content.get_height();
	scrollbar->calculate_ranges(rect_columns_content.get_height(), total_height);
	scrollbar->set_line_step(layout->get_row_height());
	scrollbar->set_visible(visible);
	if (visible == false)
		layout->set_scroll_offset(Point(0, 0));
}

void ListView_Impl::on_scroll()
{ 
	cancel_edit();

	layout->set_scroll_offset(Point(0,scrollbar->get_position()));
	layout->invalidate();
	listview->request_repaint();
}

void ListView_Impl::on_item_added()
{
	layout->invalidate();
	update_scrollbar();
	listview->request_repaint();
}

void ListView_Impl::on_item_modified(ListViewItem item)
{
	layout->invalidate();
	// update_scrollbar();
	listview->request_repaint();
}


void ListView_Impl::on_item_deleted(ListViewItem item)
{
	if (item.is_null()) // item.remove_children invokes the callback with a null item... the listview is empty.
		selection.clear();
	else
		selection.remove(item);
	layout->invalidate();
	update_scrollbar();
	listview->request_repaint();
}


ListViewItem ListView_Impl::find(ListViewItem &it, const std::string &col_id, const std::string &str, bool recursive)
{
	while (it.is_item())
	{
		if (it.get_column(col_id).get_text() == str)
			return it;

		if (it.has_children() && recursive)
		{
			ListViewItem it_child = it.get_first_child();
			ListViewItem search_result = find(it_child, col_id, str, recursive);
			if (!search_result.is_null())
				return search_result;
		}

		it = it.get_next_sibling();
	}

	return ListViewItem();
}

ListViewItem ListView_Impl::find(ListViewItem &it, int id, bool recursive)
{
	while (it.is_item())
	{
		if (it.get_id() == id)
			return it;

		if (it.has_children() && recursive)
		{
			ListViewItem it_child = it.get_first_child();
			ListViewItem search_result = find(it_child, id, recursive);
			if (!search_result.is_null())
				return search_result;
		}

		it = it.get_next_sibling();
	}

	return ListViewItem();
}

ListViewItem ListView_Impl::find(ListViewItem &it, std::shared_ptr<ListViewItemUserData> userdata, bool recursive)
{
	while (it.is_item())
	{
		if (it.get_userdata() == userdata)
			return it;

		if (it.has_children() && recursive)
		{
			ListViewItem it_child = it.get_first_child();
			ListViewItem search_result = find(it_child, userdata, recursive);
			if (!search_result.is_null())
				return search_result;
		}

		it = it.get_next_sibling();
	}

	return ListViewItem();
}

/*

 edit_item(..) and on_after_edit_item(..) call get_lineeddit_rect
 depends on text in lineedit -> pass as param!


 layout->get_lineedit_rect(item, new_text)
		1. Shownitems has cell rect... find it!
		2. Recalculate text_rect[0]!
		2. Expand text_rect[0] according to css rules.
		3. Return rect.

 */
void ListView_Impl::edit_item(ListViewShownItem &si)
{
	edited_item = si.item;
	edited_item.impl->selected = false;

	ListViewColumnHeader col = header->get_first_column();
	ListViewColumnData col_data = si.item.get_column(col.get_column_id());
	lineedit->set_text(col_data.get_text());
	lineedit->resize_to_fit(listview->get_geometry().get_width());

	just_launched_lineedit = true; // prevent listview focus lost from
	lineedit->set_focus();

	Rect edit_rect = layout->get_lineedit_rect(edited_item, lineedit->get_text_size(col_data.get_text()));
	lineedit->set_geometry(edit_rect);

	lineedit->set_visible();

	lineedit->request_repaint();
	listview->request_repaint();
}

void ListView_Impl::on_before_edit_item(InputEvent &e)
{
	if (edited_item.is_null())
		return;

	if (e.type == InputEvent::pressed)
	{
		if (e.id == keycode_return || e.id == keycode_numpad_enter)
		{
			std::string col_id = header->get_first_column().get_column_id();
			std::string new_text = lineedit->get_text();

			bool apply_change = true;
			if (!func_item_edited.is_null())
			{
				apply_change = func_item_edited.invoke(edited_item, new_text);
			}

			if (apply_change)
			{
				edited_item.set_column_text(col_id, new_text);
			}
			edited_item.impl->selected = true;
			edited_item = ListViewItem();

			lineedit->set_text("");
			lineedit->set_visible(false);
			lineedit->request_repaint();
			listview->set_focus();
			layout->invalidate();
		}
		if (e.id == keycode_escape)
		{
			cancel_edit();
			listview->set_focus();
		}
	}
}

void ListView_Impl::on_after_edit_item(InputEvent &e)
{
	Rect rect = layout->get_lineedit_rect(edited_item, lineedit->get_text_size());

	int cursor_pos = lineedit->get_cursor_pos(); // save cursor pos, because set_geometry on lineedit sets cursor pos to 0.
	lineedit->set_geometry(rect);
	lineedit->set_cursor_pos(cursor_pos);
}

void ListView_Impl::cancel_edit()
{
	if (!edited_item.is_null())
	{
		edited_item.impl->selected = true;
		edited_item = ListViewItem();
		lineedit->set_text("");
		lineedit->set_visible(false);
		listview->request_repaint();
	}
}

void ListView_Impl::on_lineedit_focus_lost()
{
	cancel_edit();
}

}
