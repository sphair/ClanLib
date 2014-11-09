
#include "precomp.h"
#include "chat_line.h"

using namespace clan;

ChatLine::ChatLine()
{
}

ChatLine::ChatLine(const std::string &nick, const clan::Colorf &nick_color) : nick(nick), nick_color(nick_color)
{
}

void ChatLine::add_text(const clan::TextStyle &style, const std::string &text, int id)
{
	inlines.push_back(InlineText(style, text, id));
}
