
#pragma once

#include <list>

class View;
class IRCSession;
class ChatLine;

class Chat : public CL_GUIComponent
{
public:
	Chat(CL_GUIComponent *parent);
	~Chat();

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

	void add_line(const CL_SpanLayout &text);
	void add_line(const CL_String &nick, const CL_Colorf &nick_color, const CL_SpanLayout &text);

	TextPosition hit_test(const CL_Point &pos);
	void set_selection(const TextPosition &start, const TextPosition &end);
	void copy_to_clipboard();

	CL_Colorf get_color_text();
	CL_Colorf get_color_channel();
	CL_Colorf get_color_system();
	CL_Colorf get_color_nick_others();
	CL_Colorf get_color_nick();
	CL_Colorf get_color_timestamp();
	CL_Colorf get_color_url();

	void scroll_page_up();
	void scroll_page_down();

	CL_Font font, font_url, font_fixed;
	CL_Font font_bold, font_url_bold, font_fixed_bold;

	CL_Signal_v1<int /* object_id*/> cb_url_clicked;

private:
	void on_resize();
	void on_render(CL_GraphicContext &gc, const CL_Rect &clip_rect);
	void on_scroll();
	void on_process_message(CL_GUIMessage &msg);
	void on_input_message(const CL_GUIMessage_Input &msg);
	void on_pointer_message(const CL_GUIMessage_Pointer &msg);

	TextPosition hit_test_line_column(CL_GraphicContext &gc, int line, int column, CL_SpanLayout &span_layout, const CL_Point &pos);
	static int offset_for_line_column(int line_index, int column, const TextPosition &pos);

	CL_String create_timestamp();
	void layout_line(CL_GraphicContext & gc, ChatLine *line, CL_Rect &client_area, int line_index);
	std::pair<int,int> get_selection_for_line(int line_index, int column);
	int get_prefix_width() const;
	void invalidate_lines(int start, int end);

	static CL_FontDescription get_font_description();
	static CL_FontDescription get_fixed_font_description();
	static CL_FontDescription get_url_font_description();

	CL_ScrollBar *scroll;
	std::list<ChatLine *> lines;
	CL_SlotContainer slots;
	CL_GUIThemePart part_background;
	Selection selection;

	int prefix_width, column1_width;

	bool mouse_down;
	TextPosition mouse_down_text_position;
};
