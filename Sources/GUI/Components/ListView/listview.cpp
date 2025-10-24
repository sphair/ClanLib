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

/////////////////////////////////////////////////////////////////////////////
// CL_ListView Construction:

CL_ListView::CL_ListView(CL_GUIComponent *parent)
: CL_GUIComponent(parent), impl(new CL_ListView_Impl)
{
	set_type_name("listview");
	set_focus_policy(focus_local);
	impl->listview = this;

	impl->renderer = new CL_ListViewRenderer(this);
	impl->renderer->create_parts();
	impl->renderer->set_icon_list(impl->icon_list);

	impl->create_components();

	func_process_message().set(impl.get(), &CL_ListView_Impl::on_process_message);
	func_render().set(impl.get(), &CL_ListView_Impl::on_render);
	func_resized().set(impl.get(), &CL_ListView_Impl::on_resized);
	func_style_changed().set(impl.get(), &CL_ListView_Impl::on_style_changed);

	impl->document_item.impl->func_item_added.set(impl.get(), &CL_ListView_Impl::on_item_added);
	impl->document_item.impl->func_item_modified.set(impl.get(), &CL_ListView_Impl::on_item_modified);
	impl->document_item.impl->func_item_deleted.set(impl.get(), &CL_ListView_Impl::on_item_deleted);
	impl->create_parts();

	set_display_mode(listview_mode_details);

//	impl->update_part_positions();
}

CL_ListView::~CL_ListView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_ListView Attributes:

int CL_ListView::get_scroll_position()
{
	return impl->scrollbar->get_position();
}

int CL_ListView::get_scroll_max_position()
{
	return impl->scrollbar->get_max();
}

CL_ListView *CL_ListView::get_named_item(CL_GUIComponent *reference_component, const CL_StringRef &id)
{
	CL_ListView *object = NULL;
	if (reference_component)
		object = dynamic_cast<CL_ListView*>(reference_component->get_named_item(id));

	if (!object)
		throw CL_Exception(cl_format("Cannot find CL_ListView named item: %1", id));

	return object;
}

CL_ListViewItem CL_ListView::get_document_item() const
{
	return impl->document_item;
}

bool CL_ListView::get_select_whole_row() const
{
	return impl->select_whole_row;
}

CL_ListViewHeader *CL_ListView::get_header() const
{
	return impl->header;
}

CL_ListViewIconList CL_ListView::get_icon_list() const
{
	return impl->icon_list;
}

CL_ListViewSelection CL_ListView::get_selected_items() const
{
	return impl->selection;
}

CL_ListViewItem CL_ListView::get_selected_item()
{
	if (impl->selection.get_first().is_item())
	{
		return impl->selection.get_first().get_item();
	}

	return CL_ListViewItem();
}

CL_Size CL_ListView::get_preferred_size() const
{
	return impl->part_component.get_preferred_size();
}

CL_ListViewDisplayMode CL_ListView::get_display_mode() const
{
	return impl->display_mode;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ListView Operations:

void CL_ListView::set_scroll_position(int pos)
{
	impl->scrollbar->set_position(pos);
	impl->on_scroll();
}

CL_ListViewItem CL_ListView::create_item()
{
	impl->cancel_edit();

	CL_SharedPtr<CL_ListViewItem_Impl> item_impl(new CL_ListViewItem_Impl());
	CL_ListViewItem new_item = CL_ListViewItem(item_impl);
	return new_item;
}

void CL_ListView::set_select_whole_row(bool value)
{
	impl->cancel_edit();

	impl->select_whole_row = value;
	request_repaint();
}

void CL_ListView::set_multi_select(bool value)
{
	impl->cancel_edit();

	impl->multiple_selection = value;
}

void CL_ListView::set_display_mode(CL_ListViewDisplayMode mode)
{
	impl->cancel_edit();

	impl->display_mode = mode;
	impl->header->set_display_mode(mode);

	if (impl->layout)
		delete impl->layout;

	if (mode == listview_mode_details)
	{
		impl->layout = new CL_ListViewLayoutDetails(this);
	}
	else if (mode == listview_mode_icons)
	{
		impl->layout = new CL_ListViewLayoutIcons(this);
	}
	else if (mode == listview_mode_thumbnails)
	{
		impl->layout = new CL_ListViewLayoutThumbnails(this);
	}
	else if (mode == listview_mode_tiles)
	{
		throw CL_Exception("ListView doesn't support listview_mode_tiles yet");
	}
	else if (mode == listview_mode_list)
	{
		throw CL_Exception("ListView doesn't support listview_mode_list yet");
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

void CL_ListView::show_detail_icon(bool enable)
{
	impl->show_detail_icon = enable;
	impl->layout->set_show_detail_icon(impl->show_detail_icon);
}

void CL_ListView::show_detail_opener(bool enable)
{
	impl->show_detail_opener = enable;
	impl->layout->set_show_detail_opener(impl->show_detail_opener);
}

void CL_ListView::clear()
{
	impl->cancel_edit();

	clear_selection();
	impl->document_item.remove_children();
	request_repaint();
}

void CL_ListView::clear_selection()
{
	impl->cancel_edit();

	impl->selection.clear();
	request_repaint();
}

void CL_ListView::set_selected(CL_ListViewItem &item, bool selected)
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

CL_ListViewItem CL_ListView::find(const CL_StringRef &col_id, const CL_StringRef &str, bool recursive)
{
	CL_ListViewItem it = impl->document_item.get_first_child();
	return impl->find(it, col_id, str, recursive);
}

CL_ListViewItem CL_ListView::find(CL_SharedPtr<CL_ListViewItemUserData> userdata, bool recursive)
{
	CL_ListViewItem it = impl->document_item.get_first_child();
	return impl->find(it, userdata, recursive);
}

CL_ListViewItem CL_ListView::find(int id, bool recursive)
{
	CL_ListViewItem it = impl->document_item.get_first_child();
	return impl->find(it, id, recursive);
}

/////////////////////////////////////////////////////////////////////////////
// CL_ListView Events:

CL_Callback_v1<CL_ListViewSelection> &CL_ListView::func_selection_changed()
{
	impl->cancel_edit();

	return impl->func_selection_changed;
}

CL_Callback_2<bool, CL_ListViewItem, CL_String &> &CL_ListView::func_item_edited()
{
	return impl->func_item_edited;
}

CL_Callback_v1<CL_InputEvent> & CL_ListView::func_key_pressed()
{
	return impl->func_key_pressed;
}

CL_Callback_v1<CL_InputEvent> & CL_ListView::func_key_released()
{
	return impl->func_key_released;
}

CL_Callback_v1<CL_Point> & CL_ListView::func_mouse_right_up()
{
	return impl->func_mouse_right_up;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ListView Implementation:

void CL_ListView_Impl::on_process_message(CL_GUIMessage &msg)
{
	if (msg.is_type(CL_GUIMessage_Input::get_type_name()))
	{
		CL_GUIMessage_Input input = msg;
		CL_InputEvent input_event = input.get_event();

		if (input_event.type == CL_InputEvent::pointer_moved)
		{
			on_mouse_move(input, input_event);
		}
		else if (input_event.type == CL_InputEvent::pressed && input_event.id == CL_MOUSE_LEFT)
		{
			on_mouse_lbutton_down(input, input_event);
			msg.set_consumed();
		}
		else if (input_event.type == CL_InputEvent::pressed && input_event.id == CL_MOUSE_WHEEL_UP)
		{
			scrollbar->set_position(scrollbar->get_position()-scrollbar->get_line_step());
			on_scroll();
			msg.set_consumed();
		}
		else if (input_event.type == CL_InputEvent::pressed && input_event.id == CL_MOUSE_WHEEL_DOWN)
		{
			scrollbar->set_position(scrollbar->get_position() + scrollbar->get_line_step());
			on_scroll();
			msg.set_consumed();
		}
		else if (input_event.type == CL_InputEvent::released && input_event.id == CL_MOUSE_LEFT)
		{
			on_mouse_lbutton_up(input, input_event);
			msg.set_consumed();
		}
		else if (input_event.type == CL_InputEvent::pressed && input_event.id == CL_MOUSE_RIGHT)
		{
			on_mouse_rbutton_down(input, input_event);
			msg.set_consumed();
		}
		else if (input_event.type == CL_InputEvent::released && input_event.id == CL_MOUSE_RIGHT)
		{
			on_mouse_rbutton_up(input, input_event);
			msg.set_consumed();
		}
		else if (input_event.type == CL_InputEvent::pressed && input_event.device.get_type() == CL_InputDevice::keyboard)
		{
			bool consumed = on_keyboard_pressed(input_event);
			if (consumed)
				msg.set_consumed();
		}
		else if (input_event.type == CL_InputEvent::released && input_event.device.get_type() == CL_InputDevice::keyboard)
		{
			bool consumed = on_keyboard_released(input_event);
			if (consumed)
				msg.set_consumed();
		}
	}
	else if (msg.is_type(CL_GUIMessage_Pointer::get_type_name()))
	{
		CL_GUIMessage_Pointer pointer = msg;
		if (pointer.get_pointer_type() == CL_GUIMessage_Pointer::pointer_leave)
			on_mouse_leave();
		else if (pointer.get_pointer_type() == CL_GUIMessage_Pointer::pointer_enter)
			on_mouse_enter();
	}
	else if (msg.is_type(CL_GUIMessage_FocusChange::get_type_name()))
	{
		CL_GUIMessage_FocusChange fc = msg;
		if (fc.get_focus_type() == CL_GUIMessage_FocusChange::gained_focus)
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

bool CL_ListView_Impl::on_keyboard_pressed(CL_InputEvent &event)
{
	bool event_consumed = false;

	if (document_item.get_child_count() == 0)
		return event_consumed;

	if (event.id == CL_KEY_LEFT || event.id == CL_KEY_RIGHT || event.id == CL_KEY_UP || event.id == CL_KEY_DOWN)
	{
		event_consumed = true;

		// Ensure we have a selected item.
		if (selection.get_first().is_null())
		{
			CL_ListViewItem item = document_item.get_first_child();
			listview->set_selected(item, true);
		}

		CL_ListViewSelectedItem sel = selection.get_last();
		CL_ListViewItem sel_item = sel.get_item();

		CL_ListViewItem new_selected_item;

		if (event.id == CL_KEY_LEFT)
		{
			if (display_mode == listview_mode_details)
			{
				if (sel_item.has_children() && sel_item.is_open())
				{
					sel_item.set_open(false);
					layout->invalidate();
					update_scrollbar();
					new_selected_item = sel_item;
				}
				else
				{
					new_selected_item = layout->get_neighbour(sel_item, CL_ListViewLayout::neighbour_left);
				}
			}
			else
			{
				new_selected_item = layout->get_neighbour(sel_item, CL_ListViewLayout::neighbour_left);
			}
		}
		else if (event.id == CL_KEY_RIGHT)
		{
			if (display_mode == listview_mode_details)
			{
				if (sel_item.has_children() && !sel_item.is_open())
				{
					sel_item.set_open(true);
					layout->invalidate();
					update_scrollbar();
					new_selected_item = sel_item;
				}
				else
				{
					new_selected_item = layout->get_neighbour(sel_item, CL_ListViewLayout::neighbour_right);
				}
			}
			else
			{
				new_selected_item = layout->get_neighbour(sel_item, CL_ListViewLayout::neighbour_right);
			}
		}
		else if (event.id == CL_KEY_UP)
		{
			new_selected_item = layout->get_neighbour(sel_item, CL_ListViewLayout::neighbour_up);
		}
		else if (event.id == CL_KEY_DOWN)
		{
			new_selected_item = layout->get_neighbour(sel_item, CL_ListViewLayout::neighbour_down);
		}

		if (!event.ctrl)
			listview->set_selected(new_selected_item);
		else
			selection.append(new_selected_item);

        if (!layout->is_visible(new_selected_item))
		{
			if (event.id == CL_KEY_UP || event.id == CL_KEY_LEFT)
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
	else if (event.id == CL_KEY_F2)
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
	else if (event.type == CL_InputEvent::pressed && event.id == CL_KEY_HOME)
	{
		scrollbar->set_position(scrollbar->get_min());
		on_scroll();
		std::vector<ListViewShownItem> show_items = layout->get_shown_items();
		if (!show_items.empty())
			listview->set_selected(show_items[0].item, true);
	}
	else if (event.type == CL_InputEvent::pressed && event.id == CL_KEY_END)
	{
		scrollbar->set_position(scrollbar->get_max());
		on_scroll();
		std::vector<ListViewShownItem> show_items = layout->get_shown_items();
		if (!show_items.empty())
			listview->set_selected(show_items.back().item, true);
	}
	else if (event.type == CL_InputEvent::pressed && event.id == CL_KEY_PRIOR)
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
	else if (event.type == CL_InputEvent::pressed && event.id == CL_KEY_NEXT)
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

bool CL_ListView_Impl::on_keyboard_released(CL_InputEvent &event)
{
	bool event_consumed = false;

	if (event.id == CL_KEY_LEFT)
	{
	}
	else if (event.id == CL_KEY_RIGHT)
	{
	}
	else if (event.id == CL_KEY_UP)
	{
	}
	else if (event.id == CL_KEY_DOWN)
	{
	}
	else
	{
		if (!func_key_released.is_null())
			func_key_released.invoke(event);
	}

	return event_consumed;
}


void CL_ListView_Impl::on_mouse_lbutton_down(CL_GUIMessage_Input &input, CL_InputEvent &input_event)
{
	listview->set_focus();

	CL_Point pos = input_event.mouse_pos;
	pos_mouse_drag_start = pos;

	lineedit->set_visible(false);
	listview->set_focus();
	if (edited_item.is_item())
	{
		edited_item.impl->selected = true;
		listview->request_repaint();
		edited_item = CL_ListViewItem();
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
	}

	// Check if icon or text is clicked.
	if (si.rect_text[0].contains(pos) || si.rect_icon.contains(pos))
	{
		if (si.item.is_selected() && si.item.is_editable())
		{
			edit_item(si);
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
			}
		}
		listview->request_repaint();
	}

	listview->capture_mouse(true);
}

void CL_ListView_Impl::on_mouse_lbutton_up(CL_GUIMessage_Input &input, CL_InputEvent &input_event)
{
	CL_Point pos = input_event.mouse_pos;

	listview->capture_mouse(false);
}

void CL_ListView_Impl::on_mouse_rbutton_down(CL_GUIMessage_Input &input, CL_InputEvent &input_event)
{
	cancel_edit();

	// This is intentional, NOT a typo! Call lbutton_down when rbutton_down happens.
	on_mouse_lbutton_down(input, input_event); 
}

void CL_ListView_Impl::on_mouse_rbutton_up(CL_GUIMessage_Input &input, CL_InputEvent &input_event)
{
	if (!func_mouse_right_up.is_null())
		func_mouse_right_up.invoke(input_event.mouse_pos);
}

void CL_ListView_Impl::on_mouse_move(CL_GUIMessage_Input &input, CL_InputEvent &input_event)
{
	std::vector<ListViewShownItem> &items = layout->get_shown_items();	

	std::vector<ListViewShownItem>::iterator it;
	for (it = items.begin(); it != items.end(); ++it)
	{
		ListViewShownItem &si = (*it);
		bool mouse_over = false;

		// mouse over the first text rectangle?
		if (!si.rect_text.empty())
		{
			CL_Rect &rect = si.rect_text[0];
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

void CL_ListView_Impl::on_mouse_enter()
{
	part_component.set_state(CssStr::hot, true);
	listview->request_repaint();
}

void CL_ListView_Impl::on_mouse_leave()
{
	part_component.set_state(CssStr::hot, false);
	listview->request_repaint();
}

void CL_ListView_Impl::on_column_added(CL_ListViewColumnHeader col)
{
	cancel_edit();
	update_part_positions();
}

void CL_ListView_Impl::on_resized()
{
	cancel_edit();
	update_part_positions();
	layout->invalidate();
}

void CL_ListView_Impl::on_render(CL_GraphicContext &gc, const CL_Rect &update_rect)
{
	CL_Rect rect = listview->get_geometry();
	part_component.render_box(gc, rect.get_size(), update_rect);
	part_columns_bg.render_box(gc, rect_columns, update_rect);

	listview->push_cliprect(gc, rect_columns_content);

	std::vector<ListViewShownItem> &items = layout->get_shown_items();
	std::vector<ListViewColumn> &columns = layout->get_columns();
	std::vector<ListViewRow> &rows = layout->get_rows();
	renderer->render(gc, columns, rows, items, update_rect);

	listview->pop_cliprect(gc);
}

void CL_ListView_Impl::create_components()
{
	lineedit = new CL_LineEdit(listview);
	lineedit->set_visible(false);
	lineedit->func_before_edit_changed().set(this, &CL_ListView_Impl::on_before_edit_item);
	lineedit->func_after_edit_changed().set(this, &CL_ListView_Impl::on_after_edit_item);
	lineedit->func_focus_lost().set(this, &CL_ListView_Impl::on_lineedit_focus_lost);

	header = new CL_ListViewHeader(listview); // geometry updated later.
	header->func_column_added().set(this, &CL_ListView_Impl::on_column_added);
	header->func_column_size_changed().set(this, &CL_ListView_Impl::on_column_added);
	header->set_display_mode(display_mode);

	scrollbar = new CL_ScrollBar(listview);
	scrollbar->func_scroll().set(this, &CL_ListView_Impl::on_scroll);
	scrollbar->set_visible(false);
	scrollbar->set_vertical();

	renderer->set_listview_header(header);
}

void CL_ListView_Impl::create_parts()
{
	part_component = CL_GUIThemePart(listview);
	part_mouse_selection = CL_GUIThemePart(listview, "mouse_selection");
	part_columns_bg = CL_GUIThemePart(listview, "columns_bg");

	update_part_positions();
}

void CL_ListView_Impl::update_part_positions()
{
	cancel_edit();

	CL_Rect rect(listview->get_geometry().get_size());
	rect_content = part_component.get_content_box(rect);

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

void CL_ListView_Impl::update_scrollbar()
{
	cancel_edit();

	if (!layout)
	{
		return;
	}

	CL_Rect rect(
		rect_content.get_width()-scrollbar->get_preferred_width(),
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
		layout->set_scroll_offset(CL_Point(0, 0));
}

void CL_ListView_Impl::on_scroll()
{ 
	cancel_edit();

	layout->set_scroll_offset(CL_Point(0,scrollbar->get_position()));
	layout->invalidate();
	listview->request_repaint();
}

void CL_ListView_Impl::on_item_added()
{
	layout->invalidate();
	update_scrollbar();
	listview->request_repaint();
}

void CL_ListView_Impl::on_item_modified(CL_ListViewItem item)
{
	layout->invalidate();
	// update_scrollbar();
	listview->request_repaint();
}


void CL_ListView_Impl::on_item_deleted(CL_ListViewItem item)
{
	if (item.is_null()) // item.remove_children invokes the callback with a null item... the listview is empty.
		selection.clear();
	else
		selection.remove(item);
	layout->invalidate();
	update_scrollbar();
	listview->request_repaint();
}


CL_ListViewItem CL_ListView_Impl::find(CL_ListViewItem &it, const CL_StringRef &col_id, const CL_StringRef &str, bool recursive)
{
	while (it.is_item())
	{
		if (it.get_column(col_id).get_text() == str)
			return it;

		if (it.has_children() && recursive)
		{
			CL_ListViewItem it_child = it.get_first_child();
			CL_ListViewItem search_result = find(it_child, col_id, str, recursive);
			if (!search_result.is_null())
				return search_result;
		}

		it = it.get_next_sibling();
	}

	return CL_ListViewItem();
}

CL_ListViewItem CL_ListView_Impl::find(CL_ListViewItem &it, int id, bool recursive)
{
	while (it.is_item())
	{
		if (it.get_id() == id)
			return it;

		if (it.has_children() && recursive)
		{
			CL_ListViewItem it_child = it.get_first_child();
			CL_ListViewItem search_result = find(it_child, id, recursive);
			if (!search_result.is_null())
				return search_result;
		}

		it = it.get_next_sibling();
	}

	return CL_ListViewItem();
}

CL_ListViewItem CL_ListView_Impl::find(CL_ListViewItem &it, CL_SharedPtr<CL_ListViewItemUserData> userdata, bool recursive)
{
	while (it.is_item())
	{
		if (it.get_userdata() == userdata)
			return it;

		if (it.has_children() && recursive)
		{
			CL_ListViewItem it_child = it.get_first_child();
			CL_ListViewItem search_result = find(it_child, userdata, recursive);
			if (!search_result.is_null())
				return search_result;
		}

		it = it.get_next_sibling();
	}

	return CL_ListViewItem();
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
void CL_ListView_Impl::edit_item(ListViewShownItem &si)
{
	edited_item = si.item;
	edited_item.impl->selected = false;

	CL_ListViewColumnHeader col = header->get_first_column();
	CL_ListViewColumnData col_data = si.item.get_column(col.get_column_id());
	lineedit->set_text(col_data.get_text());
	lineedit->resize_to_fit(listview->get_geometry().get_width());

	just_launched_lineedit = true; // prevent listview focus lost from
	lineedit->set_focus();

	CL_Rect edit_rect = layout->get_lineedit_rect(edited_item, lineedit->get_text_size(col_data.get_text()));
	lineedit->set_geometry(edit_rect);

	lineedit->set_visible();

	lineedit->request_repaint();
	listview->request_repaint();
}

void CL_ListView_Impl::on_before_edit_item(CL_InputEvent &e)
{
	if (edited_item.is_null())
		return;

	if (e.type == CL_InputEvent::pressed)
	{
		if (e.id == CL_KEY_RETURN || e.id == CL_KEY_NUMPAD_ENTER)
		{
			CL_String col_id = header->get_first_column().get_column_id();
			CL_String new_text = lineedit->get_text();

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
			edited_item = CL_ListViewItem();

			lineedit->set_text("");
			lineedit->set_visible(false);
			lineedit->request_repaint();
			listview->set_focus();
			layout->invalidate();
		}
		if (e.id == CL_KEY_ESCAPE)
		{
			cancel_edit();
			listview->set_focus();
		}
	}
}

void CL_ListView_Impl::on_after_edit_item(CL_InputEvent &e)
{
	CL_Rect rect = layout->get_lineedit_rect(edited_item, lineedit->get_text_size());

	int cursor_pos = lineedit->get_cursor_pos(); // save cursor pos, because set_geometry on lineedit sets cursor pos to 0.
	lineedit->set_geometry(rect);
	lineedit->set_cursor_pos(cursor_pos);
}


void CL_ListView_Impl::on_style_changed()
{
	renderer->create_parts();
	layout->create_parts();
	layout->invalidate();
	update_scrollbar();
	listview->request_repaint();
	cancel_edit();
}

void CL_ListView_Impl::cancel_edit()
{
	if (!edited_item.is_null())
	{
		edited_item.impl->selected = true;
		edited_item = CL_ListViewItem();
		lineedit->set_text("");
		lineedit->set_visible(false);
		listview->request_repaint();
	}
}

void CL_ListView_Impl::on_lineedit_focus_lost()
{
	cancel_edit();
}
