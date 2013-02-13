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
#include "model_lod.h"
#include "Scene/ModelData/model_data.h"

namespace clan
{


ModelLOD::ModelLOD(GraphicContext &gc, int model_index, std::shared_ptr<ModelData> model_data, int level)
{
	mesh_buffers.reserve(model_data->mesh_lods[level].meshes.size());
	for (size_t i = 0; i < model_data->mesh_lods[level].meshes.size(); i++)
	{
		ModelMeshBuffers buffers;
		buffers.primitives_array = PrimitivesArray(gc);
		buffers.vertices = upload_vector(gc, buffers.primitives_array, 0, model_data->mesh_lods[level].meshes[i].vertices);
		buffers.normals = upload_vector(gc, buffers.primitives_array, 1, model_data->mesh_lods[level].meshes[i].normals);
		buffers.bitangents = upload_vector(gc, buffers.primitives_array, 2, model_data->mesh_lods[level].meshes[i].bitangents);
		buffers.tangents = upload_vector(gc, buffers.primitives_array, 3, model_data->mesh_lods[level].meshes[i].tangents);
		buffers.bone_weights = upload_vector(gc, buffers.primitives_array, 4, model_data->mesh_lods[level].meshes[i].bone_weights, true);
		buffers.bone_selectors = upload_vector(gc, buffers.primitives_array, 5, model_data->mesh_lods[level].meshes[i].bone_selectors, false);
		buffers.colors = upload_vector(gc, buffers.primitives_array, 6, model_data->mesh_lods[level].meshes[i].colors, true);

		for (size_t channel = 0; channel < model_data->mesh_lods[level].meshes[i].channels.size(); channel++)
		{
			buffers.channels.push_back(upload_vector(gc, buffers.primitives_array, 7 + channel, model_data->mesh_lods[level].meshes[i].channels[channel]));
		}

		buffers.elements = ElementArrayVector<unsigned int>(gc, model_data->mesh_lods[level].meshes[i].elements);

		size_t num_materials = model_data->mesh_lods[level].meshes[i].material_ranges.size();
		for (size_t j = 0; j < num_materials; j++)
		{
			const ModelDataMaterialRange &material_range = model_data->mesh_lods[level].meshes[i].material_ranges[j];

			ModelMaterialUniforms block;

			block.material_ambient = Vec4f(material_range.ambient, 0.0f);
			block.material_diffuse = Vec4f(material_range.diffuse, 0.0f);
			block.material_specular = Vec4f(material_range.specular, 0.0f);
			block.material_glossiness = material_range.glossiness;
			block.material_specular_level = material_range.specular_level;

			block.model_index = model_index;
			block.vectors_per_instance = Model::instance_base_vectors + model_data->bones.size() * Model::vectors_per_bone + num_materials * Model::vectors_per_material;
			block.material_offset = Model::instance_base_vectors + model_data->bones.size() * Model::vectors_per_bone + j * Model::vectors_per_material;

			buffers.uniforms.push_back(UniformVector<ModelMaterialUniforms>(gc, &block, 1));
		}

		mesh_buffers.push_back(buffers);
	}
}

template<typename Type>
VertexArrayVector<Type> ModelLOD::upload_vector(GraphicContext &gc, PrimitivesArray &primitives_array, int index, const std::vector<Type> &vec)
{
	if (!vec.empty())
	{
		VertexArrayVector<Type> buffer(gc, vec);
		primitives_array.set_attributes(index, buffer);
		return buffer;
	}
	else
	{
		return VertexArrayVector<Type>();
	}
}

template<typename Type>
VertexArrayVector<Type> ModelLOD::upload_vector(GraphicContext &gc, PrimitivesArray &primitives_array, int index, const std::vector<Type> &vec, bool normalize)
{
	if (!vec.empty())
	{
		VertexArrayVector<Type> buffer(gc, vec);
		primitives_array.set_attributes(index, buffer, normalize);
		return buffer;
	}
	else
	{
		return VertexArrayVector<Type>();
	}
}

}

