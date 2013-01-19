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
#include "css_parser_border_radius.h"
#include "API/CSSLayout/ComputedValues/css_computed_box.h"

namespace clan
{

std::vector<std::string> CSSParserBorderRadius::get_names()
{
	std::vector<std::string> names;
	names.push_back("border-radius");
	return names;
}

void CSSParserBorderRadius::parse(const std::string &name, const std::vector<CSSToken> &tokens, std::vector<std::unique_ptr<CSSPropertyValue> > &inout_values)
{
	CSSValueBorderRadius radius[4];

	size_t pos = 0;
	CSSToken token = next_token(pos, tokens);
	if (token.type == CSSToken::type_ident && equals(token.value, "inherit") && pos == tokens.size())
	{
		radius[0].type = CSSValueBorderRadius::type_inherit;
		inout_values.push_back(std::unique_ptr<CSSValueBorderRadius>(new CSSValueBorderRadius(CSSValueBorderRadius::top_left_value, radius[0])));
		inout_values.push_back(std::unique_ptr<CSSValueBorderRadius>(new CSSValueBorderRadius(CSSValueBorderRadius::top_right_value, radius[0])));
		inout_values.push_back(std::unique_ptr<CSSValueBorderRadius>(new CSSValueBorderRadius(CSSValueBorderRadius::bottom_left_value, radius[0])));
		inout_values.push_back(std::unique_ptr<CSSValueBorderRadius>(new CSSValueBorderRadius(CSSValueBorderRadius::bottom_right_value, radius[0])));
		return;
	}

	int num_x_values = 0;
	int num_y_values = 0;
	for (int i = 0; i < 4; i++)
	{
		if (is_length(token))
		{
			if (!parse_length(token, radius[i].length1))
			{
				debug_parse_error(name, tokens);
				return;
			}
			radius[i].type = CSSValueBorderRadius::type_one_value;
			radius[i].value_type1 = CSSValueBorderRadius::value_type_length;
		}
		else if (token.type == CSSToken::type_percentage)
		{
			radius[i].type = CSSValueBorderRadius::type_one_value;
			radius[i].value_type1 = CSSValueBorderRadius::value_type_percentage;
			radius[i].percentage1 = StringHelp::text_to_float(token.value);
		}
		else
		{
			debug_parse_error(name, tokens);
			return;
		}

		num_x_values = i+1;

		token = next_token(pos, tokens);
		if (token.type == CSSToken::type_null)
			break;
		else if (i != 0 && token.type == CSSToken::type_delim && token.value == "/")
			break;
	}

	if (token.type == CSSToken::type_delim && token.value == "/")
	{
		token = next_token(pos, tokens);
		for (int i = 0; i < 4; i++)
		{
			if (is_length(token) && parse_length(token, radius[i].length2))
			{
				radius[i].type = CSSValueBorderRadius::type_two_values;
			}
			else if (token.type == CSSToken::type_percentage)
			{
				radius[i].type = CSSValueBorderRadius::type_two_values;
				radius[i].value_type2 = CSSValueBorderRadius::value_type_percentage;
				radius[i].percentage2 = StringHelp::text_to_float(token.value);
			}
			else
			{
				debug_parse_error(name, tokens);
				return;
			}

			num_y_values = i+1;

			token = next_token(pos, tokens);
			if (token.type == CSSToken::type_null)
				break;
		}
	}
	else if (token.type != CSSToken::type_null)
	{
		debug_parse_error(name, tokens);
		return;
	}

	if (num_y_values > 0)
	{
		if (num_x_values < num_y_values)
		{
			for (int i = num_x_values; i < num_y_values; i++)
			{
				radius[i].type = CSSValueBorderRadius::type_two_values;
				radius[i].value_type1 = radius[i].value_type2;
				radius[i].length1 = radius[i].length2;
				radius[i].percentage1 = radius[i].percentage2;
			}
			num_x_values = num_y_values;
		}
		else if (num_y_values < num_x_values)
		{
			for (int i = num_y_values; i < num_x_values; i++)
			{
				radius[i].type = CSSValueBorderRadius::type_two_values;
				radius[i].value_type2 = radius[i].value_type1;
				radius[i].length2 = radius[i].length1;
				radius[i].percentage2 = radius[i].percentage1;
			}
			num_y_values = num_x_values;
		}
	}

	for (int i = num_x_values; i < 4; i++)
		radius[i] = radius[i-1];

	inout_values.push_back(std::unique_ptr<CSSValueBorderRadius>(new CSSValueBorderRadius(CSSValueBorderRadius::top_right_value, radius[0])));
	inout_values.push_back(std::unique_ptr<CSSValueBorderRadius>(new CSSValueBorderRadius(CSSValueBorderRadius::bottom_left_value, radius[1])));
	inout_values.push_back(std::unique_ptr<CSSValueBorderRadius>(new CSSValueBorderRadius(CSSValueBorderRadius::bottom_right_value, radius[2])));
	inout_values.push_back(std::unique_ptr<CSSValueBorderRadius>(new CSSValueBorderRadius(CSSValueBorderRadius::top_left_value, radius[3])));
}

}
