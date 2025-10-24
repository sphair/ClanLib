/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
#include "collada_geometry.h"
#include "collada_mesh.h"

class CL_Collada_Geometry_Impl
{
public:
	CL_Collada_Geometry_Impl() {}

	CL_String id;
	CL_String name;

	CL_Collada_Mesh mesh;
};

CL_Collada_Geometry::CL_Collada_Geometry(CL_DomElement &geometry_element, std::vector<CL_Collada_Material> &library_materials) : impl(new CL_Collada_Geometry_Impl())
{
	impl->name = geometry_element.get_attribute("name");
	impl->id = geometry_element.get_attribute("id");

	CL_DomElement mesh_element = geometry_element.named_item("mesh").to_element();
	if (mesh_element.is_null())
		throw CL_Exception("Geometry currently only supports mesh");

	impl->mesh = CL_Collada_Mesh(mesh_element, library_materials);
}

CL_Collada_Mesh &CL_Collada_Geometry::get_mesh()
{
	return impl->mesh;
}

CL_String &CL_Collada_Geometry::get_name()
{
	return impl->name;
}

CL_String &CL_Collada_Geometry::get_id()
{
	return impl->id;
}
