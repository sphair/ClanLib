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

#include "Display/display_precomp.h"
#include "resourcedata_text_styler.h"
#include "API/Core/System/clanstring.h"
#include "API/Core/System/error.h"
#include "API/Core/Resources/resource_manager.h"

#include <string>
#include <map>

//////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_TextStyler construction:

CL_ResourceData_TextStyler::CL_ResourceData_TextStyler(CL_Resource &resource)
: CL_ResourceData(resource)
{
	resource.attach_data("text_styler", this);
}
	
CL_ResourceData_TextStyler::~CL_ResourceData_TextStyler()
{
}
	
//////////////////////////////////////////////////////////////////////////////
// CL_ResourceData_TextStyler implementation:

void CL_ResourceData_TextStyler::on_load()
{
	CL_Resource resource = get_resource();
	CL_ResourceManager manager = resource.get_manager();

	ts = CL_TextStyler();
	std::map<std::string, CL_Font>& fnt_map = ts.get_fonts();
	
	for (
		CL_DomNode cur_node = resource.get_element().get_first_child();
		!cur_node.is_null();
		cur_node = cur_node.get_next_sibling())
	{
		if (!cur_node.is_element()) continue;
		
		CL_DomElement cur_element = cur_node.to_element();
		if (
			cur_element.get_tag_name() == "font"
			&& cur_element.has_attribute("name")
			&& cur_element.has_attribute("font"))
		{
			if (manager.exists(cur_element.get_attribute("font")))
			{
				fnt_map[cur_element.get_attribute("name")] =
					CL_Font(cur_element.get_attribute("font"), &manager);
			}
			else
			{
				throw CL_Error(std::string("CL_TextStyler: Unable to find sub-font named ") + cur_element.get_attribute("font"));
			}
		}
		else
		{
			throw CL_Error("CL_TextStyler: Unknown sub-element of a text_styler resource");
		}
	}
}

void CL_ResourceData_TextStyler::on_unload()
{
	ts = CL_TextStyler();
}
