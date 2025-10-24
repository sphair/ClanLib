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
#include "API/Core/CSS/css_import.h"
#include "css_import_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_CSSImport Construction:

CL_CSSImport::CL_CSSImport()
: impl(new CL_CSSImport_Impl)
{
}

CL_CSSImport::~CL_CSSImport()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_CSSImport Attributes:

CL_String &CL_CSSImport::get_value()
{
	return impl->value;
}

/////////////////////////////////////////////////////////////////////////////
// CL_CSSImport Operations:

void CL_CSSImport::set_value(const CL_String &value)
{
	impl->value = value;
}

bool CL_CSSImport::operator ==(const CL_CSSImport &other) const
{
	return impl->value == other.impl->value;
}

/////////////////////////////////////////////////////////////////////////////
// CL_CSSImport Implementation:
