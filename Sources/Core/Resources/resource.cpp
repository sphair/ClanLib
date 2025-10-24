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
#include "API/Core/Resources/resource.h"
#include "API/Core/Resources/resourcedata.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/Core/System/cl_assert.h"
#include "Core/Resources/resource_manager_generic.h"
#include "resource_generic.h"
#include <ctype.h>
#include <algorithm>

/////////////////////////////////////////////////////////////////////////////
// CL_Resource construction:

CL_Resource::CL_Resource(
	CL_DomElement &element,
	CL_ResourceManager *manager)
: impl(new CL_Resource_Generic(element, manager->impl))
{
}

CL_Resource::CL_Resource(const CL_Resource &copy)
: impl(copy.impl)
{
}

CL_Resource::CL_Resource(const CL_SharedPtr<CL_Resource_Generic> &impl)
: impl(impl)
{
}

CL_Resource::CL_Resource()
{
}

CL_Resource::~CL_Resource()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_Resource attributes:

std::string CL_Resource::get_type() const
{
	if (!impl) return 0;
	return impl->element.get_tag_name();
}

std::string CL_Resource::get_name() const
{
	if (!impl) return 0;
	return impl->element.get_attribute("name");
}
/*
std::string CL_Resource::get_full_location() const
{
	CL_InputSourceProvider *provider = (CL_InputSourceProvider *) impl->manager->get_resource_provider();
	return provider->get_path(get_location().c_str());
}
*/

CL_DomElement &CL_Resource::get_element()
{
	if (!impl)
	{
		static CL_DomElement null;
		return null;
	}
	return impl->element;
}

CL_ResourceManager CL_Resource::get_manager()
{
	return CL_ResourceManager(impl->manager);
}

CL_ResourceData *CL_Resource::get_data(const std::string &name)
{
	if (!impl) return 0;
	return impl->datas[name];
}

int CL_Resource::get_reference_count() const
{
	if (!impl) return 0;
	return impl->load_ref;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Resource operations:

void CL_Resource::attach_data(const std::string &name, CL_ResourceData *data)
{
	impl->datas[name] = data;
}

void CL_Resource::detach_data(CL_ResourceData *data)
{
	std::map<std::string, CL_ResourceData *>::iterator it;
	for (it = impl->datas.begin(); it != impl->datas.end(); it++)
	{
		if (it->second == data)
		{
			impl->datas.erase(it);
			break;
		}
	}
}

void CL_Resource::unload()
{
	if (!impl) return;
	if (impl->manager == 0) return;

	impl->load_ref--;
	cl_assert(impl->load_ref >= 0);
	if (impl->load_ref == 0)
	{
		std::map<std::string, CL_ResourceData*>::iterator it;
		for (it = impl->datas.begin(); it != impl->datas.end(); ++it)
		{
			CL_ResourceData *data = it->second;
			data->on_unload();
		}
	}
}

void CL_Resource::load()
{
	if (!impl) return;
	if (impl->manager == 0) return;

	impl->load_ref++;
	if (impl->load_ref == 1)
	{
		std::map<std::string, CL_ResourceData*>::iterator it;
		for (it = impl->datas.begin(); it != impl->datas.end(); ++it)
		{
			CL_ResourceData *data = it->second;
			data->on_load();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_Resource implementation:
