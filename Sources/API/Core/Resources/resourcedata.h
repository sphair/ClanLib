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

#ifndef header_resourcedata
#define header_resourcedata

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

#include "resource.h"

class CL_ResourceData_Generic;

//: Data object attached to a resource.
//- !group=Core/Resources!
//- !header=core.h!
//- <p>The CL_ResourceData class represents a data object connected to a
//- resource. A resource can have one or more of these data objects
//- attached, and they can be retrieved by making a call to
//- CL_Resource::get_data.</p>
//- <p>Resource data objects are used to contain extra data associated with
//- a certain resource. For instance, the surface resource has an object
//- attached by the name "surface" that contain the loaded surface's
//- data.</p>
class CL_API_CORE CL_ResourceData
{
//! Construction:
public:
	//: Constructs a resource data object.
	//- <p>A resource data object will not by itself attach to a
	//- CL_Resource. This must be done manually by making a call to
	//- CL_Resource::attach_data.</p>
	//param resource: Resource object associated with this resource data
	//param resource: object.
	CL_ResourceData(CL_Resource &resource);

	//: Resource Data destructor.
	virtual ~CL_ResourceData();

//! Attributes:
public:
	//: Get Resource object.
	CL_Resource get_resource();

//! Operations:
public:
	//: Called first time CL_Resource::load() is called on the resource.
	virtual void on_load() { return; }

	//: Called last time CL_Resource::unload() is called on the resource.
	virtual void on_unload() { return; }

//! Implementation:
private:
	//: Pointer to the implementation.
	CL_ResourceData_Generic *impl;
};

#endif
