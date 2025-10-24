/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
#include "css_box_right.h"
#include "css_box_left.h"
#include "../css_box_properties.h"
#include "../../css_resource_cache.h"

CL_CSSBoxRight::CL_CSSBoxRight()
: type(type_auto)
{
}

void CL_CSSBoxRight::compute(CL_CSSBoxLeft &left, CL_CSSBoxRight &right, const CL_CSSBoxProperties *parent, CL_CSSResourceCache *layout, float em_size, float ex_size, const CL_CSSBoxPosition &position, bool is_containing_block_ltr)
{
	if (left.type == CL_CSSBoxLeft::type_inherit)
	{
		if (parent)
		{
			left.type = parent->left.type;
			left.length = parent->left.length;
			left.percentage = parent->left.percentage;
		}
		else
		{
			left.type = CL_CSSBoxLeft::type_auto;
		}
		if (left.type == CL_CSSBoxLeft::type_length)
			left.length = layout->compute_length(left.length, em_size, ex_size);
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
		if (right.type == type_length)
			right.length = layout->compute_length(right.length, em_size, ex_size);
	}

	if (position.type == CL_CSSBoxPosition::type_static)
	{
		left.type = CL_CSSBoxLeft::type_auto;
		right.type = type_auto;
	}
	else if (position.type == CL_CSSBoxPosition::type_relative)
	{
		if (left.type == CL_CSSBoxLeft::type_auto && right.type == type_auto)
		{
			left.type = CL_CSSBoxLeft::type_length;
			right.type = type_length;
			left.length = CL_CSSBoxLength(0.0f, CL_CSSBoxLength::type_computed_px);
			right.length = CL_CSSBoxLength(0.0f, CL_CSSBoxLength::type_computed_px);
		}
		else if ((is_containing_block_ltr && left.type != CL_CSSBoxLeft::type_auto) || right.type == type_auto)
		{
			if (left.type == CL_CSSBoxLeft::type_length)
			{
				right.type = type_length;
				right.length = left.length;
				right.length.value = -right.length.value;
			}
			else if (left.type == CL_CSSBoxLeft::type_percentage)
			{
				right.type = type_percentage;
				right.percentage = -left.percentage;
			}
		}
		else
		{
			if (right.type == type_length)
			{
				left.type = CL_CSSBoxLeft::type_length;
				left.length = right.length;
				left.length.value = -left.length.value;
			}
			else if (right.type == type_percentage)
			{
				left.type = CL_CSSBoxLeft::type_percentage;
				left.percentage = -right.percentage;
			}
		}
	}
}
