
#pragma once

class ExampleSceneCache : public clan::SceneCache
{
public:
	ExampleSceneCache();

	std::shared_ptr<clan::ModelData> get_model_data(const std::string &name);

	clan::Resource<clan::Texture> get_texture(clan::GraphicContext &gc, const std::string &name, bool linear);

	void update_textures(clan::GraphicContext &gc, float time_elapsed);

private:
	std::shared_ptr<clan::ModelData> create_plane();
	std::shared_ptr<clan::ModelData> create_box();

	std::map<std::string, clan::Resource<clan::Texture>> loaded_textures;
};
