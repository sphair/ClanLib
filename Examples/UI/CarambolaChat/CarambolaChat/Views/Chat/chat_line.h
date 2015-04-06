
#pragma once

class ChatLine
{
public:
	ChatLine();
	ChatLine(const std::string &nick, const clan::Colorf &nick_color);

	void add_text(const std::shared_ptr<clan::Style> &style, const std::string &text, int id = -1);

private:
	struct InlineText
	{
		InlineText(const std::shared_ptr<clan::Style> &style, const std::string &text, int id) : style(style), text(text), id(id) { }

		std::shared_ptr<clan::Style> style;
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
