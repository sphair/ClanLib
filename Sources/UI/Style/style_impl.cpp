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
**    Mark Page
*/

#include "UI/precomp.h"
#include "API/UI/Style/style.h"
#include "API/Core/Text/string_help.h"
#include "style_impl.h"

namespace clan
{
	void StyleImpl::set_value(PropertyHash hash, const StyleSetValue& value)
	{
		auto it_prop_type = prop_type.begin();
		for (; it_prop_type != prop_type.end(); ++it_prop_type)
		{
			if (it_prop_type->first == hash)
			{
				it_prop_type->second = value.type;
				break;
			}
		}
		if (it_prop_type == prop_type.end())
		{
			if (value.type != StyleValueType::undefined)
				prop_type.emplace_back(hash, value.type);
		}

		switch (value.type)
		{
		default:
		case StyleValueType::undefined:
			break;
		case StyleValueType::keyword:
		case StyleValueType::string:
		case StyleValueType::url:
		{
			auto it_prop_text = prop_text.begin();
			for (; it_prop_text != prop_text.end(); ++it_prop_text)
			{
				if (it_prop_text->first == hash)
				{
					it_prop_text->second = value.text;
					break;
				}
			}
			if (it_prop_text == prop_text.end())
			{
				prop_text.emplace_back(hash, value.text);
			}
			break;
		}
		case StyleValueType::length:
		case StyleValueType::angle:
		case StyleValueType::time:
		case StyleValueType::frequency:
		case StyleValueType::resolution:
		{
			auto it_prop_dimension = prop_dimension.begin();
			for (; it_prop_dimension != prop_dimension.end(); ++it_prop_dimension)
			{
				if (it_prop_dimension->first == hash)
				{
					it_prop_dimension->second = value.dimension;
					break;
				}
			}
			if (it_prop_dimension == prop_dimension.end())
			{
				prop_dimension.emplace_back(hash, value.dimension);
			}

			auto it_prop_number = prop_number.begin();
			for (; it_prop_number != prop_number.end(); ++it_prop_number)
			{
				if (it_prop_number->first == hash)
				{
					it_prop_number->second = value.number;
					break;
				}
			}
			if (it_prop_number == prop_number.end())
			{
				prop_number.emplace_back(hash, value.number);
			}
			break;
		}
		case StyleValueType::percentage:
		case StyleValueType::number:
		{
			auto it_prop_number = prop_number.begin();
			for (; it_prop_number != prop_number.end(); ++it_prop_number)
			{
				if (it_prop_number->first == hash)
				{
					it_prop_number->second = value.number;
					break;
				}
			}
			if (it_prop_number == prop_number.end())
			{
				prop_number.emplace_back(hash, value.number);
			}
			break;
		}
		case StyleValueType::color:
		{
			auto it_prop_color = prop_color.begin();
			for (; it_prop_color != prop_color.end(); ++it_prop_color)
			{
				if (it_prop_color->first == hash)
				{
					it_prop_color->second = value.color;
					break;
				}
			}
			if (it_prop_color == prop_color.end())
			{
				prop_color.emplace_back(hash, value.color);
			}
			break;
		}
		}
	}

	void StyleImpl::set_value_array(PropertyHash hash, const std::vector<StyleSetValue>& value_array)
	{
		for (size_t i = 0; i < value_array.size(); i++)
		{
			PropertyHash hash_index = hash;
			hash_index.append_index((int)i);
			set_value(hash_index, value_array[i]);
		}

		PropertyHash hash_index = hash;
		hash_index.append_index((int)value_array.size());
		set_value(hash_index, StyleSetValue());
	}

	const char* StyleImpl::get_text(PropertyHash hash) const
	{
		for (const auto& item : prop_text)
		{
			if (item.first == hash)
			{
				return item.second.c_str();
			}
		}
		return "";
	}

	float StyleImpl::get_number(PropertyHash hash) const
	{
		for (const auto& item : prop_number)
		{
			if (item.first == hash)
			{
				return item.second;
			}
		}
		return 0.0f;

	}

	StyleDimension StyleImpl::get_dimension(PropertyHash hash) const
	{
		for (const auto& item : prop_dimension)
		{
			if (item.first == hash)
			{
				return item.second;
			}
		}
		return StyleDimension::px;
	}

	Colorf StyleImpl::get_color(PropertyHash hash) const
	{
		for (const auto& item : prop_color)
		{
			if (item.first == hash)
			{
				return item.second;
			}
		}
		return Colorf();
	}

	StyleGetValue StyleImpl::declared_value(PropertyHash hash) const
	{
		StyleValueType value_type = StyleValueType::undefined;

		for (const auto& item : prop_type)
		{
			if (item.first == hash)
			{
				value_type = item.second;
				break;
			}
		}

		switch (value_type)
		{
		default:
		case StyleValueType::undefined:
			return StyleGetValue();
		case StyleValueType::keyword:
			return StyleGetValue::from_keyword(get_text(hash));
		case StyleValueType::string:
			return StyleGetValue::from_string(get_text(hash));
		case StyleValueType::url:
			return StyleGetValue::from_url(get_text(hash));
		case StyleValueType::length:
			return StyleGetValue::from_length(get_number(hash), get_dimension(hash));
		case StyleValueType::angle:
			return StyleGetValue::from_angle(get_number(hash), get_dimension(hash));
		case StyleValueType::time:
			return StyleGetValue::from_time(get_number(hash), get_dimension(hash));
		case StyleValueType::frequency:
			return StyleGetValue::from_frequency(get_number(hash), get_dimension(hash));
		case StyleValueType::resolution:
			return StyleGetValue::from_resolution(get_number(hash), get_dimension(hash));
		case StyleValueType::percentage:
			return StyleGetValue::from_percentage(get_number(hash));
		case StyleValueType::number:
			return StyleGetValue::from_number(get_number(hash));
		case StyleValueType::color:
			return StyleGetValue::from_color(get_color(hash));
		}
	}

}
