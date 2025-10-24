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
#include "API/Core/Resources/resourcetype_boolean.h"
#include "API/Core/Resources/resource.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_Boolean construction:

CL_ResourceData_Boolean::CL_ResourceData_Boolean(CL_Resource &resource)
: CL_ResourceData(resource)
{
	std::string location = resource.get_element().get_attribute("value");

	if (location == "true" || location == "1")
	{
		value = true;
	}
	else if (location == "false" || location == "0")
	{
		value = false;
	}
	else
	{
		throw CL_Error(std::string("Boolean resource has invalid value: ") + location);
	}

	resource.attach_data("boolean", this);
}
