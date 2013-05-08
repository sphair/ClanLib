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
#include "css_parser_font_size.h"
#include "API/CSSLayout/PropertyValues/css_value_font_size.h"

namespace clan
{

std::vector<std::string> CSSParserFontSize::get_names()
{
	std::vector<std::string> names;
	names.push_back("font-size");
	return names;
}

void CSSParserFontSize::parse(const std::string &name, const std::vector<CSSToken> &tokens, std::vector<std::unique_ptr<CSSPropertyValue> > &inout_values)
{
	std::unique_ptr<CSSValueFontSize> font_size(new CSSValueFontSize());

	size_t pos = 0;
	CSSToken token = next_token(pos, tokens);
	if (token.type == CSSToken::type_ident && pos == tokens.size())
	{
		if (equals(token.value, "xx-small"))
			font_size->type = CSSValueFontSize::type_xx_small;
		else if (equals(token.value, "x-small"))
			font_size->type = CSSValueFontSize::type_x_small;
		else if (equals(token.value, "small"))
			font_size->type = CSSValueFontSize::type_small;
		else if (equals(token.value, "medium"))
			font_size->type = CSSValueFontSize::type_medium;
		else if (equals(token.value, "large"))
			font_size->type = CSSValueFontSize::type_large;
		else if (equals(token.value, "x-large"))
			font_size->type = CSSValueFontSize::type_x_large;
		else if (equals(token.value, "xx-large"))
			font_size->type = CSSValueFontSize::type_xx_large;
		else if (equals(token.value, "smaller"))
			font_size->type = CSSValueFontSize::type_smaller;
		else if (equals(token.value, "larger"))
			font_size->type = CSSValueFontSize::type_larger;
		else if (equals(token.value, "inherit"))
			font_size->type = CSSValueFontSize::type_inherit;
		else
			return;
	}
	else if (is_length(token) && pos == tokens.size())
	{
		CSSLength length;
		if (parse_length(token, length))
		{
			font_size->type = CSSValueFontSize::type_length;
			font_size->length = length;
		}
		else
		{
			return;
		}
	}
	else if (token.type == CSSToken::type_percentage && pos == tokens.size())
	{
		font_size->type = CSSValueFontSize::type_percentage;
		font_size->percentage = StringHelp::text_to_float(token.value);
	}
	else
	{
		return;
	}

	inout_values.push_back(std::move(font_size));
}

}
