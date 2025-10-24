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

#include "Display/display_precomp.h"
#include "API/Display/Providers/provider_type.h"
#include "API/Display/Providers/provider_factory.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ProviderType construction:

CL_ProviderType::CL_ProviderType(const std::string &type)
{
	CL_ProviderFactory::types[type] = this;
}

CL_ProviderType::~CL_ProviderType()
{
	std::map<std::string, CL_ProviderType *>::iterator it;
	
	for (it = CL_ProviderFactory::types.begin(); it != CL_ProviderFactory::types.end(); it++)
	{
		if (it->second == this)
		{
			CL_ProviderFactory::types.erase(it);
			break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_ProviderType operations:


/////////////////////////////////////////////////////////////////////////////
// CL_ProviderType implementation:
