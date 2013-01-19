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
#include "css_parser_list_style_type.h"
#include "API/CSSLayout/ComputedValues/css_computed_box.h"

namespace clan
{

std::vector<std::string> CSSParserListStyleType::get_names()
{
	std::vector<std::string> names;
	names.push_back("list-style-type");
	return names;
}

void CSSParserListStyleType::parse(const std::string &name, const std::vector<CSSToken> &tokens, std::vector<std::unique_ptr<CSSPropertyValue> > &inout_values)
{
	std::unique_ptr<CSSValueListStyleType> list_style_type(new CSSValueListStyleType());

	size_t pos = 0;
	CSSToken token = next_token(pos, tokens);
	if (token.type == CSSToken::type_ident && pos == tokens.size())
	{
		if (equals(token.value, "disc"))
			list_style_type->type = CSSValueListStyleType::type_disc;
		else if (equals(token.value, "circle"))
			list_style_type->type = CSSValueListStyleType::type_circle;
		else if (equals(token.value, "square"))
			list_style_type->type = CSSValueListStyleType::type_square;
		else if (equals(token.value, "decimal"))
			list_style_type->type = CSSValueListStyleType::type_decimal;
		else if (equals(token.value, "decimal-leading-zero"))
			list_style_type->type = CSSValueListStyleType::type_decimal_leading_zero;
		else if (equals(token.value, "lower-roman"))
			list_style_type->type = CSSValueListStyleType::type_lower_roman;
		else if (equals(token.value, "upper-roman"))
			list_style_type->type = CSSValueListStyleType::type_upper_roman;
		else if (equals(token.value, "lower-greek"))
			list_style_type->type = CSSValueListStyleType::type_lower_greek;
		else if (equals(token.value, "lower-latin"))
			list_style_type->type = CSSValueListStyleType::type_lower_latin;
		else if (equals(token.value, "upper-latin"))
			list_style_type->type = CSSValueListStyleType::type_upper_latin;
		else if (equals(token.value, "armenian"))
			list_style_type->type = CSSValueListStyleType::type_armenian;
		else if (equals(token.value, "georgian"))
			list_style_type->type = CSSValueListStyleType::type_georgian;
		else if (equals(token.value, "lower-alpha"))
			list_style_type->type = CSSValueListStyleType::type_lower_alpha;
		else if (equals(token.value, "upper-alpha"))
			list_style_type->type = CSSValueListStyleType::type_upper_alpha;
		else if (equals(token.value, "none"))
			list_style_type->type = CSSValueListStyleType::type_none;
		else if (equals(token.value, "inherit"))
			list_style_type->type = CSSValueListStyleType::type_inherit;
		else
			return;
	}
	else
	{
		return;
	}

	inout_values.push_back(std::move(list_style_type));
}

}
