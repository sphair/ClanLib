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
#include "API/UI/Style/style_property_parser.h"
#include "API/UI/Style/style.h"
#include "API/UI/Style/style_token.h"

namespace clan
{
	std::map<std::string, std::string> &style_defaults()
	{
		static std::map<std::string, std::string> defaults;
		return defaults;
	}

	std::map<std::string, StylePropertyParser *> &style_parsers()
	{
		static std::map<std::string, StylePropertyParser *> parsers;
		return parsers;
	}

	/////////////////////////////////////////////////////////////////////////

	StylePropertyDefault::StylePropertyDefault(const std::string &name, const std::string &value)
	{
		style_defaults()[name] = value;
	}

	/////////////////////////////////////////////////////////////////////////

	StylePropertyParser::StylePropertyParser(const std::vector<std::string> &property_names)
	{
		auto &parsers = style_parsers();
		for (const auto &name : property_names)
			parsers[name] = this;
	}

	StyleToken StylePropertyParser::next_token(size_t &pos, const std::vector<StyleToken> &tokens, bool skip_whitespace)
	{
		StyleToken token;
		do
		{
			if (pos != tokens.size())
			{
				token = tokens[pos];
				pos++;
			}
			else
			{
				token = StyleToken();
			}
		} while (token.type == StyleTokenType::whitespace);
		return token;
	}

	bool StylePropertyParser::is_length(const StyleToken &token)
	{
		if (token.type == StyleTokenType::dimension)
			return true;
		else if (token.type == StyleTokenType::number && token.value == "0")
			return true;
		else
			return false;
	}

	bool StylePropertyParser::parse_length(const StyleToken &token, StyleValue &out_length)
	{
		if (token.type == StyleTokenType::dimension)
		{
			if (equals(token.dimension, "px"))
			{
				out_length = StyleValue::from_length(StringHelp::text_to_float(token.value), StyleDimension::px);
				return true;
			}
			else if (equals(token.dimension, "em"))
			{
				out_length = StyleValue::from_length(StringHelp::text_to_float(token.value), StyleDimension::em);
				return true;
			}
			else if (equals(token.dimension, "pt"))
			{
				out_length = StyleValue::from_length(StringHelp::text_to_float(token.value), StyleDimension::pt);
				return true;
			}
			else if (equals(token.dimension, "mm"))
			{
				out_length = StyleValue::from_length(StringHelp::text_to_float(token.value), StyleDimension::mm);
				return true;
			}
			else if (equals(token.dimension, "cm"))
			{
				out_length = StyleValue::from_length(StringHelp::text_to_float(token.value), StyleDimension::cm);
				return true;
			}
			else if (equals(token.dimension, "in"))
			{
				out_length = StyleValue::from_length(StringHelp::text_to_float(token.value), StyleDimension::in);
				return true;
			}
			else if (equals(token.dimension, "pc"))
			{
				out_length = StyleValue::from_length(StringHelp::text_to_float(token.value), StyleDimension::pc);
				return true;
			}
			else if (equals(token.dimension, "ex"))
			{
				out_length = StyleValue::from_length(StringHelp::text_to_float(token.value), StyleDimension::ex);
				return true;
			}
			else
			{
				return false;
			}
		}
		else if (token.type == StyleTokenType::number && token.value == "0")
		{
			out_length = StyleValue::from_length(0.0f, StyleDimension::px);
			return true;
		}
		else
		{
			return false;
		}
	}

	bool StylePropertyParser::parse_integer(const std::string &value, int &out_int)
	{
		if (value.empty())
			return false;
		for (size_t i = 0; i < value.length(); i++)
		{
			if (value[i] < '0' || value[i] > '9')
				return false;
		}
		out_int = StringHelp::text_to_int(value);
		return true;
	}

	bool StylePropertyParser::parse_color(const std::vector<StyleToken> &tokens, size_t &in_out_pos, Colorf &out_color)
	{
		size_t pos = in_out_pos;
		StyleToken token = next_token(pos, tokens);
		if (token.type == StyleTokenType::ident)
		{
			if (equals(token.value, "transparent"))
			{
				out_color = Colorf::transparent;
				in_out_pos = pos;
				return true;
			}

			for (int i = 0; colors[i].name != 0; i++)
			{
				if (equals(token.value, colors[i].name))
				{
					out_color = Colorf(
						(colors[i].color >> 16) / 255.0f,
						((colors[i].color >> 8) & 0xff) / 255.0f,
						(colors[i].color & 0xff) / 255.0f,
						1.0f);

					in_out_pos = pos;
					return true;
				}
			}
		}
		else if (token.type == StyleTokenType::function && equals(token.value, "rgb"))
		{
			int color[3] = { 0, 0, 0 };
			for (int channel = 0; channel < 3; channel++)
			{
				token = next_token(pos, tokens);
				if (token.type == StyleTokenType::number)
				{
					int value = StringHelp::text_to_int(token.value);
					value = min(255, value);
					value = max(0, value);
					color[channel] = value;
				}
				else if (token.type == StyleTokenType::percentage)
				{
					float value = StringHelp::text_to_float(token.value) / 100.0f;
					value = min(1.0f, value);
					value = max(0.0f, value);
					color[channel] = (int)(value*255.0f);
				}
				else
				{
					return false;
				}

				if (channel < 2)
				{
					token = next_token(pos, tokens);
					if (token.type != StyleTokenType::delim || token.value != ",")
						return false;
				}
			}
			token = next_token(pos, tokens);
			if (token.type == StyleTokenType::bracket_end)
			{
				out_color = Colorf(color[0] / 255.0f, color[1] / 255.0f, color[2] / 255.0f, 1.0f);
				in_out_pos = pos;
				return true;
			}
		}
		else if (token.type == StyleTokenType::function && equals(token.value, "rgba"))
		{
			int color[4] = { 0, 0, 0, 0 };
			for (int channel = 0; channel < 4; channel++)
			{
				token = next_token(pos, tokens);
				if (token.type == StyleTokenType::number)
				{
					int value = StringHelp::text_to_int(token.value);
					value = min(255, value);
					value = max(0, value);
					color[channel] = value;
				}
				else if (token.type == StyleTokenType::percentage)
				{
					float value = StringHelp::text_to_float(token.value) / 100.0f;
					value = min(1.0f, value);
					value = max(0.0f, value);
					color[channel] = (int)(value*255.0f);
				}
				else
				{
					return false;
				}

				if (channel < 3)
				{
					token = next_token(pos, tokens);
					if (token.type != StyleTokenType::delim || token.value != ",")
						return false;
				}
			}
			token = next_token(pos, tokens);
			if (token.type == StyleTokenType::bracket_end)
			{
				out_color = Colorf(color[0] / 255.0f, color[1] / 255.0f, color[2] / 255.0f, color[3] / 255.0f);
				in_out_pos = pos;
				return true;
			}
		}
		else if (token.type == StyleTokenType::hash)
		{
			if (token.value.length() == 3)
			{
				float channels[3] = { 0.0f, 0.0f, 0.0f };
				for (int c = 0; c < 3; c++)
				{
					int v = 0;
					if (token.value[c] >= '0' && token.value[c] <= '9')
						v = token.value[c] - '0';
					else if (token.value[c] >= 'a' && token.value[c] <= 'f')
						v = token.value[c] - 'a' + 10;
					else if (token.value[c] >= 'A' && token.value[c] <= 'F')
						v = token.value[c] - 'A' + 10;
					else
						return false;
					v = (v << 4) + v;
					channels[c] = v / 255.0f;
				}
				out_color = Colorf(channels[0], channels[1], channels[2]);
				in_out_pos = pos;
				return true;
			}
			else if (token.value.length() == 4)
			{
				float channels[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
				for (int c = 0; c < 4; c++)
				{
					int v = 0;
					if (token.value[c] >= '0' && token.value[c] <= '9')
						v = token.value[c] - '0';
					else if (token.value[c] >= 'a' && token.value[c] <= 'f')
						v = token.value[c] - 'a' + 10;
					else if (token.value[c] >= 'A' && token.value[c] <= 'F')
						v = token.value[c] - 'A' + 10;
					else
						return false;
					v = (v << 4) + v;
					channels[c] = v / 255.0f;
				}
				out_color = Colorf(channels[0], channels[1], channels[2], channels[3]);
				in_out_pos = pos;
				return true;
			}
			else if (token.value.length() == 6)
			{
				float channels[3] = { 0.0f, 0.0f, 0.0f };
				for (int c = 0; c < 3; c++)
				{
					int v = 0;
					if (token.value[c * 2] >= '0' && token.value[c * 2] <= '9')
						v = token.value[c * 2] - '0';
					else if (token.value[c * 2] >= 'a' && token.value[c * 2] <= 'f')
						v = token.value[c * 2] - 'a' + 10;
					else if (token.value[c * 2] >= 'A' && token.value[c * 2] <= 'F')
						v = token.value[c * 2] - 'A' + 10;
					else
						return false;

					v <<= 4;

					if (token.value[c * 2 + 1] >= '0' && token.value[c * 2 + 1] <= '9')
						v += token.value[c * 2 + 1] - '0';
					else if (token.value[c * 2 + 1] >= 'a' && token.value[c * 2 + 1] <= 'f')
						v += token.value[c * 2 + 1] - 'a' + 10;
					else if (token.value[c * 2 + 1] >= 'A' && token.value[c * 2 + 1] <= 'F')
						v += token.value[c * 2 + 1] - 'A' + 10;
					else
						return false;

					channels[c] = v / 255.0f;
				}
				out_color = Colorf(channels[0], channels[1], channels[2]);
				in_out_pos = pos;
				return true;
			}
			else if (token.value.length() == 8)
			{
				float channels[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
				for (int c = 0; c < 4; c++)
				{
					int v = 0;
					if (token.value[c * 2] >= '0' && token.value[c * 2] <= '9')
						v = token.value[c * 2] - '0';
					else if (token.value[c * 2] >= 'a' && token.value[c * 2] <= 'f')
						v = token.value[c * 2] - 'a' + 10;
					else if (token.value[c * 2] >= 'A' && token.value[c * 2] <= 'F')
						v = token.value[c * 2] - 'A' + 10;
					else
						return false;

					v <<= 4;

					if (token.value[c * 2 + 1] >= '0' && token.value[c * 2 + 1] <= '9')
						v += token.value[c * 2 + 1] - '0';
					else if (token.value[c * 2 + 1] >= 'a' && token.value[c * 2 + 1] <= 'f')
						v += token.value[c * 2 + 1] - 'a' + 10;
					else if (token.value[c * 2 + 1] >= 'A' && token.value[c * 2 + 1] <= 'F')
						v += token.value[c * 2 + 1] - 'A' + 10;
					else
						return false;

					channels[c] = v / 255.0f;
				}
				out_color = Colorf(channels[0], channels[1], channels[2], channels[3]);
				in_out_pos = pos;
				return true;
			}
		}

		return false;
	}

	StylePropertyParser::ColorType StylePropertyParser::colors[] =
	{
		"maroon", 0x800000,
		"red", 0xff0000,
		"orange", 0xffa500,
		"yellow", 0xffff00,
		"olive", 0x808000,
		"purple", 0x800080,
		"fuchia", 0xff00ff,
		"white", 0xffffff,
		"lime", 0x00ff00,
		"green", 0x008000,
		"navy", 0x000080,
		"blue", 0x0000ff,
		"aqua", 0x00ffff,
		"teal", 0x008080,
		"black", 0x000000,
		"silver", 0xc0c0c0,
		"gray", 0x808080,
		0, 0
	};

	bool StylePropertyParser::equals(const std::string &s1, const std::string &s2)
	{
		return StringHelp::compare(s1, s2, true) == 0;
	}

	void StylePropertyParser::debug_parse_error(const std::string &name, const std::vector<StyleToken> &tokens)
	{
		std::string s = string_format("Parse error for %1:", name);
		for (size_t i = 0; i < tokens.size(); i++)
		{
			switch (tokens[i].type)
			{
			case StyleTokenType::null: s += " null"; break;
			case StyleTokenType::ident: s += string_format(" ident(%1)", tokens[i].value); break;
			case StyleTokenType::atkeyword: s += string_format(" atkeyword(%1)", tokens[i].value); break;
			case StyleTokenType::string: s += string_format(" string(%1)", tokens[i].value); break;
			case StyleTokenType::invalid: s += string_format(" invalid(%1)", tokens[i].value); break;
			case StyleTokenType::hash: s += string_format(" hash(%1)", tokens[i].value); break;
			case StyleTokenType::number: s += string_format(" number(%1)", tokens[i].value); break;
			case StyleTokenType::percentage: s += string_format(" percentage(%1)", tokens[i].value); break;
			case StyleTokenType::dimension: s += string_format(" dimension(%1,%2)", tokens[i].value, tokens[i].dimension); break;
			case StyleTokenType::uri: s += string_format(" uri(%1)", tokens[i].value); break;
			case StyleTokenType::unicode_range: s += string_format(" unicode-range(%1)", tokens[i].value); break;
			case StyleTokenType::cdo: s += " cdo"; break;
			case StyleTokenType::cdc: s += " cdc"; break;
			case StyleTokenType::colon: s += " :"; break;
			case StyleTokenType::semi_colon: s += " ;"; break;
			case StyleTokenType::curly_brace_begin: s += " {"; break;
			case StyleTokenType::curly_brace_end: s += " }"; break;
			case StyleTokenType::bracket_begin: s += " ("; break;
			case StyleTokenType::bracket_end: s += " )"; break;
			case StyleTokenType::square_bracket_begin: s += " ["; break;
			case StyleTokenType::square_bracket_end: s += " ]"; break;
			case StyleTokenType::whitespace: s += " whitespace"; break;
			case StyleTokenType::comment: s += " comment"; break;
			case StyleTokenType::function: s += string_format(" function(%1)", tokens[i].value); break;
			case StyleTokenType::includes: s += string_format(" includes(%1)", tokens[i].value); break;
			case StyleTokenType::dashmatch: s += string_format(" dashmatch(%1)", tokens[i].value); break;
			case StyleTokenType::delim: s += string_format(" delim(%1)", tokens[i].value); break;
			default: break;
			}
		}
		Console::write_line(s);
	}

	/////////////////////////////////////////////////////////////////////////

	std::string StyleProperty::default_value(const std::string &name)
	{
		auto it = style_defaults().find(name);
		if (it != style_defaults().end())
			return it->second;
		else
			return std::string();
	}

	void StyleProperty::parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		auto it = style_parsers().find(name);
		if (it != style_parsers().end())
			it->second->parse(setter, name, value, args);
	}
}
