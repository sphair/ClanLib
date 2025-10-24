
#pragma once

#include "irc_raw_string.h"

class IRCEscape
{
public:
	enum Type
	{
		type_ctcp, // ^A (character 01)
		type_bold, // ^B (character 02)
		type_color, // ^CF[,B] (character 03, F and B being range 0 to 15)
		type_reset // ^O 
	};

	IRCEscape();
	IRCEscape(Type type, CL_String::size_type position, const IRCRawString &data);

	Type get_type() const { return type; }
	CL_String::size_type get_position() const { return position; }
	IRCRawString get_ctcp_data() const;
	CL_Colorf get_color() const;

private:
	Type type;
	CL_String::size_type position;
	IRCRawString data;
};

class IRCText
{
public:
	IRCText();

	IRCRawString to_raw() const { return text8; }

	static IRCText from_text(const CL_String &text);
	static IRCText from_raw(const IRCRawString &raw_text);
	static IRCText from_ctcp_data(const IRCRawString &ctcp_data);

	bool is_ctcp() const;
	IRCRawString get_ctcp_data() const;

	CL_String get_text() const;
	std::vector<IRCEscape> get_escapes() const;

	bool empty() const;

private:
	IRCRawString text8;
};
