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
#include "API/CSSLayout/PropertyValues/css_value_border_image_width.h"
#include "../css_resource_cache.h"
#include "API/CSSLayout/ComputedValues/css_computed_box.h"

namespace clan
{

CSSValueBorderImageWidth::CSSValueBorderImageWidth()
: type(type_values),
  value_top(value_type_number), value_right(value_type_number), value_bottom(value_type_number), value_left(value_type_number),
  number_top(1.0f), number_right(1.0f), number_bottom(1.0f), number_left(1.0f),
  percentage_top(0.0f), percentage_right(0.0f), percentage_bottom(0.0f), percentage_left(0.0f)
{
}
void CSSValueBorderImageWidth::apply_to_box(CSSComputedBox &box)
{
	box.border_image_width = *this;
}

void CSSValueBorderImageWidth::compute(const CSSValueBorderImageWidth *parent, CSSResourceCache *layout, float em_size, float ex_size)
{
	if (type == type_inherit)
	{
		if (parent)
		{
			type = parent->type;
			value_top = parent->value_top;
			value_right = parent->value_right;
			value_bottom = parent->value_bottom;
			value_left = parent->value_left;
			length_top = parent->length_top;
			length_right = parent->length_right;
			length_bottom = parent->length_bottom;
			length_left = parent->length_left;
			number_top = parent->number_top;
			number_right = parent->number_right;
			number_bottom = parent->number_bottom;
			number_left = parent->number_left;
			percentage_top = parent->percentage_top;
			percentage_right = parent->percentage_right;
			percentage_bottom = parent->percentage_bottom;
			percentage_left = parent->percentage_left;
		}
		else
		{
			type = type_values;
			value_top = value_type_percentage;
			value_right = value_type_percentage;
			value_bottom = value_type_percentage;
			value_left = value_type_percentage;
			length_top = CSSLength();
			length_right = CSSLength();
			length_bottom = CSSLength();
			length_left = CSSLength();
			number_top = 0.0f;
			number_right = 0.0f;
			number_bottom = 0.0f;
			number_left = 0.0f;
			percentage_top = 0.0f;
			percentage_right = 0.0f;
			percentage_bottom = 0.0f;
			percentage_left = 0.0f;
		}
	}

	if (value_top == value_type_length)
		length_top = layout->compute_length(length_top, em_size, ex_size);
	if (value_right == value_type_length)
		length_right = layout->compute_length(length_right, em_size, ex_size);
	if (value_bottom == value_type_length)
		length_bottom = layout->compute_length(length_bottom, em_size, ex_size);
	if (value_left == value_type_length)
		length_left = layout->compute_length(length_left, em_size, ex_size);
}

std::string CSSValueBorderImageWidth::to_string() const
{
	if (type == type_inherit)
		return "inherit";

	std::string s;

	switch (value_top)
	{
	default:
	case value_type_length:
		s += length_top.to_string();
		break;
	case value_type_percentage:
		s += StringHelp::float_to_text(percentage_top) + "%";
		break;
	case value_type_auto:
		s += "auto";
		break;
	case value_type_number:
		s += StringHelp::float_to_text(number_top);
		break;
	}

	s += " ";

	switch (value_right)
	{
	default:
	case value_type_length:
		s += length_right.to_string();
		break;
	case value_type_percentage:
		s += StringHelp::float_to_text(percentage_right) + "%";
		break;
	case value_type_auto:
		s += "auto";
		break;
	case value_type_number:
		s += StringHelp::float_to_text(number_right);
		break;
	}

	s += " ";

	switch (value_bottom)
	{
	default:
	case value_type_length:
		s += length_bottom.to_string();
		break;
	case value_type_percentage:
		s += StringHelp::float_to_text(percentage_bottom) + "%";
		break;
	case value_type_auto:
		s += "auto";
		break;
	case value_type_number:
		s += StringHelp::float_to_text(number_bottom);
		break;
	}

	s += " ";

	switch (value_left)
	{
	default:
	case value_type_length:
		s += length_left.to_string();
		break;
	case value_type_percentage:
		s += StringHelp::float_to_text(percentage_left) + "%";
		break;
	case value_type_auto:
		s += "auto";
		break;
	case value_type_number:
		s += StringHelp::float_to_text(number_left);
		break;
	}

	return s;
}

}
