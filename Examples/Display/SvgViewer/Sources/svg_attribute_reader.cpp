/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

SvgAttributeReader::SvgAttributeReader(const std::string &attr) : attr(attr)
{
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
	return !is_end() && !is_length() && ((attr[pos] >= '0' && attr[pos] <= '9') || attr[pos] == '-' || attr[pos] == '+');
}

bool SvgAttributeReader::is_length() const
{
	return false; // To do: add support for 'px', 'pt', etc. (loot it from old clanCSS)
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

	size_t end_pos = pos;
	while (end_pos < attr.length())
	{
		if (!(attr[pos] >= '0' && attr[pos] <= '9') && attr[pos] != '.' && attr[pos] != '+' && attr[pos] != '-' && attr[pos] != 'e' && attr[pos] != 'E') break;
		end_pos++;
	}

	double number = clan::StringHelp::text_to_double(attr.substr(pos, end_pos - pos));
	pos = end_pos;

	eat_whitespace();

	return number;
}

double SvgAttributeReader::get_length()
{
	if (!is_length()) throw clan::Exception("Parse error; expected length");
	return 0.0f; // To do: add support for 'px', 'pt', etc. (loot it from old clanCSS)
}

void SvgAttributeReader::parse_error(const std::string &reason)
{
	throw clan::Exception(reason);
}
