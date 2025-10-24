/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include "Core/precomp.h"
#include "API/Core/Resources/resourcetype_string.h"
#include "API/Core/Resources/resource.h"
#include "API/Core/System/clanstring.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_String construction:

CL_ResourceData_String::CL_ResourceData_String(CL_Resource &resource)
: CL_ResourceData(resource)
{
	value = resource.get_element().get_attribute("value");

	value = CL_String::convert_c_escapes(value);

	resource.attach_data("string", this);
}
