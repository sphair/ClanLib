/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

/// \brief Information needed to draw a range of vertex elements in a ModelDataMesh.
class ModelDataDrawRange
{
public:
	ModelDataDrawRange() : start_element(), num_elements(), two_sided(), transparent(), alpha_test() { }

	/// \brief First vertex element to draw
	int start_element;

	/// \brief Number of elements to draw
	int num_elements;

	/// \brief Toggles if back facing faces should be drawn too
	bool two_sided;

	/// \brief Controls if faces should be rendered with alpha blending enabled
	bool transparent;

	/// \brief Must be set to true if the faces are rendered with alpha test transparency
	bool alpha_test;

	ModelDataAnimationData<Vec3f> ambient;
	ModelDataAnimationData<Vec3f> diffuse;
	ModelDataAnimationData<Vec3f> specular;
	ModelDataAnimationData<float> self_illumination_amount;
	ModelDataAnimationData<Vec3f> self_illumination;
	ModelDataAnimationData<float> glossiness;
	ModelDataAnimationData<float> specular_level;

	ModelDataTextureMap diffuse_map;
	ModelDataTextureMap specular_map;
	ModelDataTextureMap bumpmap_map;
	ModelDataTextureMap self_illumination_map;
};

}

