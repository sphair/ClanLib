/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
*/

#pragma once

#include "model_data_texture_map.h"
#include "model_data_animation_data.h"
namespace clan
{

class ModelDataMaterialRange
{
public:
	ModelDataMaterialRange() : glossiness(), specular_level(), two_sided(), transparent(), start_element(), num_elements() { }
	clan::Vec3f ambient;
	clan::Vec3f diffuse;
	clan::Vec3f specular;
	ModelDataAnimationData<float> self_illumination_amount;
	ModelDataAnimationData<clan::Vec3f> self_illumination;
	float glossiness;
	float specular_level;
	bool two_sided;
	bool transparent;
	ModelDataTextureMap diffuse_map;
	ModelDataTextureMap specular_map;
	ModelDataTextureMap bumpmap_map;
	ModelDataTextureMap self_illumination_map;
	int start_element;
	int num_elements;
};

}

