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

#include "model_data_mesh_lod.h"
#include "model_data_bone.h"
#include "model_data_light.h"
#include "model_data_camera.h"
#include "model_data_attachment_point.h"
#include "model_data_particle_emitter.h"
#include "model_data_animation.h"
#include "model_data_texture.h"
namespace clan
{

class ModelData
{
public:
	std::vector<ModelDataMeshLOD> mesh_lods;
	std::vector<ModelDataTexture> textures;
	std::vector<std::string> replaceable_textures;
	std::vector<ModelDataBone> bones;
	std::vector<ModelDataLight> lights;
	std::vector<ModelDataCamera> cameras;
	std::vector<ModelDataAttachmentPoint> attachment_points;
	std::vector<ModelDataParticleEmitter> particle_emitters;
	std::vector<ModelDataAnimation> animations;
	Vec3f aabb_min, aabb_max;
};

}

