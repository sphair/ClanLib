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
#include "API/CSSLayout/PropertyValues/css_value_shadow.h"
#include "../css_resource_cache.h"

namespace clan
{

CSSValueShadow::CSSValueShadow()
: type(type_none)
{
}

void CSSValueShadow::compute(const CSSValueShadow *parent, CSSResourceCache *layout, float em_size, float ex_size)
{
	if (type == type_inherit)
	{
		if (parent)
		{
			type = parent->type;
			shadows = parent->shadows;
		}
		else
		{
			type = type_none;
			shadows.clear();
		}
	}

	for (size_t i = 0; i < shadows.size(); i++)
	{
		shadows[i].horizontal_offset = layout->compute_length(shadows[i].horizontal_offset, em_size, ex_size);
		shadows[i].vertical_offset = layout->compute_length(shadows[i].vertical_offset, em_size, ex_size);
		shadows[i].blur_radius = layout->compute_length(shadows[i].blur_radius, em_size, ex_size);
		shadows[i].spread_distance = layout->compute_length(shadows[i].spread_distance, em_size, ex_size);
	}
}

std::string CSSValueShadow::to_string() const
{
	if (type == type_inherit)
		return "inherit";
	else if (type == type_none)
		return "none";

	std::string s;
	for (size_t i = 0; i < shadows.size(); i++)
	{
		if (i > 0)
			s += ", ";
		if (shadows[i].inset)
			s += "inset ";
		s += shadows[i].horizontal_offset.to_string() + " ";
		s += shadows[i].vertical_offset.to_string() + " ";
		s += shadows[i].blur_radius.to_string() + " ";
		s += shadows[i].spread_distance.to_string() + " ";
		s += string_format("rgba(%1,%2,%3,%4)", (int)(shadows[i].color.r * 255.0f + 0.5f), (int)(shadows[i].color.g * 255.0f + 0.5f), (int)(shadows[i].color.b * 255.0f + 0.5f), (int)(shadows[i].color.a * 255.0f + 0.5f));
	}
	return s;
}

}
