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
#include "component_manager_generic.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ComponentManager construction:

CL_ComponentManager::CL_ComponentManager(
	const std::string &resource_id,
	CL_ResourceManager *res_manager,
	CL_Component *parent,
	CL_StyleManager *style)
{
	impl = new CL_ComponentManager_Generic(resource_id, res_manager, parent, style);
}

CL_ComponentManager::CL_ComponentManager(
	const std::string &filename,
	CL_Component *parent,
	CL_StyleManager *style, 
	CL_InputSourceProvider *input_provider,
	bool delete_provider)
{
	impl = new CL_ComponentManager_Generic(filename, parent, style, input_provider, delete_provider);
}

CL_ComponentManager::CL_ComponentManager(
	const CL_DomDocument &document,
	const CL_DomElement &element,
	CL_Component *parent,
	CL_StyleManager *style)
{
	impl = new CL_ComponentManager_Generic(document, element, parent, style);
}

CL_ComponentManager::CL_ComponentManager(const CL_ComponentManager &copy)
: impl(copy.impl)
{
}

CL_ComponentManager::~CL_ComponentManager()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_ComponentManager attributes:

CL_Component *CL_ComponentManager::get_component(const std::string &name) const
{
	return impl->get_component(name);
}

CL_StyleManager *CL_ComponentManager::get_style_manager() const
{
	return impl->get_style_manager();
}

/////////////////////////////////////////////////////////////////////////////
// CL_ComponentManager operations:

/////////////////////////////////////////////////////////////////////////////
// CL_ComponentManager implementation:
