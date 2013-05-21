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

#include "API/Scene3D/ModelData/model_data.h"
#include "model_instance.h"

namespace clan
{

class ModelDataLight;
class ModelMaterialCache;
class ModelShaderCache;
class ModelLightInstance;
class ModelMeshVisitor;
class ModelLOD;
class InstancesBuffer;

class Model
{
public:
	Model(GraphicContext &gc, ModelMaterialCache &texture_cache, ModelShaderCache &shader_cache, std::shared_ptr<ModelData> model_data, int model_index);
	const std::vector<ModelDataLight> &get_lights();
	const std::shared_ptr<ModelData> &get_model_data() const { return model_data; }

	bool add_instance(int frame, const ModelInstance &instance, const Mat4f &object_to_world);

	int get_instance_vectors_count() const;
	int get_vectors_per_instance() const;
	void upload(InstancesBuffer &instances_buffer, const Mat4f &world_to_eye, const Mat4f &eye_to_projection);

	void visit(GraphicContext &gc, InstancesBuffer &instances_buffer, ModelMeshVisitor *visitor);

	static const int vectors_per_bone = 3;
	static const int instance_base_vectors = 15;
	static const int vectors_per_material = 14;

private:
	ModelShaderCache &shader_cache;
	std::shared_ptr<ModelData> model_data;
	std::vector<Resource<Texture> > textures;

	std::vector<std::shared_ptr<ModelLOD> > levels;

	int frame;
	std::vector<const ModelInstance *> instances;
	std::vector<Mat4f> instances_object_to_world;

	PixelBuffer instance_bones_transfer;
	Texture2D instance_bones;
	int max_instances;

	int model_index;

	friend class ModelShaderCache;
};

}

