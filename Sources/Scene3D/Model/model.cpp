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

#include "Scene3D/precomp.h"
#include "model.h"
#include "Scene3D/Framework/material_cache.h"
#include "model_shader_cache.h"
#include "API/Scene3D/ModelData/model_data.h"
#include "dual_quaternion.h"
#include "model_mesh_visitor.h"
#include "model_lod.h"
#include "Scene3D/Framework/instances_buffer.h"

namespace clan
{


Model::Model(GraphicContext &gc, ModelMaterialCache &texture_cache, ModelShaderCache &shader_cache, std::shared_ptr<ModelData> model_data, int model_index)
: shader_cache(shader_cache), model_data(model_data), frame(-1), max_instances(0), model_index(model_index)
{
/*
	// Small hack until we support rendering unskinned meshes
	if (model_data->bones.empty())
	{
		model_data->bones.resize(1);
		model_data->bones[0].position.timelines.resize(model_data->animations.size());
		model_data->bones[0].orientation.timelines.resize(model_data->animations.size());
		model_data->bones[0].scale.timelines.resize(model_data->animations.size());
		for (size_t i = 0; i < model_data->animations.size(); i++)
		{
			model_data->bones[0].position.timelines[i].timestamps.resize(1);
			model_data->bones[0].position.timelines[i].values.resize(1);
			model_data->bones[0].orientation.timelines[i].timestamps.resize(1);
			model_data->bones[0].orientation.timelines[i].values.resize(1);
			model_data->bones[0].scale.timelines[i].timestamps.resize(1);
			model_data->bones[0].scale.timelines[i].values.resize(1, Vec3f(1.0f));
		}
	}
*/
	levels.push_back(std::shared_ptr<ModelLOD>(new ModelLOD(gc, model_index, model_data)));

	for (size_t i = 0; i < model_data->textures.size(); i++)
		textures.push_back(texture_cache.get_texture(gc, model_data->textures[i].name, model_data->textures[i].gamma == 1.0f));

	shader_cache.create_gbuffer_commands(gc, this, 0);
	shader_cache.create_transparency_commands(gc, this, 0);
	shader_cache.create_shadow_commands(gc, this, 0);
	shader_cache.create_early_z_commands(gc, this, 0);
}

const std::vector<ModelDataLight> &Model::get_lights()
{
	return model_data->lights;
}

bool Model::add_instance(int instance_frame, const ModelInstance &instance, const Mat4f &object_to_world)
{
	if (instance.animation_index == -1)
		return false;

	if (instance_frame != frame)
	{
		frame = instance_frame;
		instances.clear();
		instances_object_to_world.clear();
	}

	instances.push_back(&instance);
	instances_object_to_world.push_back(object_to_world);
	return instances.size() == 1;
}

int Model::get_instance_vectors_count() const
{
	return instances.size() * get_vectors_per_instance();
}

int Model::get_vectors_per_instance() const
{
	if (model_data->meshes.size() != 1)
		throw Exception("Model::get_vectors_per_instance does not support multiple meshes yet");
	size_t num_materials = model_data->meshes[0].material_ranges.size();
	return instance_base_vectors + model_data->bones.size() * vectors_per_bone + num_materials * vectors_per_material;
}

void Model::upload(InstancesBuffer &instances_buffer, const Mat4f &world_to_eye, const Mat4f &eye_to_projection)
{
	int vectors_per_instance = get_vectors_per_instance();

	Vec4f *instance_data = instances_buffer.upload(model_index, get_instance_vectors_count());
	for (size_t j = 0; j < instances.size(); j++)
	{
		Vec4f *vectors = instance_data + j * vectors_per_instance;

		Mat3f object_normal_to_eye = Mat3f(world_to_eye * instances_object_to_world[j]).inverse().transpose();

		// mat3 object_normal_to_eye
		vectors[0] = Vec4f(object_normal_to_eye[0], object_normal_to_eye[3], object_normal_to_eye[6], 0.0f);
		vectors[1] = Vec4f(object_normal_to_eye[1], object_normal_to_eye[4], object_normal_to_eye[7], 0.0f);
		vectors[2] = Vec4f(object_normal_to_eye[2], object_normal_to_eye[5], object_normal_to_eye[8], 0.0f);

		// mat4 object_to_world
		vectors[3] = Vec4f(instances_object_to_world[j][0], instances_object_to_world[j][4], instances_object_to_world[j][8], instances_object_to_world[j][12]);
		vectors[4] = Vec4f(instances_object_to_world[j][1], instances_object_to_world[j][5], instances_object_to_world[j][9], instances_object_to_world[j][13]);
		vectors[5] = Vec4f(instances_object_to_world[j][2], instances_object_to_world[j][6], instances_object_to_world[j][10], instances_object_to_world[j][14]);
		vectors[6] = Vec4f(instances_object_to_world[j][3], instances_object_to_world[j][7], instances_object_to_world[j][11], instances_object_to_world[j][15]);

		// mat4 world_to_eye
		vectors[7] = Vec4f(world_to_eye[0], world_to_eye[4], world_to_eye[8], world_to_eye[12]);
		vectors[8] = Vec4f(world_to_eye[1], world_to_eye[5], world_to_eye[9], world_to_eye[13]);
		vectors[9] = Vec4f(world_to_eye[2], world_to_eye[6], world_to_eye[10], world_to_eye[14]);
		vectors[10] = Vec4f(world_to_eye[3], world_to_eye[7], world_to_eye[11], world_to_eye[15]);

		// mat4 eye_to_projection
		vectors[11] = Vec4f(eye_to_projection[0], eye_to_projection[4], eye_to_projection[8], eye_to_projection[12]);
		vectors[12] = Vec4f(eye_to_projection[1], eye_to_projection[5], eye_to_projection[9], eye_to_projection[13]);
		vectors[13] = Vec4f(eye_to_projection[2], eye_to_projection[6], eye_to_projection[10], eye_to_projection[14]);
		vectors[14] = Vec4f(eye_to_projection[3], eye_to_projection[7], eye_to_projection[11], eye_to_projection[15]);

		for (size_t i = 0; i < model_data->bones.size(); i++)
		{
			Vec3f position = model_data->bones[i].position.get_value(instances[j]->animation_index, instances[j]->animation_time);
			Quaternionf orientation = model_data->bones[i].orientation.get_value(instances[j]->animation_index, instances[j]->animation_time);
			Vec3f scale = model_data->bones[i].scale.get_value(instances[j]->animation_index, instances[j]->animation_time);

			if (model_data->bones[i].billboarded)
			{
				// To do: optimize this away by feeding upload with the camera orientation and the instance orientation
				Vec3f camera_pos, camera_scale;
				Quaternionf camera_orientation;

				Mat4f object_to_eye = world_to_eye * instances_object_to_world[j];
				object_to_eye.decompose(camera_pos, camera_orientation, camera_scale);

				orientation = Quaternionf::inverse(camera_orientation) * orientation;
			}

			/*
			DualQuaternionf dual_quaternion(position, orientation);

			vectors[instance_base_vectors + i * vectors_per_bone + 0] = Vec4f(dual_quaternion.first.x, dual_quaternion.first.y, dual_quaternion.first.z, dual_quaternion.first.w);
			vectors[instance_base_vectors + i * vectors_per_bone + 1] = Vec4f(dual_quaternion.second.x, dual_quaternion.second.y, dual_quaternion.second.z, dual_quaternion.second.w);
			*/

			Mat4f transform = Mat4f::translate(position) * orientation.to_matrix() * Mat4f::scale(scale);
			transform.transpose();
			vectors[instance_base_vectors + i * vectors_per_bone + 0] = Vec4f(transform[0], transform[1], transform[2], transform[3]);
			vectors[instance_base_vectors + i * vectors_per_bone + 1] = Vec4f(transform[4], transform[5], transform[6], transform[7]);
			vectors[instance_base_vectors + i * vectors_per_bone + 2] = Vec4f(transform[8], transform[9], transform[10], transform[11]);
		}

		size_t num_materials = model_data->meshes[0].material_ranges.size();
		for (size_t i = 0; i < num_materials; i++)
		{
			int material_offset = instance_base_vectors + model_data->bones.size() * vectors_per_bone + i * vectors_per_material;

			Vec3f self_illumination = model_data->meshes[0].material_ranges[i].self_illumination.get_value(instances[j]->animation_index, instances[j]->animation_time);
			float self_illumination_amount = model_data->meshes[0].material_ranges[i].self_illumination_amount.get_value(instances[j]->animation_index, instances[j]->animation_time);
			vectors[material_offset + 0] = Vec4f(self_illumination, self_illumination_amount);
			vectors[material_offset + 1] = Vec4f(0.0f); // model_data->meshes[0].material_ranges[i].diffuse_map.replaceable_texture

			Mat4f uvw[4] = 
			{
				model_data->meshes[0].material_ranges[i].diffuse_map.get_uvw_matrix(instances[j]->animation_index, instances[j]->animation_time),
				model_data->meshes[0].material_ranges[i].bumpmap_map.get_uvw_matrix(instances[j]->animation_index, instances[j]->animation_time),
				model_data->meshes[0].material_ranges[i].self_illumination_map.get_uvw_matrix(instances[j]->animation_index, instances[j]->animation_time),
				model_data->meshes[0].material_ranges[i].specular_map.get_uvw_matrix(instances[j]->animation_index, instances[j]->animation_time)
			};

			for (int h = 0; h < 4; h++)
				uvw[h].transpose();

			for (int h = 0; h < 4; h++)
			{
				vectors[material_offset + 2 + h * 3 + 0] = Vec4f(uvw[h][0], uvw[h][1], uvw[h][2], uvw[h][3]);
				vectors[material_offset + 2 + h * 3 + 1] = Vec4f(uvw[h][4], uvw[h][5], uvw[h][6], uvw[h][7]);
				vectors[material_offset + 2 + h * 3 + 2] = Vec4f(uvw[h][8], uvw[h][9], uvw[h][10], uvw[h][11]);
			}
		}
	}
}

void Model::visit(GraphicContext &gc, InstancesBuffer &instances_buffer, ModelMeshVisitor *visitor)
{
	gc.set_texture(0, instances_buffer.get_indexes());
	gc.set_texture(1, instances_buffer.get_vectors());
	visitor->render(gc, levels[0].get(), instances.size());
}

}

