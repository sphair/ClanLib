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
#include "API/CSSLayout/css_property_list2.h"
#include "API/CSSLayout/css_property2.h"

/////////////////////////////////////////////////////////////////////////////

class CL_CSSPropertyList2_Impl
{
public:
	std::vector<CL_CSSProperty2> properties;
};

/////////////////////////////////////////////////////////////////////////////

CL_CSSPropertyList2::CL_CSSPropertyList2()
: impl(new CL_CSSPropertyList2_Impl())
{
}

size_t CL_CSSPropertyList2::size() const
{
	return impl->properties.size();
}

CL_CSSProperty2 &CL_CSSPropertyList2::at(size_t index)
{
	return impl->properties.at(index);
}

const CL_CSSProperty2 &CL_CSSPropertyList2::at(size_t index) const
{
	return impl->properties.at(index);
}

CL_CSSProperty2 &CL_CSSPropertyList2::operator[](size_t index)
{
	return impl->properties[index];
}

const CL_CSSProperty2 &CL_CSSPropertyList2::operator[](size_t index) const
{
	return impl->properties[index];
}

void CL_CSSPropertyList2::push_back(const CL_CSSProperty2 &value)
{
	impl->properties.push_back(value);
}
