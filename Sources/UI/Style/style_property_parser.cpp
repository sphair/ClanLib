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
