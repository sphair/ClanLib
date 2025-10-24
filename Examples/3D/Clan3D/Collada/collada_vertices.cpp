/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
**    Mark Page
*/

#include "precomp.h"
#include "collada_vertices.h"
#include "collada_source.h"
#include "collada_input_unshared.h"

class CL_Collada_Vertices_Impl
{
public:
	CL_Collada_Vertices_Impl() {}

	void load_inputs(CL_DomElement &vertices_element, std::vector<CL_Collada_Source> &sources);

	std::vector<CL_Collada_Input_Unshared> inputs;
	CL_String id;

};

CL_Collada_Vertices::CL_Collada_Vertices()
{
}

CL_Collada_Vertices::CL_Collada_Vertices(CL_DomElement &vertices_element, std::vector<CL_Collada_Source> &sources) : impl(new CL_Collada_Vertices_Impl())
{
	impl->id = vertices_element.get_attribute("id");
	impl->load_inputs(vertices_element, sources);
}

std::vector<CL_Collada_Input_Unshared> &CL_Collada_Vertices::get_inputs()
{
	return impl->inputs;
}

void CL_Collada_Vertices_Impl::load_inputs(CL_DomElement &vertices_element, std::vector<CL_Collada_Source> &sources)
{
	CL_DomNode cur_child(vertices_element.get_first_child());
	while (!cur_child.is_null())
	{
			if(cur_child.get_node_name() == "input")
			{
				CL_DomElement input_element = cur_child.to_element();
				inputs.push_back( CL_Collada_Input_Unshared(input_element, sources));
			}
			cur_child = cur_child.get_next_sibling();
	}
}

CL_String &CL_Collada_Vertices::get_id()
{
	return impl->id;
}

