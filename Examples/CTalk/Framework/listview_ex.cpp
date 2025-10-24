
#include "precomp.h"
#include "listview_ex.h"
#include "listview_ex_mode.h"
#include "listview_ex_report_mode.h"
#include "listview_ex_icon_mode.h"

ListviewEx::ListviewEx(CL_GUIComponent *parent)
: CL_GUIComponent(parent), icon_size(16, 16), mousemove_mode(mousemove_none)
{
	func_resized().set(this, &ListviewEx::on_size);
	func_render().set(this, &ListviewEx::on_render);
	func_input_pressed().set(this, &ListviewEx::on_input_pressed);
	func_input_released().set(this, &ListviewEx::on_input_released);
	func_input_doubleclick().set(this, &ListviewEx::on_input_dblclk);
	func_input_pointer_moved().set(this, &ListviewEx::on_input_pointer_moved);
	func_pointer_enter().set(this, &ListviewEx::on_pointer_enter);
	func_pointer_exit().set(this, &ListviewEx::on_pointer_exit);

	part_background = CL_GUIThemePart(this); // Application::get_image_resource(L"clientbggradient.png");
	part_mouse_selection_rect = CL_GUIThemePart(this, "selection"); // Application::get_image_resource(L"selectionrect.png");

	vert_scrollbar.reset(new CL_ScrollBar(this));
	horz_scrollbar.reset(new CL_ScrollBar(this));
	vert_scrollbar->set_vertical();
	horz_scrollbar->set_horizontal();
	vert_scrollbar->func_scroll().set(this, &ListviewEx::on_vert_scroll_position_changed);
	horz_scrollbar->func_scroll().set(this, &ListviewEx::on_horz_scroll_position_changed);
	vert_scrollbar->set_visible(false);
	horz_scrollbar->set_visible(false);

	set_report_view();
}

ListviewEx::~ListviewEx()
{
}

void ListviewEx::on_size()
{
	update_scrollbars();
}

void ListviewEx::update_scrollbars()
{
	CL_Size client_size = get_size();
	vert_scrollbar->set_geometry(CL_Rect(CL_Point(client_size.width-scrollbar_width, 0), CL_Size(scrollbar_width, client_size.height)));
	horz_scrollbar->set_geometry(CL_Rect(CL_Point(0, client_size.height-scrollbar_height), CL_Size(client_size.width, scrollbar_height)));

	CL_Size document_size = mode->get_absolute_size();
	int line_step = mode->get_line_step();
	horz_scrollbar->set_visible(document_size.width > client_size.width-scrollbar_width);
	vert_scrollbar->set_visible(document_size.height > client_size.height-scrollbar_height);
	horz_scrollbar->set_ranges(0, document_size.width, client_size.width, line_step);
	vert_scrollbar->set_ranges(0, document_size.height, (client_size.height/line_step)*line_step, line_step);
}
/*
CL_Rect ListviewEx::get_item_area() const
{
	CL_Size client_size = get_client_size();
	CL_Rect rect;
	if (view_mode == report_view_mode)
	{
		rect.bottom = items.size() * max(icon_size.height, text_height);
	}
	else
	{
		int item_width = max(icon_size.width, max_text_width) + 20;
		int icons_per_line = (client_size.width-scrollbar_width) / item_width;
		if (icons_per_line == 0)
			icons_per_line = 1;
		int line_height = icon_size.height+text_height + 20;
		int num_lines = (items.size()+icons_per_line-1) / icons_per_line;
		rect.right = item_width * icons_per_line;
		rect.bottom = line_height * num_lines;
	}
	return rect;
}
*/
void ListviewEx::set_report_view()
{
	mode.reset(new ListviewExReportMode(this));
	update_scrollbars();
}

void ListviewEx::set_icon_view()
{
	mode.reset(new ListviewExIconMode(this));
	update_scrollbars();
}

void ListviewEx::insert_column(int index, const CL_StringRef &text, int size)
{
	document.insert_column(index, text, size);
	update_scrollbars();
}

CL_Size ListviewEx::get_icon_size() const
{
	return icon_size;
}

void ListviewEx::set_icon_size(const CL_Size &size)
{
	icon_size = size;
	update_scrollbars();
	request_repaint();
}

int ListviewEx::add_icon(const CL_Image &pixelbuffer)
{
	return document.add_icon(pixelbuffer);
}

void ListviewEx::remove_all_icons()
{
	document.remove_all_icons();
}

ListviewExItem *ListviewEx::get_item(int index)
{
	return document.get_item(index);
}

int ListviewEx::find_item(ListviewExItem *item)
{
	return document.find_item(item);
}

int ListviewEx::insert_item(int item_index, ListviewExItem *item, const CL_StringRef &text, int group_id)
{
	int new_item_index = document.insert_item(item_index, item, text, group_id);
	update_scrollbars();
	request_repaint();
	return new_item_index;
}

int ListviewEx::append_item(ListviewExItem *item, const CL_StringRef &text, int group_id)
{
	int new_item_index = document.append_item(item, text, group_id);
	update_scrollbars();
	request_repaint();
	return new_item_index;
}

void ListviewEx::set_item_text(int item_index, int column, const CL_StringRef &text)
{
	document.set_item_text(item_index, column, text);
	request_repaint();
}

void ListviewEx::set_item_icon(int item_index, int icon_index)
{
	document.set_item_icon(item_index, icon_index);
	request_repaint();
}

void ListviewEx::set_item_album(int item_index, bool is_album)
{
	document.set_item_album(item_index, is_album);
	request_repaint();
}

void ListviewEx::set_item_drop_highlighted(int item_index, bool enabled)
{
	document.set_item_drop_highlighted(item_index, enabled);
	request_repaint();
}

void ListviewEx::remove_all_items()
{
	document.remove_all_items();
	update_scrollbars();
	request_repaint();
}

void ListviewEx::remove_item(int item_index)
{
	document.remove_item(item_index);
	update_scrollbars();
	request_repaint();
}

int ListviewEx::get_next_selected_item(int last_index) const
{
	return document.get_next_selected_item(last_index);
}

int ListviewEx::get_prev_selected_item(int last_index) const
{
	return document.get_prev_selected_item(last_index);
}

int ListviewEx::get_selected_count() const
{
	return document.get_selected_count();
}

void ListviewEx::select_item(int index)
{
	document.select_item(index);
	request_repaint();
}

void ListviewEx::select_item_range(int first_index, int last_index)
{
	document.select_item_range(first_index, last_index);
	request_repaint();
}

void ListviewEx::clear_selection()
{
	document.clear_selection();
	request_repaint();
}

int ListviewEx::hit_test(const CL_Point &position)
{
	CL_Point absolute_position = mode->to_absolute_position(position);
	return mode->hit_test(absolute_position);
}

void ListviewEx::set_insert_mark(int item_index)
{
	document.set_insert_mark(item_index);
	request_repaint();
}

int ListviewEx::insert_mark_hit_test(const CL_Point &position)
{
	CL_Point absolute_position = mode->to_absolute_position(position);
	return mode->insert_mark_hit_test(absolute_position);
}

void ListviewEx::ensure_visible(int item_index, bool partial_ok)
{
}

void ListviewEx::begin_label_edit(int item_index)
{
}

void ListviewEx::on_horz_scroll_position_changed()
{
	request_repaint();
}

void ListviewEx::on_vert_scroll_position_changed()
{
	request_repaint();
}

void ListviewEx::on_render(CL_GraphicContext &gc, const CL_Rect &clip_rect)
{
	CL_Size client_size = get_size();
	// gc.clear(CL_Colorf((unsigned char)0x25,(unsigned char)0x25,(unsigned char)0x25));
	//gc.draw_part_blend(CL_Rect(0, 0, client_size.width, image_background.get_height()), image_background, 0, 0, 0, 0);
	part_background.render_box(gc, client_size, clip_rect);

	CL_Rect absolute_rect = mode->to_absolute_position(CL_Rect(0, 0, client_size.width, client_size.height));
	mode->draw_area(gc, absolute_rect, clip_rect);
/*
	if (view_mode == report_view_mode)
	{
		int header_height = 24;
		for (int x=20, column_index=0; x<client_size.width && column_index < columns.size(); column_index++)
		{
			canvas.set_clip_rect(CL_Rect(CL_Point(x, 0), CL_Size(columns[column_index].size, header_height)));
			font_header->draw_text(canvas, x, header_height-5, columns[column_index].text);
			canvas.reset_clip_rect();
			x += columns[column_index].size;
		}

		int all_columns_width = 0;
		for (int column_index=0; column_index < columns.size(); column_index++)
			all_columns_width += columns[column_index].size;

		int line_height = max(icon_size.height, text_height);
		for (int y=header_height, item_index=vert_scrollbar->get_position(); y+line_height<client_size.height && item_index < items.size(); y+=line_height, item_index++)
		{
			CL_Rect item_rect(20-5, y+1, 20+all_columns_width+5, y+line_height);

			if (item_flags[item_index] & selected_flag)
				canvas.draw_part_blend(item_rect, image_item_selected, 0, 0, 0, 0);
			if (item_flags[item_index] & drophighlighted_flag)
				canvas.draw_part_blend(item_rect, image_item_drophighlighted, 0, 0, 0, 0);
			if (item_index == hot_item && mousemove_mode != mousemove_net_selection)
				canvas.draw_part_blend(item_rect, image_item_hot, 0, 0, 0, 0);

			int start_x = 20;
			if (items[item_index].icon_index != -1)
			{
				canvas.draw_image_blend(CL_Rect(CL_Point(start_x, y+line_height/2-icon_size.height/2), icon_size), icons[items[item_index].icon_index]);
				start_x += icon_size.width + 4;
			}

			for (int x=start_x, column_index=0; x<client_size.width && column_index < columns.size(); column_index++)
			{
				bool default_draw = true;
				CL_Rect column_rect(CL_Point(x,y),CL_Size(columns[column_index].size, line_height));
				canvas.set_clip_rect(column_rect);
				if (!cb_custom_draw_item.is_null())
					default_draw = cb_custom_draw_item.invoke(canvas, item_index, column_index, column_rect);
				if (default_draw)
					font_normal->draw_text(canvas, x, y+line_height-7, columns[column_index].item_texts[item_index]);
				canvas.reset_clip_rect();
				x += columns[column_index].size;
			}
		}
	}
	else
	{
		int item_width = max(icon_size.width, max_text_width) + 20;
		int icons_per_line = (client_size.width-scrollbar_width) / item_width;
		if (icons_per_line == 0)
			icons_per_line = 1;
		int line_height = icon_size.height+text_height + 20;
		int num_lines = (items.size()+icons_per_line-1) / icons_per_line;

		int line = vert_scrollbar->get_position()/line_height;
		for (int y=line*line_height-vert_scrollbar->get_position(); y<client_size.height && line < num_lines; y+=line_height)
		{
			int start_x = 10;
			for (int col = 0; col < icons_per_line; col++)
			{
				int item_index = icons_per_line*line+col;
				if (item_index >= items.size())
					break;

				CL_Rect item_rect(CL_Point(start_x+col*item_width, y), CL_Size(item_width, line_height));
				item_rect.left += 4;
				item_rect.top += 4;
				item_rect.right -= 4;
				item_rect.bottom -= 4;

				canvas.set_clip_rect(item_rect);

				if (item_flags[item_index] & selected_flag)
					canvas.draw_part_blend(item_rect, image_item_selected, 0, 0, 0, 0);
				if (item_flags[item_index] & drophighlighted_flag)
					canvas.draw_part_blend(item_rect, image_item_drophighlighted, 0, 0, 0, 0);
				if (item_index == hot_item && mousemove_mode != mousemove_net_selection)
					canvas.draw_part_blend(item_rect, image_item_hot, 0, 0, 0, 0);

				CL_Size text_size = font_normal->get_text_size(columns[0].item_texts[item_index]);

				if (items[item_index].icon_index != -1)
				{
					CL_PixelBuffer &icon = icons[items[item_index].icon_index];
					CL_Size image_size = icon.get_size();

					int aspect_width = icon_size.height*image_size.width/image_size.height;
					int aspect_height = icon_size.width*image_size.height/image_size.width;
					if (aspect_width <= icon_size.width)
						aspect_height = icon_size.height;
					else
						aspect_width = icon_size.width;
					CL_Size aspect_size(aspect_width, aspect_height);
					CL_Rect rect_frame(CL_Point(start_x+col*item_width + 10+icon_size.width/2-aspect_size.width/2, y+10+icon_size.height/2-aspect_size.height/2), aspect_size);
					CL_Rect rect_icon = rect_frame;
					if (items[item_index].album)
					{
						rect_icon.left += 4;
						rect_icon.top += 18;
						rect_icon.right -= 7;
						rect_icon.bottom -= 3;

						rect_frame = item_rect;
						rect_frame.expand(CL_Rect(-2, -2, -2, -2));
						rect_frame.bottom -= 7+10+text_size.height;

						CL_Size new_size(rect_icon.get_width()*80/100, rect_icon.get_height()*80/100);
						rect_icon.left = rect_frame.left+8;
						rect_icon.bottom = rect_frame.bottom-8;
						rect_icon.right = rect_icon.left+new_size.width;
						rect_icon.top = rect_icon.bottom-new_size.height;

						canvas.draw_part(rect_frame, image_album_icon_frame, 16, 18, 57, 6);
						canvas.draw_image(rect_icon, icons[items[item_index].icon_index]);
					}
					else
					{
						rect_icon.left += 1;
						rect_icon.top += 1;
						rect_icon.right -= 5;
						rect_icon.bottom -= 5;

						canvas.draw_part(rect_frame, image_icon_frame, 1, 1, 5, 5);
						canvas.draw_image(rect_icon, icons[items[item_index].icon_index]);
					}
				}
				font_normal->draw_text(canvas, start_x+col*item_width+item_width/2-text_size.width/2, y+line_height-7-10, columns[0].item_texts[item_index]);

				canvas.reset_clip_rect();
			}

			line++;
		}
	}
*/
	if (mouse_selection_rect.get_size() != CL_Size(0,0))
		part_mouse_selection_rect.render_box(gc, mouse_selection_rect, clip_rect);
		//gc.draw_part_blend(mouse_selection_rect, image_mouse_selection_rect, 1, 1, 1, 1);
}

bool ListviewEx::on_input_dblclk(const CL_InputEvent &e)
{
	int cursor_item = hit_test(e.mouse_pos);
	int cursor_col = 0;
	if (!cb_dblclk.is_null())
		cb_dblclk.invoke(cursor_item, cursor_col, e.mouse_pos);


/*	CL_Point mouse_pos(mouse_x, mouse_y);
	if (view_mode == report_view_mode)
	{
		int cursor_item = hit_test(CL_Point(mouse_x, mouse_y));
		if (cursor_item != -1)
		{
			CL_Size client_size = get_client_size();
			int cursor_col = 0;
			int start_x = 20;
			int header_height = 24;
			int line_height = max(icon_size.height, text_height);
			int y = line_height * cursor_item + header_height;
			for (int x=start_x, column_index=0; x<client_size.width && column_index < columns.size(); column_index++)
			{
				CL_Rect column_rect(CL_Point(x,y),CL_Size(columns[column_index].size, line_height));
				if (column_rect.contains(mouse_pos))
				{
					cursor_col = column_index;
					break;
				}
				x += columns[column_index].size;
			}

			if (!cb_dblclk.is_null())
				cb_dblclk.invoke(cursor_item, cursor_col, mouse_pos);
		}
	}
	else
	{
		int cursor_item = hit_test(CL_Point(mouse_x, mouse_y));
		int cursor_col = 0;
		if (!cb_dblclk.is_null())
			cb_dblclk.invoke(cursor_item, cursor_col, mouse_pos);
	}*/
	return true;
}

bool ListviewEx::on_input_pressed(const CL_InputEvent &e)
{
	if (e.id == CL_MOUSE_LEFT || e.id == CL_MOUSE_RIGHT)
	{
		mouse_down_pos = e.mouse_pos;
		last_mouse_move_pos = mouse_down_pos;

		int cursor_item = hit_test(e.mouse_pos);
		if (cursor_item != -1)
		{
/*			if (focus_item != cursor_item)
			{
				if (focus_item != -1 && !cb_item_focus_lost.is_null())
					cb_item_focus_lost.invoke(focus_item);
				focus_item = cursor_item;
				if (focus_item != -1 && !cb_item_focus_gained.is_null())
					cb_item_focus_gained.invoke(focus_item);
			}
*/
			if ((document.get_item_flags(cursor_item) & ListviewExDocument::selected_flag) == 0)
			{
				clear_selection();
				select_item(cursor_item);
			}
			mousemove_mode = mousemove_item_clicked;
		}
		else
		{
			clear_selection();
			mousemove_mode = mousemove_net_selection;
		}

		capture_mouse(true);
		return true;
	}
	else if (e.id == CL_MOUSE_WHEEL_UP)
	{
		vert_scrollbar->set_position(vert_scrollbar->get_position() - vert_scrollbar->get_line_step());
		request_repaint();
		return true;
	}
	else if (e.id == CL_MOUSE_WHEEL_DOWN)
	{
		vert_scrollbar->set_position(vert_scrollbar->get_position() + vert_scrollbar->get_line_step());
		request_repaint();
		return true;
	}
	else
	{
		return false;
	}
}

bool ListviewEx::on_input_released(const CL_InputEvent &e)
{
	if (e.id == CL_MOUSE_LEFT || e.id == CL_MOUSE_RIGHT)
	{
		if (mousemove_mode == mousemove_item_clicked)
		{
			// Do nothing. We already selected the item in on_button_down
		}
		else if (mousemove_mode == mousemove_net_selection)
		{
			clear_selection();
			mode->select_items(mode->to_absolute_position(mouse_selection_rect));
		}

		mousemove_mode = mousemove_none;
		mouse_selection_rect = CL_Rect();
		request_repaint();

		capture_mouse(false);

		if (e.id == CL_MOUSE_RIGHT)
		{
			int cursor_item = hit_test(e.mouse_pos);
			int cursor_col = 0;
			if (!cb_rclick.is_null())
				cb_rclick.invoke(cursor_item, cursor_col, e.mouse_pos);
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool ListviewEx::on_input_pointer_moved(const CL_InputEvent &e)
{
	int cursor_item = hit_test(e.mouse_pos);
/*	if (hot_item != cursor_item)
	{
		hot_item = cursor_item;
		request_repaint();
	}
*/
	if (mousemove_mode == mousemove_item_clicked)
	{
		// change mousemove_mode to mousemove_item_drag if moved a few pixels away from mouse_down_pos
	}
	else if (mousemove_mode == mousemove_net_selection)
	{
		mouse_selection_rect.left = min(e.mouse_pos.x, mouse_down_pos.x);
		mouse_selection_rect.top = min(e.mouse_pos.y, mouse_down_pos.y);
		mouse_selection_rect.right = max(e.mouse_pos.x, mouse_down_pos.x);
		mouse_selection_rect.bottom = max(e.mouse_pos.y, mouse_down_pos.y);
		request_repaint();


/*
		if (view_mode == report_view_mode)
		{
			int header_height = 24;
			int all_columns_width = 0;
			for (int column_index=0; column_index < columns.size(); column_index++)
				all_columns_width += columns[column_index].size;
			int line_height = max(icon_size.height, text_height);
			int all_lines_height = items.size() * line_height + header_height;

			if (mouse_selection_rect.left > 25+all_columns_width || mouse_selection_rect.right < 15 ||
				mouse_selection_rect.bottom < header_height || mouse_selection_rect.top >= all_lines_height)
			{
				clear_selection();
			}
			else
			{
				int cursor_item_start = (mouse_selection_rect.top-header_height) / line_height;
				int cursor_item_end = (mouse_selection_rect.bottom-header_height) / line_height;
				if (cursor_item_start < 0)
					cursor_item_start = 0;
				if (cursor_item_end >= items.size())
					cursor_item_end = items.size()-1;
				clear_selection();
				select_item_range(cursor_item_start, cursor_item_end);
			}
		}
*/
	}

	last_mouse_move_pos = e.mouse_pos;
	return true;
}

bool ListviewEx::on_pointer_enter()
{
	set_cursor(cl_cursor_arrow);
	return true;
}

bool ListviewEx::on_pointer_exit()
{
/*	if (hot_item != -1)
	{
		hot_item = -1;
		request_repaint();
	}*/
	return true;
}

/*
void ListviewEx::find_line_and_column(const CL_Point &position, int &line, int &col)
{
	CL_Point position_absolute = position;
	position_absolute.x += horz_scrollbar->get_position();
	position_absolute.y += vert_scrollbar->get_position();

	CL_Size client_size = get_client_size();
	int item_width = max(icon_size.width, max_text_width) + 20;
	int icons_per_line = (client_size.width-scrollbar_width) / item_width;
	if (icons_per_line == 0)
		icons_per_line = 1;
	int line_height = icon_size.height+text_height + 20;
	int num_lines = (items.size()+icons_per_line-1) / icons_per_line;

	int start_x = 10;

	line = position_absolute.y/line_height;
	col = (position_absolute.x-start_x)/item_width;
}
*/