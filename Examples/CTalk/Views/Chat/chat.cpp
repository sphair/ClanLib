
#include "precomp.h"
#include "chat.h"
#include "chatline.h"

Chat::Chat(CL_GUIComponent *parent)
: CL_GUIComponent(parent), prefix_width(60), column1_width(0), mouse_down(false)
{
	font = CL_Font(get_gc(), get_font_description());
	font_url = CL_Font(get_gc(), get_url_font_description());
	font_fixed = CL_Font(get_gc(), get_fixed_font_description());

	set_type_name("chat");
	part_background = CL_GUIThemePart(this);
	part_text = CL_GUIThemePart(this, "text");

	scroll = new CL_ScrollBar(this);
	scroll->set_vertical();

	func_process_message().set(this, &Chat::on_process_message);
	func_render().set(this, &Chat::on_render);
	func_resized().set(this, &Chat::on_resize);
	scroll->func_scroll().set(this, &Chat::on_scroll);

	on_resize();
}

CL_FontDescription Chat::get_font_description()
{
	CL_FontDescription desc;
	desc.set_typeface_name("Verdana");
	desc.set_height(-13);
	return desc;
}

CL_FontDescription Chat::get_fixed_font_description()
{
	CL_FontDescription desc;
	desc.set_typeface_name("Courier New");
	desc.set_height(-13);
	return desc;
}

CL_FontDescription Chat::get_url_font_description()
{
	CL_FontDescription desc;
	desc.set_typeface_name("Verdana");
	desc.set_height(-13);
	desc.set_underline(true);
	return desc;
}

Chat::~Chat()
{
	std::list<ChatLine *>::iterator it;
	for (it = lines.begin(); it != lines.end(); ++it) delete *it;
}

void Chat::add_line(const CL_SpanLayout &text)
{
	add_line(CL_String(), CL_Colorf::black, text);
}

void Chat::add_line(const CL_String &nick, const CL_Colorf &nick_color, const CL_SpanLayout &text)
{
	ChatLine *line = new ChatLine;
	line->timestamp = create_timestamp();
	line->nick = nick;
	line->nick_color = nick_color;
	line->column3 = text;
	lines.push_back(line);

	if (lines.size() >= 1)
	{
		scroll->set_max(lines.size());
		// if (scroll->get_position() == scroll->get_max()-2)
			scroll->set_position(scroll->get_max()-1);
	}
	request_repaint();
}

CL_String Chat::create_timestamp()
{
	CL_DateTime datetime = CL_DateTime::get_current_local_time();
	CL_String hour = CL_StringHelp::int_to_text(datetime.get_hour());
	if (hour.length() < 2)
		hour = "0"+hour;
	CL_String minute = CL_StringHelp::int_to_text(datetime.get_minutes());
	if (minute.length() < 2)
		minute = "0"+minute;

	return hour+":"+minute;
}

void Chat::on_resize()
{
	CL_Rect content_box = part_background.get_content_box(get_size());
	scroll->set_geometry(CL_Rect(content_box.right-scroll->get_preferred_width(), content_box.top, content_box.right, content_box.bottom));
}

void Chat::on_render(CL_GraphicContext &gc, const CL_Rect &clip_rect)
{
	part_background.render_box(gc, get_size(), clip_rect);

	CL_Rect content_box = part_background.get_content_box(get_size());
	content_box.right -= scroll->get_width();

	part_text.render_box(gc, content_box, clip_rect);
	content_box = part_text.get_content_box(content_box);

	CL_Colorf c(part_text.get_property(CL_GUIThemePartProperty("nick-border-color", "#f2f2f2")));
	CL_Draw::fill(gc, (float)content_box.left, (float)content_box.top, (float)content_box.left+get_prefix_width(), (float)content_box.bottom, c);

	content_box.right -= 5;
	content_box.left += 5;
	content_box.top += 5;
	content_box.bottom -= 5;

	set_cliprect(gc, content_box);

	int y = content_box.bottom;

	int skip_lines = scroll->get_max()-(scroll->get_position()+1);

	std::list<ChatLine *>::reverse_iterator it;
	int line_index = lines.size();
	for (it = lines.rbegin(); it != lines.rend(); ++it, line_index--)
	{
		if (y < 0)
			break;

		if (skip_lines > 0)
		{
			skip_lines--;
			continue;
		}

		ChatLine *line = *it;
		layout_line(gc, line, content_box, line_index-1);

		y -= cl_max(line->column2.get_size().height, line->column3.get_size().height);

		line->column1.set_position(CL_Point(content_box.left, y));
		line->column1.draw_layout(gc);
		line->column2.set_position(CL_Point(content_box.left, y));
		line->column2.draw_layout(gc);
		line->column3.set_position(CL_Point(content_box.left+get_prefix_width(), y));
		line->column3.draw_layout(gc);
	}

	CL_FontMetrics font_metrics = font.get_font_metrics(gc);
	int page_step = cl_max(1, get_height()/static_cast<int>(font_metrics.get_height() + font_metrics.get_external_leading()));
	if (scroll->get_page_step() != page_step)
		scroll->set_ranges(0, lines.empty() ? 1 : lines.size(), 1, page_step);

	reset_cliprect(gc);
}

Chat::TextPosition Chat::hit_test(const CL_Point &pos)
{
	CL_GraphicContext gc = get_gc();
	CL_Rect content_box = part_background.get_content_box(get_size());
	content_box.right -= scroll->get_width();
	content_box = part_text.get_content_box(content_box);
	content_box.left += 5;
	content_box.right -= 5;
	content_box.top += 5;
	content_box.bottom -= 5;

	int y = content_box.bottom;
	int skip_lines = scroll->get_max()-(scroll->get_position()+1);

	std::list<ChatLine *>::reverse_iterator it;
	int line_index = lines.size();
	for (it = lines.rbegin(); it != lines.rend(); ++it, line_index--)
	{
		if (y < 0)
			break;

		if (skip_lines > 0)
		{
			skip_lines--;
			continue;
		}

		ChatLine *line = *it;
		layout_line(gc, line, content_box, line_index);

		int line_height = cl_max(line->column2.get_size().height, line->column3.get_size().height);
		y -= line_height;

		line->column1.set_position(CL_Point(content_box.left, y));
		line->column2.set_position(CL_Point(content_box.left, y));
		line->column3.set_position(CL_Point(content_box.left+get_prefix_width(), y));

		CL_Rect column1_rect(content_box.left, y, content_box.left+column1_width, y+line_height);
		CL_Rect column2_rect(content_box.left+content_box.left, y, content_box.left+prefix_width, y+line_height);
		CL_Rect column3_rect(content_box.left+prefix_width, y, content_box.right, y+line_height);
		if (column1_rect.contains(pos))
			return hit_test_line_column(gc, line_index-1, 0, line->column1, pos);
		else if (column2_rect.contains(pos))
			return hit_test_line_column(gc, line_index-1, 1, line->column2, pos);
		else if (column3_rect.contains(pos))
			return hit_test_line_column(gc, line_index-1, 2, line->column3, pos);
	}

	return TextPosition();
}

Chat::TextPosition Chat::hit_test_line_column(CL_GraphicContext &gc, int line, int column, CL_SpanLayout &span_layout, const CL_Point &pos)
{
	CL_SpanLayout::HitTestResult result = span_layout.hit_test(gc, pos);
	TextPosition text_pos;
	text_pos.line = line;
	text_pos.column = column;
	text_pos.offset = result.offset;
	if(result.type == CL_SpanLayout::HitTestResult::inside)
		text_pos.id = result.object_id;
	return text_pos;
}

void Chat::on_scroll()
{
	request_repaint();
}

void Chat::layout_line(CL_GraphicContext & gc, ChatLine * line, CL_Rect &client_area, int line_index)
{
	if (line->layout_width != client_area.get_width() || line->prefix_width != prefix_width)
	{
		line->column1.clear();
		line->column2.clear();

		CL_Colorf timestamp_color = get_color_timestamp();

		std::pair<int,int> sel_col1 = get_selection_for_line(line_index, 0);
		std::pair<int,int> sel_col2 = get_selection_for_line(line_index, 1);
		std::pair<int,int> sel_col3 = get_selection_for_line(line_index, 2);
		line->column1.set_selection_range(sel_col1.first, sel_col1.second);
		line->column2.set_selection_range(sel_col2.first, sel_col2.second);
		line->column3.set_selection_range(sel_col3.first, sel_col3.second);

		if (!line->timestamp.empty())
		{
			line->column1.add_text(line->timestamp, font_fixed, CL_Colorf::darkgoldenrod);
		}
		if (!line->nick.empty())
		{
			line->column2.add_text(line->nick, font, line->nick_color);
		}

		int t = line->column1.find_preferred_size(gc).width;
		column1_width = cl_max(column1_width, t);
		int column2_width = line->column2.find_preferred_size(gc).width;
		int column12_width = column1_width + column2_width + 25;
		if (column12_width > prefix_width)
		{
			prefix_width = column12_width;
			request_repaint();
		}

		line->column2.set_align(cl_right);
		line->column1.layout(gc, get_prefix_width()-10);
		line->column2.layout(gc, get_prefix_width()-10);
		line->column3.layout(gc, client_area.get_width()-get_prefix_width());
		line->layout_width = client_area.get_width();
		line->prefix_width = prefix_width;
	}
}

int Chat::get_prefix_width() const
{
	return prefix_width;
}

std::pair<int,int> Chat::get_selection_for_line(int line_index, int column)
{
	int sel_start = offset_for_line_column(line_index, column, selection.start);
	int sel_end = offset_for_line_column(line_index, column, selection.end);
	return std::pair<int,int>(sel_start, sel_end);
}

int Chat::offset_for_line_column(int line_index, int column, const TextPosition &pos)
{
	if (pos.line < line_index)
		return 0;
	else if (pos.line > line_index)
		return 1000000;
	else if (pos.column < column)
		return 0;
	else if (pos.column > column)
		return 1000000;
	else
		return pos.offset;
}

void Chat::on_process_message(CL_GUIMessage &msg)
{
	if (msg.is_type(CL_GUIMessage_Input::get_type_name()))
		on_input_message(CL_GUIMessage_Input(msg));
	else if (msg.is_type(CL_GUIMessage_Pointer::get_type_name()))
		on_pointer_message(CL_GUIMessage_Pointer(msg));
}

void Chat::on_input_message(const CL_GUIMessage_Input &msg)
{
	CL_InputEvent input_event = msg.get_event();

	if(input_event.type == CL_InputEvent::pressed && input_event.id == CL_MOUSE_WHEEL_UP)
	{
		scroll->set_position(scroll->get_position() - 3);
		on_scroll();
	}
	else if(input_event.type == CL_InputEvent::pressed && input_event.id == CL_MOUSE_WHEEL_DOWN)
	{
		scroll->set_position(scroll->get_position() + 3);
		on_scroll();
	}
	else if (input_event.type == CL_InputEvent::pressed && input_event.id == CL_MOUSE_LEFT)
	{
		capture_mouse(true);
		mouse_down = true;
		mouse_down_text_position = hit_test(input_event.mouse_pos);
	}
	else if (input_event.type == CL_InputEvent::released && input_event.id == CL_MOUSE_LEFT)
	{
		capture_mouse(false);
		mouse_down = false;
		TextPosition mouse_up_text_position = hit_test(input_event.mouse_pos);

		if(mouse_up_text_position == mouse_down_text_position)
		{
			if(mouse_up_text_position.id != -1)
			{
				cb_url_clicked.invoke(mouse_up_text_position.id);
			}
		}
		else
		{
			set_selection(mouse_down_text_position, mouse_up_text_position);
			copy_to_clipboard();
			set_selection(TextPosition(), TextPosition());
		}
	}
	else if (input_event.type == CL_InputEvent::pointer_moved)
	{
		TextPosition text_position = hit_test(input_event.mouse_pos);

		if (mouse_down)
		{
			set_selection(mouse_down_text_position, text_position);
		}

		if(text_position.id != -1)
		{
			set_cursor(cl_cursor_hand);
		}
		else
		{
			set_cursor(cl_cursor_arrow);
		}
	}
}

void Chat::on_pointer_message(const CL_GUIMessage_Pointer &msg)
{
	/*
	if (msg.get_pointer_type() == CL_GUIMessage_Pointer::pointer_enter)
	{
		set_cursor(cl_cursor_ibeam);
	}
	else
	{
		set_cursor(cl_cursor_arrow);
	}
*/
}

void Chat::set_selection(const TextPosition &start, const TextPosition &end)
{
	if (start != selection.start ||end != selection.end)
	{
		invalidate_lines(selection.start.line, selection.end.line);
		if (start.line < end.line || (start.line == end.line && start.offset <= end.offset))
		{
			selection.start = start;
			selection.end = end;
		}
		else
		{
			selection.start = end;
			selection.end = start;
		}
		invalidate_lines(selection.start.line, selection.end.line);
	}
}

void Chat::invalidate_lines(int start, int end)
{
	std::list<ChatLine*>::iterator it;
	int line = 0;
	for (it = lines.begin(); it != lines.end(); ++it, line++)
	{
		if (line >= start && line <= end)
		{
			(*it)->layout_width = 0;
		}
	}
	request_repaint();
}

void Chat::copy_to_clipboard()
{
	CL_String text;
	std::list<ChatLine*>::iterator it;
	int line = 0;
	for (it = lines.begin(); it != lines.end(); ++it, line++)
	{
		if (line >= selection.start.line && line <= selection.end.line)
		{
			if (!text.empty())
				text += "\r\n";

			append_column_text(line, 0, *it, "[", "] ", text);
			append_column_text(line, 1, *it, "<", "> ", text);
			append_column_text(line, 2, *it, "", "", text);

//			int start_offset = offset_for_line_column(line, 2, selection.start);
//			int end_offset = offset_for_line_column(line, 2, selection.end);
//			text.append((*it)->column3.get_combined_text().substr(start_offset, end_offset-start_offset));
		}
	}
	if (!text.empty())
		get_display_window().set_clipboard_text(text);
}

void Chat::append_column_text(int line, int column, ChatLine * chatline, CL_String prefix, CL_String postfix, CL_String &out_text)
{
	int start_offset = offset_for_line_column(line, column, selection.start);
	int end_offset = offset_for_line_column(line, column, selection.end);
	if (start_offset != end_offset)
	{
		CL_String column_text;
		if (column == 0)
			column_text = chatline->column1.get_combined_text();
		else if (column == 1)
			column_text = chatline->column2.get_combined_text();
		else if (column == 2)
			column_text = chatline->column3.get_combined_text();

		if (!column_text.empty())
		{
			if (start_offset == 0)
				out_text.append(prefix);
			out_text.append(column_text.substr(start_offset, end_offset-start_offset));
			if (end_offset >= (int)column_text.length())
				out_text.append(postfix);
		}
	}
}

CL_Colorf Chat::get_color_text()
{
	return CL_Colorf(part_text.get_property(CL_GUIThemePartProperty("text-color", "#000000"))); // black
}

CL_Colorf Chat::get_color_channel()
{
	return CL_Colorf(part_text.get_property(CL_GUIThemePartProperty("channel-color", "#228b22"))); // forestgreen
}

CL_Colorf Chat::get_color_system()
{
	return CL_Colorf(part_text.get_property(CL_GUIThemePartProperty("system-color", "#5f9ea0"))); // cadetblue
}

CL_Colorf Chat::get_color_nick_others()
{
	return CL_Colorf(part_text.get_property(CL_GUIThemePartProperty("nick-others-color", "#000099")));
}

CL_Colorf Chat::get_color_nick()
{
	return CL_Colorf(part_text.get_property(CL_GUIThemePartProperty("nick-color", "#990000")));
}

CL_Colorf Chat::get_color_timestamp()
{
	return CL_Colorf(part_text.get_property(CL_GUIThemePartProperty("timestamp-color", "#6495ed"))); // darkgoldenrod
}

CL_Colorf Chat::get_color_url()
{
	return CL_Colorf(part_text.get_property(CL_GUIThemePartProperty("url-color", "#000080"))); // navy
}

void Chat::scroll_page_up()
{
	scroll->set_position(scroll->get_position() - scroll->get_page_step());
	on_scroll();
}

void Chat::scroll_page_down()
{
	scroll->set_position(scroll->get_position() + scroll->get_page_step());
	on_scroll();
}
