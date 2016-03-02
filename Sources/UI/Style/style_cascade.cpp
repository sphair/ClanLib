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

#include "UI/precomp.h"
#include "API/UI/Style/style.h"
#include "API/UI/Style/style_cascade.h"
#include "API/UI/View/view_geometry.h"
#include "API/UI/UIThread/ui_thread.h"
#include "API/Display/Font/font.h"
#include "API/Display/2D/canvas.h"
#include "API/Core/Text/string_help.h"
#include "style_background_renderer.h"
#include "style_border_image_renderer.h"
#include "style_impl.h"

namespace clan
{
	StyleGetValue StyleCascade::cascade_value(const char *property_name) const
	{
		for (Style *style : cascade)
		{
			StyleGetValue value = style->declared_value(property_name);
			if (!value.is_undefined())
				return value;
		}
		return StyleGetValue();
	}

	StyleGetValue StyleCascade::specified_value(const char *property_name) const
	{
		StyleGetValue value = cascade_value(property_name);
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

	StyleGetValue StyleCascade::computed_value(const char *property_name) const
	{
		// To do: pass on to property compute functions

		StyleGetValue specified = specified_value(property_name);
		switch (specified.type())
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

	StyleGetValue StyleCascade::compute_length(const StyleGetValue &length) const
	{
		switch (length.dimension())
		{
		default:
		case StyleDimension::px:
			return length;
		case StyleDimension::pt:
			return StyleGetValue::from_length(length.number() * (float)(96.0 / 72.0));
		case StyleDimension::mm:
			return StyleGetValue::from_length(length.number() * (float)(96.0 / 25.4));
		case StyleDimension::cm:
			return StyleGetValue::from_length(length.number() * (float)(96.0 / 2.54));
		case StyleDimension::in:
			return StyleGetValue::from_length(length.number() * 96.0f);
		case StyleDimension::pc:
			return StyleGetValue::from_length(length.number() * (float)(12.0 * 96.0 / 72.0));
		case StyleDimension::em:
			return StyleGetValue::from_length(computed_value("font-size").number() * length.number());
		case StyleDimension::ex:
			return StyleGetValue::from_length(computed_value("font-size").number() * length.number() * 0.5f);
		}
	}

	StyleGetValue StyleCascade::compute_angle(const StyleGetValue &angle) const
	{
		switch (angle.dimension())
		{
		default:
		case StyleDimension::rad:
			return angle;
		case StyleDimension::deg:
			return StyleGetValue::from_angle(angle.number() * PI / 180.0f);
		case StyleDimension::grad:
			return StyleGetValue::from_angle(angle.number() * PI / 400.0f);
		case StyleDimension::turn:
			return StyleGetValue::from_angle(angle.number() * PI / 2.0f);
		}
	}

	StyleGetValue StyleCascade::compute_time(const StyleGetValue &time) const
	{
		switch (time.dimension())
		{
		default:
		case StyleDimension::s:
			return time;
		case StyleDimension::ms:
			return StyleGetValue::from_time(time.number() / 1000.0f);
		}
	}

	StyleGetValue StyleCascade::compute_frequency(const StyleGetValue &frequency) const
	{
		switch (frequency.dimension())
		{
		default:
		case StyleDimension::hz:
			return frequency;
		case StyleDimension::khz:
			return StyleGetValue::from_frequency(frequency.number() * 1000.0f);
		}
	}

	StyleGetValue StyleCascade::compute_resolution(const StyleGetValue &resolution) const
	{
		switch (resolution.dimension())
		{
		default:
		case StyleDimension::dppx:
			return resolution;
		case StyleDimension::dpi:
			return StyleGetValue::from_resolution(resolution.number() / 96.0f);
		case StyleDimension::dpcm:
			return StyleGetValue::from_resolution(resolution.number() / (float)(96.0 / 2.54));
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

	Font StyleCascade::font(Canvas &canvas) const
	{
		auto font_size = computed_value("font-size");
		auto line_height = computed_value("line-height");
		auto font_weight = computed_value("font-weight");
		auto font_style = computed_value("font-style");
		//auto font_variant = computed_value("font-variant"); // To do: needs FontDescription support
		auto font_rendering = computed_value("-clan-font-rendering");
		auto font_family_name = computed_value("font-family-names[0]");

		FontDescription font_desc;
		font_desc.set_height(font_size.number());

		if (line_height.is_length())
			font_desc.set_line_height(line_height.number());
		else if (line_height.is_number())
			font_desc.set_line_height(line_height.number() * font_size.number());

		if (font_weight.is_keyword("normal"))
			font_desc.set_weight(FontWeight::normal);
		else if (font_weight.is_keyword("bold") || font_weight.is_keyword("bolder"))
			font_desc.set_weight(FontWeight::bold);
		else if (font_weight.is_keyword("lighter"))
			font_desc.set_weight(FontWeight::light);
		else if (font_weight.is_number())
			font_desc.set_weight((FontWeight)(int)font_weight.number());

		if (font_style.is_keyword("normal"))
			font_desc.set_style(FontStyle::normal);
		else if (font_style.is_keyword("italic"))
			font_desc.set_style(FontStyle::italic);
		else if (font_style.is_keyword("oblique"))
			font_desc.set_style(FontStyle::oblique);

		font_desc.set_subpixel(!font_rendering.is_keyword("anti-alias"));

		std::string family;
		if (font_family_name.is_string())
			family = font_family_name.text();
		else if (font_family_name.is_keyword())
			family = font_family_name.text();
		else
			family = "sans-serif";

		return Font::resource(canvas, family, font_desc, UIThread::get_resources());
	}

	int StyleCascade::array_size(const char *property_name) const
	{
		int size = 0;
		while (true)
		{
			StyleString prop_name;
			prop_name.append(property_name);
			prop_name.append("[");
			prop_name.append(StringHelp::int_to_text(size));
			prop_name.append("]");
			if (specified_value(prop_name).is_undefined())
				break;
			size++;
		}
		return size;
	}
}
