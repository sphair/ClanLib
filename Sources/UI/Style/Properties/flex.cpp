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

	StylePropertyDefault style_default_flex_grow("flex-grow", StyleGetValue::from_number(0.0f), false);
	StylePropertyDefault style_default_flex_shrink("flex-shrink", StyleGetValue::from_number(1.0f), false);
	StylePropertyDefault style_default_flex_basis("flex-basis", StyleGetValue::from_keyword("main-size"), false);
	StylePropertyDefault style_default_flex_direction("flex-direction", StyleGetValue::from_keyword("row"), false);
	StylePropertyDefault style_default_flex_wrap("flex-wrap", StyleGetValue::from_keyword("nowrap"), false);
	StylePropertyDefault style_default_order("order", StyleGetValue::from_number(0.0f), false);
	StylePropertyDefault style_default_align_content("align-content", StyleGetValue::from_keyword("stretch"), false);
	StylePropertyDefault style_default_align_items("align-items", StyleGetValue::from_keyword("stretch"), false);
	StylePropertyDefault style_default_align_self("align-self", StyleGetValue::from_keyword("auto"), false);
	StylePropertyDefault style_default_justify_content("justify-content", StyleGetValue::from_keyword("flex-start"), false);

	FlexPropertyParser style_parser_flex;
	FlexBasisPropertyParser style_parser_flex_basis;
	FlexDirectionPropertyParser style_parser_flex_direction;
	FlexFlowPropertyParser style_parser_flex_flow;
	FlexGrowPropertyParser style_parser_flex_grow;
	FlexShrinkPropertyParser style_parser_flex_shrink;
	FlexWrapPropertyParser style_parser_flex_wrap;
	OrderPropertyParser style_parser_order;
	AlignContentPropertyParser style_parser_align_content;
	AlignItemsPropertyParser style_parser_align_items;
	AlignSelfPropertyParser style_parser_align_self;
	JustifyContentPropertyParser style_parser_justify_content;

	void FlexPropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		StyleSetValue flex_grow;
		StyleSetValue flex_shrink;
		StyleSetValue flex_basis;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "none"))
			{
				flex_grow = StyleSetValue::from_number(0.0f);
				flex_shrink = StyleSetValue::from_number(0.0f);
				flex_basis = StyleSetValue::from_keyword("main-size");

				setter->set_value("flex-grow", flex_grow);
				setter->set_value("flex-shrink", flex_shrink);
				setter->set_value("flex-basis", flex_basis);
			}
			else if (equals(token.value, "auto"))
			{
				flex_grow = StyleSetValue::from_number(1.0f);
				flex_shrink = StyleSetValue::from_number(1.0f);
				flex_basis = StyleSetValue::from_keyword("main-size");

				setter->set_value("flex-grow", flex_grow);
				setter->set_value("flex-shrink", flex_shrink);
				setter->set_value("flex-basis", flex_basis);
			}
			else if (equals(token.value, "inherit"))
			{
				flex_grow = StyleSetValue::from_keyword("inherit");
				flex_shrink = StyleSetValue::from_keyword("inherit");
				flex_basis = StyleSetValue::from_keyword("inherit");

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

			flex_grow = StyleSetValue::from_number(1.0f);
			flex_shrink = StyleSetValue::from_number(1.0f);
			flex_basis = StyleSetValue::from_length(0.0f);

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

	bool FlexPropertyParser::parse_grow_shrink(StyleSetValue &grow, StyleSetValue &shrink, size_t &parse_pos, const std::vector<StyleToken> &tokens)
	{
		size_t pos = parse_pos;
		StyleToken token = next_token(pos, tokens);

		if (token.type == StyleTokenType::number)
		{
			grow = StyleSetValue::from_number(StringHelp::text_to_float(token.value));
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
				shrink = StyleSetValue::from_number(StringHelp::text_to_float(token.value));

				parse_pos = pos;
			}
		}

		return true;
	}

	bool FlexPropertyParser::parse_basis(StyleSetValue &basis, size_t &parse_pos, const std::vector<StyleToken> &tokens)
	{
		size_t pos = parse_pos;
		StyleToken token = next_token(pos, tokens);

		if (token.type == StyleTokenType::ident)
		{
			if (equals(token.value, "main-size"))
				basis = StyleSetValue::from_keyword("main-size");
			else
				return false;
		}
		else if (is_length(token))
		{
			StyleSetValue length;
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
				basis = StyleSetValue::from_percentage(v);
			}
		}
		else
		{
			return false;
		}

		parse_pos = pos;
		return true;
	}

	void FlexBasisPropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		StyleSetValue flex_basis;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "main-size"))
				flex_basis = StyleSetValue::from_keyword("main-size");
			else if (equals(token.value, "inherit"))
				flex_basis = StyleSetValue::from_keyword("inherit");
			else
				return;
		}
		else if (is_length(token) && pos == tokens.size())
		{
			StyleSetValue length;
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
				flex_basis = StyleSetValue::from_percentage(v);
			}
			else
			{
				return;
			}
		}

		setter->set_value("flex-basis", flex_basis);
	}

	void FlexDirectionPropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		StyleSetValue flex_direction;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "row"))
				flex_direction = StyleSetValue::from_keyword("row");
			else if (equals(token.value, "row-reverse"))
				flex_direction = StyleSetValue::from_keyword("row-reverse");
			else if (equals(token.value, "column"))
				flex_direction = StyleSetValue::from_keyword("column");
			else if (equals(token.value, "column-reverse"))
				flex_direction = StyleSetValue::from_keyword("column-reverse");
			else if (equals(token.value, "inherit"))
				flex_direction = StyleSetValue::from_keyword("inherit");
			else
				return;
		}
		else
		{
			return;
		}

		setter->set_value("flex-direction", flex_direction);
	}

	void FlexFlowPropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);

		StyleSetValue direction = StyleSetValue::from_keyword("row");
		StyleSetValue wrap = StyleSetValue::from_keyword("nowrap");

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

	bool FlexFlowPropertyParser::parse_direction(StyleSetValue &direction, size_t &parse_pos, const std::vector<StyleToken> &tokens)
	{
		size_t pos = parse_pos;
		StyleToken token = next_token(pos, tokens);

		if (token.type == StyleTokenType::ident)
		{
			if (equals(token.value, "row"))
				direction = StyleSetValue::from_keyword("row");
			else if (equals(token.value, "row-reverse"))
				direction = StyleSetValue::from_keyword("row-reverse");
			else if (equals(token.value, "column"))
				direction = StyleSetValue::from_keyword("column");
			else if (equals(token.value, "column-reverse"))
				direction = StyleSetValue::from_keyword("column-reverse");
			else if (equals(token.value, "inherit"))
				direction = StyleSetValue::from_keyword("inherit");
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

	bool FlexFlowPropertyParser::parse_wrap(StyleSetValue &wrap, size_t &parse_pos, const std::vector<StyleToken> &tokens)
	{
		size_t pos = parse_pos;
		StyleToken token = next_token(pos, tokens);

		if (token.type == StyleTokenType::ident)
		{
			if (equals(token.value, "nowrap"))
				wrap = StyleSetValue::from_keyword("nowrap");
			else if (equals(token.value, "wrap"))
				wrap = StyleSetValue::from_keyword("wrap");
			else if (equals(token.value, "wrap-reverse"))
				wrap = StyleSetValue::from_keyword("wrap-reverse");
			else if (equals(token.value, "inherit"))
				wrap = StyleSetValue::from_keyword("inherit");
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

	void FlexGrowPropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		StyleSetValue flex_grow;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "inherit"))
				flex_grow = StyleSetValue::from_keyword("inherit");
			else
				return;
		}
		else if (token.type == StyleTokenType::number && pos == tokens.size())
		{
			flex_grow = StyleSetValue::from_number(StringHelp::text_to_float(token.value));
		}
		else
		{
			return;
		}

		setter->set_value("flex-grow", flex_grow);
	}

	void FlexShrinkPropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		StyleSetValue flex_shrink;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "inherit"))
				flex_shrink = StyleSetValue::from_keyword("inherit");
			else
				return;
		}
		else if (token.type == StyleTokenType::number && pos == tokens.size())
		{
			flex_shrink = StyleSetValue::from_number(StringHelp::text_to_float(token.value));
		}
		else
		{
			return;
		}
	}

	void FlexWrapPropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		StyleSetValue flex_wrap;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "nowrap"))
				flex_wrap = StyleSetValue::from_keyword("nowrap");
			else if (equals(token.value, "wrap"))
				flex_wrap = StyleSetValue::from_keyword("wrap");
			else if (equals(token.value, "wrap-reverse"))
				flex_wrap = StyleSetValue::from_keyword("wrap-reverse");
			else if (equals(token.value, "inherit"))
				flex_wrap = StyleSetValue::from_keyword("inherit");
			else
				return;
		}
		else
		{
			return;
		}

		setter->set_value("flex-wrap", flex_wrap);
	}

	void OrderPropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		StyleSetValue order;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "inherit"))
				order = StyleSetValue::from_keyword("inherit");
			else
				return;
		}
		else if (token.type == StyleTokenType::number && pos == tokens.size())
		{
			int value = 0;
			if (parse_integer(token.value, value))
			{
				order = StyleSetValue::from_number((float)value);
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

	void AlignContentPropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			StyleSetValue align_content;
			if (equals(token.value, "flex-start"))
			{
				align_content = StyleSetValue::from_keyword("flex-start");
				setter->set_value("align-content", align_content);
			}
			else if (equals(token.value, "flex-end"))
			{
				align_content = StyleSetValue::from_keyword("flex-end");
				setter->set_value("align-content", align_content);
			}
			else if (equals(token.value, "center"))
			{
				align_content = StyleSetValue::from_keyword("center");
				setter->set_value("align-content", align_content);
			}
			else if (equals(token.value, "space-between"))
			{
				align_content = StyleSetValue::from_keyword("space-between");
				setter->set_value("align-content", align_content);
			}
			else if (equals(token.value, "space-around"))
			{
				align_content = StyleSetValue::from_keyword("space-around");
				setter->set_value("align-content", align_content);
			}
			else if (equals(token.value, "stretch"))
			{
				align_content = StyleSetValue::from_keyword("stretch");
				setter->set_value("align-content", align_content);
			}
			else if (equals(token.value, "inherit"))
			{
				align_content = StyleSetValue::from_keyword("inherit");
				setter->set_value("align-content", align_content);
			}
		}
	}

	void AlignItemsPropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			StyleSetValue align_items;
			if (equals(token.value, "flex-start"))
			{
				align_items = StyleSetValue::from_keyword("flex-start");
				setter->set_value("align-items", align_items);
			}
			else if (equals(token.value, "flex-end"))
			{
				align_items = StyleSetValue::from_keyword("flex-end");
				setter->set_value("align-items", align_items);
			}
			else if (equals(token.value, "center"))
			{
				align_items = StyleSetValue::from_keyword("center");
				setter->set_value("align-items", align_items);
			}
			else if (equals(token.value, "baseline"))
			{
				align_items = StyleSetValue::from_keyword("baseline");
				setter->set_value("align-items", align_items);
			}
			else if (equals(token.value, "stretch"))
			{
				align_items = StyleSetValue::from_keyword("stretch");
				setter->set_value("align-items", align_items);
			}
			else if (equals(token.value, "inherit"))
			{
				align_items = StyleSetValue::from_keyword("inherit");
				setter->set_value("align-items", align_items);
			}
		}
	}

	void AlignSelfPropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			StyleSetValue align_self;

			if (equals(token.value, "auto"))
			{
				align_self = StyleSetValue::from_keyword("auto");
				setter->set_value("align-self", align_self);
			}
			else if (equals(token.value, "flex-start"))
			{
				align_self = StyleSetValue::from_keyword("flex-start");
				setter->set_value("align-self", align_self);
			}
			else if (equals(token.value, "flex-end"))
			{
				align_self = StyleSetValue::from_keyword("flex-end");
				setter->set_value("align-self", align_self);
			}
			else if (equals(token.value, "center"))
			{
				align_self = StyleSetValue::from_keyword("center");
				setter->set_value("align-self", align_self);
			}
			else if (equals(token.value, "baseline"))
			{
				align_self = StyleSetValue::from_keyword("baseline");
				setter->set_value("align-self", align_self);
			}
			else if (equals(token.value, "stretch"))
			{
				align_self = StyleSetValue::from_keyword("stretch");
				setter->set_value("align-self", align_self);
			}
			else if (equals(token.value, "inherit"))
			{
				align_self = StyleSetValue::from_keyword("inherit");
				setter->set_value("align-self", align_self);
			}
		}
	}

	void JustifyContentPropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		StyleSetValue justify_content;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "flex-start"))
				justify_content = StyleSetValue::from_keyword("flex-start");
			else if (equals(token.value, "flex-end"))
				justify_content = StyleSetValue::from_keyword("flex-end");
			else if (equals(token.value, "center"))
				justify_content = StyleSetValue::from_keyword("center");
			else if (equals(token.value, "space-between"))
				justify_content = StyleSetValue::from_keyword("space-between");
			else if (equals(token.value, "space-around"))
				justify_content = StyleSetValue::from_keyword("space-around");
			else if (equals(token.value, "inherit"))
				justify_content = StyleSetValue::from_keyword("inherit");
			else
				return;
		}
		else
		{
			return;
		}

		setter->set_value("justify-content", justify_content);
	}
}
