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
#include "API/CSSLayout/PropertyValues/css_value_margin_width.h"

namespace clan
{

std::vector<std::string> CSSParserMargin::get_names()
{
	std::vector<std::string> names;
	names.push_back("margin");
	return names;
}

void CSSParserMargin::parse(const std::string &name, const std::vector<CSSToken> &tokens, std::vector<std::unique_ptr<CSSPropertyValue> > &inout_values)
{
	CSSValueMarginWidth margin_widths[4];
	int count;
	size_t pos = 0;
	for (count = 0; count < 4; count++)
	{
		CSSToken token = next_token(pos, tokens);
		if (token.type == CSSToken::type_ident && equals(token.value, "auto"))
		{
			margin_widths[count].type = CSSValueMarginWidth::type_auto;
		}
		else if (token.type == CSSToken::type_ident && equals(token.value, "inherit") && count == 0 && pos == tokens.size())
		{
			margin_widths[0].type = CSSValueMarginWidth::type_inherit;
			inout_values.push_back(std::unique_ptr<CSSValueMarginWidth>(new CSSValueMarginWidth(CSSValueMarginWidth::left_value, margin_widths[0])));
			inout_values.push_back(std::unique_ptr<CSSValueMarginWidth>(new CSSValueMarginWidth(CSSValueMarginWidth::top_value, margin_widths[0])));
			inout_values.push_back(std::unique_ptr<CSSValueMarginWidth>(new CSSValueMarginWidth(CSSValueMarginWidth::right_value, margin_widths[0])));
			inout_values.push_back(std::unique_ptr<CSSValueMarginWidth>(new CSSValueMarginWidth(CSSValueMarginWidth::bottom_value, margin_widths[0])));
			return;
		}
		else if (token.type == CSSToken::type_number && equals(token.value, "0"))
		{
			margin_widths[count].type = CSSValueMarginWidth::type_length;
			margin_widths[count].length = CSSLength(0.0f, CSSLength::type_px);
		}
		else if (is_length(token))
		{
			CSSLength length;
			if (parse_length(token, length))
			{
				margin_widths[count].type = CSSValueMarginWidth::type_length;
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
			margin_widths[count].type = CSSValueMarginWidth::type_percentage;
			margin_widths[count].percentage = StringHelp::text_to_float(token.value);
		}
		else if (token.type == CSSToken::type_delim && token.value == "-")
		{
			token = next_token(pos, tokens);
			if (is_length(token))
			{
				CSSLength length;
				if (parse_length(token, length))
				{
					length.value = -length.value;
					margin_widths[count].type = CSSValueMarginWidth::type_length;
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
				margin_widths[count].type = CSSValueMarginWidth::type_percentage;
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
			inout_values.push_back(std::unique_ptr<CSSValueMarginWidth>(new CSSValueMarginWidth(CSSValueMarginWidth::left_value, margin_widths[0])));
			inout_values.push_back(std::unique_ptr<CSSValueMarginWidth>(new CSSValueMarginWidth(CSSValueMarginWidth::top_value, margin_widths[0])));
			inout_values.push_back(std::unique_ptr<CSSValueMarginWidth>(new CSSValueMarginWidth(CSSValueMarginWidth::right_value, margin_widths[0])));
			inout_values.push_back(std::unique_ptr<CSSValueMarginWidth>(new CSSValueMarginWidth(CSSValueMarginWidth::bottom_value, margin_widths[0])));
			break;
		case 2:
			inout_values.push_back(std::unique_ptr<CSSValueMarginWidth>(new CSSValueMarginWidth(CSSValueMarginWidth::top_value, margin_widths[0])));
			inout_values.push_back(std::unique_ptr<CSSValueMarginWidth>(new CSSValueMarginWidth(CSSValueMarginWidth::bottom_value, margin_widths[0])));
			inout_values.push_back(std::unique_ptr<CSSValueMarginWidth>(new CSSValueMarginWidth(CSSValueMarginWidth::left_value, margin_widths[1])));
			inout_values.push_back(std::unique_ptr<CSSValueMarginWidth>(new CSSValueMarginWidth(CSSValueMarginWidth::right_value, margin_widths[1])));
			break;
		case 3:
			inout_values.push_back(std::unique_ptr<CSSValueMarginWidth>(new CSSValueMarginWidth(CSSValueMarginWidth::top_value, margin_widths[0])));
			inout_values.push_back(std::unique_ptr<CSSValueMarginWidth>(new CSSValueMarginWidth(CSSValueMarginWidth::left_value, margin_widths[1])));
			inout_values.push_back(std::unique_ptr<CSSValueMarginWidth>(new CSSValueMarginWidth(CSSValueMarginWidth::right_value, margin_widths[1])));
			inout_values.push_back(std::unique_ptr<CSSValueMarginWidth>(new CSSValueMarginWidth(CSSValueMarginWidth::bottom_value, margin_widths[2])));
			break;
		case 4:
			inout_values.push_back(std::unique_ptr<CSSValueMarginWidth>(new CSSValueMarginWidth(CSSValueMarginWidth::top_value, margin_widths[0])));
			inout_values.push_back(std::unique_ptr<CSSValueMarginWidth>(new CSSValueMarginWidth(CSSValueMarginWidth::right_value, margin_widths[1])));
			inout_values.push_back(std::unique_ptr<CSSValueMarginWidth>(new CSSValueMarginWidth(CSSValueMarginWidth::bottom_value, margin_widths[2])));
			inout_values.push_back(std::unique_ptr<CSSValueMarginWidth>(new CSSValueMarginWidth(CSSValueMarginWidth::left_value, margin_widths[3])));
			break;
		default:
			break;
		}
	}
}

}
