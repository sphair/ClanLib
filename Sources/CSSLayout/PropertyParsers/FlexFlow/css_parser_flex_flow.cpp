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
*/

#include "CSSLayout/precomp.h"
#include "css_parser_flex_flow.h"
#include "API/CSSLayout/PropertyValues/css_value_flex_direction.h"
#include "API/CSSLayout/PropertyValues/css_value_flex_wrap.h"

namespace clan
{

std::vector<std::string> CSSParserFlexFlow::get_names()
{
	std::vector<std::string> names;
	names.push_back("flex-flow");
	return names;
}

void CSSParserFlexFlow::parse(const std::string &name, const std::vector<CSSToken> &tokens, std::vector<std::unique_ptr<CSSPropertyValue> > &inout_values)
{
	size_t pos = 0;
	CSSToken token = next_token(pos, tokens);

	std::unique_ptr<CSSValueFlexDirection> direction(new CSSValueFlexDirection());
	std::unique_ptr<CSSValueFlexWrap> wrap(new CSSValueFlexWrap());

	bool direction_specified = false;
	bool wrap_specified = false;

	do
	{
		if (!direction_specified && parse_direction(*direction.get(), pos, tokens))
		{
			direction_specified = true;
		}
		else if (!wrap_specified && parse_wrap(*wrap.get(), pos, tokens))
		{
			wrap_specified = true;
		}
		else
		{
			return;
		}
	} while (pos != tokens.size());

	inout_values.push_back(std::move(direction));
	inout_values.push_back(std::move(wrap));
}

bool CSSParserFlexFlow::parse_direction(CSSValueFlexDirection &direction, size_t &parse_pos, const std::vector<CSSToken> &tokens)
{
	size_t pos = parse_pos;
	CSSToken token = next_token(pos, tokens);

	if (token.type == CSSToken::type_ident)
	{
		if (equals(token.value, "row"))
			direction.type = CSSValueFlexDirection::type_row;
		else if (equals(token.value, "row-reverse"))
			direction.type = CSSValueFlexDirection::type_row_reverse;
		else if (equals(token.value, "column"))
			direction.type = CSSValueFlexDirection::type_column;
		else if (equals(token.value, "column-reverse"))
			direction.type = CSSValueFlexDirection::type_column_reverse;
		else if (equals(token.value, "inherit"))
			direction.type = CSSValueFlexDirection::type_inherit;
		else
			return false;
	}
	else
	{
		return false;
	}

	parse_pos = pos;
	return true;
}

bool CSSParserFlexFlow::parse_wrap(CSSValueFlexWrap &wrap, size_t &parse_pos, const std::vector<CSSToken> &tokens)
{
	size_t pos = parse_pos;
	CSSToken token = next_token(pos, tokens);

	if (token.type == CSSToken::type_ident)
	{
		if (equals(token.value, "nowrap"))
			wrap.type = CSSValueFlexWrap::type_nowrap;
		else if (equals(token.value, "wrap"))
			wrap.type = CSSValueFlexWrap::type_wrap;
		else if (equals(token.value, "wrap-reverse"))
			wrap.type = CSSValueFlexWrap::type_wrap_reverse;
		else if (equals(token.value, "inherit"))
			wrap.type = CSSValueFlexWrap::type_inherit;
		else
			return false;
	}
	else
	{
		return false;
	}

	parse_pos = pos;
	return true;
}

}
