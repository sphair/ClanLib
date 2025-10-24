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
#include "collada_material.h"
#include "collada_effect.h"

class CL_Collada_Material_Impl
{
public:
	CL_Collada_Material_Impl() {}

	void load_material(CL_DomElement &material_element, std::vector<CL_Collada_Effect> &effects);

	CL_String id;
	CL_Collada_Effect effect;
};

CL_Collada_Material::CL_Collada_Material()
{
}

CL_Collada_Material::CL_Collada_Material(CL_DomElement &material_element, std::vector<CL_Collada_Effect> &effects) : impl(new CL_Collada_Material_Impl())
{
	impl->load_material(material_element, effects);
}

void CL_Collada_Material_Impl::load_material(CL_DomElement &material_element, std::vector<CL_Collada_Effect> &effects)
{
	id = material_element.get_attribute("id");

	CL_DomElement instance_effect_element = material_element.named_item("instance_effect").to_element();
	if (instance_effect_element.is_null())
		throw CL_Exception("Only instance_effect materials are supported");

	CL_String url = instance_effect_element.get_attribute("url");
	url = url.substr(1);	// Remove the initial '#' symbol

	std::vector<CL_Collada_Effect>::size_type size, cnt;
	size = effects.size();
	for (cnt=0; cnt< size; cnt++)
	{
		if (effects[cnt].get_id() == url)
		{
			effect = effects[cnt];
			break;
		}
	}

	if (effect.is_null())
		throw CL_Exception("Unable to find effect");

}

bool CL_Collada_Material::is_null()
{
	return impl.is_null();
}

CL_Collada_Effect &CL_Collada_Material::get_effect()
{
	return impl->effect;
}

CL_String &CL_Collada_Material::get_id()
{
	return impl->id;
}

