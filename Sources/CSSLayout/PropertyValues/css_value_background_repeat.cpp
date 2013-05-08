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
#include "API/CSSLayout/PropertyValues/css_value_background_repeat.h"
#include "API/CSSLayout/ComputedValues/css_computed_values_updater.h"

namespace clan
{

CSSValueBackgroundRepeat::CSSValueBackgroundRepeat()
: type(type_repeat_style)
{
	repeat_x.push_back(style_repeat);
	repeat_y.push_back(style_repeat);
}

void CSSValueBackgroundRepeat::apply(CSSComputedValuesUpdater *updater)
{
	updater->get_background().background_repeat = *this;
}

void CSSValueBackgroundRepeat::compute(const CSSValueBackgroundRepeat *parent, CSSResourceCache *layout, float em_size, float ex_size)
{
	if (type == type_inherit)
	{
		if (parent)
		{
			type = parent->type;
			repeat_x = parent->repeat_x;
			repeat_y = parent->repeat_y;
		}
		else
		{
			type = type_repeat_style;
			repeat_x.clear();
			repeat_y.clear();
			repeat_x.push_back(style_repeat);
			repeat_y.push_back(style_repeat);
		}
	}
}

std::string CSSValueBackgroundRepeat::to_string() const
{
	if (type == type_inherit)
		return "inherit";

	std::string s;
	for (size_t i = 0; i < repeat_x.size(); i++)
	{
		if (i > 0)
			s += ", ";
		switch (repeat_x[i])
		{
		case style_no_repeat:
			s += "no-repeat";
			break;
		case style_repeat:
			s += "repeat";
			break;
		case style_round:
			s += "round";
			break;
		case style_space:
			s += "space";
			break;
		}
	}
	return s;

}

}
