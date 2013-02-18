
#pragma once

class ExampleSceneCacheProvider : public clan::SceneCacheProvider
{
public:
	ExampleSceneCacheProvider();

	std::shared_ptr<clan::ModelData> get_model_data(const std::string &name);

	clan::CachedTexture get_texture(clan::GraphicContext &gc, const std::string &name, bool linear);

	void update_textures(clan::GraphicContext &gc, float time_elapsed);

private:
	std::shared_ptr<clan::ModelData> create_plane();
	std::shared_ptr<clan::ModelData> create_box();

	std::map<std::string, clan::CachedTexture> loaded_textures;
};
