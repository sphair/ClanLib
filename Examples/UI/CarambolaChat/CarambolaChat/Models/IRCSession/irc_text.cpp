
#include "precomp.h"
#include "irc_text.h"

IRCText::IRCText()
{
}

IRCText IRCText::from_text(const std::string &text)
{
	IRCText t;
	t.text8 = clan::StringHelp::text_to_utf8(text);
	return t;
}

IRCText IRCText::from_raw(const IRCRawString &raw_text)
{
	IRCText t;
	t.text8 = raw_text;
	return t;
}

IRCText IRCText::from_ctcp_data(const IRCRawString &ctcp_data)
{
	IRCRawString s;
	s.append(1, 1);
	s.append(ctcp_data);
	s.append(1, 1);
	return from_raw(s);
}

std::string IRCText::get_text() const
{
	if (is_ctcp())
	{
		return std::string();
	}
	else
	{
		IRCRawString s = text8;
		IRCRawString::size_type pos = 0;
		bool ctcp = false;
		for (size_t i = 0; i<s.size();)
		{
			if (((unsigned int)s[i]) < 32)
			{
				if (s[i] == 1 || s[i] == 2 || s[i] == 15) // ^A (ctcp), ^B (bold) or ^O (reset)
				{
					s.erase(s.begin()+i);
				}
				else if (s[i] == 3)
				{
					IRCRawString::size_type end_pos = s.find_first_not_of("0123456789,", i+1);
					if (end_pos > i+6)
						end_pos = i+6;
					s = s.substr(0,i)+s.substr(end_pos);
				}
				else if (s[i] == '\t')
				{
					s = s.substr(0, i)+"    "+s.substr(i+1);
					i+=4;
				}
				else
				{
					// s[i] = 'A'+s[i];
					s[i] = '?';
					i++;
				}
			}
			else
			{
				i++;
			}
		}
		return clan::StringHelp::utf8_to_text(s);
	}
}

std::vector<IRCEscape> IRCText::get_escapes() const
{
	if (is_ctcp())
	{
		std::vector<IRCEscape> escapes;
		escapes.push_back(IRCEscape(IRCEscape::type_ctcp, 0, text8));
		return escapes;
	}
	else
	{
		return std::vector<IRCEscape>();
	}
}

bool IRCText::empty() const
{
	return text8.empty();
}

bool IRCText::is_ctcp() const
{
	if (text8.length() >= 2 && text8[0] == 1 && text8[text8.length()-1] == 1)
		return true;
	else
		return false;
}

IRCRawString IRCText::get_ctcp_data() const
{
	if (text8.empty())
		return text8;
	else
		return text8.substr(1, text8.length()-2);
}

/////////////////////////////////////////////////////////////////////////////

IRCEscape::IRCEscape()
: type(type_ctcp), position(0)
{
}

IRCEscape::IRCEscape(Type type, std::string::size_type position, const IRCRawString &data)
: type(type), position(position), data(data)
{
}

IRCRawString IRCEscape::get_ctcp_data() const
{
	if (data.empty())
		return data;
	else
		return data.substr(1, data.length()-1);
}

clan::Colorf IRCEscape::get_color() const
{
	return clan::Colorf::black;
}
