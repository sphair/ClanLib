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
#include "flex.h"

namespace clan
{
	void force_link_style_parser_flex()
	{
	}

	FlexPropertyParser style_parser_flex;
	FlexBasisPropertyParser style_parser_flex_basis;
	FlexDirectionPropertyParser style_parser_flex_direction;
	FlexFlowPropertyParser style_parser_flex_flow;
	FlexGrowPropertyParser style_parser_flex_grow;
	FlexShrinkPropertyParser style_parser_flex_shrink;
	FlexWrapPropertyParser style_parser_flex_wrap;
	OrderPropertyParser style_parser_order;

	void FlexPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		std::vector<StyleToken> tokens = StyleTokenizer::tokenize(value);

		StyleValue flex_grow;
		StyleValue flex_shrink;
		StyleValue flex_basis;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "none"))
			{
				flex_grow = StyleValue::from_number(0.0f);
				flex_shrink = StyleValue::from_number(0.0f);
				flex_basis = StyleValue::from_keyword("main-size");

				setter->set_value("flex-grow", flex_grow);
				setter->set_value("flex-shrink", flex_shrink);
				setter->set_value("flex-basis", flex_basis);
			}
			else if (equals(token.value, "inherit"))
			{
				flex_grow = StyleValue::from_keyword("inherit");
				flex_shrink = StyleValue::from_keyword("inherit");
				flex_basis = StyleValue::from_keyword("inherit");

				setter->set_value("flex-grow", flex_grow);
				setter->set_value("flex-shrink", flex_shrink);
				setter->set_value("flex-basis", flex_basis);
			}
			else
			{
				return;
			}
		}
		else
		{
			pos = 0;

			flex_grow = StyleValue::from_number(1.0f);
			flex_shrink = StyleValue::from_number(1.0f);
			flex_basis = StyleValue::from_length(0.0f);

			bool grow_shrink_specified = false;
			bool basis_specified = false;

			do
			{
				if (!grow_shrink_specified && parse_grow_shrink(flex_grow, flex_shrink, pos, tokens))
				{
					grow_shrink_specified = true;
				}
				else if (!basis_specified && parse_basis(flex_basis, pos, tokens))
				{
					basis_specified = true;
				}
				else
				{
					return;
				}
			} while (pos != tokens.size());

			setter->set_value("flex-grow", flex_grow);
			setter->set_value("flex-shrink", flex_shrink);
			setter->set_value("flex-basis", flex_basis);
		}
	}

	bool FlexPropertyParser::parse_grow_shrink(StyleValue &grow, StyleValue &shrink, size_t &parse_pos, const std::vector<StyleToken> &tokens)
	{
		size_t pos = parse_pos;
		StyleToken token = next_token(pos, tokens);

		if (token.type == StyleTokenType::number)
		{
			grow = StyleValue::from_number(StringHelp::text_to_float(token.value));
		}
		else
		{
			return false;
		}

		parse_pos = pos;

		if (pos != tokens.size())
		{
			token = next_token(pos, tokens);

			if (token.type == StyleTokenType::number)
			{
				shrink = StyleValue::from_number(StringHelp::text_to_float(token.value));

				parse_pos = pos;
			}
		}

		return true;
	}

	bool FlexPropertyParser::parse_basis(StyleValue &basis, size_t &parse_pos, const std::vector<StyleToken> &tokens)
	{
		size_t pos = parse_pos;
		StyleToken token = next_token(pos, tokens);

		if (token.type == StyleTokenType::ident)
		{
			if (equals(token.value, "main-size"))
				basis = StyleValue::from_keyword("main-size");
			else
				return false;
		}
		else if (is_length(token))
		{
			StyleValue length;
			if (parse_length(token, length) && length.number >= 0.0f)
			{
				basis = length;
			}
		}
		else if (token.type == StyleTokenType::percentage)
		{
			float v = StringHelp::text_to_float(token.value);
			if (v >= 0.0f)
			{
				basis = StyleValue::from_percentage(v);
			}
		}
		else
		{
			return false;
		}

		parse_pos = pos;
		return true;
	}

	void FlexBasisPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		std::vector<StyleToken> tokens = StyleTokenizer::tokenize(value);

		StyleValue flex_basis;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "main-size"))
				flex_basis = StyleValue::from_keyword("main-size");
			else if (equals(token.value, "inherit"))
				flex_basis = StyleValue::from_keyword("inherit");
			else
				return;
		}
		else if (is_length(token) && pos == tokens.size())
		{
			StyleValue length;
			if (parse_length(token, length) && length.number >= 0.0f)
			{
				flex_basis = length;
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
				flex_basis = StyleValue::from_percentage(v);
			}
			else
			{
				return;
			}
		}

		setter->set_value("flex-basis", flex_basis);
	}

	void FlexDirectionPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		std::vector<StyleToken> tokens = StyleTokenizer::tokenize(value);

		StyleValue flex_direction;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "row"))
				flex_direction = StyleValue::from_keyword("row");
			else if (equals(token.value, "row-reverse"))
				flex_direction = StyleValue::from_keyword("row-reverse");
			else if (equals(token.value, "column"))
				flex_direction = StyleValue::from_keyword("column");
			else if (equals(token.value, "column-reverse"))
				flex_direction = StyleValue::from_keyword("column-reverse");
			else if (equals(token.value, "inherit"))
				flex_direction = StyleValue::from_keyword("inherit");
			else
				return;
		}
		else
		{
			return;
		}

		setter->set_value("flex-direction", flex_direction);
	}

	void FlexFlowPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		std::vector<StyleToken> tokens = StyleTokenizer::tokenize(value);

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);

		StyleValue direction = StyleValue::from_keyword("row");
		StyleValue wrap = StyleValue::from_keyword("nowrap");

		bool direction_specified = false;
		bool wrap_specified = false;

		do
		{
			if (!direction_specified && parse_direction(direction, pos, tokens))
			{
				direction_specified = true;
			}
			else if (!wrap_specified && parse_wrap(wrap, pos, tokens))
			{
				wrap_specified = true;
			}
			else
			{
				return;
			}
		} while (pos != tokens.size());

		setter->set_value("flex-direction", direction);
		setter->set_value("flex-wrap", wrap);
	}

	bool FlexFlowPropertyParser::parse_direction(StyleValue &direction, size_t &parse_pos, const std::vector<StyleToken> &tokens)
	{
		size_t pos = parse_pos;
		StyleToken token = next_token(pos, tokens);

		if (token.type == StyleTokenType::ident)
		{
			if (equals(token.value, "row"))
				direction = StyleValue::from_keyword("row");
			else if (equals(token.value, "row-reverse"))
				direction = StyleValue::from_keyword("row-reverse");
			else if (equals(token.value, "column"))
				direction = StyleValue::from_keyword("column");
			else if (equals(token.value, "column-reverse"))
				direction = StyleValue::from_keyword("column-reverse");
			else if (equals(token.value, "inherit"))
				direction = StyleValue::from_keyword("inherit");
			else
				return false;
		}
		else
		{
			return false;
		}

		parse_pos = pos;
		return true;
	}

	bool FlexFlowPropertyParser::parse_wrap(StyleValue &wrap, size_t &parse_pos, const std::vector<StyleToken> &tokens)
	{
		size_t pos = parse_pos;
		StyleToken token = next_token(pos, tokens);

		if (token.type == StyleTokenType::ident)
		{
			if (equals(token.value, "nowrap"))
				wrap = StyleValue::from_keyword("nowrap");
			else if (equals(token.value, "wrap"))
				wrap = StyleValue::from_keyword("wrap");
			else if (equals(token.value, "wrap-reverse"))
				wrap = StyleValue::from_keyword("wrap-reverse");
			else if (equals(token.value, "inherit"))
				wrap = StyleValue::from_keyword("inherit");
			else
				return false;
		}
		else
		{
			return false;
		}

		parse_pos = pos;
		return true;
	}

	void FlexGrowPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		std::vector<StyleToken> tokens = StyleTokenizer::tokenize(value);

		StyleValue flex_grow;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "inherit"))
				flex_grow = StyleValue::from_keyword("inherit");
			else
				return;
		}
		else if (token.type == StyleTokenType::number && pos == tokens.size())
		{
			flex_grow = StyleValue::from_number(StringHelp::text_to_float(token.value));
		}
		else
		{
			return;
		}

		setter->set_value("flex-grow", flex_grow);
	}

	void FlexShrinkPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		std::vector<StyleToken> tokens = StyleTokenizer::tokenize(value);

		StyleValue flex_shrink;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "inherit"))
				flex_shrink = StyleValue::from_keyword("inherit");
			else
				return;
		}
		else if (token.type == StyleTokenType::number && pos == tokens.size())
		{
			flex_shrink = StyleValue::from_number(StringHelp::text_to_float(token.value));
		}
		else
		{
			return;
		}
	}

	void FlexWrapPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		std::vector<StyleToken> tokens = StyleTokenizer::tokenize(value);

		StyleValue flex_wrap;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "nowrap"))
				flex_wrap = StyleValue::from_keyword("nowrap");
			else if (equals(token.value, "wrap"))
				flex_wrap = StyleValue::from_keyword("wrap");
			else if (equals(token.value, "wrap-reverse"))
				flex_wrap = StyleValue::from_keyword("wrap-reverse");
			else if (equals(token.value, "inherit"))
				flex_wrap = StyleValue::from_keyword("inherit");
			else
				return;
		}
		else
		{
			return;
		}

		setter->set_value("flex-wrap", flex_wrap);
	}

	void OrderPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		std::vector<StyleToken> tokens = StyleTokenizer::tokenize(value);

		StyleValue order;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "inherit"))
				order = StyleValue::from_keyword("inherit");
			else
				return;
		}
		else if (token.type == StyleTokenType::number && pos == tokens.size())
		{
			int value = 0;
			if (parse_integer(token.value, value))
			{
				order = StyleValue::from_number((float)value);
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

		setter->set_value("order", order);
	}
}
