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
#include "content.h"

namespace clan
{
	void force_link_style_parser_content()
	{
	}

	StylePropertyDefault style_default_width("width", StyleGetValue::from_keyword("auto"));
	StylePropertyDefault style_default_height("height", StyleGetValue::from_keyword("auto"));
	StylePropertyDefault style_default_min_width("min-width", StyleGetValue::from_length(0.0f));
	StylePropertyDefault style_default_min_height("min-height", StyleGetValue::from_length(0.0f));
	StylePropertyDefault style_default_max_width("max-width", StyleGetValue::from_keyword("none"));
	StylePropertyDefault style_default_max_height("max-height", StyleGetValue::from_keyword("none"));

	WidthPropertyParser style_parser_width;
	HeightPropertyParser style_parser_height;
	MinWidthPropertyParser style_parser_min_width;
	MinHeightPropertyParser style_parser_min_height;
	MaxWidthPropertyParser style_parser_max_width;
	MaxHeightPropertyParser style_parser_max_height;

	void WidthPropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		StyleSetValue width;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "auto"))
				width = StyleSetValue::from_keyword("auto");
			else if (equals(token.value, "inherit"))
				width = StyleSetValue::from_keyword("inherit");
			else
				return;
		}
		else if (is_length(token) && pos == tokens.size())
		{
			StyleSetValue length;
			if (parse_length(token, length) && length.number >= 0.0f)
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
			float v = StringHelp::text_to_float(token.value);
			if (v >= 0.0f)
			{
				width = StyleSetValue::from_percentage(v);
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

		setter->set_value("width", width);
	}

	void HeightPropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		StyleSetValue height;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "auto"))
				height = StyleSetValue::from_keyword("auto");
			else if (equals(token.value, "inherit"))
				height = StyleSetValue::from_keyword("inherit");
			else
				return;
		}
		else if (is_length(token) && pos == tokens.size())
		{
			StyleSetValue length;
			if (parse_length(token, length) && length.number >= 0.0f)
			{
				height = length;
			}
			else
			{
				return;
			}
		}
		else if (token.type == StyleTokenType::percentage && pos == tokens.size())
		{
			float v = StringHelp::text_to_float(token.value);
			if (v >= 0.0f)
			{
				height = StyleSetValue::from_percentage(v);
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

		setter->set_value("height", height);
	}

	void MinWidthPropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		StyleSetValue min_width;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "inherit"))
				min_width = StyleSetValue::from_keyword("inherit");
			else if (equals(token.value, "auto"))
				min_width = StyleSetValue::from_keyword("auto");
			else
				return;
		}
		else if (is_length(token) && pos == tokens.size())
		{
			StyleSetValue length;
			if (parse_length(token, length))
			{
				min_width = length;
			}
			else
			{
				return;
			}
		}
		else if (token.type == StyleTokenType::percentage && pos == tokens.size())
		{
			min_width = StyleSetValue::from_percentage(StringHelp::text_to_float(token.value));
		}
		else
		{
			return;
		}

		setter->set_value("min-width", min_width);
	}

	void MinHeightPropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		StyleSetValue min_height;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "inherit"))
				min_height = StyleSetValue::from_keyword("inherit");
			else if (equals(token.value, "auto"))
				min_height = StyleSetValue::from_keyword("auto");
			else
				return;
		}
		else if (is_length(token) && pos == tokens.size())
		{
			StyleSetValue length;
			if (parse_length(token, length))
			{
				min_height = length;
			}
			else
			{
				return;
			}
		}
		else if (token.type == StyleTokenType::percentage && pos == tokens.size())
		{
			min_height = StyleSetValue::from_percentage(StringHelp::text_to_float(token.value));
		}
		else
		{
			return;
		}

		setter->set_value("min-height", min_height);
	}

	void MaxWidthPropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		StyleSetValue max_width;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "none"))
				max_width = StyleSetValue::from_keyword("none");
			else if (equals(token.value, "inherit"))
				max_width = StyleSetValue::from_keyword("inherit");
			else
				return;
		}
		else if (is_length(token) && pos == tokens.size())
		{
			StyleSetValue length;
			if (parse_length(token, length))
			{
				max_width = length;
			}
			else
			{
				return;
			}
		}
		else if (token.type == StyleTokenType::percentage && pos == tokens.size())
		{
			max_width = StyleSetValue::from_percentage(StringHelp::text_to_float(token.value));
		}
		else
		{
			return;
		}

		setter->set_value("max-width", max_width);
	}

	void MaxHeightPropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		StyleSetValue max_height;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "none"))
				max_height = StyleSetValue::from_keyword("none");
			else if (equals(token.value, "inherit"))
				max_height = StyleSetValue::from_keyword("inherit");
			else
				return;
		}
		else if (is_length(token) && pos == tokens.size())
		{
			StyleSetValue length;
			if (parse_length(token, length))
			{
				max_height = length;
			}
			else
			{
				return;
			}
		}
		else if (token.type == StyleTokenType::percentage && pos == tokens.size())
		{
			max_height = StyleSetValue::from_percentage(StringHelp::text_to_float(token.value));
		}
		else
		{
			return;
		}

		setter->set_value("max-height", max_height);
	}
}
