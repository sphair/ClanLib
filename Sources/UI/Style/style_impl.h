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

#include "API/UI/Style/style_property_parser.h"
#include <memory>
#include <string>

namespace clan
{
	class Style;
	class ImageSource;
	class Colorf;

	class StyleImpl : public StylePropertySetter
	{
	public:
		void set_value(const std::string &name, const StyleValue &value) override;
		void set_value_array(const std::string &name, const std::vector<StyleValue> &value_array) override;

		std::shared_ptr<Style> parent;
		std::shared_ptr<Style> cascade_base;

		std::map<std::string, StyleValueType> prop_type;
		std::map<std::string, std::string> prop_text;
		std::map<std::string, float> prop_number;
		std::map<std::string, StyleDimension> prop_dimension;
		std::map<std::string, Colorf> prop_color;
	};
}
