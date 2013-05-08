/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

#include "CSSLayout/precomp.h"
#include "API/CSSLayout/PropertyValues/css_value_font_size.h"
#include "API/CSSLayout/ComputedValues/css_computed_values_updater.h"
#include "../css_resource_cache.h"

namespace clan
{

CSSValueFontSize::CSSValueFontSize()
: type(type_inherit), percentage(0.0f)
{
}

void CSSValueFontSize::apply(CSSComputedValuesUpdater *updater)
{
	updater->get_font().font_size = *this;
}

void CSSValueFontSize::compute(const CSSValueFontSize *parent, CSSResourceCache *layout, float em_size, float ex_size)
{
	if (type == type_inherit)
	{
		if (parent)
		{
			type = parent->type;
			length = parent->length;
			percentage = parent->percentage;
		}
		else
		{
			type = type_medium;
		}
	}

	switch (type)
	{
	case type_xx_small:
		type = type_length;
		length = layout->get_font_table_size(0);
		break;
	case type_x_small:
		type = type_length;
		length = layout->get_font_table_size(1);
		break;
	case type_small:
		type = type_length;
		length = layout->get_font_table_size(2);
		break;
	case type_medium:
		type = type_length;
		length = layout->get_font_table_size(3);
		break;
	case type_large:
		type = type_length;
		length = layout->get_font_table_size(4);
		break;
	case type_x_large:
		type = type_length;
		length = layout->get_font_table_size(5);
		break;
	case type_xx_large:
		type = type_length;
		length = layout->get_font_table_size(6);
		break;
	case type_smaller:
		type = type_length;
		length = layout->get_font_table_smaller(em_size);
		break;
	case type_larger:
		type = type_length;
		length = layout->get_font_table_larger(em_size);
		break;
	default:
		break;
	}

	if (type == type_percentage)
	{
		type = type_length;
		length = CSSLength(percentage / 100.0f, CSSLength::type_em);
	}

	if (type == type_length)
		length = layout->compute_length(length, em_size, ex_size);
}

std::string CSSValueFontSize::to_string() const
{
	switch (type)
	{
	default:
	case type_xx_small:
		return "xx-small";
	case type_x_small:
		return "x-small";
	case type_small:
		return "small";
	case type_medium:
		return "medium";
	case type_large:
		return "large";
	case type_x_large:
		return "x-large";
	case type_xx_large:
		return "xx-large";
	case type_larger:
		return "larger";
	case type_smaller:
		return "smaller";
	case type_length:
		return length.to_string();
	case type_percentage:
		return StringHelp::float_to_text(percentage) + "%";
	case type_inherit:
		return "inherit";
	}
}

}
