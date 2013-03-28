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
#include "gbuffer_pass.h"
#include "API/Scene3D/scene.h"
#include "Scene3D/Model/model_lod.h"
#include "Scene3D/scene_impl.h"

namespace clan
{


GBufferPass::GBufferPass()
{
}

void GBufferPass::run(GraphicContext &render_gc, Scene_Impl *scene)
{
	gc = render_gc;
	setup_gbuffer(gc);

	gc.set_frame_buffer(fb_gbuffer);

	if (gc.get_shader_language() == shader_glsl)
	{
		// To do: support this in clanlib
		OpenGL::set_active(gc);
		GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(4, buffers);
	}

	Size viewport_size = viewport->get_size();
	gc.set_viewport(viewport_size);

	gc.clear_depth(1.0f);
	gc.clear(); // Needed by skybox pass

	gc.set_depth_range(0.0f, 0.9f);
	gc.set_depth_stencil_state(depth_stencil_state);
	gc.set_blend_state(blend_state);

	Mat4f eye_to_projection = Mat4f::perspective(field_of_view.get(), viewport_size.width/(float)viewport_size.height, 0.1f, 1.e10f, handed_left, gc.get_clip_z_range());
	Mat4f eye_to_cull_projection = Mat4f::perspective(field_of_view.get(), viewport_size.width/(float)viewport_size.height, 0.1f, 150.0f, handed_left, clip_negative_positive_w);
	ClippingFrustum frustum(eye_to_cull_projection * world_to_eye.get());

	render_list.clear();
	scene->visit(gc, world_to_eye.get(), eye_to_projection, frustum, this);

	for (size_t i = 0; i < render_list.size(); i++)
		render_list[i].model_lod->gbuffer_commands.execute(gc, render_list[i].num_instances);

	if (gc.get_shader_language() == shader_glsl)
	{
		// To do: support this in ClanLib
		OpenGL::set_active(gc);
		glDisablei(GL_BLEND, 0);
		glDisablei(GL_BLEND, 1);
		glDisablei(GL_BLEND, 2);
		glDisablei(GL_BLEND, 3);
	}

	gc.reset_rasterizer_state();
	gc.reset_depth_stencil_state();
	gc.set_depth_range(0.0f, 1.0f);

	gc.reset_program_object();
	gc.reset_primitives_elements();
	gc.reset_texture(0);
	gc.reset_texture(1);
	gc.reset_texture(2);
	gc.reset_texture(3);
	gc.reset_uniform_buffer(0);
	gc.reset_uniform_buffer(1);

	if (gc.get_shader_language() == shader_glsl)
	{
		OpenGL::set_active(gc);
		GLenum buffers2[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, buffers2);
	}
	gc.reset_frame_buffer();

	gc = GraphicContext();
}

void GBufferPass::setup_gbuffer(GraphicContext &gc)
{
	Size viewport_size = viewport->get_size();
	if (diffuse_color_gbuffer->is_null() || diffuse_color_gbuffer->get_size() != viewport_size || !gc.is_frame_buffer_owner(fb_gbuffer))
	{
		diffuse_color_gbuffer.set(Texture2D(gc, viewport_size.width, viewport_size.height, tf_rgba16));
		specular_color_gbuffer.set(Texture2D(gc, viewport_size.width, viewport_size.height, tf_rgba8));
		specular_level_gbuffer.set(Texture2D(gc, viewport_size.width, viewport_size.height, tf_rg16f));
		self_illumination_gbuffer.set(Texture2D(gc, viewport_size.width, viewport_size.height, tf_rgba16));
		normal_z_gbuffer.set(Texture2D(gc, viewport_size.width, viewport_size.height, tf_rgba16f));
		zbuffer.set(Texture2D(gc, viewport_size.width, viewport_size.height, tf_depth_component24));

		fb_gbuffer = FrameBuffer(gc);
		fb_gbuffer.attach_color(0, diffuse_color_gbuffer.get());
		fb_gbuffer.attach_color(1, specular_color_gbuffer.get());
		fb_gbuffer.attach_color(2, specular_level_gbuffer.get());
		fb_gbuffer.attach_color(3, self_illumination_gbuffer.get());
		fb_gbuffer.attach_color(4, normal_z_gbuffer.get());
		fb_gbuffer.attach_depth(zbuffer.get());

		BlendStateDescription blend_desc;
		blend_desc.enable_blending(false);
		blend_desc.set_blend_function(blend_one, blend_one_minus_src_alpha, blend_zero, blend_zero);
		blend_state = BlendState(gc, blend_desc);

		DepthStencilStateDescription depth_stencil_desc;
		depth_stencil_desc.enable_depth_write(true);
		depth_stencil_desc.enable_depth_test(true);
		depth_stencil_desc.set_depth_compare_function(compare_lequal);
		depth_stencil_state = DepthStencilState(gc, depth_stencil_desc);
	}
}

void GBufferPass::render(GraphicContext &gc, ModelLOD *model_lod, int num_instances)
{
	model_lod->early_z_commands.execute(gc, num_instances);
	render_list.push_back(RenderEntry(model_lod, num_instances));
}

}

