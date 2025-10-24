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
#include "resource_generic.h"
#include "API/Core/Resources/resourcedata.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Resource_Generic construction:

CL_Resource_Generic::CL_Resource_Generic(
	CL_DomElement &element,
	const CL_WeakPtr<CL_ResourceManager_Generic> &manager)
: load_ref(0), manager(manager), element(element)
{
}

CL_Resource_Generic::~CL_Resource_Generic()
{
	std::map<std::string, CL_ResourceData*>::iterator it;
	for (it = datas.begin(); it != datas.end(); ++it)
	{
		delete it->second;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_Resource_Generic attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_Resource_Generic operations:

/////////////////////////////////////////////////////////////////////////////
// CL_Resource_Generic implementation:
