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
#include "lightsource_simple_pass.h"
#include "Scene3D/Framework/shader_setup.h"
#include "Scene3D/Passes/VSMShadowMap/vsm_shadow_map_pass.h"
#include "Scene3D/Culling/clipping_frustum.h"
#include "Scene3D/scene_impl.h"
#include "API/Scene3D/scene.h"
#include "API/Scene3D/Performance/scope_timer.h"
#include "API/Scene3D/Performance/gpu_timer.h"

namespace clan
{

LightsourceSimplePass::LightsourceSimplePass(GraphicContext &gc, const std::string &shader_path)
{
	if (gc.get_shader_language() == shader_glsl)
	{
		light_program = ShaderSetup::compile(gc, "", PathHelp::combine(shader_path, "LightsourceSimple/vertex_light.glsl"), PathHelp::combine(shader_path, "LightsourceSimple/fragment_light.glsl"), "");
	}
	else
	{
		light_program = ShaderSetup::compile(gc, "", PathHelp::combine(shader_path, "LightsourceSimple/vertex_light.hlsl"), PathHelp::combine(shader_path, "LightsourceSimple/fragment_light.hlsl"), "");
	}

	light_program.bind_frag_data_location(0, "FragColor");

	if (!light_program.link())
		throw Exception("Shader linking failed!");

	light_program.bind_attribute_location(0, "AttrPositionInObject");
	light_program.set_uniform_buffer_index("Uniforms", 0);
	light_program.set_uniform1i("InstanceTexture", 0);
	light_program.set_uniform1i("NormalZTexture", 1);
	light_program.set_uniform1i("DiffuseColorTexture", 2);
	light_program.set_uniform1i("SpecularColorTexture", 3);
	light_program.set_uniform1i("SpecularLevelTexture", 4);
	light_program.set_uniform1i("ShadowMapsTexture", 5);
	light_program.set_uniform1i("ShadowMapsTextureSampler", 5);
	light_program.set_uniform1i("SelfIlluminationTexture", 6);

	light_instance_texture = Texture1D(gc, max_lights * vectors_per_light, tf_rgba32f);
	light_instance_transfer = PixelBuffer(max_lights * vectors_per_light, 1, tf_rgba32f);

	icosahedron.reset(new Icosahedron(gc));
}

LightsourceSimplePass::~LightsourceSimplePass()
{
}

void LightsourceSimplePass::setup(GraphicContext &gc)
{
	Size viewport_size = viewport->get_size();
	if (fb.is_null() || !gc.is_frame_buffer_owner(fb) || final_color.updated() || zbuffer.updated() || diffuse_color_gbuffer.updated())
	{
		final_color.set(Texture2D(gc, viewport->get_width(), viewport->get_height(), tf_rgba32f));

		fb = FrameBuffer(gc);
		fb.attach_color(0, final_color.get());
		fb.attach_depth(zbuffer.get());

		BlendStateDescription blend_desc;
		blend_desc.enable_blending(true);
		blend_desc.set_blend_function(blend_one, blend_one, blend_one, blend_one);
		blend_state = BlendState(gc, blend_desc);

		DepthStencilStateDescription depth_stencil_desc;
		depth_stencil_desc.enable_depth_write(false);
		depth_stencil_desc.enable_depth_test(true);
		depth_stencil_desc.set_depth_compare_function(compare_lequal);
		depth_stencil_state = DepthStencilState(gc, depth_stencil_desc);

		RasterizerStateDescription rasterizer_desc;
		rasterizer_desc.set_culled(true);
		rasterizer_state = RasterizerState(gc, rasterizer_desc);

		uniforms = UniformVector<Uniforms>(gc, 1);

		prim_array = PrimitivesArray(gc);
		prim_array.set_attributes(0, icosahedron->vertices);
	}
}

void LightsourceSimplePass::run(GraphicContext &gc, Scene_Impl *scene)
{
	setup(gc);
	find_lights(gc, scene);
	upload(gc, scene);
	render(gc, scene->get_gpu_timer());
}

void LightsourceSimplePass::find_lights(GraphicContext &gc, Scene_Impl *scene)
{
	lights.clear();

	Size viewport_size = viewport->get_size();

	Mat4f eye_to_projection = Mat4f::perspective(field_of_view.get(), viewport_size.width/(float)viewport_size.height, 0.1f, 1.e10f, handed_left, gc.get_clip_z_range());
	Mat4f eye_to_cull_projection = Mat4f::perspective(field_of_view.get(), viewport_size.width/(float)viewport_size.height, 0.1f, 150.0f, handed_left, clip_negative_positive_w);
	ClippingFrustum frustum(eye_to_cull_projection * world_to_eye.get());
	scene->visit_lights(gc, world_to_eye.get(), eye_to_projection, frustum, this);
}

void LightsourceSimplePass::light(GraphicContext &gc, const Mat4f &world_to_eye, const Mat4f &eye_to_projection, SceneLight_Impl *light)
{
	if ((light->type == SceneLight::type_omni || light->type == SceneLight::type_spot) && light->light_caster && lights.size() < max_lights - 1)
	{
		lights.push_back(light);
	}
}

void LightsourceSimplePass::upload(GraphicContext &gc, Scene_Impl *scene)
{
	ScopeTimeFunction();

	Size viewport_size = viewport->get_size();
	Mat4f eye_to_projection = Mat4f::perspective(field_of_view.get(), viewport_size.width/(float)viewport_size.height, 0.1f, 1.e4f, handed_left, gc.get_clip_z_range());

	float aspect = viewport->get_width()/(float)viewport->get_height();
	float field_of_view_y_degrees = field_of_view.get();
	float field_of_view_y_rad = (float)(field_of_view_y_degrees * PI / 180.0);
	float f = 1.0f / tan(field_of_view_y_rad * 0.5f);
	float rcp_f = 1.0f / f;
	float rcp_f_div_aspect = 1.0f / (f / aspect);
	Vec2f two_rcp_viewport_size(2.0f / viewport->get_width(), 2.0f / viewport->get_height());

	Uniforms cpu_uniforms;
	cpu_uniforms.eye_to_projection = eye_to_projection;
	cpu_uniforms.object_to_eye = Quaternionf::inverse(scene->get_camera().get_orientation()).to_matrix();
	cpu_uniforms.rcp_f = rcp_f;
	cpu_uniforms.rcp_f_div_aspect = rcp_f_div_aspect;
	cpu_uniforms.two_rcp_viewport_size = two_rcp_viewport_size;
	uniforms.upload_data(gc, &cpu_uniforms, 1);

	int num_lights = lights.size();
	if (num_lights == 0)
		return;

	Mat4f normal_world_to_eye = Mat4f(Mat3f(world_to_eye.get())); // This assumes uniform scale
	Mat4f eye_to_world = Mat4f::inverse(world_to_eye.get());

	PixelBufferLock4f lock(light_instance_transfer);
	Vec4f *instance_data = lock.get_row(0);

	for (int i = 0; i < num_lights; i++)
	{
		float radius = lights[i]->attenuation_end;
		if (lights[i]->rectangle_shape)
			radius *= 1.414213562373f;

		float attenuation_delta = lights[i]->attenuation_end - lights[i]->attenuation_start;
		if (attenuation_delta == 0.0f)
			attenuation_delta = 1e-6f;
		float sqr_radius = radius * radius;
#ifdef USE_QUADRATIC_ATTENUATION
		float sqr_attenuation_start = lights[i]->attenuation_start * lights[i]->attenuation_start;
		float sqr_attenuation_delta = attenuation_delta * attenuation_delta;
#else
		float attenuation_start = lights[i]->attenuation_start;
#endif
		float sqr_falloff_begin = 0.0f;
		float light_type = 0.0f;
		if (lights[i]->type == SceneLight::type_spot)
		{
			light_type = lights[i]->rectangle_shape ? 2.0f : 1.0f;
			float falloff_begin = lights[i]->hotspot / lights[i]->falloff;
			sqr_falloff_begin = falloff_begin * falloff_begin;
		}
		Vec3f position_in_eye = Vec3f(world_to_eye.get() * Vec4f(lights[i]->position, 1.0f));
		Mat4f eye_to_shadow_projection = lights[i]->vsm_data->world_to_shadow_projection * eye_to_world;

		int shadow_map_index = lights[i]->vsm_data->shadow_map.get_index();

		instance_data[i * vectors_per_light + 0] = Vec4f(position_in_eye, (float)shadow_map_index);
		instance_data[i * vectors_per_light + 1] = Vec4f(lights[i]->color, lights[i]->ambient_illumination);
#ifdef USE_QUADRATIC_ATTENUATION
		instance_data[i * vectors_per_light + 2] = Vec4f(sqr_radius, sqr_attenuation_start, 1.0f / sqr_attenuation_delta, sqr_falloff_begin);
#else
		instance_data[i * vectors_per_light + 2] = Vec4f(sqr_radius, attenuation_start, 1.0f / attenuation_delta, sqr_falloff_begin);
#endif
		instance_data[i * vectors_per_light + 3] = Vec4f(eye_to_shadow_projection[0], eye_to_shadow_projection[4], eye_to_shadow_projection[8], light_type);
		instance_data[i * vectors_per_light + 4] = Vec4f(eye_to_shadow_projection[1], eye_to_shadow_projection[5], eye_to_shadow_projection[9], 0.0f);
		instance_data[i * vectors_per_light + 5] = Vec4f(eye_to_shadow_projection[2], eye_to_shadow_projection[6], eye_to_shadow_projection[10], 0.0f);
	}

	lock.unlock();

	light_instance_texture.set_image(gc, light_instance_transfer);
}

void LightsourceSimplePass::render(GraphicContext &gc, GPUTimer &timer)
{
	ScopeTimeFunction();

	if (!lights.empty())
	{
		timer.begin_time(gc, "light(simple)");

		gc.set_frame_buffer(fb);

		gc.set_viewport(viewport->get_size());

		gc.set_depth_stencil_state(depth_stencil_state);
		gc.set_blend_state(blend_state);
		gc.set_rasterizer_state(rasterizer_state);

		gc.set_depth_range(0.0f, 0.9f);

		gc.set_uniform_buffer(0, uniforms);
		gc.set_texture(0, light_instance_texture);
		gc.set_texture(1, normal_z_gbuffer.get());
		gc.set_texture(2, diffuse_color_gbuffer.get());
		gc.set_texture(3, specular_color_gbuffer.get());
		gc.set_texture(4, specular_level_gbuffer.get());
		gc.set_texture(5, shadow_maps.get());
		gc.set_texture(6, self_illumination_gbuffer.get());

		gc.set_program_object(light_program);

		gc.clear();

		gc.set_primitives_array(prim_array);
		gc.draw_primitives_elements_instanced(type_triangles, icosahedron->num_elements, icosahedron->elements, 0, lights.size());
		gc.reset_primitives_array();

		timer.end_time(gc);
		timer.begin_time(gc, "light(simple)");

		gc.reset_texture(6);
		gc.reset_texture(5);
		gc.reset_texture(4);
		gc.reset_texture(3);
		gc.reset_texture(2);
		gc.reset_texture(1);
		gc.reset_texture(0);
		gc.reset_uniform_buffer(0);

		timer.end_time(gc);
	}
}

}
