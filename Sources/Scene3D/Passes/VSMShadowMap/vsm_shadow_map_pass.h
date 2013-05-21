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
#pragma once

#include "Scene3D/Framework/shadow_maps.h"
#include "Scene3D/Model/model_mesh_visitor.h"
#include "Scene3D/Passes/GaussianBlur/gaussian_blur.h"
#include "API/Scene3D/scene_light.h"
#include "Scene3D/scene_light_impl.h"

namespace clan
{

class Scene_Impl;
class VSMShadowMapPassLightData;

class VSMShadowMapPass : ModelMeshVisitor, SceneLightVisitor
{
public:
	VSMShadowMapPass(GraphicContext &gc, ResourceContainer &inout);
	void run(GraphicContext &gc, Scene_Impl *scene);

private:
	void find_lights(Scene_Impl *scene);
	void assign_shadow_map_indexes();
	void render_maps(Scene_Impl *scene);
	void blur_maps();

	// ModelMeshVisitor
	void render(GraphicContext &gc, ModelLOD *model_lod, int num_instances);

	// SceneLightVisitor
	void light(GraphicContext &gc, const Mat4f &world_to_eye, const Mat4f &eye_to_projection, SceneLight_Impl *light);

	// In:
	Resource<Rect> viewport;
	Resource<float> field_of_view;
	Resource<Mat4f> world_to_eye;

	std::vector<SceneLight_Impl *> lights;
	int round_robin;
	std::vector<size_t> blur_indexes;

	ShadowMaps maps;
	GaussianBlur blur;
	GraphicContext gc;
	BlendState blend_state;
	DepthStencilState depth_stencil_state;

	friend class VSMShadowMapPassLightData;
};

class VSMShadowMapPassLightData
{
public:
	VSMShadowMapPassLightData(VSMShadowMapPass *pass, SceneLight_Impl *light) : pass(pass), light(light), shadow_map(&pass->maps) { }

	VSMShadowMapPass *pass;
	SceneLight_Impl *light;

	Mat4f eye_to_projection;
	Mat4f world_to_eye;
	Mat4f world_to_shadow_projection;
	ShadowMapEntry shadow_map;
};

}

