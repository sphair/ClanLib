
#include "precomp.h"
#include "listview_ex_report_mode.h"
#include "listview_ex.h"

ListviewExReportMode::ListviewExReportMode(ListviewEx *listview)
: listview(listview)
{
	CL_GraphicContext gc = listview->get_gc();
	font_text = CL_Font(gc, /*L"Segoe UI"*/L"Tahoma", 11);//, CL_Colorf((unsigned char)237,(unsigned char)237,(unsigned char)237)/*CL_Colorf::whitesmoke)*/, false, false);
	metrics = font_text.get_font_metrics(gc);
}

int ListviewExReportMode::get_line_step()
{
	return get_line_height();
}

CL_Size ListviewExReportMode::get_absolute_size()
{
	return CL_Size(
		get_all_columns_width(),
		get_document()->get_item_count()*get_line_height());
}

CL_Point ListviewExReportMode::to_absolute_position(const CL_Point &position)
{
	int horz_pos = listview->get_horz_scrollbar_position();
	int vert_pos = listview->get_vert_scrollbar_position();
	return CL_Point(position.x + horz_pos, position.y + vert_pos * get_line_height());
}

CL_Rect ListviewExReportMode::to_absolute_position(const CL_Rect &position)
{
	int horz_pos = listview->get_horz_scrollbar_position();
	int vert_pos = listview->get_vert_scrollbar_position();
	return CL_Rect(
		position.left + horz_pos,
		position.top + vert_pos * get_line_height(),
		position.right + horz_pos,
		position.bottom + vert_pos * get_line_height());
}

CL_Point ListviewExReportMode::to_relative_position(const CL_Point &position)
{
	int horz_pos = listview->get_horz_scrollbar_position();
	int vert_pos = listview->get_vert_scrollbar_position();
	return CL_Point(position.x - horz_pos, position.y - vert_pos * get_line_height());
}

CL_Rect ListviewExReportMode::to_relative_position(const CL_Rect &position)
{
	int horz_pos = listview->get_horz_scrollbar_position();
	int vert_pos = listview->get_vert_scrollbar_position();
	return CL_Rect(
		position.left - horz_pos,
		position.top - vert_pos * get_line_height(),
		position.right - horz_pos,
		position.bottom - vert_pos * get_line_height());
}

int ListviewExReportMode::hit_test(const CL_Point &position)
{
	int item_index = position.y / get_line_height();
	if (item_index >= 0 && item_index < get_document()->get_item_count())
		return item_index;
	else
		return -1;
}

int ListviewExReportMode::insert_mark_hit_test(const CL_Point &position)
{
	int item_index = position.y + (get_line_height()/2) / get_line_height();
	if (item_index >= 0 && item_index < get_document()->get_item_count())
		return item_index;
	else if (item_index < 0)
		return 0;
	else
		return -1;
	return -1;
}

CL_Rect ListviewExReportMode::get_item_line_rect(int item_index)
{
	return CL_Rect(
		CL_Point(0, get_line_start(item_index)),
		CL_Size(get_all_columns_width(), get_line_height()));
}

CL_Rect ListviewExReportMode::get_item_rect(int item_index, int column)
{
	return CL_Rect(
		CL_Point(get_column_start(column), get_line_start(item_index)),
		CL_Size(get_document()->get_column_size(column), get_line_height()));
}

CL_Rect ListviewExReportMode::get_item_icon_rect(int item_index, int column)
{
	int x = get_column_start(column);
	int y = get_line_start(item_index);
	return CL_Rect(CL_Point(x, y), listview->get_icon_size());
}

CL_Rect ListviewExReportMode::get_item_text_rect(int item_index, int column)
{
	int text_offset = listview->get_icon_size().width+2;
	return CL_Rect(
		CL_Point(get_column_start(column)+text_offset, get_line_start(item_index)),
		CL_Size(get_document()->get_column_size(column)-text_offset, get_line_height()));
}

void ListviewExReportMode::draw_area(CL_GraphicContext &gc, const CL_Rect &rect, const CL_Rect &clip_rect)
{
	CoveredArea covered_area = get_covered_area(rect);

	ListviewExDocument *doc = get_document();
	for (int index = covered_area.start_line; index < covered_area.end_line; index++)
	{
		unsigned int flags = doc->get_item_flags(index);
		int icon = doc->get_item_icon(index);
		for (int column = covered_area.start_column; column < covered_area.end_column; column++)
		{
			CL_Rect column_area = to_relative_position(get_item_rect(index, column));
			CL_Rect icon_area = to_relative_position(get_item_icon_rect(index, column));
			CL_Rect text_area = to_relative_position(get_item_text_rect(index, column));

			listview->set_cliprect(gc, column_area);

			bool default_draw = true;
			if (!listview->cb_custom_draw_item.is_null())
				default_draw = listview->cb_custom_draw_item.invoke(gc, index, column, column_area);

			if (default_draw)
			{
				if (column == 0 && icon != -1)
					doc->get_icon(icon).draw(gc, icon_area);
//					gc.draw_image_blend(icon_area, doc->get_icon(icon));

				CL_StringRef text = doc->get_item_text(index, column);
				font_text.draw_text(gc, text_area.left, text_area.top+(int)metrics.get_ascent(), text, CL_Colorf::black);
			}

			listview->reset_cliprect(gc);
		}
	}
}

void ListviewExReportMode::select_items(const CL_Rect &selection_rect)
{
}

ListviewExReportMode::CoveredArea ListviewExReportMode::get_covered_area(const CL_Rect &rect) const
{
	int line_height = get_line_height();
	CoveredArea covered_area;
	covered_area.start_line = clamp(rect.top / line_height, 0, get_document()->get_item_count());
	covered_area.end_line = clamp((rect.bottom + line_height-1) / line_height, 0, get_document()->get_item_count());
	covered_area.start_column = 0;
	covered_area.end_column = get_document()->get_column_count();
	return covered_area;
}

int ListviewExReportMode::clamp(int value, int min_value, int max_value)
{
	if (value < min_value)
		value = min_value;
	if (value > max_value)
		value = max_value;
	return value;
}

int ListviewExReportMode::get_column_start(int column) const
{
	const ListviewExDocument *doc = get_document();
	int num_columns = doc->get_column_count();
	int x = 0;
	for (int index = 0; index < column; index++)
		x += doc->get_column_size(index);
	return x;
}

int ListviewExReportMode::get_line_start(int item_index) const
{
	return get_line_height()*item_index;
}

int ListviewExReportMode::get_line_height() const
{
	return (int)metrics.get_height()+(int)metrics.get_external_leading();
}

int ListviewExReportMode::get_all_columns_width() const
{
	const ListviewExDocument *doc = get_document();
	int total_size = 0;
	for (int index = 0; index < doc->get_column_count(); index++)
		total_size += doc->get_column_size(index);
	return total_size;
}

const ListviewExDocument *ListviewExReportMode::get_document() const
{
	return listview->get_document();
}

ListviewExDocument *ListviewExReportMode::get_document()
{
	return listview->get_document();
}
