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

#include "Scene3D/precomp.h"
#include "API/Scene3D/scene.h"
#include "API/Scene3D/scene_object.h"
#include "API/Scene3D/scene_light.h"
#include "API/Scene3D/scene_particle_emitter.h"
#include "API/Scene3D/Performance/scope_timer.h"
#include "API/Core/Math/frustum_planes.h"
#include "Scene3D/Culling/OctTree/oct_tree.h"
#include "scene_impl.h"
#include "scene_object_impl.h"
#include "scene_light_probe_impl.h"
#include "scene_pass_impl.h"

namespace clan
{

Scene::Scene()
{
}

Scene::Scene(GraphicContext &gc, const ResourceManager &resources, const std::string &shader_path)
: impl(new Scene_Impl(gc, resources, shader_path))
{
	impl->set_camera(SceneCamera(*this));
}

bool Scene::is_null() const
{
	return !impl;
}

const SceneCamera &Scene::get_camera() const
{
	return impl->get_camera();
}

SceneCamera &Scene::get_camera()
{
	return impl->get_camera();
}

ResourceContainer &Scene::get_inout_container()
{
	return impl->inout_data;
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

Mat4f Scene::world_to_eye() const
{
	Quaternionf inv_orientation = Quaternionf::inverse(impl->camera.get_orientation());
	return inv_orientation.to_matrix() * Mat4f::translate(-impl->camera.get_position());
}

Mat4f Scene::eye_to_projection() const
{
	Size viewport_size = impl->viewport->get_size();
	return Mat4f::perspective(impl->camera_field_of_view.get(), viewport_size.width/(float)viewport_size.height, 0.1f, 1.e10f, handed_left, clip_negative_positive_w);
}

Mat4f Scene::world_to_projection() const
{
	return eye_to_projection() * world_to_eye();
}

void Scene::unproject(const Vec2i &screen_pos, Vec3f &out_ray_start, Vec3f &out_ray_direction)
{
	Size viewport_size = impl->viewport->get_size();

	float aspect = impl->viewport->get_width()/(float)impl->viewport->get_height();
	float field_of_view_y_degrees = impl->camera_field_of_view.get();
	float field_of_view_y_rad = (float)(field_of_view_y_degrees * PI / 180.0);
	float f = 1.0f / tan(field_of_view_y_rad * 0.5f);
	float rcp_f = 1.0f / f;
	float rcp_f_div_aspect = 1.0f / (f / aspect);

	Vec2f pos(screen_pos.x - impl->viewport->left, impl->viewport->bottom - screen_pos.y);

	Vec2f normalized(pos.x * 2.0f / impl->viewport->get_width(), pos.y * 2.0f / impl->viewport->get_height());
	normalized -= 1.0f;

	Vec3f ray_direction(normalized.x * rcp_f_div_aspect, normalized.y * rcp_f, 1.0f);

	out_ray_start = impl->camera.get_position();
	out_ray_direction = impl->camera.get_orientation().rotate_vector(ray_direction);
}

void Scene::set_cull_oct_tree(const AxisAlignedBoundingBox &aabb)
{
	impl->cull_provider = std::unique_ptr<SceneCullProvider>(new OctTree(aabb));
}

void Scene::set_cull_oct_tree(const Vec3f &aabb_min, const Vec3f &aabb_max)
{
	if (!impl->objects.empty() || !impl->lights.empty() || !impl->emitters.empty() || !impl->light_probes.empty())
		throw Exception("Cannot change scene culling strategy after objects have been added");

	set_cull_oct_tree(AxisAlignedBoundingBox(aabb_min, aabb_max));
}

void Scene::set_cull_oct_tree(float max_size)
{
	set_cull_oct_tree(AxisAlignedBoundingBox(Vec3f(-max_size), Vec3f(max_size)));
}

ScenePass Scene::add_pass(const std::string &name, const std::string &insert_before)
{
	return impl->add_pass(name, insert_before);
}

void Scene::remove_pass(const std::string &name)
{
	for (size_t i = 0; i < impl->passes.size(); i++)
	{
		if (impl->passes[i].get_name() == name)
		{
			impl->passes.erase(impl->passes.begin() + i);
			return;
		}
	}
	throw Exception(string_format("Pass %1 not found", name));
}

void Scene::show_skybox_stars(bool enable)
{
	impl->skybox_pass->show_skybox_stars(enable);
}

void Scene::set_skybox_gradient(GraphicContext &gc, std::vector<Colorf> &colors)
{
	PixelBuffer pb(1, colors.size(), tf_rgba32f);
	Vec4f *pixels = pb.get_data<Vec4f>();

	for (size_t i = 0; i < colors.size(); i++)
	{
		pixels[i] = Vec4f(colors[i].r, colors[i].g, colors[i].b, colors[i].a * 0.25f);
	}

	Texture2D texture(gc, pb.get_size(), tf_rgba32f);
	texture.set_image(gc, pb);
	texture.set_min_filter(filter_linear);
	texture.set_mag_filter(filter_linear);

	impl->skybox_pass->set_skybox_texture(texture);
}

int Scene::instances_drawn = 0;
int Scene::models_drawn = 0;
int Scene::draw_calls = 0;
int Scene::triangles_drawn = 0;
int Scene::scene_visits = 0;
std::vector<GPUTimer::Result> Scene::gpu_results;

/////////////////////////////////////////////////////////////////////////////

Scene_Impl::Scene_Impl(GraphicContext &gc, const ResourceManager &resources, const std::string &shader_path)
: resources(resources), frame(0)
{
	cull_provider = std::unique_ptr<SceneCullProvider>(new OctTree());

	material_cache = std::unique_ptr<MaterialCache>(new MaterialCache(this));
	model_shader_cache = std::unique_ptr<ModelShaderCache>(new ModelShaderCache(shader_path));
	model_cache = std::unique_ptr<ModelCache>(new ModelCache(this, *material_cache, *model_shader_cache, instances_buffer));

	vsm_shadow_map_pass = std::unique_ptr<VSMShadowMapPass>(new VSMShadowMapPass(gc, inout_data));
	gbuffer_pass = std::unique_ptr<GBufferPass>(new GBufferPass(inout_data));
	skybox_pass = std::unique_ptr<SkyboxPass>(new SkyboxPass(shader_path, inout_data));
	transparency_pass = std::unique_ptr<TransparencyPass>(new TransparencyPass(inout_data));
	particle_emitter_pass = std::unique_ptr<ParticleEmitterPass>(new ParticleEmitterPass(*material_cache, shader_path, inout_data));
	bloom_pass = std::unique_ptr<BloomPass>(new BloomPass(gc, shader_path, inout_data));
	//ssao_pass = std::unique_ptr<SSAOPass>(new SSAOPass(gc, shader_path, inout_data));
	final_pass = std::unique_ptr<FinalPass>(new FinalPass(gc, shader_path, inout_data));

	viewport = inout_data.get<Rect>("Viewport");
	camera_field_of_view = inout_data.get<float>("FieldOfView");
	out_world_to_eye = inout_data.get<Mat4f>("WorldToEye");

	viewport.set(Size(640, 480));
	camera_field_of_view.set(60.0f);

	bool use_compute_shader_pass = true;

	if (gc.get_shader_language() == shader_glsl) // Compute shaders introduced in OpenGL 4.3
	{
		use_compute_shader_pass = gc.has_compute_shader_support();
	}
	else if (gc.get_shader_language() == shader_hlsl) // We need compute shaders of at least Direct3D feature level 11
	{
		use_compute_shader_pass = gc.get_major_version() > 10;
	}

	if (use_compute_shader_pass)
	{
		lightsource_pass = std::unique_ptr<LightsourcePass>(new LightsourcePass(gc, shader_path, inout_data));
	}
	else
	{
		lightsource_simple_pass = std::unique_ptr<LightsourceSimplePass>(new LightsourceSimplePass(gc, shader_path, inout_data));
	}

	add_pass("gbuffer").func_run() = [=](clan::GraphicContext &gc){gbuffer_pass->run(gc, this);};
	add_pass("skybox").func_run() = [=](clan::GraphicContext &gc){skybox_pass->run(gc, this); };
	add_pass("vsm").func_run() = [=](clan::GraphicContext &gc){vsm_shadow_map_pass->run(gc, this); };
	if (lightsource_pass)
		add_pass("light").func_run() = [=](clan::GraphicContext &gc){lightsource_pass->run(gc, this); };
	else
		add_pass("light").func_run() = [=](clan::GraphicContext &gc){lightsource_simple_pass->run(gc, this); };
	add_pass("transparency").func_run() = [=](clan::GraphicContext &gc){transparency_pass->run(gc, this); };
	add_pass("particles").func_run() = [=](clan::GraphicContext &gc){particle_emitter_pass->run(gc, this); };
	add_pass("bloom").func_run() = [=](clan::GraphicContext &gc){bloom_pass->run(gc); };
	//add_pass("ssao").func_run() = [=](clan::GraphicContext &gc){ssao_pass->run(gc);};
	add_pass("final").func_run() = [=](clan::GraphicContext &gc){final_pass->run(gc); };
}

ScenePass Scene_Impl::add_pass(const std::string &name, const std::string &insert_before)
{
	ScenePass pass(std::shared_ptr<ScenePass_Impl>(new ScenePass_Impl(this, name)));
	if (insert_before.empty())
	{
		passes.push_back(pass);
		return pass;
	}
	else
	{
		for (size_t i = 0; i < passes.size(); i++)
		{
			if (passes[i].get_name() == insert_before)
			{
				passes.insert(passes.begin() + i, pass);
				return pass;
			}
		}
		throw Exception(string_format("Pass %1 not found", insert_before));
	}
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

	out_world_to_eye.set(world_to_eye);

	for (size_t i = 0; i < passes.size(); i++)
	{
		if (passes[i].func_run())
		{
			gpu_timer.begin_time(gc, passes[i].get_name());
			passes[i].func_run()(gc);
			gpu_timer.end_time(gc);
		}
	}

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

void Scene_Impl::visit(GraphicContext &gc, const Mat4f &world_to_eye, const Mat4f &eye_to_projection, FrustumPlanes frustum, ModelMeshVisitor *visitor)
{
	ScopeTimeFunction();
	Scene::scene_visits++;

	std::vector<Model *> models;

	std::vector<SceneItem *> visible_objects = cull_provider->cull(frustum);
	for (size_t i = 0; i < visible_objects.size(); i++)
	{
		SceneObject_Impl *object = dynamic_cast<SceneObject_Impl*>(visible_objects[i]);
		if (object)
		{
			if (object->instance.get_renderer())
			{
				Vec3f light_probe_color;
				if (object->light_probe_receiver)
				{
					SceneLightProbe_Impl *probe = find_nearest_probe(object->position);
					if (probe)
						light_probe_color = probe->color;
				}

				Scene::instances_drawn++;
				bool first_instance = object->instance.get_renderer()->add_instance(frame, object->instance, object->get_object_to_world(), light_probe_color);
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

void Scene_Impl::visit_lights(GraphicContext &gc, const Mat4f &world_to_eye, const Mat4f &eye_to_projection, FrustumPlanes frustum, SceneLightVisitor *visitor)
{
	ScopeTimeFunction();

	std::vector<SceneItem *> visible_objects = cull_provider->cull(frustum);
	for (size_t i = 0; i < visible_objects.size(); i++)
	{
		SceneLight_Impl *light = dynamic_cast<SceneLight_Impl*>(visible_objects[i]);
		if (light)
		{
			visitor->light(gc, world_to_eye, eye_to_projection, light);
		}
	}
}

void Scene_Impl::visit_emitters(GraphicContext &gc, const Mat4f &world_to_eye, const Mat4f &eye_to_projection, FrustumPlanes frustum, SceneParticleEmitterVisitor *visitor)
{
	ScopeTimeFunction();

	std::vector<SceneItem *> visible_objects = cull_provider->cull(frustum);
	for (size_t i = 0; i < visible_objects.size(); i++)
	{
		SceneParticleEmitter_Impl *emitter = dynamic_cast<SceneParticleEmitter_Impl*>(visible_objects[i]);
		if (emitter)
		{
			visitor->emitter(gc, world_to_eye, eye_to_projection, emitter);
		}
	}
}

SceneLightProbe_Impl *Scene_Impl::find_nearest_probe(const Vec3f &position)
{
	SceneLightProbe_Impl *probe = 0;
	float sqr_distance = 0.0f;

	std::vector<SceneItem *> visible_objects = cull_provider->cull(position);
	for (size_t i = 0; i < visible_objects.size(); i++)
	{
		SceneLightProbe_Impl *current_probe = dynamic_cast<SceneLightProbe_Impl*>(visible_objects[i]);
		if (current_probe)
		{
			Vec3f delta = current_probe->position - position;
			float current_sqr_distance = Vec3f::dot(delta, delta);
			if (probe == 0 || current_sqr_distance < sqr_distance)
			{
				probe = current_probe;
				sqr_distance = current_sqr_distance;
			}
		}
	}

	return probe;
}

}

