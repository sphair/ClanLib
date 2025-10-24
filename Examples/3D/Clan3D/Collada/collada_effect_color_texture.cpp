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
#include "collada_effect_color_texture.h"
#include "collada_effect_texture.h"

class CL_Collada_Effect_ColorTexture_Impl
{
public:
	CL_Collada_Effect_ColorTexture_Impl() {}

	void load_color(CL_DomElement &color_element);
	void load_texture(CL_DomElement &texture_element, std::vector<CL_Collada_Effect_Texture> &samplers);

	bool color_set;
	CL_Colorf color;
	CL_Collada_Effect_Texture texture;
};

CL_Collada_Effect_ColorTexture::CL_Collada_Effect_ColorTexture()
{
}

CL_Collada_Effect_ColorTexture::CL_Collada_Effect_ColorTexture(CL_DomElement &color_texture_element, std::vector<CL_Collada_Effect_Texture> &samplers) : impl(new CL_Collada_Effect_ColorTexture_Impl())
{
	CL_DomElement color_element = color_texture_element.named_item("color").to_element();
	if (!color_element.is_null())
	{
		impl->load_color(color_element);
		return;
	}
	impl->color_set = false;

	CL_DomElement texture_element = color_texture_element.named_item("texture").to_element();
	if (!texture_element.is_null())
	{
		impl->load_texture(texture_element, samplers);
		return;
	}
	throw CL_Exception("color or texture not found. If you want param, then fixme :)");
}

void CL_Collada_Effect_ColorTexture_Impl::load_texture(CL_DomElement &texture_element, std::vector<CL_Collada_Effect_Texture> &samplers)
{
	//  <texture texture="tux-sampler" texcoord="TEX0"></texture>
	CL_String texture_name = texture_element.get_attribute("texture");

	std::vector<CL_Collada_Effect_Texture>::size_type size = samplers.size();
	for (unsigned int cnt=0; cnt<size; cnt++)
	{
		if (samplers[cnt].get_sid() == texture_name)
		{
			texture = samplers[cnt];
			return;
		}
	}
	throw CL_Exception("Sampler not found");
}

void CL_Collada_Effect_ColorTexture_Impl::load_color(CL_DomElement &color_element)
{
	CL_String points = color_element.get_text();
	const CL_String::char_type *point_text = points.c_str();

	const int num_components = 4;
	float colours[num_components];
	for (int cnt=0; cnt < num_components; cnt++)
	{
		if (!(*point_text))
			throw CL_Exception("Color - Data count mismatch");

		float value = (float) atof(point_text);
		colours[cnt] = value;

		while(*point_text)
		{
			if (*(point_text++) <= ' ')	// Find whitespace
				break;
		}

		while(*point_text)
		{
			if ((*point_text) > ' ')	// Find end of whitespace
				break;
			point_text++;
		}

	}
	if (*point_text)
			throw CL_Exception("Color - Data count mismatch (end)");

	color_set = true;
	color = CL_Colorf(colours[0], colours[1], colours[2], colours[3]);
}

bool CL_Collada_Effect_ColorTexture::is_color_set()
{
	return impl->color_set;
}

CL_Colorf &CL_Collada_Effect_ColorTexture::get_color()
{
	return impl->color;
}

CL_Vec4f CL_Collada_Effect_ColorTexture::get_color2()
{
	CL_Colorf &color = impl->color;
	return CL_Vec4f(color.r, color.g, color.b, color.a);
}

CL_Collada_Effect_Texture &CL_Collada_Effect_ColorTexture::get_texture()
{
	return impl->texture;
}

bool CL_Collada_Effect_ColorTexture::is_null()
{
	return impl.is_null();
}


