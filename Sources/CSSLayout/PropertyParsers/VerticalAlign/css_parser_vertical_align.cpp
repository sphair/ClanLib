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

#include "CSSLayout/precomp.h"
#include "css_parser_vertical_align.h"
#include "API/CSSLayout/PropertyValues/css_value_vertical_align.h"

namespace clan
{

std::vector<std::string> CSSParserVerticalAlign::get_names()
{
	std::vector<std::string> names;
	names.push_back("vertical-align");
	return names;
}

void CSSParserVerticalAlign::parse(const std::string &name, const std::vector<CSSToken> &tokens, std::vector<std::unique_ptr<CSSPropertyValue> > &inout_values)
{
	std::unique_ptr<CSSValueVerticalAlign> vertical_align(new CSSValueVerticalAlign());

	size_t pos = 0;
	CSSToken token = next_token(pos, tokens);
	if (token.type == CSSToken::type_ident && pos == tokens.size())
	{
		if (equals(token.value, "baseline"))
			vertical_align->type = CSSValueVerticalAlign::type_baseline;
		else if (equals(token.value, "sub"))
			vertical_align->type = CSSValueVerticalAlign::type_sub;
		else if (equals(token.value, "super"))
			vertical_align->type = CSSValueVerticalAlign::type_super;
		else if (equals(token.value, "top"))
			vertical_align->type = CSSValueVerticalAlign::type_top;
		else if (equals(token.value, "text-top"))
			vertical_align->type = CSSValueVerticalAlign::type_text_top;
		else if (equals(token.value, "middle"))
			vertical_align->type = CSSValueVerticalAlign::type_middle;
		else if (equals(token.value, "bottom"))
			vertical_align->type = CSSValueVerticalAlign::type_bottom;
		else if (equals(token.value, "text-bottom"))
			vertical_align->type = CSSValueVerticalAlign::type_text_bottom;
		else if (equals(token.value, "inherit"))
			vertical_align->type = CSSValueVerticalAlign::type_inherit;
		else
			return;
	}
	else if (is_length(token) && pos == tokens.size())
	{
		CSSLength length;
		if (parse_length(token, length))
		{
			vertical_align->type = CSSValueVerticalAlign::type_length;
			vertical_align->length = length;
		}
		else
		{
			return;
		}
	}
	else if (token.type == CSSToken::type_percentage && pos == tokens.size())
	{
		vertical_align->type = CSSValueVerticalAlign::type_percentage;
		vertical_align->percentage = StringHelp::text_to_float(token.value);
	}
	else if (token.type == CSSToken::type_delim && token.value == "-")
	{
		token = next_token(pos, tokens);
		if (is_length(token) && pos == tokens.size())
		{
			CSSLength length;
			if (parse_length(token, length))
			{
				length.value = -length.value;
				vertical_align->type = CSSValueVerticalAlign::type_length;
				vertical_align->length = length;
			}
			else
			{
				return;
			}
		}
		else if (token.type == CSSToken::type_percentage && pos == tokens.size())
		{
			vertical_align->type = CSSValueVerticalAlign::type_percentage;
			vertical_align->percentage = -StringHelp::text_to_float(token.value);
		}
		else
		{
			return;
		}
	}
	else
	{
		return;
	}

	inout_values.push_back(std::move(vertical_align));
}

}
