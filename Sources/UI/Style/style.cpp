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
#include "API/UI/View/view_geometry.h"
#include "API/UI/UIThread/ui_thread.h"
#include "API/Display/Font/font.h"
#include "API/Display/2D/canvas.h"
#include "style_background_renderer.h"
#include "style_border_image_renderer.h"
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

	const std::shared_ptr<Style> &Style::get_parent()
	{
		return impl->parent;
	}
	
	void Style::set_parent(const std::shared_ptr<Style> &parent)
	{
		impl->parent = parent;
	}
	
	const std::shared_ptr<Style> &Style::get_cascade_base()
	{
		return impl->cascade_base;
	}

	void Style::set_cascade_base(const std::shared_ptr<Style> &base)
	{
		impl->cascade_base = base;
	}

	void Style::set(const std::string &properties)
	{
		StyleProperty::parse(impl.get(), properties);
	}

	StyleValue Style::declared_value(const std::string &property_name) const
	{
		const auto it = impl->prop_type.find(property_name);
		if (it != impl->prop_type.end())
		{
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
				case StyleValueType::angle:
					return StyleValue::from_angle(impl->prop_number.find(property_name)->second, impl->prop_dimension.find(property_name)->second);
				case StyleValueType::time:
					return StyleValue::from_time(impl->prop_number.find(property_name)->second, impl->prop_dimension.find(property_name)->second);
				case StyleValueType::frequency:
					return StyleValue::from_frequency(impl->prop_number.find(property_name)->second, impl->prop_dimension.find(property_name)->second);
				case StyleValueType::resolution:
					return StyleValue::from_resolution(impl->prop_number.find(property_name)->second, impl->prop_dimension.find(property_name)->second);
				case StyleValueType::percentage:
					return StyleValue::from_percentage(impl->prop_number.find(property_name)->second);
				case StyleValueType::number:
					return StyleValue::from_number(impl->prop_number.find(property_name)->second);
				case StyleValueType::color:
					return StyleValue::from_color(impl->prop_color.find(property_name)->second);
			}
		}
		return StyleValue();
	}
	
	/////////////////////////////////////////////////////////////////////////
	
	StyleValue StyleCascade::cascade_value(const std::string &property_name) const
	{
		for (Style *style : cascade)
		{
			StyleValue value = style->declared_value(property_name);
			if (!value.is_undefined())
				return value;
		}
		return StyleValue();
	}

	StyleValue StyleCascade::specified_value(const std::string &property_name) const
	{
		StyleValue value = cascade_value(property_name);
		bool inherit = (value.is_undefined() && StyleProperty::is_inherited(property_name)) || value.is_keyword("inherit");
		if (inherit && parent)
		{
			return parent->computed_value(property_name);
		}
		else if (value.is_undefined() || value.is_keyword("initial") || value.is_keyword("inherit"))
		{
			return StyleProperty::default_value(property_name);
		}
		else
		{
			return value;
		}
	}

	StyleValue StyleCascade::computed_value(const std::string &property_name) const
	{
		// To do: pass on to property compute functions

		StyleValue specified = specified_value(property_name);
		switch (specified.type)
		{
		case StyleValueType::length:
			return compute_length(specified);
		case StyleValueType::angle:
			return compute_angle(specified);
		case StyleValueType::time:
			return compute_time(specified);
		case StyleValueType::frequency:
			return compute_frequency(specified);
		case StyleValueType::resolution:
			return compute_resolution(specified);
		default:
			return specified;
		}
	}

	StyleValue StyleCascade::compute_length(const StyleValue &length) const
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

	StyleValue StyleCascade::compute_angle(const StyleValue &angle) const
	{
		switch (angle.dimension)
		{
		default:
		case StyleDimension::rad:
			return angle;
		case StyleDimension::deg:
			return StyleValue::from_angle(angle.number * PI / 180.0f);
		case StyleDimension::grad:
			return StyleValue::from_angle(angle.number * PI / 400.0f);
		case StyleDimension::turn:
			return StyleValue::from_angle(angle.number * PI / 2.0f);
		}
	}

	StyleValue StyleCascade::compute_time(const StyleValue &time) const
	{
		switch (time.dimension)
		{
		default:
		case StyleDimension::s:
			return time;
		case StyleDimension::ms:
			return StyleValue::from_time(time.number / 1000.0f);
		}
	}

	StyleValue StyleCascade::compute_frequency(const StyleValue &frequency) const
	{
		switch (frequency.dimension)
		{
		default:
		case StyleDimension::hz:
			return frequency;
		case StyleDimension::khz:
			return StyleValue::from_frequency(frequency.number * 1000.0f);
		}
	}

	StyleValue StyleCascade::compute_resolution(const StyleValue &resolution) const
	{
		switch (resolution.dimension)
		{
		default:
		case StyleDimension::dppx:
			return resolution;
		case StyleDimension::dpi:
			return StyleValue::from_resolution(resolution.number / 96.0f);
		case StyleDimension::dpcm:
			return StyleValue::from_resolution(resolution.number / (float)(96.0 / 2.54));
		}
	}

	void StyleCascade::render_background(Canvas &canvas, const ViewGeometry &geometry) const
	{
		StyleBackgroundRenderer renderer(canvas, geometry, *this);
		renderer.render_background();
	}

	void StyleCascade::render_border(Canvas &canvas, const ViewGeometry &geometry) const
	{
		StyleBackgroundRenderer renderer(canvas, geometry, *this);
		renderer.render_border();

		StyleBorderImageRenderer image_renderer(canvas, geometry, *this);
		image_renderer.render();
	}

	Font StyleCascade::get_font(Canvas &canvas) const
	{
		auto font_size = computed_value("font-size");
		auto line_height = computed_value("line-height");
		auto font_weight = computed_value("font-weight");
		auto font_style = computed_value("font-style");
		//auto font_variant = computed_value("font-variant"); // To do: needs FontDescription support
		auto font_rendering = computed_value("-clan-font-rendering");
		auto font_family_name = computed_value("font-family-names[0]");

		FontDescription font_desc;
		font_desc.set_height(font_size.number);

		if (line_height.is_length())
			font_desc.set_line_height(line_height.number);
		else if (line_height.is_number())
			font_desc.set_line_height(line_height.number * font_size.number);

		if (font_weight.is_keyword("normal"))
			font_desc.set_weight(FontWeight::normal);
		else if (font_weight.is_keyword("bold") || font_weight.is_keyword("bolder"))
			font_desc.set_weight(FontWeight::bold);
		else if (font_weight.is_keyword("lighter"))
			font_desc.set_weight(FontWeight::light);
		else if (font_weight.is_number())
			font_desc.set_weight((FontWeight)(int)font_weight.number);

		if (font_style.is_keyword("normal"))
			font_desc.set_style(FontStyle::normal);
		else if (font_style.is_keyword("italic"))
			font_desc.set_style(FontStyle::italic);
		else if (font_style.is_keyword("oblique"))
			font_desc.set_style(FontStyle::oblique);

		font_desc.set_subpixel(!font_rendering.is_keyword("anti-alias"));

		std::string family;
		if (font_family_name.is_string())
			family = font_family_name.text;
		else if (font_family_name.is_keyword())
			family = font_family_name.text;
		else
			family = "sans-serif";

		return Font::resource(canvas, family, font_desc, UIThread::get_resources());
	}

	int StyleCascade::array_size(const std::string &property_name) const
	{
		int size = 0;
		while (true)
		{
			std::string prop_name = property_name + "[" + StringHelp::int_to_text(size) + "]";
			if (specified_value(prop_name).is_undefined())
				break;
			size++;
		}
		return size;
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

	void StyleImpl::set_value_array(const std::string &name, const std::vector<StyleValue> &value_array)
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
			set_value(index_name, StyleValue());
		}
	}
}
