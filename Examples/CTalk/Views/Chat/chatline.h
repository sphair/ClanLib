
#pragma once

class ChatLine
{
public:
	ChatLine(): bold(false), layout_width(0), prefix_width(0) { }

public:
	bool bold;
	CL_String timestamp;
	CL_String nick;
	CL_Colorf nick_color;
	CL_SpanLayout column1;
	CL_SpanLayout column2;
	CL_SpanLayout column3;
	int layout_width, prefix_width;
};
