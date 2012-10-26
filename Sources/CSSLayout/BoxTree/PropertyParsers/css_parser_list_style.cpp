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
#include "css_parser_list_style.h"
#include "API/CSSLayout/css_box_properties.h"

namespace clan
{

std::vector<std::string> CSSParserListStyle::get_names()
{
	std::vector<std::string> names;
	names.push_back("list-style");
	return names;
}

void CSSParserListStyle::parse(CSSBoxProperties &properties, const std::string &name, const std::vector<CSSToken> &tokens, std::map<std::string, CSSBoxProperty *> *out_change_set)
{
	CSSBoxListStyleType list_style_type;
	CSSBoxListStylePosition list_style_position;
	CSSBoxListStyleImage list_style_image;
	bool type_specified = false;
	bool position_specified = false;
	bool image_specified = false;

	size_t pos = 0;
	while (pos != tokens.size())
	{
		CSSToken token = next_token(pos, tokens);
		if (token.type == CSSToken::type_ident && equals(token.value, "inherit") && tokens.size() == 1)
		{
			properties.list_style_type.type = CSSBoxListStyleType::type_inherit;
			properties.list_style_position.type = CSSBoxListStylePosition::type_inherit;
			properties.list_style_image.type = CSSBoxListStyleImage::type_inherit;
			return;
		}
		else if (token.type == CSSToken::type_ident)
		{
			if (!type_specified && equals(token.value, "disc"))
			{
				list_style_type.type = CSSBoxListStyleType::type_disc;
				type_specified = true;
			}
			else if (!type_specified && equals(token.value, "circle"))
			{
				list_style_type.type = CSSBoxListStyleType::type_circle;
				type_specified = true;
			}
			else if (!type_specified && equals(token.value, "square"))
			{
				list_style_type.type = CSSBoxListStyleType::type_square;
				type_specified = true;
			}
			else if (!type_specified && equals(token.value, "decimal"))
			{
				list_style_type.type = CSSBoxListStyleType::type_decimal;
				type_specified = true;
			}
			else if (!type_specified && equals(token.value, "decimal-leading-zero"))
			{
				list_style_type.type = CSSBoxListStyleType::type_decimal_leading_zero;
				type_specified = true;
			}
			else if (!type_specified && equals(token.value, "lower-roman"))
			{
				list_style_type.type = CSSBoxListStyleType::type_lower_roman;
				type_specified = true;
			}
			else if (!type_specified && equals(token.value, "upper-roman"))
			{
				list_style_type.type = CSSBoxListStyleType::type_upper_roman;
				type_specified = true;
			}
			else if (!type_specified && equals(token.value, "lower-greek"))
			{
				list_style_type.type = CSSBoxListStyleType::type_lower_greek;
				type_specified = true;
			}
			else if (!type_specified && equals(token.value, "lower-latin"))
			{
				list_style_type.type = CSSBoxListStyleType::type_lower_latin;
				type_specified = true;
			}
			else if (!type_specified && equals(token.value, "upper-latin"))
			{
				list_style_type.type = CSSBoxListStyleType::type_upper_latin;
				type_specified = true;
			}
			else if (!type_specified && equals(token.value, "armenian"))
			{
				list_style_type.type = CSSBoxListStyleType::type_armenian;
				type_specified = true;
			}
			else if (!type_specified && equals(token.value, "georgian"))
			{
				list_style_type.type = CSSBoxListStyleType::type_georgian;
				type_specified = true;
			}
			else if (!type_specified && equals(token.value, "lower-alpha"))
			{
				list_style_type.type = CSSBoxListStyleType::type_lower_alpha;
				type_specified = true;
			}
			else if (!type_specified && equals(token.value, "upper-alpha"))
			{
				list_style_type.type = CSSBoxListStyleType::type_upper_alpha;
				type_specified = true;
			}
			else if (equals(token.value, "none"))
			{
				if (!type_specified && !image_specified) // This copies Firefox' behavior - unclear in the standard if it is correct or not.
				{
					list_style_type.type = CSSBoxListStyleType::type_none;
					type_specified = true;
				}
				else if (!image_specified)
				{
					list_style_image.type = CSSBoxListStyleImage::type_none;
					image_specified = true;
				}
				else
				{
					debug_parse_error(name, tokens);
					return;
				}
			}
			else if (!position_specified && equals(token.value, "inside"))
			{
				list_style_position.type = CSSBoxListStylePosition::type_inside;
				position_specified = true;
			}
			else if (!position_specified && equals(token.value, "outside"))
			{
				list_style_position.type = CSSBoxListStylePosition::type_outside;
				position_specified = true;
			}
			else
			{
				debug_parse_error(name, tokens);
				return;
			}
		}
		else if (!image_specified && token.type == CSSToken::type_uri)
		{
			list_style_image.type = CSSBoxListStyleImage::type_uri;
			list_style_image.url = token.value;
			image_specified = true;
		}
		else
		{
			debug_parse_error(name, tokens);
			return;
		}
	}

	properties.list_style_type = list_style_type;
	properties.list_style_position = list_style_position;
	properties.list_style_image = list_style_image;
	if (out_change_set)
	{
		(*out_change_set)["list-style-type"] = &properties.list_style_type;
		(*out_change_set)["list-style-position"] = &properties.list_style_position;
		(*out_change_set)["list-style-image"] = &properties.list_style_image;
	}
}

}
