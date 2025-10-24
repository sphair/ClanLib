
#pragma once

#include "listview_ex_document.h"

class ListviewExItem;
class ListviewExMode;

class ListviewEx : public CL_GUIComponent
{
public:
	ListviewEx(CL_GUIComponent *parent);
	~ListviewEx();

	void set_report_view();
	void set_icon_view();

	void insert_column(int index, const CL_StringRef &text, int size);

	CL_Size get_icon_size() const;
	void set_icon_size(const CL_Size &size);
	int add_icon(const CL_Image &pixelbuffer);
	void remove_all_icons();

	ListviewExItem *get_item(int index);
	int find_item(ListviewExItem *item);
	int insert_item(int index, ListviewExItem *item, const CL_StringRef &text, int group_id = -1);
	int append_item(ListviewExItem *item, const CL_StringRef &text, int group_id = -1);
	void set_item_text(int item_index, int column, const CL_StringRef &text);
	void set_item_icon(int item_index, int icon_index);
	void set_item_album(int item_index, bool is_album);
	void set_item_drop_highlighted(int item_index, bool enabled);
	void remove_all_items();
	void remove_item(int item_index);

	int get_next_selected_item(int last_index = -1) const;
	int get_prev_selected_item(int last_index = -1) const;
	int get_selected_count() const;

	void select_item(int index);
	void select_item_range(int first_index, int last_index);
	void clear_selection();

	void set_insert_mark(int item_index);

	int hit_test(const CL_Point &position);
	int insert_mark_hit_test(const CL_Point &position);

	void ensure_visible(int item_index, bool partial_ok = false);
	void begin_label_edit(int item_index);

	CL_Callback_v2<int, const CL_Point &> cb_begin_drag;                // void on_begin_drag(index, cursor_pos)
//	CL_Callback_v1<int> cb_delete_item;                                 // void on_delete_item(index)
	CL_Callback_1<bool, int> cb_begin_label_edit;                       // bool on_begin_label_edit(index)
	CL_Callback_2<bool, int, const CL_String &> cb_end_label_edit;      // bool on_end_label_edit(index, new_label)
	CL_Callback_v1<int> cb_cancelled_label_edit;                        // void on_cancelled_label_edit(index)
	CL_Callback_v3<int, int, const CL_Point &> cb_dblclk;               // void on_dblclk(index, column, cursor_pos)
	CL_Callback_v3<int, int, const CL_Point &> cb_rclick;               // void on_rclick(index, column, cursor_pos)
	CL_Callback_v0 cb_return;                                           // void on_return()
	CL_Callback_v1<int> &func_item_focus_gained() { return document.cb_item_focus_gained; }  // void on_item_focus_gained(index)
	CL_Callback_v1<int> &func_item_focus_lost() { return document.cb_item_focus_lost; }      // void on_item_focus_lost(index)
	CL_Callback_4<bool, CL_GraphicContext &, int, int, const CL_Rect &> cb_custom_draw_item; // bool on_custom_draw_item(gc, item_index, column, item_column_rect)

	ListviewExDocument *get_document() { return &document; }
	int get_horz_scrollbar_position() const { return horz_scrollbar->get_position(); }
	int get_vert_scrollbar_position() const { return vert_scrollbar->get_position(); }
	const CL_Rect &get_selection_rect() const { return mouse_selection_rect; }

private:
	void on_size();
	void on_render(CL_GraphicContext &gc, const CL_Rect &clip_rect);
	bool on_input_pressed(const CL_InputEvent &e);
	bool on_input_released(const CL_InputEvent &e);
	bool on_input_dblclk(const CL_InputEvent &e);
	bool on_input_pointer_moved(const CL_InputEvent &e);
	bool on_pointer_enter();
	bool on_pointer_exit();
	void on_horz_scroll_position_changed();
	void on_vert_scroll_position_changed();
	void update_scrollbars();
//	CL_Rect get_item_area() const;
//	void find_line_and_column(const CL_Point &position, int &out_line, int &out_col);

	CL_Size icon_size;
	std::auto_ptr<CL_ScrollBar> vert_scrollbar;
	std::auto_ptr<CL_ScrollBar> horz_scrollbar;
	enum
	{
		scrollbar_width = 16,
		scrollbar_height = 16
	};

	CL_GUIThemePart part_background;
	CL_GUIThemePart part_mouse_selection_rect;
	enum MouseMoveMode
	{
		mousemove_none,
		mousemove_item_clicked,
		mousemove_net_selection,
		mousemove_item_drag
	};
	MouseMoveMode mousemove_mode;
	CL_Point mouse_down_pos;
	CL_Point last_mouse_move_pos;
	CL_Rect mouse_selection_rect;

	ListviewExDocument document;
	std::auto_ptr<ListviewExMode> mode;
};

class ListviewExItem
{
public:
	virtual ~ListviewExItem() { }
};
