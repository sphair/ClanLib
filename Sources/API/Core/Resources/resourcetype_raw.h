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

#ifndef header_resourcetype_raw
#define header_resourcetype_raw

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

#include "resourcedata.h"
#include "resource_manager.h"
#include "resource.h"
#include "../IOData/inputsource_memory.h"
#include <string>
#include <vector>

//: Resource Data object for a 'raw' resource type.
//- !group=Core/Resources!
//- !header=core.h!
class CL_API_CORE CL_ResourceData_Raw : public CL_ResourceData
{
//! Construction:
public:
	//: Resource Data Raw Constructor.
	CL_ResourceData_Raw(class CL_Resource &resource);

//! Attributes:
public:
	//: Vector array containing the raw data.
	std::vector<unsigned char> value;

//! Implementation:
private:
	void on_load();

	void on_unload();
};

//: Raw resource accessor class.
//- !group=Core/Resources!
//- !header=core.h!
class CL_Raw
{
//! Construction:
public:
	//: Constructs an raw object.
	//param res_id: Resource id of an raw resource to load from.
	//param resources: Resource manager to load resource from.
	CL_Raw(const std::string &res_id, CL_ResourceManager *resources)
	{
		res = resources->get_resource(res_id);
		res.load();
		data = (CL_ResourceData_Raw *) res.get_data("raw");
	}

	//: Raw Class destructor.
	~CL_Raw()
	{
		res.unload();
	}

//! Attributes:
public:
	//: Returns the raw data as a vector.
	std::vector<unsigned char> &get_data()
	{
		return data->value;
	}

	const std::vector<unsigned char> &get_data() const
	{
		return data->value;
	}

private:
	CL_Resource res;
	CL_ResourceData_Raw *data;
};

//: Input source interface for 'raw' resource types.
//- !group=Core/Resources!
//- !header=core.h!
class CL_InputSource_Raw : public CL_Raw, public CL_InputSource_Memory
{
public:
	//: Construct input source reading from a 'raw' resource type.
	//param res_id: Resource id of an raw resource to load from.
	//param resources: Resource manager to load resource from.
	CL_InputSource_Raw(const std::string &res_id, CL_ResourceManager *resources)
		: CL_Raw(res_id, resources), CL_InputSource_Memory(std::string((char *) &CL_Raw::get_data()[0], CL_Raw::get_data().size()))
	{
	}
};

#endif
