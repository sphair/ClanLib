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
#include "API/CSSLayout/PropertyValues/css_value_list_style_type.h"
#include "API/CSSLayout/ComputedValues/css_computed_values_updater.h"

namespace clan
{

CSSValueListStyleType::CSSValueListStyleType()
: type(type_inherit)
{
}

void CSSValueListStyleType::apply(CSSComputedValuesUpdater *updater)
{
	updater->get_list_style().list_style_type = *this;
}

void CSSValueListStyleType::compute(const CSSValueListStyleType *parent, CSSResourceCache *layout, float em_size, float ex_size)
{
	if (type == type_inherit)
	{
		if (parent)
			type = parent->type;
		else
			type = type_disc;
	}
}

std::string CSSValueListStyleType::to_string() const
{
	switch (type)
	{
	default:
	case type_disc:
		return "disc";
	case type_circle:
		return "circle";
	case type_square:
		return "square";
	case type_decimal:
		return "decimal";
	case type_decimal_leading_zero:
		return "decimal-leading-zero";
	case type_lower_roman:
		return "lower-roman";
	case type_upper_roman:
		return "upper-roman";
	case type_lower_greek:
		return "lower-greek";
	case type_lower_latin:
		return "lower-latin";
	case type_upper_latin:
		return "upper-latin";
	case type_armenian:
		return "armenian";
	case type_georgian:
		return "georgian";
	case type_lower_alpha:
		return "lower-alpha";
	case type_upper_alpha:
		return "upper-alpha";
	case type_none:
		return "none";
	case type_inherit:
		return "inherit";
	}
}

}
