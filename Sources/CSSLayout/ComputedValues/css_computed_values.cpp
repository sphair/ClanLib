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
#include "API/CSSLayout/ComputedValues/css_computed_values.h"
#include "css_computed_values_impl.h"

namespace clan
{

CSSComputedValues::CSSComputedValues()
{
}

CSSComputedValues::CSSComputedValues(CSSResourceCache *resource_cache)
: impl(new CSSComputedValues_Impl(resource_cache))
{
}

bool CSSComputedValues::is_null() const
{
	return !impl;
}

void CSSComputedValues::set_parent(const CSSComputedValues &parent)
{
	impl->set_parent(parent);
}

void CSSComputedValues::set_specified_values(const CSSSelectResult &selected_values)
{
	set_specified_values(selected_values, CSSStyleProperties());
}

void CSSComputedValues::set_specified_values(const CSSSelectResult &selected_values, const std::string &style_values)
{
	set_specified_values(selected_values, CSSStyleProperties(style_values));
}

void CSSComputedValues::set_specified_values(const CSSSelectResult &selected_values, const CSSStyleProperties &style_values)
{
	impl->selected_values = selected_values;
	impl->style_values = style_values;
	impl->set_specified_values_changed();
}

const CSSComputedBox &CSSComputedValues::get_box() const
{
	const_cast<CSSComputedValues*>(this)->impl->update_if_changed();
	return impl->box;
}

int CSSComputedValues::get_box_generation() const
{
	const_cast<CSSComputedValues*>(this)->impl->update_if_changed();
	return impl->box_generation;
}

}
