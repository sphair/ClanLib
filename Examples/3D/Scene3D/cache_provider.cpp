
#include "precomp.h"
#include "cache_provider.h"

using namespace clan;

ExampleSceneCacheProvider::ExampleSceneCacheProvider()
{
}

std::shared_ptr<ModelData> ExampleSceneCacheProvider::get_model_data(const std::string &name)
{
	if (name == "plane")
		return create_plane();
	else if (name == "box")
		return create_box();
	else
		throw Exception(string_format("Unknown model: %1", name));
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

		Texture2D texture2d(gc, name, desc);
		texture.set_texture(texture2d);

		loaded_textures[name] = texture;

		return texture;
	}
}

void ExampleSceneCacheProvider::update_textures(GraphicContext &gc, float time_elapsed)
{
	// Update animated textures here.
}

std::shared_ptr<ModelData> ExampleSceneCacheProvider::create_plane()
{
	std::shared_ptr<ModelData> model_data(new ModelData());

	float size = 75.0f;

	model_data->aabb_min = Vec3f(-size, 0.0f, -size);
	model_data->aabb_max = Vec3f( size, 0.0f,  size);

	model_data->animations.resize(1);
	model_data->animations[0].name = "default";

	model_data->textures.resize(1);
	model_data->textures[0].gamma = 2.2f;
	model_data->textures[0].name = "../Clan3D/Resources/tux.png";

	model_data->mesh_lods.resize(1);
	model_data->mesh_lods[0].meshes.resize(1);
	model_data->mesh_lods[0].meshes[0].channels.resize(4);

	model_data->mesh_lods[0].meshes[0].vertices.push_back(Vec3f(-size, 0.0f,-size));
	model_data->mesh_lods[0].meshes[0].vertices.push_back(Vec3f( size, 0.0f,-size));
	model_data->mesh_lods[0].meshes[0].vertices.push_back(Vec3f(-size, 0.0f, size));
	model_data->mesh_lods[0].meshes[0].vertices.push_back(Vec3f( size, 0.0f, size));

	model_data->mesh_lods[0].meshes[0].normals.push_back(Vec3f(0.0f, 1.0f, 0.0f));
	model_data->mesh_lods[0].meshes[0].normals.push_back(Vec3f(0.0f, 1.0f, 0.0f));
	model_data->mesh_lods[0].meshes[0].normals.push_back(Vec3f(0.0f, 1.0f, 0.0f));
	model_data->mesh_lods[0].meshes[0].normals.push_back(Vec3f(0.0f, 1.0f, 0.0f));

	model_data->mesh_lods[0].meshes[0].tangents.push_back(Vec3f(1.0f, 0.0f, 0.0f));
	model_data->mesh_lods[0].meshes[0].tangents.push_back(Vec3f(1.0f, 0.0f, 0.0f));
	model_data->mesh_lods[0].meshes[0].tangents.push_back(Vec3f(1.0f, 0.0f, 0.0f));
	model_data->mesh_lods[0].meshes[0].tangents.push_back(Vec3f(1.0f, 0.0f, 0.0f));

	model_data->mesh_lods[0].meshes[0].bitangents.push_back(Vec3f(0.0f, 0.0f, 1.0f));
	model_data->mesh_lods[0].meshes[0].bitangents.push_back(Vec3f(0.0f, 0.0f, 1.0f));
	model_data->mesh_lods[0].meshes[0].bitangents.push_back(Vec3f(0.0f, 0.0f, 1.0f));
	model_data->mesh_lods[0].meshes[0].bitangents.push_back(Vec3f(0.0f, 0.0f, 1.0f));

	for (int j = 0; j < 4; j++)
	{
		model_data->mesh_lods[0].meshes[0].channels[j].push_back(Vec2f(0.0f, 0.0f));
		model_data->mesh_lods[0].meshes[0].channels[j].push_back(Vec2f(1.0f, 0.0f));
		model_data->mesh_lods[0].meshes[0].channels[j].push_back(Vec2f(0.0f, 1.0f));
		model_data->mesh_lods[0].meshes[0].channels[j].push_back(Vec2f(1.0f, 1.0f));
	}

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
	range.glossiness = 20.0f;
	range.specular_level = 25.0f;

	range.self_illumination_amount.timelines.resize(1);
	range.self_illumination_amount.timelines[0].timestamps.push_back(0.0f);
	range.self_illumination_amount.timelines[0].values.push_back(0.0);

	range.self_illumination.timelines.resize(1);
	range.self_illumination.timelines[0].timestamps.push_back(0.0f);
	range.self_illumination.timelines[0].values.push_back(Vec3f());

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

	range.specular_map.channel = 1;
	range.specular_map.texture = 0;
	range.specular_map.uvw_offset.timelines.resize(1);
	range.specular_map.uvw_offset.timelines[0].timestamps.push_back(0.0f);
	range.specular_map.uvw_offset.timelines[0].values.push_back(Vec3f());
	range.specular_map.uvw_rotation.timelines.resize(1);
	range.specular_map.uvw_rotation.timelines[0].timestamps.push_back(0.0f);
	range.specular_map.uvw_rotation.timelines[0].values.push_back(Quaternionf());
	range.specular_map.uvw_scale.timelines.resize(1);
	range.specular_map.uvw_scale.timelines[0].timestamps.push_back(0.0f);
	range.specular_map.uvw_scale.timelines[0].values.push_back(Vec3f(1.0f));

	model_data->mesh_lods[0].meshes[0].material_ranges.push_back(range);

	return model_data;
}

std::shared_ptr<ModelData> ExampleSceneCacheProvider::create_box()
{
	std::shared_ptr<ModelData> model_data(new ModelData());

	float size = 5.0f;

	model_data->aabb_min = Vec3f(-size, 0.0f, -size);
	model_data->aabb_max = Vec3f( size, 0.0f,  size);

	model_data->animations.resize(1);
	model_data->animations[0].name = "default";

	model_data->textures.resize(1);
	model_data->textures[0].gamma = 2.2f;
	model_data->textures[0].name = "../Clan3D/Resources/tux.png";

	model_data->mesh_lods.resize(1);
	model_data->mesh_lods[0].meshes.resize(1);
	model_data->mesh_lods[0].meshes[0].channels.resize(4);

	Vec3f normal[6] =
	{
		Vec3f( 0.0f,  1.0f,  0.0f),
		Vec3f( 0.0f, -1.0f,  0.0f),
		Vec3f( 1.0f,  0.0f,  0.0f),
		Vec3f(-1.0f,  0.0f,  0.0f),
		Vec3f( 0.0f,  0.0f,  1.0f),
		Vec3f( 0.0f,  0.0f, -1.0f)
	};

	Vec3f tangent[6] =
	{
		Vec3f(1.0,  0.0f, 0.0f),
		Vec3f(1.0,  0.0f, 0.0f),
		Vec3f(0.0f, 1.0f, 0.0f),
		Vec3f(0.0f,-1.0f, 0.0f),
		Vec3f(0.0f, 1.0f, 0.0f),
		Vec3f(0.0f,-1.0f, 0.0f)
	};

	for (int i = 0; i < 6; i++)
	{
		Vec3f bitangent = Vec3f::cross(normal[i], tangent[i]);

		model_data->mesh_lods[0].meshes[0].vertices.push_back((normal[i] - tangent[i] - bitangent) * size);
		model_data->mesh_lods[0].meshes[0].vertices.push_back((normal[i] + tangent[i] - bitangent) * size);
		model_data->mesh_lods[0].meshes[0].vertices.push_back((normal[i] - tangent[i] + bitangent) * size);
		model_data->mesh_lods[0].meshes[0].vertices.push_back((normal[i] + tangent[i] + bitangent) * size);

		for (int j = 0; j < 4; j++)
		{
			model_data->mesh_lods[0].meshes[0].normals.push_back(normal[i]);
			model_data->mesh_lods[0].meshes[0].tangents.push_back(tangent[i]);
			model_data->mesh_lods[0].meshes[0].bitangents.push_back(bitangent);
		}

		for (int j = 0; j < 4; j++)
		{
			model_data->mesh_lods[0].meshes[0].channels[j].push_back(Vec2f(0.0f, 0.0f));
			model_data->mesh_lods[0].meshes[0].channels[j].push_back(Vec2f(1.0f, 0.0f));
			model_data->mesh_lods[0].meshes[0].channels[j].push_back(Vec2f(0.0f, 1.0f));
			model_data->mesh_lods[0].meshes[0].channels[j].push_back(Vec2f(1.0f, 1.0f));
		}

		model_data->mesh_lods[0].meshes[0].elements.push_back(i * 4 + 0);
		model_data->mesh_lods[0].meshes[0].elements.push_back(i * 4 + 1);
		model_data->mesh_lods[0].meshes[0].elements.push_back(i * 4 + 2);
		model_data->mesh_lods[0].meshes[0].elements.push_back(i * 4 + 3);
		model_data->mesh_lods[0].meshes[0].elements.push_back(i * 4 + 2);
		model_data->mesh_lods[0].meshes[0].elements.push_back(i * 4 + 1);
	}

	ModelDataMaterialRange range;
	range.start_element = 0;
	range.num_elements = 36;
	range.ambient = Vec3f(1.0f);
	range.diffuse = Vec3f(1.0f);
	range.specular = Vec3f(1.0f);
	range.glossiness = 20.0f;
	range.specular_level = 25.0f;

	range.self_illumination_amount.timelines.resize(1);
	range.self_illumination_amount.timelines[0].timestamps.push_back(0.0f);
	range.self_illumination_amount.timelines[0].values.push_back(0.0);

	range.self_illumination.timelines.resize(1);
	range.self_illumination.timelines[0].timestamps.push_back(0.0f);
	range.self_illumination.timelines[0].values.push_back(Vec3f());

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

	range.specular_map.channel = 1;
	range.specular_map.texture = 0;
	range.specular_map.uvw_offset.timelines.resize(1);
	range.specular_map.uvw_offset.timelines[0].timestamps.push_back(0.0f);
	range.specular_map.uvw_offset.timelines[0].values.push_back(Vec3f());
	range.specular_map.uvw_rotation.timelines.resize(1);
	range.specular_map.uvw_rotation.timelines[0].timestamps.push_back(0.0f);
	range.specular_map.uvw_rotation.timelines[0].values.push_back(Quaternionf());
	range.specular_map.uvw_scale.timelines.resize(1);
	range.specular_map.uvw_scale.timelines[0].timestamps.push_back(0.0f);
	range.specular_map.uvw_scale.timelines[0].values.push_back(Vec3f(1.0f));

	model_data->mesh_lods[0].meshes[0].material_ranges.push_back(range);

	return model_data;
}
