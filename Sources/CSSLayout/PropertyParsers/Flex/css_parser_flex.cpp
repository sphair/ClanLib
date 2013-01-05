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
#include "css_parser_flex.h"
#include "API/CSSLayout/css_box_properties.h"

namespace clan
{

std::vector<std::string> CSSParserFlex::get_names()
{
	std::vector<std::string> names;
	names.push_back("flex");
	return names;
}

void CSSParserFlex::parse(CSSBoxProperties &properties, const std::string &name, const std::vector<CSSToken> &tokens, std::map<std::string, CSSPropertyValue *> *out_change_set)
{
	size_t pos = 0;
	CSSToken token = next_token(pos, tokens);
	if (token.type == CSSToken::type_ident && pos == tokens.size())
	{
		if (equals(token.value, "none"))
		{
			properties.flex_grow.type = CSSValueFlexGrow::type_number;
			properties.flex_grow.number = 0.0f;
			properties.flex_shrink.type = CSSValueFlexShrink::type_number;
			properties.flex_shrink.number = 0.0f;
			properties.flex_basis.type = CSSValueFlexBasis::type_auto;
		}
	}
	else
	{
		CSSValueFlexGrow grow;
		CSSValueFlexShrink shrink;
		CSSValueFlexBasis basis;

		grow.number = 1.0f;
		shrink.number = 1.0f;
		basis.type = CSSValueFlexBasis::type_length;
		basis.length = CSSLength(0.0f, CSSLength::type_px);

		bool grow_shrink_specified = false;
		bool basis_specified = false;

		do
		{
			if (!grow_shrink_specified && parse_grow_shrink(grow, shrink, pos, tokens))
			{
				grow_shrink_specified = true;
			}
			else if (!basis_specified && parse_basis(basis, pos, tokens))
			{
				basis_specified = true;
			}
			else
			{
				return;
			}
		} while (pos != tokens.size());

		properties.flex_grow = grow;
		properties.flex_shrink = shrink;
		properties.flex_basis = basis;
	}
}

bool CSSParserFlex::parse_grow_shrink(CSSValueFlexGrow &grow, CSSValueFlexShrink &shrink, size_t &parse_pos, const std::vector<CSSToken> &tokens)
{
	size_t pos = parse_pos;
	CSSToken token = next_token(pos, tokens);

	if (token.type == CSSToken::type_ident)
	{
		if (equals(token.value, "inherit"))
			grow.type = CSSValueFlexGrow::type_inherit;
	}
	else if (token.type == CSSToken::type_number)
	{
		grow.type = CSSValueFlexGrow::type_number;
		grow.number = StringHelp::text_to_float(token.value);
	}
	else
	{
		return false;
	}

	parse_pos = pos;

	if (pos != tokens.size())
	{
		token = next_token(pos, tokens);

		if (token.type == CSSToken::type_ident)
		{
			if (equals(token.value, "inherit"))
				shrink.type = CSSValueFlexShrink::type_inherit;

			parse_pos = pos;
		}
		else if (token.type == CSSToken::type_number)
		{
			shrink.type = CSSValueFlexShrink::type_number;
			shrink.number = StringHelp::text_to_float(token.value);

			parse_pos = pos;
		}
	}

	return true;
}

bool CSSParserFlex::parse_basis(CSSValueFlexBasis &basis, size_t &parse_pos, const std::vector<CSSToken> &tokens)
{
	size_t pos = parse_pos;
	CSSToken token = next_token(pos, tokens);

	if (token.type == CSSToken::type_ident)
	{
		if (equals(token.value, "auto"))
			basis.type = CSSValueFlexBasis::type_auto;
		else if (equals(token.value, "inherit"))
			basis.type = CSSValueFlexBasis::type_inherit;
	}
	else if (is_length(token))
	{
		CSSLength length;
		if (parse_length(token, length) && length.value >= 0.0f)
		{
			basis.type = CSSValueFlexBasis::type_length;
			basis.length = length;
		}
	}
	else if (token.type == CSSToken::type_percentage)
	{
		float v = StringHelp::text_to_float(token.value);
		if (v >= 0.0f)
		{
			basis.type = CSSValueFlexBasis::type_percentage;
			basis.percentage = v;
		}
	}
	else
	{
		return false;
	}

	parse_pos = pos;
	return true;
}


}
