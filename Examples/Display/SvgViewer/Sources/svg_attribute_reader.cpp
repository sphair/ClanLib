/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
**    Mark Page
*/

#include "precomp.h"
#include "svg_attribute_reader.h"
#include "svg.h"

SvgAttributeReader::SvgAttributeReader(const std::string &attr) : attr(attr)
{
	eat_whitespace();
}

SvgAttributeReader::SvgAttributeReader(clan::DomElement e, const std::string &attr_name, bool inherit)
{
	while (!e.is_null() && !e.has_attribute_ns(Svg::svg_ns, attr_name))
		e = e.get_parent_node().to_element();

	attr = e.get_attribute_ns(Svg::svg_ns, attr_name);
	eat_whitespace();
}

bool SvgAttributeReader::is_whitespace() const
{
	if (!is_end())
	{
		switch (attr[pos])
		{
		case ' ':
		case '\t':
		case '\r':
		case '\n':
			return true;
		default:
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool SvgAttributeReader::is_keyword(const std::string &keyword) const
{
	if (!is_end() && attr.substr(pos, keyword.length()) == keyword)
	{
		size_t end_pos = pos + keyword.length();
		if (end_pos == attr.length())
		{
			return true;
		}
		else
		{
			bool is_keyword_char = (attr[end_pos] >= 'a' && attr[end_pos] <= 'z') || (attr[end_pos] >= 'A' && attr[end_pos] <= 'Z') || (attr[end_pos] >= '0' && attr[end_pos] <= '9') || attr[end_pos] == '_';
			return !is_keyword_char;
		}
	}
	else
	{
		return false;
	}
}

bool SvgAttributeReader::is_operator(const std::string &op) const
{
	// To do: doesn't support multi-character operators properly (does svg have any of those?)
	return attr.substr(pos, op.length()) == op;
}

bool SvgAttributeReader::is_end() const
{
	return pos == attr.length() || pos == std::string::npos;
}

bool SvgAttributeReader::is_number() const
{
	// To do: look for invalid suffixes, multiple dots and such
	return !is_end() && ((attr[pos] >= '0' && attr[pos] <= '9') || attr[pos] == '-' || attr[pos] == '+');
}

bool SvgAttributeReader::is_length() const
{
	return is_number(); // To do: add support for 'px', 'pt', etc. (loot it from old clanCSS)
}

bool SvgAttributeReader::is_sequence_number() const
{
	return is_operator(",") || is_number();
}

bool SvgAttributeReader::is_color() const
{
	return !is_end() && attr[pos] == '#'; // To do: support all the colors
}

clan::Colorf SvgAttributeReader::get_color()
{
	if (!is_color()) parse_error("expected color");

	size_t end_pos = pos + 1;
	while (end_pos < attr.length())
	{
		bool is_hex_char = (attr[end_pos] >= 'a' && attr[end_pos] <= 'f') || (attr[end_pos] >= 'A' && attr[end_pos] <= 'F') || (attr[end_pos] >= '0' && attr[end_pos] <= '9');
		if (!is_hex_char) break;
		end_pos++;
	}

	if (end_pos != pos + 4 && end_pos != pos + 7) parse_error("invalid color");

	clan::Colorf color;
	std::string hexstr = attr.substr(pos + 1, end_pos - pos - 1);
	unsigned int value = strtoul(hexstr.c_str(), 0, 16);
	if (end_pos == pos + 4)
	{
		int red = ((((value >> 8) & 0xf) + 1) << 4) - 1;
		int green = ((((value >> 4) & 0xf) + 1) << 4) - 1;
		int blue = (((value & 0xf) + 1) << 4) - 1;
		color = clan::Colorf(red / 255.0f, green / 255.0f, blue / 255.0f);
	}
	else
	{
		int red = (value >> 16) & 0xff;
		int green = (value >> 8) & 0xff;
		int blue = value & 0xff;
		color = clan::Colorf(red / 255.0f, green / 255.0f, blue / 255.0f);
	}
	pos = end_pos;
	return color;
}

std::string SvgAttributeReader::peek_keyword()
{
	if (is_end()) return std::string();

	bool is_first_keyword_char = (attr[pos] >= 'a' && attr[pos] <= 'z') || (attr[pos] >= 'A' && attr[pos] <= 'Z') || attr[pos] == '_';
	if (!is_first_keyword_char) return std::string();

	size_t end_pos = pos;
	while (end_pos < attr.length())
	{
		bool is_keyword_char = (attr[end_pos] >= 'a' && attr[end_pos] <= 'z') || (attr[end_pos] >= 'A' && attr[end_pos] <= 'Z') || (attr[end_pos] >= '0' && attr[end_pos] <= '9') || attr[end_pos] == '_';
		if (!is_keyword_char) break;
		end_pos++;
	}
	
	return attr.substr(pos, end_pos - pos);
}

std::string SvgAttributeReader::peek_operator()
{
	if (is_end() || is_whitespace()) return std::string();

	bool is_first_keyword_char = (attr[pos] >= 'a' && attr[pos] <= 'z') || (attr[pos] >= 'A' && attr[pos] <= 'Z') || attr[pos] == '_';
	if (is_first_keyword_char) return std::string();

	size_t end_pos = pos;
	while (end_pos < attr.length())
	{
		bool is_keyword_char = (attr[end_pos] >= 'a' && attr[end_pos] <= 'z') || (attr[end_pos] >= 'A' && attr[end_pos] <= 'Z') || (attr[end_pos] >= '0' && attr[end_pos] <= '9') || attr[end_pos] == '_';
		bool is_whitespace_char = (attr[end_pos] == ' ' || attr[end_pos] == '\r' || attr[end_pos] == '\n');
		if (is_keyword_char || is_whitespace_char) break;
		end_pos++;
	}

	return attr.substr(pos, end_pos - pos);
}

void SvgAttributeReader::eat_whitespace()
{
	while (is_whitespace()) pos++;
}

void SvgAttributeReader::get_keyword(const std::string &keyword)
{
	if (!is_keyword(keyword)) parse_error("expected " + keyword);
	pos += keyword.length();
	eat_whitespace();
}

void SvgAttributeReader::get_operator(const std::string &op)
{
	if (!is_operator(op)) parse_error("expected " + op);
	pos += op.length();
	eat_whitespace();
}

double SvgAttributeReader::get_number()
{
	if (!is_number()) parse_error("expected number");

	size_t end_pos = pos + 1;
	bool found_exponent = false;
	while (end_pos < attr.length())
	{
		if (!found_exponent && (attr[end_pos] == 'e' || attr[end_pos] == 'E'))
		{
			found_exponent = true;
			end_pos++;
			if (end_pos < attr.length() && (attr[end_pos] == '+' || attr[end_pos] == '-'))
				end_pos++;
		}
		else
		{
			if (!(attr[end_pos] >= '0' && attr[end_pos] <= '9') && attr[end_pos] != '.') break;
			end_pos++;
		}
	}

	double number = clan::StringHelp::text_to_double(attr.substr(pos, end_pos - pos));
	pos = end_pos;

	eat_whitespace();

	return number;
}

double SvgAttributeReader::get_length()
{
	if (!is_length()) parse_error("expected length");
	return get_number(); // To do: add support for 'px', 'pt', etc. (loot it from old clanCSS)
}

void SvgAttributeReader::parse_error(const std::string &reason)
{
	throw clan::Exception(reason);
}

double SvgAttributeReader::single_number(clan::DomElement &e, const std::string &attr_name, double default_value)
{
	std::string attr = e.get_attribute_ns(Svg::svg_ns, attr_name);

	SvgAttributeReader reader(attr);
	if (reader.is_number())
	{
		double len = reader.get_number();
		return reader.is_end() ? len : default_value;
	}
	else
	{
		return default_value;
	}
}

double SvgAttributeReader::single_length(clan::DomElement &e, const std::string &attr_name, double default_value)
{
	std::string attr = e.get_attribute_ns(Svg::svg_ns, attr_name);

	SvgAttributeReader reader(attr);
	if (reader.is_length())
	{
		double len = reader.get_length();
		return reader.is_end() ? len : default_value;
	}
	else
	{
		return default_value;
	}
}

char SvgAttributeReader::get_path_command()
{
	if (is_end()) parse_error("expected path command");

	if ((attr[pos] >= 'a' && attr[pos] <= 'z') || (attr[pos] >= 'A' && attr[pos] <= 'Z'))
	{
		char command = attr[pos++];
		eat_whitespace();
		return command;
	}
	else
	{
		parse_error("expected path command");
		return 0;
	}
}

double SvgAttributeReader::get_sequence_number()
{
	if (is_operator(",")) get_operator(",");
	return get_number();
}
