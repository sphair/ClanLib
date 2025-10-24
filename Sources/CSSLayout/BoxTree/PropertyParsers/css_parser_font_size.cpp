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
#include "css_parser_font_size.h"
#include "../css_box_properties.h"

std::vector<CL_String> CL_CSSParserFontSize::get_names()
{
	std::vector<CL_String> names;
	names.push_back("font-size");
	return names;
}

void CL_CSSParserFontSize::parse(CL_CSSBoxProperties &properties, const CL_String &name, const std::vector<CL_CSSToken> &tokens)
{
	size_t pos = 0;
	CL_CSSToken token = next_token(pos, tokens);
	if (token.type == CL_CSSToken::type_ident && pos == tokens.size())
	{
		if (token.value == "xx-small")
			properties.font_size.type = CL_CSSBoxFontSize::type_xx_small;
		else if (token.value == "x-small")
			properties.font_size.type = CL_CSSBoxFontSize::type_x_small;
		else if (token.value == "small")
			properties.font_size.type = CL_CSSBoxFontSize::type_small;
		else if (token.value == "medium")
			properties.font_size.type = CL_CSSBoxFontSize::type_medium;
		else if (token.value == "large")
			properties.font_size.type = CL_CSSBoxFontSize::type_large;
		else if (token.value == "x-large")
			properties.font_size.type = CL_CSSBoxFontSize::type_x_large;
		else if (token.value == "xx-large")
			properties.font_size.type = CL_CSSBoxFontSize::type_xx_large;
		else if (token.value == "smaller")
			properties.font_size.type = CL_CSSBoxFontSize::type_smaller;
		else if (token.value == "larger")
			properties.font_size.type = CL_CSSBoxFontSize::type_larger;
		else if (token.value == "inherit")
			properties.font_size.type = CL_CSSBoxFontSize::type_inherit;
	}
	else if (is_length(token) && pos == tokens.size())
	{
		CL_CSSBoxLength length;
		if (parse_length(token, length))
		{
			properties.font_size.type = CL_CSSBoxFontSize::type_length;
			properties.font_size.length = length;
		}
	}
	else if (token.type == CL_CSSToken::type_percentage && pos == tokens.size())
	{
		properties.font_size.type = CL_CSSBoxFontSize::type_percentage;
		properties.font_size.percentage = CL_StringHelp::text_to_float(token.value);
	}
}
