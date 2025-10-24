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

//! clanCore="Resources"
//! header=core.h

#ifndef header_resource_manager
#define header_resource_manager

#ifdef CL_API_DLL
#ifdef CL_CORE_EXPORT
#define CL_API_CORE __declspec(dllexport)
#else
#define CL_API_CORE __declspec(dllimport)
#endif
#else
#define CL_API_CORE
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "../System/sharedptr.h"
#include "../../signals.h"
#include <list>
#include <string>

class CL_Resource;
class CL_InputSourceProvider;
class CL_ResourceManager_Generic;

class CL_API_CORE CL_ResourceManager
//: The ClanLib resource manager.
//- !group=Core/Resources!
//- !header=core.h!
//- <p>The resource manager is used to retrieve resources from a given
//- resource source.</p>
//-
//- <p>This can either be a resource script file (used as input to the
//- datafile compiler), or a datafile with all the resources included into
//- one large gzipped resource file.</p>
//-
//- <p>To speedup loading of resources in a game, you can load entire sections
//- of resources at once. When a resource in the section is requested, it is
//- returned instantly without having to access the disk. This is especially
//- useful to make sure all the game resources are loaded before the game is
//- started.</p>
//-
//- <p>Resources are normally not retrieved using the get_resource() function.
//- Instead, you should load the resource using the appropiate resource type
//- class. For instance, a surface is easiest loaded like this:</p>
//-
//- <pre>
//- CL_ResourceManager res_manager("my_datafile.dat", true);
//- CL_Surface my_surface("my_surface", &res_manager);
//- </pre>
//-
//- <p>Getting the same resource twice won't create a new instance of the
//- resource; they are reference counted.</p>
{
//! Construction:
public:
	//: Resource Manager constructor.
	//param config_file: The name of the file in which the resources are defined.
	//param provider: The optional inputprovider in which, the resource file is stored.
	//param delete_inputsource_provider: If true, deletes the provider when CL_ResourceManager is destroyed.
	CL_ResourceManager(
		const std::string &config_file,
		CL_InputSourceProvider *provider = 0,
		bool delete_inputsource_provider = false);

	CL_ResourceManager(const CL_ResourceManager &copy);

	CL_ResourceManager();

	//: Resource Manager destructor.
	~CL_ResourceManager();

//! Attributes:
public:
	//: Returns true if a resource exists.
	bool exists(const std::string &res_id);
	
	//: Returns a pointer to the CL_Resource representing the given resource.
	CL_Resource &get_resource(const std::string &res_id, bool resolve_alias = true, int reserved = 0);

	//: Returns a list of all resources available.
	//return: The list of resources available, in the form [section/subsection/.../]resourcename.
	std::vector<std::string> get_all_resources();

	//: Returns a list of all resources available matching a given section. 
	//return: The list of resources available, in the form [section/subsection/.../]resourcename.
	std::vector<std::string> get_all_resources(const std::string &section_name);

	//: Returns a list of all resource sections available.
	//return: The list of resource sections available, in the form [section/subsection/.../].
	std::vector<std::string> get_all_sections();

	//: Returns a list of the resource sections immediately under the given secion.
	//return: The list of sections immediately available under the given section.
	std::vector<std::string> get_sections(const std::string &section_name);
	
	//: Returns a list of all resources available matching a given type. 
	//return: The list of resources available.
	std::vector<std::string> get_resources_of_type(const std::string &type_id);

	//: Returns a list of all resources available matching a given type in a given section. 
	//return: The list of resources available.
	std::vector<std::string> get_resources_of_type(const std::string &type_id, const std::string &section_name);

	//: Returns a pointer to the input source provider, in which all resources are stored.
	//- <p>This can be a file provider or a datafile provider depending
	//- on method used to load the script file.</p>
	//return: Pointer to inputsourceprovider containing resource data.
	CL_InputSourceProvider *get_resource_provider() const;

//! Signals:
public:
	//: Signal invoked when a resource is added during loading of a resource file.
	static CL_Signal_v1<CL_Resource &> &sig_resource_added();

//! Operations:
public:
	//: Returns true resource managers handles the same object.
	bool operator == (const CL_ResourceManager &other) const;

	//: Add resources from an other resource manager.
	void add_resources(const CL_ResourceManager &additional_resources);

	//: Remove resources from an other resource manager.
	void remove_resources(const CL_ResourceManager &additional_resources);

	//: Loads all resources into memory.
	void load_all();

	//: Unloads all resources from memory.
	void unload_all();

	//: Loads all resources in a given section into memory.
	void load_section(const std::string &section_name);

	//: Unloads all resources in a given section into memory.
	void unload_section(const std::string &section_name);

//! Implementation:
private:
	CL_ResourceManager(const CL_SharedPtr<CL_ResourceManager_Generic> &impl);

	//: Pointer to the implementation.
	CL_SharedPtr<CL_ResourceManager_Generic> impl;

	friend class CL_Resource;
};
#endif
