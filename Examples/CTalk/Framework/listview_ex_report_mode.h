
#pragma once

#include "listview_ex_mode.h"
#include "listview_ex_document.h"

class ListviewEx;
class ListviewExDocument;

class ListviewExReportMode : public ListviewExMode
{
public:
	ListviewExReportMode(ListviewEx *listview);

	int get_line_step();
	CL_Size get_absolute_size();

	CL_Point to_absolute_position(const CL_Point &position);
	CL_Rect to_absolute_position(const CL_Rect &position);
	CL_Point to_relative_position(const CL_Point &position);
	CL_Rect to_relative_position(const CL_Rect &position);

	int hit_test(const CL_Point &absolute_position);
	int insert_mark_hit_test(const CL_Point &absolute_position);

	CL_Rect get_item_line_rect(int item_index);
	CL_Rect get_item_rect(int item_index, int column);
	CL_Rect get_item_icon_rect(int item_index, int column);
	CL_Rect get_item_text_rect(int item_index, int column);

	void draw_area(CL_GraphicContext &gc, const CL_Rect &absolute_rect, const CL_Rect &clip_rect);
	void select_items(const CL_Rect &selection_rect);

private:
	int get_all_columns_width() const;
	int get_column_start(int column) const;
	int get_line_start(int item_index) const;
	int get_line_height() const;
	static int clamp(int value, int min_value, int max_value);

	struct CoveredArea
	{
		CoveredArea() : start_line(0), end_line(0), start_column(0), end_column(0) { }
		int start_line;
		int end_line;
		int start_column;
		int end_column;
	};
	CoveredArea get_covered_area(const CL_Rect &rect) const;

	ListviewExDocument *get_document();
	const ListviewExDocument *get_document() const;

	ListviewEx *listview;
	CL_Font font_text;
	CL_FontMetrics metrics;
};
