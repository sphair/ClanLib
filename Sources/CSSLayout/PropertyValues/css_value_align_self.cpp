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
#include "API/CSSLayout/PropertyValues/css_value_align_self.h"
#include "API/CSSLayout/PropertyValues/css_value_align_items.h"
#include "API/CSSLayout/ComputedValues/css_computed_box.h"

namespace clan
{

CSSValueAlignSelf::CSSValueAlignSelf()
: type(type_auto)
{
}

void CSSValueAlignSelf::apply_to_box(CSSComputedBox &box)
{
	box.align_self = *this;
}

void CSSValueAlignSelf::compute(const CSSValueAlignSelf *parent, CSSResourceCache *layout, float em_size, float ex_size, const CSSValueAlignItems *parent_align_items)
{
	if (type == type_inherit)
	{
		if (parent)
			type = parent->type;
		else
			type = type_auto;
	}

	if (type == type_auto)
	{
		if (parent_align_items)
		{
			switch (parent_align_items->type)
			{
			default:
			case CSSValueAlignItems::type_flex_start:
				type = CSSValueAlignSelf::type_flex_start;
				break;
			case CSSValueAlignItems::type_flex_end:
				type = CSSValueAlignSelf::type_flex_end;
				break;
			case CSSValueAlignItems::type_center:
				type = CSSValueAlignSelf::type_center;
				break;
			case CSSValueAlignItems::type_baseline:
				type = CSSValueAlignSelf::type_baseline;
				break;
			case CSSValueAlignItems::type_stretch:
				type = CSSValueAlignSelf::type_stretch;
				break;
			}
		}
		else
		{
			type = type_stretch;
		}
	}
}

std::string CSSValueAlignSelf::to_string() const
{
	switch (type)
	{
	default:
	case type_auto:
		return "auto";
	case type_flex_start:
		return "flex-start";
	case type_flex_end:
		return "flex-end";
	case type_center:
		return "center";
	case type_baseline:
		return "baseline";
	case type_stretch:
		return "stretch";
	case type_inherit:
		return "inherit";
	}
}

}
