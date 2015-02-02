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
#include "API/Display/Font/font.h"
#include "style_impl.h"
#include "Properties/background.h"
#include "Properties/border.h"
#include "Properties/border_image.h"
#include "Properties/box_shadow.h"
#include "Properties/content.h"
#include "Properties/flex.h"
#include "Properties/layout.h"
#include "Properties/margin.h"
#include "Properties/outline.h"
#include "Properties/padding.h"
#include "Properties/text.h"

namespace clan
{
	class StyleForceLink
	{
	public:
		StyleForceLink()
		{
			force_link_style_parser_background();
			force_link_style_parser_border();
			force_link_style_parser_border_image();
			force_link_style_parser_box_shadow();
			force_link_style_parser_content();
			force_link_style_parser_flex();
			force_link_style_parser_layout();
			force_link_style_parser_margin();
			force_link_style_parser_outline();
			force_link_style_parser_padding();
			force_link_style_parser_text();
		}
	} style_force_link;

	Style::Style() : impl(new StyleImpl())
	{
	}

	Style::~Style()
	{
	}

	const std::shared_ptr<Style> &Style::get_base()
	{
		return impl->base;
	}

	void Style::set_base(const std::shared_ptr<Style> &new_base)
	{
		impl->base = new_base;
	}

	void Style::set(const std::string &property_name, const std::string &property_value, const std::initializer_list<StylePropertyInitializerValue> &args)
	{
		StyleProperty::parse(impl.get(), property_name, property_value, args);
	}

	bool Style::has(const std::string &property_name) const
	{
		const auto it = impl->prop_type.find(property_name);
		return it != impl->prop_type.end();
	}

	void Style::remove(const std::string &property_name)
	{
		impl->set_value(property_name, StyleValue());
	}

	StyleValue Style::specified_value(const std::string &property_name) const
	{
		const auto it = impl->prop_type.find(property_name);
		if (it == impl->prop_type.end())
			return StyleValue();

		switch (it->second)
		{
		default:
		case StyleValueType::undefined:
			return StyleValue();
		case StyleValueType::keyword:
			return StyleValue::from_keyword(impl->prop_text.find(property_name)->second);
		case StyleValueType::string:
			return StyleValue::from_string(impl->prop_text.find(property_name)->second);
		case StyleValueType::url:
			return StyleValue::from_url(impl->prop_text.find(property_name)->second);
		case StyleValueType::length:
			return StyleValue::from_length(impl->prop_number.find(property_name)->second, impl->prop_dimension.find(property_name)->second);
		case StyleValueType::percentage:
			return StyleValue::from_percentage(impl->prop_number.find(property_name)->second);
		case StyleValueType::number:
			return StyleValue::from_percentage(impl->prop_number.find(property_name)->second);
		case StyleValueType::color:
			return StyleValue::from_color(impl->prop_color.find(property_name)->second);
		case StyleValueType::image:
			return StyleValue::from_image(impl->prop_image.find(property_name)->second);
		}
	}

	StyleValue Style::computed_value(const std::string &property_name) const
	{
		// To do: pass on to property compute functions

		StyleValue specified = specified_value(property_name);
		if (specified.is_length())
		{
			return compute_length(specified);
		}
		else
		{
			return specified;
		}
	}

	StyleValue Style::compute_length(const StyleValue &length) const
	{
		switch (length.dimension)
		{
		default:
		case StyleDimension::px:
			return length;
		case StyleDimension::pt:
			return StyleValue::from_length(length.number * (float)(96.0 / 72.0));
		case StyleDimension::mm:
			return StyleValue::from_length(length.number * (float)(96.0 / 25.4));
		case StyleDimension::cm:
			return StyleValue::from_length(length.number * (float)(96.0 / 2.54));
		case StyleDimension::in:
			return StyleValue::from_length(length.number * 96.0f);
		case StyleDimension::pc:
			return StyleValue::from_length(length.number * (float)(12.0 * 96.0 / 72.0));
		case StyleDimension::em:
			return StyleValue::from_length(computed_value("font-size").number * length.number);
		case StyleDimension::ex:
			return StyleValue::from_length(computed_value("font-size").number * length.number * 0.5f);
		}
	}

	void Style::render_background(Canvas &canvas, const BoxGeometry &geometry) const
	{
	}

	void Style::render_border(Canvas &canvas, const BoxGeometry &geometry) const
	{
	}

	Font Style::get_font(Canvas &canvas)
	{
		return Font();
	}

	/////////////////////////////////////////////////////////////////////////

	void StyleImpl::set_value(const std::string &name, const StyleValue &value)
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
				prop_dimension.erase(prop_dimension.find(name));
				// intentional fall through
			case StyleValueType::percentage:
			case StyleValueType::number:
				prop_number.erase(prop_number.find(name));
				break;
			case StyleValueType::color:
				prop_color.erase(prop_color.find(name));
				break;
			case StyleValueType::image:
				prop_image.erase(prop_image.find(name));
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
			prop_dimension[name] = value.dimension;
			// intentional fall through
		case StyleValueType::percentage:
		case StyleValueType::number:
			prop_number[name] = value.number;
			break;
		case StyleValueType::color:
			prop_color[name] = value.color;
			break;
		case StyleValueType::image:
			prop_image[name] = value.image;
			break;
		}
	}

	void StyleImpl::set_value_array(const std::string &name, const std::vector<StyleValue> &value_array)
	{
	}
}
