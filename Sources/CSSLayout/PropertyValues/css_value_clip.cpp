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
#include "API/CSSLayout/PropertyValues/css_value_clip.h"
#include "../css_resource_cache.h"

namespace clan
{

CSSValueClip::CSSValueClip()
: type(type_auto), left_auto(false), top_auto(false), right_auto(false), bottom_auto(false)
{
}

void CSSValueClip::compute(const CSSValueClip *parent, CSSResourceCache *layout, float em_size, float ex_size)
{
	if (type == type_inherit)
	{
		if (parent)
		{
			type = parent->type;
			left = parent->left;
			top = parent->top;
			right = parent->right;
			bottom = parent->bottom;
			left_auto = parent->left_auto;
			top_auto = parent->top_auto;
			right_auto = parent->right_auto;
			bottom_auto = parent->bottom_auto;
		}
		else
		{
			type = type_auto;
		}
	}

	if (type == type_rect)
	{
		if (!left_auto)
			left = layout->compute_length(left, em_size, ex_size);
		if (!top_auto)
			top = layout->compute_length(left, em_size, ex_size);
		if (!right_auto)
			right = layout->compute_length(left, em_size, ex_size);
		if (!bottom_auto)
			bottom = layout->compute_length(left, em_size, ex_size);

		// To do: Should we also compute the values of 'auto' here or should we postpone this to actual rendering?
		// If we do it now, we have to ensure rounding issues doesn't cause too much clipping when we map from
		// computed values to actual values.
	}
}

std::string CSSValueClip::to_string() const
{
	switch (type)
	{
	default:
	case type_rect:
		return string_format("rect(%1,%2,%3,%4)", top.to_string(), right.to_string(), bottom.to_string(), left.to_string());
	case type_auto:
		return "auto";
	case type_inherit:
		return "inherit";
	}
}

}
