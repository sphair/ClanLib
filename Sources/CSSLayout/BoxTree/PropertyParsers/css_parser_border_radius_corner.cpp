/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
#include "css_parser_border_radius_corner.h"
#include "API/CSSLayout/css_box_properties.h"

std::vector<CL_String> CL_CSSParserBorderRadiusCorner::get_names()
{
	std::vector<CL_String> names;
	names.push_back("border-top-right-radius");
	names.push_back("border-bottom-right-radius");
	names.push_back("border-bottom-left-radius");
	names.push_back("border-top-left-radius");
	return names;
}

void CL_CSSParserBorderRadiusCorner::parse(CL_CSSBoxProperties &properties, const CL_String &name, const std::vector<CL_CSSToken> &tokens, std::map<CL_String, CL_CSSBoxProperty *> *out_change_set)
{
	CL_CSSBoxBorderRadius *border_radius = 0;
	if (equals(name, "border-top-right-radius"))
		border_radius = &properties.border_radius_top_right;
	else if (equals(name, "border-bottom-right-radius"))
		border_radius = &properties.border_radius_bottom_right;
	else if (equals(name, "border-bottom-left-radius"))
		border_radius = &properties.border_radius_bottom_left;
	else if (equals(name, "border-top-left-radius"))
		border_radius = &properties.border_radius_top_left;

	if (border_radius)
	{
		CL_CSSBoxBorderRadius radius;

		size_t pos = 0;
		CL_CSSToken token = next_token(pos, tokens);
		if (token.type == CL_CSSToken::type_ident && equals(token.value, "inherit") && pos == tokens.size())
		{
			radius.type = CL_CSSBoxBorderRadius::type_inherit;
			*border_radius = radius;
			return;
		}
		else if (is_length(token))
		{
			if (!parse_length(token, radius.length1))
			{
				debug_parse_error(name, tokens);
				return;
			}
			radius.type = CL_CSSBoxBorderRadius::type_one_value;
			radius.value_type1 = CL_CSSBoxBorderRadius::value_type_length;
		}
		else if (token.type == CL_CSSToken::type_percentage)
		{
			radius.type = CL_CSSBoxBorderRadius::type_one_value;
			radius.value_type1 = CL_CSSBoxBorderRadius::value_type_percentage;
			radius.percentage1 = CL_StringHelp::text_to_float(token.value);
		}
		else
		{
			debug_parse_error(name, tokens);
			return;
		}

		if (pos != tokens.size())
		{
			token = next_token(pos, tokens);
			if (is_length(token) && pos == tokens.size() && parse_length(token, radius.length2))
			{
				radius.type = CL_CSSBoxBorderRadius::type_two_values;
				*border_radius = radius;
			}
			else if (token.type == CL_CSSToken::type_percentage)
			{
				radius.type = CL_CSSBoxBorderRadius::type_two_values;
				radius.value_type2 = CL_CSSBoxBorderRadius::value_type_percentage;
				radius.percentage2 = CL_StringHelp::text_to_float(token.value);
				*border_radius = radius;
			}
			else
			{
				debug_parse_error(name, tokens);
				return;
			}
		}
		else
		{
			*border_radius = radius;
		}

		if (out_change_set)
		{
			(*out_change_set)[name] = border_radius;
		}
	}
}
