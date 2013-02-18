
#include "precomp.h"
#include "cache_provider.h"

using namespace clan;

ExampleSceneCacheProvider::ExampleSceneCacheProvider()
{
}

std::shared_ptr<ModelData> ExampleSceneCacheProvider::get_model_data(const std::string &name)
{
	return std::shared_ptr<ModelData>();
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
