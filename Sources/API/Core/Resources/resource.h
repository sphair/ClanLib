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

/// \addtogroup clanCore_Resources clanCore Resources
/// \{

#pragma once

#include "../api_core.h"
#include "../Resources/resource_manager.h"
#include <memory>

namespace clan
{

class DomElement;
class ResourceManager;
class Resource_Impl;

class ResourceData
{
public:
	virtual ~ResourceData() { }
};

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

	/// \brief Returns the object stored in the given data name.
	std::shared_ptr<ResourceData> get_data(const std::string &data_name);

	/// \brief Returns the number of ResourceDataSession objects using this resource.
	int get_data_session_count(const std::string &data_name);

/// \}
/// \name Operations
/// \{

public:
	/// \brief Compares this resource to another resource.
	bool operator ==(const Resource &other) const;

	/// \brief Store object in resource.
	void set_data(const std::string &data_name, const std::shared_ptr<ResourceData> &ptr);

	/// \brief Remove object stored with the given data name.
	void clear_data(const std::string &data_name);

	/// \brief Increment the usage session count for the resource.
	int add_data_session(const std::string &data_name);

	/// \brief Decrement the usage session count for the resource.
	int remove_data_session(const std::string &data_name);

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
