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
#include "collada_effect_texture.h"
#include "collada_image.h"

class CL_Collada_Effect_Texture_Impl
{
public:
	CL_Collada_Effect_Texture_Impl() {}

	void load_texture(CL_DomElement &profile_element, CL_DomElement &newparam_element, CL_DomElement &sampler2d_element, std::vector<CL_Collada_Image> &library_images);

	CL_String sid;
	CL_Collada_Image image;
};

CL_Collada_Effect_Texture::CL_Collada_Effect_Texture()
{
}

CL_Collada_Effect_Texture::CL_Collada_Effect_Texture(CL_DomElement &profile_element, CL_DomElement &newparam_element, CL_DomElement &sampler2d_element, std::vector<CL_Collada_Image> &library_images) : impl(new CL_Collada_Effect_Texture_Impl())
{
	impl->load_texture(profile_element, newparam_element, sampler2d_element, library_images);
}

void CL_Collada_Effect_Texture_Impl::load_texture(CL_DomElement &profile_element, CL_DomElement &newparam_element, CL_DomElement &sampler2d_element, std::vector<CL_Collada_Image> &library_images)
{
	sid = newparam_element.get_attribute("sid");

	CL_DomElement source_element = sampler2d_element.named_item("source").to_element();
	if (source_element.is_null())
		throw CL_Exception("source is missing");

	CL_String source_name = source_element.get_text();

	// Find the corresponding surface
	CL_DomElement surface_element;
	CL_DomNode cur_child(profile_element.get_first_child());
	while (!cur_child.is_null())
	{
			if(cur_child.get_node_name() == "newparam")
			{
				CL_DomElement newparam_element = cur_child.to_element();
				CL_String new_sid = newparam_element.get_attribute("sid");
				if (new_sid == source_name)
				{
					surface_element = newparam_element.named_item("surface").to_element();
					if (!surface_element.is_null())
						break;	// Found match
				}
			}
			cur_child = cur_child.get_next_sibling();
	}

	if (surface_element.is_null())
		throw CL_Exception("Cannot find the corresponding surface");

	CL_DomElement init_from_element = surface_element.named_item("init_from").to_element();
	if (init_from_element.is_null())
		throw CL_Exception("Only init_from surfaces are supported");

	CL_String image_name = init_from_element.get_text();

	unsigned int size = library_images.size();
	for (unsigned int cnt=0; cnt < size; cnt++)
	{
		if (library_images[cnt].get_id() == image_name)
		{
			image = library_images[cnt];
			break;
		}
	}
	if (image.is_null())
		throw CL_Exception("Cannot find requested image in the image library");

}

bool CL_Collada_Effect_Texture::is_null()
{
	return impl.is_null();
}

CL_String &CL_Collada_Effect_Texture::get_sid()
{
	return impl->sid;
}

CL_Collada_Image &CL_Collada_Effect_Texture::get_image()
{
	return impl->image;
}
