/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
#include "API/Core/Resources/resource.h"
#include "API/Core/IOData/virtual_directory.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/IOData/file.h"
#include "API/Core/IOData/virtual_file_system.h"
#include "API/Core/IOData/virtual_directory.h"
#include "API/Core/XML/dom_document.h"
#include "API/Core/XML/dom_element.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include <map>

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceManager_Impl Class:

class CL_ResourceManager_Impl
{
//! Attributes:
public:
	CL_VirtualDirectory directory;

	CL_DomDocument document;

	std::map<CL_String, CL_Resource> resources;

	std::vector<CL_ResourceManager> additional_resources;

	CL_String ns_resources;
};

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceManager Construction:

CL_ResourceManager::CL_ResourceManager()
: impl(new CL_ResourceManager_Impl)
{
	impl->ns_resources = "http://clanlib.org/xmlns/resources-1.0";
	CL_DomElement document_element = impl->document.create_element_ns(
		impl->ns_resources,
		"clres:resources");
	document_element.set_attribute_ns(
		"http://www.w3.org/2000/xmlns/",
		"xmlns:clres",
		impl->ns_resources);
	impl->document.append_child(document_element);
}

CL_ResourceManager::CL_ResourceManager(const CL_String &filename)
: impl(new CL_ResourceManager_Impl)
{
	load(filename);
}

CL_ResourceManager::CL_ResourceManager(const CL_String &filename, CL_VirtualDirectory directory)
: impl(new CL_ResourceManager_Impl)
{
	load(filename, directory);
}

CL_ResourceManager::CL_ResourceManager(CL_IODevice file, CL_VirtualDirectory directory)
: impl(new CL_ResourceManager_Impl)
{
	load(file, directory);
}

CL_ResourceManager::CL_ResourceManager(const CL_ResourceManager &other)
: impl(other.impl)
{
}

CL_ResourceManager::~CL_ResourceManager()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceManager Attributes:

bool CL_ResourceManager::resource_exists(const CL_String &resource_id) const
{
	std::map<CL_String, CL_Resource>::const_iterator it;
	it = impl->resources.find(resource_id);
	return (it != impl->resources.end());
}

std::vector<CL_String> CL_ResourceManager::get_section_names() const
{
	std::vector<CL_String> names;
	CL_String last_section;
	std::map<CL_String, CL_Resource>::const_iterator it;
	for (it = impl->resources.begin(); it != impl->resources.end(); ++it)
	{
		CL_String section = CL_PathHelp::get_fullpath(it->first, CL_PathHelp::path_type_virtual);
		if (section != last_section)
		{
			names.push_back(section);
			last_section = section;
		}
	}

	std::vector<CL_ResourceManager>::size_type i;
	for (i = 0; i < impl->additional_resources.size(); i++)
	{
		try
		{
			std::vector<CL_String> additional_names = impl->additional_resources[i].get_section_names();
			names.insert(names.end(), additional_names.begin(), additional_names.end());
		}
		catch (CL_Exception)
		{
		}
	}

	return names;
}

std::vector<CL_String> CL_ResourceManager::get_resource_names() const
{
	std::vector<CL_String> names;
	CL_String last_section;
	std::map<CL_String, CL_Resource>::const_iterator it;
	for (it = impl->resources.begin(); it != impl->resources.end(); ++it)
	{
		CL_String resource_id = it->first;
		names.push_back(resource_id);
	}
	return names;
}

std::vector<CL_String> CL_ResourceManager::get_resource_names(const CL_String &section) const
{
	std::vector<CL_String> names;
	std::map<CL_String, CL_Resource>::const_iterator it;
	for (it = impl->resources.begin(); it != impl->resources.end(); ++it)
	{
		CL_String cur_section = CL_PathHelp::get_basepath(it->first, CL_PathHelp::path_type_virtual);
		if (section == cur_section)
		{
			CL_String name = CL_PathHelp::get_filename(it->first, CL_PathHelp::path_type_virtual);
			names.push_back(name);
		}
	}

	std::vector<CL_ResourceManager>::size_type i;
	for (i = 0; i < impl->additional_resources.size(); i++)
	{
		try
		{
			std::vector<CL_String> additional_names = impl->additional_resources[i].get_resource_names(section);
			names.insert(names.end(), additional_names.begin(), additional_names.end());
		}
		catch (CL_Exception)
		{
		}
	}

	return names;
}

std::vector<CL_String> CL_ResourceManager::get_resource_names_of_type(const CL_String &type) const
{
	std::vector<CL_String> names;
	std::map<CL_String, CL_Resource>::const_iterator it;
	for (it = impl->resources.begin(); it != impl->resources.end(); ++it)
	{
		if (it->second.get_type() == type)
			names.push_back(it->first);
	}
	return names;
}

std::vector<CL_String> CL_ResourceManager::get_resource_names_of_type(
	const CL_String &type,
	const CL_String &section) const
{
	std::vector<CL_String> names;
	std::map<CL_String, CL_Resource>::const_iterator it;
	for (it = impl->resources.begin(); it != impl->resources.end(); ++it)
	{
		CL_String cur_section = CL_PathHelp::get_filename(it->first, CL_PathHelp::path_type_virtual);
		if (section == cur_section)
		{
			if (it->second.get_type() == type)
			{
				CL_String name = CL_PathHelp::get_filename(it->first, CL_PathHelp::path_type_virtual);
				names.push_back(name);
			}
		}
	}
	return names;
}

CL_Resource CL_ResourceManager::get_resource(
	const CL_String &resource_id,
	bool resolve_alias,
	int reserved)
{
 	std::map<CL_String, CL_Resource>::const_iterator it;
	it = impl->resources.find(resource_id);
	if (it != impl->resources.end())
		return it->second;

	std::vector<CL_ResourceManager>::size_type i;
	for (i = 0; i < impl->additional_resources.size(); i++)
	{
		try
		{
			return impl->additional_resources[i].get_resource(
				resource_id, resolve_alias, reserved);
		}
		catch (CL_Exception)
		{
		}
	}

	throw CL_Exception(cl_format("Resource not found: %1", resource_id));
	return CL_Resource(impl->document.get_document_element(), *this);
}

CL_VirtualDirectory CL_ResourceManager::get_directory(const CL_Resource &resource) const
{
	CL_Resource resource_const_hack = resource;
	if (resource_const_hack.get_manager().impl == impl)
	{
		return impl->directory;
	}
	else
	{
		return resource_const_hack.get_manager().get_directory(resource);
	}
}

bool CL_ResourceManager::get_boolean_resource(
	const CL_String &resource_id,
	bool default_value)
{
	if (!resource_exists(resource_id))
		return default_value;

	CL_Resource resource = get_resource(resource_id);
	return CL_StringHelp::text_to_bool(resource.get_element().get_attribute("value"));
}

int CL_ResourceManager::get_integer_resource(
	const CL_String &resource_id,
	int default_value)
{
	if (!resource_exists(resource_id))
		return default_value;

	CL_Resource resource = get_resource(resource_id);
	return CL_StringHelp::text_to_int(resource.get_element().get_attribute("value"));
}

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceManager Operations:

CL_ResourceManager &CL_ResourceManager::operator =(const CL_ResourceManager &copy)
{
	impl = copy.impl;
	return *this;
}

bool CL_ResourceManager::operator ==(const CL_ResourceManager &manager) const
{
	return impl == manager.impl;
}

void CL_ResourceManager::add_resources(const CL_ResourceManager& additional_resources)
{
	impl->additional_resources.push_back(additional_resources);
}

void CL_ResourceManager::remove_resources(const CL_ResourceManager& additional_resources)
{
	std::vector<CL_ResourceManager>::size_type i;
	for (i = 0; i < impl->additional_resources.size(); i++)
	{
		if (impl->additional_resources[i] == additional_resources)
		{
			impl->additional_resources.erase(impl->additional_resources.begin() + i);
			break;
		}
	}
}

CL_Resource CL_ResourceManager::create_resource(const CL_String &resource_id, const CL_String &type)
{
	if (resource_exists(resource_id))
		throw CL_Exception(cl_format("Resource %1 already exists", resource_id));

	std::vector<CL_String> path_elements = CL_PathHelp::split_basepath(resource_id);
	CL_String name = CL_PathHelp::get_filename(resource_id);

	// Walk tree as deep as we can get:
	CL_DomNode parent = impl->document.get_document_element();
	CL_DomNode cur = parent.get_first_child();
	std::vector<CL_String>::iterator path_it = path_elements.begin();
	while (!cur.is_null() && path_it != path_elements.end())
	{
		if (cur.is_element() &&
			cur.get_namespace_uri() == impl->ns_resources &&
			cur.get_local_name() == "section")
		{
			CL_DomElement element = cur.to_element();
			CL_String name = element.get_attribute_ns(impl->ns_resources, "name");
			if (name == *path_it)
			{
				++path_it;
				parent = cur;
				cur = cur.get_first_child();
				continue;
			}
		}
		cur = cur.get_next_sibling();
	}

	// Create any missing parent nodes:
	CL_String prefix = parent.get_prefix();
	while (path_it != path_elements.end())
	{
		CL_DomElement section;
		if (prefix.empty())
		{
			section = impl->document.create_element_ns( impl->ns_resources, (*path_it) );
		}
		else
		{
			section = impl->document.create_element_ns( impl->ns_resources,(prefix + ":" + *path_it));
		}
		parent.append_child(section);
		parent = section;
		++path_it;
	}

	// Create node:
	CL_DomElement resource_node;
	if (prefix.empty())
	{
		resource_node = impl->document.create_element_ns( impl->ns_resources, (type));
	}
	else
	{
		resource_node = impl->document.create_element_ns( impl->ns_resources,(prefix + ":" + type));
	}

	resource_node.set_attribute_ns(
		impl->ns_resources,
		prefix.empty() ? "name" : (prefix + ":name"),
		name);
	parent.append_child(resource_node);

	// Create resource:
	impl->resources[resource_id] = CL_Resource(resource_node, *this);
	return impl->resources[resource_id];
}

void CL_ResourceManager::destroy_resource(const CL_String &resource_id)
{
	std::map<CL_String, CL_Resource>::iterator it;
	it = impl->resources.find(resource_id);
	if (it == impl->resources.end())
		return;
	CL_DomNode cur = it->second.get_element();
	impl->resources.erase(it);
	CL_DomNode parent = cur.get_parent_node();
	while (!parent.is_null())
	{
		parent.remove_child(cur);
		if (parent.has_child_nodes())
			break;
		cur = parent;
		parent = parent.get_parent_node();
	}
}

void CL_ResourceManager::save(const CL_String &filename)
{
	CL_File file(filename, CL_File::create_always, CL_File::access_read_write);
	save(file);
}

void CL_ResourceManager::save(const CL_String &filename, CL_VirtualDirectory directory)
{
	save(directory.open_file(filename, CL_File::create_always, CL_File::access_read|CL_File::access_write, CL_File::share_read));
}

void CL_ResourceManager::save(CL_IODevice file)
{
	impl->document.save(file);
}

void CL_ResourceManager::load(const CL_String &fullname)
{
	CL_String path = CL_PathHelp::get_fullpath(fullname, CL_PathHelp::path_type_file);
	CL_String filename = CL_PathHelp::get_filename(fullname, CL_PathHelp::path_type_file);
	CL_VirtualFileSystem vfs(path);
	load(filename, vfs.get_root_directory());
}

void CL_ResourceManager::load(const CL_String &fullname, CL_VirtualDirectory directory)
{
	CL_String path = CL_PathHelp::get_fullpath(fullname, CL_PathHelp::path_type_virtual);
	CL_String filename = CL_PathHelp::get_filename(fullname, CL_PathHelp::path_type_virtual);
	CL_VirtualDirectory dir = directory.open_directory(path);
	load(dir.open_file(filename, CL_File::open_existing, CL_File::access_read, CL_File::share_read), dir);
}

void CL_ResourceManager::load(CL_IODevice file, CL_VirtualDirectory directory)
{
	CL_DomDocument new_document;
	new_document.load(file);

	// Check if loaded document uses namespaces and if its a clanlib resources xml document:
	CL_DomElement doc_element = new_document.get_document_element();
	if (doc_element.get_namespace_uri().empty() && doc_element.get_local_name() == "resources")
	{
		impl->ns_resources = CL_String();
	}
	else if (doc_element.get_namespace_uri() == "http://clanlib.org/xmlns/resources-1.0")
	{
		if (doc_element.get_local_name() != "resources")
			throw CL_Exception("ClanLib resource documents must begin with a resources element.");

		impl->ns_resources = "http://clanlib.org/xmlns/resources-1.0";
	}
	else
	{
		throw CL_Exception("XML document is not a ClanLib resources document.");
	}

	impl->document = new_document;
	impl->directory = directory;
	impl->resources.clear();

	std::vector<CL_String> section_stack;
	std::vector<CL_DomNode> nodes_stack;
	section_stack.push_back(CL_String());
	nodes_stack.push_back(doc_element.get_first_child());
	while (!nodes_stack.empty())
	{
		if (nodes_stack.back().is_element())
		{
			CL_DomElement element = nodes_stack.back().to_element();
			if (element.get_namespace_uri() == impl->ns_resources && element.get_local_name() == "section")
			{
				CL_String section_name = element.get_attribute_ns(impl->ns_resources, "name");
				section_stack.push_back(section_stack.back() + CL_PathHelp::add_trailing_slash(section_name, CL_PathHelp::path_type_virtual));
				nodes_stack.push_back(element.get_first_child());
				continue;
			}
			else if (element.has_attribute_ns(impl->ns_resources, "name"))
			{
				CL_String resource_name = element.get_attribute_ns(impl->ns_resources, "name");
				CL_String resource_id = section_stack.back() + resource_name;
				impl->resources[resource_id] = CL_Resource(element, *this);
			}
		}

		nodes_stack.back() = nodes_stack.back().get_next_sibling();

		while (nodes_stack.back().is_null())
		{
			nodes_stack.pop_back();
			section_stack.pop_back();
			if (nodes_stack.empty())
				break;
			nodes_stack.back() = nodes_stack.back().get_next_sibling();
		}
	}
}

void CL_ResourceManager::set_directory(const CL_VirtualDirectory &directory)
{
	impl->directory = directory;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceManager Implementation:

CL_ResourceManager::CL_ResourceManager(CL_WeakPtr<CL_ResourceManager_Impl> &impl) : impl(impl.to_sharedptr())
{
}
