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
#include "css_parser_font_weight.h"
#include "API/CSSLayout/PropertyValues/css_value_font_weight.h"

namespace clan
{

std::vector<std::string> CSSParserFontWeight::get_names()
{
	std::vector<std::string> names;
	names.push_back("font-weight");
	return names;
}

void CSSParserFontWeight::parse(const std::string &name, const std::vector<CSSToken> &tokens, std::vector<std::unique_ptr<CSSPropertyValue> > &inout_values)
{
	std::unique_ptr<CSSValueFontWeight> font_weight(new CSSValueFontWeight());

	size_t pos = 0;
	CSSToken token = next_token(pos, tokens);
	if (token.type == CSSToken::type_ident && pos == tokens.size())
	{
		if (equals(token.value, "normal"))
			font_weight->type = CSSValueFontWeight::type_normal;
		else if (equals(token.value, "bold"))
			font_weight->type = CSSValueFontWeight::type_bold;
		else if (equals(token.value, "bolder"))
			font_weight->type = CSSValueFontWeight::type_bolder;
		else if (equals(token.value, "lighter"))
			font_weight->type = CSSValueFontWeight::type_lighter;
		else if (equals(token.value, "inherit"))
			font_weight->type = CSSValueFontWeight::type_inherit;
		else
			return;
	}
	else if (token.type == CSSToken::type_number && pos == tokens.size())
	{
		if (token.value == "100")
			font_weight->type = CSSValueFontWeight::type_100;
		else if (token.value == "200")
			font_weight->type = CSSValueFontWeight::type_200;
		else if (token.value == "300")
			font_weight->type = CSSValueFontWeight::type_300;
		else if (token.value == "400")
			font_weight->type = CSSValueFontWeight::type_400;
		else if (token.value == "500")
			font_weight->type = CSSValueFontWeight::type_500;
		else if (token.value == "600")
			font_weight->type = CSSValueFontWeight::type_600;
		else if (token.value == "700")
			font_weight->type = CSSValueFontWeight::type_700;
		else if (token.value == "800")
			font_weight->type = CSSValueFontWeight::type_800;
		else if (token.value == "900")
			font_weight->type = CSSValueFontWeight::type_900;
		else
			return;
	}
	else
	{
		return;
	}

	inout_values.push_back(std::move(font_weight));
}

}
