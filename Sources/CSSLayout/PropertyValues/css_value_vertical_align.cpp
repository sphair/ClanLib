/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
#include "API/CSSLayout/PropertyValues/css_value_vertical_align.h"
#include "API/CSSLayout/PropertyValues/css_value_line_height.h"
#include "../css_resource_cache.h"
#include "API/CSSLayout/ComputedValues/css_computed_box.h"

namespace clan
{

CSSValueVerticalAlign::CSSValueVerticalAlign()
: type(type_baseline), percentage(0.0f)
{
}
void CSSValueVerticalAlign::apply_to_box(CSSComputedBox &box)
{
	box.vertical_align = *this;
}

void CSSValueVerticalAlign::compute(const CSSValueVerticalAlign *parent, CSSResourceCache *layout, float em_size, float ex_size, const CSSValueLineHeight &line_height)
{
	if (type == type_inherit)
	{
		if (parent)
		{
			type = parent->type;
			percentage = parent->percentage;
			length = parent->length;
		}
		else
		{
			type = type_baseline;
		}
	}

	if (type == type_percentage)
	{
		if (line_height.type == CSSValueLineHeight::type_number)
		{
			type = type_length;
			length = CSSLength(percentage * line_height.number / 100.0f, CSSLength::type_em);
		}
		else if (line_height.type == CSSValueLineHeight::type_length)
		{
			type = type_length;
			length = CSSLength(percentage * line_height.length.value / 100.0f, line_height.length.type);
		}
		else if (line_height.type == CSSValueLineHeight::type_normal)
		{
			type = type_length;
			length = CSSLength(percentage/100.0f, CSSLength::type_em);
		}
	}

	if (type == type_length)
		length = layout->compute_length(length, em_size, ex_size);
}

std::string CSSValueVerticalAlign::to_string() const
{
	switch (type)
	{
	default:
	case type_baseline:
		return "baseline";
	case type_sub:
		return "sub";
	case type_super:
		return "super";
	case type_top:
		return "top";
	case type_text_top:
		return "text-top";
	case type_middle:
		return "middle";
	case type_bottom:
		return "bottom";
	case type_text_bottom:
		return "text-bottom";
	case type_percentage:
		return StringHelp::float_to_text(percentage) + "%";
	case type_length:
		return length.to_string();
	case type_inherit:
		return "inherit";
	}
}

}
