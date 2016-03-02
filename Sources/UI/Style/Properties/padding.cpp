/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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

#include "UI/precomp.h"
#include "API/Core/Text/string_help.h"
#include "padding.h"

namespace clan
{
	void force_link_style_parser_padding()
	{
	}

	StylePropertyDefault style_default_padding_left("padding-left", StyleGetValue::from_length(0.0f), false);
	StylePropertyDefault style_default_padding_top("padding-top", StyleGetValue::from_length(0.0f), false);
	StylePropertyDefault style_default_padding_right("padding-right", StyleGetValue::from_length(0.0f), false);
	StylePropertyDefault style_default_padding_bottom("padding-bottom", StyleGetValue::from_length(0.0f), false);

	PaddingPropertyParser style_parser_padding;
	PaddingLTRBPropertyParser style_parser_padding_ltrb;

	void PaddingPropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		StyleSetValue padding_widths[4];

		int count;
		size_t pos = 0;
		for (count = 0; count < 4; count++)
		{
			StyleToken token = next_token(pos, tokens);
			if (token.type == StyleTokenType::ident && equals(token.value, "inherit") && count == 0 && pos == tokens.size())
			{
				padding_widths[0] = StyleSetValue::from_keyword("inherit");
				setter->set_value("padding-left", padding_widths[0]);
				setter->set_value("padding-top", padding_widths[0]);
				setter->set_value("padding-right", padding_widths[0]);
				setter->set_value("padding-bottom", padding_widths[0]);
				return;
			}
			else if (is_length(token))
			{
				StyleSetValue length;
				if (parse_length(token, length))
				{
					padding_widths[count] = length;
				}
				else
				{
					debug_parse_error(name, tokens);
					return;
				}
			}
			else if (token.type == StyleTokenType::percentage)
			{
				padding_widths[count] = StyleSetValue::from_percentage(StringHelp::text_to_float(token.value));
			}
			else if (token.type == StyleTokenType::delim && token.value == "-")
			{
				token = next_token(pos, tokens);
				if (is_length(token))
				{
					StyleSetValue length;
					if (parse_length(token, length))
					{
						length.number = -length.number;
						padding_widths[count] = length;
					}
					else
					{
						debug_parse_error(name, tokens);
						return;
					}
				}
				else if (token.type == StyleTokenType::percentage)
				{
					padding_widths[count] = StyleSetValue::from_percentage(-StringHelp::text_to_float(token.value));
				}
				else
				{
					debug_parse_error(name, tokens);
					return;
				}
			}
			else if (token.type == StyleTokenType::null)
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
				setter->set_value("padding-left", padding_widths[0]);
				setter->set_value("padding-top", padding_widths[0]);
				setter->set_value("padding-right", padding_widths[0]);
				setter->set_value("padding-bottom", padding_widths[0]);
				break;
			case 2:
				setter->set_value("padding-top", padding_widths[0]);
				setter->set_value("padding-bottom", padding_widths[0]);
				setter->set_value("padding-left", padding_widths[1]);
				setter->set_value("padding-right", padding_widths[1]);
				break;
			case 3:
				setter->set_value("padding-top", padding_widths[0]);
				setter->set_value("padding-left", padding_widths[1]);
				setter->set_value("padding-right", padding_widths[1]);
				setter->set_value("padding-bottom", padding_widths[2]);
				break;
			case 4:
				setter->set_value("padding-top", padding_widths[0]);
				setter->set_value("padding-right", padding_widths[1]);
				setter->set_value("padding-bottom", padding_widths[2]);
				setter->set_value("padding-left", padding_widths[3]);
				break;
			default:
				break;
			}
		}
	}

	void PaddingLTRBPropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		StyleSetValue width = StyleSetValue::from_length(0.0f);

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "inherit"))
				width = StyleSetValue::from_keyword("inherit");
			else
				return;
		}
		else if (is_length(token) && pos == tokens.size())
		{
			if (!parse_length(token, width))
			{
				return;
			}
		}
		else if (token.type == StyleTokenType::percentage && pos == tokens.size())
		{
			width = StyleSetValue::from_percentage(StringHelp::text_to_float(token.value));
		}
		else if (token.type == StyleTokenType::delim && token.value == "-")
		{
			token = next_token(pos, tokens);
			if (is_length(token) && pos == tokens.size())
			{
				StyleSetValue length;
				if (parse_length(token, length))
				{
					length.number = -length.number;
					width = length;
				}
				else
				{
					return;
				}
			}
			else if (token.type == StyleTokenType::percentage && pos == tokens.size())
			{
				width = StyleSetValue::from_percentage(-StringHelp::text_to_float(token.value));
			}
			else
			{
				return;
			}
		}
		else
		{
			return;
		}

		setter->set_value(name, width);
	}
}
