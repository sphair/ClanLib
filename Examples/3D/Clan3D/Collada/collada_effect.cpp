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
#include "collada_effect.h"
#include "collada_effect_color_texture.h"
#include "collada_effect_float_param.h"
#include "collada_effect_texture.h"

class CL_Collada_Effect_Impl
{
public:
	CL_Collada_Effect_Impl() {}

	void load_profile_common(CL_DomElement &profile_common_element, std::vector<CL_Collada_Image> &library_images);

	CL_String id;
	CL_String name;

	CL_Collada_Effect_ColorTexture emission;
	CL_Collada_Effect_ColorTexture ambient;
	CL_Collada_Effect_ColorTexture diffuse;
	CL_Collada_Effect_ColorTexture specular;
	CL_Collada_Effect_FloatParam shininess;
	CL_Collada_Effect_ColorTexture reflective;
	CL_Collada_Effect_FloatParam reflectivity;
	CL_Collada_Effect_ColorTexture transparent;
	CL_Collada_Effect_FloatParam transparency;
	CL_Collada_Effect_FloatParam index_of_refraction;

	std::vector<CL_Collada_Effect_Texture> samplers;

private:
	void load_blinn(CL_DomElement &blinn_element);
	void load_textures(CL_DomElement &profile_common_element, std::vector<CL_Collada_Image> &library_images);

};

CL_Collada_Effect::CL_Collada_Effect()
{
}

CL_Collada_Effect::CL_Collada_Effect(CL_DomElement &effect_element, std::vector<CL_Collada_Image> &library_images) : impl(new CL_Collada_Effect_Impl())
{
	impl->name = effect_element.get_attribute("name");
	impl->id = effect_element.get_attribute("id");

	CL_DomElement profile_common_element = effect_element.named_item("profile_COMMON").to_element();
	if (!profile_common_element.is_null())
	{
		impl->load_profile_common(profile_common_element, library_images);
	}

}

void CL_Collada_Effect_Impl::load_profile_common(CL_DomElement &profile_common_element, std::vector<CL_Collada_Image> &library_images)
{
	load_textures(profile_common_element, library_images);

	CL_DomElement technique_element = profile_common_element.named_item("technique").to_element();
	if (profile_common_element.is_null())
		throw CL_Exception("technique element missing");

	CL_DomElement blinn_element = technique_element.named_item("blinn").to_element();
	if (blinn_element.is_null())
	{
		blinn_element = technique_element.named_item("phong").to_element();
		if (blinn_element.is_null())
		throw CL_Exception("blinn or phong element missing");
	}

	load_blinn(blinn_element);

}

void CL_Collada_Effect_Impl::load_textures(CL_DomElement &profile_common_element, std::vector<CL_Collada_Image> &library_images)
{
	// Scan for examples
	CL_DomNode cur_child(profile_common_element.get_first_child());
	while (!cur_child.is_null())
	{
			if(cur_child.get_node_name() == "newparam")
			{
				CL_DomElement newparam_element = cur_child.to_element();
				CL_DomElement sampler2d_element = newparam_element.named_item("sampler2D").to_element();
				if (!sampler2d_element.is_null())
					samplers.push_back( CL_Collada_Effect_Texture(profile_common_element, newparam_element, sampler2d_element, library_images) );
			}
			cur_child = cur_child.get_next_sibling();
	}

}

void CL_Collada_Effect_Impl::load_blinn(CL_DomElement &blinn_element)
{
	CL_DomElement element;
	element = blinn_element.named_item("emission").to_element();
	if (!element.is_null())
		emission = CL_Collada_Effect_ColorTexture(element, samplers);

	element = blinn_element.named_item("ambient").to_element();
	if (!element.is_null())
		ambient = CL_Collada_Effect_ColorTexture(element, samplers);

	element = blinn_element.named_item("diffuse").to_element();
	if (!element.is_null())
		diffuse = CL_Collada_Effect_ColorTexture(element, samplers);

	element = blinn_element.named_item("specular").to_element();
	if (!element.is_null())
		specular = CL_Collada_Effect_ColorTexture(element, samplers);

	element = blinn_element.named_item("shininess").to_element();
	if (!element.is_null())
		shininess = CL_Collada_Effect_FloatParam(element);

	element = blinn_element.named_item("reflective").to_element();
	if (!element.is_null())
		reflective = CL_Collada_Effect_ColorTexture(element, samplers);

	element = blinn_element.named_item("reflectivity").to_element();
	if (!element.is_null())
		reflectivity = CL_Collada_Effect_FloatParam(element);

	element = blinn_element.named_item("transparent").to_element();
	if (!element.is_null())
		transparent = CL_Collada_Effect_ColorTexture(element, samplers);

	element = blinn_element.named_item("transparency").to_element();
	if (!element.is_null())
		transparency = CL_Collada_Effect_FloatParam(element);

	element = blinn_element.named_item("index_of_refraction").to_element();
	if (!element.is_null())
		index_of_refraction = CL_Collada_Effect_FloatParam(element);
}

CL_String &CL_Collada_Effect::get_name()
{
	return impl->name;
}

CL_String &CL_Collada_Effect::get_id()
{
	return impl->id;
}

CL_Collada_Effect_ColorTexture &CL_Collada_Effect::get_emission()
{
	return impl->emission;
}

CL_Collada_Effect_ColorTexture &CL_Collada_Effect::get_ambient()
{
	return impl->ambient;
}

CL_Collada_Effect_ColorTexture &CL_Collada_Effect::get_diffuse()
{
	return impl->diffuse;
}

CL_Collada_Effect_ColorTexture &CL_Collada_Effect::get_specular()
{
	return impl->specular;
}

CL_Collada_Effect_FloatParam &CL_Collada_Effect::get_shininess()
{
	return impl->shininess;
}

CL_Collada_Effect_ColorTexture &CL_Collada_Effect::get_reflective()
{
	return impl->reflective;
}

CL_Collada_Effect_FloatParam &CL_Collada_Effect::get_reflectivity()
{
	return impl->reflectivity;
}

CL_Collada_Effect_ColorTexture &CL_Collada_Effect::get_transparent()
{
	return impl->transparent;
}

CL_Collada_Effect_FloatParam &CL_Collada_Effect::get_transparency()
{
	return impl->transparency;
}

CL_Collada_Effect_FloatParam &CL_Collada_Effect::get_index_of_refraction()
{
	return impl->index_of_refraction;
}

bool CL_Collada_Effect::is_null()
{
	return impl.is_null();
}

