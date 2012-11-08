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
#include "css_parser_clan_box_sizing_factor.h"
#include "API/CSSLayout/css_box_properties.h"

namespace clan
{

std::vector<std::string> CSSParserClanBoxSizingFactor::get_names()
{
	std::vector<std::string> names;
	names.push_back("-clan-box-width-expand-factor");
	names.push_back("-clan-box-width-shrink-factor");
	names.push_back("-clan-box-height-expand-factor");
	names.push_back("-clan-box-height-shrink-factor");
	return names;
}

void CSSParserClanBoxSizingFactor::parse(CSSBoxProperties &properties, const std::string &name, const std::vector<CSSToken> &tokens, std::map<std::string, CSSBoxProperty *> *out_change_set)
{
	CSSBoxClanBoxSizingFactor *sizing_prop = 0;
	if (equals(name, "-clan-box-width-expand-factor"))
	{
		sizing_prop = &properties.clan_box_width_expand_factor;
	}
	else if (equals(name, "-clan-box-width-shrink-factor"))
	{
		sizing_prop = &properties.clan_box_width_shrink_factor;
	}
	else if (equals(name, "-clan-box-height-expand-factor"))
	{
		sizing_prop = &properties.clan_box_height_expand_factor;
	}
	else if (equals(name, "-clan-box-height-shrink-factor"))
	{
		sizing_prop = &properties.clan_box_height_shrink_factor;
	}
	else
	{
		debug_parse_error(name, tokens);
		return;
	}

	size_t pos = 0;
	CSSToken token = next_token(pos, tokens);
	if (token.type == CSSToken::type_ident && pos == tokens.size())
	{
		if (equals(token.value, "auto"))
			sizing_prop->type = CSSBoxClanBoxSizingFactor::type_auto;
		else if (equals(token.value, "inherit"))
			sizing_prop->type = CSSBoxClanBoxSizingFactor::type_inherit;
	}
	else if (token.type == CSSToken::type_number && pos == tokens.size())
	{
		sizing_prop->type = CSSBoxClanBoxSizingFactor::type_number;
		sizing_prop->number = StringHelp::text_to_float(token.value);
	}

	if (out_change_set)
	{
		(*out_change_set)[name] = &properties.direction;
	}
}

}
