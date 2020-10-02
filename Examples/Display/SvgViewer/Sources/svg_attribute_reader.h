/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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

#pragma once

class SvgAttributeReader
{
public:
	SvgAttributeReader(const std::string &attr);
	SvgAttributeReader(clan::DomElement e, const std::string &attr_name, bool inherit = false);

	bool is_whitespace() const;
	bool is_keyword(const std::string &keyword) const;
	bool is_operator(const std::string &op) const;
	bool is_end() const;
	bool is_number() const;
	bool is_length() const;
	bool is_sequence_number() const;
	bool is_color() const;

	std::string peek_keyword();
	std::string peek_operator();

	void eat_whitespace();
	void get_keyword(const std::string &keyword);
	void get_operator(const std::string &op);
	double get_number();
	double get_length();
	clan::Colorf get_color();

	char get_path_command();
	double get_sequence_number();

	void parse_error(const std::string &reason);

	static double single_number(clan::DomElement &e, const std::string &attr_name, double default_value = 0.0);
	static double single_length(clan::DomElement &e, const std::string &attr_name, double default_value = 0.0);

private:
	std::string attr;
	size_t pos = 0;
};
