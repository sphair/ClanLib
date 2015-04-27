/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
#include "margin.h"

namespace clan
{
	void force_link_style_parser_margin()
	{
	}

	StylePropertyDefault style_default_margin_left("margin-left", StyleGetValue::from_length(0.0f));
	StylePropertyDefault style_default_margin_top("margin-top", StyleGetValue::from_length(0.0f));
	StylePropertyDefault style_default_margin_right("margin-right", StyleGetValue::from_length(0.0f));
	StylePropertyDefault style_default_margin_bottom("margin-bottom", StyleGetValue::from_length(0.0f));

	MarginPropertyParser style_parser_margin;
	MarginLTRBPropertyParser style_parser_margin_ltrb;

	void MarginPropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		StyleValue margin_widths[4];
		int count;
		size_t pos = 0;
		for (count = 0; count < 4; count++)
		{
			StyleToken token = next_token(pos, tokens);
			if (token.type == StyleTokenType::ident && equals(token.value, "auto"))
			{
				margin_widths[count] = StyleValue::from_keyword("auto");
			}
			else if (token.type == StyleTokenType::ident && equals(token.value, "inherit") && count == 0 && pos == tokens.size())
			{
				margin_widths[0] = StyleValue::from_keyword("inherit");
				setter->set_value("margin-left", margin_widths[0]);
				setter->set_value("margin-top", margin_widths[0]);
				setter->set_value("margin-right", margin_widths[0]);
				setter->set_value("margin-bottom", margin_widths[0]);
				return;
			}
			else if (is_length(token))
			{
				StyleValue length;
				if (parse_length(token, length))
				{
					margin_widths[count] = length;
				}
				else
				{
					debug_parse_error(name, tokens);
					return;
				}
			}
			else if (token.type == StyleTokenType::percentage)
			{
				margin_widths[count] = StyleValue::from_percentage(StringHelp::text_to_float(token.value));
			}
			else if (token.type == StyleTokenType::delim && token.value == "-")
			{
				token = next_token(pos, tokens);
				if (is_length(token))
				{
					StyleValue length;
					if (parse_length(token, length))
					{
						length.number = -length.number;
						margin_widths[count] = length;
					}
					else
					{
						debug_parse_error(name, tokens);
						return;
					}
				}
				else if (token.type == StyleTokenType::percentage)
				{
					margin_widths[count] = StyleValue::from_percentage(-StringHelp::text_to_float(token.value));
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
				setter->set_value("margin-left", margin_widths[0]);
				setter->set_value("margin-top", margin_widths[0]);
				setter->set_value("margin-right", margin_widths[0]);
				setter->set_value("margin-bottom", margin_widths[0]);
				break;
			case 2:
				setter->set_value("margin-top", margin_widths[0]);
				setter->set_value("margin-bottom", margin_widths[0]);
				setter->set_value("margin-left", margin_widths[1]);
				setter->set_value("margin-right", margin_widths[1]);
				break;
			case 3:
				setter->set_value("margin-top", margin_widths[0]);
				setter->set_value("margin-left", margin_widths[1]);
				setter->set_value("margin-right", margin_widths[1]);
				setter->set_value("margin-bottom", margin_widths[2]);
				break;
			case 4:
				setter->set_value("margin-top", margin_widths[0]);
				setter->set_value("margin-right", margin_widths[1]);
				setter->set_value("margin-bottom", margin_widths[2]);
				setter->set_value("margin-left", margin_widths[3]);
				break;
			default:
				break;
			}
		}
	}

	void MarginLTRBPropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		StyleValue width;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "auto"))
				width = StyleValue::from_keyword("auto");
			else if (equals(token.value, "inherit"))
				width = StyleValue::from_keyword("inherit");
			else
				return;
		}
		else if (is_length(token) && pos == tokens.size())
		{
			StyleValue length;
			if (parse_length(token, length))
			{
				width = length;
			}
			else
			{
				return;
			}
		}
		else if (token.type == StyleTokenType::percentage && pos == tokens.size())
		{
			width = StyleValue::from_percentage(StringHelp::text_to_float(token.value));
		}
		else if (token.type == StyleTokenType::delim && token.value == "-")
		{
			token = next_token(pos, tokens);
			if (is_length(token) && pos == tokens.size())
			{
				StyleValue length;
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
				width = StyleValue::from_percentage(-StringHelp::text_to_float(token.value));
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
