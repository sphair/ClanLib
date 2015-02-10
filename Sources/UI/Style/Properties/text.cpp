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
#include "text.h"

namespace clan
{
	void force_link_style_parser_text()
	{
	}

	StylePropertyDefault style_default_color("color", StyleValue::from_color(Colorf(0.0f, 0.0f, 0.0f, 1.0f)));
	StylePropertyDefault style_default_text_align("text-align", StyleValue::from_keyword("left"));
	StylePropertyDefault style_default_text_decoration_underline("text-decoration-underline", StyleValue::from_keyword("hidden"));
	StylePropertyDefault style_default_text_decoration_overline("text-decoration-overline", StyleValue::from_keyword("hidden"));
	StylePropertyDefault style_default_text_decoration_line_through("text-decoration-line-through", StyleValue::from_keyword("hidden"));
	StylePropertyDefault style_default_text_decoration_blink("text-decoration-blink", StyleValue::from_keyword("hidden"));
	StylePropertyDefault style_default_text_indent("text-indent", StyleValue::from_length(0.0f));
	StylePropertyDefault style_default_text_transform("text-transform", StyleValue::from_keyword("none"));
	StylePropertyDefault style_default_word_spacing("word-spacing", StyleValue::from_keyword("normal"));
	StylePropertyDefault style_default_letter_spacing("letter-spacing", StyleValue::from_keyword("normal"));
	StylePropertyDefault style_default_font_style("font-style", StyleValue::from_keyword("normal"));
	StylePropertyDefault style_default_font_variant("font-variant", StyleValue::from_keyword("normal"));
	StylePropertyDefault style_default_font_weight("font-weight", StyleValue::from_keyword("normal"));
	StylePropertyDefault style_default_font_size("font-size", StyleValue::from_keyword("medium"));
	StylePropertyDefault style_default_line_height("line-height", StyleValue::from_keyword("normal"));
	StylePropertyDefault style_default_font_family("font-family", StyleValue::from_keyword("array"));
	StylePropertyDefault style_default_font_family_names("font-family-names[0]", StyleValue::from_keyword("sans-serif"));

	ColorPropertyParser style_parser_color;
	TextAlignPropertyParser style_parser_text_align;
	TextDecorationPropertyParser style_parser_text_decoration;
	TextIndentPropertyParser style_parser_text_indent;
	TextTransformPropertyParser style_parser_text_transform;
	TextShadowPropertyParser style_parser_text_shadow;
	WordSpacingPropertyParser style_parser_word_spacing;
	LetterSpacingPropertyParser style_parser_letter_spacing;
	FontPropertyParser style_parser_font;
	FontFamilyPropertyParser style_parser_font_family;
	FontSizePropertyParser style_parser_font_size;
	LineHeightPropertyParser style_parser_line_height;
	FontStylePropertyParser style_parser_font_style;
	FontVariantPropertyParser style_parser_font_variant;
	FontWeightPropertyParser style_parser_font_weight;

	void ColorPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		std::vector<StyleToken> tokens = StyleTokenizer::tokenize(value);

		StyleValue color;

		size_t pos = 0;
		Colorf colorf;
		if (parse_color(tokens, pos, colorf) && pos == tokens.size())
		{
			color = StyleValue::from_color(colorf);
		}
		else
		{
			StyleToken token = next_token(pos, tokens);
			if (token.type == StyleTokenType::ident && pos == tokens.size())
			{
				if (equals(token.value, "inherit"))
				{
					color = StyleValue::from_keyword("inherit");
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

		setter->set_value("color", color);
	}

	void TextAlignPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		std::vector<StyleToken> tokens = StyleTokenizer::tokenize(value);

		StyleValue text_align;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "left"))
				text_align = StyleValue::from_keyword("left");
			else if (equals(token.value, "right"))
				text_align = StyleValue::from_keyword("right");
			else if (equals(token.value, "center"))
				text_align = StyleValue::from_keyword("center");
			else if (equals(token.value, "justify"))
				text_align = StyleValue::from_keyword("justify");
			else if (equals(token.value, "inherit"))
				text_align = StyleValue::from_keyword("inherit");
			else
				return;
		}
		else
		{
			return;
		}

		setter->set_value("text-align", text_align);
	}

	void TextDecorationPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		std::vector<StyleToken> tokens = StyleTokenizer::tokenize(value);

		StyleValue text_decoration_underline;
		StyleValue text_decoration_overline;
		StyleValue text_decoration_line_through;
		StyleValue text_decoration_blink;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident)
		{
			if (equals(token.value, "none") && pos == tokens.size())
			{
				text_decoration_underline = StyleValue::from_keyword("hidden");
				text_decoration_overline = StyleValue::from_keyword("hidden");
				text_decoration_line_through = StyleValue::from_keyword("hidden");
				text_decoration_blink = StyleValue::from_keyword("disabled");
			}
			else if (equals(token.value, "inherit") && pos == tokens.size())
			{
				text_decoration_underline = StyleValue::from_keyword("inherit");
				text_decoration_overline = StyleValue::from_keyword("inherit");
				text_decoration_line_through = StyleValue::from_keyword("inherit");
				text_decoration_blink = StyleValue::from_keyword("inherit");
			}
			else
			{
				int underline = 0;
				int overline = 0;
				int line_through = 0;
				int blink = 0;
				do
				{
					if (token.type == StyleTokenType::ident)
					{
						if (equals(token.value, "underline"))
							underline++;
						else if (equals(token.value, "overline"))
							overline++;
						else if (equals(token.value, "line-through"))
							line_through++;
						else if (equals(token.value, "blink"))
							blink++;
					}
					else
					{
						debug_parse_error(name, tokens);
						return;
					}

					token = next_token(pos, tokens);
				} while (token.type != StyleTokenType::null);

				if (underline < 2 && overline < 2 && line_through < 2 && blink < 2)
				{
					text_decoration_underline = StyleValue::from_keyword((underline == 1) ? "visible" : "hidden");
					text_decoration_overline = StyleValue::from_keyword((overline == 1) ? "visible" : "hidden");
					text_decoration_line_through = StyleValue::from_keyword((line_through == 1) ? "visible" : "hidden");
					text_decoration_blink = StyleValue::from_keyword((blink == 1) ? "enabled" : "disabled");
				}
				else
				{
					return;
				}
			}
		}
		else
		{
			return;
		}

		setter->set_value("text-decoration-underline", text_decoration_underline);
		setter->set_value("text-decoration-overline", text_decoration_overline);
		setter->set_value("text-decoration-line-through", text_decoration_line_through);
		setter->set_value("text-decoration-blink", text_decoration_blink);
	}

	void TextIndentPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		std::vector<StyleToken> tokens = StyleTokenizer::tokenize(value);

		StyleValue text_indent;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size() && equals(token.value, "inherit"))
		{
			text_indent = StyleValue::from_keyword("inherit");
		}
		else if (is_length(token) && pos == tokens.size())
		{
			StyleValue length;
			if (parse_length(token, length))
			{
				text_indent = length;
			}
			else
			{
				return;
			}
		}
		else if (token.type == StyleTokenType::percentage && pos == tokens.size())
		{
			text_indent = StyleValue::from_percentage(StringHelp::text_to_float(token.value));
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
					text_indent = length;
				}
				else
				{
					return;
				}
			}
			else if (token.type == StyleTokenType::percentage && pos == tokens.size())
			{
				text_indent = StyleValue::from_percentage(-StringHelp::text_to_float(token.value));
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

		setter->set_value("text-indent", text_indent);
	}

	void TextTransformPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		std::vector<StyleToken> tokens = StyleTokenizer::tokenize(value);

		StyleValue text_transform;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "capitalize"))
				text_transform = StyleValue::from_keyword("capitalize");
			else if (equals(token.value, "uppercase"))
				text_transform = StyleValue::from_keyword("uppercase");
			else if (equals(token.value, "lowercase"))
				text_transform = StyleValue::from_keyword("lowercase");
			else if (equals(token.value, "none"))
				text_transform = StyleValue::from_keyword("none");
			else if (equals(token.value, "inherit"))
				text_transform = StyleValue::from_keyword("inherit");
			else
				return;
		}
		else
		{
			return;
		}

		setter->set_value("text-transform", text_transform);
	}

	void TextShadowPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
	}

	void WordSpacingPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		std::vector<StyleToken> tokens = StyleTokenizer::tokenize(value);

		StyleValue word_spacing;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "normal"))
				word_spacing = StyleValue::from_keyword("normal");
			else if (equals(token.value, "inherit"))
				word_spacing = StyleValue::from_keyword("inherit");
			else
				return;
		}
		else if (is_length(token) && pos == tokens.size())
		{
			StyleValue length;
			if (parse_length(token, length))
			{
				word_spacing = length;
			}
			else
			{
				return;
			}
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
					word_spacing = length;
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
		else
		{
			return;
		}

		setter->set_value("word-spacing", word_spacing);
	}

	void LetterSpacingPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		std::vector<StyleToken> tokens = StyleTokenizer::tokenize(value);

		StyleValue letter_spacing;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "normal"))
				letter_spacing = StyleValue::from_keyword("normal");
			else if (equals(token.value, "inherit"))
				letter_spacing = StyleValue::from_keyword("inherit");
			else
				return;
		}
		else if (is_length(token) && pos == tokens.size())
		{
			StyleValue length;
			if (parse_length(token, length))
			{
				letter_spacing = length;
			}
			else
			{
				return;
			}
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
					letter_spacing = length;
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
		else
		{
			return;
		}

		setter->set_value("letter-spacing", letter_spacing);
	}

	void FontPropertyParser::parse(StylePropertySetter *setter, const std::string &propname, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		std::vector<StyleToken> tokens = StyleTokenizer::tokenize(value);

		StyleValue style = StyleValue::from_keyword("normal");
		StyleValue variant = StyleValue::from_keyword("normal");
		StyleValue weight = StyleValue::from_keyword("normal");
		StyleValue size = StyleValue::from_keyword("medium");
		StyleValue line_height = StyleValue::from_keyword("normal");
		StyleValue family = StyleValue::from_keyword("array");
		std::vector<StyleValue> family_names;
		family_names.push_back(StyleValue::from_keyword("sans-serif"));

		bool font_style_set = false;
		bool font_variant_set = false;
		bool font_weight_set = false;
		int normal_count = 0;
		size_t pos = 0;
		StyleToken token;
		while (pos < tokens.size())
		{
			token = next_token(pos, tokens);
			if (token.type == StyleTokenType::ident)
			{
				if (tokens.size() == 1 &&
					(equals(token.value, "caption") ||
					equals(token.value, "icon") ||
					equals(token.value, "menu") ||
					equals(token.value, "message-box") ||
					equals(token.value, "small-caption") ||
					equals(token.value, "status-bar")))
				{
					setter->set_value("font-style", style);
					setter->set_value("font-variant", variant);
					setter->set_value("font-weight", weight);
					setter->set_value("font-size", size);
					setter->set_value("line-height", line_height);
					setter->set_value("font-family", family);
					setter->set_value_array("font-family-names", family_names);
					return;
				}
				else if (equals(token.value, "inherit") && tokens.size() == 1)
				{
					style = StyleValue::from_keyword("inherit");
					variant = StyleValue::from_keyword("inherit");
					weight = StyleValue::from_keyword("inherit");
					size = StyleValue::from_keyword("inherit");
					line_height = StyleValue::from_keyword("inherit");
					family = StyleValue::from_keyword("inherit");
					family_names.clear();

					setter->set_value("font-style", style);
					setter->set_value("font-variant", variant);
					setter->set_value("font-weight", weight);
					setter->set_value("font-size", size);
					setter->set_value("line-height", line_height);
					setter->set_value("font-family", family);
					setter->set_value_array("font-family-names", family_names);
					return;
				}
				else if (equals(token.value, "normal")) // font-style or font-weight or font-variant
				{
					int allowed = 3;
					if (font_style_set)
						allowed--;
					if (font_weight_set)
						allowed--;
					if (font_variant_set)
						allowed--;
					if (normal_count < allowed)
						normal_count++;
				}
				else if (equals(token.value, "italic") && !font_style_set) // font-style
				{
					font_style_set = true;
					style = StyleValue::from_keyword("italic");
				}
				else if (equals(token.value, "oblique") && !font_style_set) // font-style
				{
					font_style_set = true;
					style = StyleValue::from_keyword("oblique");
				}
				else if (equals(token.value, "small-caps") && !font_variant_set) // font-variant
				{
					font_style_set = true;
					variant = StyleValue::from_keyword("small-caps");
				}
				else if (equals(token.value, "bold") && !font_weight_set) // font-weight
				{
					font_weight_set = true;
					weight = StyleValue::from_keyword("bold");
				}
				else if (equals(token.value, "bolder") && !font_weight_set) // font-weight
				{
					font_weight_set = true;
					weight = StyleValue::from_keyword("bolder");
				}
				else if (equals(token.value, "lighter") && !font_weight_set) // font-weight
				{
					font_weight_set = true;
					weight = StyleValue::from_keyword("lighter");
				}
				else if (token.value == "100" && !font_weight_set) // font-weight
				{
					font_weight_set = true;
					weight = StyleValue::from_number(100.0f);
				}
				else if (token.value == "200" && !font_weight_set) // font-weight
				{
					font_weight_set = true;
					weight = StyleValue::from_number(200.0f);
				}
				else if (token.value == "300" && !font_weight_set) // font-weight
				{
					font_weight_set = true;
					weight = StyleValue::from_number(300.0f);
				}
				else if (token.value == "400" && !font_weight_set) // font-weight
				{
					font_weight_set = true;
					weight = StyleValue::from_number(400.0f);
				}
				else if (token.value == "500" && !font_weight_set) // font-weight
				{
					font_weight_set = true;
					weight = StyleValue::from_number(500.0f);
				}
				else if (token.value == "600" && !font_weight_set) // font-weight
				{
					font_weight_set = true;
					weight = StyleValue::from_number(600.0f);
				}
				else if (token.value == "700" && !font_weight_set) // font-weight
				{
					font_weight_set = true;
					weight = StyleValue::from_number(700.0f);
				}
				else if (token.value == "800" && !font_weight_set) // font-weight
				{
					font_weight_set = true;
					weight = StyleValue::from_number(800.0f);
				}
				else if (token.value == "900" && !font_weight_set) // font-weight
				{
					font_weight_set = true;
					weight = StyleValue::from_number(900.0f);
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}

		if (pos == tokens.size())
		{
			debug_parse_error(propname, tokens);
			return;
		}

		if (token.type == StyleTokenType::ident)
		{
			if (equals(token.value, "xx-small"))
				size = StyleValue::from_keyword("xx-small");
			else if (equals(token.value, "x-small"))
				size = StyleValue::from_keyword("x-small");
			else if (equals(token.value, "small"))
				size = StyleValue::from_keyword("small");
			else if (equals(token.value, "medium"))
				size = StyleValue::from_keyword("medium");
			else if (equals(token.value, "large"))
				size = StyleValue::from_keyword("large");
			else if (equals(token.value, "x-large"))
				size = StyleValue::from_keyword("x-large");
			else if (equals(token.value, "xx-large"))
				size = StyleValue::from_keyword("xx-large");
			else if (equals(token.value, "smaller"))
				size = StyleValue::from_keyword("smaller");
			else if (equals(token.value, "larger"))
				size = StyleValue::from_keyword("larger");
			else if (equals(token.value, "inherit"))
				size = StyleValue::from_keyword("inherit");
			else
			{
				debug_parse_error(propname, tokens);
				return;
			}
		}
		else if (is_length(token))
		{
			StyleValue length;
			if (parse_length(token, length))
			{
				size = length;
			}
			else
			{
				debug_parse_error(propname, tokens);
				return;
			}
		}
		else if (token.type == StyleTokenType::percentage)
		{
			size = StyleValue::from_percentage(StringHelp::text_to_float(token.value));
		}
		else
		{
			debug_parse_error(propname, tokens);
			return;
		}

		token = next_token(pos, tokens);
		if (token.type == StyleTokenType::delim && token.value == "/")
		{
			token = next_token(pos, tokens);

			if (token.type == StyleTokenType::ident)
			{
				if (equals(token.value, "normal"))
					line_height = StyleValue::from_keyword("normal");
				else if (equals(token.value, "inherit"))
					line_height = StyleValue::from_keyword("inherit");
				else
				{
					debug_parse_error(propname, tokens);
					return;
				}
			}
			else if (token.type == StyleTokenType::number)
			{
				line_height = StyleValue::from_number(StringHelp::text_to_float(token.value));
			}
			else if (is_length(token))
			{
				StyleValue length;
				if (parse_length(token, length))
				{
					line_height = length;
				}
				else
				{
					debug_parse_error(propname, tokens);
					return;
				}
			}
			else if (token.type == StyleTokenType::percentage)
			{
				line_height = StyleValue::from_percentage(StringHelp::text_to_float(token.value));
			}
			else
			{
				debug_parse_error(propname, tokens);
				return;
			}

			token = next_token(pos, tokens);
		}

		family_names.clear();
		while (true)
		{
			if (token.type == StyleTokenType::ident)
			{
				StyleValue name;
				bool parse_name = false;
				if (equals(token.value, "serif"))
				{
					name = StyleValue::from_keyword("serif");
				}
				else if (equals(token.value, "sans-serif"))
				{
					name = StyleValue::from_keyword("sans-serif");
				}
				else if (equals(token.value, "cursive"))
				{
					name = StyleValue::from_keyword("cursive");
				}
				else if (equals(token.value, "fantasy"))
				{
					name = StyleValue::from_keyword("fantasy");
				}
				else if (equals(token.value, "monospace"))
				{
					name = StyleValue::from_keyword("monospace");
				}
				else if (equals(token.value, "default"))
				{
					// reserved for future use
					return;
				}
				else if (equals(token.value, "initial"))
				{
					// reserved for future use
					return;
				}
				else
				{
					parse_name = true;
				}

				if (parse_name)
				{
					name = StyleValue::from_string(token.value);
					while (pos != tokens.size())
					{
						token = tokens[pos++];
						if (token.type == StyleTokenType::whitespace)
						{
							name.text += " ";
						}
						else if (token.type == StyleTokenType::ident)
						{
							name.text += token.value;
						}
						else if (token.type == StyleTokenType::delim && token.value == ",")
						{
							break;
						}
					}
					family_names.push_back(name);
					if (pos == tokens.size())
						break;
					token = next_token(pos, tokens);
				}
				else
				{
					family_names.push_back(name);

					if (pos == tokens.size())
						break;
					token = next_token(pos, tokens);
					if (token.type != StyleTokenType::delim || token.value != ",")
					{
						debug_parse_error(propname, tokens);
						return;
					}
					token = next_token(pos, tokens);
				}
			}
			else if (token.type == StyleTokenType::string)
			{
				StyleValue name = StyleValue::from_string(token.value);
				family_names.push_back(name);

				if (pos == tokens.size())
					break;
				token = next_token(pos, tokens);
				if (token.type != StyleTokenType::delim || token.value != ",")
				{
					debug_parse_error(propname, tokens);
					return;
				}
				token = next_token(pos, tokens);
			}
			else
			{
				debug_parse_error(propname, tokens);
				return;
			}
		}

		setter->set_value("font-style", style);
		setter->set_value("font-variant", variant);
		setter->set_value("font-weight", weight);
		setter->set_value("font-size", size);
		setter->set_value("line-height", line_height);
		setter->set_value("font-family", family);
		setter->set_value_array("font-family-names", family_names);
	}

	void FontFamilyPropertyParser::parse(StylePropertySetter *setter, const std::string &propname, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		std::vector<StyleToken> tokens = StyleTokenizer::tokenize(value);

		StyleValue family = StyleValue::from_keyword("array");
		std::vector<StyleValue> family_names;

		size_t pos = 0;
		StyleToken token;
		token = next_token(pos, tokens);

		if (equals(token.value, "inherit") && tokens.size() == 1)
		{
			family = StyleValue::from_keyword("inherit");
			setter->set_value("font-family", family);
			setter->set_value_array("font-family-names", family_names);
			return;
		}

		while (true)
		{
			if (token.type != StyleTokenType::ident && token.type != StyleTokenType::string)
			{
				debug_parse_error(propname, tokens);
				return;
			}

			StyleValue name;
			bool parse_name = false;
			if (equals(token.value, "serif"))
			{
				name = StyleValue::from_keyword("serif");
			}
			else if (equals(token.value, "sans-serif"))
			{
				name = StyleValue::from_keyword("sans-serif");
			}
			else if (equals(token.value, "cursive"))
			{
				name = StyleValue::from_keyword("cursive");
			}
			else if (equals(token.value, "fantasy"))
			{
				name = StyleValue::from_keyword("fantasy");
			}
			else if (equals(token.value, "monospace"))
			{
				name = StyleValue::from_keyword("monospace");
			}
			else if (equals(token.value, "default"))
			{
				// reserved for future use
				return;
			}
			else if (equals(token.value, "initial"))
			{
				// reserved for future use
				return;
			}
			else
			{
				parse_name = true;
			}

			if (parse_name)
			{
				name = StyleValue::from_string(token.value);
				while (pos != tokens.size())
				{
					token = tokens[pos++];
					if (token.type == StyleTokenType::whitespace)
					{
						name.text += " ";
					}
					else if (token.type == StyleTokenType::ident)
					{
						name.text += token.value;
					}
					else if (token.type == StyleTokenType::delim && token.value == ",")
					{
						break;
					}
				}
				family_names.push_back(name);
				if (pos == tokens.size())
					break;
				token = next_token(pos, tokens);
			}
			else
			{
				family_names.push_back(name);

				if (pos == tokens.size())
					break;
				token = next_token(pos, tokens);
				if (token.type != StyleTokenType::delim || token.value != ",")
				{
					debug_parse_error(propname, tokens);
					return;
				}
				token = next_token(pos, tokens);
			}
		}

		setter->set_value("font-family", family);
		setter->set_value_array("font-family-names", family_names);
	}

	void FontSizePropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		std::vector<StyleToken> tokens = StyleTokenizer::tokenize(value);

		StyleValue font_size;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "xx-small"))
				font_size = StyleValue::from_keyword("xx-small");
			else if (equals(token.value, "x-small"))
				font_size = StyleValue::from_keyword("x-small");
			else if (equals(token.value, "small"))
				font_size = StyleValue::from_keyword("small");
			else if (equals(token.value, "medium"))
				font_size = StyleValue::from_keyword("medium");
			else if (equals(token.value, "large"))
				font_size = StyleValue::from_keyword("large");
			else if (equals(token.value, "x-large"))
				font_size = StyleValue::from_keyword("x-large");
			else if (equals(token.value, "xx-large"))
				font_size = StyleValue::from_keyword("xx-large");
			else if (equals(token.value, "smaller"))
				font_size = StyleValue::from_keyword("smaller");
			else if (equals(token.value, "larger"))
				font_size = StyleValue::from_keyword("larger");
			else if (equals(token.value, "inherit"))
				font_size = StyleValue::from_keyword("inherit");
			else
				return;
		}
		else if (is_length(token) && pos == tokens.size())
		{
			StyleValue length;
			if (parse_length(token, length))
			{
				font_size = length;
			}
			else
			{
				return;
			}
		}
		else if (token.type == StyleTokenType::percentage && pos == tokens.size())
		{
			font_size = StyleValue::from_percentage(StringHelp::text_to_float(token.value));
		}
		else
		{
			return;
		}

		setter->set_value("font-size", font_size);
	}

	void LineHeightPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		std::vector<StyleToken> tokens = StyleTokenizer::tokenize(value);

		StyleValue line_height;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "normal"))
				line_height = StyleValue::from_keyword("normal");
			else if (equals(token.value, "inherit"))
				line_height = StyleValue::from_keyword("inherit");
			else
				return;
		}
		else if (token.type == StyleTokenType::number && pos == tokens.size())
		{
			line_height = StyleValue::from_number(StringHelp::text_to_float(token.value));
		}
		else if (is_length(token) && pos == tokens.size())
		{
			StyleValue length;
			if (parse_length(token, length))
			{
				line_height = length;
			}
			else
			{
				return;
			}
		}
		else if (token.type == StyleTokenType::percentage && pos == tokens.size())
		{
			line_height = StyleValue::from_percentage(StringHelp::text_to_float(token.value));
		}
		else
		{
			return;
		}

		setter->set_value("line-height", line_height);
	}

	void FontStylePropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		std::vector<StyleToken> tokens = StyleTokenizer::tokenize(value);

		StyleValue font_style;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "normal"))
				font_style = StyleValue::from_keyword("normal");
			else if (equals(token.value, "italic"))
				font_style = StyleValue::from_keyword("italic");
			else if (equals(token.value, "oblique"))
				font_style = StyleValue::from_keyword("oblique");
			else if (equals(token.value, "inherit"))
				font_style = StyleValue::from_keyword("inherit");
			else
				return;
		}
		else
		{
			return;
		}

		setter->set_value("font-style", font_style);
	}

	void FontVariantPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		std::vector<StyleToken> tokens = StyleTokenizer::tokenize(value);

		StyleValue font_variant;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "normal"))
				font_variant = StyleValue::from_keyword("normal");
			else if (equals(token.value, "small-caps"))
				font_variant = StyleValue::from_keyword("small-caps");
			else if (equals(token.value, "inherit"))
				font_variant = StyleValue::from_keyword("inherit");
			else
				return;
		}
		else
		{
			return;
		}

		setter->set_value("font-variant", font_variant);
	}

	void FontWeightPropertyParser::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		std::vector<StyleToken> tokens = StyleTokenizer::tokenize(value);

		StyleValue font_weight;

		size_t pos = 0;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident && pos == tokens.size())
		{
			if (equals(token.value, "normal"))
				font_weight = StyleValue::from_keyword("normal");
			else if (equals(token.value, "bold"))
				font_weight = StyleValue::from_keyword("bold");
			else if (equals(token.value, "bolder"))
				font_weight = StyleValue::from_keyword("bolder");
			else if (equals(token.value, "lighter"))
				font_weight = StyleValue::from_keyword("lighter");
			else if (equals(token.value, "inherit"))
				font_weight = StyleValue::from_keyword("inherit");
			else
				return;
		}
		else if (token.type == StyleTokenType::number && pos == tokens.size())
		{
			if (token.value == "100")
				font_weight = StyleValue::from_number(100.0f);
			else if (token.value == "200")
				font_weight = StyleValue::from_number(200.0f);
			else if (token.value == "300")
				font_weight = StyleValue::from_number(300.0f);
			else if (token.value == "400")
				font_weight = StyleValue::from_number(400.0f);
			else if (token.value == "500")
				font_weight = StyleValue::from_number(500.0f);
			else if (token.value == "600")
				font_weight = StyleValue::from_number(600.0f);
			else if (token.value == "700")
				font_weight = StyleValue::from_number(700.0f);
			else if (token.value == "800")
				font_weight = StyleValue::from_number(800.0f);
			else if (token.value == "900")
				font_weight = StyleValue::from_number(900.0f);
			else
				return;
		}
		else
		{
			return;
		}

		setter->set_value("font-weight", font_weight);
	}
}
