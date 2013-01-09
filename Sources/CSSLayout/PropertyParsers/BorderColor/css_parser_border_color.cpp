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
#include "css_parser_border_color.h"
#include "API/CSSLayout/css_box_properties.h"

namespace clan
{

std::vector<std::string> CSSParserBorderColor::get_names()
{
	std::vector<std::string> names;
	names.push_back("border-color");
	return names;
}

void CSSParserBorderColor::parse(const std::string &name, const std::vector<CSSToken> &tokens, std::vector<std::unique_ptr<CSSPropertyValue> > &inout_values)
{
	CSSValueBorderColor border_colors[4];
	int count;
	size_t pos = 0;
	for (count = 0; count < 4; count++)
	{
		Colorf color;
		if (parse_color(tokens, pos, color))
		{
			border_colors[count].type = CSSValueBorderColor::type_color;
			border_colors[count].color = color;
		}
		else
		{
			CSSToken token = next_token(pos, tokens);
			if (token.type == CSSToken::type_ident)
			{
				if (equals(token.value, "inherit"))
				{
					if (count == 0 && pos == tokens.size())
					{
						border_colors[0].type = CSSValueBorderColor::type_inherit;

						inout_values.push_back(std::unique_ptr<CSSValueBorderColor>(new CSSValueBorderColor(CSSValueBorderColor::left_value, border_colors[0])));
						inout_values.push_back(std::unique_ptr<CSSValueBorderColor>(new CSSValueBorderColor(CSSValueBorderColor::right_value, border_colors[0])));
						inout_values.push_back(std::unique_ptr<CSSValueBorderColor>(new CSSValueBorderColor(CSSValueBorderColor::top_value, border_colors[0])));
						inout_values.push_back(std::unique_ptr<CSSValueBorderColor>(new CSSValueBorderColor(CSSValueBorderColor::bottom_value, border_colors[0])));
					}
					else
					{
						debug_parse_error(name, tokens);
					}
					return;
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
	}

	if (pos == tokens.size())
	{
		switch (count)
		{
		case 1:
			inout_values.push_back(std::unique_ptr<CSSValueBorderColor>(new CSSValueBorderColor(CSSValueBorderColor::left_value, border_colors[0])));
			inout_values.push_back(std::unique_ptr<CSSValueBorderColor>(new CSSValueBorderColor(CSSValueBorderColor::right_value, border_colors[0])));
			inout_values.push_back(std::unique_ptr<CSSValueBorderColor>(new CSSValueBorderColor(CSSValueBorderColor::top_value, border_colors[0])));
			inout_values.push_back(std::unique_ptr<CSSValueBorderColor>(new CSSValueBorderColor(CSSValueBorderColor::bottom_value, border_colors[0])));
			break;
		case 2:
			inout_values.push_back(std::unique_ptr<CSSValueBorderColor>(new CSSValueBorderColor(CSSValueBorderColor::top_value, border_colors[0])));
			inout_values.push_back(std::unique_ptr<CSSValueBorderColor>(new CSSValueBorderColor(CSSValueBorderColor::bottom_value, border_colors[0])));
			inout_values.push_back(std::unique_ptr<CSSValueBorderColor>(new CSSValueBorderColor(CSSValueBorderColor::left_value, border_colors[1])));
			inout_values.push_back(std::unique_ptr<CSSValueBorderColor>(new CSSValueBorderColor(CSSValueBorderColor::right_value, border_colors[1])));
			break;
		case 3:
			inout_values.push_back(std::unique_ptr<CSSValueBorderColor>(new CSSValueBorderColor(CSSValueBorderColor::top_value, border_colors[0])));
			inout_values.push_back(std::unique_ptr<CSSValueBorderColor>(new CSSValueBorderColor(CSSValueBorderColor::left_value, border_colors[1])));
			inout_values.push_back(std::unique_ptr<CSSValueBorderColor>(new CSSValueBorderColor(CSSValueBorderColor::right_value, border_colors[1])));
			inout_values.push_back(std::unique_ptr<CSSValueBorderColor>(new CSSValueBorderColor(CSSValueBorderColor::bottom_value, border_colors[2])));
			break;
		case 4:
			inout_values.push_back(std::unique_ptr<CSSValueBorderColor>(new CSSValueBorderColor(CSSValueBorderColor::top_value, border_colors[0])));
			inout_values.push_back(std::unique_ptr<CSSValueBorderColor>(new CSSValueBorderColor(CSSValueBorderColor::right_value, border_colors[1])));
			inout_values.push_back(std::unique_ptr<CSSValueBorderColor>(new CSSValueBorderColor(CSSValueBorderColor::left_value, border_colors[2])));
			inout_values.push_back(std::unique_ptr<CSSValueBorderColor>(new CSSValueBorderColor(CSSValueBorderColor::bottom_value, border_colors[3])));
			break;
		default:
			break;
		}
	}
}

}
