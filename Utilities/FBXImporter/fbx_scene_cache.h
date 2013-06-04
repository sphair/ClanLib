
#pragma once

class FBXSceneCache : public clan::SceneCache
{
public:
	FBXSceneCache();

	std::shared_ptr<clan::ModelData> get_model_data(const std::string &name);
	clan::Resource<clan::Texture> get_texture(clan::GraphicContext &gc, const std::string &name, bool linear);
	void update_textures(clan::GraphicContext &gc, float time_elapsed);

private:
	clan::Texture2D get_dummy_texture(clan::GraphicContext &gc);

	clan::Texture2D dummy_texture;
	std::map<std::string, clan::Resource<clan::Texture> > loaded_textures;
};
