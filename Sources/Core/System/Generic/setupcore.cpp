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

#include "Core/precomp.h"
#include <cstdlib>
#include "API/Core/Resources/resourcetype_boolean.h"
#include "API/Core/Resources/resourcetype_integer.h"
#include "API/Core/Resources/resourcetype_float.h"
#include "API/Core/Resources/resourcetype_string.h"
#include "API/Core/Resources/resourcetype_raw.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/Core/System/setupcore.h"

void init_system(); // this function is implemented in init_PLATFORM.cpp
void deinit_system(); // this function is implemented in init_PLATFORM.cpp

static int ref_count = 0;
static CL_Slot slot_resource_added;

static void clancore_resource_added(CL_Resource &resource)
{
	std::string type = resource.get_type();

	if (type == "boolean") new CL_ResourceData_Boolean(resource);
	if (type == "integer") new CL_ResourceData_Integer(resource);
	if (type == "float") new CL_ResourceData_Float(resource);
	if (type == "string") new CL_ResourceData_String(resource);
	if (type == "raw") new CL_ResourceData_Raw(resource);
}

CL_SetupCore::CL_SetupCore(bool register_resources_only)
{
	CL_SetupCore::init(register_resources_only);
}

CL_SetupCore::~CL_SetupCore()
{
	CL_SetupCore::deinit();
}

void CL_SetupCore::init(bool register_resources_only)
{
	ref_count++;
	if (ref_count > 1) return;

	slot_resource_added = CL_ResourceManager::sig_resource_added().connect(&clancore_resource_added);
	if (register_resources_only) return;
	
	init_system();
}

void CL_SetupCore::deinit()
{
	// Since, CL_SetupCore::init() may be called more than once - Only call deinit at the final call to this function
	if (!ref_count) return;	// Do not allow ref_count to become negative (may occur if something calls deinit() without an init() )
	ref_count--;
	if (ref_count > 0) return;	// Wait until final call to deinit

	slot_resource_added = CL_Slot();

	deinit_system();
}
