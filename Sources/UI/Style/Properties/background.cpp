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
#include "background.h"

namespace clan
{
	BackgroundPropertyParser style_parser_background;
	BackgroundAttachmentPropertyParser style_parser_background_attachment;
	BackgroundClipPropertyParser style_parser_background_clip;
	BackgroundColorPropertyParser style_parser_background_color;
	BackgroundImagePropertyParser style_parser_background_image;
	BackgroundOriginPropertyParser style_parser_background_origin;
	BackgroundPositionPropertyParser style_parser_background_position;
	BackgroundRepeatPropertyParser style_parser_background_repeat;
	BackgroundSizePropertyParser style_parser_background_size;

	void BackgroundPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}

	void BackgroundAttachmentPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		std::vector<StyleToken> tokens = StyleTokenizer::tokenize(value);

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);

		StyleValue attachment;
		std::vector<StyleValue> attachments;

		if (token.type == StyleTokenType::ident && pos == tokens.size() && equals(token.value, "inherit"))
		{
			attachment = StyleValue::from_keyword("inherit");
			setter->set_value("background-attachment", attachment);
			setter->set_value_array("background-attachments", attachments);
		}
		else
		{
			attachment = StyleValue::from_keyword("array");
			attachments.clear();
			while (true)
			{
				if (token.type != StyleTokenType::ident)
					return;

				if (equals(token.value, "scroll"))
					attachments.push_back(StyleValue::from_keyword("scroll"));
				else if (equals(token.value, "fixed"))
					attachments.push_back(StyleValue::from_keyword("fixed"));
				else
					return;

				if (pos != tokens.size())
				{
					token = next_token(pos, tokens);
					if (token.type != StyleTokenType::delim || token.value != ",")
						return;
					token = next_token(pos, tokens);
				}
				else
				{
					break;
				}
			}
			setter->set_value("background-attachment", attachment);
			setter->set_value_array("background-attachments", attachments);
		}
	}

	void BackgroundClipPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		std::vector<StyleToken> tokens = StyleTokenizer::tokenize(value);

		StyleValue background_clip;
		std::vector<StyleValue> background_clips;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);

		if (token.type == StyleTokenType::ident && pos == tokens.size() && equals(token.value, "inherit"))
		{
			background_clip = StyleValue::from_keyword("inherit");
			setter->set_value("background-clip", background_clip);
			setter->set_value_array("background-clips", background_clips);
		}
		else
		{
			background_clip = StyleValue::from_keyword("array");
			background_clips.clear();
			while (true)
			{
				if (token.type != StyleTokenType::ident)
					return;

				if (equals(token.value, "border-box"))
				{
					background_clips.push_back(StyleValue::from_keyword("border-box"));
				}
				else if (equals(token.value, "padding-box"))
				{
					background_clips.push_back(StyleValue::from_keyword("padding-box"));
				}
				else if (equals(token.value, "content-box"))
				{
					background_clips.push_back(StyleValue::from_keyword("content-box"));
				}
				else
				{
					return;
				}

				if (pos == tokens.size())
					break;

				token = next_token(pos, tokens);
				if (token.type != StyleTokenType::delim || token.value != ",")
					return;

				token = next_token(pos, tokens);
			}

			setter->set_value("background-clip", background_clip);
			setter->set_value_array("background-clips", background_clips);
		}
	}

	void BackgroundColorPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		std::vector<StyleToken> tokens = StyleTokenizer::tokenize(value);

		StyleValue background_color;

		size_t pos = 0;
		Colorf color;
		if (parse_color(tokens, pos, color) && pos == tokens.size())
		{
			background_color = StyleValue::from_color(color);
			setter->set_value("background-color", background_color);
		}
		else
		{
			StyleToken token = next_token(pos, tokens);
			if (token.type == StyleTokenType::ident && pos == tokens.size())
			{
				if (equals(token.value, "inherit"))
				{
					background_color = StyleValue::from_keyword("inherit");
					setter->set_value("background-color", background_color);
				}
			}
		}
	}

	void BackgroundImagePropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		std::vector<StyleToken> tokens = StyleTokenizer::tokenize(value);

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);

		StyleValue background_image;
		std::vector<StyleValue> background_images;

		if (token.type == StyleTokenType::ident && pos == tokens.size() && equals(token.value, "inherit"))
		{
			background_image = StyleValue::from_keyword("inherit");
			setter->set_value("background-image", background_image);
			setter->set_value_array("background-images", background_images);
		}
		else
		{
			background_image = StyleValue::from_keyword("array");
			background_images.clear();
			while (true)
			{
				if (token.type == StyleTokenType::ident && equals(token.value, "none"))
					background_images.push_back(StyleValue::from_keyword("none"));
				else if (token.type == StyleTokenType::uri)
					background_images.push_back(StyleValue::from_url(token.value));
				else
					return;

				if (pos != tokens.size())
				{
					token = next_token(pos, tokens);
					if (token.type != StyleTokenType::delim || token.value != ",")
						return;
					token = next_token(pos, tokens);
				}
				else
				{
					break;
				}
			}
		}

		setter->set_value("background-image", background_image);
		setter->set_value_array("background-images", background_images);
	}

	void BackgroundOriginPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		std::vector<StyleToken> tokens = StyleTokenizer::tokenize(value);

		StyleValue background_origin;
		std::vector<StyleValue> background_origins;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);

		if (token.type == StyleTokenType::ident && pos == tokens.size() && equals(token.value, "inherit"))
		{
			background_origin = StyleValue::from_keyword("inherit");
			setter->set_value("background-origin", background_origin);
			setter->set_value_array("background-origins", background_origins);
		}
		else
		{
			background_origin = StyleValue::from_keyword("array");
			background_origins.clear();
			while (true)
			{
				if (token.type != StyleTokenType::ident)
					return;

				if (equals(token.value, "border-box"))
				{
					background_origin = StyleValue::from_keyword("border-box");
				}
				else if (equals(token.value, "padding-box"))
				{
					background_origin = StyleValue::from_keyword("padding-box");
				}
				else if (equals(token.value, "content-box"))
				{
					background_origin = StyleValue::from_keyword("content-box");
				}
				else
				{
					return;
				}

				if (pos == tokens.size())
					break;

				token = next_token(pos, tokens);
				if (token.type != StyleTokenType::delim || token.value != ",")
					return;

				token = next_token(pos, tokens);
			}
		}

		setter->set_value("background-origin", background_origin);
		setter->set_value_array("background-origins", background_origins);
	}

	void BackgroundPositionPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		std::vector<StyleToken> tokens = StyleTokenizer::tokenize(value);

		StyleValue position;
		std::vector<StyleValue> positions_x;
		std::vector<StyleValue> positions_y;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);

		if (token.type == StyleTokenType::ident && equals(token.value, "inherit") && tokens.size() == 1)
		{
			position = StyleValue::from_keyword("inherit");
			setter->set_value("background-position", position);
			setter->set_value_array("background-positions-x", positions_x);
			setter->set_value_array("background-positions-y", positions_y);
			return;
		}

		position = StyleValue::from_keyword("array");

		bool done = false;
		while (!done)
		{
			StyleValue bg_pos_x, bg_pos_y;
			bool x_specified = false;
			bool y_specified = false;
			bool center_specified = false;

			while (true)
			{
				if (token.type == StyleTokenType::ident)
				{
					if (!y_specified && equals(token.value, "top"))
					{
						bg_pos_y = StyleValue::from_keyword("top");
						y_specified = true;

						if (center_specified)
						{
							bg_pos_x = StyleValue::from_keyword("center");
							x_specified = true;
							center_specified = false;
						}
					}
					else if (!y_specified && equals(token.value, "bottom"))
					{
						bg_pos_y = StyleValue::from_keyword("bottom");
						y_specified = true;

						if (center_specified)
						{
							bg_pos_x = StyleValue::from_keyword("center");
							x_specified = true;
							center_specified = false;
						}
					}
					else if (!x_specified && equals(token.value, "left"))
					{
						bg_pos_x = StyleValue::from_keyword("left");
						x_specified = true;

						if (center_specified)
						{
							bg_pos_y = StyleValue::from_keyword("center");
							y_specified = true;
							center_specified = false;
						}
					}
					else if (!x_specified && equals(token.value, "right"))
					{
						bg_pos_x = StyleValue::from_keyword("right");
						x_specified = true;

						if (center_specified)
						{
							bg_pos_y = StyleValue::from_keyword("center");
							y_specified = true;
							center_specified = false;
						}
					}
					else if (equals(token.value, "center"))
					{
						if (center_specified)
						{
							bg_pos_x = StyleValue::from_keyword("center");
							x_specified = true;
							center_specified = false;
						}

						if (x_specified && !y_specified)
						{
							bg_pos_y = StyleValue::from_keyword("center");
							y_specified = true;
						}
						else if (y_specified && !x_specified)
						{
							bg_pos_x = StyleValue::from_keyword("center");
							x_specified = true;
						}
						else if (!x_specified && !y_specified)
						{
							center_specified = true;
						}
						else
						{
							debug_parse_error(name, tokens);
							return;
						}
					}
				}
				else if (is_length(token))
				{
					StyleValue length;
					if (parse_length(token, length))
					{
						if (center_specified)
						{
							bg_pos_x = StyleValue::from_keyword("center");
							x_specified = true;
							center_specified = false;
						}

						if (!x_specified && !y_specified)
						{
							bg_pos_x = length;
							x_specified = true;
						}
						else if (x_specified && !y_specified)
						{
							bg_pos_y = length;
							y_specified = true;
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
				else if (token.type == StyleTokenType::percentage)
				{
					if (center_specified)
					{
						bg_pos_x = StyleValue::from_keyword("center");
						x_specified = true;
						center_specified = false;
					}

					if (!x_specified && !y_specified)
					{
						bg_pos_x = StyleValue::from_percentage(StringHelp::text_to_float(token.value));
						x_specified = true;
					}
					else if (x_specified && !y_specified)
					{
						bg_pos_y = StyleValue::from_percentage(StringHelp::text_to_float(token.value));
						y_specified = true;
					}
					else
					{
						debug_parse_error(name, tokens);
						return;
					}
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
							if (center_specified)
							{
								bg_pos_x = StyleValue::from_keyword("center");
								x_specified = true;
								center_specified = false;
							}

							if (!x_specified && !y_specified)
							{
								bg_pos_x = length;
								x_specified = true;
							}
							else if (x_specified && !y_specified)
							{
								bg_pos_y = length;
								y_specified = true;
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
					else if (token.type == StyleTokenType::percentage)
					{
						if (center_specified)
						{
							bg_pos_x = StyleValue::from_keyword("center");
							x_specified = true;
							center_specified = false;
						}

						if (!x_specified && !y_specified)
						{
							bg_pos_x = StyleValue::from_percentage(-StringHelp::text_to_float(token.value));
							x_specified = true;
						}
						else if (x_specified && !y_specified)
						{
							bg_pos_y = StyleValue::from_percentage(-StringHelp::text_to_float(token.value));
							y_specified = true;
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
				else
				{
					debug_parse_error(name, tokens);
					return;
				}

				if (pos == tokens.size())
				{
					done = true;
					break;
				}
				else
				{
					token = next_token(pos, tokens);
					if (token.type == StyleTokenType::delim && token.value == ",")
						break;
				}
			}

			if (!x_specified)
				bg_pos_x = StyleValue::from_keyword("center");
			else if (!y_specified)
				bg_pos_y = StyleValue::from_keyword("center");

			positions_x.push_back(bg_pos_x);
			positions_y.push_back(bg_pos_y);
		}

		setter->set_value("background-position", position);
		setter->set_value_array("background-positions-x", positions_x);
		setter->set_value_array("background-positions-y", positions_y);
	}

	void BackgroundRepeatPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		std::vector<StyleToken> tokens = StyleTokenizer::tokenize(value);

		StyleValue background_repeat;
		std::vector<StyleValue> background_repeats_x;
		std::vector<StyleValue> background_repeats_y;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);

		if (token.type == StyleTokenType::ident && pos == tokens.size() && equals(token.value, "inherit"))
		{
			background_repeat = StyleValue::from_keyword("inherit");
			setter->set_value("background-repeat", background_repeat);
			setter->set_value_array("background-repeat-x", background_repeats_x);
			setter->set_value_array("background-repeat-y", background_repeats_y);
		}
		else
		{
			background_repeat = StyleValue::from_keyword("array");
			background_repeats_x.clear();
			background_repeats_y.clear();
			while (true)
			{
				if (token.type != StyleTokenType::ident)
					return;

				StyleValue repeat_x, repeat_y;
				bool single_style = false;

				if (equals(token.value, "repeat"))
				{
					repeat_x = StyleValue::from_keyword("repeat");
				}
				else if (equals(token.value, "repeat-x"))
				{
					repeat_x = StyleValue::from_keyword("repeat");
					repeat_y = StyleValue::from_keyword("no-repeat");
					single_style = true;
				}
				else if (equals(token.value, "repeat-y"))
				{
					repeat_x = StyleValue::from_keyword("no-repeat");
					repeat_y = StyleValue::from_keyword("repeat");
					single_style = true;
				}
				else if (equals(token.value, "no-repeat"))
				{
					repeat_x = StyleValue::from_keyword("no-repeat");
				}
				else if (equals(token.value, "space"))
				{
					repeat_x = StyleValue::from_keyword("space");
				}
				else if (equals(token.value, "round"))
				{
					repeat_x = StyleValue::from_keyword("round");
				}
				else
				{
					return;
				}

				if (pos == tokens.size())
				{
					if (!single_style)
						repeat_y = repeat_x;
					background_repeats_x.push_back(repeat_x);
					background_repeats_y.push_back(repeat_y);
					break;
				}

				token = next_token(pos, tokens);
				if (token.type == StyleTokenType::delim && token.value == ",")
				{
					if (!single_style)
						repeat_y = repeat_x;
					background_repeats_x.push_back(repeat_x);
					background_repeats_y.push_back(repeat_y);
				}
				else if (token.type == StyleTokenType::ident && !single_style)
				{
					if (equals(token.value, "repeat"))
					{
						repeat_y = StyleValue::from_keyword("repeat");
					}
					else if (equals(token.value, "no-repeat"))
					{
						repeat_y = StyleValue::from_keyword("no-repeat");
					}
					else if (equals(token.value, "space"))
					{
						repeat_y = StyleValue::from_keyword("space");
					}
					else if (equals(token.value, "round"))
					{
						repeat_y = StyleValue::from_keyword("round");
					}
					else
					{
						return;
					}

					background_repeats_x.push_back(repeat_x);
					background_repeats_y.push_back(repeat_y);

					if (pos == tokens.size())
						break;

					if (token.type != StyleTokenType::delim || token.value != ",")
						return;
				}
				else
				{
					return;
				}

				token = next_token(pos, tokens);
			}
		}

		setter->set_value("background-repeat", background_repeat);
		setter->set_value_array("background-repeat-x", background_repeats_x);
		setter->set_value_array("background-repeat-y", background_repeats_y);
	}

	void BackgroundSizePropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		std::vector<StyleToken> tokens = StyleTokenizer::tokenize(value);

		StyleValue background_size;
		std::vector<StyleValue> background_sizes_x;
		std::vector<StyleValue> background_sizes_y;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);

		if (token.type == StyleTokenType::ident && pos == tokens.size() && equals(token.value, "inherit"))
		{
			background_size = StyleValue::from_keyword("inherit");
			setter->set_value("background-size", background_size);
			setter->set_value_array("background-sizes-x", background_sizes_x);
			setter->set_value_array("background-sizes-y", background_sizes_y);
		}
		else
		{
			background_size = StyleValue::from_keyword("array");
			while (true)
			{
				StyleValue size_x, size_y;

				bool single_value = false;
				if (token.type == StyleTokenType::ident)
				{
					if (equals(token.value, "contain"))
					{
						size_x = StyleValue::from_keyword("contain");
						size_y = StyleValue::from_keyword("contain");
						single_value = true;
					}
					else if (equals(token.value, "cover"))
					{
						size_x = StyleValue::from_keyword("cover");
						size_y = StyleValue::from_keyword("cover");
						single_value = true;
					}
					else if (equals(token.value, "auto"))
					{
						size_x = StyleValue::from_keyword("auto");
					}
					else
					{
						return;
					}
				}
				else if (is_length(token))
				{
					StyleValue length;
					if (parse_length(token, length))
					{
						size_x = length;
					}
					else
					{
						return;
					}
				}
				else if (token.type == StyleTokenType::percentage)
				{
					size_x = StyleValue::from_percentage(StringHelp::text_to_float(token.value));
				}
				else
				{
					return;
				}

				if (pos == tokens.size())
				{
					background_sizes_x.push_back(size_x);
					background_sizes_y.push_back(size_y);
					break;
				}

				token = next_token(pos, tokens);
				if (token.type != StyleTokenType::delim || token.value != ",")
				{
					if (single_value)
					{
						return;
					}
					else if (token.type == StyleTokenType::ident && equals(token.value, "auto"))
					{
						size_y = StyleValue::from_keyword("auto");
					}
					else if (is_length(token))
					{
						StyleValue length;
						if (parse_length(token, length))
						{
							size_y = length;
						}
						else
						{
							return;
						}
					}
					else if (token.type == StyleTokenType::percentage)
					{
						size_y = StyleValue::from_percentage(StringHelp::text_to_float(token.value));
					}
					else
					{
						return;
					}

					if (pos == tokens.size())
					{
						background_sizes_x.push_back(size_x);
						background_sizes_y.push_back(size_y);
						break;
					}

					token = next_token(pos, tokens);
				}

				background_sizes_x.push_back(size_x);
				background_sizes_y.push_back(size_y);

				if (token.type != StyleTokenType::delim || token.value != ",")
					return;

				token = next_token(pos, tokens);
			}
		}

		setter->set_value("background-size", background_size);
		setter->set_value_array("background-sizes-x", background_sizes_x);
		setter->set_value_array("background-sizes-y", background_sizes_y);
	}
}
