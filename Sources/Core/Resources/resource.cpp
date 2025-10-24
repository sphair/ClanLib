/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
#include "API/Core/Resources/resource.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/Core/System/weakptr.h"
#include "API/Core/XML/dom_element.h"
#include <vector>

/////////////////////////////////////////////////////////////////////////////
// CL_Resource_Impl Class:

class CL_ResourceManager_Impl;

struct CL_ResourceCacheEntry
{
	CL_String name;

	CL_SharedPtr<CL_ResourceData> data;

	int reference_count;
};

class CL_Resource_Impl
{
//! Attributes:
public:
	CL_WeakPtr<CL_ResourceManager_Impl> resource_manager;

	CL_DomElement element;

	std::vector<CL_ResourceCacheEntry> cache_objects;
};

/////////////////////////////////////////////////////////////////////////////
// CL_Resource Construction:

CL_Resource::CL_Resource()
{
}

CL_Resource::CL_Resource(CL_DomElement element, CL_ResourceManager &resource_manager)
: impl(new CL_Resource_Impl)
{
	impl->element = element;
	impl->resource_manager = CL_WeakPtr<CL_ResourceManager_Impl>(resource_manager.impl);
}

CL_Resource::~CL_Resource()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_Resource Attributes:

CL_String CL_Resource::get_type() const
{
	return impl->element.get_local_name();
}

CL_String CL_Resource::get_name() const
{
	return impl->element.get_attribute("name");
}

CL_DomElement &CL_Resource::get_element()
{
	return impl->element;
}

CL_ResourceManager CL_Resource::get_manager()
{
	return CL_ResourceManager(impl->resource_manager);
}

CL_SharedPtr<CL_ResourceData> CL_Resource::get_data(const CL_String &name)
{
	std::vector<CL_ResourceCacheEntry>::size_type i;
	for (i = 0; i < impl->cache_objects.size(); i++)
		if (impl->cache_objects[i].name == name)
			return impl->cache_objects[i].data;
	return CL_SharedPtr<CL_ResourceData>();
}

int CL_Resource::get_data_session_count(const CL_String &data_name)
{
	std::vector<CL_ResourceCacheEntry>::size_type i;
	for (i = 0; i < impl->cache_objects.size(); i++)
	{
		if (impl->cache_objects[i].name == data_name)
		{
			return impl->cache_objects[i].reference_count;
		}
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Resource Operations:

bool CL_Resource::operator ==(const CL_Resource &other) const
{
	return impl == other.impl;
}

void CL_Resource::set_data(const CL_String &name, const CL_SharedPtr<CL_ResourceData> &ptr)
{
	std::vector<CL_ResourceCacheEntry>::size_type i;
	for (i = 0; i < impl->cache_objects.size(); i++)
	{
		if (impl->cache_objects[i].name == name)
		{
			impl->cache_objects[i].data = ptr;
			return;
		}
	}

	CL_ResourceCacheEntry data;
	data.name = name;
	data.data = ptr;
	data.reference_count = 0;
	impl->cache_objects.push_back(data);
}

void CL_Resource::clear_data(const CL_String &data_name)
{
	std::vector<CL_ResourceCacheEntry>::size_type i;
	for (i = 0; i < impl->cache_objects.size(); i++)
	{
		if (impl->cache_objects[i].name == data_name)
		{
			impl->cache_objects.erase(impl->cache_objects.begin() + i);
			return;
		}
	}
}

int CL_Resource::add_data_session(const CL_String &data_name)
{
	std::vector<CL_ResourceCacheEntry>::size_type i;
	for (i = 0; i < impl->cache_objects.size(); i++)
	{
		if (impl->cache_objects[i].name == data_name)
		{
			return ++impl->cache_objects[i].reference_count;
		}
	}

	CL_ResourceCacheEntry data;
	data.name = data_name;
	data.reference_count = 1;
	impl->cache_objects.push_back(data);
	return 1;
}

int CL_Resource::remove_data_session(const CL_String &data_name)
{
	std::vector<CL_ResourceCacheEntry>::size_type i;
	for (i = 0; i < impl->cache_objects.size(); i++)
	{
		if (impl->cache_objects[i].name == data_name)
		{
			if (impl->cache_objects[i].reference_count == 0)
				return 0;
			return --impl->cache_objects[i].reference_count;
		}
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Resource Implementation:
