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

#include "Core/precomp.h"
#include "API/Core/CSS/css_property.h"
#include "css_property_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_CSSProperty Construction:

CL_CSSProperty::CL_CSSProperty()
: impl(new CL_CSSProperty_Impl)
{
}

CL_CSSProperty::~CL_CSSProperty()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_CSSProperty Attributes:

const CL_String &CL_CSSProperty::get_name() const
{
	return impl->name;
}

const CL_String &CL_CSSProperty::get_value() const
{
	return impl->value;
}

CL_CSSProperty::Priority CL_CSSProperty::get_priority() const
{
	return impl->priority;
}

/////////////////////////////////////////////////////////////////////////////
// CL_CSSProperty Operations:

void CL_CSSProperty::set_name(const CL_String &name)
{
	impl->name = name;
}

void CL_CSSProperty::set_value(const CL_String &value)
{
	impl->value = value;
}

void CL_CSSProperty::set_priority(Priority priority)
{
	impl->priority = priority;
}

bool CL_CSSProperty::operator ==(const CL_CSSProperty &other) const
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
// CL_CSSProperty Implementation:
