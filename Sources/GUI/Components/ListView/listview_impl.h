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
*/

#pragma once

#include "GUI/precomp.h"
#include "API/GUI/gui_component.h"
#include "API/GUI/gui_message.h"
#include "API/GUI/gui_manager.h"
#include "API/GUI/gui_message_input.h"
#include "API/GUI/gui_message_pointer.h"
#include "API/GUI/gui_theme_part.h"
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
#include "API/GUI/Components/popupmenu.h"
#include "API/Display/Window/input_event.h"
#include "API/Display/Font/font.h"
#include "API/Display/Window/keys.h"
#include "listview_item_impl.h"
#include "listview_shown_item.h"
#include "listview_layout.h"
#include "listview_renderer.h"
#include "listview_layout_details.h"
#include "listview_layout_icons.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// ListView_Impl Class:

class ListView_Impl
{
public:
	ListView_Impl()
	  : display_mode(listview_mode_details), listview(0), layout(0), renderer(0), scrollbar(0),
		  header(0), lineedit(0), drag_or_edit_started(false), multiple_selection(false), select_whole_row(false),
		  context_menu(PopupMenu::create_null_object()), just_launched_lineedit(false),
		  show_detail_icon(true), show_detail_opener(true)
	{
		std::shared_ptr<ListViewItem_Impl> item_impl(new ListViewItem_Impl());
		document_item = ListViewItem(item_impl);
		edit_timer.func_expired() = bind_member(this, &ListView_Impl::on_drag_or_edit_timeout);
	}

	~ListView_Impl()
	{
		delete renderer;
		delete layout;
	}

	void on_process_message(std::shared_ptr<GUIMessage> &msg);

	void on_resized();

	void on_render(Canvas &canvas, const Rect &update_rect);

	void on_mouse_lbutton_down(std::shared_ptr<GUIMessage> &msg, InputEvent &input_event);
	void on_mouse_lbutton_up(std::shared_ptr<GUIMessage> &msg, InputEvent &input_event);
	void on_mouse_lbutton_doubleclick(std::shared_ptr<GUIMessage> &msg, InputEvent &input_event);
	void on_mouse_button_down(std::shared_ptr<GUIMessage> &msg, InputEvent &input_event);

	void on_mouse_rbutton_down(std::shared_ptr<GUIMessage> &msg, InputEvent &input_event);
	void on_mouse_rbutton_up(std::shared_ptr<GUIMessage> &msg, InputEvent &input_event);

	bool on_keyboard_pressed(InputEvent &event);
	bool on_keyboard_released(InputEvent &event);

	void on_mouse_move(std::shared_ptr<GUIMessage> &msg, InputEvent &input_event);
	void on_mouse_enter();
	void on_mouse_leave();

	void on_column_added(ListViewColumnHeader col);

	void on_item_added();
	void on_item_modified(ListViewItem item);
	void on_item_deleted(ListViewItem item);

	void on_scroll();
	void on_before_edit_item(InputEvent &e);
	void on_after_edit_item(InputEvent &e);
	void on_lineedit_focus_lost();
	void update_part_positions();
	void create_components();
	void create_parts();
	void cancel_edit();
	ListViewItem find(ListViewItem &item, const std::string &col_id, const std::string &str, bool recursive);
	ListViewItem find(ListViewItem &item, const std::shared_ptr<ListViewItemUserData> userdata, bool recursive);
	ListViewItem find(ListViewItem &it, int id, bool recursive);
	ListViewColumnHeader create_header_column(const std::string &column_id, const std::string &caption);

	ListViewDisplayMode display_mode;
	ListView *listview;
	ListViewLayout *layout;
	ListViewRenderer *renderer;
	ScrollBar *scrollbar;
	ListViewItem document_item;
	ListViewHeader *header;

	std::function<void(ListViewSelection)> func_selection_changed;
	std::function<void(const ListViewItem &)> func_item_doubleclick;

	std::function<bool(ListViewItem, std::string &)> func_item_edited;

	std::function<void(InputEvent&)> func_key_pressed;
	std::function<void(InputEvent&)> func_key_released;

	std::function<void(Point)> func_mouse_right_up;

	std::function<void(const ListViewItem &)> func_item_opened;
	std::function<void(const ListViewItem &)> func_item_closed;
	std::function<void()> func_begin_drag;

	ListViewSelection selection;
	ListViewIconList icon_list;
	ListViewItem edited_item;
	LineEdit *lineedit;
	PopupMenu context_menu;
	PopupMenu item_context_menu;

	GUIThemePart part_mouse_selection;
	GUIThemePart part_columns_bg;
	

	Rect rect_content;
	Rect rect_columns;
	Rect rect_columns_content;
	Rect selection_border;

	Point pos_mouse_drag_start;
	bool drag_or_edit_started;
	Timer edit_timer;

	bool multiple_selection;
	bool select_whole_row;
	bool just_launched_lineedit;

	bool show_detail_icon;
	bool show_detail_opener;

private:
	// void update_shown_items();

	void update_scrollbar();
	void edit_item(ListViewShownItem &si);
	void on_drag_or_edit_timeout();

	Rect get_opener_rect(
		const Rect &cell_content_rect,
		ListViewItem item,
		int offset_x);

	Rect get_icon_rect(
		const Rect &cell_content_rect,
		ListViewItem item,
		int offset_x);
};

}
