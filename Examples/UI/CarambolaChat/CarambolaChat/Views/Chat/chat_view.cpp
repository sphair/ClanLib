
#include "precomp.h"
#include "chat_view.h"
#include "chat_line.h"
#include <algorithm>

using namespace clan;

ChatView::ChatView()
{
	box_style.set_layout_hbox();

	auto text_view = std::make_shared<ChatTextView>(this);
	text_view->box_style.set_flex(1.0f, 1.0f);

	scroll = std::make_shared<ScrollBarView>();
	scroll->set_vertical();
	scroll->box_style.set_flex(0.0f, 0.0f);
	scroll->box_style.set_background(Colorf::orangered);
	scroll->button_increment()->box_style.set_background(Colorf::gray70);
	scroll->button_decrement()->box_style.set_background(Colorf::gray70);
	scroll->track()->box_style.set_background(Colorf::gray90);
	scroll->thumb()->box_style.set_background(Colorf::gray80);

	add_subview(text_view);
	add_subview(scroll);

	slots.connect(scroll->sig_scroll(), this, &ChatView::on_scroll);
}

ChatView::~ChatView()
{
}

Font ChatView::get_font_description()
{
	return Font("Verdana", 13.0f);
}

Font ChatView::get_fixed_font_description()
{
	return Font("Consolas", 13.0f);
}

Font ChatView::get_url_font_description()
{
	return Font("Verdana", 13.0f);
}

void ChatView::add_line(ChatLine line)
{
	line.timestamp = create_timestamp();
	lines.push_back(line);

	if (lines.size() >= 1)
	{
		scroll->set_max_position(lines.size());
		if (scroll->position() == scroll->max_position() - 2.0)
			scroll->set_position(scroll->max_position() - 1.0);
	}
	set_needs_render();
}

std::string ChatView::create_timestamp()
{
	DateTime datetime = DateTime::get_current_local_time();
	std::string hour = StringHelp::int_to_text(datetime.get_hour());
	if (hour.length() < 2)
		hour = "0"+hour;
	std::string minute = StringHelp::int_to_text(datetime.get_minutes());
	if (minute.length() < 2)
		minute = "0"+minute;

	return hour+":"+minute;
}

void ChatView::layout_subviews(clan::Canvas &canvas)
{
	scroll->set_page_step(geometry().content_box().get_height() / 15.0f);
	View::layout_subviews(canvas);
}

void ChatView::render_text_content(ChatTextView *text_view, Canvas &canvas)
{
	Vec4f clip_offset = canvas.get_transform() * Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	canvas.push_cliprect(Rectf(Pointf(clip_offset.x, clip_offset.y), geometry().content_box().get_size()));

	if (font.is_null())
	{
		font = get_font_description();
		font_url = get_url_font_description();
		font_fixed = get_fixed_font_description();
		baseline_offset1 = (int)(font.get_font_metrics().get_ascent() - font_fixed.get_font_metrics().get_ascent());
	}

	Rect content_box = text_view->geometry().content_box();

	Path::rect(0.0f, 0.0f, (float)get_prefix_width(), (float)content_box.get_height()).fill(canvas, Brush::solid(0.94901f, 0.94901f, 0.94901f));

	content_box.shrink(5);

	int y = content_box.bottom;

	int skip_lines = (int)std::round(scroll->max_position() - (scroll->position() + 1));

	std::list<ChatLine>::reverse_iterator it;
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

		ChatLine &line = *it;
		layout_line(canvas, line, content_box, line_index - 1);

		y -= std::max(line.column2.get_size().height, line.column3.get_size().height);

		line.column1.set_position(Point(content_box.left, y + baseline_offset1));
		line.column1.draw_layout(canvas);
		line.column2.set_position(Point(content_box.left, y));
		line.column2.draw_layout(canvas);
		line.column3.set_position(Point(content_box.left + get_prefix_width(), y));
		line.column3.draw_layout(canvas);
	}

	canvas.pop_cliprect();
}

/*
ChatView::TextPosition ChatView::hit_test(const Point &pos)
{
	GraphicContext gc = get_gc();
	Rect content_box = part_background.get_content_box(get_size());
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

		int line_height = std::max(line->column2.get_size().height, line->column3.get_size().height);
		y -= line_height;

		line->column1.set_position(Point(content_box.left, y));
		line->column2.set_position(Point(content_box.left, y));
		line->column3.set_position(Point(content_box.left+get_prefix_width(), y));

		Rect column1_rect(content_box.left, y, content_box.left+column1_width, y+line_height);
		Rect column2_rect(content_box.left+content_box.left, y, content_box.left+prefix_width, y+line_height);
		Rect column3_rect(content_box.left+prefix_width, y, content_box.right, y+line_height);
		if (column1_rect.contains(pos))
			return hit_test_line_column(gc, line_index-1, 0, line->column1, pos);
		else if (column2_rect.contains(pos))
			return hit_test_line_column(gc, line_index-1, 1, line->column2, pos);
		else if (column3_rect.contains(pos))
			return hit_test_line_column(gc, line_index-1, 2, line->column3, pos);
	}

	return TextPosition();
}

ChatView::TextPosition ChatView::hit_test_line_column(GraphicContext &gc, int line, int column, SpanLayout &span_layout, const Point &pos)
{
	SpanLayout::HitTestResult result = span_layout.hit_test(gc, pos);
	TextPosition text_pos;
	text_pos.line = line;
	text_pos.column = column;
	text_pos.offset = result.offset;
	if(result.type == SpanLayout::HitTestResult::inside)
		text_pos.id = result.object_id;
	return text_pos;
}
*/
void ChatView::on_scroll()
{
	set_needs_render();
}

void ChatView::layout_line(Canvas &canvas, ChatLine &line, Rect &client_area, int line_index)
{
	if (line.layout_width != client_area.get_width() || line.prefix_width != prefix_width)
	{
		line.column1.clear();
		line.column2.clear();

		if (!line.column3_rendered)
		{
			for (auto &i : line.inlines)
			{
				line.column3.add_text(i.text, i.style.get_font(canvas), i.style.color(), i.id);
			}

			line.column3_rendered = true;
		}

		Colorf timestamp_color = get_color_timestamp();

		std::pair<int,int> sel_col1 = get_selection_for_line(line_index, 0);
		std::pair<int,int> sel_col2 = get_selection_for_line(line_index, 1);
		std::pair<int,int> sel_col3 = get_selection_for_line(line_index, 2);
		line.column1.set_selection_range(sel_col1.first, sel_col1.second);
		line.column2.set_selection_range(sel_col2.first, sel_col2.second);
		line.column3.set_selection_range(sel_col3.first, sel_col3.second);

		if (!line.timestamp.empty())
		{
			line.column1.add_text(line.timestamp, font_fixed, Colorf::darkgoldenrod);
		}
		if (!line.nick.empty())
		{
			line.column2.add_text(line.nick, font, line.nick_color);
		}

		int t = line.column1.find_preferred_size(canvas).width;
		column1_width = std::max(column1_width, t);
		int column2_width = line.column2.find_preferred_size(canvas).width;
		int column12_width = column1_width + column2_width + 25;
		if (column12_width > prefix_width)
		{
			prefix_width = column12_width;
			set_needs_render();
		}

		line.column2.set_align(span_right);
		line.column1.layout(canvas, get_prefix_width()-10);
		line.column2.layout(canvas, get_prefix_width()-10);
		line.column3.layout(canvas, client_area.get_width()-get_prefix_width());
		line.layout_width = client_area.get_width();
		line.prefix_width = prefix_width;
	}
}

int ChatView::get_prefix_width() const
{
	return prefix_width;
}

std::pair<int,int> ChatView::get_selection_for_line(int line_index, int column)
{
	int sel_start = offset_for_line_column(line_index, column, selection.start);
	int sel_end = offset_for_line_column(line_index, column, selection.end);
	return std::pair<int,int>(sel_start, sel_end);
}

int ChatView::offset_for_line_column(int line_index, int column, const TextPosition &pos)
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

/*
void ChatView::on_input_message(const GUIMessage_Input &msg)
{
	InputEvent input_event = msg.get_event();

	if(input_event.type == InputEvent::pressed && input_event.id == MOUSE_WHEEL_UP)
	{
		scroll->set_position(scroll->get_position() - 3);
		on_scroll();
	}
	else if(input_event.type == InputEvent::pressed && input_event.id == MOUSE_WHEEL_DOWN)
	{
		scroll->set_position(scroll->get_position() + 3);
		on_scroll();
	}
	else if (input_event.type == InputEvent::pressed && input_event.id == MOUSE_LEFT)
	{
		capture_mouse(true);
		mouse_down = true;
		mouse_down_text_position = hit_test(input_event.mouse_pos);
	}
	else if (input_event.type == InputEvent::released && input_event.id == MOUSE_LEFT)
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
	else if (input_event.type == InputEvent::pointer_moved)
	{
		TextPosition text_position = hit_test(input_event.mouse_pos);

		if (mouse_down)
		{
			set_selection(mouse_down_text_position, text_position);
		}

		if(text_position.id != -1)
		{
			set_cursor(cursor_hand);
		}
		else
		{
			set_cursor(cursor_arrow);
		}
	}
}
*/

void ChatView::set_selection(const TextPosition &start, const TextPosition &end)
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

void ChatView::invalidate_lines(int start, int end)
{
	std::list<ChatLine>::iterator it;
	int line = 0;
	for (it = lines.begin(); it != lines.end(); ++it, line++)
	{
		if (line >= start && line <= end)
		{
			(*it).layout_width = 0;
		}
	}
	set_needs_render();
}

void ChatView::copy_to_clipboard()
{
	std::string text;
	std::list<ChatLine>::iterator it;
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
		}
	}
	if (!text.empty())
	{
		WindowView *window_view = dynamic_cast<WindowView*>(root_view());
		if (window_view)
			window_view->get_display_window().set_clipboard_text(text);
	}
}

void ChatView::append_column_text(int line, int column, ChatLine &chatline, std::string prefix, std::string postfix, std::string &out_text)
{
	int start_offset = offset_for_line_column(line, column, selection.start);
	int end_offset = offset_for_line_column(line, column, selection.end);
	if (start_offset != end_offset)
	{
		std::string column_text;
		if (column == 0)
			column_text = chatline.column1.get_combined_text();
		else if (column == 1)
			column_text = chatline.column2.get_combined_text();
		else if (column == 2)
			column_text = chatline.column3.get_combined_text();

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

Colorf ChatView::get_color_text()
{
	return Colorf("#000000"); // black
}

Colorf ChatView::get_color_channel()
{
	return Colorf("#228b22"); // forestgreen
}

Colorf ChatView::get_color_system()
{
	return Colorf("#5f9ea0"); // cadetblue
}

Colorf ChatView::get_color_nick_others()
{
	return Colorf("#000099");
}

Colorf ChatView::get_color_nick()
{
	return Colorf("#990000");
}

Colorf ChatView::get_color_timestamp()
{
	return Colorf("#6495ed"); // darkgoldenrod
}

Colorf ChatView::get_color_url()
{
	return Colorf("#000080"); // navy
}

void ChatView::scroll_page_up()
{
	scroll->set_position(scroll->position() - scroll->page_step());
	on_scroll();
}

void ChatView::scroll_page_down()
{
	scroll->set_position(scroll->position() + scroll->page_step());
	on_scroll();
}
