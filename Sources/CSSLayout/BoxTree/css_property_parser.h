/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

class CL_CSSToken;
class CL_CSSBoxProperties;
class CL_CSSBoxLength;
class CL_CSSBoxProperty;

class CL_CSSPropertyParser
{
public:
	virtual ~CL_CSSPropertyParser() { }
	virtual std::vector<CL_String> get_names() = 0;
	virtual void parse(CL_CSSBoxProperties &properties, const CL_String &name, const std::vector<CL_CSSToken> &tokens, std::map<CL_String, CL_CSSBoxProperty *> *out_change_set) = 0;

protected:
	CL_CSSToken next_token(size_t &pos, const std::vector<CL_CSSToken> &tokens, bool skip_whitespace = true);
	bool is_length(const CL_CSSToken &token);
	bool parse_length(const CL_CSSToken &token, CL_CSSBoxLength &out_length);
	bool parse_integer(const CL_String &value, int &out_int);
	bool parse_color(const std::vector<CL_CSSToken> &tokens, size_t &in_out_pos, CL_Colorf &out_color);
	static bool equals(const CL_String &s1, const CL_String &s2);
	void debug_parse_error(const CL_String &name, const std::vector<CL_CSSToken> &tokens);

private:
	struct ColorType
	{
		const CL_String::char_type *name;
		int color;
	};
	static ColorType colors[];
};
