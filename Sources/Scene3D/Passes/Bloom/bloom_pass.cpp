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
#include "bloom_pass.h"
#include "API/Scene3D/Performance/scope_timer.h"
#include "Scene3D/Framework/shader_setup.h"

namespace clan
{

BloomPass::BloomPass(GraphicContext &gc, const std::string &shader_path, ResourceContainer &inout)
{
	viewport = inout.get<Rect>("Viewport");
	final_color = inout.get<Texture2D>("FinalColor");
	bloom_contribution = inout.get<Texture2D>("BloomContribution");

	if (gc.get_shader_language() == shader_glsl)
	{
		bloom_shader = ShaderSetup::compile(gc, "", PathHelp::combine(shader_path, "Final/vertex_present.glsl"), PathHelp::combine(shader_path, "Bloom/fragment_bloom_extract.glsl"), "");
		bloom_shader.bind_frag_data_location(0, "FragColor");
	}
	else
	{
		bloom_shader = ShaderSetup::compile(gc, "", PathHelp::combine(shader_path, "Final/vertex_present.hlsl"), PathHelp::combine(shader_path, "Bloom/fragment_bloom_extract.hlsl"), "");
	}

	ShaderSetup::link(bloom_shader, "bloom extract program");

	bloom_shader.bind_attribute_location(0, "PositionInProjection");
	bloom_shader.set_uniform1i("FinalColors", 0);
	bloom_shader.set_uniform1i("FinalColorsSampler", 0);
	bloom_shader.set_uniform1i("LogAverageLight", 1);

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
	rect_primarray = PrimitivesArray(gc);
	rect_primarray.set_attributes(0, rect_positions);

	bloom_blur.input = bloom_contribution;

	BlendStateDescription blend_desc;
	blend_desc.enable_blending(false);
	blend_state = BlendState(gc, blend_desc);
}

void BloomPass::run(GraphicContext &gc)
{
	final_color->set_min_filter(filter_linear);
	final_color->set_mag_filter(filter_linear);

	ScopeTimeFunction();
	setup_bloom_extract(gc);
	gc.set_frame_buffer(fb_bloom_extract);
	gc.set_viewport(bloom_contribution->get_size());
	gc.set_texture(0, final_color.get());
	gc.set_blend_state(blend_state);
	gc.set_program_object(bloom_shader);
	gc.draw_primitives(type_triangles, 6, rect_primarray);
	gc.reset_program_object();
	gc.reset_texture(0);
	gc.reset_frame_buffer();

	bloom_blur.blur(gc, tf_rgba8, 4.0f, 15);
}

void BloomPass::setup_bloom_extract(GraphicContext &gc)
{
	Size viewport_size = viewport->get_size();
	Size bloom_size = viewport_size / 2;
	if (bloom_contribution->is_null() || bloom_contribution->get_size() != bloom_size || !gc.is_frame_buffer_owner(fb_bloom_extract))
	{
		bloom_contribution.set(Texture2D(gc, bloom_size.width, bloom_size.height, tf_rgba8));

		fb_bloom_extract = FrameBuffer(gc);
		fb_bloom_extract.attach_color(0, bloom_contribution.get());

		bloom_blur.output.set(fb_bloom_extract);
	}
}

}

