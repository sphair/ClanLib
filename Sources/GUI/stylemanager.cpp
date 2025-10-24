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

#include "precomp.h"
#include "API/GUI/component_manager.h"
#include "API/GUI/stylemanager.h"

/////////////////////////////////////////////////////////////////////////////
// CL_StyleManager_Generic:

class CL_StyleManager_Generic
{
public:
	CL_ResourceManager *resources;
};

/////////////////////////////////////////////////////////////////////////////
// CL_StyleManager construction:

CL_StyleManager::CL_StyleManager(CL_ResourceManager *resources)
: impl(new CL_StyleManager_Generic)
{
	impl->resources = resources;
}

CL_StyleManager::~CL_StyleManager()
{
	delete impl;
}

/////////////////////////////////////////////////////////////////////////////
// CL_StyleManager attributes:

CL_ResourceManager *CL_StyleManager::get_resources()
{
	return impl->resources;
}

/////////////////////////////////////////////////////////////////////////////
// CL_StyleManager overridables:

CL_Component *CL_StyleManager::create_component(
	const std::string &type,
	CL_Component *parent)
{
	return CL_ComponentType::create_component(type, parent, this);
}
