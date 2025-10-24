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
#include "collada.h"
#include "collada_geometry.h"
#include "collada_effect.h"
#include "collada_image.h"
#include "collada_material.h"

class CL_Collada_Impl
{
public:
	CL_Collada_Impl() {}

	void load_library_geometries(CL_DomElement &library_geometries_element);
	void load_library_effects(CL_DomElement &library_effects_element, std::vector<CL_Collada_Image> &library_images);
	void load_library_images(CL_DomElement &library_images_element, std::vector<CL_Collada_Image> &library_images);
	void load_library_materials(CL_DomElement &library_materials_element);

	CL_Collada_Geometry &get_geometry(const CL_String &name);

	std::vector<CL_Collada_Geometry> geometries;
	std::vector<CL_Collada_Effect> effects;
	std::vector<CL_Collada_Material> library_materials;
};

CL_Collada::CL_Collada()
{
}

CL_Collada::CL_Collada(CL_DomDocument &collada, std::vector<CL_Collada_Image> &library_images) : impl(new CL_Collada_Impl())
{
	CL_DomElement root = collada.get_document_element();
	if (root.is_null())
		throw CL_Exception("Collada root element not found");

	CL_String version = root.get_attribute("version");
	if (!((version == "1.4.1")	|| (version == "1.4.0"))) // Note, this may not be required
		throw CL_Exception("Unsupported collada version");

	CL_DomElement library_images_element = root.named_item("library_images").to_element();
	if (!library_images_element.is_null())
		impl->load_library_images(library_images_element, library_images);

	CL_DomElement library_effects_element = root.named_item("library_effects").to_element();
	if (!library_effects_element.is_null())
		impl->load_library_effects(library_effects_element, library_images);

	CL_DomElement library_materials_element = root.named_item("library_materials").to_element();
	if (!library_materials_element.is_null())
		impl->load_library_materials(library_materials_element);

	CL_DomElement library_geometries_element = root.named_item("library_geometries").to_element();
	if (!library_geometries_element.is_null())
		impl->load_library_geometries(library_geometries_element);
}

std::vector<CL_Collada_Geometry> &CL_Collada::get_geometries()
{
	return impl->geometries;
}

std::vector<CL_Collada_Effect> &CL_Collada::get_effects()
{
	return impl->effects;
}


void CL_Collada_Impl::load_library_geometries(CL_DomElement &library_geometries_element)
{
	CL_DomNode cur_child(library_geometries_element.get_first_child());
	while (!cur_child.is_null())
	{
			if(cur_child.get_node_name() == "geometry")
			{
				CL_DomElement geometry_element = cur_child.to_element();
				geometries.push_back( CL_Collada_Geometry(geometry_element, library_materials));
			}
			cur_child = cur_child.get_next_sibling();
	}
}

void CL_Collada_Impl::load_library_effects(CL_DomElement &library_effects_element, std::vector<CL_Collada_Image> &library_images)
{
	CL_DomNode cur_child(library_effects_element.get_first_child());
	while (!cur_child.is_null())
	{
			if(cur_child.get_node_name() == "effect")
			{
				CL_DomElement effect_element = cur_child.to_element();
				effects.push_back( CL_Collada_Effect(effect_element, library_images));
			}
			cur_child = cur_child.get_next_sibling();
	}
}

void CL_Collada_Impl::load_library_images(CL_DomElement &library_images_element, std::vector<CL_Collada_Image> &library_images)
{
	CL_DomNode cur_child(library_images_element.get_first_child());
	while (!cur_child.is_null())
	{
			if(cur_child.get_node_name() == "image")
			{
				CL_DomElement image_element = cur_child.to_element();
				library_images.push_back( CL_Collada_Image(image_element));
			}
			cur_child = cur_child.get_next_sibling();
	}
}

void CL_Collada_Impl::load_library_materials(CL_DomElement &library_materials_element)
{
	CL_DomNode cur_child(library_materials_element.get_first_child());
	while (!cur_child.is_null())
	{
			if(cur_child.get_node_name() == "material")
			{
				CL_DomElement material_element = cur_child.to_element();
				library_materials.push_back( CL_Collada_Material(material_element, effects));
			}
			cur_child = cur_child.get_next_sibling();
	}
}

CL_Collada_Geometry &CL_Collada::get_geometry(const CL_String &name)
{
	return impl->get_geometry(name);
}

CL_Collada_Geometry &CL_Collada_Impl::get_geometry(const CL_String &name)
{
	std::vector<CL_Collada_Geometry>::size_type size = geometries.size();
	for (unsigned int cnt=0; cnt<size; cnt++)
	{
		if (geometries[cnt].get_name() == name)
		{
			return geometries[cnt];
		}
	}
	throw CL_Exception("Geometry not found");
}
