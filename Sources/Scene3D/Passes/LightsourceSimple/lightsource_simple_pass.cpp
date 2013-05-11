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
#include "lightsource_simple_pass.h"
#include "Scene3D/Framework/shader_setup.h"
#include "Scene3D/Passes/VSMShadowMap/vsm_shadow_map_pass.h"
#include "API/Core/Math/frustum_planes.h"
#include "Scene3D/scene_impl.h"
#include "API/Scene3D/scene.h"
#include "API/Scene3D/Performance/scope_timer.h"
#include "API/Scene3D/Performance/gpu_timer.h"

namespace clan
{

LightsourceSimplePass::LightsourceSimplePass(GraphicContext &gc, const std::string &shader_path, SceneInOutDataContainer &inout)
{
	viewport = inout.get<Rect>("Viewport");
	field_of_view = inout.get<float>("FieldOfView");
	world_to_eye = inout.get<Mat4f>("WorldToEye");
	diffuse_color_gbuffer = inout.get<Texture2D>("DiffuseColorGBuffer");
	specular_color_gbuffer = inout.get<Texture2D>("SpecularColorGBuffer");
	specular_level_gbuffer = inout.get<Texture2D>("SpecularLevelGBuffer");
	self_illumination_gbuffer = inout.get<Texture2D>("SelfIlluminationGBuffer");
	normal_z_gbuffer = inout.get<Texture2D>("NormalZGBuffer");
	shadow_maps = inout.get<Texture2DArray>("ShadowMaps");
	zbuffer = inout.get<Texture2D>("ZBuffer");

	final_color = inout.get<Texture2D>("FinalColor");

	icosahedron_light_program = compile_and_link(gc, shader_path, "icosahedron");
	rect_light_program = compile_and_link(gc, shader_path, "rect");

	light_instance_texture = Texture1D(gc, max_lights * vectors_per_light, tf_rgba32f);
	light_instance_transfer = PixelBuffer(max_lights * vectors_per_light, 1, tf_rgba32f);

	icosahedron.reset(new Icosahedron(gc, true));
}

LightsourceSimplePass::~LightsourceSimplePass()
{
}

ProgramObject LightsourceSimplePass::compile_and_link(GraphicContext &gc, const std::string &shader_path, const std::string &type)
{
	ProgramObject program;

	std::string vertex_filename = PathHelp::combine(shader_path, string_format("LightsourceSimple/vertex_%1.%2", type, gc.get_shader_language() == shader_glsl ? "glsl" : "hlsl"));
	std::string fragment_filename = PathHelp::combine(shader_path, string_format("LightsourceSimple/fragment_light.%1", type, gc.get_shader_language() == shader_glsl ? "glsl" : "hlsl"));

	if (gc.get_shader_language() == shader_glsl)
	{
		program = ShaderSetup::compile(gc, "", PathHelp::combine(shader_path, "LightsourceSimple/vertex_icosahedron.glsl"), PathHelp::combine(shader_path, "LightsourceSimple/fragment_light.glsl"), "");
	}
	else
	{
		program = ShaderSetup::compile(gc, "", PathHelp::combine(shader_path, "LightsourceSimple/vertex_icosahedron.hlsl"), PathHelp::combine(shader_path, "LightsourceSimple/fragment_light.hlsl"), "");
	}

	program.bind_frag_data_location(0, "FragColor");

	if (!program.link())
		throw Exception("Shader linking failed!");

	program.bind_attribute_location(0, "AttrPositionInObject");
	program.set_uniform_buffer_index("Uniforms", 0);
	program.set_uniform1i("InstanceTexture", 0);
	program.set_uniform1i("NormalZTexture", 1);
	program.set_uniform1i("DiffuseColorTexture", 2);
	program.set_uniform1i("SpecularColorTexture", 3);
	program.set_uniform1i("SpecularLevelTexture", 4);
	program.set_uniform1i("ShadowMapsTexture", 5);
	program.set_uniform1i("ShadowMapsTextureSampler", 5);
	program.set_uniform1i("SelfIlluminationTexture", 6);

	return program;
}

void LightsourceSimplePass::setup(GraphicContext &gc)
{
	Size viewport_size = viewport->get_size();
	if (fb.is_null() || !gc.is_frame_buffer_owner(fb) || final_color.updated() || zbuffer.updated() || diffuse_color_gbuffer.updated())
	{
		final_color.set(Texture2D(gc, viewport->get_width(), viewport->get_height(), tf_rgba16f));

		fb = FrameBuffer(gc);
		fb.attach_color(0, final_color.get());
		fb.attach_depth(zbuffer.get());

		BlendStateDescription blend_desc;
		blend_desc.enable_blending(true);
		blend_desc.set_blend_function(blend_one, blend_one, blend_one, blend_one);
		blend_state = BlendState(gc, blend_desc);

		DepthStencilStateDescription icosahedron_depth_stencil_desc;
		icosahedron_depth_stencil_desc.enable_depth_write(false);
		icosahedron_depth_stencil_desc.enable_depth_test(true);
		icosahedron_depth_stencil_desc.set_depth_compare_function(compare_lequal);
		icosahedron_depth_stencil_state = DepthStencilState(gc, icosahedron_depth_stencil_desc);

		RasterizerStateDescription icosahedron_rasterizer_desc;
		icosahedron_rasterizer_desc.set_culled(true);
		icosahedron_rasterizer_state = RasterizerState(gc, icosahedron_rasterizer_desc);

		DepthStencilStateDescription rect_depth_stencil_desc;
		rect_depth_stencil_desc.enable_depth_write(false);
		rect_depth_stencil_desc.enable_depth_test(false);
		rect_depth_stencil_state = DepthStencilState(gc, rect_depth_stencil_desc);

		RasterizerStateDescription rect_rasterizer_desc;
		rect_rasterizer_desc.set_culled(false);
		rect_rasterizer_state = RasterizerState(gc, rect_rasterizer_desc);

		uniforms = UniformVector<Uniforms>(gc, 1);

		icosahedron_prim_array = PrimitivesArray(gc);
		icosahedron_prim_array.set_attributes(0, icosahedron->vertices);

		Vec4f positions[6] =
		{
			Vec4f(-1.0f, -1.0f, 1.0f, 1.0f),
			Vec4f( 1.0f, -1.0f, 1.0f, 1.0f),
			Vec4f(-1.0f,  1.0f, 1.0f, 1.0f),
			Vec4f( 1.0f, -1.0f, 1.0f, 1.0f),
			Vec4f(-1.0f,  1.0f, 1.0f, 1.0f),
			Vec4f( 1.0f,  1.0f, 1.0f, 1.0f)
		};
		rect_positions = VertexArrayVector<Vec4f>(gc, positions, 6);

		rect_prim_array  = PrimitivesArray(gc);
		rect_prim_array.set_attributes(0, rect_positions);
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
	FrustumPlanes frustum(eye_to_cull_projection * world_to_eye.get());
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
		//timer.begin_time(gc, "light(simple)");

		gc.set_frame_buffer(fb);

		gc.set_viewport(viewport->get_size());

		gc.set_depth_range(0.0f, 0.9f);

		gc.set_uniform_buffer(0, uniforms);
		gc.set_texture(0, light_instance_texture);
		gc.set_texture(1, normal_z_gbuffer.get());
		gc.set_texture(2, diffuse_color_gbuffer.get());
		gc.set_texture(3, specular_color_gbuffer.get());
		gc.set_texture(4, specular_level_gbuffer.get());
		gc.set_texture(5, shadow_maps.get());
		gc.set_texture(6, self_illumination_gbuffer.get());

		gc.set_blend_state(blend_state);

		gc.clear();

		// To do: use icosahedron for smaller lights and when the camera is not inside the light influence sphere
		// To do: combine multiple lights into the same rect pass to reduce overdraw penalty

		gc.set_depth_stencil_state(rect_depth_stencil_state);
		gc.set_rasterizer_state(rect_rasterizer_state);
		gc.set_program_object(rect_light_program);

		gc.set_primitives_array(rect_prim_array);
		gc.draw_primitives_array_instanced(type_triangles, 0, 6, lights.size());
		gc.reset_primitives_array();

/*
		gc.set_depth_stencil_state(icosahedron_depth_stencil_state);
		gc.set_rasterizer_state(icosahedron_rasterizer_state);
		gc.set_program_object(icosahedron_light_program);

		gc.set_primitives_array(icosahedron_prim_array);
		gc.draw_primitives_elements_instanced(type_triangles, icosahedron->num_elements, icosahedron->elements, 0, lights.size());
		gc.reset_primitives_array();
*/

		gc.set_depth_stencil_state(icosahedron_depth_stencil_state);
		gc.set_rasterizer_state(icosahedron_rasterizer_state);
		gc.set_program_object(icosahedron_light_program);

		gc.set_primitives_array(icosahedron_prim_array);
		gc.draw_primitives_elements_instanced(type_triangles, icosahedron->num_elements, icosahedron->elements, 0, lights.size());
		gc.reset_primitives_array();

		//timer.end_time(gc);
		//timer.begin_time(gc, "light(simple)");

		gc.reset_texture(6);
		gc.reset_texture(5);
		gc.reset_texture(4);
		gc.reset_texture(3);
		gc.reset_texture(2);
		gc.reset_texture(1);
		gc.reset_texture(0);
		gc.reset_uniform_buffer(0);

		//timer.end_time(gc);
	}
}

}
