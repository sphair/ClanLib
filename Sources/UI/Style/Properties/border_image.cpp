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
#include "border_image.h"

namespace clan
{
	void force_link_style_parser_border_image()
	{
	}

	StylePropertyDefault style_default_border_image_source("border-image-source", StyleGetValue::from_keyword("none"));
	StylePropertyDefault style_default_border_image_slice_top("border-image-slice-top", StyleGetValue::from_percentage(100.0f));
	StylePropertyDefault style_default_border_image_slice_right("border-image-slice-right", StyleGetValue::from_percentage(100.0f));
	StylePropertyDefault style_default_border_image_slice_bottom("border-image-slice-bottom", StyleGetValue::from_percentage(100.0f));
	StylePropertyDefault style_default_border_image_slice_left("border-image-slice-left", StyleGetValue::from_percentage(100.0f));
	StylePropertyDefault style_default_border_image_slice_center("border-image-slice-center", StyleGetValue::from_keyword("none"));
	StylePropertyDefault style_default_border_image_width_top("border-image-width-top", StyleGetValue::from_number(1.0f));
	StylePropertyDefault style_default_border_image_width_right("border-image-width-right", StyleGetValue::from_number(1.0f));
	StylePropertyDefault style_default_border_image_width_bottom("border-image-width-bottom", StyleGetValue::from_number(1.0f));
	StylePropertyDefault style_default_border_image_width_left("border-image-width-left", StyleGetValue::from_number(1.0f));
	StylePropertyDefault style_default_border_image_outset_top("border-image-outset-top", StyleGetValue::from_length(0.0f));
	StylePropertyDefault style_default_border_image_outset_right("border-image-outset-right", StyleGetValue::from_length(0.0f));
	StylePropertyDefault style_default_border_image_outset_bottom("border-image-outset-bottom", StyleGetValue::from_length(0.0f));
	StylePropertyDefault style_default_border_image_outset_left("border-image-outset-left", StyleGetValue::from_length(0.0f));
	StylePropertyDefault style_default_border_image_image_repeat_x("border-image-repeat-x", StyleGetValue::from_keyword("stretch"));
	StylePropertyDefault style_default_border_image_image_repeat_y("border-image-repeat-y", StyleGetValue::from_keyword("stretch"));

	BorderImagePropertyParser style_parser_border_image;
	BorderImageOutsetPropertyParser style_parser_border_image_outset;
	BorderImageRepeatPropertyParser style_parser_border_image_repeat;
	BorderImageSlicePropertyParser style_parser_border_image_slice;
	BorderImageSourcePropertyParser style_parser_border_image_source;
	BorderImageWidthPropertyParser style_parser_border_image_width;

	void BorderImagePropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		StyleValue border_image_source;
		StyleValue border_image_slice[5];
		StyleValue border_image_width[4];
		StyleValue border_image_outset[4];
		StyleValue border_image_repeat[2];

		if (tokens.size() == 1 && tokens[0].type == StyleTokenType::ident && equals(tokens[0].value, "inherit"))
		{
			border_image_source = StyleValue::from_keyword("inherit");
			for (int i = 0; i < 4; i++)
			{
				border_image_slice[i] = StyleValue::from_keyword("inherit");
				border_image_width[i] = StyleValue::from_keyword("inherit");
				border_image_outset[i] = StyleValue::from_keyword("inherit");
			}
			border_image_slice[4] = StyleValue::from_keyword("inherit");
			border_image_repeat[0] = StyleValue::from_keyword("inherit");
			border_image_repeat[1] = StyleValue::from_keyword("inherit");

			setter->set_value("border-image-source", border_image_source);
			setter->set_value("border-image-slice-top", border_image_slice[0]);
			setter->set_value("border-image-slice-right", border_image_slice[1]);
			setter->set_value("border-image-slice-bottom", border_image_slice[2]);
			setter->set_value("border-image-slice-left", border_image_slice[3]);
			setter->set_value("border-image-slice-center", border_image_slice[4]);
			setter->set_value("border-image-width-top", border_image_width[0]);
			setter->set_value("border-image-width-right", border_image_width[1]);
			setter->set_value("border-image-width-bottom", border_image_width[2]);
			setter->set_value("border-image-width-left", border_image_width[3]);
			setter->set_value("border-image-outset-top", border_image_outset[0]);
			setter->set_value("border-image-outset-right", border_image_outset[1]);
			setter->set_value("border-image-outset-bottom", border_image_outset[2]);
			setter->set_value("border-image-outset-left", border_image_outset[3]);
			setter->set_value("border-image-repeat-x", border_image_repeat[0]);
			setter->set_value("border-image-repeat-y", border_image_repeat[1]);

			return;
		}

		bool source_specified = false;
		bool slice_specified = false;
		bool repeat_specified = false;

		size_t pos = 0;
		do
		{
			if (!source_specified && parse_source(border_image_source, pos, tokens))
			{
				source_specified = true;
			}
			else if (!slice_specified && parse_slice(border_image_slice, pos, tokens))
			{
				slice_specified = true;

				size_t next_pos = pos;
				StyleToken token = next_token(next_pos, tokens);
				if (token.type == StyleTokenType::delim && token.value == "/")
				{
					pos = next_pos;
					if (parse_width(border_image_width, pos, tokens))
					{
						next_pos = pos;
						StyleToken token = next_token(next_pos, tokens);
						if (token.type == StyleTokenType::delim && token.value == "/")
						{
							pos = next_pos;
							if (!parse_outset(border_image_outset, pos, tokens))
							{
								return;
							}
						}
					}
					else if (!parse_outset(border_image_outset, pos, tokens))
					{
						return;
					}
				}
			}
			else if (!repeat_specified && parse_repeat(border_image_repeat, pos, tokens))
			{
				repeat_specified = true;
			}
			else
			{
				return;
			}
		} while (pos != tokens.size());

		setter->set_value("border-image-source", border_image_source);
		setter->set_value("border-image-slice-top", border_image_slice[0]);
		setter->set_value("border-image-slice-right", border_image_slice[1]);
		setter->set_value("border-image-slice-bottom", border_image_slice[2]);
		setter->set_value("border-image-slice-left", border_image_slice[3]);
		setter->set_value("border-image-slice-center", border_image_slice[4]);
		setter->set_value("border-image-width-top", border_image_width[0]);
		setter->set_value("border-image-width-right", border_image_width[1]);
		setter->set_value("border-image-width-bottom", border_image_width[2]);
		setter->set_value("border-image-width-left", border_image_width[3]);
		setter->set_value("border-image-outset-top", border_image_outset[0]);
		setter->set_value("border-image-outset-right", border_image_outset[1]);
		setter->set_value("border-image-outset-bottom", border_image_outset[2]);
		setter->set_value("border-image-outset-left", border_image_outset[3]);
		setter->set_value("border-image-repeat-x", border_image_repeat[0]);
		setter->set_value("border-image-repeat-y", border_image_repeat[1]);
	}

	bool BorderImagePropertyParser::parse_source(StyleValue &border_image_source, size_t &parse_pos, const std::vector<StyleToken> &tokens)
	{
		size_t pos = parse_pos;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident)
		{
			if (equals(token.value, "none"))
				border_image_source = StyleValue::from_keyword("none");
			else
				return false;
		}
		else if (token.type == StyleTokenType::uri)
		{
			border_image_source = StyleValue::from_url(token.value);
		}
		else
		{
			return false;
		}

		parse_pos = pos;
		return true;
	}

	bool BorderImagePropertyParser::parse_slice(StyleValue *border_image_slice, size_t &parse_pos, const std::vector<StyleToken> &tokens)
	{
		size_t pos = parse_pos;
		size_t last_pos = pos;
		StyleToken token = next_token(pos, tokens);

		int num_lengths;
		StyleValue values[4];
		for (num_lengths = 0; num_lengths < 4; num_lengths++)
		{
			if (token.type == StyleTokenType::percentage)
			{
				values[num_lengths] = StyleValue::from_percentage(StringHelp::text_to_float(token.value));
			}
			else if (token.type == StyleTokenType::number)
			{
				values[num_lengths] = StyleValue::from_number(StringHelp::text_to_float(token.value));
			}
			else
			{
				break;
			}

			last_pos = pos;
			token = next_token(pos, tokens);
		}

		bool fill_center = false;
		if (token.type == StyleTokenType::ident && equals(token.value, "fill"))
		{
			fill_center = true;
		}
		else
		{
			pos = last_pos;
		}

		if (num_lengths < 1)
			return false;

		if (num_lengths == 1)
		{
			for (int i = 1; i < 4; i++)
			{
				values[i] = values[0];
			}
		}
		else if (num_lengths == 2)
		{
			values[2] = values[0];
			values[3] = values[1];
		}
		else if (num_lengths == 3)
		{
			values[3] = values[1];
		}

		for (int i = 0; i < 4; i++)
			border_image_slice[i] = values[i];
		border_image_slice[4] = StyleValue::from_keyword(fill_center ? "fill" : "none");

		parse_pos = pos;
		return true;
	}

	bool BorderImagePropertyParser::parse_width(StyleValue *border_image_width, size_t &parse_pos, const std::vector<StyleToken> &tokens)
	{
		size_t pos = parse_pos;
		size_t last_pos = pos;
		StyleToken token = next_token(pos, tokens);

		int num_lengths;
		StyleValue values[4];
		for (num_lengths = 0; num_lengths < 4; num_lengths++)
		{
			if (is_length(token))
			{
				if (!parse_length(token, values[num_lengths]))
					return false;
			}
			else if (token.type == StyleTokenType::number)
			{
				values[num_lengths] = StyleValue::from_number(StringHelp::text_to_float(token.value));
			}
			else if (token.type == StyleTokenType::percentage)
			{
				values[num_lengths] = StyleValue::from_percentage(StringHelp::text_to_float(token.value));
			}
			else if (token.type == StyleTokenType::ident && equals(token.value, "auto"))
			{
				values[num_lengths] = StyleValue::from_keyword("auto");
			}
			else
			{
				break;
			}

			last_pos = pos;
			token = next_token(pos, tokens);
		}
		if (num_lengths < 1)
			return false;

		pos = last_pos;

		if (num_lengths == 1)
		{
			for (int i = 1; i < 4; i++)
			{
				values[i] = values[0];
			}
		}
		else if (num_lengths == 2)
		{
			values[2] = values[0];
			values[3] = values[1];
		}
		else if (num_lengths == 3)
		{
			values[3] = values[1];
		}

		for (int i = 0; i < 4; i++)
			border_image_width[i] = values[i];

		parse_pos = pos;
		return true;
	}

	bool BorderImagePropertyParser::parse_outset(StyleValue *border_image_outset, size_t &parse_pos, const std::vector<StyleToken> &tokens)
	{
		size_t pos = parse_pos;
		size_t last_pos = pos;
		StyleToken token = next_token(pos, tokens);

		int num_lengths;
		StyleValue values[4];
		for (num_lengths = 0; num_lengths < 4; num_lengths++)
		{
			if (is_length(token))
			{
				if (!parse_length(token, values[num_lengths]))
					return false;
			}
			else if (token.type == StyleTokenType::number)
			{
				values[num_lengths] = StyleValue::from_number(StringHelp::text_to_float(token.value));
			}
			else
			{
				break;
			}

			last_pos = pos;
			token = next_token(pos, tokens);
		}
		if (num_lengths < 1)
			return false;

		pos = last_pos;

		if (num_lengths == 1)
		{
			for (int i = 1; i < 4; i++)
			{
				values[i] = values[0];
			}
		}
		else if (num_lengths == 2)
		{
			values[2] = values[0];
			values[3] = values[1];
		}
		else if (num_lengths == 3)
		{
			values[3] = values[1];
		}

		for (int i = 0; i < 4; i++)
			border_image_outset[i] = values[i];

		parse_pos = pos;

		return true;
	}

	bool BorderImagePropertyParser::parse_repeat(StyleValue *border_image_repeat, size_t &parse_pos, const std::vector<StyleToken> &tokens)
	{
		size_t pos = parse_pos;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && equals(token.value, "stretch"))
		{
			border_image_repeat[0] = StyleValue::from_keyword("stretch");
		}
		else if (token.type == StyleTokenType::ident && equals(token.value, "repeat"))
		{
			border_image_repeat[0] = StyleValue::from_keyword("repeat");
		}
		else if (token.type == StyleTokenType::ident && equals(token.value, "round"))
		{
			border_image_repeat[0] = StyleValue::from_keyword("round");
		}
		else
		{
			return false;
		}

		border_image_repeat[1] = border_image_repeat[0];
		parse_pos = pos;

		if (pos != tokens.size())
		{
			token = next_token(pos, tokens);
			if (token.type == StyleTokenType::ident && equals(token.value, "stretch"))
			{
				border_image_repeat[1] = StyleValue::from_keyword("stretch");
				parse_pos = pos;
			}
			else if (token.type == StyleTokenType::ident && equals(token.value, "repeat"))
			{
				border_image_repeat[1] = StyleValue::from_keyword("repeat");
				parse_pos = pos;
			}
			else if (token.type == StyleTokenType::ident && equals(token.value, "round"))
			{
				border_image_repeat[1] = StyleValue::from_keyword("round");
				parse_pos = pos;
			}
		}
		return true;
	}

	void BorderImageOutsetPropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		StyleValue border_image_outset_left;
		StyleValue border_image_outset_top;
		StyleValue border_image_outset_right;
		StyleValue border_image_outset_bottom;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);

		if (token.type == StyleTokenType::ident && pos == tokens.size() && equals(token.value, "inherit"))
		{
			border_image_outset_left = StyleValue::from_keyword("inherit");
			border_image_outset_top = StyleValue::from_keyword("inherit");
			border_image_outset_right = StyleValue::from_keyword("inherit");
			border_image_outset_bottom = StyleValue::from_keyword("inherit");
		}
		else
		{
			int num_lengths;
			StyleValue values[4];
			for (num_lengths = 0; num_lengths < 4; num_lengths++)
			{
				if (is_length(token))
				{
					if (!parse_length(token, values[num_lengths]))
						return;
				}
				else if (token.type == StyleTokenType::number)
				{
					values[num_lengths] = StyleValue::from_number(StringHelp::text_to_float(token.value));
				}
				else
				{
					break;
				}

				token = next_token(pos, tokens);
			}
			if (num_lengths < 1 || pos != tokens.size())
				return;

			if (num_lengths == 1)
			{
				for (int i = 1; i < 4; i++)
				{
					values[i] = values[0];
				}
			}
			else if (num_lengths == 2)
			{
				values[2] = values[0];
				values[3] = values[1];
			}
			else if (num_lengths == 3)
			{
				values[3] = values[1];
			}

			border_image_outset_top = values[0];
			border_image_outset_right = values[1];
			border_image_outset_bottom = values[2];
			border_image_outset_left = values[3];
		}

		setter->set_value("border-image-outset-top", border_image_outset_top);
		setter->set_value("border-image-outset-right", border_image_outset_right);
		setter->set_value("border-image-outset-bottom", border_image_outset_bottom);
		setter->set_value("border-image-outset-left", border_image_outset_left);
	}

	void BorderImageRepeatPropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		StyleValue border_image_repeat_x, border_image_repeat_y;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size() && equals(token.value, "inherit"))
		{
			border_image_repeat_x = StyleValue::from_keyword("inherit");
			border_image_repeat_y = StyleValue::from_keyword("inherit");
			setter->set_value("border-image-repeat-x", border_image_repeat_x);
			setter->set_value("border-image-repeat-y", border_image_repeat_y);
			return;
		}
		else if (token.type == StyleTokenType::ident && equals(token.value, "stretch"))
		{
			border_image_repeat_x = StyleValue::from_keyword("stretch");
		}
		else if (token.type == StyleTokenType::ident && equals(token.value, "repeat"))
		{
			border_image_repeat_x = StyleValue::from_keyword("repeat");
		}
		else if (token.type == StyleTokenType::ident && equals(token.value, "round"))
		{
			border_image_repeat_x = StyleValue::from_keyword("round");
		}
		else
		{
			return;
		}

		if (pos == tokens.size())
		{
			border_image_repeat_y = border_image_repeat_x;
			setter->set_value("border-image-repeat-x", border_image_repeat_x);
			setter->set_value("border-image-repeat-y", border_image_repeat_y);
		}
		else
		{
			token = next_token(pos, tokens);
			if (token.type == StyleTokenType::ident && equals(token.value, "stretch"))
			{
				border_image_repeat_y = StyleValue::from_keyword("stretch");
			}
			else if (token.type == StyleTokenType::ident && equals(token.value, "repeat"))
			{
				border_image_repeat_y = StyleValue::from_keyword("repeat");
			}
			else if (token.type == StyleTokenType::ident && equals(token.value, "round"))
			{
				border_image_repeat_y = StyleValue::from_keyword("round");
			}
			else
			{
				return;
			}

			setter->set_value("border-image-repeat-x", border_image_repeat_x);
			setter->set_value("border-image-repeat-y", border_image_repeat_y);
		}
	}

	void BorderImageSlicePropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		StyleValue border_image_slice_top;
		StyleValue border_image_slice_right;
		StyleValue border_image_slice_bottom;
		StyleValue border_image_slice_left;
		StyleValue border_image_slice_center;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);

		if (token.type == StyleTokenType::ident && pos == tokens.size() && equals(token.value, "inherit"))
		{
			border_image_slice_top = StyleValue::from_keyword("inherit");
			border_image_slice_right = StyleValue::from_keyword("inherit");
			border_image_slice_bottom = StyleValue::from_keyword("inherit");
			border_image_slice_left = StyleValue::from_keyword("inherit");
			border_image_slice_center = StyleValue::from_keyword("inherit");
			setter->set_value("border-image-slice-top", border_image_slice_top);
			setter->set_value("border-image-slice-right", border_image_slice_right);
			setter->set_value("border-image-slice-bottom", border_image_slice_bottom);
			setter->set_value("border-image-slice-left", border_image_slice_left);
			setter->set_value("border-image-slice-center", border_image_slice_center);
		}
		else
		{
			int num_lengths;
			StyleValue values[4];
			for (num_lengths = 0; num_lengths < 4; num_lengths++)
			{
				if (token.type == StyleTokenType::percentage)
				{
					values[num_lengths] = StyleValue::from_percentage(StringHelp::text_to_float(token.value));
				}
				else if (token.type == StyleTokenType::number)
				{
					values[num_lengths] = StyleValue::from_number(StringHelp::text_to_float(token.value));
				}
				else
				{
					break;
				}

				token = next_token(pos, tokens);
			}

			bool fill_center = false;
			if (token.type == StyleTokenType::ident && equals(token.value, "fill"))
			{
				fill_center = true;
				token = next_token(pos, tokens);
			}

			if (num_lengths < 1 || pos != tokens.size())
				return;

			if (num_lengths == 1)
			{
				for (int i = 1; i < 4; i++)
				{
					values[i] = values[0];
				}
			}
			else if (num_lengths == 2)
			{
				values[2] = values[0];
				values[3] = values[1];
			}
			else if (num_lengths == 3)
			{
				values[3] = values[1];
			}

			border_image_slice_center = StyleValue::from_keyword(fill_center ? "fill" : "none");
			border_image_slice_top = values[0];
			border_image_slice_right = values[1];
			border_image_slice_bottom = values[2];
			border_image_slice_left = values[3];

			setter->set_value("border-image-slice-top", border_image_slice_top);
			setter->set_value("border-image-slice-right", border_image_slice_right);
			setter->set_value("border-image-slice-bottom", border_image_slice_bottom);
			setter->set_value("border-image-slice-left", border_image_slice_left);
			setter->set_value("border-image-slice-center", border_image_slice_center);
		}
	}

	void BorderImageSourcePropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		StyleValue border_image_source;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "none"))
			{
				border_image_source = StyleValue::from_keyword("none");
				setter->set_value("border-image-source", border_image_source);
			}
			else if (equals(token.value, "inherit"))
			{
				border_image_source = StyleValue::from_keyword("inherit");
				setter->set_value("border-image-source", border_image_source);
			}
		}
		else if (token.type == StyleTokenType::uri && pos == tokens.size())
		{
			border_image_source = StyleValue::from_url(token.value);
			setter->set_value("border-image-source", border_image_source);
		}
	}

	void BorderImageWidthPropertyParser::parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser)
	{
		auto &tokens = parser.tokens;

		StyleValue border_image_width_top;
		StyleValue border_image_width_right;
		StyleValue border_image_width_bottom;
		StyleValue border_image_width_left;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);

		if (token.type == StyleTokenType::ident && pos == tokens.size() && equals(token.value, "inherit"))
		{
			border_image_width_top = StyleValue::from_keyword("inherit");
			border_image_width_right = StyleValue::from_keyword("inherit");
			border_image_width_bottom = StyleValue::from_keyword("inherit");
			border_image_width_left = StyleValue::from_keyword("inherit");
			setter->set_value("border-image-width-top", border_image_width_top);
			setter->set_value("border-image-width-right", border_image_width_right);
			setter->set_value("border-image-width-bottom", border_image_width_bottom);
			setter->set_value("border-image-width-left", border_image_width_left);
		}
		else
		{
			int num_lengths;
			StyleValue values[4];
			for (num_lengths = 0; num_lengths < 4; num_lengths++)
			{
				if (is_length(token))
				{
					if (!parse_length(token, values[num_lengths]))
						return;
				}
				else if (token.type == StyleTokenType::number)
				{
					values[num_lengths] = StyleValue::from_number(StringHelp::text_to_float(token.value));
				}
				else if (token.type == StyleTokenType::percentage)
				{
					values[num_lengths] = StyleValue::from_percentage(StringHelp::text_to_float(token.value));
				}
				else if (token.type == StyleTokenType::ident && equals(token.value, "auto"))
				{
					values[num_lengths] = StyleValue::from_keyword("auto");
				}
				else
				{
					break;
				}

				token = next_token(pos, tokens);
			}
			if (num_lengths < 1 || pos != tokens.size())
				return;

			if (num_lengths == 1)
			{
				for (int i = 1; i < 4; i++)
				{
					values[i] = values[0];
				}
			}
			else if (num_lengths == 2)
			{
				values[2] = values[0];
				values[3] = values[1];
			}
			else if (num_lengths == 3)
			{
				values[3] = values[1];
			}

			border_image_width_top = values[0];
			border_image_width_right = values[1];
			border_image_width_bottom = values[2];
			border_image_width_left = values[3];

			setter->set_value("border-image-width-top", border_image_width_top);
			setter->set_value("border-image-width-right", border_image_width_right);
			setter->set_value("border-image-width-bottom", border_image_width_bottom);
			setter->set_value("border-image-width-left", border_image_width_left);
		}
	}
}
