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
#include "API/CSSLayout/PropertyValues/css_value_right.h"
#include "API/CSSLayout/PropertyValues/css_value_left.h"
#include "API/CSSLayout/ComputedValues/css_computed_values_updater.h"
#include "../css_resource_cache.h"

namespace clan
{

CSSValueRight::CSSValueRight()
: type(type_auto)
{
}

void CSSValueRight::apply(CSSComputedValuesUpdater *updater)
{
	updater->get_box().right = *this;
}

void CSSValueRight::compute(CSSValueLeft &left, CSSValueRight &right, const CSSComputedBox *parent, CSSResourceCache *layout, float em_size, float ex_size, const CSSValuePosition &position, bool is_containing_block_ltr)
{
	if (left.type == CSSValueLeft::type_inherit)
	{
		if (parent)
		{
			left.type = parent->left.type;
			left.length = parent->left.length;
			left.percentage = parent->left.percentage;
		}
		else
		{
			left.type = CSSValueLeft::type_auto;
		}
	}

	if (right.type == type_inherit)
	{
		if (parent)
		{
			right.type = parent->right.type;
			right.length = parent->right.length;
			right.percentage = parent->right.percentage;
		}
		else
		{
			right.type = type_auto;
		}
	}

	if (left.type == CSSValueLeft::type_length)
		left.length = layout->compute_length(left.length, em_size, ex_size);

	if (right.type == type_length)
		right.length = layout->compute_length(right.length, em_size, ex_size);

	if (position.type == CSSValuePosition::type_static)
	{
		left.type = CSSValueLeft::type_auto;
		right.type = type_auto;
	}
	else if (position.type == CSSValuePosition::type_relative)
	{
		if (left.type == CSSValueLeft::type_auto && right.type == type_auto)
		{
			left.type = CSSValueLeft::type_length;
			right.type = type_length;
			left.length = CSSLength(0.0f, CSSLength::type_computed_px);
			right.length = CSSLength(0.0f, CSSLength::type_computed_px);
		}
		else if ((is_containing_block_ltr && left.type != CSSValueLeft::type_auto) || right.type == type_auto)
		{
			if (left.type == CSSValueLeft::type_length)
			{
				right.type = type_length;
				right.length = left.length;
				right.length.value = -right.length.value;
			}
			else if (left.type == CSSValueLeft::type_percentage)
			{
				right.type = type_percentage;
				right.percentage = -left.percentage;
			}
		}
		else
		{
			if (right.type == type_length)
			{
				left.type = CSSValueLeft::type_length;
				left.length = right.length;
				left.length.value = -left.length.value;
			}
			else if (right.type == type_percentage)
			{
				left.type = CSSValueLeft::type_percentage;
				left.percentage = -right.percentage;
			}
		}
	}
}

std::string CSSValueRight::to_string() const
{
	switch (type)
	{
	default:
	case type_length:
		return length.to_string();
	case type_percentage:
		return StringHelp::float_to_text(percentage) + "%";
	case type_auto:
		return "auto";
	case type_inherit:
		return "inherit";
	}
}

}
