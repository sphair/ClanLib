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
#include "css_parser_outline.h"
#include "API/CSSLayout/css_box_properties.h"

namespace clan
{

std::vector<std::string> CSSParserOutline::get_names()
{
	std::vector<std::string> names;
	names.push_back("outline");
	return names;
}

void CSSParserOutline::parse(CSSBoxProperties &properties, const std::string &name, const std::vector<CSSToken> &tokens, std::map<std::string, CSSPropertyValue *> *out_change_set)
{
	CSSValueOutlineWidth outline_width;
	CSSValueOutlineStyle outline_style;
	CSSValueOutlineColor outline_color;
	bool width_specified = false;
	bool style_specified = false;
	bool color_specified = false;

	size_t pos = 0;
	while (pos != tokens.size())
	{
		Colorf color;
		if (!color_specified && parse_color(tokens, pos, color))
		{
			outline_color.type = CSSValueOutlineColor::type_color;
			outline_color.color = color;
			color_specified = true;
		}
		else
		{
			CSSToken token = next_token(pos, tokens);
			if (token.type == CSSToken::type_ident)
			{
				if (equals(token.value, "inherit") && tokens.size() == 1)
				{
					properties.outline_width.type = CSSValueOutlineWidth::type_inherit;
					properties.outline_style.type = CSSValueOutlineStyle::type_inherit;
					properties.outline_color.type = CSSValueOutlineColor::type_inherit;
					return;
				}
				else if (!width_specified && equals(token.value, "thin"))
				{
					outline_width.type = CSSValueOutlineWidth::type_thin;
					width_specified = true;
				}
				else if (!width_specified && equals(token.value, "medium"))
				{
					outline_width.type = CSSValueOutlineWidth::type_medium;
					width_specified = true;
				}
				else if (!width_specified && equals(token.value, "thick"))
				{
					outline_width.type = CSSValueOutlineWidth::type_thick;
					width_specified = true;
				}
				else if (!style_specified && equals(token.value, "none"))
				{
					outline_style.type = CSSValueOutlineStyle::type_none;
					style_specified = true;
				}
				else if (!style_specified && equals(token.value, "hidden"))
				{
					outline_style.type = CSSValueOutlineStyle::type_hidden;
					style_specified = true;
				}
				else if (!style_specified && equals(token.value, "dotted"))
				{
					outline_style.type = CSSValueOutlineStyle::type_dotted;
					style_specified = true;
				}
				else if (!style_specified && equals(token.value, "dashed"))
				{
					outline_style.type = CSSValueOutlineStyle::type_dashed;
					style_specified = true;
				}
				else if (!style_specified && equals(token.value, "solid"))
				{
					outline_style.type = CSSValueOutlineStyle::type_solid;
					style_specified = true;
				}
				else if (!style_specified && equals(token.value, "double"))
				{
					outline_style.type = CSSValueOutlineStyle::type_double;
					style_specified = true;
				}
				else if (!style_specified && equals(token.value, "groove"))
				{
					outline_style.type = CSSValueOutlineStyle::type_groove;
					style_specified = true;
				}
				else if (!style_specified && equals(token.value, "ridge"))
				{
					outline_style.type = CSSValueOutlineStyle::type_ridge;
					style_specified = true;
				}
				else if (!style_specified && equals(token.value, "inset"))
				{
					outline_style.type = CSSValueOutlineStyle::type_inset;
					style_specified = true;
				}
				else if (!style_specified && equals(token.value, "outset"))
				{
					outline_style.type = CSSValueOutlineStyle::type_outset;
					style_specified = true;
				}
				else if (!color_specified && equals(token.value, "invert"))
				{
					outline_color.type = CSSValueOutlineColor::type_invert;
					color_specified = true;
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
					outline_width.type = CSSValueOutlineWidth::type_length;
					outline_width.length = length;
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

	properties.outline_width = outline_width;
	properties.outline_style = outline_style;
	properties.outline_color = outline_color;
	if (out_change_set)
	{
		(*out_change_set)["outline-width"] = &properties.outline_width;
		(*out_change_set)["outline-style"] = &properties.outline_style;
		(*out_change_set)["outline-color"] = &properties.outline_color;
	}
}

}
