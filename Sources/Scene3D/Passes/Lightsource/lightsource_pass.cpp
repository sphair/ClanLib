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
#include "lightsource_pass.h"
#include "Scene3D/Framework/shader_setup.h"
#include "Scene3D/Passes/VSMShadowMap/vsm_shadow_map_pass.h"
#include "API/Scene3D/scene.h"
#include "Scene3D/Culling/clipping_frustum.h"
#include "API/Scene3D/Performance/scope_timer.h"
#include "API/Scene3D/Performance/gpu_timer.h"
#include "Scene3D/scene_impl.h"

namespace clan
{


LightsourcePass::LightsourcePass(GraphicContext &gc, const std::string &shader_path, SceneInOutDataContainer &inout)
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

	final_color = inout.get<Texture2D>("FinalColor");

	if (gc.get_shader_language() == shader_glsl)
	{
		cull_tiles_program = compile_and_link(gc, PathHelp::combine(shader_path, "Lightsource/cull_tiles.glsl"));
		render_tiles_program = compile_and_link(gc, PathHelp::combine(shader_path, "Lightsource/render_tiles.glsl"));
	}
	else
	{
		cull_tiles_program = compile_and_link(gc, PathHelp::combine(shader_path, "Lightsource/cull_tiles.hlsl"));
		render_tiles_program = compile_and_link(gc, PathHelp::combine(shader_path, "Lightsource/render_tiles.hlsl"));
	}

	compute_uniforms = UniformVector<Uniforms>(gc, 1);
	compute_lights = StorageVector<GPULight>(gc, max_lights);
	transfer_lights = TransferVector<GPULight>(gc, max_lights);

	zminmax.viewport = viewport;
	zminmax.normal_z = normal_z_gbuffer;
}

LightsourcePass::~LightsourcePass()
{
}

void LightsourcePass::run(GraphicContext &gc, Scene_Impl *scene)
{
	find_lights(gc, scene);
	upload(gc);
	render(gc, scene->get_gpu_timer());
}

void LightsourcePass::find_lights(GraphicContext &gc, Scene_Impl *scene)
{
	lights.clear();

	Size viewport_size = viewport->get_size();

	Mat4f eye_to_projection = Mat4f::perspective(field_of_view.get(), viewport_size.width/(float)viewport_size.height, 0.1f, 1.e10f, handed_left, gc.get_clip_z_range());
	Mat4f eye_to_cull_projection = Mat4f::perspective(field_of_view.get(), viewport_size.width/(float)viewport_size.height, 0.1f, 150.0f, handed_left, clip_negative_positive_w);
	ClippingFrustum frustum(eye_to_cull_projection * world_to_eye.get());
	scene->visit_lights(gc, world_to_eye.get(), eye_to_projection, frustum, this);
}

void LightsourcePass::light(GraphicContext &gc, const Mat4f &world_to_eye, const Mat4f &eye_to_projection, SceneLight_Impl *light)
{
	if ((light->type == SceneLight::type_omni || light->type == SceneLight::type_spot) && light->light_caster && lights.size() < max_lights - 1)
	{
#ifdef TEST_LIGHT_DISTANCE
		float dist = (world_to_eye * Vec4f(light->position, 1.0f)).length3() - light->attenuation_end;
		if (dist < 30.0f)
			lights.push_back(light);
#else
		lights.push_back(light);
#endif
	}
}

class LightsourcePass_LightCompare
{
public:
	bool operator()(SceneLight_Impl *a, SceneLight_Impl *b)
	{
		bool a_has_shadow = a->vsm_data->shadow_map.get_index() >= 0;
		bool b_has_shadow = b->vsm_data->shadow_map.get_index() >= 0;

		if (a_has_shadow != b_has_shadow)
			return a_has_shadow < b_has_shadow;
		else
			return a->type < b->type;
	}
};

void LightsourcePass::upload(GraphicContext &gc)
{
	ScopeTimeFunction();

	int num_lights = lights.size();
	if (num_lights == 0)
		return;

	std::sort(lights.begin(), lights.end(), LightsourcePass_LightCompare());

	float aspect = viewport->get_width()/(float)viewport->get_height();
	float field_of_view_y_degrees = field_of_view.get();
	float field_of_view_y_rad = (float)(field_of_view_y_degrees * PI / 180.0);
	float f = 1.0f / tan(field_of_view_y_rad * 0.5f);
	float rcp_f = 1.0f / f;
	float rcp_f_div_aspect = 1.0f / (f / aspect);
	Vec2f two_rcp_viewport_size(2.0f / viewport->get_width(), 2.0f / viewport->get_height());

	Uniforms uniforms;
	uniforms.rcp_f = rcp_f;
	uniforms.rcp_f_div_aspect = rcp_f_div_aspect;
	uniforms.two_rcp_viewport_size = two_rcp_viewport_size;
	uniforms.num_lights = num_lights;
	uniforms.num_tiles_x = num_tiles_x;
	uniforms.num_tiles_y = num_tiles_y;
	compute_uniforms.upload_data(gc, &uniforms, 1);

	Mat4f normal_world_to_eye = Mat4f(Mat3f(world_to_eye.get())); // This assumes uniform scale
	Mat4f eye_to_world = Mat4f::inverse(world_to_eye.get());

	transfer_lights.lock(gc, access_write_only);
	GPULight *data = transfer_lights.get_data();
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

		data[i].position = Vec4f(position_in_eye, (float)shadow_map_index);
		data[i].color = Vec4f(lights[i]->color, lights[i]->ambient_illumination);
#ifdef USE_QUADRATIC_ATTENUATION
		data[i].range = Vec4f(sqr_radius, sqr_attenuation_start, 1.0f / sqr_attenuation_delta, sqr_falloff_begin);
#else
		data[i].range = Vec4f(sqr_radius, attenuation_start, 1.0f / attenuation_delta, sqr_falloff_begin);
#endif
		data[i].spot_x = Vec4f(eye_to_shadow_projection[0], eye_to_shadow_projection[4], eye_to_shadow_projection[8], light_type);
		data[i].spot_y = Vec4f(eye_to_shadow_projection[1], eye_to_shadow_projection[5], eye_to_shadow_projection[9], 0.0f);
		data[i].spot_z = Vec4f(eye_to_shadow_projection[2], eye_to_shadow_projection[6], eye_to_shadow_projection[10], 0.0f);
	}
	data[num_lights].position = Vec4f(0.0f);
	data[num_lights].color = Vec4f(0.0f);
	data[num_lights].range = Vec4f(0.0f);
	transfer_lights.unlock();
	compute_lights.copy_from(gc, transfer_lights, 0, 0, num_lights + 1);
}

void LightsourcePass::render(GraphicContext &gc, GPUTimer &timer)
{
	ScopeTimeFunction();
	zminmax.minmax(gc);
	update_buffers(gc);
	if (!lights.empty())
	{
		//timer.begin_time(gc, "light(cull)");

		gc.set_uniform_buffer(0, compute_uniforms);
		gc.set_storage_buffer(0, compute_lights);
		gc.set_storage_buffer(1, compute_visible_lights);
		gc.set_texture(0, zminmax.result.get());
		gc.set_texture(1, normal_z_gbuffer.get());
		gc.set_texture(2, diffuse_color_gbuffer.get());
		gc.set_texture(3, specular_color_gbuffer.get());
		gc.set_texture(4, specular_level_gbuffer.get());
		gc.set_texture(5, shadow_maps.get());
		gc.set_texture(6, self_illumination_gbuffer.get());
		gc.set_image_texture(0, final_color.get());

		gc.set_program_object(cull_tiles_program);
		gc.dispatch((num_tiles_x + tile_size - 1) / tile_size, (num_tiles_y + tile_size - 1) / tile_size);

		//timer.end_time(gc);
		//timer.begin_time(gc, "light(render)");

		gc.set_program_object(render_tiles_program);
		gc.dispatch(num_tiles_x, num_tiles_y);

		gc.reset_image_texture(0);
		gc.reset_texture(6);
		gc.reset_texture(5);
		gc.reset_texture(4);
		gc.reset_texture(3);
		gc.reset_texture(2);
		gc.reset_texture(1);
		gc.reset_texture(0);
		gc.reset_uniform_buffer(2);
		gc.reset_uniform_buffer(1);
		gc.reset_uniform_buffer(0);

		//timer.end_time(gc);
	}
}

void LightsourcePass::update_buffers(GraphicContext &gc)
{
	if (diffuse_color_gbuffer.updated())
	{
		tile_size = 16;
		num_tiles_x = (viewport->get_width() + tile_size - 1) / tile_size;
		num_tiles_y = (viewport->get_height() + tile_size - 1) / tile_size;
		final_color.set(Texture2D(gc, viewport->get_width(), viewport->get_height(), tf_rgba32f));

		compute_visible_lights = StorageVector<unsigned int>(gc, num_tiles_x * num_tiles_y * light_slots_per_tile);
	}
}

ProgramObject LightsourcePass::compile_and_link(GraphicContext &gc, const std::string &compute_filename, const std::string &defines)
{
	std::string source = File::read_text(compute_filename);

	std::string prefix;
	std::vector<std::string> define_list = StringHelp::split_text(defines, " ");
	for (size_t i = 0; i < define_list.size(); i++)
		prefix += string_format("#define %1\r\n", define_list[i]);
	prefix += "#line 0\r\n";

	ShaderObject compute_shader(gc, shadertype_compute, prefix + source);
	if (!compute_shader.compile())
		throw Exception(string_format("Unable to compile %1 compute shader: %2", compute_filename, compute_shader.get_info_log()));

	ProgramObject program(gc);
	program.attach(compute_shader);
	if (!program.link())
		throw Exception(string_format("Failed to link %1: %2", compute_filename, program.get_info_log()));

	// Uniforms
	program.set_uniform_buffer_index("Uniforms", 0);

	// Storage buffers
	program.set_storage_buffer_index("Lights", 0);
	program.set_storage_buffer_index("VisibleLightIndices", 1);

	// Textures
	program.set_uniform1i("zminmax", 0);
	program.set_uniform1i("normal_z", 1);
	program.set_uniform1i("diffuse", 2);
	program.set_uniform1i("specular", 3);
	program.set_uniform1i("specular_level", 4);
	program.set_uniform1i("shadow_maps", 5);
	program.set_uniform1i("self_illumination", 6);

	// Samplers
	program.set_uniform1i("shadow_maps_sampler", 5);

	// Images
	program.set_uniform1i("out_final", 0);

	return program;
}

}

