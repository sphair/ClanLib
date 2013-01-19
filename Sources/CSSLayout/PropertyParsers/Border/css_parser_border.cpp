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
#include "css_parser_border.h"
#include "API/CSSLayout/ComputedValues/css_computed_box.h"

namespace clan
{

std::vector<std::string> CSSParserBorder::get_names()
{
	std::vector<std::string> names;
	names.push_back("border");
	return names;
}

void CSSParserBorder::parse(const std::string &name, const std::vector<CSSToken> &tokens, std::vector<std::unique_ptr<CSSPropertyValue> > &inout_values)
{
	CSSValueBorderWidth border_width;
	CSSValueBorderStyle border_style;
	CSSValueBorderColor border_color;
	bool width_specified = false;
	bool style_specified = false;
	bool color_specified = false;

	size_t pos = 0;
	while (pos != tokens.size())
	{
		Colorf color;
		if (!color_specified && parse_color(tokens, pos, color))
		{
			border_color.type = CSSValueBorderColor::type_color;
			border_color.color = color;
			color_specified = true;
		}
		else
		{
			CSSToken token = next_token(pos, tokens);
			if (token.type == CSSToken::type_ident)
			{
				if (equals(token.value, "inherit") && tokens.size() == 1)
				{
					border_width.type = CSSValueBorderWidth::type_inherit;
					border_style.type = CSSValueBorderStyle::type_inherit;
					border_color.type = CSSValueBorderColor::type_inherit;

					inout_values.push_back(std::unique_ptr<CSSValueBorderWidth>(new CSSValueBorderWidth(CSSValueBorderWidth::left_value, border_width)));
					inout_values.push_back(std::unique_ptr<CSSValueBorderWidth>(new CSSValueBorderWidth(CSSValueBorderWidth::right_value, border_width)));
					inout_values.push_back(std::unique_ptr<CSSValueBorderWidth>(new CSSValueBorderWidth(CSSValueBorderWidth::top_value, border_width)));
					inout_values.push_back(std::unique_ptr<CSSValueBorderWidth>(new CSSValueBorderWidth(CSSValueBorderWidth::bottom_value, border_width)));

					inout_values.push_back(std::unique_ptr<CSSValueBorderStyle>(new CSSValueBorderStyle(CSSValueBorderStyle::left_value, border_style)));
					inout_values.push_back(std::unique_ptr<CSSValueBorderStyle>(new CSSValueBorderStyle(CSSValueBorderStyle::right_value, border_style)));
					inout_values.push_back(std::unique_ptr<CSSValueBorderStyle>(new CSSValueBorderStyle(CSSValueBorderStyle::top_value, border_style)));
					inout_values.push_back(std::unique_ptr<CSSValueBorderStyle>(new CSSValueBorderStyle(CSSValueBorderStyle::bottom_value, border_style)));

					inout_values.push_back(std::unique_ptr<CSSValueBorderColor>(new CSSValueBorderColor(CSSValueBorderColor::left_value, border_color)));
					inout_values.push_back(std::unique_ptr<CSSValueBorderColor>(new CSSValueBorderColor(CSSValueBorderColor::right_value, border_color)));
					inout_values.push_back(std::unique_ptr<CSSValueBorderColor>(new CSSValueBorderColor(CSSValueBorderColor::top_value, border_color)));
					inout_values.push_back(std::unique_ptr<CSSValueBorderColor>(new CSSValueBorderColor(CSSValueBorderColor::bottom_value, border_color)));

					return;
				}
				else if (!width_specified && equals(token.value, "thin"))
				{
					border_width.type = CSSValueBorderWidth::type_thin;
					width_specified = true;
				}
				else if (!width_specified && equals(token.value, "medium"))
				{
					border_width.type = CSSValueBorderWidth::type_medium;
					width_specified = true;
				}
				else if (!width_specified && equals(token.value, "thick"))
				{
					border_width.type = CSSValueBorderWidth::type_thick;
					width_specified = true;
				}
				else if (!style_specified && equals(token.value, "none"))
				{
					border_style.type = CSSValueBorderStyle::type_none;
					style_specified = true;
				}
				else if (!style_specified && equals(token.value, "hidden"))
				{
					border_style.type = CSSValueBorderStyle::type_hidden;
					style_specified = true;
				}
				else if (!style_specified && equals(token.value, "dotted"))
				{
					border_style.type = CSSValueBorderStyle::type_dotted;
					style_specified = true;
				}
				else if (!style_specified && equals(token.value, "dashed"))
				{
					border_style.type = CSSValueBorderStyle::type_dashed;
					style_specified = true;
				}
				else if (!style_specified && equals(token.value, "solid"))
				{
					border_style.type = CSSValueBorderStyle::type_solid;
					style_specified = true;
				}
				else if (!style_specified && equals(token.value, "double"))
				{
					border_style.type = CSSValueBorderStyle::type_double;
					style_specified = true;
				}
				else if (!style_specified && equals(token.value, "groove"))
				{
					border_style.type = CSSValueBorderStyle::type_groove;
					style_specified = true;
				}
				else if (!style_specified && equals(token.value, "ridge"))
				{
					border_style.type = CSSValueBorderStyle::type_ridge;
					style_specified = true;
				}
				else if (!style_specified && equals(token.value, "inset"))
				{
					border_style.type = CSSValueBorderStyle::type_inset;
					style_specified = true;
				}
				else if (!style_specified && equals(token.value, "outset"))
				{
					border_style.type = CSSValueBorderStyle::type_outset;
					style_specified = true;
				}
				else
				{
					debug_parse_error(name, tokens);
					return;
				}
			}
			else if (is_length(token))
			{
				CSSLength length;
				if (!width_specified && parse_length(token, length))
				{
					border_width.type = CSSValueBorderWidth::type_length;
					border_width.length = length;
					width_specified = true;
				}
				else
				{
					debug_parse_error(name, tokens);
					return;
				}
			}
			else
			{
				debug_parse_error(name, tokens);
				return;
			}
		}
	}

	inout_values.push_back(std::unique_ptr<CSSValueBorderWidth>(new CSSValueBorderWidth(CSSValueBorderWidth::left_value, border_width)));
	inout_values.push_back(std::unique_ptr<CSSValueBorderWidth>(new CSSValueBorderWidth(CSSValueBorderWidth::right_value, border_width)));
	inout_values.push_back(std::unique_ptr<CSSValueBorderWidth>(new CSSValueBorderWidth(CSSValueBorderWidth::top_value, border_width)));
	inout_values.push_back(std::unique_ptr<CSSValueBorderWidth>(new CSSValueBorderWidth(CSSValueBorderWidth::bottom_value, border_width)));

	inout_values.push_back(std::unique_ptr<CSSValueBorderStyle>(new CSSValueBorderStyle(CSSValueBorderStyle::left_value, border_style)));
	inout_values.push_back(std::unique_ptr<CSSValueBorderStyle>(new CSSValueBorderStyle(CSSValueBorderStyle::right_value, border_style)));
	inout_values.push_back(std::unique_ptr<CSSValueBorderStyle>(new CSSValueBorderStyle(CSSValueBorderStyle::top_value, border_style)));
	inout_values.push_back(std::unique_ptr<CSSValueBorderStyle>(new CSSValueBorderStyle(CSSValueBorderStyle::bottom_value, border_style)));

	inout_values.push_back(std::unique_ptr<CSSValueBorderColor>(new CSSValueBorderColor(CSSValueBorderColor::left_value, border_color)));
	inout_values.push_back(std::unique_ptr<CSSValueBorderColor>(new CSSValueBorderColor(CSSValueBorderColor::right_value, border_color)));
	inout_values.push_back(std::unique_ptr<CSSValueBorderColor>(new CSSValueBorderColor(CSSValueBorderColor::top_value, border_color)));
	inout_values.push_back(std::unique_ptr<CSSValueBorderColor>(new CSSValueBorderColor(CSSValueBorderColor::bottom_value, border_color)));
}

}
