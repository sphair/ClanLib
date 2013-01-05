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
#include "css_parser_outline_color.h"
#include "API/CSSLayout/css_box_properties.h"

namespace clan
{

std::vector<std::string> CSSParserOutlineColor::get_names()
{
	std::vector<std::string> names;
	names.push_back("outline-color");
	return names;
}

void CSSParserOutlineColor::parse(CSSBoxProperties &properties, const std::string &name, const std::vector<CSSToken> &tokens, std::map<std::string, CSSPropertyValue *> *out_change_set)
{
	size_t pos = 0;
	Colorf color;
	if (parse_color(tokens, pos, color) && pos == tokens.size())
	{
		properties.outline_color.type = CSSValueOutlineColor::type_color;
		properties.outline_color.color = color;
	}
	else
	{
		CSSToken token = next_token(pos, tokens);
		if (token.type == CSSToken::type_ident && pos == tokens.size())
		{
			if (equals(token.value, "invert"))
			{
				properties.outline_color.type = CSSValueOutlineColor::type_invert;
			}
			else if (equals(token.value, "inherit"))
			{
				properties.outline_color.type = CSSValueOutlineColor::type_inherit;
			}
		}
	}
	if (out_change_set)
	{
		(*out_change_set)["outline-color"] = &properties.outline_color;
	}
}

}
