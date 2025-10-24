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
#include "css_parser_border.h"
#include "API/CSSLayout/css_box_properties.h"

std::vector<CL_String> CL_CSSParserBorder::get_names()
{
	std::vector<CL_String> names;
	names.push_back("border");
	return names;
}

void CL_CSSParserBorder::parse(CL_CSSBoxProperties &properties, const CL_String &name, const std::vector<CL_CSSToken> &tokens, std::map<CL_String, CL_CSSBoxProperty *> *out_change_set)
{
	CL_CSSBoxBorderWidth border_width;
	CL_CSSBoxBorderStyle border_style;
	CL_CSSBoxBorderColor border_color;
	bool width_specified = false;
	bool style_specified = false;
	bool color_specified = false;

	size_t pos = 0;
	while (pos != tokens.size())
	{
		CL_Colorf color;
		if (!color_specified && parse_color(tokens, pos, color))
		{
			border_color.type = CL_CSSBoxBorderColor::type_color;
			border_color.color = color;
			color_specified = true;
		}
		else
		{
			CL_CSSToken token = next_token(pos, tokens);
			if (token.type == CL_CSSToken::type_ident)
			{
				if (equals(token.value, "inherit") && tokens.size() == 1)
				{
					properties.border_width_left.type = CL_CSSBoxBorderWidth::type_inherit;
					properties.border_width_right.type = CL_CSSBoxBorderWidth::type_inherit;
					properties.border_width_top.type = CL_CSSBoxBorderWidth::type_inherit;
					properties.border_width_bottom.type = CL_CSSBoxBorderWidth::type_inherit;
					properties.border_style_left.type = CL_CSSBoxBorderStyle::type_inherit;
					properties.border_style_right.type = CL_CSSBoxBorderStyle::type_inherit;
					properties.border_style_top.type = CL_CSSBoxBorderStyle::type_inherit;
					properties.border_style_bottom.type = CL_CSSBoxBorderStyle::type_inherit;
					properties.border_color_left.type = CL_CSSBoxBorderColor::type_inherit;
					properties.border_color_right.type = CL_CSSBoxBorderColor::type_inherit;
					properties.border_color_top.type = CL_CSSBoxBorderColor::type_inherit;
					properties.border_color_bottom.type = CL_CSSBoxBorderColor::type_inherit;
					return;
				}
				else if (!width_specified && equals(token.value, "thin"))
				{
					border_width.type = CL_CSSBoxBorderWidth::type_thin;
					width_specified = true;
				}
				else if (!width_specified && equals(token.value, "medium"))
				{
					border_width.type = CL_CSSBoxBorderWidth::type_medium;
					width_specified = true;
				}
				else if (!width_specified && equals(token.value, "thick"))
				{
					border_width.type = CL_CSSBoxBorderWidth::type_thick;
					width_specified = true;
				}
				else if (!style_specified && equals(token.value, "none"))
				{
					border_style.type = CL_CSSBoxBorderStyle::type_none;
					style_specified = true;
				}
				else if (!style_specified && equals(token.value, "hidden"))
				{
					border_style.type = CL_CSSBoxBorderStyle::type_hidden;
					style_specified = true;
				}
				else if (!style_specified && equals(token.value, "dotted"))
				{
					border_style.type = CL_CSSBoxBorderStyle::type_dotted;
					style_specified = true;
				}
				else if (!style_specified && equals(token.value, "dashed"))
				{
					border_style.type = CL_CSSBoxBorderStyle::type_dashed;
					style_specified = true;
				}
				else if (!style_specified && equals(token.value, "solid"))
				{
					border_style.type = CL_CSSBoxBorderStyle::type_solid;
					style_specified = true;
				}
				else if (!style_specified && equals(token.value, "double"))
				{
					border_style.type = CL_CSSBoxBorderStyle::type_double;
					style_specified = true;
				}
				else if (!style_specified && equals(token.value, "groove"))
				{
					border_style.type = CL_CSSBoxBorderStyle::type_groove;
					style_specified = true;
				}
				else if (!style_specified && equals(token.value, "ridge"))
				{
					border_style.type = CL_CSSBoxBorderStyle::type_ridge;
					style_specified = true;
				}
				else if (!style_specified && equals(token.value, "inset"))
				{
					border_style.type = CL_CSSBoxBorderStyle::type_inset;
					style_specified = true;
				}
				else if (!style_specified && equals(token.value, "outset"))
				{
					border_style.type = CL_CSSBoxBorderStyle::type_outset;
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
				CL_CSSBoxLength length;
				if (!width_specified && parse_length(token, length))
				{
					border_width.type = CL_CSSBoxBorderWidth::type_length;
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

	properties.border_width_left = border_width;
	properties.border_width_right = border_width;
	properties.border_width_top = border_width;
	properties.border_width_bottom = border_width;
	properties.border_style_left = border_style;
	properties.border_style_right = border_style;
	properties.border_style_top = border_style;
	properties.border_style_bottom = border_style;
	properties.border_color_left = border_color;
	properties.border_color_right = border_color;
	properties.border_color_top = border_color;
	properties.border_color_bottom = border_color;
	if (out_change_set)
	{
		(*out_change_set)["border-width-left"] = &properties.border_width_left;
		(*out_change_set)["border-width-right"] = &properties.border_width_right;
		(*out_change_set)["border-width-top"] = &properties.border_width_top;
		(*out_change_set)["border-width-bottom"] = &properties.border_width_bottom;

		(*out_change_set)["border-style-left"] = &properties.border_style_left;
		(*out_change_set)["border-style-right"] = &properties.border_style_right;
		(*out_change_set)["border-style-top"] = &properties.border_style_top;
		(*out_change_set)["border-style-bottom"] = &properties.border_style_bottom;

		(*out_change_set)["border-color-left"] = &properties.border_color_left;
		(*out_change_set)["border-color-right"] = &properties.border_color_right;
		(*out_change_set)["border-color-top"] = &properties.border_color_top;
		(*out_change_set)["border-color-bottom"] = &properties.border_color_bottom;
	}
}
