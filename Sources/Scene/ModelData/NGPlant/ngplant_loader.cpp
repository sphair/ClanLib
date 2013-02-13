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

#include "Scene/precomp.h"
#include "ngplant_loader.h"
#include "tree_material.h"
#include "Scene/ModelData/model_data.h"

namespace clan
{


std::shared_ptr<ModelData> NGPlantLoader::load(const std::string &filename)
{
	std::string base_path = PathHelp::get_fullpath(filename);

	TreeMaterialFactory material_factory;
	P3DPlantModel model;

	P3DInputStringStreamFile stream;
	stream.Open(filename.c_str());
	model.Load(&stream, &material_factory);
	stream.Close();

	return load(&model, base_path);
}

std::shared_ptr<ModelData> NGPlantLoader::load(const P3DPlantModel *plant_model, const std::string &base_path)
{
	NGPlantLoader loader(plant_model, base_path);
	return loader.data;
}

NGPlantLoader::NGPlantLoader(const P3DPlantModel *plant_model, const std::string &base_path)
: base_path(base_path), data(new ModelData())
{
	P3DHLIPlantTemplate plant_template(plant_model);
	int num_groups = plant_template.GetGroupCount();
	std::unique_ptr<P3DHLIPlantInstance> instance(plant_template.CreateInstance());

	data->mesh_lods.resize(1);
	data->mesh_lods[0].meshes.resize(1);
	data->mesh_lods[0].meshes[0].channels.resize(1);

	for (int group_index = 0; group_index < num_groups; group_index++)
	{
		materials.push_back(load_material(plant_template, group_index));

		int vertex_start = data->mesh_lods[0].meshes[0].vertices.size();

		int num_attributes = instance->GetVAttrCountI(group_index);
		if (num_attributes > 0)
		{
			data->mesh_lods[0].meshes[0].vertices.resize(vertex_start + num_attributes);
			data->mesh_lods[0].meshes[0].normals.resize(vertex_start + num_attributes);
			data->mesh_lods[0].meshes[0].channels[0].resize(vertex_start + num_attributes);
			data->mesh_lods[0].meshes[0].tangents.resize(vertex_start + num_attributes);
			data->mesh_lods[0].meshes[0].bitangents.resize(vertex_start + num_attributes);

			data->mesh_lods[0].meshes[0].bone_selectors.resize(vertex_start + num_attributes);
			data->mesh_lods[0].meshes[0].bone_weights.resize(vertex_start + num_attributes);

			P3DHLIVAttrBuffers buffers;
			buffers.AddAttr(P3D_ATTR_VERTEX, &data->mesh_lods[0].meshes[0].vertices[vertex_start], 0, sizeof(Vec3f));
			buffers.AddAttr(P3D_ATTR_NORMAL, &data->mesh_lods[0].meshes[0].normals[vertex_start], 0, sizeof(Vec3f));
			buffers.AddAttr(P3D_ATTR_TEXCOORD0, &data->mesh_lods[0].meshes[0].channels[0][vertex_start], 0, sizeof(Vec2f));
			buffers.AddAttr(P3D_ATTR_TANGENT, &data->mesh_lods[0].meshes[0].tangents[vertex_start], 0, sizeof(Vec3f));
			buffers.AddAttr(P3D_ATTR_BINORMAL, &data->mesh_lods[0].meshes[0].bitangents[vertex_start], 0, sizeof(Vec3f));
			// To do: seems we must only ask for billboard data in branch with billboard data!
			//buffers.AddAttr(P3D_ATTR_BILLBOARD_POS, &vertices[vertex_start], offsetof(Vertex, billboard_pos), sizeof(Vertex));

			instance->FillVAttrBuffersI(&buffers, group_index);
		}

		int num_branches = instance->GetBranchCount(group_index);
		int indices_per_branch = plant_template.GetIndexCount(group_index, P3D_TRIANGLE_LIST);
		int attributes_per_branch = plant_template.GetVAttrCountI(group_index);

		int index_count = indices_per_branch * num_branches;
		int index_start = data->mesh_lods[0].meshes[0].elements.size();
		data->mesh_lods[0].meshes[0].elements.resize(index_start + index_count);

		for (int branch_index = 0; branch_index < num_branches; branch_index++)
		{
			plant_template.FillIndexBuffer(&data->mesh_lods[0].meshes[0].elements[index_start + branch_index * indices_per_branch], group_index, P3D_TRIANGLE_LIST, P3D_UNSIGNED_INT, branch_index * attributes_per_branch);
		}

		for (int i = 0; i < index_count; i++)
			data->mesh_lods[0].meshes[0].elements[index_start + i] += vertex_start;

		group_lengths.push_back(index_count);
	}

	data->mesh_lods[0].meshes[0].material_ranges.resize(materials.size());
	int pos = 0;
	for (size_t i = 0; i < materials.size(); i++)
	{
		if (!materials[i].filename.empty())
		{
			data->mesh_lods[0].meshes[0].material_ranges[i].diffuse_map.channel = 0;
			data->mesh_lods[0].meshes[0].material_ranges[i].diffuse_map.texture = data->textures.size();
			data->textures.push_back(ModelDataTexture(PathHelp::combine(base_path, materials[i].filename), false));
		}
		data->mesh_lods[0].meshes[0].material_ranges[i].diffuse = Vec3f(1.0f);
		data->mesh_lods[0].meshes[0].material_ranges[i].specular = Vec3f(1.0f);
		data->mesh_lods[0].meshes[0].material_ranges[i].two_sided = materials[i].is_two_sided;
		data->mesh_lods[0].meshes[0].material_ranges[i].start_element = pos;
		data->mesh_lods[0].meshes[0].material_ranges[i].num_elements = group_lengths[i];
		pos += group_lengths[i];
	}

	if (!data->mesh_lods[0].meshes[0].vertices.empty())
	{
		data->aabb_min = data->mesh_lods[0].meshes[0].vertices[0];
		data->aabb_max = data->mesh_lods[0].meshes[0].vertices[0];
	}
	for (size_t i = 1; i < data->mesh_lods[0].meshes[0].vertices.size(); i++)
	{
		data->aabb_min.x = min(data->aabb_min.x, data->mesh_lods[0].meshes[0].vertices[i].x);
		data->aabb_min.y = min(data->aabb_min.y, data->mesh_lods[0].meshes[0].vertices[i].y);
		data->aabb_min.z = min(data->aabb_min.z, data->mesh_lods[0].meshes[0].vertices[i].z);
		data->aabb_max.x = max(data->aabb_max.x, data->mesh_lods[0].meshes[0].vertices[i].x);
		data->aabb_max.y = max(data->aabb_max.y, data->mesh_lods[0].meshes[0].vertices[i].y);
		data->aabb_max.z = max(data->aabb_max.z, data->mesh_lods[0].meshes[0].vertices[i].z);
	}

	data->bones.resize(1);
	data->bones[0].position.timelines.resize(1);
	data->bones[0].position.timelines[0].timestamps.push_back(0.0f);
	data->bones[0].position.timelines[0].values.resize(1);
	data->bones[0].orientation.timelines.resize(1);
	data->bones[0].orientation.timelines[0].timestamps.push_back(0.0f);
	data->bones[0].orientation.timelines[0].values.resize(1);
	data->bones[0].scale.timelines.resize(1);
	data->bones[0].scale.timelines[0].timestamps.push_back(0.0f);
	data->bones[0].scale.timelines[0].values.push_back(Vec3f(1.0f));

	data->animations.resize(1);
	data->animations[0].name = "default";
	data->animations[0].length = 1.0f;
}

NGPlantLoader::Material NGPlantLoader::load_material(P3DHLIPlantTemplate &plant_template, int group_index)
{
	Material material;
	if (plant_template.GetMaterial(group_index)->GetTexName(P3D_TEX_DIFFUSE))
		material.filename = plant_template.GetMaterial(group_index)->GetTexName(P3D_TEX_DIFFUSE);

	material.is_two_sided = plant_template.GetMaterial(group_index)->IsDoubleSided();

	switch (plant_template.GetMaterial(group_index)->GetBillboardMode())
	{
	case P3D_BILLBOARD_MODE_SPHERICAL:
		material.billboard_mode = Material::type_spherical;
		break;

	case P3D_BILLBOARD_MODE_CYLINDRICAL:
		material.billboard_mode = Material::type_cylindrical;
		break;

	case P3D_BILLBOARD_MODE_NONE:
	default:
		material.billboard_mode = Material::type_none;
		break;
	}

	return material;
}

}

