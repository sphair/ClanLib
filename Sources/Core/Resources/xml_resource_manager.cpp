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
#include "API/Core/Resources/xml_resource_manager.h"
#include "API/Core/Resources/xml_resource_document.h"

namespace clan
{
	namespace
	{
		std::vector<std::function<void(ResourceManager &, const XMLResourceDocument &)>> xml_cache_factories;
	}

	ResourceManager XMLResourceManager::create(const XMLResourceDocument &doc)
	{
		ResourceManager manager;
		manager.set_cache<XMLResourceDocument>("clan.xmldoc", std::shared_ptr<XMLResourceDocument>(new XMLResourceDocument(doc)));
		for (auto & xml_cache_factory : xml_cache_factories)
		{
			xml_cache_factory(manager, doc);
		}
		return manager;
	}

	void XMLResourceManager::add_cache_factory(std::function<void(ResourceManager &, const XMLResourceDocument &)> factory_callback)
	{
		xml_cache_factories.push_back(factory_callback);
	}

	XMLResourceDocument &XMLResourceManager::get_doc(const ResourceManager &manager)
	{
		return *manager.get_cache<XMLResourceDocument>("clan.xmldoc").get();
	}
}
