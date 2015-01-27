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

	enum class StyleValueType
	{
		keyword,
		length,
		percentage,
		string,
		url,
		color,
		image
	};

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

		bool is(const std::string &property_name, StyleValueType value_type) const;
		bool is(const std::string &property_name, const std::string &keyword) const;

		StyleValueType value_type(const std::string &property_name) const;

		const std::string &keyword(const std::string &property_name) const;
		float length(const std::string &property_name) const;
		float percentage(const std::string &property_name) const;
		const std::string &string(const std::string &property_name) const;
		const std::string &url(const std::string &property_name) const;
		Colorf color(const std::string &property_name) const;
		const std::shared_ptr<ImageSource> &image(const std::string &property_name) const;

		void render_background(Canvas &canvas, const BoxGeometry &geometry) const;
		void render_border(Canvas &canvas, const BoxGeometry &geometry) const;

		Font get_font(Canvas &canvas);

	private:
		std::unique_ptr<StyleImpl> impl;
	};

	class StylePropertySetter
	{
	public:
		virtual ~StylePropertySetter() { }
		virtual void set_keyword(const std::string &name, const std::string &keyword) = 0;
		virtual void set_length(const std::string &name, float length) = 0;
		virtual void set_percentage(const std::string &name, float length) = 0;
		virtual void set_string(const std::string &name, const std::string &str) = 0;
		virtual void set_url(const std::string &name, const std::string &url) = 0;
		virtual void set_color(const std::string &name, const Colorf &color) = 0;
		virtual void set_image(const std::string &name, const std::shared_ptr<ImageSource> &image) = 0;
	};

	class StylePropertyParser
	{
	public:
		StylePropertyParser();
		virtual ~StylePropertyParser() { }
		virtual const std::vector<std::string> &property_names() = 0;
		virtual void parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args) = 0;
	};

	class StylePropertyDefault
	{
	public:
		StylePropertyDefault(const std::string &name, const std::string &value, StyleValueType type);
	};
}
