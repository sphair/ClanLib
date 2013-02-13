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

#include "Scene3D/Framework/material_cache.h"
#include "Scene3D/Framework/instances_buffer.h"
#include "Scene3D/Model/model_shader_cache.h"
#include "Scene3D/Model/model_cache.h"
#include "Scene3D/Passes/VSMShadowMap/vsm_shadow_map_pass.h"
#include "Scene3D/Passes/Lightsource/lightsource_pass.h"
#include "Scene3D/Passes/GBuffer/gbuffer_pass.h"
#include "Scene3D/Passes/Skybox/skybox_pass.h"
#include "Scene3D/Passes/Bloom/bloom_pass.h"
#include "Scene3D/Passes/SSAO/ssao_pass.h"
#include "Scene3D/Passes/Final/final_pass.h"
#include "Scene3D/Passes/Transparency/transparency_pass.h"
#include "Scene3D/Passes/ParticleEmitter/particle_emitter_pass.h"
#include "Scene3D/Culling/OctTree/oct_tree.h"
#include "Performance/gpu_timer.h"
#include <list>
namespace clan
{

class ModelMeshVisitor;
class ClippingFrustum;
class SceneObject_Impl;
class SceneLight_Impl;
class SceneLightVisitor;
class SceneParticleEmitter_Impl;
class SceneParticleEmitterVisitor;

class Scene_Impl
{
public:
	Scene_Impl(clan::GraphicContext &gc);
	void set_viewport(const clan::Rect &box);
	void set_camera(const clan::Vec3f &position, const clan::Quaternionf &orientation);
	void set_camera_position(const clan::Vec3f &position);
	void set_camera_orientation(const clan::Quaternionf &orientation);
	void set_camera_field_of_view(float fov) { camera_field_of_view.set(fov); }
	void render(clan::GraphicContext &gc);
	void update(clan::GraphicContext &gc, float time_elapsed);

	void visit(clan::GraphicContext &gc, const clan::Mat4f &world_to_eye, const clan::Mat4f &eye_to_projection, ClippingFrustum frustum, ModelMeshVisitor *visitor);
	void visit_lights(clan::GraphicContext &gc, const clan::Mat4f &world_to_eye, const clan::Mat4f &eye_to_projection, ClippingFrustum frustum, SceneLightVisitor *visitor);
	void visit_emitters(clan::GraphicContext &gc, const clan::Mat4f &world_to_eye, const clan::Mat4f &eye_to_projection, ClippingFrustum frustum, SceneParticleEmitterVisitor *visitor);
	GPUTimer &get_gpu_timer() { return gpu_timer; }

	clan::Vec3f get_camera_position() const { return camera_position; }
	clan::Quaternionf get_camera_orientation() const { return camera_orientation; }

	static int models_drawn;
	static int instances_drawn;
	static int draw_calls;
	static int triangles_drawn;
	static int scene_visits;
	static std::vector<GPUTimer::Result> gpu_results;

private:
	int frame;
	InstancesBuffer instances_buffer;
	ModelCache model_cache;
	WorkQueue work_queue;
	MaterialCache material_cache;
	ModelShaderCache model_shader_cache;

	std::list<SceneObject_Impl *> objects;
	std::list<SceneLight_Impl *> lights;
	std::list<SceneParticleEmitter_Impl *> emitters;
	OctTree tree;

	clan::Vec3f camera_position;
	clan::Quaternionf camera_orientation;
	OutData<float> camera_field_of_view;

	OutData<clan::Rect> viewport;
	VSMShadowMapPass vsm_shadow_map_pass;
	GBufferPass gbuffer_pass;
	SkyboxPass skybox_pass;
	LightsourcePass lightsource_pass;
	TransparencyPass transparency_pass;
	ParticleEmitterPass particle_emitter_pass;
	BloomPass bloom_pass;
	//SSAOPass ssao_pass;
	FinalPass final_pass;

	GPUTimer gpu_timer;

	friend class SceneObject;
	friend class SceneObject_Impl;
	friend class SceneLight;
	friend class SceneLight_Impl;
	friend class SceneParticleEmitter;
	friend class SceneParticleEmitter_Impl;
};

}

