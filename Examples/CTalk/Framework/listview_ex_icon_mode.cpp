
#include "precomp.h"
#include "listview_ex_icon_mode.h"
#include "listview_ex.h"
#include "listview_ex_document.h"

ListviewExIconMode::ListviewExIconMode(ListviewEx *listview)
: listview(listview)
{
	part_icon_frame = CL_GUIThemePart(listview, "icon-frame");//Application::get_image_resource(L"pictureframe.png");
	part_album_icon_frame = CL_GUIThemePart(listview, "album-icon-frame");//Application::get_image_resource(L"album-frame.png");
	part_item_hot = CL_GUIThemePart(listview, "item"); //Application::get_image_resource(L"listview-item-hot.png");
	part_item_hot.set_state("hot", true);
	part_item_selected = CL_GUIThemePart(listview, "item");
	part_item_selected.set_state("selected", true);
	part_item_drophighlighted = CL_GUIThemePart(listview, "item");
	part_item_drophighlighted.set_state("drophighlighted", true);

	CL_GraphicContext gc = listview->get_gc();
	font_text = CL_Font(gc, /*L"Segoe UI"*/L"Tahoma", 11);//, CL_Colorf((unsigned char)237,(unsigned char)237,(unsigned char)237)/*CL_Colorf::whitesmoke)*/, false, false);
}

int ListviewExIconMode::get_line_step()
{
	CL_Size item_size = get_item_size();
	return item_size.height;
}

CL_Size ListviewExIconMode::get_absolute_size()
{
	CL_Size item_size = get_item_size();
	return CL_Size(item_size.width * get_items_per_line(), item_size.height * get_num_lines());
}

CL_Point ListviewExIconMode::to_absolute_position(const CL_Point &position)
{
	int horz_pos = listview->get_horz_scrollbar_position();
	int vert_pos = listview->get_vert_scrollbar_position();
	return CL_Point(position.x + horz_pos, position.y + vert_pos);
}

CL_Rect ListviewExIconMode::to_absolute_position(const CL_Rect &position)
{
	int horz_pos = listview->get_horz_scrollbar_position();
	int vert_pos = listview->get_vert_scrollbar_position();
	return CL_Rect(
		position.left + horz_pos,
		position.top + vert_pos,
		position.right + horz_pos,
		position.bottom + vert_pos);
}

CL_Point ListviewExIconMode::to_relative_position(const CL_Point &position)
{
	int horz_pos = listview->get_horz_scrollbar_position();
	int vert_pos = listview->get_vert_scrollbar_position();
	return CL_Point(position.x - horz_pos, position.y - vert_pos);
}

CL_Rect ListviewExIconMode::to_relative_position(const CL_Rect &position)
{
	int horz_pos = listview->get_horz_scrollbar_position();
	int vert_pos = listview->get_vert_scrollbar_position();
	return CL_Rect(
		position.left - horz_pos,
		position.top - vert_pos,
		position.right - horz_pos,
		position.bottom - vert_pos);
}

int ListviewExIconMode::hit_test(const CL_Point &position)
{
	CoveredArea covered_area = get_covered_area(CL_Rect(position, CL_Size(1,1)));
	for (int index = covered_area.start_index; index < covered_area.end_index; index++)
		if (get_item_icon_rect(index).contains(position) || get_item_text_rect(index).contains(position))
			return index;
	return -1;
}

int ListviewExIconMode::insert_mark_hit_test(const CL_Point &position)
{
	return -1;
}

CL_Rect ListviewExIconMode::get_item_rect(int item_index, int)
{
	CL_Size item_size = get_item_size();
	int line = get_item_line(item_index);
	int column = get_item_column(item_index);
	return CL_Rect(CL_Point(column*item_size.width, line*item_size.height), item_size);
}

CL_Rect ListviewExIconMode::get_item_icon_rect(int item_index, int column)
{
	CL_Size icon_size = listview->get_icon_size();
	if (!(get_document()->get_item_flags(item_index) & ListviewExDocument::album_flag))
	{
		int icon_index = get_document()->get_item_icon(item_index);
		if (icon_index != -1)
		{
			// Scale according to aspect ratio
			CL_Size real_icon_size = get_document()->get_icon(icon_index).get_size();
			CL_Size scaled_icon_vert(real_icon_size.width*icon_size.height/real_icon_size.height, icon_size.height);
			CL_Size scaled_icon_horz(icon_size.width, real_icon_size.height*icon_size.width/real_icon_size.width);
			if (scaled_icon_vert.width > icon_size.width)
				icon_size = scaled_icon_horz;
			else
				icon_size = scaled_icon_vert;
		}
	}

	icon_size.width += 6;  // icon frame is 6 pixels
	icon_size.height += 6;
	CL_Rect item_rect = get_item_rect(item_index, column);
	item_rect.bottom -= text_height;
	CL_Point pos(item_rect.left + item_rect.get_width()/2 - icon_size.width/2, item_rect.top + item_rect.get_height()/2 - icon_size.height/2);
	return CL_Rect(pos, icon_size);
}

CL_Rect ListviewExIconMode::get_item_text_rect(int item_index, int column)
{
	CL_Rect rect = get_item_rect(item_index, column);
	rect.top = rect.bottom - text_height;
	return rect;
}

void ListviewExIconMode::draw_area(CL_GraphicContext &gc, const CL_Rect &rect, const CL_Rect &clip_rect)
{
	CoveredArea covered_area = get_covered_area(rect);
	ListviewExDocument *doc = get_document();
	CL_Rect selection_rect = listview->get_selection_rect();

	for (int index = covered_area.start_index; index < covered_area.end_index; index++)
	{
		unsigned int flags = doc->get_item_flags(index);
		int icon = doc->get_item_icon(index);

		CL_Rect column_area = to_relative_position(get_item_rect(index, 0));
		CL_Rect icon_area = to_relative_position(get_item_icon_rect(index, 0));
		CL_Rect text_area = to_relative_position(get_item_text_rect(index, 0));

		listview->set_cliprect(gc, column_area);

		bool selected = selection_rect.is_overlapped(column_area);
		if (flags & ListviewExDocument::selected_flag)
			selected = true;

		if (selected)
		{
			part_item_selected.render_box(gc, column_area, clip_rect);
		}
		if (flags & ListviewExDocument::drophighlighted_flag)
		{
			part_item_drophighlighted.render_box(gc, column_area, clip_rect);
		}
/*		if (index == hot_item)
		{
			part_item_hot.render_box(gc, column_area, clip_rect);
		}*/

		CL_Rect frame_area = icon_area;
		if (flags & ListviewExDocument::album_flag)
		{
			CL_Size icon_size(frame_area.get_width()-30, frame_area.get_height()-40);
			if (icon != -1)
			{
				// Scale according to aspect ratio
				CL_Size real_icon_size = get_document()->get_icon(icon).get_size();
				CL_Size scaled_icon_vert(real_icon_size.width*icon_size.height/real_icon_size.height, icon_size.height);
				CL_Size scaled_icon_horz(icon_size.width, real_icon_size.height*icon_size.width/real_icon_size.width);
				if (scaled_icon_vert.width > icon_size.width)
					icon_size = scaled_icon_horz;
				else
					icon_size = scaled_icon_vert;
			}

			icon_area.left += 10;
			icon_area.bottom -= 10;
			icon_area.right = icon_area.left + icon_size.width;
			icon_area.top = icon_area.bottom - icon_size.height;
			part_album_icon_frame.render_box(gc, frame_area, clip_rect);
			//gc.draw_part_blend(frame_area, image_album_icon_frame, 16, 18, 87, 6);

			CL_Rect inner_frame_area = icon_area;
			inner_frame_area.expand(4, 4, 4, 4);
			part_icon_frame.render_box(gc, inner_frame_area, clip_rect);
			//gc.draw_part_blend(inner_frame_area, image_icon_frame, 8, 8, 8, 8);
		}
		else
		{
			icon_area.expand(-4, -4, -4, -4);
			part_icon_frame.render_box(gc, frame_area, clip_rect);
			//gc.draw_part_blend(frame_area, image_icon_frame, 8, 8, 8, 8);
		}

		if (icon != -1)
			doc->get_icon(icon).draw(gc, icon_area);
			//gc.draw_image_blend(icon_area, doc->get_icon(icon));

		CL_StringRef text = doc->get_item_text(index, 0);
		CL_Size text_size = font_text.get_text_size(gc, text);
		font_text.draw_text(gc, text_area.get_center().x-text_size.width/2, text_area.bottom-5, text);

		listview->reset_cliprect(gc);
	}
}

void ListviewExIconMode::select_items(const CL_Rect &selection_rect)
{
	CoveredArea covered_area = get_covered_area(selection_rect);
	ListviewExDocument *doc = get_document();
	for (int index = covered_area.start_index; index < covered_area.end_index; index++)
	{
		CL_Rect column_area = get_item_rect(index, 0);
		bool selected = selection_rect.is_overlapped(column_area);
		if (selected)
			doc->select_item(index);
	}
}

ListviewExIconMode::CoveredArea ListviewExIconMode::get_covered_area(const CL_Rect &rect) const
{
	const ListviewExDocument *doc = get_document();
	CL_Size item_size = get_item_size();
	int num_lines = get_num_lines();
	int items_per_line = get_items_per_line();

	CoveredArea covered_area;
	covered_area.start_line = clamp(rect.top / item_size.height, 0, num_lines);
	covered_area.end_line = clamp((rect.bottom + item_size.height-1) / item_size.height, 0, num_lines);
	covered_area.start_index = clamp(covered_area.start_line * items_per_line, 0, doc->get_item_count());
	covered_area.end_index = clamp(covered_area.end_line * items_per_line, 0, doc->get_item_count());
	return covered_area;
}

ListviewExDocument *ListviewExIconMode::get_document()
{
	return listview->get_document();
}

const ListviewExDocument *ListviewExIconMode::get_document() const
{
	return listview->get_document();
}

int ListviewExIconMode::get_item_line(int item_index) const
{
	return item_index / get_items_per_line();
}

int ListviewExIconMode::get_item_column(int item_index) const
{
	return item_index % get_items_per_line();
}

CL_Size ListviewExIconMode::get_item_size() const
{
	CL_Size icon_size = listview->get_icon_size();
	int item_width = max(max_text_width, icon_size.width) + 16;
	int item_height = icon_size.height + text_height + 16;
	return CL_Size(item_width, item_height);
}

int ListviewExIconMode::get_items_per_line() const
{
	CL_Size item_size = get_item_size();
	CL_Size client_size = listview->get_size();
	int items_per_line = client_size.width / item_size.width;
	if (items_per_line <= 0)
		items_per_line = 1;
	return items_per_line;
}

int ListviewExIconMode::get_num_lines() const
{
	int items_per_line = get_items_per_line();
	return (get_document()->get_item_count() + items_per_line-1) / items_per_line;
}

int ListviewExIconMode::clamp(int value, int min_value, int max_value)
{
	if (value < min_value)
		value = min_value;
	if (value > max_value)
		value = max_value;
	return value;
}
