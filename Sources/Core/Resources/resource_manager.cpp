/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
#include "API/Core/Resources/resource_manager.h"
#include "API/Core/Text/string_format.h"
#include "resource_manager_impl.h"

namespace clan
{
	ResourceManager::ResourceManager()
		: impl(std::make_shared<ResourceManager_Impl>())
	{
	}

	ResourceManager::~ResourceManager()
	{
	}

	UserDataOwner &ResourceManager::get_cache_owner(const std::string &name) const
	{
		auto it = impl->caches.find(name);
		if (it == impl->caches.end())
			throw Exception(string_format("ResourceManager has no cache named %1", name));
		return *it->second.get();
	}

	UserDataOwner &ResourceManager::set_cache_owner(const std::string &name)
	{
		std::shared_ptr<UserDataOwner> &cache = impl->caches[name];
		if (!cache)
			cache = std::shared_ptr<UserDataOwner>(new UserDataOwner());
		return *cache.get();
	}
}
