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
#include "css_parser_border_ltrb_style.h"
#include "../css_box_properties.h"

std::vector<CL_String> CL_CSSParserBorderLTRBStyle::get_names()
{
	std::vector<CL_String> names;
	names.push_back("border-left-style");
	names.push_back("border-top-style");
	names.push_back("border-right-style");
	names.push_back("border-bottom-style");
	return names;
}

void CL_CSSParserBorderLTRBStyle::parse(CL_CSSBoxProperties &properties, const CL_String &name, const std::vector<CL_CSSToken> &tokens)
{
	CL_CSSBoxBorderStyle *style = 0;
	if (name == "border-top-style")
		style = &properties.border_style_top;
	else if (name == "border-right-style")
		style = &properties.border_style_right;
	else if (name == "border-bottom-style")
		style = &properties.border_style_bottom;
	else if (name == "border-left-style")
		style = &properties.border_style_left;

	if (style)
	{
		size_t pos = 0;
		CL_CSSToken token = next_token(pos, tokens);
		if (token.type == CL_CSSToken::type_ident && pos == tokens.size())
		{
			if (token.value == "none")
				style->type = CL_CSSBoxBorderStyle::type_none;
			else if (token.value == "hidden")
				style->type = CL_CSSBoxBorderStyle::type_hidden;
			else if (token.value == "dotted")
				style->type = CL_CSSBoxBorderStyle::type_dotted;
			else if (token.value == "dashed")
				style->type = CL_CSSBoxBorderStyle::type_dashed;
			else if (token.value == "solid")
				style->type = CL_CSSBoxBorderStyle::type_solid;
			else if (token.value == "double")
				style->type = CL_CSSBoxBorderStyle::type_double;
			else if (token.value == "groove")
				style->type = CL_CSSBoxBorderStyle::type_groove;
			else if (token.value == "ridge")
				style->type = CL_CSSBoxBorderStyle::type_ridge;
			else if (token.value == "inset")
				style->type = CL_CSSBoxBorderStyle::type_inset;
			else if (token.value == "outset")
				style->type = CL_CSSBoxBorderStyle::type_outset;
			else if (token.value == "inherit")
				style->type = CL_CSSBoxBorderStyle::type_inherit;
		}
	}
}
