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

void CSSParserBorderLTRBStyle::parse(CSSBoxProperties &properties, const std::string &name, const std::vector<CSSToken> &tokens, std::map<std::string, CSSBoxProperty *> *out_change_set)
{
	CSSBoxBorderStyle *style = 0;
	if (equals(name, "border-top-style"))
		style = &properties.border_style_top;
	else if (equals(name, "border-right-style"))
		style = &properties.border_style_right;
	else if (equals(name, "border-bottom-style"))
		style = &properties.border_style_bottom;
	else if (equals(name, "border-left-style"))
		style = &properties.border_style_left;

	if (style)
	{
		size_t pos = 0;
		CSSToken token = next_token(pos, tokens);
		if (token.type == CSSToken::type_ident && pos == tokens.size())
		{
			if (equals(token.value, "none"))
				style->type = CSSBoxBorderStyle::type_none;
			else if (equals(token.value, "hidden"))
				style->type = CSSBoxBorderStyle::type_hidden;
			else if (equals(token.value, "dotted"))
				style->type = CSSBoxBorderStyle::type_dotted;
			else if (equals(token.value, "dashed"))
				style->type = CSSBoxBorderStyle::type_dashed;
			else if (equals(token.value, "solid"))
				style->type = CSSBoxBorderStyle::type_solid;
			else if (equals(token.value, "double"))
				style->type = CSSBoxBorderStyle::type_double;
			else if (equals(token.value, "groove"))
				style->type = CSSBoxBorderStyle::type_groove;
			else if (equals(token.value, "ridge"))
				style->type = CSSBoxBorderStyle::type_ridge;
			else if (equals(token.value, "inset"))
				style->type = CSSBoxBorderStyle::type_inset;
			else if (equals(token.value, "outset"))
				style->type = CSSBoxBorderStyle::type_outset;
			else if (equals(token.value, "inherit"))
				style->type = CSSBoxBorderStyle::type_inherit;
		}

		if (out_change_set)
		{
			(*out_change_set)[name] = style;
		}
	}
}

}
