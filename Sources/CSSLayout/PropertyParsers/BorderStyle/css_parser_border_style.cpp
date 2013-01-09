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
#include "css_parser_border_style.h"
#include "API/CSSLayout/css_box_properties.h"

namespace clan
{

std::vector<std::string> CSSParserBorderStyle::get_names()
{
	std::vector<std::string> names;
	names.push_back("border-style");
	return names;
}

void CSSParserBorderStyle::parse(const std::string &name, const std::vector<CSSToken> &tokens, std::vector<std::unique_ptr<CSSPropertyValue> > &inout_values)
{
	CSSValueBorderStyle border_styles[4];
	int count;
	size_t pos = 0;
	for (count = 0; count < 4; count++)
	{
		CSSToken token = next_token(pos, tokens);
		if (token.type == CSSToken::type_ident)
		{
			if (equals(token.value, "none"))
			{
				border_styles[count].type = CSSValueBorderStyle::type_none;
			}
			else if (equals(token.value, "hidden"))
			{
				border_styles[count].type = CSSValueBorderStyle::type_hidden;
			}
			else if (equals(token.value, "dotted"))
			{
				border_styles[count].type = CSSValueBorderStyle::type_dotted;
			}
			else if (equals(token.value, "dashed"))
			{
				border_styles[count].type = CSSValueBorderStyle::type_dashed;
			}
			else if (equals(token.value, "solid"))
			{
				border_styles[count].type = CSSValueBorderStyle::type_solid;
			}
			else if (equals(token.value, "double"))
			{
				border_styles[count].type = CSSValueBorderStyle::type_double;
			}
			else if (equals(token.value, "groove"))
			{
				border_styles[count].type = CSSValueBorderStyle::type_groove;
			}
			else if (equals(token.value, "ridge"))
			{
				border_styles[count].type = CSSValueBorderStyle::type_ridge;
			}
			else if (equals(token.value, "inset"))
			{
				border_styles[count].type = CSSValueBorderStyle::type_inset;
			}
			else if (equals(token.value, "outset"))
			{
				border_styles[count].type = CSSValueBorderStyle::type_outset;
			}
			else if (equals(token.value, "inherit"))
			{
				if (count == 0 && pos == tokens.size())
				{
					border_styles[0].type = CSSValueBorderStyle::type_inherit;
					inout_values.push_back(std::unique_ptr<CSSValueBorderStyle>(new CSSValueBorderStyle(CSSValueBorderStyle::left_value, border_styles[0])));
					inout_values.push_back(std::unique_ptr<CSSValueBorderStyle>(new CSSValueBorderStyle(CSSValueBorderStyle::top_value, border_styles[0])));
					inout_values.push_back(std::unique_ptr<CSSValueBorderStyle>(new CSSValueBorderStyle(CSSValueBorderStyle::right_value, border_styles[0])));
					inout_values.push_back(std::unique_ptr<CSSValueBorderStyle>(new CSSValueBorderStyle(CSSValueBorderStyle::bottom_value, border_styles[0])));
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

	if (pos == tokens.size())
	{
		switch (count)
		{
		case 1:
			inout_values.push_back(std::unique_ptr<CSSValueBorderStyle>(new CSSValueBorderStyle(CSSValueBorderStyle::left_value, border_styles[0])));
			inout_values.push_back(std::unique_ptr<CSSValueBorderStyle>(new CSSValueBorderStyle(CSSValueBorderStyle::top_value, border_styles[0])));
			inout_values.push_back(std::unique_ptr<CSSValueBorderStyle>(new CSSValueBorderStyle(CSSValueBorderStyle::right_value, border_styles[0])));
			inout_values.push_back(std::unique_ptr<CSSValueBorderStyle>(new CSSValueBorderStyle(CSSValueBorderStyle::bottom_value, border_styles[0])));
			break;
		case 2:
			inout_values.push_back(std::unique_ptr<CSSValueBorderStyle>(new CSSValueBorderStyle(CSSValueBorderStyle::top_value, border_styles[0])));
			inout_values.push_back(std::unique_ptr<CSSValueBorderStyle>(new CSSValueBorderStyle(CSSValueBorderStyle::bottom_value, border_styles[0])));
			inout_values.push_back(std::unique_ptr<CSSValueBorderStyle>(new CSSValueBorderStyle(CSSValueBorderStyle::left_value, border_styles[1])));
			inout_values.push_back(std::unique_ptr<CSSValueBorderStyle>(new CSSValueBorderStyle(CSSValueBorderStyle::right_value, border_styles[1])));
			break;
		case 3:
			inout_values.push_back(std::unique_ptr<CSSValueBorderStyle>(new CSSValueBorderStyle(CSSValueBorderStyle::top_value, border_styles[0])));
			inout_values.push_back(std::unique_ptr<CSSValueBorderStyle>(new CSSValueBorderStyle(CSSValueBorderStyle::left_value, border_styles[1])));
			inout_values.push_back(std::unique_ptr<CSSValueBorderStyle>(new CSSValueBorderStyle(CSSValueBorderStyle::right_value, border_styles[1])));
			inout_values.push_back(std::unique_ptr<CSSValueBorderStyle>(new CSSValueBorderStyle(CSSValueBorderStyle::bottom_value, border_styles[2])));
			break;
		case 4:
			inout_values.push_back(std::unique_ptr<CSSValueBorderStyle>(new CSSValueBorderStyle(CSSValueBorderStyle::top_value, border_styles[0])));
			inout_values.push_back(std::unique_ptr<CSSValueBorderStyle>(new CSSValueBorderStyle(CSSValueBorderStyle::right_value, border_styles[1])));
			inout_values.push_back(std::unique_ptr<CSSValueBorderStyle>(new CSSValueBorderStyle(CSSValueBorderStyle::bottom_value, border_styles[2])));
			inout_values.push_back(std::unique_ptr<CSSValueBorderStyle>(new CSSValueBorderStyle(CSSValueBorderStyle::left_value, border_styles[3])));
			break;
		default:
			break;
		}
	}
}

}
