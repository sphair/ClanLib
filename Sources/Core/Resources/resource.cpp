/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
#include "API/Core/XML/dom_element.h"
#include <memory>
#include <vector>

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// Resource_Impl Class:

class ResourceManager_Impl;

struct ResourceCacheEntry
{
	std::string name;

	std::shared_ptr<ResourceData> data;

	int reference_count;
};

class Resource_Impl
{
//! Attributes:
public:
	std::weak_ptr<ResourceManager_Impl> resource_manager;

	DomElement element;

	std::vector<ResourceCacheEntry> cache_objects;
};

/////////////////////////////////////////////////////////////////////////////
// Resource Construction:

Resource::Resource()
{
}

Resource::Resource(DomElement element, ResourceManager &resource_manager)
: impl(new Resource_Impl)
{
	impl->element = element;
	impl->resource_manager = std::weak_ptr<ResourceManager_Impl>(resource_manager.impl);
}

Resource::~Resource()
{
}

/////////////////////////////////////////////////////////////////////////////
// Resource Attributes:

std::string Resource::get_type() const
{
	return impl->element.get_local_name();
}

std::string Resource::get_name() const
{
	return impl->element.get_attribute("name");
}

DomElement &Resource::get_element()
{
	return impl->element;
}

ResourceManager Resource::get_manager()
{
	return ResourceManager(impl->resource_manager);
}

std::shared_ptr<ResourceData> Resource::get_data(const std::string &name)
{
	std::vector<ResourceCacheEntry>::size_type i;
	for (i = 0; i < impl->cache_objects.size(); i++)
		if (impl->cache_objects[i].name == name)
			return impl->cache_objects[i].data;
	return std::shared_ptr<ResourceData>();
}

int Resource::get_data_session_count(const std::string &data_name)
{
	std::vector<ResourceCacheEntry>::size_type i;
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
// Resource Operations:

bool Resource::operator ==(const Resource &other) const
{
	return impl == other.impl;
}

void Resource::set_data(const std::string &name, const std::shared_ptr<ResourceData> &ptr)
{
	std::vector<ResourceCacheEntry>::size_type i;
	for (i = 0; i < impl->cache_objects.size(); i++)
	{
		if (impl->cache_objects[i].name == name)
		{
			impl->cache_objects[i].data = ptr;
			return;
		}
	}

	ResourceCacheEntry data;
	data.name = name;
	data.data = ptr;
	data.reference_count = 0;
	impl->cache_objects.push_back(data);
}

void Resource::clear_data(const std::string &data_name)
{
	std::vector<ResourceCacheEntry>::size_type i;
	for (i = 0; i < impl->cache_objects.size(); i++)
	{
		if (impl->cache_objects[i].name == data_name)
		{
			impl->cache_objects.erase(impl->cache_objects.begin() + i);
			return;
		}
	}
}

int Resource::add_data_session(const std::string &data_name)
{
	std::vector<ResourceCacheEntry>::size_type i;
	for (i = 0; i < impl->cache_objects.size(); i++)
	{
		if (impl->cache_objects[i].name == data_name)
		{
			return ++impl->cache_objects[i].reference_count;
		}
	}

	ResourceCacheEntry data;
	data.name = data_name;
	data.reference_count = 1;
	impl->cache_objects.push_back(data);
	return 1;
}

int Resource::remove_data_session(const std::string &data_name)
{
	std::vector<ResourceCacheEntry>::size_type i;
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
// Resource Implementation:

}
