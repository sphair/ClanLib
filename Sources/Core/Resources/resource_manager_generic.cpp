/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#include <iterator>
#include "Core/precomp.h"
#include "resource_manager_generic.h"
#include "resource_generic.h"
#include "API/Core/IOData/inputsource_provider_file.h"
#include "API/Core/System/error.h"
#include "API/Core/System/clanstring.h"
#include "API/Core/IOData/zip_archive.h"
#include "API/Core/XML/dom_node_list.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceManager_Generic Construction:

CL_ResourceManager_Generic::CL_ResourceManager_Generic() : provider(0), delete_provider(false)
{
}

CL_ResourceManager_Generic::~CL_ResourceManager_Generic()
{
	if (delete_provider) delete provider;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceManager_Generic Attributes:

bool CL_ResourceManager_Generic::exists(const std::string &res_id)
{
	if (resources.find(res_id) != resources.end()) return true;

	// Search in all additional resources:
	std::list<CL_ResourceManager>::iterator it;
	for (it = additional_resources.begin(); it != additional_resources.end(); ++it)
	{
		if (it->exists(res_id)) return true;
	}

	return false;
}

CL_Resource &CL_ResourceManager_Generic::get_resource(const std::string &res_id)
{
	std::map<std::string, CL_Resource>::iterator it = resources.find(res_id);
	if (it == resources.end())
	{
		std::list<CL_ResourceManager>::iterator it2;
		for (it2 = additional_resources.begin(); it2 != additional_resources.end(); ++it2)
		{
			CL_ResourceManager &additional_manager = *it2;
			if (additional_manager.exists(res_id))
			{
				return additional_manager.get_resource(res_id, false);
			}
		}

		throw CL_Error(CL_String::format("Resource '%1' not found", res_id));
	}
	return it->second;
}

std::vector<std::string> CL_ResourceManager_Generic::get_all_resources(const std::string &original_section_name)
{
	std::vector<std::string> result;

	// Make sure we have a trailing / in section
	std::string section_name = add_trailing_slash(original_section_name);

	// Add local resources:
	{
		std::map<std::string, CL_Resource>::iterator it;
		for (it = resources.begin(); it != resources.end(); ++it)
		{
			if (it->first.substr(0, section_name.length()) == section_name)
			{
				result.push_back(it->first);
			}
		}
	}

	// Add resources for all additional resources:
	{
		std::list<CL_ResourceManager>::iterator it;
		for (it = additional_resources.begin(); it != additional_resources.end(); ++it)
		{
			std::vector<std::string> additionals = it->get_all_resources(section_name);
			std::copy(additionals.begin(), additionals.end(), std::back_inserter(result));
		}
	}

	return result;
}

std::vector<std::string> CL_ResourceManager_Generic::get_all_resources()
{
	std::vector<std::string> result;

	// Add local resources:
	{
		std::map<std::string, CL_Resource>::iterator it;
		for (it = resources.begin(); it != resources.end(); ++it)
		{
			result.push_back(it->first);
		}
	}

	// Add resources for all additional resources:
	{
		std::list<CL_ResourceManager>::iterator it;
		for (it = additional_resources.begin(); it != additional_resources.end(); ++it)
		{
			std::vector<std::string> additionals = it->get_all_resources();
			std::copy(additionals.begin(), additionals.end(), std::back_inserter(result));
		}
	}

	return result;
}

std::vector<std::string> CL_ResourceManager_Generic::get_all_sections()
{
	std::vector<std::string> result;

	// Add local sections:
	{
		CL_DomElement element = document.named_item("resources").to_element();
		add_sections(result, element);
	}

	// Add sections for all additional resources:
	{
		std::list<CL_ResourceManager>::iterator it;
		for (it = additional_resources.begin(); it != additional_resources.end(); ++it)
		{
			std::vector<std::string> additionals = it->get_all_sections();
			std::copy(additionals.begin(), additionals.end(), std::back_inserter(result));
		}
	}

	return result;
}

std::vector<std::string> CL_ResourceManager_Generic::get_sections(const std::string &section_name)
{
	std::vector<std::string> result;

	// Add local sections:
	{
		CL_DomElement element = document.named_item("resources").to_element();
		add_section(result, element, section_name);
	}

	// Add sections for all additional resources:
	{
		std::list<CL_ResourceManager>::iterator it;
		for (it = additional_resources.begin(); it != additional_resources.end(); ++it)
		{
			std::vector<std::string> additionals = it->get_sections(section_name);
			std::copy(additionals.begin(), additionals.end(), std::back_inserter(result));
		}
	}

	return result;
}

std::vector<std::string> CL_ResourceManager_Generic::get_resources_of_type(const std::string &type_id)
{
	std::vector<std::string> result;

	// Add local resources:
	{
		std::map<std::string, CL_Resource>::iterator it;
		for (it = resources.begin(); it != resources.end(); ++it)
		{
			if (it->second.get_type() == type_id) result.push_back(it->first);
		}
	}

	// Add resources for all additional resources:
	{
		std::list<CL_ResourceManager>::iterator it;
		for (it = additional_resources.begin(); it != additional_resources.end(); ++it)
		{
			std::vector<std::string> additionals = it->get_resources_of_type(type_id);
                        std::copy(additionals.begin(), additionals.end(), std::back_inserter(result));
		}
	}

	return result;
}

std::vector<std::string> CL_ResourceManager_Generic::get_resources_of_type(const std::string &type_id, const std::string &original_section_name)
{
	std::vector<std::string> result;

	// Make sure we have a trailing / in section
	std::string section_name = add_trailing_slash(original_section_name);

	// Add local resources:
	{
		std::map<std::string, CL_Resource>::iterator it;
		for (it = resources.begin(); it != resources.end(); ++it)
		{
			if (it->first.substr(0, section_name.length()) == section_name)
			{
				if (it->second.get_type() == type_id) result.push_back(it->first);
			}
		}
	}

	// Add resources for all additional resources:
	{
		std::list<CL_ResourceManager>::iterator it;
		for (it = additional_resources.begin(); it != additional_resources.end(); ++it)
		{
			std::vector<std::string> additionals = it->get_resources_of_type(type_id, section_name);
                        std::copy(additionals.begin(), additionals.end(), std::back_inserter(result));
		}
	}

	return result;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceManager_Generic Signals:

CL_Signal_v1<CL_Resource &> CL_ResourceManager_Generic::sig_resource_added;

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceManager_Generic Operations:

void CL_ResourceManager_Generic::load(
	const std::string &config_file,
	CL_InputSourceProvider *input_provider,
	bool delete_inputsource_provider)
{
	if (delete_provider)
		delete provider;

	provider = input_provider;
	delete_provider = delete_inputsource_provider;

	if (provider == 0)
	{
		provider = new CL_InputSourceProvider_File(CL_String::get_path(config_file));
		delete_provider = true;
	}
	else
	{
		CL_InputSourceProvider *new_provider = provider->create_relative(CL_String::get_path(config_file));
		if (delete_provider) delete provider;
		provider = new_provider;
		delete_provider = true;
	}

	try
	{
		document.load(provider->open_source(CL_String::get_filename(config_file)), true);
	}
	catch( CL_Error err )
	{
		// a hack to report the filename of the malformed xml file
		err.message.insert(0, config_file + ": " );
		throw err;
	}

	CL_DomElement root = document.named_item("resources").to_element();
	if (root.is_null()) throw CL_Error(config_file + ": Not a valid XML resource file.");
	parse_section(root);

	std::map<std::string, CL_Resource>::iterator it;
	for (it = resources.begin(); it != resources.end(); ++it)
	{
		sig_resource_added(it->second);
	}
}

void CL_ResourceManager_Generic::add_resources(const CL_ResourceManager &resources)
{
	additional_resources.push_back(resources);
}

void CL_ResourceManager_Generic::remove_resources(const CL_ResourceManager &resources)
{
	additional_resources.remove(resources);
}

void CL_ResourceManager_Generic::load_all()
{
	load_section(std::string());
}

void CL_ResourceManager_Generic::unload_all()
{
	unload_section(std::string());
}

void CL_ResourceManager_Generic::load_section(const std::string &original_section_name)
{
	// Make sure we have a trailing / in section
	std::string section_name = add_trailing_slash(original_section_name);

	// Load local resources:
	{
		std::map<std::string, CL_Resource>::iterator it;
		for (it = resources.begin(); it != resources.end(); ++it)
		{
			if (it->first.substr(0, section_name.length()) == section_name)
			{
				it->second.load();
			}
		}
	}

	// Load additional resources:
	{
		std::list<CL_ResourceManager>::iterator it;
		for (it = additional_resources.begin(); it != additional_resources.end(); ++it)
		{
			CL_ResourceManager &additional_manager = *it;
			additional_manager.load_section(section_name);
		}
	}
}

void CL_ResourceManager_Generic::unload_section(const std::string &original_section_name)
{
	// Make sure we have a trailing / in section
	std::string section_name = add_trailing_slash(original_section_name);

	// Load local resources:
	{
		std::map<std::string, CL_Resource>::iterator it;
		for (it = resources.begin(); it != resources.end(); ++it)
		{
			if (it->first.substr(0, section_name.length()) == section_name)
			{
				it->second.unload();
			}
		}
	}

	// Load additional resources:
	{
		std::list<CL_ResourceManager>::iterator it;
		for (it = additional_resources.begin(); it != additional_resources.end(); ++it)
		{
			CL_ResourceManager &additional_manager = *it;
			additional_manager.unload_section(section_name);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceManager_Generic Implementation:

void CL_ResourceManager_Generic::parse_section(CL_DomElement &sectionElement, const std::string &prefix)
{
	std::string path = prefix + sectionElement.get_attribute("name") + "/";
	if (sectionElement.get_tag_name() == "resources") path = prefix;

	for (CL_DomNode node = sectionElement.get_first_child(); !node.is_null(); node = node.get_next_sibling())
	{
		if (!node.is_element()) continue;

		CL_DomElement element = node.to_element();
		if (element.get_tag_name() == "section")
		{
			parse_section(element, path);
		}
		else if(element.get_tag_name() == "include")
		{
			// get the file name and provider name
			std::string file_name = element.get_attribute("file");
			std::string zip_name = element.get_attribute("provider");
			bool local = element.get_attribute("local","false") == "true";

			CL_ResourceManager *additional_resource;
			if(local)
			{
				file_name = provider->get_pathname(file_name);
				additional_resource = new CL_ResourceManager(file_name, provider);
			}
			else if(zip_name != "")
			{
				CL_Zip_Archive *arch = new CL_Zip_Archive(zip_name);
				file_name = arch->get_pathname(file_name);
				additional_resource = new CL_ResourceManager(file_name, arch);
			}
			else
			{
				file_name = provider->get_pathname(file_name);
				additional_resource = new CL_ResourceManager(file_name);
			}

			add_resources(*additional_resource);
		}
		else
		{
			CL_Resource resource(CL_SharedPtr<CL_Resource_Generic>(new CL_Resource_Generic(element, self)));
			resources[path + element.get_attribute("name")] = resource;
		}
	}
}

void CL_ResourceManager_Generic::add_sections(std::vector<std::string> &returnList, CL_DomElement &sectionElement, const std::string &prefix)
{
	std::string path = prefix + sectionElement.get_attribute("name") + "/";
	if (sectionElement.get_tag_name() == "resources") path = prefix;

	for (CL_DomNode node = sectionElement.get_first_child(); !node.is_null(); node = node.get_next_sibling())
	{
		if (!node.is_element()) continue;

		CL_DomElement element = node.to_element();
		if (element.get_tag_name() == "section")
		{
			returnList.push_back(path + element.get_attribute("name"));
			add_sections(returnList, element, path);
		}
	}
}

void CL_ResourceManager_Generic::add_section(std::vector<std::string> &returnList, CL_DomElement &sectionElement, const std::string &section_name)
{
	for (CL_DomNode node = sectionElement.get_first_child(); !node.is_null(); node = node.get_next_sibling())
	{
		if (!node.is_element()) continue;

		CL_DomElement element = node.to_element();
		if (element.get_tag_name() == "section")
		{
			if (sectionElement.get_attribute("name") == section_name)
				returnList.push_back(element.get_attribute("name"));
			add_section(returnList, element, section_name);
		}
	}
}

std::string CL_ResourceManager_Generic::add_trailing_slash(const std::string &string)
{
	if(string.length() && string.at(string.length() - 1) != '/')
		return string + '/';
	else
		return string;
}
