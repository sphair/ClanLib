/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

/// \addtogroup clanScene_Scene clanScene Scene
/// \{

#pragma once

#include "api_scene3d.h"
#include "../Display/Render/graphic_context.h"
#include "Performance/gpu_timer.h"
#include <memory>

namespace clan
{

class Scene_Impl;
class SceneLight;
class SceneParticleEmitter;
class SceneObject;
class SceneCamera;
class SceneCache;
class ScenePass;

class CL_API_SCENE Scene
{
public:
	Scene();
	Scene(GraphicContext &gc, const SceneCache &cache, const std::string &shader_path);

	bool is_null() const;

	const SceneCamera &get_camera() const;
	SceneCamera &get_camera();

	ResourceContainer &get_inout_container();

	template<typename Type>
	Resource<Type> get_inout(const std::string &name)
	{
		return get_inout_container().get<Type>(name);
	}

	void set_viewport(const Rect &box);
	void set_camera(const SceneCamera &camera);

	void render(GraphicContext &gc);

	void update(GraphicContext &gc, float time_elapsed);

	Mat4f world_to_eye() const;
	Mat4f eye_to_projection() const;
	Mat4f world_to_projection() const;

	void unproject(const Vec2i &screen_pos, Vec3f &out_ray_start, Vec3f &out_ray_direction);

	void set_cull_oct_tree(const AxisAlignedBoundingBox &aabb);
	void set_cull_oct_tree(const Vec3f &aabb_min, const Vec3f &aabb_max);
	void set_cull_oct_tree(float max_size);

	ScenePass add_pass(const std::string &name, const std::string &insert_before = std::string());
	void remove_pass(const std::string &name);

	void show_skybox_stars(bool enable);
	void set_skybox_gradient(GraphicContext &gc, std::vector<Colorf> &colors);

	// To do: should not be static, should be getter functions, etc.
	static int models_drawn;
	static int instances_drawn;
	static int draw_calls;
	static int triangles_drawn;
	static int scene_visits;
	static std::vector<GPUTimer::Result> gpu_results;

private:
	std::shared_ptr<Scene_Impl> impl;

	friend class SceneLight;
	friend class SceneParticleEmitter;
	friend class SceneObject;
	friend class SceneCamera;
	friend class SceneModel;
	friend class ScenePass;
};

}

/// \}
