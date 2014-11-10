
#pragma once

class ChatLine
{
public:
	ChatLine();
	ChatLine(const std::string &nick, const clan::Colorf &nick_color);

	void add_text(const clan::TextStyle &style, const std::string &text, int id = -1);

private:
	struct InlineText
	{
		InlineText(const clan::TextStyle &style, const std::string &text, int id) : style(style), text(text), id(id) { }

		clan::TextStyle style;
		std::string text;
		int id;
	};

	std::vector<InlineText> inlines;

	bool bold = false;
	std::string timestamp;
	std::string nick;
	clan::Colorf nick_color = clan::Colorf::black;
	clan::SpanLayout column1;
	clan::SpanLayout column2;
	clan::SpanLayout column3;
	bool column3_rendered = false;
	int layout_width = 0;
	int prefix_width = 0;

	friend class ChatView;
};
