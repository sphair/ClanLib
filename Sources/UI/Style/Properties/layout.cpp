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
#include "layout.h"

namespace clan
{
	void force_link_style_parser_layout()
	{
	}

	StylePropertyDefault style_default_layout("layout", StyleValue::from_keyword("flex"));
	StylePropertyDefault style_default_position("position", StyleValue::from_keyword("static"));
	StylePropertyDefault style_default_left("left", StyleValue::from_keyword("auto"));
	StylePropertyDefault style_default_top("top", StyleValue::from_keyword("auto"));
	StylePropertyDefault style_default_right("right", StyleValue::from_keyword("auto"));
	StylePropertyDefault style_default_bottom("bottom", StyleValue::from_keyword("auto"));
	StylePropertyDefault style_default_zindex("z-index", StyleValue::from_keyword("auto"));

	LayoutPropertyParser style_parser_layout;
	PositionPropertyParser style_parser_position;
	LeftPropertyParser style_parser_left;
	TopPropertyParser style_parser_top;
	RightPropertyParser style_parser_right;
	BottomPropertyParser style_parser_bottom;
	ZIndexPropertyParser style_parser_zindex;

	void LayoutPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		std::vector<StyleToken> tokens = StyleTokenizer::tokenize(value);

		StyleValue layout;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "none"))
				layout = StyleValue::from_keyword("none");
			else if (equals(token.value, "flex"))
				layout = StyleValue::from_keyword("flex");
			else if (equals(token.value, "block"))
				layout = StyleValue::from_keyword("block");
			else if (equals(token.value, "inline-block"))
				layout = StyleValue::from_keyword("inline-block");
			else if (equals(token.value, "inherit"))
				layout = StyleValue::from_keyword("inherit");
			else
				return;
		}
		else
		{
			return;
		}

		setter->set_value("layout", layout);
	}

	void PositionPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		std::vector<StyleToken> tokens = StyleTokenizer::tokenize(value);

		StyleValue position;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "static"))
				position = StyleValue::from_keyword("static");
			else if (equals(token.value, "relative"))
				position = StyleValue::from_keyword("relative");
			else if (equals(token.value, "absolute"))
				position = StyleValue::from_keyword("absolute");
			else if (equals(token.value, "fixed"))
				position = StyleValue::from_keyword("fixed");
			else if (equals(token.value, "inherit"))
				position = StyleValue::from_keyword("inherit");
			else
				return;
		}
		else
		{
			return;
		}

		setter->set_value("position", position);
	}

	void LeftPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		std::vector<StyleToken> tokens = StyleTokenizer::tokenize(value);

		StyleValue left;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "auto"))
				left = StyleValue::from_keyword("auto");
			else if (equals(token.value, "inherit"))
				left = StyleValue::from_keyword("inherit");
			else
				return;
		}
		else if (is_length(token) && pos == tokens.size())
		{
			StyleValue length;
			if (parse_length(token, length))
			{
				left = length;
			}
			else
			{
				return;
			}
		}
		else if (token.type == StyleTokenType::percentage && pos == tokens.size())
		{
			left = StyleValue::from_percentage(StringHelp::text_to_float(token.value));
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
					left = length;
				}
				else
				{
					return;
				}
			}
			else if (token.type == StyleTokenType::percentage && pos == tokens.size())
			{
				left = StyleValue::from_percentage(-StringHelp::text_to_float(token.value));
			}
			else
			{
				return;
			}
		}

		setter->set_value("left", left);
	}

	void TopPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		std::vector<StyleToken> tokens = StyleTokenizer::tokenize(value);

		StyleValue top;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "auto"))
				top = StyleValue::from_keyword("auto");
			else if (equals(token.value, "inherit"))
				top = StyleValue::from_keyword("inherit");
			else
				return;
		}
		else if (is_length(token) && pos == tokens.size())
		{
			StyleValue length;
			if (parse_length(token, length))
			{
				top = length;
			}
			else
			{
				return;
			}
		}
		else if (token.type == StyleTokenType::percentage && pos == tokens.size())
		{
			top = StyleValue::from_percentage(StringHelp::text_to_float(token.value));
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
					top = length;
				}
				else
				{
					return;
				}
			}
			else if (token.type == StyleTokenType::percentage && pos == tokens.size())
			{
				top = StyleValue::from_percentage(-StringHelp::text_to_float(token.value));
			}
			else
			{
				return;
			}
		}

		setter->set_value("top", top);
	}

	void RightPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		std::vector<StyleToken> tokens = StyleTokenizer::tokenize(value);

		StyleValue right;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "auto"))
				right = StyleValue::from_keyword("auto");
			else if (equals(token.value, "inherit"))
				right = StyleValue::from_keyword("inherit");
			else
				return;
		}
		else if (is_length(token) && pos == tokens.size())
		{
			StyleValue length;
			if (parse_length(token, length))
			{
				right = length;
			}
			else
			{
				return;
			}
		}
		else if (token.type == StyleTokenType::percentage && pos == tokens.size())
		{
			right = StyleValue::from_percentage(StringHelp::text_to_float(token.value));
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
					right = length;
				}
				else
				{
					return;
				}
			}
			else if (token.type == StyleTokenType::percentage && pos == tokens.size())
			{
				right = StyleValue::from_percentage(-StringHelp::text_to_float(token.value));
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

		setter->set_value("right", right);
	}

	void BottomPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		std::vector<StyleToken> tokens = StyleTokenizer::tokenize(value);

		StyleValue bottom;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "auto"))
				bottom = StyleValue::from_keyword("auto");
			else if (equals(token.value, "inherit"))
				bottom = StyleValue::from_keyword("inherit");
			else
				return;
		}
		else if (is_length(token) && pos == tokens.size())
		{
			StyleValue length;
			if (parse_length(token, length))
			{
				bottom = length;
			}
			else
			{
				return;
			}
		}
		else if (token.type == StyleTokenType::percentage && pos == tokens.size())
		{
			bottom = StyleValue::from_percentage(StringHelp::text_to_float(token.value));
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
					bottom = length;
				}
				else
				{
					return;
				}
			}
			else if (token.type == StyleTokenType::percentage && pos == tokens.size())
			{
				bottom = StyleValue::from_percentage(-StringHelp::text_to_float(token.value));
			}
			else
			{
				return;
			}
		}

		setter->set_value("bottom", bottom);
	}

	void ZIndexPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		std::vector<StyleToken> tokens = StyleTokenizer::tokenize(value);

		StyleValue z_index;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "auto"))
				z_index = StyleValue::from_keyword("auto");
			else if (equals(token.value, "inherit"))
				z_index = StyleValue::from_keyword("inherit");
			else
				return;
		}
		else if (token.type == StyleTokenType::number && pos == tokens.size())
		{
			int value = 0;
			if (parse_integer(token.value, value))
			{
				z_index = StyleValue::from_number((float)value);
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

		setter->set_value("z-index", z_index);
	}
}
