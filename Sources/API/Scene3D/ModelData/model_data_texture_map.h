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

#include "model_data_animation_data.h"

namespace clan
{

/// \brief Texture sampler state and UVW animation data used when drawing vertex elements
class ModelDataTextureMap
{
public:
	enum WrapMode
	{
		wrap_repeat,
		wrap_clamp_to_edge,
		wrap_mirror
	};

	ModelDataTextureMap() : texture(-1), channel(-1), wrap_x(wrap_repeat), wrap_y(wrap_repeat) { }

	int texture;
	int channel;

	WrapMode wrap_x, wrap_y;

	ModelDataAnimationData<Vec3f> uvw_offset;
	ModelDataAnimationData<Quaternionf> uvw_rotation;
	ModelDataAnimationData<Vec3f> uvw_scale;

	Mat4f get_uvw_matrix(int animation_index, float animation_time)
	{
		if (channel != -1 && texture != -1)
			return
				Mat4f::translate(uvw_offset.get_value(animation_index, animation_time)) *
				Mat4f::scale(uvw_scale.get_value(animation_index, animation_time)) *
				uvw_rotation.get_value(animation_index, animation_time).to_matrix();
		else
			return Mat4f::identity();
	}

};

}

