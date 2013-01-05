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
#include "API/CSSLayout/css_box_properties.h"

namespace clan
{

std::vector<std::string> CSSParserFontWeight::get_names()
{
	std::vector<std::string> names;
	names.push_back("font-weight");
	return names;
}

void CSSParserFontWeight::parse(CSSBoxProperties &properties, const std::string &name, const std::vector<CSSToken> &tokens, std::map<std::string, CSSPropertyValue *> *out_change_set)
{
	size_t pos = 0;
	CSSToken token = next_token(pos, tokens);
	if (token.type == CSSToken::type_ident && pos == tokens.size())
	{
		if (equals(token.value, "normal"))
			properties.font_weight.type = CSSValueFontWeight::type_normal;
		else if (equals(token.value, "bold"))
			properties.font_weight.type = CSSValueFontWeight::type_bold;
		else if (equals(token.value, "bolder"))
			properties.font_weight.type = CSSValueFontWeight::type_bolder;
		else if (equals(token.value, "lighter"))
			properties.font_weight.type = CSSValueFontWeight::type_lighter;
		else if (equals(token.value, "inherit"))
			properties.font_weight.type = CSSValueFontWeight::type_inherit;
	}
	else if (token.type == CSSToken::type_number && pos == tokens.size())
	{
		if (token.value == "100")
			properties.font_weight.type = CSSValueFontWeight::type_100;
		else if (token.value == "200")
			properties.font_weight.type = CSSValueFontWeight::type_200;
		else if (token.value == "300")
			properties.font_weight.type = CSSValueFontWeight::type_300;
		else if (token.value == "400")
			properties.font_weight.type = CSSValueFontWeight::type_400;
		else if (token.value == "500")
			properties.font_weight.type = CSSValueFontWeight::type_500;
		else if (token.value == "600")
			properties.font_weight.type = CSSValueFontWeight::type_600;
		else if (token.value == "700")
			properties.font_weight.type = CSSValueFontWeight::type_700;
		else if (token.value == "800")
			properties.font_weight.type = CSSValueFontWeight::type_800;
		else if (token.value == "900")
			properties.font_weight.type = CSSValueFontWeight::type_900;
	}
	if (out_change_set)
	{
		(*out_change_set)["font-weight"] = &properties.font_weight;
	}
}

}
