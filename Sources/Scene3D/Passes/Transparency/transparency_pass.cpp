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
#include "transparency_pass.h"
#include "API/Scene3D/scene.h"
#include "Scene3D/Model/model_lod.h"
#include "Scene3D/Culling/clipping_frustum.h"
#include "Scene3D/scene_impl.h"

namespace clan
{


TransparencyPass::TransparencyPass()
{
}

void TransparencyPass::run(GraphicContext &render_gc, Scene_Impl *scene)
{
	gc = render_gc;
	setup(gc);

	gc.set_frame_buffer(fb_transparency);

	Size viewport_size = viewport->get_size();
	gc.set_viewport(viewport_size);

	gc.set_depth_range(0.0f, 0.9f);
	gc.set_depth_stencil_state(depth_stencil_state);
	gc.set_blend_state(blend_state);

	Mat4f eye_to_projection = Mat4f::perspective(field_of_view.get(), viewport_size.width/(float)viewport_size.height, 0.1f, 1.e10f, handed_left, gc.get_clip_z_range());
	Mat4f eye_to_cull_projection = Mat4f::perspective(field_of_view.get(), viewport_size.width/(float)viewport_size.height, 0.1f, 150.0f, handed_left, clip_negative_positive_w);
	ClippingFrustum frustum(eye_to_cull_projection * world_to_eye.get());
	scene->visit(gc, world_to_eye.get(), eye_to_projection, frustum, this);

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

	gc.reset_frame_buffer();

	gc = GraphicContext();
}

void TransparencyPass::setup(GraphicContext &gc)
{
	Size viewport_size = viewport->get_size();
	if (fb_transparency.is_null() || !gc.is_frame_buffer_owner(fb_transparency) || final_color.updated() || zbuffer.updated())
	{
		fb_transparency = FrameBuffer(gc);
		fb_transparency.attach_color(0, final_color.get());
		fb_transparency.attach_depth(zbuffer.get());

		BlendStateDescription blend_desc;
		blend_desc.enable_blending(true);
		blend_desc.set_blend_function(blend_one, blend_one_minus_src_alpha, blend_zero, blend_zero);
		blend_state = BlendState(gc, blend_desc);

		DepthStencilStateDescription depth_stencil_desc;
		depth_stencil_desc.enable_depth_write(false);
		depth_stencil_desc.enable_depth_test(true);
		depth_stencil_desc.set_depth_compare_function(compare_lequal);
		depth_stencil_state = DepthStencilState(gc, depth_stencil_desc);
	}
}

void TransparencyPass::render(GraphicContext &gc, ModelLOD *model_lod, int num_instances)
{
	model_lod->transparency_commands.execute(gc, num_instances);
}

}

