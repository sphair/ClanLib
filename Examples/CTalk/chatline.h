
#pragma once

class ChatLine
{
public:
	ChatLine(): layout_width(0) { }

public:
	CL_String timestamp;
	CL_String nick;
	CL_String text;
	CL_Colorf color;
	CL_SpanLayout layout;
	int layout_width;
};
