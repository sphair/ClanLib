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
#include "../Resources/resource.h"
#include "../IOData/virtual_directory.h"
#include <vector>
#include <memory>

namespace clan
{

class IODevice;
class Resource;
class VirtualDirectory;
class ResourceManager_Impl;

/// \brief Resource Manager.
class CL_API_CORE ResourceManager
{
/// \name Construction
/// \{
public:
	/// \brief Construct a resource manager.
	ResourceManager();

	/// \brief Constructs a ResourceManager
	///
	/// \param filename = String
	ResourceManager(const std::string &filename);

	/// \brief Constructs a ResourceManager
	///
	/// \param filename = String
	/// \param directory = Virtual Directory
	ResourceManager(const std::string &filename, VirtualDirectory directory);

	/// \brief Constructs a ResourceManager
	///
	/// \param file = IODevice
	/// \param directory = Virtual Directory
	ResourceManager(IODevice file, VirtualDirectory directory = VirtualDirectory());

	/// \brief Constructs a ResourceManager
	///
	/// \param other = Resource Manager
	ResourceManager(const ResourceManager &other);

	~ResourceManager();

/// \}
/// \name Attributes
/// \{
public:
	/// \brief Returns true if a resource exists.
	bool resource_exists(const std::string &resource_id) const;

	/// \brief Returns all the resource sections available.
	std::vector<std::string> get_section_names() const;

	/// \brief Returns a list of all resources available.
	/** <p>The returned resources are in the form "section/subsection/.../resourcename".</p>*/
	std::vector<std::string> get_resource_names() const;

	std::vector<std::string> get_resource_names(const std::string &section) const;

	/// \brief Returns a list of all resources available matching a given type.
	/** <p>The returned resources are in the form "section/subsection/.../resourcename".</p>*/
	std::vector<std::string> get_resource_names_of_type(const std::string &type) const;

	std::vector<std::string> get_resource_names_of_type(
		const std::string &type,
		const std::string &section) const;

	/// \brief Returns Resource representing the given resource.
	Resource get_resource(
		const std::string &resource_id,
		bool resolve_alias = true,
		int reserved = 0);

	/// \brief Returns the directory to load resource data from.
	VirtualDirectory get_directory(const Resource &resource) const;

	/// \brief Returns the value of a boolean resource. (using the value attribute)
	bool get_boolean_resource(
		const std::string &resource_id,
		bool default_value);

	/// \brief Returns the value of an integer resource. (using the value attribute)
	int get_integer_resource(
		const std::string &resource_id,
		int default_value);

	/// \brief Returns the value of an string resource. (using the value attribute)
	std::string get_string_resource(
		const std::string &resource_id,
		const std::string &default_value);

/// \}
/// \name Operations
/// \{
public:
	ResourceManager &operator =(const ResourceManager &copy);

	bool operator ==(const ResourceManager &manager) const;

	/// \brief Set the resource data directory.
	void set_directory(const VirtualDirectory &directory);

	/// \brief Add resources from an other resource manager.
	/** <p>This function only makes the resource manager search other managers, it
	    does not copy the resources into this manager.</p>*/
	void add_resources(const ResourceManager& additional_resources);

	/// \brief Remove resources from an other resource manager.
	void remove_resources(const ResourceManager& additional_resources);

	/// \brief Construct a new resource object.
	Resource create_resource(const std::string &resource_id, const std::string &type);

	/// \brief Destroy resource object.
	void destroy_resource(const std::string &resource_id);

	/// \brief Save resource XML tree to file.
	void save(const std::string &filename);

	/// \brief Save
	///
	/// \param filename = the filename to save
	/// \param directory = Virtual Directory
	void save(const std::string &filename, VirtualDirectory directory);

	/// \brief Save
	///
	/// \param file = IODevice
	void save(IODevice file);

	/// \brief Load resource XML tree from file.
	void load(const std::string &filename);

	/// \brief Load
	///
	/// \param filename = the filename to save
	/// \param directory = Virtual Directory
	void load(const std::string &filename, VirtualDirectory directory);

	/// \brief Load
	///
	/// \param file = the file to load
	/// \param directory = Virtual Directory
	void load(IODevice file, VirtualDirectory directory = VirtualDirectory());

/// \}
/// \name Implementation
/// \{
private:

	/// \brief Constructs a ResourceManager
	///
	/// \param ResourceManager_Impl = Weak Ptr
	ResourceManager(std::weak_ptr<ResourceManager_Impl> &impl);

	std::shared_ptr<ResourceManager_Impl> impl;

	friend class Resource;
/// \}
};

}

/// \}
