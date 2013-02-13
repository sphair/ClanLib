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

#include "Scene/precomp.h"
#include "final_pass.h"
#include "Performance/scope_timer.h"
#include "Scene/Framework/shader_setup.h"

namespace clan
{


FinalPass::FinalPass(GraphicContext &gc)
{
	if (gc.get_shader_language() == shader_glsl)
	{
		present_shader = ShaderSetup::compile(gc, "", "Resources/Engine/Final/vertex_present.glsl", "Resources/Engine/Final/fragment_present.glsl", "");
		present_shader.bind_frag_data_location(0, "FragColor");
	}
	else
	{
		present_shader = ShaderSetup::compile(gc, "", "Resources/Engine/Final/vertex_present.hlsl", "Resources/Engine/Final/fragment_present.hlsl", "");
	}
	if (!present_shader.link())
		throw Exception("Shader linking failed!");
	present_shader.bind_attribute_location(0, "PositionInProjection");
	present_shader.set_uniform1i("FinalColors", 0);
	present_shader.set_uniform1i("FinalColorsSampler", 0);
	present_shader.set_uniform1i("LogAverageLight", 1);
	present_shader.set_uniform1i("BloomColors", 2);
	present_shader.set_uniform1i("BloomColorsSampler", 2);
	present_shader.set_uniform1i("AmbientOcclusion", 3);

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
	rect_primarray  = PrimitivesArray(gc);
	rect_primarray.set_attributes(0, rect_positions);

	RasterizerStateDescription rasterizer_desc;
	rasterizer_desc.set_culled(false);
	rasterizer_state = RasterizerState(gc, rasterizer_desc);
}

void FinalPass::run(GraphicContext &gc)
{
	ScopeTimeFunction();
	//Texture2D &log_average_light_texture = log_average_light.find_log_average_light(gc, lightsource_pass.final_color);

	bloom_blur_texture->set_min_filter(filter_linear);
	bloom_blur_texture->set_mag_filter(filter_linear);
	final_color->set_min_filter(filter_nearest);
	final_color->set_mag_filter(filter_nearest);

	gc.set_viewport(viewport.get());
	gc.set_texture(0, final_color.get());
	gc.set_texture(2, bloom_blur_texture.get());
	gc.set_program_object(present_shader);
	gc.set_rasterizer_state(rasterizer_state);
	gc.draw_primitives(type_triangles, 6, rect_primarray);
	gc.reset_program_object();
	gc.reset_texture(0);
	gc.reset_texture(2);
}

}

