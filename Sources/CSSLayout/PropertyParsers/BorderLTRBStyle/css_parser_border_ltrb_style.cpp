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
#include "css_parser_border_ltrb_style.h"
#include "API/CSSLayout/css_box_properties.h"

namespace clan
{

std::vector<std::string> CSSParserBorderLTRBStyle::get_names()
{
	std::vector<std::string> names;
	names.push_back("border-left-style");
	names.push_back("border-top-style");
	names.push_back("border-right-style");
	names.push_back("border-bottom-style");
	return names;
}

void CSSParserBorderLTRBStyle::parse(const std::string &name, const std::vector<CSSToken> &tokens, std::vector<std::unique_ptr<CSSPropertyValue> > &inout_values)
{
	std::unique_ptr<CSSValueBorderStyle> style(new CSSValueBorderStyle());

	if (equals(name, "border-top-style"))
		style->value_type = CSSValueBorderStyle::top_value;
	else if (equals(name, "border-right-style"))
		style->value_type = CSSValueBorderStyle::right_value;
	else if (equals(name, "border-bottom-style"))
		style->value_type = CSSValueBorderStyle::bottom_value;
	else if (equals(name, "border-left-style"))
		style->value_type = CSSValueBorderStyle::left_value;

	if (style)
	{
		size_t pos = 0;
		CSSToken token = next_token(pos, tokens);
		if (token.type == CSSToken::type_ident && pos == tokens.size())
		{
			if (equals(token.value, "none"))
				style->type = CSSValueBorderStyle::type_none;
			else if (equals(token.value, "hidden"))
				style->type = CSSValueBorderStyle::type_hidden;
			else if (equals(token.value, "dotted"))
				style->type = CSSValueBorderStyle::type_dotted;
			else if (equals(token.value, "dashed"))
				style->type = CSSValueBorderStyle::type_dashed;
			else if (equals(token.value, "solid"))
				style->type = CSSValueBorderStyle::type_solid;
			else if (equals(token.value, "double"))
				style->type = CSSValueBorderStyle::type_double;
			else if (equals(token.value, "groove"))
				style->type = CSSValueBorderStyle::type_groove;
			else if (equals(token.value, "ridge"))
				style->type = CSSValueBorderStyle::type_ridge;
			else if (equals(token.value, "inset"))
				style->type = CSSValueBorderStyle::type_inset;
			else if (equals(token.value, "outset"))
				style->type = CSSValueBorderStyle::type_outset;
			else if (equals(token.value, "inherit"))
				style->type = CSSValueBorderStyle::type_inherit;
			else
				return;

			inout_values.push_back(std::move(style));
		}
	}
}

}
