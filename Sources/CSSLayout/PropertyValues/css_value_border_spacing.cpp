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
#include "API/CSSLayout/PropertyValues/css_value_border_spacing.h"
#include "../css_resource_cache.h"

namespace clan
{

CSSValueBorderSpacing::CSSValueBorderSpacing()
: type(type_inherit)
{
}

void CSSValueBorderSpacing::compute(const CSSValueBorderSpacing *parent, CSSResourceCache *layout, float em_size, float ex_size)
{
	if (type == type_inherit)
	{
		if (parent)
		{
			type = parent->type;
			length1 = parent->length1;
			length2 = parent->length2;
		}
		else
		{
			type = type_two_lengths;
			length1 = CSSLength(0.0f, CSSLength::type_px);
			length2 = CSSLength(0.0f, CSSLength::type_px);
		}
	}

	if (type == type_one_length)
	{
		type = type_two_lengths;
		length2 = length1;
	}

	length1 = layout->compute_length(length1, em_size, ex_size);
	length2 = layout->compute_length(length2, em_size, ex_size);
}

std::string CSSValueBorderSpacing::to_string() const
{
	switch (type)
	{
	default:
	case type_one_length:
		return length1.to_string();
	case type_two_lengths:
		return length1.to_string() + " " + length2.to_string();
	case type_inherit:
		return "inherit";
	}
}

}
