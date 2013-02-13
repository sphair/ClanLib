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
#include "css_parser_border_ltrb_width.h"
#include "API/CSSLayout/PropertyValues/css_value_border_width.h"

namespace clan
{

std::vector<std::string> CSSParserBorderLTRBWidth::get_names()
{
	std::vector<std::string> names;
	names.push_back("border-left-width");
	names.push_back("border-top-width");
	names.push_back("border-right-width");
	names.push_back("border-bottom-width");
	return names;
}

void CSSParserBorderLTRBWidth::parse(const std::string &name, const std::vector<CSSToken> &tokens, std::vector<std::unique_ptr<CSSPropertyValue> > &inout_values)
{
	std::unique_ptr<CSSValueBorderWidth> width(new CSSValueBorderWidth());
	if (equals(name, "border-top-width"))
		width->value_type = CSSValueBorderWidth::top_value;
	else if (equals(name, "border-right-width"))
		width->value_type = CSSValueBorderWidth::right_value;
	else if (equals(name, "border-bottom-width"))
		width->value_type = CSSValueBorderWidth::bottom_value;
	else if (equals(name, "border-left-width"))
		width->value_type = CSSValueBorderWidth::left_value;

	if (width)
	{
		size_t pos = 0;
		CSSToken token = next_token(pos, tokens);
		if (token.type == CSSToken::type_ident && pos == tokens.size())
		{
			if (equals(token.value, "thin"))
				width->type = CSSValueBorderWidth::type_thin;
			else if (equals(token.value, "medium"))
				width->type = CSSValueBorderWidth::type_medium;
			else if (equals(token.value, "thick"))
				width->type = CSSValueBorderWidth::type_thick;
			else if (equals(token.value, "inherit"))
				width->type = CSSValueBorderWidth::type_inherit;
			else
				return;

			inout_values.push_back(std::move(width));
		}
		else if (is_length(token) && pos == tokens.size())
		{
			CSSLength length;
			if (parse_length(token, length))
			{
				width->type = CSSValueBorderWidth::type_length;
				width->length = length;

				inout_values.push_back(std::move(width));
			}
		}
	}
}

}
