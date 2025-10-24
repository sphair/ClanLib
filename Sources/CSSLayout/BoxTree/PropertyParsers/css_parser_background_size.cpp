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
#include "css_parser_background_size.h"
#include "API/CSSLayout/css_box_properties.h"

std::vector<CL_String> CL_CSSParserBackgroundSize::get_names()
{
	std::vector<CL_String> names;
	names.push_back("background-size");
	return names;
}

void CL_CSSParserBackgroundSize::parse(CL_CSSBoxProperties &properties, const CL_String &name, const std::vector<CL_CSSToken> &tokens, std::map<CL_String, CL_CSSBoxProperty *> *out_change_set)
{
	size_t pos = 0;
	CL_CSSToken token = next_token(pos, tokens);

	if (token.type == CL_CSSToken::type_ident && pos == tokens.size() && equals(token.value, "inherit"))
	{
		properties.background_size.type = CL_CSSBoxBackgroundSize::type_inherit;
	}
	else
	{
		CL_CSSBoxBackgroundSize background_size;
		background_size.type = CL_CSSBoxBackgroundSize::type_value;
		background_size.values.clear();
		while (true)
		{
			CL_CSSBoxBackgroundSize::Size size;

			bool single_value = false;
			if (token.type == CL_CSSToken::type_ident)
			{
				if (equals(token.value, "contain"))
				{
					size.type = CL_CSSBoxBackgroundSize::size_contain;
					single_value = true;
				}
				else if (equals(token.value, "cover"))
				{
					size.type = CL_CSSBoxBackgroundSize::size_cover;
					single_value = true;
				}
				else if (equals(token.value, "auto"))
				{
					size.type = CL_CSSBoxBackgroundSize::size_values;
					size.value_x = CL_CSSBoxBackgroundSize::value_type_auto;
				}
				else
				{
					return;
				}
			}
			else if (is_length(token))
			{
				CL_CSSBoxLength length;
				if (parse_length(token, length))
				{
					size.value_x = CL_CSSBoxBackgroundSize::value_type_length;
					size.length_x = length;
				}
				else
				{
					return;
				}
			}
			else if (token.type == CL_CSSToken::type_percentage)
			{
				size.value_x = CL_CSSBoxBackgroundSize::value_type_percentage;
				size.percentage_x = CL_StringHelp::text_to_float(token.value);
			}
			else
			{
				return;
			}

			if (pos == tokens.size())
			{
				background_size.values.push_back(size);
				break;
			}

			token = next_token(pos, tokens);
			if (token.type != CL_CSSToken::type_delim || token.value != ",")
			{
				if (single_value)
				{
					return;
				}
				else if (token.type == CL_CSSToken::type_ident && equals(token.value, "auto"))
				{
					size.value_y = CL_CSSBoxBackgroundSize::value_type_auto;
				}
				else if (is_length(token))
				{
					CL_CSSBoxLength length;
					if (parse_length(token, length))
					{
						size.value_y = CL_CSSBoxBackgroundSize::value_type_length;
						size.length_y = length;
					}
					else
					{
						return;
					}
				}
				else if (token.type == CL_CSSToken::type_percentage)
				{
					size.value_y = CL_CSSBoxBackgroundSize::value_type_percentage;
					size.percentage_y = CL_StringHelp::text_to_float(token.value);
				}
				else
				{
					return;
				}

				if (pos == tokens.size())
				{
					background_size.values.push_back(size);
					break;
				}

				token = next_token(pos, tokens);
			}

			background_size.values.push_back(size);

			if (token.type != CL_CSSToken::type_delim || token.value != ",")
				return;

			token = next_token(pos, tokens);
		}

		properties.background_size = background_size;
	}
	if (out_change_set)
	{
		(*out_change_set)["background-size"] = &properties.background_size;
	}
}
