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

#include "API/Scene3D/ModelData/model_data_draw_range.h"

namespace clan
{

class ModelDataDrawRange;

class ModelShaderDescription
{
public:
	ModelShaderDescription()
	: color_channel(), diffuse_channel(), specular_channel(), bumpmap_channel(), self_illumination_channel(), bones()
	{
	}

	ModelShaderDescription(const ModelDataDrawRange &description, bool use_bones, bool use_color_channel)
		: color_channel(use_color_channel), diffuse_channel(), specular_channel(), bumpmap_channel(), self_illumination_channel(), bones(use_bones)
	{
		diffuse_channel = (description.diffuse_map.texture != -1);
		specular_channel = (description.specular_map.texture != -1);
		bumpmap_channel = (description.bumpmap_map.texture != -1);
		self_illumination_channel = (description.self_illumination_map.texture != -1);
	}

	bool operator<(const ModelShaderDescription &other) const
	{
		if (bones != other.bones)
			return bones < other.bones;
		else if (color_channel != other.color_channel)
			return color_channel < other.color_channel;
		else if (diffuse_channel != other.diffuse_channel)
			return diffuse_channel < other.diffuse_channel;
		else if (specular_channel != other.specular_channel)
			return specular_channel < other.specular_channel;
		else if (bumpmap_channel != other.bumpmap_channel)
			return bumpmap_channel < other.bumpmap_channel;
		else
			return self_illumination_channel < other.self_illumination_channel;
	}

	bool color_channel;
	bool diffuse_channel;
	bool specular_channel;
	bool bumpmap_channel;
	bool self_illumination_channel;
	bool bones;
};

}

