
#pragma once

#include "listview_ex_mode.h"
#include "listview_ex_document.h"

class ListviewEx;
class ListviewExDocument;

class ListviewExIconMode : public ListviewExMode
{
public:
	ListviewExIconMode(ListviewEx *listview);

	int get_line_step();
	CL_Size get_absolute_size();

	CL_Point to_absolute_position(const CL_Point &position);
	CL_Rect to_absolute_position(const CL_Rect &position);
	CL_Point to_relative_position(const CL_Point &position);
	CL_Rect to_relative_position(const CL_Rect &position);

	int hit_test(const CL_Point &position);
	int insert_mark_hit_test(const CL_Point &position);

	CL_Rect get_item_rect(int item_index, int column = 0);
	CL_Rect get_item_icon_rect(int item_index, int column = 0);
	CL_Rect get_item_text_rect(int item_index, int column = 0);

	void draw_area(CL_GraphicContext &gc, const CL_Rect &rect, const CL_Rect &clip_rect);
	void select_items(const CL_Rect &selection_rect);

private:
	ListviewExDocument *get_document();
	const ListviewExDocument *get_document() const;
	CL_Size get_item_size() const;
	int get_items_per_line() const;
	int get_num_lines() const;
	int get_item_line(int item_index) const;
	int get_item_column(int item_index) const;
	static int clamp(int value, int min_value, int max_value);

	struct CoveredArea
	{
		CoveredArea() : start_line(0), end_line(0), start_index(0), end_index(0) { }
		int start_line;
		int end_line;
		int start_index;
		int end_index;
	};
	CoveredArea get_covered_area(const CL_Rect &rect) const;

	ListviewEx *listview;
	CL_Font font_text;

	enum
	{
		max_text_width = 100,
		text_height = 24,
	};

	CL_GUIThemePart part_icon_frame;
	CL_GUIThemePart part_album_icon_frame;
	CL_GUIThemePart part_item_hot;
	CL_GUIThemePart part_item_selected;
	CL_GUIThemePart part_item_drophighlighted;
};
