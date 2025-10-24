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

#ifndef header_resourcetype_float
#define header_resourcetype_float

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
#include <string>
#include "../System/error.h"
#include "../System/error.h"

//: Resource Data object for an 'float' resource type.
//- !group=Core/Resources!
//- !header=core.h!
class CL_API_CORE CL_ResourceData_Float : public CL_ResourceData
{
//! Construction:
public:
	//: Resource Data Float Constructor.
	CL_ResourceData_Float(class CL_Resource &resource);

//! Attributes:
public:
	//: Float Value.
	float value;
};

//: r resource accessor class.
//- !group=Core/Resources!
//- !header=core.h!
class CL_Float
{
//! Construction:
public:
	//: Constructs a float
	//param res_id: Resource id of a float resource to be constructed from.
	//param resources: Resource manager to load resource from.
	//param def_value: Default value to use if resource is not found.
	CL_Float(const std::string &res_id, CL_ResourceManager *resources)
	{
		CL_Resource res = resources->get_resource(res_id);
		res.load();
		CL_ResourceData_Float *data = (CL_ResourceData_Float *) res.get_data("float");
		value = data->value;
		res.unload();
	}

	CL_Float(const std::string &res_id, CL_ResourceManager *resources, float def_value)
	{
		try
		{
			CL_Resource res = resources->get_resource(res_id);
			res.load();
			CL_ResourceData_Float *data = (CL_ResourceData_Float *) res.get_data("float");
			value = data->value;
			res.unload();
		}
		catch (CL_Error err)
		{
			value = def_value;
		}
	}

//! Operators:
	//: Converts a CL_Float to a float.
	operator float() const
	{
		return value;
	}

private:
	float value;
};

#endif
