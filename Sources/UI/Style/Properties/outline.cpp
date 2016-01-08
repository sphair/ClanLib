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
#include "outline.h"

namespace clan
{
	void force_link_style_parser_outline()
	{
	}

	StylePropertyDefault style_default_outline_color("outline-color", StyleGetValue::from_keyword("invert"), false);
	StylePropertyDefault style_default_outline_style("outline-style", StyleGetValue::from_keyword("none"), false);
	StylePropertyDefault style_default_outline_width("outline-width", StyleGetValue::from_keyword("medium"), false);

	OutlinePropertyParser style_parser_outline;
	OutlineColorPropertyParser style_parser_outline_color;
	OutlineStylePropertyParser style_parser_outline_style;
	OutlineWidthPropertyParser style_parser_outline_width;

	void OutlinePropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		StyleSetValue outline_width;
		StyleSetValue outline_style;
		StyleSetValue outline_color;
		bool width_specified = false;
		bool style_specified = false;
		bool color_specified = false;

		size_t pos = 0;
		while (pos != tokens.size())
		{
			Colorf color;
			if (!color_specified && parse_color(tokens, pos, color))
			{
				outline_color = StyleSetValue::from_color(color);
				color_specified = true;
			}
			else
			{
				StyleToken token = next_token(pos, tokens);
				if (token.type == StyleTokenType::ident)
				{
					if (equals(token.value, "inherit") && tokens.size() == 1)
					{
						outline_width = StyleSetValue::from_keyword("inherit");
						outline_style = StyleSetValue::from_keyword("inherit");
						outline_color = StyleSetValue::from_keyword("inherit");
						setter->set_value("outline-width", outline_width);
						setter->set_value("outline-style", outline_style);
						setter->set_value("outline-color", outline_color);
						return;
					}
					else if (!width_specified && equals(token.value, "thin"))
					{
						outline_width = StyleSetValue::from_keyword("thin");
						width_specified = true;
					}
					else if (!width_specified && equals(token.value, "medium"))
					{
						outline_width = StyleSetValue::from_keyword("medium");
						width_specified = true;
					}
					else if (!width_specified && equals(token.value, "thick"))
					{
						outline_width = StyleSetValue::from_keyword("thick");
						width_specified = true;
					}
					else if (!style_specified && equals(token.value, "none"))
					{
						outline_style = StyleSetValue::from_keyword("none");
						style_specified = true;
					}
					else if (!style_specified && equals(token.value, "hidden"))
					{
						outline_style = StyleSetValue::from_keyword("hidden");
						style_specified = true;
					}
					else if (!style_specified && equals(token.value, "dotted"))
					{
						outline_style = StyleSetValue::from_keyword("dotted");
						style_specified = true;
					}
					else if (!style_specified && equals(token.value, "dashed"))
					{
						outline_style = StyleSetValue::from_keyword("dashed");
						style_specified = true;
					}
					else if (!style_specified && equals(token.value, "solid"))
					{
						outline_style = StyleSetValue::from_keyword("solid");
						style_specified = true;
					}
					else if (!style_specified && equals(token.value, "double"))
					{
						outline_style = StyleSetValue::from_keyword("double");
						style_specified = true;
					}
					else if (!style_specified && equals(token.value, "groove"))
					{
						outline_style = StyleSetValue::from_keyword("groove");
						style_specified = true;
					}
					else if (!style_specified && equals(token.value, "ridge"))
					{
						outline_style = StyleSetValue::from_keyword("ridge");
						style_specified = true;
					}
					else if (!style_specified && equals(token.value, "inset"))
					{
						outline_style = StyleSetValue::from_keyword("inset");
						style_specified = true;
					}
					else if (!style_specified && equals(token.value, "outset"))
					{
						outline_style = StyleSetValue::from_keyword("outset");
						style_specified = true;
					}
					else if (!color_specified && equals(token.value, "invert"))
					{
						outline_color = StyleSetValue::from_keyword("invert");
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
					StyleSetValue length;
					if (!width_specified && parse_length(token, length))
					{
						outline_width = length;
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

		setter->set_value("outline-width", outline_width);
		setter->set_value("outline-style", outline_style);
		setter->set_value("outline-color", outline_color);
	}

	void OutlineColorPropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		StyleSetValue outline_color;

		size_t pos = 0;
		Colorf color;
		if (parse_color(tokens, pos, color) && pos == tokens.size())
		{
			outline_color = StyleSetValue::from_color(color);
		}
		else
		{
			StyleToken token = next_token(pos, tokens);
			if (token.type == StyleTokenType::ident && pos == tokens.size())
			{
				if (equals(token.value, "invert"))
				{
					outline_color = StyleSetValue::from_keyword("invert");
				}
				else if (equals(token.value, "inherit"))
				{
					outline_color = StyleSetValue::from_keyword("inherit");
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
		}

		setter->set_value("outline-color", outline_color);
	}

	void OutlineStylePropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		StyleSetValue outline_style;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "none"))
				outline_style = StyleSetValue::from_keyword("none");
			else if (equals(token.value, "hidden"))
				outline_style = StyleSetValue::from_keyword("hidden");
			else if (equals(token.value, "dotted"))
				outline_style = StyleSetValue::from_keyword("dotted");
			else if (equals(token.value, "dashed"))
				outline_style = StyleSetValue::from_keyword("dashed");
			else if (equals(token.value, "solid"))
				outline_style = StyleSetValue::from_keyword("solid");
			else if (equals(token.value, "double"))
				outline_style = StyleSetValue::from_keyword("double");
			else if (equals(token.value, "groove"))
				outline_style = StyleSetValue::from_keyword("groove");
			else if (equals(token.value, "ridge"))
				outline_style = StyleSetValue::from_keyword("ridge");
			else if (equals(token.value, "inset"))
				outline_style = StyleSetValue::from_keyword("inset");
			else if (equals(token.value, "outset"))
				outline_style = StyleSetValue::from_keyword("outset");
			else if (equals(token.value, "inherit"))
				outline_style = StyleSetValue::from_keyword("inherit");
			else
				return;
		}
		else
		{
			return;
		}

		setter->set_value("outline-style", outline_style);
	}

	void OutlineWidthPropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		StyleSetValue outline_width;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "thin"))
				outline_width = StyleSetValue::from_keyword("thin");
			else if (equals(token.value, "medium"))
				outline_width = StyleSetValue::from_keyword("medium");
			else if (equals(token.value, "thick"))
				outline_width = StyleSetValue::from_keyword("thick");
			else if (equals(token.value, "inherit"))
				outline_width = StyleSetValue::from_keyword("inherit");
			else
				return;
		}
		else if (is_length(token) && pos == tokens.size())
		{
			StyleSetValue length;
			if (parse_length(token, length))
			{
				outline_width = length;
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

		setter->set_value("outline-width", outline_width);
	}
}
