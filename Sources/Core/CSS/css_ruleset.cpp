/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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

#include "Core/precomp.h"
#include "API/Core/CSS/css_ruleset.h"
#include "css_ruleset_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_CSSRuleSet Construction:

CL_CSSRuleSet::CL_CSSRuleSet()
: impl(new CL_CSSRuleSet_Impl)
{
}

CL_CSSRuleSet::~CL_CSSRuleSet()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_CSSRuleSet Attributes:

std::vector<CL_CSSSelector> &CL_CSSRuleSet::get_selectors()
{
	return impl->selectors;
}

std::vector<CL_CSSProperty> &CL_CSSRuleSet::get_properties()
{
	return impl->properties;
}

/////////////////////////////////////////////////////////////////////////////
// CL_CSSRuleSet Operations:

bool CL_CSSRuleSet::operator ==(const CL_CSSRuleSet &other) const
{
	if (impl->selectors != other.impl->selectors)
		return false;
	if (impl->properties != other.impl->properties)
		return false;
	return true;
}

void CL_CSSRuleSet::add_selector(const CL_CSSSelector &selector)
{
	impl->selectors.push_back(selector);
}

void CL_CSSRuleSet::add_property(const CL_CSSProperty &property)
{
	impl->properties.push_back(property);
}

/////////////////////////////////////////////////////////////////////////////
// CL_CSSRuleSet Implementation:
