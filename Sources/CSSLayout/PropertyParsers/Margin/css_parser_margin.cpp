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
#include "css_parser_margin.h"
#include "API/CSSLayout/css_box_properties.h"

namespace clan
{

std::vector<std::string> CSSParserMargin::get_names()
{
	std::vector<std::string> names;
	names.push_back("margin");
	return names;
}

void CSSParserMargin::parse(CSSBoxProperties &properties, const std::string &name, const std::vector<CSSToken> &tokens, std::map<std::string, CSSBoxProperty *> *out_change_set)
{
	CSSBoxMarginWidth margin_widths[4];
	int count;
	size_t pos = 0;
	for (count = 0; count < 4; count++)
	{
		CSSToken token = next_token(pos, tokens);
		if (token.type == CSSToken::type_ident && equals(token.value, "auto"))
		{
			margin_widths[count].type = CSSBoxMarginWidth::type_auto;
		}
		else if (token.type == CSSToken::type_ident && equals(token.value, "inherit") && count == 0 && pos == tokens.size())
		{
			properties.margin_width_left.type = CSSBoxMarginWidth::type_inherit;
			properties.margin_width_top.type = CSSBoxMarginWidth::type_inherit;
			properties.margin_width_right.type = CSSBoxMarginWidth::type_inherit;
			properties.margin_width_bottom.type = CSSBoxMarginWidth::type_inherit;
			return;
		}
		else if (token.type == CSSToken::type_number && equals(token.value, "0"))
		{
			margin_widths[count].type = CSSBoxMarginWidth::type_length;
			margin_widths[count].length = CSSBoxLength(0.0f, CSSBoxLength::type_px);
		}
		else if (is_length(token))
		{
			CSSBoxLength length;
			if (parse_length(token, length))
			{
				margin_widths[count].type = CSSBoxMarginWidth::type_length;
				margin_widths[count].length = length;
			}
			else
			{
				debug_parse_error(name, tokens);
				return;
			}
		}
		else if (token.type == CSSToken::type_percentage)
		{
			margin_widths[count].type = CSSBoxMarginWidth::type_percentage;
			margin_widths[count].percentage = StringHelp::text_to_float(token.value);
		}
		else if (token.type == CSSToken::type_delim && token.value == "-")
		{
			token = next_token(pos, tokens);
			if (is_length(token))
			{
				CSSBoxLength length;
				if (parse_length(token, length))
				{
					length.value = -length.value;
					margin_widths[count].type = CSSBoxMarginWidth::type_length;
					margin_widths[count].length = length;
				}
				else
				{
					debug_parse_error(name, tokens);
					return;
				}
			}
			else if (token.type == CSSToken::type_percentage)
			{
				margin_widths[count].type = CSSBoxMarginWidth::type_percentage;
				margin_widths[count].percentage = -StringHelp::text_to_float(token.value);
			}
			else
			{
				debug_parse_error(name, tokens);
				return;
			}
		}
		else if (token.type == CSSToken::type_null)
		{
			break;
		}
		else
		{
			debug_parse_error(name, tokens);
			return;
		}
	}

	if (pos == tokens.size())
	{
		switch (count)
		{
		case 1:
			properties.margin_width_left = margin_widths[0];
			properties.margin_width_top = margin_widths[0];
			properties.margin_width_right = margin_widths[0];
			properties.margin_width_bottom = margin_widths[0];
			break;
		case 2:
			properties.margin_width_top = margin_widths[0];
			properties.margin_width_bottom = margin_widths[0];
			properties.margin_width_left = margin_widths[1];
			properties.margin_width_right = margin_widths[1];
			break;
		case 3:
			properties.margin_width_top = margin_widths[0];
			properties.margin_width_left = margin_widths[1];
			properties.margin_width_right = margin_widths[1];
			properties.margin_width_bottom = margin_widths[2];
			break;
		case 4:
			properties.margin_width_top = margin_widths[0];
			properties.margin_width_right = margin_widths[1];
			properties.margin_width_bottom = margin_widths[2];
			properties.margin_width_left = margin_widths[3];
			break;
		default:
			break;
		}
	}
	if (out_change_set)
	{
		(*out_change_set)["margin-left"] = &properties.margin_width_left;
		(*out_change_set)["margin-right"] = &properties.margin_width_right;
		(*out_change_set)["margin-top"] = &properties.margin_width_top;
		(*out_change_set)["margin-bottom"] = &properties.margin_width_bottom;
	}
}

}
