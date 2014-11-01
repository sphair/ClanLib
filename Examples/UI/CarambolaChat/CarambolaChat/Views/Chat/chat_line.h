
#pragma once

class ChatLine
{
public:
	bool bold = false;
	std::string timestamp;
	std::string nick;
	clan::Colorf nick_color;
	clan::SpanLayout column1;
	clan::SpanLayout column2;
	clan::SpanLayout column3;
	int layout_width = 0;
	int prefix_width = 0;
};
