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

/// \addtogroup clanCore_Resources clanCore Resources
/// \{

#pragma once

#include "../api_core.h"
#include "../Resources/resource.h"
#include "../System/weakptr.h"
#include "../IOData/virtual_directory.h"
#include <vector>

class CL_IODevice;
class CL_Resource;
class CL_VirtualDirectory;
class CL_ResourceManager_Impl;

/// \brief Resource Manager.
///
/// \xmlonly !group=Core/Resources! !header=core.h! \endxmlonly
class CL_API_CORE CL_ResourceManager
{
/// \name Construction
/// \{
public:
	/// \brief Construct a resource manager.
	CL_ResourceManager();

	/// \brief Constructs a ResourceManager
	///
	/// \param filename = String
	CL_ResourceManager(const CL_String &filename);

	/// \brief Constructs a ResourceManager
	///
	/// \param filename = String
	/// \param directory = Virtual Directory
	CL_ResourceManager(const CL_String &filename, CL_VirtualDirectory directory);

	/// \brief Constructs a ResourceManager
	///
	/// \param file = IODevice
	/// \param directory = Virtual Directory
	CL_ResourceManager(CL_IODevice file, CL_VirtualDirectory directory = CL_VirtualDirectory());

	/// \brief Constructs a ResourceManager
	///
	/// \param other = Resource Manager
	CL_ResourceManager(const CL_ResourceManager &other);

	~CL_ResourceManager();

/// \}
/// \name Attributes
/// \{
public:
	/// \brief Returns true if a resource exists.
	bool resource_exists(const CL_String &resource_id) const;

	/// \brief Returns all the resource sections available.
	std::vector<CL_String> get_section_names() const;

	/// \brief Returns a list of all resources available.
	/** <p>The returned resources are in the form "section/subsection/.../resourcename".</p>*/
	std::vector<CL_String> get_resource_names() const;

	std::vector<CL_String> get_resource_names(const CL_String &section) const;

	/// \brief Returns a list of all resources available matching a given type.
	/** <p>The returned resources are in the form "section/subsection/.../resourcename".</p>*/
	std::vector<CL_String> get_resource_names_of_type(const CL_String &type) const;

	std::vector<CL_String> get_resource_names_of_type(
		const CL_String &type,
		const CL_String &section) const;

	/// \brief Returns CL_Resource representing the given resource.
	CL_Resource get_resource(
		const CL_String &resource_id,
		bool resolve_alias = true,
		int reserved = 0);

	/// \brief Returns the directory to load resource data from.
	CL_VirtualDirectory get_directory(const CL_Resource &resource) const;

	/// \brief Returns the value of a boolean resource. (using the value attribute)
	bool get_boolean_resource(
		const CL_String &resource_id,
		bool default_value);

	/// \brief Returns the value of an integer resource. (using the value attribute)
	int get_integer_resource(
		const CL_String &resource_id,
		int default_value);

	/// \brief Returns the value of an string resource. (using the value attribute)
	CL_String get_string_resource(
		const CL_String &resource_id,
		const CL_StringRef &default_value);

/// \}
/// \name Operations
/// \{
public:
	CL_ResourceManager &operator =(const CL_ResourceManager &copy);

	bool operator ==(const CL_ResourceManager &manager) const;

	/// \brief Set the resource data directory.
	void set_directory(const CL_VirtualDirectory &directory);

	/// \brief Add resources from an other resource manager.
	/** <p>This function only makes the resource manager search other managers, it
	    does not copy the resources into this manager.</p>*/
	void add_resources(const CL_ResourceManager& additional_resources);

	/// \brief Remove resources from an other resource manager.
	void remove_resources(const CL_ResourceManager& additional_resources);

	/// \brief Construct a new resource object.
	CL_Resource create_resource(const CL_String &resource_id, const CL_String &type);

	/// \brief Destroy resource object.
	void destroy_resource(const CL_String &resource_id);

	/// \brief Save resource XML tree to file.
	void save(const CL_String &filename);

	/// \brief Save
	///
	/// \param filename = the filename to save
	/// \param directory = Virtual Directory
	void save(const CL_String &filename, CL_VirtualDirectory directory);

	/// \brief Save
	///
	/// \param file = IODevice
	void save(CL_IODevice file);

	/// \brief Load resource XML tree from file.
	void load(const CL_String &filename);

	/// \brief Load
	///
	/// \param filename = the filename to save
	/// \param directory = Virtual Directory
	void load(const CL_String &filename, CL_VirtualDirectory directory);

	/// \brief Load
	///
	/// \param file = the file to load
	/// \param directory = Virtual Directory
	void load(CL_IODevice file, CL_VirtualDirectory directory = CL_VirtualDirectory());

/// \}
/// \name Implementation
/// \{
private:

	/// \brief Constructs a ResourceManager
	///
	/// \param CL_ResourceManager_Impl = Weak Ptr
	CL_ResourceManager(CL_WeakPtr<CL_ResourceManager_Impl> &impl);

	CL_SharedPtr<CL_ResourceManager_Impl> impl;

	friend class CL_Resource;
/// \}
};

/// \}
