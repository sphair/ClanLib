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
#include "css_parser_font.h"
#include "API/CSSLayout/PropertyValues/css_value_font_style.h"
#include "API/CSSLayout/PropertyValues/css_value_font_variant.h"
#include "API/CSSLayout/PropertyValues/css_value_font_weight.h"
#include "API/CSSLayout/PropertyValues/css_value_font_size.h"
#include "API/CSSLayout/PropertyValues/css_value_font_family.h"
#include "API/CSSLayout/PropertyValues/css_value_line_height.h"

namespace clan
{

std::vector<std::string> CSSParserFont::get_names()
{
	std::vector<std::string> names;
	names.push_back("font");
	return names;
}

void CSSParserFont::parse(const std::string &propname, const std::vector<CSSToken> &tokens, std::vector<std::unique_ptr<CSSPropertyValue> > &inout_values)
{
	std::unique_ptr<CSSValueFontStyle> style(new CSSValueFontStyle());
	std::unique_ptr<CSSValueFontVariant> variant(new CSSValueFontVariant());
	std::unique_ptr<CSSValueFontWeight> weight(new CSSValueFontWeight());
	std::unique_ptr<CSSValueFontSize> size(new CSSValueFontSize());
	std::unique_ptr<CSSValueLineHeight> line_height(new CSSValueLineHeight());
	std::unique_ptr<CSSValueFontFamily> family(new CSSValueFontFamily());
	style->type = CSSValueFontStyle::type_normal;
	variant->type = CSSValueFontVariant::type_normal;
	weight->type = CSSValueFontWeight::type_normal;
	size->type = CSSValueFontSize::type_medium;
	line_height->type = CSSValueLineHeight::type_normal;
	family->type = CSSValueFontFamily::type_names;
	family->names.push_back(CSSValueFontFamilyName());

	bool font_style_set = false;
	bool font_variant_set = false;
	bool font_weight_set = false;
	int normal_count = 0;
	size_t pos = 0;
	CSSToken token;
	while (pos < tokens.size())
	{
		token = next_token(pos, tokens);
		if (token.type == CSSToken::type_ident)
		{
			if (tokens.size() == 1 &&
				(equals(token.value, "caption") ||
				equals(token.value, "icon") ||
				equals(token.value, "menu") ||
				equals(token.value, "message-box") ||
				equals(token.value, "small-caption") ||
				equals(token.value, "status-bar")))
			{
				inout_values.push_back(std::move(style));
				inout_values.push_back(std::move(variant));
				inout_values.push_back(std::move(weight));
				inout_values.push_back(std::move(size));
				inout_values.push_back(std::move(line_height));
				inout_values.push_back(std::move(family));
				return;
			}
			else if (equals(token.value, "inherit") && tokens.size() == 1)
			{
				style->type = CSSValueFontStyle::type_inherit;
				variant->type = CSSValueFontVariant::type_inherit;
				weight->type = CSSValueFontWeight::type_inherit;
				size->type = CSSValueFontSize::type_inherit;
				line_height->type = CSSValueLineHeight::type_inherit;
				family->type = CSSValueFontFamily::type_inherit;
				
				inout_values.push_back(std::move(style));
				inout_values.push_back(std::move(variant));
				inout_values.push_back(std::move(weight));
				inout_values.push_back(std::move(size));
				inout_values.push_back(std::move(line_height));
				inout_values.push_back(std::move(family));
				return;
			}
			else if (equals(token.value, "normal")) // font-style or font-weight or font-variant
			{
				int allowed = 3;
				if (font_style_set)
					allowed--;
				if (font_weight_set)
					allowed--;
				if (font_variant_set)
					allowed--;
				if (normal_count < allowed)
					normal_count++;
			}
			else if (equals(token.value, "italic") && !font_style_set) // font-style
			{
				font_style_set = true;
				style->type = CSSValueFontStyle::type_italic;
			}
			else if (equals(token.value, "oblique") && !font_style_set) // font-style
			{
				font_style_set = true;
				style->type = CSSValueFontStyle::type_oblique;
			}
			else if (equals(token.value, "small-caps") && !font_variant_set) // font-variant
			{
				font_style_set = true;
				variant->type = CSSValueFontVariant::type_small_caps;
			}
			else if (equals(token.value, "bold") && !font_weight_set) // font-weight
			{
				font_weight_set = true;
				weight->type = CSSValueFontWeight::type_bold;
			}
			else if (equals(token.value, "bolder") && !font_weight_set) // font-weight
			{
				font_weight_set = true;
				weight->type = CSSValueFontWeight::type_bolder;
			}
			else if (equals(token.value, "lighter") && !font_weight_set) // font-weight
			{
				font_weight_set = true;
				weight->type = CSSValueFontWeight::type_lighter;
			}
			else if (token.value == "100" && !font_weight_set) // font-weight
			{
				font_weight_set = true;
				weight->type = CSSValueFontWeight::type_100;
			}
			else if (token.value == "200" && !font_weight_set) // font-weight
			{
				font_weight_set = true;
				weight->type = CSSValueFontWeight::type_200;
			}
			else if (token.value == "300" && !font_weight_set) // font-weight
			{
				font_weight_set = true;
				weight->type = CSSValueFontWeight::type_300;
			}
			else if (token.value == "400" && !font_weight_set) // font-weight
			{
				font_weight_set = true;
				weight->type = CSSValueFontWeight::type_400;
			}
			else if (token.value == "500" && !font_weight_set) // font-weight
			{
				font_weight_set = true;
				weight->type = CSSValueFontWeight::type_500;
			}
			else if (token.value == "600" && !font_weight_set) // font-weight
			{
				font_weight_set = true;
				weight->type = CSSValueFontWeight::type_600;
			}
			else if (token.value == "700" && !font_weight_set) // font-weight
			{
				font_weight_set = true;
				weight->type = CSSValueFontWeight::type_700;
			}
			else if (token.value == "800" && !font_weight_set) // font-weight
			{
				font_weight_set = true;
				weight->type = CSSValueFontWeight::type_800;
			}
			else if (token.value == "900" && !font_weight_set) // font-weight
			{
				font_weight_set = true;
				weight->type = CSSValueFontWeight::type_900;
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}

	if (pos == tokens.size())
	{
		debug_parse_error(propname, tokens);
		return;
	}

	if (token.type == CSSToken::type_ident)
	{
		if (equals(token.value, "xx-small"))
			size->type = CSSValueFontSize::type_xx_small;
		else if (equals(token.value, "x-small"))
			size->type = CSSValueFontSize::type_x_small;
		else if (equals(token.value, "small"))
			size->type = CSSValueFontSize::type_small;
		else if (equals(token.value, "medium"))
			size->type = CSSValueFontSize::type_medium;
		else if (equals(token.value, "large"))
			size->type = CSSValueFontSize::type_large;
		else if (equals(token.value, "x-large"))
			size->type = CSSValueFontSize::type_x_large;
		else if (equals(token.value, "xx-large"))
			size->type = CSSValueFontSize::type_xx_large;
		else if (equals(token.value, "smaller"))
			size->type = CSSValueFontSize::type_smaller;
		else if (equals(token.value, "larger"))
			size->type = CSSValueFontSize::type_larger;
		else if (equals(token.value, "inherit"))
			size->type = CSSValueFontSize::type_inherit;
		else
		{
			debug_parse_error(propname, tokens);
			return;
		}
	}
	else if (is_length(token))
	{
		CSSLength length;
		if (parse_length(token, length))
		{
			size->type = CSSValueFontSize::type_length;
			size->length = length;
		}
		else
		{
			debug_parse_error(propname, tokens);
			return;
		}
	}
	else if (token.type == CSSToken::type_percentage)
	{
		size->type = CSSValueFontSize::type_percentage;
		size->percentage = StringHelp::text_to_float(token.value);
	}
	else
	{
		debug_parse_error(propname, tokens);
		return;
	}

	token = next_token(pos, tokens);
	if (token.type == CSSToken::type_delim && token.value == "/")
	{
		token = next_token(pos, tokens);

		if (token.type == CSSToken::type_ident)
		{
			if (equals(token.value, "normal"))
				line_height->type = CSSValueLineHeight::type_normal;
			else if (equals(token.value, "inherit"))
				line_height->type = CSSValueLineHeight::type_inherit;
			else
			{
				debug_parse_error(propname, tokens);
				return;
			}
		}
		else if (token.type == CSSToken::type_number)
		{
			line_height->type = CSSValueLineHeight::type_number;
			line_height->number = StringHelp::text_to_float(token.value);
		}
		else if (is_length(token))
		{
			CSSLength length;
			if (parse_length(token, length))
			{
				line_height->type = CSSValueLineHeight::type_length;
				line_height->length = length;
			}
			else
			{
				debug_parse_error(propname, tokens);
				return;
			}
		}
		else if (token.type == CSSToken::type_percentage)
		{
			line_height->type = CSSValueLineHeight::type_percentage;
			line_height->percentage = StringHelp::text_to_float(token.value);
		}
		else
		{
			debug_parse_error(propname, tokens);
			return;
		}

		token = next_token(pos, tokens);
	}

	family->names.clear();
	while (true)
	{
		if (token.type == CSSToken::type_ident)
		{
			CSSValueFontFamilyName name;
			if (equals(token.value, "serif"))
			{
				name.type = CSSValueFontFamilyName::type_serif;
			}
			else if (equals(token.value, "sans-serif"))
			{
				name.type = CSSValueFontFamilyName::type_sans_serif;
			}
			else if (equals(token.value, "cursive"))
			{
				name.type = CSSValueFontFamilyName::type_cursive;
			}
			else if (equals(token.value, "fantasy"))
			{
				name.type = CSSValueFontFamilyName::type_fantasy;
			}
			else if (equals(token.value, "monospace"))
			{
				name.type = CSSValueFontFamilyName::type_monospace;
			}
			else if (equals(token.value, "default"))
			{
				// reserved for future use
				return;
			}
			else if (equals(token.value, "initial"))
			{
				// reserved for future use
				return;
			}
			else
			{
				name.type = CSSValueFontFamilyName::type_family_name;
			}

			if (name.type == CSSValueFontFamilyName::type_family_name)
			{
				name.name = token.value;
				while (pos != tokens.size())
				{
					token = tokens[pos++];
					if (token.type == CSSToken::type_whitespace)
					{
						name.name += " ";
					}
					else if (token.type == CSSToken::type_ident)
					{
						name.name += token.value;
					}
					else if (token.type == CSSToken::type_delim && token.value == ",")
					{
						break;
					}
				}
				family->names.push_back(name);
				if (pos == tokens.size())
					break;
				token = next_token(pos, tokens);
			}
			else
			{
				family->names.push_back(name);

				if (pos == tokens.size())
					break;
				token = next_token(pos, tokens);
				if (token.type != CSSToken::type_delim || token.value != ",")
				{
					debug_parse_error(propname, tokens);
					return;
				}
				token = next_token(pos, tokens);
			}
		}
		else if (token.type == CSSToken::type_string)
		{
			CSSValueFontFamilyName name;
			name.type = CSSValueFontFamilyName::type_family_name;
			name.name = token.value;
			family->names.push_back(name);

			if (pos == tokens.size())
				break;
			token = next_token(pos, tokens);
			if (token.type != CSSToken::type_delim || token.value != ",")
			{
				debug_parse_error(propname, tokens);
				return;
			}
			token = next_token(pos, tokens);
		}
		else
		{
			debug_parse_error(propname, tokens);
			return;
		}
	}

	inout_values.push_back(std::move(style));
	inout_values.push_back(std::move(variant));
	inout_values.push_back(std::move(weight));
	inout_values.push_back(std::move(size));
	inout_values.push_back(std::move(line_height));
	inout_values.push_back(std::move(family));
}

}
