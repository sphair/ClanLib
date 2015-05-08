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
#include "API/Core/Resources/xml_resource_document.h"
#include "API/Core/Resources/xml_resource_node.h"
#include "API/Core/IOData/file_system.h"
#include "API/Core/IOData/path_help.h"
#include "API/Core/IOData/file.h"
#include "API/Core/XML/dom_document.h"
#include "API/Core/XML/dom_element.h"
#include "API/Core/Text/string_format.h"
#include "API/Core/Text/string_help.h"
#include "xml_resource_document_impl.h"
#include <map>

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// XMLResourceDocument Construction:

XMLResourceDocument::XMLResourceDocument()
: impl(std::make_shared<XMLResourceDocument_Impl>())
{
	impl->ns_resources = "http://clanlib.org/xmlns/resources-1.0";
	DomElement document_element = impl->document.create_element_ns(
		impl->ns_resources,
		"clres:resources");
	document_element.set_attribute_ns(
		"http://www.w3.org/2000/xmlns/",
		"xmlns:clres",
		impl->ns_resources);
	impl->document.append_child(document_element);
}

XMLResourceDocument::XMLResourceDocument(const std::string &filename)
: impl(std::make_shared<XMLResourceDocument_Impl>())
{
	load(filename);
}

XMLResourceDocument::XMLResourceDocument(const std::string &filename, FileSystem fs)
: impl(std::make_shared<XMLResourceDocument_Impl>())
{
	load(filename, fs);
}

XMLResourceDocument::XMLResourceDocument(IODevice file, const std::string &base_path, FileSystem fs)
: impl(std::make_shared<XMLResourceDocument_Impl>())
{
	load(file, base_path, fs);
}

XMLResourceDocument::XMLResourceDocument(const XMLResourceDocument &other)
: impl(other.impl)
{
}

XMLResourceDocument::~XMLResourceDocument()
{
}

/////////////////////////////////////////////////////////////////////////////
// XMLResourceDocument Attributes:

bool XMLResourceDocument::resource_exists(const std::string &resource_id) const
{
	std::map<std::string, XMLResourceNode>::const_iterator it;
	it = impl->resources.find(resource_id);
	if (it != impl->resources.end())
		return true;

	for (std::vector<XMLResourceDocument>::const_iterator it = impl->additional_resources.begin();
		it != impl->additional_resources.end();
		++it)
		if ((*it).resource_exists(resource_id))
			return true;
	return false;
}

std::vector<std::string> XMLResourceDocument::get_section_names() const
{
	std::vector<std::string> names;
	std::string last_section;
	std::map<std::string, XMLResourceNode>::const_iterator it;
	for (it = impl->resources.begin(); it != impl->resources.end(); ++it)
	{
		std::string section = PathHelp::get_fullpath(it->first, PathHelp::path_type_virtual);
		if (section != last_section)
		{
			names.push_back(section);
			last_section = section;
		}
	}

	std::vector<XMLResourceDocument>::size_type i;
	for (i = 0; i < impl->additional_resources.size(); i++)
	{
		try
		{
			std::vector<std::string> additional_names = impl->additional_resources[i].get_section_names();
			names.insert(names.end(), additional_names.begin(), additional_names.end());
		}
		catch (const Exception&)
		{
		}
	}

	return names;
}

std::vector<std::string> XMLResourceDocument::get_resource_names() const
{
	std::vector<std::string> names;
	std::map<std::string, XMLResourceNode>::const_iterator it;
	for (it = impl->resources.begin(); it != impl->resources.end(); ++it)
	{
		std::string resource_id = it->first;
		names.push_back(resource_id);
	}
	return names;
}

std::vector<std::string> XMLResourceDocument::get_resource_names(const std::string &section) const
{
	std::vector<std::string> names;
	std::map<std::string, XMLResourceNode>::const_iterator it;
	for (it = impl->resources.begin(); it != impl->resources.end(); ++it)
	{
		std::string cur_section = PathHelp::get_basepath(it->first, PathHelp::path_type_virtual);
		if (section == cur_section)
		{
			std::string name = PathHelp::get_filename(it->first, PathHelp::path_type_virtual);
			names.push_back(name);
		}
	}

	std::vector<XMLResourceDocument>::size_type i;
	for (i = 0; i < impl->additional_resources.size(); i++)
	{
		try
		{
			std::vector<std::string> additional_names = impl->additional_resources[i].get_resource_names(section);
			names.insert(names.end(), additional_names.begin(), additional_names.end());
		}
		catch (const Exception&)
		{
		}
	}

	return names;
}

std::vector<std::string> XMLResourceDocument::get_resource_names_of_type(const std::string &type) const
{
	std::vector<std::string> names;
	std::map<std::string, XMLResourceNode>::const_iterator it;
	for (it = impl->resources.begin(); it != impl->resources.end(); ++it)
	{
		if (it->second.get_type() == type)
			names.push_back(it->first);
	}
	return names;
}

std::vector<std::string> XMLResourceDocument::get_resource_names_of_type(
	const std::string &type,
	const std::string &section) const
{

	std::string section_trailing_slash = PathHelp::add_trailing_slash(section, PathHelp::path_type_virtual);

	std::vector<std::string> names;
	std::map<std::string, XMLResourceNode>::const_iterator it;
	for (it = impl->resources.begin(); it != impl->resources.end(); ++it)
	{
		std::string cur_section = PathHelp::get_fullpath(it->first, PathHelp::path_type_virtual);
		if (section_trailing_slash == cur_section)
		{
			if (it->second.get_type() == type)
				names.push_back(it->first);
		}
	}
	return names;
}

XMLResourceNode XMLResourceDocument::get_resource(
	const std::string &resource_id) const
{
	XMLResourceNode node = impl->get_resource(resource_id);
	if (node.is_null())
		throw Exception(string_format("Resource not found: %1", resource_id));
	return node;
}

XMLResourceNode XMLResourceDocument_Impl::get_resource(const std::string &resource_id) const
{
 	std::map<std::string, XMLResourceNode>::const_iterator it;
	it = resources.find(resource_id);
	if (it != resources.end())
		return it->second;

	std::vector<XMLResourceDocument>::size_type i;
	for (i = 0; i < additional_resources.size(); i++)
	{
		XMLResourceNode node = additional_resources[i].impl->get_resource(resource_id);
		if (!node.is_null())
			return node;
	}

	return XMLResourceNode();
}

bool XMLResourceDocument::get_boolean_resource(
	const std::string &resource_id,
	bool default_value) const
{
	if (!resource_exists(resource_id))
		return default_value;

	XMLResourceNode resource = get_resource(resource_id);
	return StringHelp::text_to_bool(resource.get_element().get_attribute("value"));
}

int XMLResourceDocument::get_integer_resource(
	const std::string &resource_id,
	int default_value) const
{
	if (!resource_exists(resource_id))
		return default_value;

	XMLResourceNode resource = get_resource(resource_id);
	return StringHelp::text_to_int(resource.get_element().get_attribute("value"));
}

std::string XMLResourceDocument::get_string_resource(
	const std::string &resource_id,
	const std::string &default_value) const
{
	if (!resource_exists(resource_id))
		return default_value;

	XMLResourceNode resource = get_resource(resource_id);
	return resource.get_element().get_attribute("value");
}

/////////////////////////////////////////////////////////////////////////////
// XMLResourceDocument Operations:

XMLResourceDocument &XMLResourceDocument::operator =(const XMLResourceDocument &copy)
{
	impl = copy.impl;
	return *this;
}

bool XMLResourceDocument::operator ==(const XMLResourceDocument &that) const
{
	return impl == that.impl;
}

void XMLResourceDocument::add_resources(const XMLResourceDocument& additional_resources)
{
	impl->additional_resources.push_back(additional_resources);
}

void XMLResourceDocument::remove_resources(const XMLResourceDocument& additional_resources)
{
	std::vector<XMLResourceDocument>::size_type i;
	for (i = 0; i < impl->additional_resources.size(); i++)
	{
		if (impl->additional_resources[i] == additional_resources)
		{
			impl->additional_resources.erase(impl->additional_resources.begin() + i);
			break;
		}
	}
}

XMLResourceNode XMLResourceDocument::create_resource(const std::string &resource_id, const std::string &type)
{
	if (resource_exists(resource_id))
		throw Exception(string_format("Resource %1 already exists", resource_id));

	std::vector<std::string> path_elements = PathHelp::split_basepath(resource_id);
	std::string name = PathHelp::get_filename(resource_id);

	// Walk tree as deep as we can get:
	DomNode parent = impl->document.get_document_element();
	DomNode cur = parent.get_first_child();
	auto path_it = path_elements.begin();
	while (!cur.is_null() && path_it != path_elements.end())
	{
		if (cur.is_element() &&
			cur.get_namespace_uri() == impl->ns_resources &&
			cur.get_local_name() == "section")
		{
			DomElement element = cur.to_element();
			std::string name = element.get_attribute_ns(impl->ns_resources, "name");
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
	std::string prefix = parent.get_prefix();
	while (path_it != path_elements.end())
	{
		DomElement section;
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
	DomElement resource_node;
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
	impl->resources[resource_id] = XMLResourceNode(resource_node, *this);
	return impl->resources[resource_id];
}

void XMLResourceDocument::destroy_resource(const std::string &resource_id)
{
	std::map<std::string, XMLResourceNode>::iterator it;
	it = impl->resources.find(resource_id);
	if (it == impl->resources.end())
		return;
	DomNode cur = it->second.get_element();
	impl->resources.erase(it);
	DomNode parent = cur.get_parent_node();
	while (!parent.is_null())
	{
		parent.remove_child(cur);
		if (parent.has_child_nodes())
			break;
		cur = parent;
		parent = parent.get_parent_node();
	}
}

void XMLResourceDocument::save(const std::string &filename)
{
	File file(filename, File::create_always, File::access_read_write);
	save(file);
}

void XMLResourceDocument::save(const std::string &filename, const FileSystem &fs)
{
	save(fs.open_file(filename, File::create_always, File::access_read_write, File::share_read));
}

void XMLResourceDocument::save(IODevice file)
{
	impl->document.save(file);
}

void XMLResourceDocument::load(const std::string &fullname)
{
	std::string path = PathHelp::get_fullpath(fullname, PathHelp::path_type_file);
	std::string filename = PathHelp::get_filename(fullname, PathHelp::path_type_file);
	FileSystem vfs(path);
	load(filename, vfs);
}

void XMLResourceDocument::load(const std::string &fullname, const FileSystem &fs)
{
	std::string path = PathHelp::get_fullpath(fullname, PathHelp::path_type_virtual);
	std::string filename = PathHelp::get_filename(fullname, PathHelp::path_type_virtual);
	load(fs.open_file(fullname, File::open_existing, File::access_read, File::share_read), path, fs);
}

void XMLResourceDocument::load(IODevice file, const std::string &base_path, const FileSystem &fs)
{
	DomDocument new_document;
	new_document.load(file);

	// Check if loaded document uses namespaces and if its a clanlib resources xml document:
	DomElement doc_element = new_document.get_document_element();
	if (doc_element.get_namespace_uri().empty() && doc_element.get_local_name() == "resources")
	{
		impl->ns_resources = std::string();
	}
	else if (doc_element.get_namespace_uri() == "http://clanlib.org/xmlns/resources-1.0")
	{
		if (doc_element.get_local_name() != "resources")
			throw Exception("ClanLib resource documents must begin with a resources element.");

		impl->ns_resources = "http://clanlib.org/xmlns/resources-1.0";
	}
	else
	{
		throw Exception("XML document is not a ClanLib resources document.");
	}

	impl->document = new_document;
	impl->fs = fs;
	impl->base_path = base_path;
	impl->resources.clear();

	std::vector<std::string> section_stack;
	std::vector<DomNode> nodes_stack;
	section_stack.push_back(std::string());
	nodes_stack.push_back(doc_element.get_first_child());
	while (!nodes_stack.empty())
	{
		if (nodes_stack.back().is_element())
		{
			DomElement element = nodes_stack.back().to_element();
			if (element.get_namespace_uri() == impl->ns_resources && element.get_local_name() == "section")
			{
				std::string section_name = element.get_attribute_ns(impl->ns_resources, "name");
				section_stack.push_back(section_stack.back() + PathHelp::add_trailing_slash(section_name, PathHelp::path_type_virtual));
				nodes_stack.push_back(element.get_first_child());
				continue;
			}
			else if (element.has_attribute_ns(impl->ns_resources, "name"))
			{
				std::string resource_name = element.get_attribute_ns(impl->ns_resources, "name");
				std::string resource_id = section_stack.back() + resource_name;
				impl->resources[resource_id] = XMLResourceNode(element, *this);
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

/////////////////////////////////////////////////////////////////////////////
// XMLResourceDocument Implementation:

XMLResourceDocument::XMLResourceDocument(std::weak_ptr<XMLResourceDocument_Impl> &impl) : impl(impl.lock())
{
}

}
