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
#include "css_parser_outline_style.h"
#include "API/CSSLayout/PropertyValues/css_value_outline_style.h"

namespace clan
{

std::vector<std::string> CSSParserOutlineStyle::get_names()
{
	std::vector<std::string> names;
	names.push_back("outline-style");
	return names;
}

void CSSParserOutlineStyle::parse(const std::string &name, const std::vector<CSSToken> &tokens, std::vector<std::unique_ptr<CSSPropertyValue> > &inout_values)
{
	std::unique_ptr<CSSValueOutlineStyle> outline_style(new CSSValueOutlineStyle());

	size_t pos = 0;
	CSSToken token = next_token(pos, tokens);
	if (token.type == CSSToken::type_ident && pos == tokens.size())
	{
		if (equals(token.value, "none"))
			outline_style->type = CSSValueOutlineStyle::type_none;
		else if (equals(token.value, "hidden"))
			outline_style->type = CSSValueOutlineStyle::type_hidden;
		else if (equals(token.value, "dotted"))
			outline_style->type = CSSValueOutlineStyle::type_dotted;
		else if (equals(token.value, "dashed"))
			outline_style->type = CSSValueOutlineStyle::type_dashed;
		else if (equals(token.value, "solid"))
			outline_style->type = CSSValueOutlineStyle::type_solid;
		else if (equals(token.value, "double"))
			outline_style->type = CSSValueOutlineStyle::type_double;
		else if (equals(token.value, "groove"))
			outline_style->type = CSSValueOutlineStyle::type_groove;
		else if (equals(token.value, "ridge"))
			outline_style->type = CSSValueOutlineStyle::type_ridge;
		else if (equals(token.value, "inset"))
			outline_style->type = CSSValueOutlineStyle::type_inset;
		else if (equals(token.value, "outset"))
			outline_style->type = CSSValueOutlineStyle::type_outset;
		else if (equals(token.value, "inherit"))
			outline_style->type = CSSValueOutlineStyle::type_inherit;
		else
			return;
	}
	else
	{
		return;
	}

	inout_values.push_back(std::move(outline_style));
}

}
