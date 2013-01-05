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
#include "API/CSSLayout/PropertyValues/css_value_flex_direction.h"

namespace clan
{

CSSValueFlexDirection::CSSValueFlexDirection()
: type(type_row)
{
}

void CSSValueFlexDirection::compute(const CSSValueFlexDirection *parent, CSSResourceCache *layout, float em_size, float ex_size)
{
	if (type == type_inherit)
	{
		if (parent)
			type = parent->type;
		else
			type = type_row;
	}
}

std::string CSSValueFlexDirection::to_string() const
{
	switch (type)
	{
	default:
	case type_row:
		return "row";
	case type_row_reverse:
		return "row-reverse";
	case type_column:
		return "column";
	case type_column_reverse:
		return "column-reverse";
	case type_inherit:
		return "inherit";
	}
}

}
