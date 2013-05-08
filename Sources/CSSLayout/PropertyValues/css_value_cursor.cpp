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
#include "API/CSSLayout/PropertyValues/css_value_cursor.h"
#include "API/CSSLayout/ComputedValues/css_computed_values_updater.h"

namespace clan
{

CSSValueCursor::CSSValueCursor()
: type(type_inherit)
{
}

void CSSValueCursor::apply(CSSComputedValuesUpdater *updater)
{
	updater->get_misc_inherit().cursor = *this;
}

void CSSValueCursor::compute(const CSSValueCursor *parent, CSSResourceCache *layout, float em_size, float ex_size)
{
	if (type == type_inherit)
	{
		if (parent)
		{
			type = parent->type;
			urls = parent->urls;
		}
		else
		{
			type = type_auto;
		}
	}

	// To do: any relative URLs converted to absolute.
}

std::string CSSValueCursor::to_string() const
{
	switch (type)
	{
	default:
	case type_uris:
		{
			std::string s;
			for (size_t i = 0; i < urls.size(); i++)
			{
				if (i > 0)
					s += ", ";
				s += string_format("uri(\"%1\")", urls[i]);
			}
			return s;
		}
	case type_auto:
		return "auto";
	case type_crosshair:
		return "crosshair";
	case type_default:
		return "default";
	case type_pointer:
		return "pointer";
	case type_move:
		return "move";
	case type_e_resize:
		return "e-resize";
	case type_ne_resize:
		return "ne-resize";
	case type_nw_resize:
		return "nw-resize";
	case type_n_resize:
		return "n-resize";
	case type_se_resize:
		return "se-resize";
	case type_sw_resize:
		return "sw-resize";
	case type_s_resize:
		return "s-resize";
	case type_w_resize:
		return "w-resize";
	case type_text:
		return "text";
	case type_wait:
		return "wait";
	case type_help:
		return "help";
	case type_progress:
		return "progress";
	case type_inherit:
		return "inherit";
	}
}

}
