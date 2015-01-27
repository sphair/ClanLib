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

namespace clan
{
	class ImageSource;

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
		StylePropertyParser(const std::vector<std::string> &property_names);
		virtual ~StylePropertyParser() { }
		virtual void parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args) = 0;
	};

	class StylePropertyDefault
	{
	public:
		StylePropertyDefault(const std::string &name, const std::string &value);
	};

	class StyleProperty
	{
	public:
		static std::string default_value(const std::string &name);
		static void parse(StylePropertySetter *setter, const std::string &name, const std::string &value, const std::initializer_list<StylePropertyInitializerValue> &args);
	};
}
