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

#pragma once

#include "API/UI/Style/style_property_parser.h"

namespace clan
{
	void force_link_style_parser_border_image();

	class BorderImagePropertyParser : public StylePropertyParser
	{
	public:
		BorderImagePropertyParser() : StylePropertyParser({ "border-image" }) { }
		void parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser) override;

	private:
		bool parse_source(StyleSetValue &border_image_source, size_t &parse_pos, const std::vector<StyleToken> &tokens);
		bool parse_slice(StyleSetValue *border_image_slice, size_t &parse_pos, const std::vector<StyleToken> &tokens);
		bool parse_width(StyleSetValue *border_image_width, size_t &parse_pos, const std::vector<StyleToken> &tokens);
		bool parse_outset(StyleSetValue *border_image_outset, size_t &parse_pos, const std::vector<StyleToken> &tokens);
		bool parse_repeat(StyleSetValue *border_image_repeat, size_t &parse_pos, const std::vector<StyleToken> &tokens);
	};

	class BorderImageOutsetPropertyParser : public StylePropertyParser
	{
	public:
		BorderImageOutsetPropertyParser() : StylePropertyParser({ "border-image-outset" }) { }
		void parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser) override;
	};

	class BorderImageRepeatPropertyParser : public StylePropertyParser
	{
	public:
		BorderImageRepeatPropertyParser() : StylePropertyParser({ "border-image-repeat" }) { }
		void parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser) override;
	};

	class BorderImageSlicePropertyParser : public StylePropertyParser
	{
	public:
		BorderImageSlicePropertyParser() : StylePropertyParser({ "border-image-slice" }) { }
		void parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser) override;
	};

	class BorderImageSourcePropertyParser : public StylePropertyParser
	{
	public:
		BorderImageSourcePropertyParser() : StylePropertyParser({ "border-image-source" }) { }
		void parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser) override;
	};

	class BorderImageWidthPropertyParser : public StylePropertyParser
	{
	public:
		BorderImageWidthPropertyParser() : StylePropertyParser({ "border-image-width" }) { }
		void parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser) override;
	};
}
