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
#include "particle_emitter_pass.h"
#include "API/Scene3D/scene.h"
#include "Scene3D/Framework/material_cache.h"
#include "API/Core/Math/frustum_planes.h"
#include "Scene3D/scene_impl.h"

namespace clan
{

ParticleEmitterPass::ParticleEmitterPass(MaterialCache &texture_cache, const std::string &shader_path, ResourceContainer &inout)
: shader_path(shader_path), texture_cache(texture_cache)
{
	viewport = inout.get<Rect>("Viewport");
	field_of_view = inout.get<float>("FieldOfView");
	world_to_eye = inout.get<Mat4f>("WorldToEye");
	zbuffer = inout.get<Texture2D>("ZBuffer");
	normal_z_gbuffer = inout.get<Texture2D>("NormalZGBuffer");

	final_color = inout.get<Texture2D>("FinalColor");
}

void ParticleEmitterPass::run(GraphicContext &gc, Scene_Impl *scene)
{
	setup(gc);

	Size viewport_size = viewport->get_size();
	Mat4f eye_to_projection = Mat4f::perspective(field_of_view.get(), viewport_size.width/(float)viewport_size.height, 0.1f, 1.e10f, handed_left, gc.get_clip_z_range());
	Mat4f eye_to_cull_projection = Mat4f::perspective(field_of_view.get(), viewport_size.width/(float)viewport_size.height, 0.1f, 150.0f, handed_left, clip_negative_positive_w);
	FrustumPlanes frustum(eye_to_cull_projection * world_to_eye.get());

	for (size_t i = 0; i < active_emitters.size(); i++)
		active_emitters[i]->visible = false;
	scene->visit_emitters(gc, world_to_eye.get(), eye_to_projection, frustum, this);

	const int vectors_per_particle = 2;

	size_t total_particle_count = 0;
	for (size_t i = 0; i < active_emitters.size(); i++)
	{
		float depth_fade_distance = 1.0f;
		ParticleUniforms uniforms;
		uniforms.eye_to_projection = eye_to_projection;
		uniforms.object_to_eye = world_to_eye.get();
		uniforms.rcp_depth_fade_distance = 1.0f / depth_fade_distance;
		uniforms.instance_vectors_offset = total_particle_count * vectors_per_particle;
		active_emitters[i]->gpu_uniforms.upload_data(gc, &uniforms, 1);

		total_particle_count += active_emitters[i]->cpu_particles.size();
	}

	if (total_particle_count == 0)
		return;

	if (instance_texture.is_null() || instance_texture.get_width() < (int)total_particle_count)
	{
		instance_texture = Texture2D(gc, total_particle_count * vectors_per_particle, 1, tf_rgba32f);
		instance_transfer = TransferTexture(gc, total_particle_count * vectors_per_particle, 1, data_to_gpu, tf_rgba32f, 0, usage_stream_draw);
	}

	instance_transfer.lock(gc, access_write_discard);
	Vec4f *vectors = instance_transfer.get_data<Vec4f>();
	size_t vector_offset = 0;
	for (size_t j = 0; j < active_emitters.size(); j++)
	{
		Vec3f eye_pos = scene->get_camera().get_position();
		std::vector<ParticleOrderIndex> sorted_particles;
		sorted_particles.reserve(active_emitters[j]->cpu_particles.size());
		for (size_t i = 0; i < active_emitters[j]->cpu_particles.size(); i++)
		{
			Vec3f delta = active_emitters[j]->cpu_particles[i].position - eye_pos;
			sorted_particles.push_back(ParticleOrderIndex(i, Vec3f::dot(delta, delta)));
		}
		std::sort(sorted_particles.begin(), sorted_particles.end());

		for (size_t k = 0; k < sorted_particles.size(); k++)
		{
			int i = sorted_particles[k].index;
			float size = mix(active_emitters[j]->cpu_particles[i].start_size, active_emitters[j]->cpu_particles[i].end_size, active_emitters[j]->cpu_particles[i].life);
			vectors[vector_offset + k * vectors_per_particle + 0] = Vec4f(active_emitters[j]->cpu_particles[i].position, size);
			vectors[vector_offset + k * vectors_per_particle + 1] = Vec4f(active_emitters[j]->cpu_particles[i].life, 0.0f, 0.0f, 0.0f);
		}

		vector_offset += active_emitters[j]->cpu_particles.size() * vectors_per_particle;
	}
	instance_transfer.unlock();
	instance_texture.set_image(gc, instance_transfer);

	gc.set_depth_range(0.0f, 0.9f);

	gc.set_frame_buffer(fb);
	gc.set_viewport(viewport_size);
	gc.set_depth_stencil_state(depth_stencil_state);
	gc.set_blend_state(blend_state);
	gc.set_rasterizer_state(rasterizer_state);
	gc.set_primitives_array(prim_array);

	gc.set_program_object(program);
	gc.set_texture(0, normal_z_gbuffer.get());
	gc.set_texture(1, instance_texture);

	for (size_t i = 0; i < active_emitters.size(); i++)
	{
		gc.set_uniform_buffer(0, active_emitters[i]->gpu_uniforms);
		gc.set_texture(2, active_emitters[i]->particle_animation.get_texture());
		gc.set_texture(3, active_emitters[i]->life_color_gradient.get_texture());
		gc.draw_primitives_array_instanced(type_triangles, 0, 6, active_emitters[i]->cpu_particles.size());
	}

	gc.reset_primitives_array();
	gc.reset_rasterizer_state();
	gc.reset_depth_stencil_state();
	gc.reset_program_object();
	gc.reset_primitives_elements();
	gc.reset_texture(0);
	gc.reset_texture(1);
	gc.reset_texture(2);
	gc.reset_texture(3);
	gc.reset_uniform_buffer(0);
	gc.reset_frame_buffer();

	gc.set_depth_range(0.0f, 1.0f);
}

void ParticleEmitterPass::setup(GraphicContext &gc)
{
	if (program.is_null())
	{
		std::string vertex_filename = PathHelp::combine(shader_path, "ParticleEmitter/vertex.hlsl");
		std::string fragment_filename = PathHelp::combine(shader_path, "ParticleEmitter/fragment.hlsl");
		program = ProgramObject::load(gc, vertex_filename, fragment_filename);
		program.bind_attribute_location(0, "AttrPosition");
		program.bind_frag_data_location(0, "FragColor");
		if (!program.link())
			throw Exception(string_format("Particle emitter program failed to link: %1", program.get_info_log()));
		program.set_uniform_buffer_index("Uniforms", 0);
		program.set_uniform1i("NormalZTexture", 0);
		program.set_uniform1i("InstanceTexture", 1);
		program.set_uniform1i("ParticleTexture", 2);
		program.set_uniform1i("ParticleSampler", 2);
		program.set_uniform1i("ColorGradientTexture", 3);
		program.set_uniform1i("ColorGradientSampler", 3);

		billboard_positions = VertexArrayVector<Vec3f>(gc, cpu_billboard_positions, 6);
	}

	Size viewport_size = viewport->get_size();
	if (fb.is_null() || !gc.is_frame_buffer_owner(fb) || final_color.updated() || zbuffer.updated())
	{
		fb = FrameBuffer(gc);
		fb.attach_color(0, final_color.get());
		fb.attach_depth(zbuffer.get());

		BlendStateDescription blend_desc;
		blend_desc.enable_blending(true);
		blend_desc.set_blend_function(blend_src_alpha, blend_one_minus_src_alpha, blend_zero, blend_zero);
		blend_state = BlendState(gc, blend_desc);

		DepthStencilStateDescription depth_stencil_desc;
		depth_stencil_desc.enable_depth_write(false);
		depth_stencil_desc.enable_depth_test(true);
		depth_stencil_desc.set_depth_compare_function(compare_lequal);
		depth_stencil_state = DepthStencilState(gc, depth_stencil_desc);

		RasterizerStateDescription rasterizer_desc;
		rasterizer_desc.set_culled(false);
		rasterizer_state = RasterizerState(gc, rasterizer_desc);

		prim_array = PrimitivesArray(gc);
		prim_array.set_attributes(0, billboard_positions);
	}
}

void ParticleEmitterPass::emitter(GraphicContext &gc, const Mat4f &world_to_eye, const Mat4f &eye_to_projection, SceneParticleEmitter_Impl *emitter)
{
	if (!emitter->pass_data)
	{
		emitter->pass_data.reset(new ParticleEmitterPassData());
		emitter->pass_data->emitter = emitter;
		emitter->pass_data->life_color_gradient = texture_cache.get_texture(gc, emitter->gradient_texture, false);
		emitter->pass_data->particle_animation = texture_cache.get_texture(gc, emitter->particle_texture, false);
		emitter->pass_data->gpu_uniforms = UniformVector<ParticleUniforms>(gc, 1);
	}

	emitter->pass_data->visible = true;

	if (!emitter->pass_data->in_active_list)
	{
		active_emitters.push_back(emitter->pass_data);
		emitter->pass_data->in_active_list = true;
	}
}

void ParticleEmitterPass::update(GraphicContext &gc, float time_elapsed)
{
	size_t i = 0;
	while (i != active_emitters.size())
	{
		bool active = active_emitters[i]->update(time_elapsed);
		if (!active)
		{
			active_emitters[i]->in_active_list = false;
			active_emitters.erase(active_emitters.begin() + i);
		}
		else
		{
			i++;
		}
	}
}

Vec3f ParticleEmitterPass::cpu_billboard_positions[6] = 
{
	Vec3f(-1.0f, -1.0f, 0.0f),
	Vec3f( 1.0f, -1.0f, 0.0f),
	Vec3f(-1.0f,  1.0f, 0.0f),
	Vec3f(-1.0f,  1.0f, 0.0f),
	Vec3f( 1.0f, -1.0f, 0.0f),
	Vec3f( 1.0f,  1.0f, 0.0f)
};

}

