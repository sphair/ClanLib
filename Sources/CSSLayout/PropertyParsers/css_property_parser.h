/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
*/

#pragma once

namespace clan
{

class CSSToken;
class CSSBoxProperties;
class CSSLength;
class CSSPropertyValue;

class CSSPropertyParser
{
public:
	virtual ~CSSPropertyParser() { }
	virtual std::vector<std::string> get_names() = 0;
	virtual void parse(CSSBoxProperties &properties, const std::string &name, const std::vector<CSSToken> &tokens, std::map<std::string, CSSPropertyValue *> *out_change_set) = 0;

protected:
	CSSToken next_token(size_t &pos, const std::vector<CSSToken> &tokens, bool skip_whitespace = true);
	bool is_length(const CSSToken &token);
	bool parse_length(const CSSToken &token, CSSLength &out_length);
	bool parse_integer(const std::string &value, int &out_int);
	bool parse_color(const std::vector<CSSToken> &tokens, size_t &in_out_pos, Colorf &out_color);
	static bool equals(const std::string &s1, const std::string &s2);
	void debug_parse_error(const std::string &name, const std::vector<CSSToken> &tokens);

private:
	struct ColorType
	{
		const std::string::value_type *name;
		int color;
	};
	static ColorType colors[];
};

}
