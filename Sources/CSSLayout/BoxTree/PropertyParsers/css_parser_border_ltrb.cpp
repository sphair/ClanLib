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
#include "css_parser_border_ltrb.h"
#include "API/CSSLayout/css_box_properties.h"

namespace clan
{

std::vector<std::string> CSSParserBorderLTRB::get_names()
{
	std::vector<std::string> names;
	names.push_back("border-left");
	names.push_back("border-top");
	names.push_back("border-right");
	names.push_back("border-bottom");
	return names;
}

void CSSParserBorderLTRB::parse(CSSBoxProperties &properties, const std::string &name, const std::vector<CSSToken> &tokens, std::map<std::string, CSSBoxProperty *> *out_change_set)
{
	CSSBoxBorderWidth *width_prop = 0;
	CSSBoxBorderStyle *style_prop = 0;
	CSSBoxBorderColor *color_prop = 0;
	if (equals(name, "border-top"))
	{
		width_prop = &properties.border_width_top;
		style_prop = &properties.border_style_top;
		color_prop = &properties.border_color_top;
	}
	else if (equals(name, "border-right"))
	{
		width_prop = &properties.border_width_right;
		style_prop = &properties.border_style_right;
		color_prop = &properties.border_color_right;
	}
	else if (equals(name, "border-bottom"))
	{
		width_prop = &properties.border_width_bottom;
		style_prop = &properties.border_style_bottom;
		color_prop = &properties.border_color_bottom;
	}
	else if (equals(name, "border-left"))
	{
		width_prop = &properties.border_width_left;
		style_prop = &properties.border_style_left;
		color_prop = &properties.border_color_left;
	}
	else
	{
		debug_parse_error(name, tokens);
		return;
	}

	CSSBoxBorderWidth border_width;
	CSSBoxBorderStyle border_style;
	CSSBoxBorderColor border_color;

	bool width_specified = false;
	bool style_specified = false;
	bool color_specified = false;

	size_t pos = 0;
	while (pos != tokens.size())
	{
		Colorf color;
		if (!color_specified && parse_color(tokens, pos, color))
		{
			border_color.type = CSSBoxBorderColor::type_color;
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
					width_prop->type = CSSBoxBorderWidth::type_inherit;
					style_prop->type = CSSBoxBorderStyle::type_inherit;
					color_prop->type = CSSBoxBorderColor::type_inherit;
					return;
				}
				else if (!width_specified && equals(token.value, "thin"))
				{
					border_width.type = CSSBoxBorderWidth::type_thin;
					width_specified = true;
				}
				else if (!width_specified && equals(token.value, "medium"))
				{
					border_width.type = CSSBoxBorderWidth::type_medium;
					width_specified = true;
				}
				else if (!width_specified && equals(token.value, "thick"))
				{
					border_width.type = CSSBoxBorderWidth::type_thick;
					width_specified = true;
				}
				else if (!style_specified && equals(token.value, "none"))
				{
					border_style.type = CSSBoxBorderStyle::type_none;
					style_specified = true;
				}
				else if (!style_specified && equals(token.value, "hidden"))
				{
					border_style.type = CSSBoxBorderStyle::type_hidden;
					style_specified = true;
				}
				else if (!style_specified && equals(token.value, "dotted"))
				{
					border_style.type = CSSBoxBorderStyle::type_dotted;
					style_specified = true;
				}
				else if (!style_specified && equals(token.value, "dashed"))
				{
					border_style.type = CSSBoxBorderStyle::type_dashed;
					style_specified = true;
				}
				else if (!style_specified && equals(token.value, "solid"))
				{
					border_style.type = CSSBoxBorderStyle::type_solid;
					style_specified = true;
				}
				else if (!style_specified && equals(token.value, "double"))
				{
					border_style.type = CSSBoxBorderStyle::type_double;
					style_specified = true;
				}
				else if (!style_specified && equals(token.value, "groove"))
				{
					border_style.type = CSSBoxBorderStyle::type_groove;
					style_specified = true;
				}
				else if (!style_specified && equals(token.value, "ridge"))
				{
					border_style.type = CSSBoxBorderStyle::type_ridge;
					style_specified = true;
				}
				else if (!style_specified && equals(token.value, "inset"))
				{
					border_style.type = CSSBoxBorderStyle::type_inset;
					style_specified = true;
				}
				else if (!style_specified && equals(token.value, "outset"))
				{
					border_style.type = CSSBoxBorderStyle::type_outset;
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
				CSSBoxLength length;
				if (!width_specified && parse_length(token, length))
				{
					border_width.type = CSSBoxBorderWidth::type_length;
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

	*width_prop = border_width;
	*style_prop = border_style;
	*color_prop = border_color;
	if (out_change_set)
	{
		if (width_specified)
		{
			(*out_change_set)[name + "-width"] = width_prop;
		}

		if (style_specified)
		{
			(*out_change_set)[name + "-style"] = style_prop;
		}

		if (color_specified)
		{
			(*out_change_set)[name + "-color"] = color_prop;
		}
	}
}

}
