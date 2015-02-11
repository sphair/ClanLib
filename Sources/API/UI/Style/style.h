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
#include <functional>
#include <initializer_list>
#include "../../Display/2D/color.h"

namespace clan
{
	class StyleImpl;
	class StyleProperty;
	class BoxGeometry;
	class Canvas;
	class Font;
	class Colorf;
	class ImageSource;
	class StylePropertyInitializerValue;

	enum class StyleValueType
	{
		undefined,
		keyword,
		length,
		percentage,
		number,
		string,
		url,
		color,
		image
	};

	enum class StyleDimension
	{
		px, // device independent pixel (96 dpi)
		em, // relative to font-size length property
		pt, // point, 1/72 inch
		mm, // millimeter
		cm, // centimeter
		in, // inch, 1in is equal to 2.54cm
		pc, // picas, 1pc is equal to 12pt
		ex  // x-height, 1ex is equal to 0.5em
	};

	class StyleValue
	{
	public:
		StyleValueType type = StyleValueType::undefined;
		std::string text;
		float number = 0.0f;
		StyleDimension dimension = StyleDimension::px;
		Colorf color;
		std::shared_ptr<ImageSource> image;

		bool is_undefined() const { return type == StyleValueType::undefined; }
		bool is_keyword() const { return type == StyleValueType::keyword; }
		bool is_keyword(const char *keyword) const { return is_keyword() && text == keyword; }
		bool is_keyword(const std::string &keyword) const { return is_keyword() && text == keyword; }
		bool is_length() const { return type == StyleValueType::length; }
		bool is_percentage() const { return type == StyleValueType::percentage; }
		bool is_number() const { return type == StyleValueType::number; }
		bool is_string() const { return type == StyleValueType::string; }
		bool is_url() const { return type == StyleValueType::url; }
		bool is_color() const { return type == StyleValueType::color; }
		bool is_image() const { return type == StyleValueType::image; }

		static StyleValue from_keyword(const std::string &keyword) { StyleValue v; v.type = StyleValueType::keyword; v.text = keyword; return v; }
		static StyleValue from_string(const std::string &text) { StyleValue v; v.type = StyleValueType::string; v.text = text; return v; }
		static StyleValue from_length(float length, StyleDimension dimension = StyleDimension::px) { StyleValue v; v.type = StyleValueType::length; v.number = length; v.dimension = dimension; return v; }
		static StyleValue from_percentage(float length) { StyleValue v; v.type = StyleValueType::percentage; v.number = length; return v; }
		static StyleValue from_number(float length) { StyleValue v; v.type = StyleValueType::number; v.number = length; return v; }
		static StyleValue from_url(const std::string &url) { StyleValue v; v.type = StyleValueType::url; v.text = url; return v; }
		static StyleValue from_color(const Colorf &color) { StyleValue v; v.type = StyleValueType::color; v.color = color; return v; }
		static StyleValue from_image(const std::shared_ptr<ImageSource> &image) { StyleValue v; v.type = StyleValueType::image; v.image = image; return v; }
	};

	class Style
	{
	public:
		Style();
		Style(const Style &) = delete;
		~Style();
		Style &operator=(const Style &) = delete;

		const std::shared_ptr<Style> &get_base();
		void set_base(const std::shared_ptr<Style> &base);

		void set(const std::string &property_name, const std::string &property_value, const std::initializer_list<StylePropertyInitializerValue> &args = std::initializer_list<StylePropertyInitializerValue>());
		bool has(const std::string &property_name) const;
		void remove(const std::string &property_name);

		StyleValue specified_value(const std::string &property_name) const;
		StyleValue computed_value(const std::string &property_name) const;

		StyleValue compute_length(const StyleValue &length) const;

		void render_background(Canvas &canvas, const BoxGeometry &geometry) const;
		void render_border(Canvas &canvas, const BoxGeometry &geometry) const;

		Font get_font(Canvas &canvas);

	private:
		std::unique_ptr<StyleImpl> impl;
	};
}
