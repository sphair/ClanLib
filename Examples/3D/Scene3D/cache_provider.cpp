
#include "precomp.h"
#include "cache_provider.h"

using namespace clan;

ExampleSceneCacheProvider::ExampleSceneCacheProvider()
{
}

std::shared_ptr<ModelData> ExampleSceneCacheProvider::get_model_data(const std::string &name)
{
	std::shared_ptr<ModelData> model_data(new ModelData());

	float size = 50.0f;

	model_data->aabb_min = Vec3f(-size, 0.0f, -size);
	model_data->aabb_max = Vec3f( size, 0.0f,  size);

	model_data->animations.resize(1);
	model_data->animations[0].name = "default";
/*
	model_data->textures.resize(1);
	model_data->textures[0].gamma = 2.2f;
	model_data->textures[0].name = "texture.png";
*/
	model_data->mesh_lods.resize(1);
	model_data->mesh_lods[0].meshes.resize(1);
//	model_data->mesh_lods[0].meshes[0].channels.resize(1);

	model_data->mesh_lods[0].meshes[0].vertices.push_back(Vec3f(-size, 0.0f,-size));
	model_data->mesh_lods[0].meshes[0].vertices.push_back(Vec3f( size, 0.0f,-size));
	model_data->mesh_lods[0].meshes[0].vertices.push_back(Vec3f(-size, 0.0f, size));
	model_data->mesh_lods[0].meshes[0].vertices.push_back(Vec3f( size, 0.0f, size));

	model_data->mesh_lods[0].meshes[0].normals.push_back(Vec3f(0.0f, 0.0f, 1.0f));
	model_data->mesh_lods[0].meshes[0].normals.push_back(Vec3f(0.0f, 0.0f, 1.0f));
	model_data->mesh_lods[0].meshes[0].normals.push_back(Vec3f(0.0f, 0.0f, 1.0f));
	model_data->mesh_lods[0].meshes[0].normals.push_back(Vec3f(0.0f, 0.0f, 1.0f));

	model_data->mesh_lods[0].meshes[0].tangents.push_back(Vec3f(1.0f, 0.0f, 0.0f));
	model_data->mesh_lods[0].meshes[0].tangents.push_back(Vec3f(1.0f, 0.0f, 0.0f));
	model_data->mesh_lods[0].meshes[0].tangents.push_back(Vec3f(1.0f, 0.0f, 0.0f));
	model_data->mesh_lods[0].meshes[0].tangents.push_back(Vec3f(1.0f, 0.0f, 0.0f));

	model_data->mesh_lods[0].meshes[0].bitangents.push_back(Vec3f(0.0f, 1.0f, 0.0f));
	model_data->mesh_lods[0].meshes[0].bitangents.push_back(Vec3f(0.0f, 1.0f, 0.0f));
	model_data->mesh_lods[0].meshes[0].bitangents.push_back(Vec3f(0.0f, 1.0f, 0.0f));
	model_data->mesh_lods[0].meshes[0].bitangents.push_back(Vec3f(0.0f, 1.0f, 0.0f));
/*
	model_data->mesh_lods[0].meshes[0].channels[0].push_back(Vec2f(0.0f, 0.0f));
	model_data->mesh_lods[0].meshes[0].channels[0].push_back(Vec2f(1.0f, 0.0f));
	model_data->mesh_lods[0].meshes[0].channels[0].push_back(Vec2f(0.0f, 1.0f));
	model_data->mesh_lods[0].meshes[0].channels[0].push_back(Vec2f(1.0f, 1.0f));
*/
	model_data->mesh_lods[0].meshes[0].elements.push_back(2);
	model_data->mesh_lods[0].meshes[0].elements.push_back(1);
	model_data->mesh_lods[0].meshes[0].elements.push_back(0);
	model_data->mesh_lods[0].meshes[0].elements.push_back(1);
	model_data->mesh_lods[0].meshes[0].elements.push_back(2);
	model_data->mesh_lods[0].meshes[0].elements.push_back(3);

	ModelDataMaterialRange range;
	range.start_element = 0;
	range.num_elements = 6;
	range.ambient = Vec3f(1.0f);
	range.diffuse = Vec3f(1.0f);
	range.specular = Vec3f(1.0f);

	range.self_illumination_amount.timelines.resize(1);
	range.self_illumination_amount.timelines[0].timestamps.push_back(0.0f);
	range.self_illumination_amount.timelines[0].values.push_back(0.0);

	range.self_illumination.timelines.resize(1);
	range.self_illumination.timelines[0].timestamps.push_back(0.0f);
	range.self_illumination.timelines[0].values.push_back(Vec3f());

/*
	range.diffuse_map.channel = 0;
	range.diffuse_map.texture = 0;
	range.diffuse_map.uvw_offset.timelines.resize(1);
	range.diffuse_map.uvw_offset.timelines[0].timestamps.push_back(0.0f);
	range.diffuse_map.uvw_offset.timelines[0].values.push_back(Vec3f());
	range.diffuse_map.uvw_rotation.timelines.resize(1);
	range.diffuse_map.uvw_rotation.timelines[0].timestamps.push_back(0.0f);
	range.diffuse_map.uvw_rotation.timelines[0].values.push_back(Quaternionf());
	range.diffuse_map.uvw_scale.timelines.resize(1);
	range.diffuse_map.uvw_scale.timelines[0].timestamps.push_back(0.0f);
	range.diffuse_map.uvw_scale.timelines[0].values.push_back(Vec3f(1.0f));
*/

	model_data->mesh_lods[0].meshes[0].material_ranges.push_back(range);

	return model_data;
}

CachedTexture ExampleSceneCacheProvider::get_texture(GraphicContext &gc, const std::string &name, bool linear)
{
	auto it = loaded_textures.find(name);
	if (it != loaded_textures.end())
	{
		return it->second;
	}
	else
	{
		CachedTexture texture;

		ImageImportDescription desc;
		desc.set_srgb(!linear);

		texture.set_texture(Texture2D(gc, name, desc));

		loaded_textures[name] = texture;

		return texture;
	}
}

void ExampleSceneCacheProvider::update_textures(GraphicContext &gc, float time_elapsed)
{
	// Update animated textures here.
}
