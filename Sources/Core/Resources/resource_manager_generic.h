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

#ifndef header_resource_manager_generic
#define header_resource_manager_generic

#if _MSC_VER > 1000
#pragma once
#endif

#include "API/Core/Resources/resource.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/Core/XML/dom_document.h"
#include "API/Core/System/weakptr.h"
#include <string>
#include <list>
#include <map>

class CL_InputSourceProvider;
class CL_DomElement;

class CL_ResourceManager_Generic
{
//! Construction:
public:
	CL_ResourceManager_Generic();

	~CL_ResourceManager_Generic();

//! Attributes:
public:
	bool exists(const std::string &res_id);
	
	CL_Resource &get_resource(const std::string &res_id);

	std::vector<std::string> get_all_resources();

	std::vector<std::string> get_all_resources(const std::string &section_name);

	std::vector<std::string> get_all_sections();

	std::vector<std::string> get_sections(const std::string &section_name);

	std::vector<std::string> get_resources_of_type(const std::string &type_id);

	std::vector<std::string> get_resources_of_type(const std::string &type_id, const std::string &section_name);

	CL_InputSourceProvider *get_resource_provider() const { return provider; }

	CL_WeakPtr<CL_ResourceManager_Generic> self;

//! Signals:
public:
	static CL_Signal_v1<CL_Resource &> sig_resource_added;

//! Operations:
public:
	void load(
		const std::string &config_file,
		CL_InputSourceProvider *provider,
		bool delete_inputsource_provider);

	void add_resources(const CL_ResourceManager &additional_resources);

	void remove_resources(const CL_ResourceManager &additional_resources);

	void load_all();

	void unload_all();

	void load_section(const std::string &section_name);

	void unload_section(const std::string &section_name);

	void parse_section(CL_DomElement &sectionElement, const std::string &prefix = std::string());

	void add_sections(std::vector<std::string> &returnList, CL_DomElement &sectionElement, const std::string &prefix = std::string());

//! Implementation:
private:
	std::string add_trailing_slash(const std::string &string);

	void add_section(std::vector<std::string> &returnList, CL_DomElement &sectionElement, const std::string &section_name);

	CL_DomDocument document;
	
	std::map<std::string, CL_Resource> resources;
	
	std::list<CL_ResourceManager> additional_resources;

	CL_InputSourceProvider *provider;

	bool delete_provider;
};

#endif
