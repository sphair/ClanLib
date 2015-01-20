
#pragma once

#include <list>

class IRCSession;
class ChatLine;
class ChatTextView;

class ChatView : public clan::View
{
public:
	ChatView();
	~ChatView();

	struct TextPosition
	{
		TextPosition() : line(-1), column(0), offset(0), id(-1) { }
		bool operator ==(const TextPosition &other) const { return line == other.line && column == other.column && offset == other.offset; }
		bool operator !=(const TextPosition &other) const { return line != other.line || column != other.column || offset != other.offset; }
		int line, column, offset, id;
	};

	struct Selection
	{
		TextPosition start, end;
	};

	void add_line(ChatLine text);

	TextPosition hit_test(const clan::Point &pos);
	void set_selection(const TextPosition &start, const TextPosition &end);
	void copy_to_clipboard();

	clan::Colorf get_color_text();
	clan::Colorf get_color_channel();
	clan::Colorf get_color_system();
	clan::Colorf get_color_nick_others();
	clan::Colorf get_color_nick();
	clan::Colorf get_color_timestamp();
	clan::Colorf get_color_url();

	void scroll_page_up();
	void scroll_page_down();

	clan::Font font, font_url, font_fixed;

	clan::Signal<void(int/* object_id*/)> cb_url_clicked;

	void layout_subviews(clan::Canvas &canvas) override;

private:
	void render_text_content(ChatTextView *text_view, clan::Canvas &canvas);

	void on_scroll();
	void on_pointer_press(clan::PointerEvent &e);
	void on_pointer_release(clan::PointerEvent &e);
	void on_pointer_move(clan::PointerEvent &e);

	TextPosition hit_test_line_column(clan::Canvas &canvas, int line, int column, clan::SpanLayout &span_layout, const clan::Point &pos);
	static int offset_for_line_column(int line_index, int column, const TextPosition &pos);

	void append_column_text(int line, int column, ChatLine &chatline, std::string prefix, std::string postfix, std::string &out_text);

	std::string create_timestamp();
	void layout_line(clan::Canvas &canvas, ChatLine &line, clan::Rect &client_area, int line_index);
	std::pair<int,int> get_selection_for_line(int line_index, int column);
	int get_prefix_width() const;
	void invalidate_lines(int start, int end);

	std::shared_ptr<ChatTextView> text_view;
	std::shared_ptr<clan::ScrollBarView> scroll;
	std::list<ChatLine> lines;
	clan::SlotContainer slots;
	Selection selection;

	int prefix_width = 60;
	int column1_width = 0;
	int baseline_offset1 = 0;

	bool mouse_down = false;
	TextPosition mouse_down_text_position;

	friend class ChatTextView;
};

class ChatTextView : public clan::View
{
public:
	ChatTextView(ChatView *chat_view) : chat_view(chat_view) { }
	void render_content(clan::Canvas &canvas) override { chat_view->render_text_content(this, canvas); }

private:
	ChatView *chat_view;
};
