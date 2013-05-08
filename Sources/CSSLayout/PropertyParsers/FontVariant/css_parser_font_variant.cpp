/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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
#include "css_parser_font_variant.h"
#include "API/CSSLayout/PropertyValues/css_value_font_variant.h"

namespace clan
{

std::vector<std::string> CSSParserFontVariant::get_names()
{
	std::vector<std::string> names;
	names.push_back("font-variant");
	return names;
}

void CSSParserFontVariant::parse(const std::string &name, const std::vector<CSSToken> &tokens, std::vector<std::unique_ptr<CSSPropertyValue> > &inout_values)
{
	std::unique_ptr<CSSValueFontVariant> font_variant(new CSSValueFontVariant());

	size_t pos = 0;
	CSSToken token = next_token(pos, tokens);
	if (token.type == CSSToken::type_ident && pos == tokens.size())
	{
		if (equals(token.value, "normal"))
			font_variant->type = CSSValueFontVariant::type_normal;
		else if (equals(token.value, "small-caps"))
			font_variant->type = CSSValueFontVariant::type_small_caps;
		else if (equals(token.value, "inherit"))
			font_variant->type = CSSValueFontVariant::type_inherit;
		else
			return;
	}
	else
	{
		return;
	}

	inout_values.push_back(std::move(font_variant));
}

}
