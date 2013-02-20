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

#include "Scene3D/precomp.h"
#include "API/Scene3D/scene.h"
#include "API/Scene3D/scene_object.h"
#include "API/Scene3D/scene_light.h"
#include "API/Scene3D/scene_particle_emitter.h"
#include "API/Scene3D/Performance/scope_timer.h"
#include "scene_impl.h"
#include "Culling/clipping_frustum.h"
#include "Scene3D/scene_object_impl.h"

namespace clan
{

Scene::Scene()
{
}

Scene::Scene(GraphicContext &gc, const SceneCache &cache, const std::string &shader_path)
: impl(new Scene_Impl(gc, cache, shader_path))
{
	impl->set_camera(SceneCamera(*this));
}

bool Scene::is_null() const
{
	return !impl;
}

void Scene::set_viewport(const Rect &box)
{
	impl->set_viewport(box);
}

void Scene::set_camera(const SceneCamera &camera)
{
	impl->set_camera(camera);
}

void Scene::render(GraphicContext &gc)
{
	impl->render(gc);
}

void Scene::update(GraphicContext &gc, float time_elapsed)
{
	impl->update(gc, time_elapsed);
}

int Scene::instances_drawn = 0;
int Scene::models_drawn = 0;
int Scene::draw_calls = 0;
int Scene::triangles_drawn = 0;
int Scene::scene_visits = 0;
std::vector<GPUTimer::Result> Scene::gpu_results;

/////////////////////////////////////////////////////////////////////////////

Scene_Impl::Scene_Impl(GraphicContext &gc, SceneCache cache, const std::string &shader_path)
: cache(cache), frame(0)
{
	material_cache = std::unique_ptr<MaterialCache>(new MaterialCache(this));
	model_shader_cache = std::unique_ptr<ModelShaderCache>(new ModelShaderCache(shader_path));
	model_cache = std::unique_ptr<ModelCache>(new ModelCache(this, *material_cache, *model_shader_cache, instances_buffer));

	vsm_shadow_map_pass = std::unique_ptr<VSMShadowMapPass>(new VSMShadowMapPass(gc));
	gbuffer_pass = std::unique_ptr<GBufferPass>(new GBufferPass());
	skybox_pass = std::unique_ptr<SkyboxPass>(new SkyboxPass(shader_path));
	lightsource_pass = std::unique_ptr<LightsourcePass>(new LightsourcePass(gc, shader_path));
	transparency_pass = std::unique_ptr<TransparencyPass>(new TransparencyPass());
	particle_emitter_pass = std::unique_ptr<ParticleEmitterPass>(new ParticleEmitterPass(*material_cache, shader_path));
	bloom_pass = std::unique_ptr<BloomPass>(new BloomPass(gc, shader_path));
	//ssao_pass = std::unique_ptr<SSAOPass>(new SSAOPass(gc, shader_path));
	final_pass = std::unique_ptr<FinalPass>(new FinalPass(gc, shader_path));

	viewport.set(Size(640, 480));
	camera_field_of_view.set(60.0f);

	vsm_shadow_map_pass->viewport.bind_from(viewport);
	vsm_shadow_map_pass->field_of_view.bind_from(camera_field_of_view);

	gbuffer_pass->viewport.bind_from(viewport);
	gbuffer_pass->field_of_view.bind_from(camera_field_of_view);

	skybox_pass->viewport.bind_from(viewport);
	skybox_pass->field_of_view.bind_from(camera_field_of_view);
	skybox_pass->diffuse_color_gbuffer.bind_from(gbuffer_pass->diffuse_color_gbuffer);
	skybox_pass->specular_color_gbuffer.bind_from(gbuffer_pass->specular_color_gbuffer);
	skybox_pass->specular_level_gbuffer.bind_from(gbuffer_pass->specular_level_gbuffer);
	skybox_pass->self_illumination_gbuffer.bind_from(gbuffer_pass->self_illumination_gbuffer);
	skybox_pass->normal_z_gbuffer.bind_from(gbuffer_pass->normal_z_gbuffer);
	skybox_pass->zbuffer.bind_from(gbuffer_pass->zbuffer);

	lightsource_pass->viewport.bind_from(viewport);
	lightsource_pass->field_of_view.bind_from(camera_field_of_view);
	lightsource_pass->diffuse_color_gbuffer.bind_from(gbuffer_pass->diffuse_color_gbuffer);
	lightsource_pass->specular_color_gbuffer.bind_from(gbuffer_pass->specular_color_gbuffer);
	lightsource_pass->specular_level_gbuffer.bind_from(gbuffer_pass->specular_level_gbuffer);
	lightsource_pass->self_illumination_gbuffer.bind_from(gbuffer_pass->self_illumination_gbuffer);
	lightsource_pass->normal_z_gbuffer.bind_from(gbuffer_pass->normal_z_gbuffer);
	lightsource_pass->shadow_maps.bind_from(vsm_shadow_map_pass->shadow_maps);

	transparency_pass->viewport.bind_from(viewport);
	transparency_pass->field_of_view.bind_from(camera_field_of_view);
	transparency_pass->zbuffer.bind_from(gbuffer_pass->zbuffer);
	transparency_pass->final_color.bind_from(lightsource_pass->final_color);

	particle_emitter_pass->viewport.bind_from(viewport);
	particle_emitter_pass->field_of_view.bind_from(camera_field_of_view);
	particle_emitter_pass->zbuffer.bind_from(gbuffer_pass->zbuffer);
	particle_emitter_pass->normal_z_gbuffer.bind_from(gbuffer_pass->normal_z_gbuffer);
	particle_emitter_pass->final_color.bind_from(lightsource_pass->final_color);

	bloom_pass->viewport.bind_from(viewport);
	bloom_pass->final_color.bind_from(lightsource_pass->final_color);

	//ssao_pass->viewport.bind_from(viewport);
	//ssao_pass->normal_z_gbuffer.bind_from(gbuffer_pass_normal_z_gbuffer);

	final_pass->viewport.bind_from(viewport);
	final_pass->final_color.bind_from(lightsource_pass->final_color);
	final_pass->bloom_blur_texture.bind_from(bloom_pass->bloom_contribution);
	//final_pass->ambient_occlusion.bind_from(ssao_pass->ssao_contribution);
}

void Scene_Impl::set_viewport(const Rect &box)
{
	viewport.set(box);
}

void Scene_Impl::render(GraphicContext &gc)
{
	ScopeTimeFunction();

	Scene::models_drawn = 0;
	Scene::instances_drawn = 0;
	Scene::draw_calls = 0;
	Scene::triangles_drawn = 0;
	Scene::scene_visits = 0;

	gpu_timer.begin_frame(gc);

	if (camera_field_of_view.get() != camera.get_field_of_view())
		camera_field_of_view.set(camera.get_field_of_view());

	Quaternionf inv_orientation = Quaternionf::inverse(camera.get_orientation());
	Mat4f world_to_eye = inv_orientation.to_matrix() * Mat4f::translate(-camera.get_position());

	// To do: move this elsewhere and set as OutData<Mat4f>:
	gbuffer_pass->world_to_eye.set(world_to_eye);
	skybox_pass->world_to_eye.set(world_to_eye);
	vsm_shadow_map_pass->world_to_eye.set(world_to_eye);
	lightsource_pass->world_to_eye.set(world_to_eye);
	transparency_pass->world_to_eye.set(world_to_eye);
	particle_emitter_pass->world_to_eye.set(world_to_eye);

	gpu_timer.begin_time(gc, "gbuffer");
	gbuffer_pass->run(gc, this);
	gpu_timer.end_time(gc);

	gpu_timer.begin_time(gc, "skybox");
	skybox_pass->run(gc, this);
	gpu_timer.end_time(gc);

	gpu_timer.begin_time(gc, "vsm");
	vsm_shadow_map_pass->run(gc, this);
	gpu_timer.end_time(gc);

	lightsource_pass->run(gc, this);

	gpu_timer.begin_time(gc, "transparency");
	transparency_pass->run(gc, this);
	gpu_timer.end_time(gc);

	gpu_timer.begin_time(gc, "particles");
	particle_emitter_pass->run(gc, this);
	gpu_timer.end_time(gc);

	gpu_timer.begin_time(gc, "bloom");
	bloom_pass->run(gc);
	gpu_timer.end_time(gc);

	//gpu_timer.begin_time(gc, "ssao");
	//ssao_pass->run(gc);

	gpu_timer.begin_time(gc, "final");
	final_pass->run(gc);
	gpu_timer.end_time(gc);

	gpu_timer.end_frame(gc);

	Scene::gpu_results = gpu_timer.get_results(gc);

	if (gc.get_shader_language() == shader_glsl)
		OpenGL::check_error();
}

void Scene_Impl::update(GraphicContext &gc, float time_elapsed)
{
	material_cache->update(gc, time_elapsed);
	particle_emitter_pass->update(gc, time_elapsed);
	// To do: update scene object animations here too
}

void Scene_Impl::visit(GraphicContext &gc, const Mat4f &world_to_eye, const Mat4f &eye_to_projection, ClippingFrustum frustum, ModelMeshVisitor *visitor)
{
	ScopeTimeFunction();
	Scene::scene_visits++;

	std::vector<Model *> models;

	std::vector<VisibleObject *> visible_objects = tree.cull(frame, frustum);
	for (size_t i = 0; i < visible_objects.size(); i++)
	{
		SceneObject_Impl *object = dynamic_cast<SceneObject_Impl*>(visible_objects[i]);
		if (object)
		{
			if (object->instance.get_renderer())
			{
				Scene::instances_drawn++;
				bool first_instance = object->instance.get_renderer()->add_instance(frame, object->instance, object->get_object_to_world());
				if (first_instance)
				{
					models.push_back(object->instance.get_renderer().get());
					Scene::models_drawn++;
				}
			}
		}
	}

	frame++;

	instances_buffer.clear();
	for (size_t i = 0; i < models.size(); i++)
		instances_buffer.add(models[i]->get_instance_vectors_count());

	instances_buffer.lock(gc);
	for (size_t i = 0; i < models.size(); i++)
		models[i]->upload(instances_buffer, world_to_eye, eye_to_projection);
	instances_buffer.unlock(gc);

	for (size_t i = 0; i < models.size(); i++)
		models[i]->visit(gc, instances_buffer, visitor);
}

void Scene_Impl::visit_lights(GraphicContext &gc, const Mat4f &world_to_eye, const Mat4f &eye_to_projection, ClippingFrustum frustum, SceneLightVisitor *visitor)
{
	ScopeTimeFunction();

	std::vector<VisibleObject *> visible_objects = tree.cull(frame++, frustum);
	for (size_t i = 0; i < visible_objects.size(); i++)
	{
		SceneLight_Impl *light = dynamic_cast<SceneLight_Impl*>(visible_objects[i]);
		if (light)
		{
			visitor->light(gc, world_to_eye, eye_to_projection, light);
		}
	}
}

void Scene_Impl::visit_emitters(GraphicContext &gc, const Mat4f &world_to_eye, const Mat4f &eye_to_projection, ClippingFrustum frustum, SceneParticleEmitterVisitor *visitor)
{
	ScopeTimeFunction();

	std::vector<VisibleObject *> visible_objects = tree.cull(frame++, frustum);
	for (size_t i = 0; i < visible_objects.size(); i++)
	{
		SceneParticleEmitter_Impl *emitter = dynamic_cast<SceneParticleEmitter_Impl*>(visible_objects[i]);
		if (emitter)
		{
			visitor->emitter(gc, world_to_eye, eye_to_projection, emitter);
		}
	}
}

}

