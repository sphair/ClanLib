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
#include "API/CSSLayout/PropertyValues/css_value_min_width.h"
#include "API/CSSLayout/ComputedValues/css_computed_values_updater.h"
#include "../css_resource_cache.h"

namespace clan
{

CSSValueMinWidth::CSSValueMinWidth()
: type(type_auto), length(0, CSSLength::type_px), percentage(0.0f)
{
}

std::string CSSValueMinWidth::get_name() const
{
	return "min-width";
}
std::unique_ptr<CSSPropertyValue> CSSValueMinWidth::clone() const
{
	return std::unique_ptr<CSSPropertyValue>(new CSSValueMinWidth (*this));
}


void CSSValueMinWidth::apply(CSSComputedValuesUpdater *updater)
{
	updater->get_box().min_width = *this;
}

void CSSValueMinWidth::compute(const CSSValueMinWidth *parent, CSSResourceCache *layout, float em_size, float ex_size)
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
			type = type_auto;
		}
	}

	if (type == type_length)
		length = layout->compute_length(length, em_size, ex_size);
}

std::string CSSValueMinWidth::to_string() const
{
	switch (type)
	{
	default:
	case type_auto:
		return "auto";
	case type_length:
		return length.to_string();
	case type_percentage:
		return StringHelp::float_to_text(percentage) + "%";
	case type_inherit:
		return "inherit";
	}
}

}
