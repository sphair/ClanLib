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
#include "API/UI/Style/style.h"
#include "style_impl.h"

namespace clan
{
	void StyleImpl::set_value(const std::string &name, const StyleSetValue &value)
	{
		auto type_it = prop_type.find(name);
		if (type_it != prop_type.end() && type_it->second != value.type)
		{
			switch (type_it->second)
			{
			default:
			case StyleValueType::undefined:
				break;
			case StyleValueType::keyword:
			case StyleValueType::string:
			case StyleValueType::url:
				prop_text.erase(prop_text.find(name));
				break;
			case StyleValueType::length:
			case StyleValueType::angle:
			case StyleValueType::time:
			case StyleValueType::frequency:
			case StyleValueType::resolution:
				prop_dimension.erase(prop_dimension.find(name));
				prop_number.erase(prop_number.find(name));
				break;
			case StyleValueType::percentage:
			case StyleValueType::number:
				prop_number.erase(prop_number.find(name));
				break;
			case StyleValueType::color:
				prop_color.erase(prop_color.find(name));
				break;
			}
		}

		if (value.type != StyleValueType::undefined)
			prop_type[name] = value.type;
		else if (type_it != prop_type.end())
			prop_type.erase(type_it);

		switch (value.type)
		{
		default:
		case StyleValueType::undefined:
			break;
		case StyleValueType::keyword:
		case StyleValueType::string:
		case StyleValueType::url:
			prop_text[name] = value.text;
			break;
		case StyleValueType::length:
		case StyleValueType::angle:
		case StyleValueType::time:
		case StyleValueType::frequency:
		case StyleValueType::resolution:
			prop_dimension[name] = value.dimension;
			prop_number[name] = value.number;
			break;
		case StyleValueType::percentage:
		case StyleValueType::number:
			prop_number[name] = value.number;
			break;
		case StyleValueType::color:
			prop_color[name] = value.color;
			break;
		}
	}

	void StyleImpl::set_value_array(const std::string &name, const std::vector<StyleSetValue> &value_array)
	{
		for (size_t i = 0; i < value_array.size(); i++)
		{
			set_value(name + "[" + StringHelp::int_to_text(i) + "]", value_array[i]);
		}

		size_t i = value_array.size();
		while (true)
		{
			auto index_name = name + "[" + StringHelp::int_to_text(i) + "]";
			if (prop_type.find(index_name) == prop_type.end())
				break;
			set_value(index_name, StyleSetValue());
		}
	}
}
