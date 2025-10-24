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

#ifndef header_resource
#define header_resource

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
#include <string>

class CL_ResourceManager;
class CL_ResourceData;
class CL_Resource_Generic;

#include "../XML/dom_element.h"

//: Interface for accessing a resource in the resource manager.
//- !group=Core/Resources!
//- !header=core.h!
class CL_API_CORE CL_Resource
{
//! Construction:
public:
	//: Resource constructor.
	CL_Resource(
		CL_DomElement &element,
		CL_ResourceManager *manager);

	CL_Resource(const CL_Resource &copy);

	CL_Resource();

	//: Resource destructor.
	virtual ~CL_Resource();

//! Attributes:
public:
	//: Returns the type of the resource.
	std::string get_type() const;

	//: Returns the name of the resource.
	std::string get_name() const;

	//: Returns the resource DOM element.
	CL_DomElement &get_element();

	//: Returns the resource manager.
	CL_ResourceManager get_manager();

	//: Returns the data with the matching name.
	CL_ResourceData *get_data(const std::string &name);

	//: Returns the current reference count.
	int get_reference_count() const;

//! Operations:
public:
	//: Attach some data to the resource.
	void attach_data(const std::string &name, CL_ResourceData *data);

	//: Detach some data from the resource.
	void detach_data(CL_ResourceData *data);

	//: Unloads the resource from memory.
	void unload();

	//: Loads the resource, using the prefered source as specified by
	//: the resource manager.
	void load();

//! Implementation:
private:
	CL_Resource(const CL_SharedPtr<CL_Resource_Generic> &impl);

	//: Pointer to implementation
	CL_SharedPtr<CL_Resource_Generic> impl;

	friend class CL_ResourceData;
	friend class CL_ResourceManager_Generic;
};

#endif
