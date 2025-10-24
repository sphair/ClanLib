/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
#include "../css_box_properties.h"

std::vector<CL_String> CL_CSSParserListStyleType::get_names()
{
	std::vector<CL_String> names;
	names.push_back("list-style-type");
	return names;
}

void CL_CSSParserListStyleType::parse(CL_CSSBoxProperties &properties, const CL_String &name, const std::vector<CL_CSSToken> &tokens)
{
	size_t pos = 0;
	CL_CSSToken token = next_token(pos, tokens);
	if (token.type == CL_CSSToken::type_ident && pos == tokens.size())
	{
		if (token.value == "disc")
			properties.list_style_type.type = CL_CSSBoxListStyleType::type_disc;
		else if (token.value == "circle")
			properties.list_style_type.type = CL_CSSBoxListStyleType::type_circle;
		else if (token.value == "square")
			properties.list_style_type.type = CL_CSSBoxListStyleType::type_square;
		else if (token.value == "decimal")
			properties.list_style_type.type = CL_CSSBoxListStyleType::type_decimal;
		else if (token.value == "decimal-leading-zero")
			properties.list_style_type.type = CL_CSSBoxListStyleType::type_decimal_leading_zero;
		else if (token.value == "lower-roman")
			properties.list_style_type.type = CL_CSSBoxListStyleType::type_lower_roman;
		else if (token.value == "upper-roman")
			properties.list_style_type.type = CL_CSSBoxListStyleType::type_upper_roman;
		else if (token.value == "lower-greek")
			properties.list_style_type.type = CL_CSSBoxListStyleType::type_lower_greek;
		else if (token.value == "lower-latin")
			properties.list_style_type.type = CL_CSSBoxListStyleType::type_lower_latin;
		else if (token.value == "upper-latin")
			properties.list_style_type.type = CL_CSSBoxListStyleType::type_upper_latin;
		else if (token.value == "armenian")
			properties.list_style_type.type = CL_CSSBoxListStyleType::type_armenian;
		else if (token.value == "georgian")
			properties.list_style_type.type = CL_CSSBoxListStyleType::type_georgian;
		else if (token.value == "lower-alpha")
			properties.list_style_type.type = CL_CSSBoxListStyleType::type_lower_alpha;
		else if (token.value == "upper-alpha")
			properties.list_style_type.type = CL_CSSBoxListStyleType::type_upper_alpha;
		else if (token.value == "none")
			properties.list_style_type.type = CL_CSSBoxListStyleType::type_none;
		else if (token.value == "inherit")
			properties.list_style_type.type = CL_CSSBoxListStyleType::type_inherit;
	}
}
