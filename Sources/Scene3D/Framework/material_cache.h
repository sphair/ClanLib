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

#pragma once

#include "Scene3D/Model/model_material_cache.h"
#include "API/Scene3D/cached_texture.h"
#include <unordered_map>

namespace clan
{

class Scene_Impl;

class MaterialCache : public ModelMaterialCache
{
public:
	MaterialCache(Scene_Impl *scene);
	CachedTexture get_texture(GraphicContext &gc, const std::string &material_name, bool linear);

	void update(GraphicContext &gc, float time_elapsed);

private:
	Scene_Impl *scene;

/*
private:
	static std::string to_key(const std::string &material_name, bool linear);
	Texture2D get_dummy_texture(GraphicContext &gc);

	Scene_Impl *scene;

	Texture2D dummy_texture;
	std::unordered_map<std::string, CachedTexture> textures;
	WorkQueue &work_queue;

//	std::vector<VideoTexture> video_textures;

	friend class MaterialCacheLoadTexture;
*/
};
/*
class MaterialCacheLoadTexture : public WorkItem
{
public:
	MaterialCacheLoadTexture(MaterialCache *cache, CachedTexture texture, const std::string &material_name, bool linear);

private:
	void process_work();
	void work_completed(GraphicContext &gc);
	void load_ctexture(const std::string &material_name);
	void load_clanlib_texture(const std::string &material_name);
	void load_dds_texture(const std::string &material_name);
	void load_blp_texture(const std::string &material_name);
	static bool is_power_of_two(int width, int height);

	MaterialCache *cache;
	CachedTexture texture;
	std::string material_name;
	bool linear;
	std::string video_file;

	PixelBufferSet pixelbuffer_set;
};
*/
}

