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

#pragma once

#include <memory>
#include <string>
#include <vector>
#include <initializer_list>
#include "../../Display/2D/color.h"
#include "style.h"
#include "style_token.h"
#include "style_tokenizer.h"

namespace clan
{
	enum class StyleDimension;
	class StyleValue;
	class ImageSource;
	class StyleToken;

	class StylePropertyInitializerValue
	{
	public:
		enum class Type
		{
			string,
			number,
			color,
			image
		};

		StylePropertyInitializerValue(const std::string &str) : type(Type::string), str(str) { }
		StylePropertyInitializerValue(float number) : type(Type::number), number(number) { }
		StylePropertyInitializerValue(const Colorf &color) : type(Type::color), color(color) { }
		StylePropertyInitializerValue(const std::shared_ptr<ImageSource> &image) : type(Type::image), image(image) { }

		Type type;
		std::string str;
		float number = 0.0f;
		Colorf color;
		std::shared_ptr<ImageSource> image;
	};

	class StylePropertySetter
	{
	public:
		virtual ~StylePropertySetter() { }
		virtual void set_value(const std::string &name, const StyleValue &value) = 0;
		virtual void set_value_array(const std::string &name, const std::vector<StyleValue> &value_array) = 0;
	};

	class StylePropertyParser
	{
	public:
		StylePropertyParser(const std::vector<std::string> &property_names);
		virtual ~StylePropertyParser() { }
		virtual void parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args) = 0;

	protected:
		static StyleToken next_token(size_t &pos, const std::vector<StyleToken> &tokens, bool skip_whitespace = true);
		static bool is_length(const StyleToken &token);
		static bool is_angle(const StyleToken &token);
		static bool is_time(const StyleToken &token);
		static bool is_frequency(const StyleToken &token);
		static bool is_resolution(const StyleToken &token);
		static bool parse_length(const StyleToken &token, StyleValue &out_length);
		static bool parse_angle(const StyleToken &token, StyleValue &out_angle);
		static bool parse_time(const StyleToken &token, StyleValue &out_time);
		static bool parse_frequency(const StyleToken &token, StyleValue &out_frequency);
		static bool parse_resolution(const StyleToken &token, StyleValue &out_resolution);
		static bool parse_integer(const std::string &value, int &out_int);
		static bool parse_gradient(const std::vector<StyleToken> &tokens, size_t &in_out_pos, StyleGradient &out_gradient);
		static bool parse_color(const std::vector<StyleToken> &tokens, size_t &in_out_pos, Colorf &out_color);
		static bool parse_position(const std::vector<StyleToken> &tokens, size_t &in_out_pos, StyleValue &out_position_x, StyleValue &out_position_y);
		static bool equals(const std::string &s1, const std::string &s2);
		static void debug_parse_error(const std::string &name, const std::vector<StyleToken> &tokens);

	private:
		struct ColorType
		{
			const std::string::value_type *name;
			int color;
		};
		static ColorType colors[];
	};

	class StylePropertyDefault
	{
	public:
		StylePropertyDefault(const std::string &name, const StyleValue &value);
	};

	class StyleProperty
	{
	public:
		static const StyleValue &default_value(const std::string &name);
		static void parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args);
	};
}
