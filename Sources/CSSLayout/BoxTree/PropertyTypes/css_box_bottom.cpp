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
#include "API/CSSLayout/PropertyTypes/css_box_bottom.h"
#include "API/CSSLayout/css_box_properties.h"
#include "../../css_resource_cache.h"

namespace clan
{

CSSBoxBottom::CSSBoxBottom()
: type(type_auto), percentage(0.0f)
{
}

void CSSBoxBottom::compute(CSSBoxTop &top, CSSBoxBottom &bottom, const CSSBoxProperties *parent, CSSResourceCache *layout, float em_size, float ex_size, const CSSBoxPosition &position)
{
	if (top.type == CSSBoxTop::type_inherit)
	{
		if (parent)
		{
			top.type = parent->top.type;
			top.length = parent->top.length;
			top.percentage = parent->top.percentage;
		}
		else
		{
			top.type = CSSBoxTop::type_auto;
		}
	}

	if (bottom.type == type_inherit)
	{
		if (parent)
		{
			bottom.type = parent->bottom.type;
			bottom.length = parent->bottom.length;
			bottom.percentage = parent->bottom.percentage;
		}
		else
		{
			bottom.type = type_auto;
		}
	}

	if (top.type == CSSBoxTop::type_length)
		top.length = layout->compute_length(top.length, em_size, ex_size);
	if (bottom.type == type_length)
		bottom.length = layout->compute_length(bottom.length, em_size, ex_size);

	if (position.type == CSSBoxPosition::type_static)
	{
		top.type = CSSBoxTop::type_auto;
		bottom.type = type_auto;
	}
	else if (position.type == CSSBoxPosition::type_relative)
	{
		if (top.type == CSSBoxTop::type_auto && bottom.type == type_auto)
		{
			top.type = CSSBoxTop::type_length;
			bottom.type = type_length;
			top.length = CSSBoxLength(0.0f, CSSBoxLength::type_computed_px);
			bottom.length = CSSBoxLength(0.0f, CSSBoxLength::type_computed_px);
		}
		else if (top.type != CSSBoxTop::type_auto)
		{
			if (top.type == CSSBoxTop::type_length)
			{
				bottom.type = type_length;
				bottom.length = top.length;
				bottom.length.value = -bottom.length.value;
			}
			else if (top.type == CSSBoxTop::type_percentage)
			{
				bottom.type = type_percentage;
				bottom.percentage = -top.percentage;
			}
		}
		else
		{
			if (bottom.type == type_length)
			{
				top.type = CSSBoxTop::type_length;
				top.length = bottom.length;
				top.length.value = -top.length.value;
			}
			else if (bottom.type == type_percentage)
			{
				top.type = CSSBoxTop::type_percentage;
				top.percentage = -bottom.percentage;
			}
		}
	}
}

std::string CSSBoxBottom::to_string() const
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
