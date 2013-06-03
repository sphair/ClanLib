
#include "precomp.h"
#include "fbx_scene_cache.h"
#include "fbx_model_loader.h"

using namespace clan;

FBXSceneCache::FBXSceneCache()
{
}

std::shared_ptr<ModelData> FBXSceneCache::get_model_data(const std::string &name)
{
	return FBXModelLoader::load("test.fbx");
}

Resource<Texture> FBXSceneCache::get_texture(GraphicContext &gc, const std::string &name, bool linear)
{
	return Resource<Texture>();
}

void FBXSceneCache::update_textures(GraphicContext &gc, float time_elapsed)
{
}
