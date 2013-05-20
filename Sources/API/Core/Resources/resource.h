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
#include "../Resources/resource_manager.h"
#include "../IOData/file_system.h"
#include <memory>

namespace clan
{

class DomElement;
class ResourceManager;
class Resource_Impl;

/// \brief Resource Manager resource.
class CL_API_CORE Resource
{
/// \name Construction
/// \{

public:
	Resource();

	~Resource();

/// \}
/// \name Attributes
/// \{

public:
	/// \brief Returns the resource type.
	std::string get_type() const;

	/// \brief Returns the name of the resource.
	std::string get_name() const;

	/// \brief Returns the DOM element describing the resource.
	DomElement &get_element();

	/// \brief Returns the resource manager owning the resource.
	ResourceManager get_manager();

	/// \brief Returns the file system to load resource from.
	FileSystem get_file_system() const;

	/// \brief Returns the base path of the resource.
	std::string get_base_path() const;

/// \}
/// \name Operations
/// \{

public:
	/// \brief Opens a file relative to the resource base path.
	/** param: mode = File::OpenMode modes
	    param: access = File::AccessFlags flags
	    param: share = File::ShareFlags flags
	    param: flags = File::Flags flags
	    \return The IODevice*/
	IODevice open_file(const std::string &filename,
		File::OpenMode mode = File::open_existing,
		unsigned int access = File::access_read,
		unsigned int share = File::share_all,
		unsigned int flags = 0) const;

	/// \brief Compares this resource to another resource.
	bool operator ==(const Resource &other) const;

/// \}
/// \name Implementation
/// \{

private:

	/// \brief Constructs a Resource
	///
	/// \param element = Dom Element
	/// \param resource_manager = Resource Manager
	Resource(DomElement element, ResourceManager &resource_manager);

	std::shared_ptr<Resource_Impl> impl;

	friend class ResourceManager;
/// \}
};

}

/// \}
