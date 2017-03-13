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
#include "border.h"

namespace clan
{
	void force_link_style_parser_border()
	{
	}

	StylePropertyDefault style_default_border_left_color("border-left-color", StyleGetValue::from_keyword("from-color"), false);
	StylePropertyDefault style_default_border_top_color("border-top-color", StyleGetValue::from_keyword("from-color"), false);
	StylePropertyDefault style_default_border_right_color("border-right-color", StyleGetValue::from_keyword("from-color"), false);
	StylePropertyDefault style_default_border_bottom_color("border-bottom-color", StyleGetValue::from_keyword("from-color"), false);
	StylePropertyDefault style_default_border_left_style("border-left-style", StyleGetValue::from_keyword("none"), false);
	StylePropertyDefault style_default_border_top_style("border-top-style", StyleGetValue::from_keyword("none"), false);
	StylePropertyDefault style_default_border_right_style("border-right-style", StyleGetValue::from_keyword("none"), false);
	StylePropertyDefault style_default_border_bottom_style("border-bottom-style", StyleGetValue::from_keyword("none"), false);
	StylePropertyDefault style_default_border_left_width("border-left-width", StyleGetValue::from_keyword("medium"), false);
	StylePropertyDefault style_default_border_top_width("border-top-width", StyleGetValue::from_keyword("medium"), false);
	StylePropertyDefault style_default_border_right_width("border-right-width", StyleGetValue::from_keyword("medium"), false);
	StylePropertyDefault style_default_border_bottom_width("border-bottom-width", StyleGetValue::from_keyword("medium"), false);

	BorderPropertyParser style_parser_border;
	BorderColorPropertyParser style_parser_border_color;
	BorderStylePropertyParser style_parser_border_style;
	BorderWidthPropertyParser style_parser_border_width;
	BorderLTRBPropertyParser style_parser_border_ltrb;
	BorderLTRBColorPropertyParser style_parser_border_ltrb_color;
	BorderLTRBStylePropertyParser style_parser_border_ltrb_style;
	BorderLTRBWidthPropertyParser style_parser_border_ltrb_width;
	BorderRadiusPropertyParser style_parser_border_radius;
	BorderRadiusCornerPropertyParser style_parser_border_radius_corner;

	void BorderPropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		StyleSetValue border_width;
		StyleSetValue border_style;
		StyleSetValue border_color;
		bool width_specified = false;
		bool style_specified = false;
		bool color_specified = false;

		size_t pos = 0;
		while (pos != tokens.size())
		{
			Colorf color;
			if (!color_specified && parse_color(tokens, pos, color))
			{
				border_color = StyleSetValue::from_color(color);
				color_specified = true;
			}
			else
			{
				StyleToken token = next_token(pos, tokens);
				if (token.type == StyleTokenType::ident)
				{
					if (equals(token.value, "inherit") && tokens.size() == 1)
					{
						border_width = StyleSetValue::from_keyword("inherit");
						border_style = StyleSetValue::from_keyword("inherit");
						border_color = StyleSetValue::from_keyword("inherit");

						setter->set_value("border-left-width", border_width);
						setter->set_value("border-top-width", border_width);
						setter->set_value("border-right-width", border_width);
						setter->set_value("border-bottom-width", border_width);

						setter->set_value("border-left-style", border_style);
						setter->set_value("border-top-style", border_style);
						setter->set_value("border-right-style", border_style);
						setter->set_value("border-bottom-style", border_style);

						setter->set_value("border-left-color", border_color);
						setter->set_value("border-top-color", border_color);
						setter->set_value("border-right-color", border_color);
						setter->set_value("border-bottom-color", border_color);

						return;
					}
					else if (!width_specified && equals(token.value, "thin"))
					{
						border_width = StyleSetValue::from_keyword("thin");
						width_specified = true;
					}
					else if (!width_specified && equals(token.value, "medium"))
					{
						border_width = StyleSetValue::from_keyword("medium");
						width_specified = true;
					}
					else if (!width_specified && equals(token.value, "thick"))
					{
						border_width = StyleSetValue::from_keyword("thick");
						width_specified = true;
					}
					else if (!style_specified && equals(token.value, "none"))
					{
						border_style = StyleSetValue::from_keyword("none");
						style_specified = true;
					}
					else if (!style_specified && equals(token.value, "hidden"))
					{
						border_style = StyleSetValue::from_keyword("hidden");
						style_specified = true;
					}
					else if (!style_specified && equals(token.value, "dotted"))
					{
						border_style = StyleSetValue::from_keyword("dotted");
						style_specified = true;
					}
					else if (!style_specified && equals(token.value, "dashed"))
					{
						border_style = StyleSetValue::from_keyword("dashed");
						style_specified = true;
					}
					else if (!style_specified && equals(token.value, "solid"))
					{
						border_style = StyleSetValue::from_keyword("solid");
						style_specified = true;
					}
					else if (!style_specified && equals(token.value, "double"))
					{
						border_style = StyleSetValue::from_keyword("double");
						style_specified = true;
					}
					else if (!style_specified && equals(token.value, "groove"))
					{
						border_style = StyleSetValue::from_keyword("groove");
						style_specified = true;
					}
					else if (!style_specified && equals(token.value, "ridge"))
					{
						border_style = StyleSetValue::from_keyword("ridge");
						style_specified = true;
					}
					else if (!style_specified && equals(token.value, "inset"))
					{
						border_style = StyleSetValue::from_keyword("inset");
						style_specified = true;
					}
					else if (!style_specified && equals(token.value, "outset"))
					{
						border_style = StyleSetValue::from_keyword("outset");
						style_specified = true;
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
						border_width = length;
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

		setter->set_value("border-left-width", border_width);
		setter->set_value("border-top-width", border_width);
		setter->set_value("border-right-width", border_width);
		setter->set_value("border-bottom-width", border_width);

		setter->set_value("border-left-style", border_style);
		setter->set_value("border-top-style", border_style);
		setter->set_value("border-right-style", border_style);
		setter->set_value("border-bottom-style", border_style);

		setter->set_value("border-left-color", border_color);
		setter->set_value("border-top-color", border_color);
		setter->set_value("border-right-color", border_color);
		setter->set_value("border-bottom-color", border_color);
	}

	void BorderColorPropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		StyleSetValue border_colors[4];
		int count;
		size_t pos = 0;
		for (count = 0; count < 4; count++)
		{
			Colorf color;
			if (parse_color(tokens, pos, color))
			{
				border_colors[count] = StyleSetValue::from_color(color);
			}
			else
			{
				StyleToken token = next_token(pos, tokens);
				if (token.type == StyleTokenType::ident)
				{
					if (equals(token.value, "inherit"))
					{
						if (count == 0 && pos == tokens.size())
						{
							border_colors[0] = StyleSetValue::from_keyword("inherit");

							setter->set_value("border-left-color", border_colors[0]);
							setter->set_value("border-right-color", border_colors[0]);
							setter->set_value("border-top-color", border_colors[0]);
							setter->set_value("border-bottom-color", border_colors[0]);
						}
						else
						{
							debug_parse_error(name, tokens);
						}
						return;
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
		}

		if (pos == tokens.size())
		{
			switch (count)
			{
			case 1:
				setter->set_value("border-left-color", border_colors[0]);
				setter->set_value("border-right-color", border_colors[0]);
				setter->set_value("border-top-color", border_colors[0]);
				setter->set_value("border-bottom-color", border_colors[0]);
				break;
			case 2:
				setter->set_value("border-top-color", border_colors[0]);
				setter->set_value("border-bottom-color", border_colors[0]);
				setter->set_value("border-left-color", border_colors[1]);
				setter->set_value("border-right-color", border_colors[1]);
				break;
			case 3:
				setter->set_value("border-top-color", border_colors[0]);
				setter->set_value("border-left-color", border_colors[1]);
				setter->set_value("border-right-color", border_colors[1]);
				setter->set_value("border-bottom-color", border_colors[2]);
				break;
			case 4:
				setter->set_value("border-top-color", border_colors[0]);
				setter->set_value("border-right-color", border_colors[1]);
				setter->set_value("border-left-color", border_colors[2]);
				setter->set_value("border-bottom-color", border_colors[3]);
				break;
			default:
				break;
			}
		}
	}

	void BorderStylePropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		StyleSetValue border_styles[4];
		int count;
		size_t pos = 0;
		for (count = 0; count < 4; count++)
		{
			StyleToken token = next_token(pos, tokens);
			if (token.type == StyleTokenType::ident)
			{
				if (equals(token.value, "none"))
				{
					border_styles[count] = StyleSetValue::from_keyword("none");
				}
				else if (equals(token.value, "hidden"))
				{
					border_styles[count] = StyleSetValue::from_keyword("hidden");
				}
				else if (equals(token.value, "dotted"))
				{
					border_styles[count] = StyleSetValue::from_keyword("dotted");
				}
				else if (equals(token.value, "dashed"))
				{
					border_styles[count] = StyleSetValue::from_keyword("dashed");
				}
				else if (equals(token.value, "solid"))
				{
					border_styles[count] = StyleSetValue::from_keyword("solid");
				}
				else if (equals(token.value, "double"))
				{
					border_styles[count] = StyleSetValue::from_keyword("double");
				}
				else if (equals(token.value, "groove"))
				{
					border_styles[count] = StyleSetValue::from_keyword("groove");
				}
				else if (equals(token.value, "ridge"))
				{
					border_styles[count] = StyleSetValue::from_keyword("ridge");
				}
				else if (equals(token.value, "inset"))
				{
					border_styles[count] = StyleSetValue::from_keyword("inset");
				}
				else if (equals(token.value, "outset"))
				{
					border_styles[count] = StyleSetValue::from_keyword("outset");
				}
				else if (equals(token.value, "inherit"))
				{
					if (count == 0 && pos == tokens.size())
					{
						border_styles[0] = StyleSetValue::from_keyword("inherit");
						setter->set_value("border-left-style", border_styles[0]);
						setter->set_value("border-top-style", border_styles[0]);
						setter->set_value("border-right-style", border_styles[0]);
						setter->set_value("border-bottom-style", border_styles[0]);
					}
					else
					{
						debug_parse_error(name, tokens);
					}
					return;
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
				setter->set_value("border-left-style", border_styles[0]);
				setter->set_value("border-top-style", border_styles[0]);
				setter->set_value("border-right-style", border_styles[0]);
				setter->set_value("border-bottom-style", border_styles[0]);
				break;
			case 2:
				setter->set_value("border-top-style", border_styles[0]);
				setter->set_value("border-bottom-style", border_styles[0]);
				setter->set_value("border-left-style", border_styles[1]);
				setter->set_value("border-right-style", border_styles[1]);
				break;
			case 3:
				setter->set_value("border-top-style", border_styles[0]);
				setter->set_value("border-left-style", border_styles[1]);
				setter->set_value("border-right-style", border_styles[1]);
				setter->set_value("border-bottom-style", border_styles[2]);
				break;
			case 4:
				setter->set_value("border-top-style", border_styles[0]);
				setter->set_value("border-right-style", border_styles[1]);
				setter->set_value("border-bottom-style", border_styles[2]);
				setter->set_value("border-left-style", border_styles[3]);
				break;
			default:
				break;
			}
		}
	}

	void BorderWidthPropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		StyleSetValue border_widths[4];
		int count;
		size_t pos = 0;
		for (count = 0; count < 4; count++)
		{
			StyleToken token = next_token(pos, tokens);
			if (token.type == StyleTokenType::ident)
			{
				if (equals(token.value, "thin"))
				{
					border_widths[count] = StyleSetValue::from_keyword("thin");
				}
				else if (equals(token.value, "medium"))
				{
					border_widths[count] = StyleSetValue::from_keyword("medium");
				}
				else if (equals(token.value, "thick"))
				{
					border_widths[count] = StyleSetValue::from_keyword("thick");
				}
				else if (equals(token.value, "inherit") && count == 0 && pos == tokens.size())
				{
					border_widths[0] = StyleSetValue::from_keyword("inherit");

					setter->set_value("border-left-width", border_widths[0]);
					setter->set_value("border-top-width", border_widths[0]);
					setter->set_value("border-right-width", border_widths[0]);
					setter->set_value("border-bottom-width", border_widths[0]);
					return;
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
				if (parse_length(token, length))
				{
					border_widths[count] = length;
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
				setter->set_value("border-left-width", border_widths[0]);
				setter->set_value("border-top-width", border_widths[0]);
				setter->set_value("border-right-width", border_widths[0]);
				setter->set_value("border-bottom-width", border_widths[0]);
				break;
			case 2:
				setter->set_value("border-top-width", border_widths[0]);
				setter->set_value("border-bottom-width", border_widths[0]);
				setter->set_value("border-left-width", border_widths[1]);
				setter->set_value("border-right-width", border_widths[1]);
				break;
			case 3:
				setter->set_value("border-top-width", border_widths[0]);
				setter->set_value("border-left-width", border_widths[1]);
				setter->set_value("border-right-width", border_widths[1]);
				setter->set_value("border-bottom-width", border_widths[2]);
				break;
			case 4:
				setter->set_value("border-top-width", border_widths[0]);
				setter->set_value("border-right-width", border_widths[1]);
				setter->set_value("border-bottom-width", border_widths[2]);
				setter->set_value("border-left-width", border_widths[3]);
				break;
			default:
				break;
			}
		}
	}

	void BorderLTRBPropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		StyleSetValue border_width;
		StyleSetValue border_style;
		StyleSetValue border_color;

		bool width_specified = false;
		bool style_specified = false;
		bool color_specified = false;

		size_t pos = 0;
		while (pos != tokens.size())
		{
			Colorf color;
			if (!color_specified && parse_color(tokens, pos, color))
			{
				border_color = StyleSetValue::from_color(color);
				color_specified = true;
			}
			else
			{
				StyleToken token = next_token(pos, tokens);
				if (token.type == StyleTokenType::ident)
				{
					if (equals(token.value, "inherit") && tokens.size() == 1)
					{
						border_width = StyleSetValue::from_keyword("inherit");
						border_style = StyleSetValue::from_keyword("inherit");
						border_color = StyleSetValue::from_keyword("inherit");
						setter->set_value(name + "-width", border_width);
						setter->set_value(name + "-style", border_style);
						setter->set_value(name + "-color", border_color);
						return;
					}
					else if (!width_specified && equals(token.value, "thin"))
					{
						border_width = StyleSetValue::from_keyword("thin");
						width_specified = true;
					}
					else if (!width_specified && equals(token.value, "medium"))
					{
						border_width = StyleSetValue::from_keyword("medium");
						width_specified = true;
					}
					else if (!width_specified && equals(token.value, "thick"))
					{
						border_width = StyleSetValue::from_keyword("thick");
						width_specified = true;
					}
					else if (!style_specified && equals(token.value, "none"))
					{
						border_style = StyleSetValue::from_keyword("none");
						style_specified = true;
					}
					else if (!style_specified && equals(token.value, "hidden"))
					{
						border_style = StyleSetValue::from_keyword("hidden");
						style_specified = true;
					}
					else if (!style_specified && equals(token.value, "dotted"))
					{
						border_style = StyleSetValue::from_keyword("dotted");
						style_specified = true;
					}
					else if (!style_specified && equals(token.value, "dashed"))
					{
						border_style = StyleSetValue::from_keyword("dashed");
						style_specified = true;
					}
					else if (!style_specified && equals(token.value, "solid"))
					{
						border_style = StyleSetValue::from_keyword("solid");
						style_specified = true;
					}
					else if (!style_specified && equals(token.value, "double"))
					{
						border_style = StyleSetValue::from_keyword("double");
						style_specified = true;
					}
					else if (!style_specified && equals(token.value, "groove"))
					{
						border_style = StyleSetValue::from_keyword("groove");
						style_specified = true;
					}
					else if (!style_specified && equals(token.value, "ridge"))
					{
						border_style = StyleSetValue::from_keyword("ridge");
						style_specified = true;
					}
					else if (!style_specified && equals(token.value, "inset"))
					{
						border_style = StyleSetValue::from_keyword("inset");
						style_specified = true;
					}
					else if (!style_specified && equals(token.value, "outset"))
					{
						border_style = StyleSetValue::from_keyword("outset");
						style_specified = true;
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
						border_width = length;
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

		setter->set_value(name + "-width", border_width);
		setter->set_value(name + "-style", border_style);
		setter->set_value(name + "-color", border_color);
	}

	void BorderLTRBColorPropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		StyleSetValue border_color;

		size_t pos = 0;
		Colorf color;
		if (parse_color(tokens, pos, color) && pos == tokens.size())
		{
			border_color = StyleSetValue::from_color(color);
			setter->set_value(name, border_color);
		}
		else
		{
			StyleToken token = next_token(pos, tokens);
			if (token.type == StyleTokenType::ident && pos == tokens.size())
			{
				if (equals(token.value, "inherit"))
				{
					border_color = StyleSetValue::from_keyword("inherit");
					setter->set_value(name, border_color);
				}
			}
		}
	}

	void BorderLTRBStylePropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		StyleSetValue style;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "none"))
				style = StyleSetValue::from_keyword("none");
			else if (equals(token.value, "hidden"))
				style = StyleSetValue::from_keyword("hidden");
			else if (equals(token.value, "dotted"))
				style = StyleSetValue::from_keyword("dotted");
			else if (equals(token.value, "dashed"))
				style = StyleSetValue::from_keyword("dashed");
			else if (equals(token.value, "solid"))
				style = StyleSetValue::from_keyword("solid");
			else if (equals(token.value, "double"))
				style = StyleSetValue::from_keyword("double");
			else if (equals(token.value, "groove"))
				style = StyleSetValue::from_keyword("groove");
			else if (equals(token.value, "ridge"))
				style = StyleSetValue::from_keyword("ridge");
			else if (equals(token.value, "inset"))
				style = StyleSetValue::from_keyword("inset");
			else if (equals(token.value, "outset"))
				style = StyleSetValue::from_keyword("outset");
			else if (equals(token.value, "inherit"))
				style = StyleSetValue::from_keyword("inherit");
			else
				return;

			setter->set_value(name, style);
		}
	}

	void BorderLTRBWidthPropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		StyleSetValue width;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "thin"))
				width = StyleSetValue::from_keyword("thin");
			else if (equals(token.value, "medium"))
				width = StyleSetValue::from_keyword("medium");
			else if (equals(token.value, "thick"))
				width = StyleSetValue::from_keyword("thick");
			else if (equals(token.value, "inherit"))
				width = StyleSetValue::from_keyword("inherit");
			else
				return;

			setter->set_value(name, width);
		}
		else if (is_length(token) && pos == tokens.size())
		{
			StyleSetValue length;
			if (parse_length(token, length))
			{
				width = length;

				setter->set_value(name, width);
			}
		}
	}

	void BorderRadiusPropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		StyleSetValue radius_x[4];
		StyleSetValue radius_y[4];

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && equals(token.value, "inherit") && pos == tokens.size())
		{
			radius_x[0] = StyleSetValue::from_keyword("inherit");
			radius_y[0] = StyleSetValue::from_keyword("inherit");
			setter->set_value("border-top-left-radius-x", radius_x[0]);
			setter->set_value("border-top-left-radius-y", radius_y[0]);
			setter->set_value("border-top-right-radius-x", radius_x[0]);
			setter->set_value("border-top-right-radius-y", radius_y[0]);
			setter->set_value("border-bottom-left-radius-x", radius_x[0]);
			setter->set_value("border-bottom-left-radius-y", radius_y[0]);
			setter->set_value("border-bottom-right-radius-x", radius_x[0]);
			setter->set_value("border-bottom-right-radius-y", radius_y[0]);
			return;
		}

		int num_x_values = 0;
		int num_y_values = 0;
		for (int i = 0; i < 4; i++)
		{
			if (is_length(token))
			{
				if (!parse_length(token, radius_x[i]))
				{
					debug_parse_error(name, tokens);
					return;
				}
				radius_y[i] = radius_x[i];
			}
			else if (token.type == StyleTokenType::percentage)
			{
				radius_x[i] = StyleSetValue::from_percentage(StringHelp::text_to_float(token.value));
				radius_y[i] = radius_x[i];
			}
			else
			{
				debug_parse_error(name, tokens);
				return;
			}

			num_x_values = i + 1;

			token = next_token(pos, tokens);
			if (token.type == StyleTokenType::null)
				break;
			else if (i != 0 && token.type == StyleTokenType::delim && token.value == "/")
				break;
		}

		if (token.type == StyleTokenType::delim && token.value == "/")
		{
			token = next_token(pos, tokens);
			for (int i = 0; i < 4; i++)
			{
				if (is_length(token) && parse_length(token, radius_y[i]))
				{
				}
				else if (token.type == StyleTokenType::percentage)
				{
					radius_y[i] = StyleSetValue::from_percentage(StringHelp::text_to_float(token.value));
				}
				else
				{
					debug_parse_error(name, tokens);
					return;
				}

				num_y_values = i + 1;

				token = next_token(pos, tokens);
				if (token.type == StyleTokenType::null)
					break;
			}
		}
		else if (token.type != StyleTokenType::null)
		{
			debug_parse_error(name, tokens);
			return;
		}

		if (num_y_values > 0)
		{
			if (num_x_values < num_y_values)
			{
				for (int i = num_x_values; i < num_y_values; i++)
				{
					radius_x[i] = radius_y[i];
				}
				num_x_values = num_y_values;
			}
			else if (num_y_values < num_x_values)
			{
				for (int i = num_y_values; i < num_x_values; i++)
				{
					radius_y[i] = radius_x[i];
				}
				num_y_values = num_x_values;
			}
		}

		if (num_x_values == 3)
		{
			radius_x[3] = radius_x[1];
			radius_y[3] = radius_y[1];
		}
		else if (num_x_values == 2)
		{
			radius_x[3] = radius_x[1];
			radius_y[3] = radius_y[1];
			radius_x[2] = radius_x[0];
			radius_y[2] = radius_y[0];
		}
		else if (num_x_values == 1)
		{
			radius_x[3] = radius_x[0];
			radius_y[3] = radius_y[0];
			radius_x[2] = radius_x[0];
			radius_y[2] = radius_y[0];
			radius_x[1] = radius_x[0];
			radius_y[1] = radius_y[0];
		}

		setter->set_value("border-top-left-radius-x", radius_x[0]);
		setter->set_value("border-top-left-radius-y", radius_y[0]);
		setter->set_value("border-top-right-radius-x", radius_x[1]);
		setter->set_value("border-top-right-radius-y", radius_y[1]);
		setter->set_value("border-bottom-right-radius-x", radius_x[2]);
		setter->set_value("border-bottom-right-radius-y", radius_y[2]);
		setter->set_value("border-bottom-left-radius-x", radius_x[3]);
		setter->set_value("border-bottom-left-radius-y", radius_y[3]);
	}

	void BorderRadiusCornerPropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		StyleSetValue radius_x;
		StyleSetValue radius_y;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && equals(token.value, "inherit") && pos == tokens.size())
		{
			radius_x = StyleSetValue::from_keyword("inherit");
			radius_y = StyleSetValue::from_keyword("inherit");
			setter->set_value(name + "-x", radius_x);
			setter->set_value(name + "-y", radius_y);
			return;
		}
		else if (is_length(token))
		{
			if (!parse_length(token, radius_x))
			{
				debug_parse_error(name, tokens);
				return;
			}
			radius_y = radius_x;
		}
		else if (token.type == StyleTokenType::percentage)
		{
			radius_x = StyleSetValue::from_percentage(StringHelp::text_to_float(token.value));
			radius_y = radius_x;
		}
		else
		{
			debug_parse_error(name, tokens);
			return;
		}

		if (pos != tokens.size())
		{
			token = next_token(pos, tokens);
			if (is_length(token) && pos == tokens.size() && parse_length(token, radius_y))
			{
				setter->set_value(name + "-x", radius_x);
				setter->set_value(name + "-y", radius_y);
			}
			else if (token.type == StyleTokenType::percentage)
			{
				radius_y = StyleSetValue::from_percentage(StringHelp::text_to_float(token.value));

				setter->set_value(name + "-x", radius_x);
				setter->set_value(name + "-y", radius_y);
			}
			else
			{
				debug_parse_error(name, tokens);
				return;
			}
		}
		else
		{
			setter->set_value(name + "-x", radius_x);
			setter->set_value(name + "-y", radius_y);
		}
	}
}
