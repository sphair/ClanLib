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
*/

#pragma once


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

/////////////////////////////////////////////////////////////////////////////
// CL_ListView_Impl Class:

class CL_ListView_Impl
{
public:
	CL_ListView_Impl()
	  : display_mode(listview_mode_details), listview(0), layout(0), renderer(0), scrollbar(0),
		  header(0), lineedit(0), multiple_selection(false), select_whole_row(false),
		  context_menu(CL_PopupMenu::create_null_object()), just_launched_lineedit(false),
		  show_detail_icon(true), show_detail_opener(true)
	{
		CL_SharedPtr<CL_ListViewItem_Impl> item_impl(new CL_ListViewItem_Impl());
		document_item = CL_ListViewItem(item_impl);
	}

	~CL_ListView_Impl()
	{
		delete renderer;
		delete layout;
	}

	void on_process_message(CL_GUIMessage &msg);

	void on_resized();

	void on_render(CL_GraphicContext &gc, const CL_Rect &update_rect);

	void on_mouse_lbutton_down(CL_GUIMessage_Input &input, CL_InputEvent &input_event);
	void on_mouse_lbutton_up(CL_GUIMessage_Input &input, CL_InputEvent &input_event);

	void on_mouse_rbutton_down(CL_GUIMessage_Input &input, CL_InputEvent &input_event);
	void on_mouse_rbutton_up(CL_GUIMessage_Input &input, CL_InputEvent &input_event);

	bool on_keyboard_pressed(CL_InputEvent &event);

	bool on_keyboard_released(CL_InputEvent &event);

	void on_mouse_move(CL_GUIMessage_Input &input, CL_InputEvent &input_event);

	void on_mouse_enter();

	void on_mouse_leave();

	void on_column_added(CL_ListViewColumnHeader col);

	void on_item_added();

	void on_item_modified(CL_ListViewItem item);

	void on_item_deleted(CL_ListViewItem item);

	void on_scroll();

	void on_before_edit_item(CL_InputEvent &e);

	void on_after_edit_item(CL_InputEvent &e);

	void on_style_changed();

	void on_lineedit_focus_lost();

	void update_part_positions();

	void create_components();

	void create_parts();

	void cancel_edit();

	CL_ListViewItem find(CL_ListViewItem &item, const CL_StringRef &col_id, const CL_StringRef &str, bool recursive);

	CL_ListViewItem find(CL_ListViewItem &item, const CL_SharedPtr<CL_ListViewItemUserData> userdata, bool recursive);

	CL_ListViewItem find(CL_ListViewItem &it, int id, bool recursive);

	CL_ListViewColumnHeader create_header_column(const CL_StringRef &column_id, const CL_StringRef &caption);

	CL_ListViewDisplayMode display_mode;

	CL_ListView *listview;

	CL_ListViewLayout *layout;

	CL_ListViewRenderer *renderer;

	CL_ScrollBar *scrollbar;

	CL_ListViewItem document_item;

	CL_ListViewHeader *header;

	CL_Callback_v1<CL_ListViewSelection> func_selection_changed;

	CL_Callback_2<bool, CL_ListViewItem, CL_String &> func_item_edited;

	CL_Callback_v1<CL_InputEvent> func_key_pressed;
	CL_Callback_v1<CL_InputEvent> func_key_released;

	CL_Callback_v1<CL_Point> func_mouse_right_up;

	CL_ListViewSelection selection;

	CL_ListViewIconList icon_list;

	CL_ListViewItem edited_item;

	CL_LineEdit *lineedit;

	CL_PopupMenu context_menu;

	CL_PopupMenu item_context_menu;

	CL_GUIThemePart part_mouse_selection;
	CL_GUIThemePart part_columns_bg;
	CL_GUIThemePart part_component;

	CL_Rect rect_content;
	CL_Rect rect_columns;
	CL_Rect rect_columns_content;
	CL_Rect selection_border;

	CL_Point pos_mouse_drag_start;

	bool multiple_selection;
	bool select_whole_row;
	bool just_launched_lineedit;

	bool show_detail_icon;
	bool show_detail_opener;

private:
	// void update_shown_items();

	void update_scrollbar();

	void edit_item(ListViewShownItem &si);


	CL_Rect get_opener_rect(
		const CL_Rect &cell_content_rect,
		CL_ListViewItem item,
		int offset_x);

	CL_Rect get_icon_rect(
		const CL_Rect &cell_content_rect,
		CL_ListViewItem item,
		int offset_x);
};
