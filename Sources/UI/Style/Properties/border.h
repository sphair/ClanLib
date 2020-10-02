/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
	void force_link_style_parser_border();

	class BorderPropertyParser : public StylePropertyParser
	{
	public:
		BorderPropertyParser() : StylePropertyParser({ "border" }) { }
		void parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser) override;
	};

	class BorderColorPropertyParser : public StylePropertyParser
	{
	public:
		BorderColorPropertyParser() : StylePropertyParser({ "border-color" }) { }
		void parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser) override;
	};

	class BorderStylePropertyParser : public StylePropertyParser
	{
	public:
		BorderStylePropertyParser() : StylePropertyParser({ "border-style" }) { }
		void parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser) override;
	};

	class BorderWidthPropertyParser : public StylePropertyParser
	{
	public:
		BorderWidthPropertyParser() : StylePropertyParser({ "border-width" }) { }
		void parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser) override;
	};

	class BorderLTRBPropertyParser : public StylePropertyParser
	{
	public:
		BorderLTRBPropertyParser() : StylePropertyParser({ "border-left", "border-top", "border-right", "border-bottom" }) { }
		void parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser) override;
	};

	class BorderLTRBColorPropertyParser : public StylePropertyParser
	{
	public:
		BorderLTRBColorPropertyParser() : StylePropertyParser({ "border-left-color", "border-top-color", "border-right-color", "border-bottom-color" }) { }
		void parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser) override;
	};

	class BorderLTRBStylePropertyParser : public StylePropertyParser
	{
	public:
		BorderLTRBStylePropertyParser() : StylePropertyParser({ "border-left-style", "border-top-style", "border-right-style", "border-bottom-style" }) { }
		void parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser) override;
	};

	class BorderLTRBWidthPropertyParser : public StylePropertyParser
	{
	public:
		BorderLTRBWidthPropertyParser() : StylePropertyParser({ "border-left-width", "border-top-width", "border-right-width", "border-bottom-width" }) { }
		void parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser) override;
	};

	class BorderRadiusPropertyParser : public StylePropertyParser
	{
	public:
		BorderRadiusPropertyParser() : StylePropertyParser({ "border-radius" }) { }
		void parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser) override;
	};

	class BorderRadiusCornerPropertyParser : public StylePropertyParser
	{
	public:
		BorderRadiusCornerPropertyParser() : StylePropertyParser({ "border-top-right-radius", "border-bottom-right-radius", "border-bottom-left-radius", "border-top-left-radius" }) { }
		void parse(StylePropertySetter *setter, const std::string &name, StyleParser &parser) override;
	};
}
