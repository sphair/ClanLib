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
#include "css_parser_flex.h"
#include "API/CSSLayout/PropertyValues/css_value_flex_grow.h"
#include "API/CSSLayout/PropertyValues/css_value_flex_shrink.h"
#include "API/CSSLayout/PropertyValues/css_value_flex_basis.h"

namespace clan
{

std::vector<std::string> CSSParserFlex::get_names()
{
	std::vector<std::string> names;
	names.push_back("flex");
	return names;
}

void CSSParserFlex::parse(const std::string &name, const std::vector<CSSToken> &tokens, std::vector<std::unique_ptr<CSSPropertyValue> > &inout_values)
{
	std::unique_ptr<CSSValueFlexGrow> flex_grow(new CSSValueFlexGrow());
	std::unique_ptr<CSSValueFlexShrink> flex_shrink(new CSSValueFlexShrink());
	std::unique_ptr<CSSValueFlexBasis> flex_basis(new CSSValueFlexBasis());

	size_t pos = 0;
	CSSToken token = next_token(pos, tokens);
	if (token.type == CSSToken::type_ident && pos == tokens.size())
	{
		if (equals(token.value, "none"))
		{
			flex_grow->type = CSSValueFlexGrow::type_number;
			flex_grow->number = 0.0f;
			flex_shrink->type = CSSValueFlexShrink::type_number;
			flex_shrink->number = 0.0f;
			flex_basis->type = CSSValueFlexBasis::type_auto;

			inout_values.push_back(std::move(flex_grow));
			inout_values.push_back(std::move(flex_shrink));
			inout_values.push_back(std::move(flex_basis));
		}
		else if (equals(token.value, "inherit"))
		{
			flex_grow->type = CSSValueFlexGrow::type_inherit;
			flex_shrink->type = CSSValueFlexShrink::type_inherit;
			flex_basis->type = CSSValueFlexBasis::type_inherit;
			inout_values.push_back(std::move(flex_grow));
			inout_values.push_back(std::move(flex_shrink));
			inout_values.push_back(std::move(flex_basis));
		}
		else
		{
			return;
		}
	}
	else
	{
		pos = 0;

		flex_grow->type = CSSValueFlexGrow::type_number;
		flex_grow->number = 1.0f;
		flex_shrink->type = CSSValueFlexShrink::type_number;
		flex_shrink->number = 1.0f;
		flex_basis->type = CSSValueFlexBasis::type_length;
		flex_basis->length = CSSLength(0.0f, CSSLength::type_px);

		bool grow_shrink_specified = false;
		bool basis_specified = false;

		do
		{
			if (!grow_shrink_specified && parse_grow_shrink(*flex_grow.get(), *flex_shrink.get(), pos, tokens))
			{
				grow_shrink_specified = true;
			}
			else if (!basis_specified && parse_basis(*flex_basis.get(), pos, tokens))
			{
				basis_specified = true;
			}
			else
			{
				return;
			}
		} while (pos != tokens.size());

		inout_values.push_back(std::move(flex_grow));
		inout_values.push_back(std::move(flex_shrink));
		inout_values.push_back(std::move(flex_basis));
	}
}

bool CSSParserFlex::parse_grow_shrink(CSSValueFlexGrow &grow, CSSValueFlexShrink &shrink, size_t &parse_pos, const std::vector<CSSToken> &tokens)
{
	size_t pos = parse_pos;
	CSSToken token = next_token(pos, tokens);

	if (token.type == CSSToken::type_number)
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

		if (token.type == CSSToken::type_number)
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
		else
			return false;
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
