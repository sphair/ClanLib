/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

/// \addtogroup clanCore_Resources clanCore Resources
/// \{

#pragma once

#include "../api_core.h"
#include "../IOData/file_system.h"
#include "resource_object.h"
#include <memory>
#include <map>

namespace clan
{

class ResourceManagerProvider;
class ResourceManager_Impl;

class ResourceManager
{
public:
	ResourceManager();
	ResourceManager(ResourceManagerProvider *provider);
	ResourceManager(const std::string &xml_filename, const FileSystem &file_system = FileSystem());

	bool is_null() const;

	ResourceManagerProvider *get_provider() const;

	template<typename Type>
	Resource<Type> get_resource(const std::string &id)
	{
		return get_resource(id).cast<Type>();
	}

	ResourceObject get_resource(const std::string &id);

private:
	std::shared_ptr<ResourceManager_Impl> impl;
};

}

/// \}
