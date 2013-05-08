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
**    Mark Page
*/

#include "CSSLayout/precomp.h"
#include "API/CSSLayout/PropertyValues/css_value_generic.h"
#include "API/CSSLayout/ComputedValues/css_computed_values_updater.h"
#include "../css_resource_cache.h"

namespace clan
{

CSSValueGeneric::CSSValueGeneric(const std::string &property_name)
: type(type_inherit), property_name(property_name)
{
}

void CSSValueGeneric::apply(CSSComputedValuesUpdater *updater)
{
	CSSComputedGeneric &generic_values = updater->get_generic();
	for (size_t cnt = 0; cnt < generic_values.generic_values.size(); cnt++)
	{
		if (generic_values.generic_values[cnt].property_name == property_name)
		{
			generic_values.generic_values[cnt] = *this;
			return;
		}
	}
	generic_values.generic_values.push_back(*this);
}

void CSSValueGeneric::compute(const std::vector<CSSValueGeneric> *parent, CSSResourceCache *layout, float em_size, float ex_size)
{
	if (type == type_inherit)
	{
		const CSSValueGeneric *matching_parent = 0;
		if (parent)
		{
			for (size_t cnt = 0; cnt < parent->size(); cnt++)
			{
				if ((*parent)[cnt].property_name == property_name)
				{
					matching_parent = &(*parent)[cnt];
				}
			}
		}

		if (matching_parent)
		{
			type = matching_parent->type;
			tokens = matching_parent->tokens;
		}
		else
		{
			type = type_tokens;
			tokens.clear();
		}
	}
}

std::string CSSValueGeneric::to_string() const
{
	if (type == type_inherit)
		return "inherit";

	return "CSSValueGeneric::to_string() - not implemented";
}

}
