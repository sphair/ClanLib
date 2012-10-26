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

#include "Core/precomp.h"
#include "API/Core/CSS/css_property.h"
#include "css_property_impl.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// CSSProperty Construction:

CSSProperty::CSSProperty()
: impl(new CSSProperty_Impl)
{
}

CSSProperty::~CSSProperty()
{
}

/////////////////////////////////////////////////////////////////////////////
// CSSProperty Attributes:

const std::string &CSSProperty::get_name() const
{
	return impl->name;
}

const std::string &CSSProperty::get_value() const
{
	return impl->value;
}

CSSProperty::Priority CSSProperty::get_priority() const
{
	return impl->priority;
}

/////////////////////////////////////////////////////////////////////////////
// CSSProperty Operations:

void CSSProperty::set_name(const std::string &name)
{
	impl->name = name;
}

void CSSProperty::set_value(const std::string &value)
{
	impl->value = value;
}

void CSSProperty::set_priority(Priority priority)
{
	impl->priority = priority;
}

bool CSSProperty::operator ==(const CSSProperty &other) const
{
	if (impl->name != other.impl->name)
		return false;
	if (impl->value != other.impl->value)
		return false;
	if (impl->priority != other.impl->priority)
		return false;
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// CSSProperty Implementation:

}
