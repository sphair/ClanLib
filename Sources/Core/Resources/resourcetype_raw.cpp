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
#include "API/Core/Resources/resourcetype_raw.h"
#include "API/Core/Resources/resource.h"
#include "API/Core/Resources/resource_manager.h"
#include "API/Core/IOData/inputsource.h"
#include "API/Core/IOData/inputsource_provider.h"
#include "API/Core/IOData/outputsource.h"
#include "API/Core/IOData/outputsource_provider.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_Raw construction:

CL_ResourceData_Raw::CL_ResourceData_Raw(CL_Resource &resource)
: CL_ResourceData(resource)
{
	resource.attach_data("raw", this);
}

/////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_Raw implementation:

void CL_ResourceData_Raw::on_load()
{
	CL_ResourceManager manager = get_resource().get_manager();
	std::string filename = get_resource().get_element().get_attribute("file");

	CL_InputSource *input = manager.get_resource_provider()->open_source(filename.c_str());

	try
	{
		int length = input->size();
		value = std::vector<unsigned char>(length);
		input->read(&value.front(), length);
	}
	catch (...)
	{
		delete input;
		throw;
	}

	delete input;
}

void CL_ResourceData_Raw::on_unload()
{
	value = std::vector<unsigned char>();
}
