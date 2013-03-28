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
#include "diffuse_gi_pass_cs.h"
#include "API/Scene3D/Performance/scope_timer.h"
#include "Scene3D/scene_impl.h"

namespace clan
{


DiffuseGIPassCS::DiffuseGIPassCS(GraphicContext &gc, const std::string &shader_path)
{
	if (gc.get_shader_language() == shader_glsl)
	{
		init_lpv_program = compile_and_link(gc, PathHelp::combine(shader_path, "DiffuseGI/init_lpv.glsl"));
		init_gv_program = compile_and_link(gc, PathHelp::combine(shader_path, "DiffuseGI/init_gv.glsl"));
		propagate_lpv_program = compile_and_link(gc, PathHelp::combine(shader_path, "DiffuseGI/propagate_lpv.glsl"));
		accumulate_lpv_program = compile_and_link(gc, PathHelp::combine(shader_path, "DiffuseGI/accumulate_lpv.glsl"));
		render_result_program = compile_and_link(gc, PathHelp::combine(shader_path, "DiffuseGI/render_result.glsl"));
	}
	else
	{
		init_lpv_program = compile_and_link(gc, PathHelp::combine(shader_path, "DiffuseGI/init_lpv.hlsl"));
		init_gv_program = compile_and_link(gc, PathHelp::combine(shader_path, "DiffuseGI/init_gv.hlsl"));
		propagate_lpv_program = compile_and_link(gc, PathHelp::combine(shader_path, "DiffuseGI/propagate_lpv.hlsl"));
		accumulate_lpv_program = compile_and_link(gc, PathHelp::combine(shader_path, "DiffuseGI/accumulate_lpv.hlsl"));
		render_result_program = compile_and_link(gc, PathHelp::combine(shader_path, "DiffuseGI/render_result.hlsl"));
	}
}

DiffuseGIPassCS::~DiffuseGIPassCS()
{
}

void DiffuseGIPassCS::run(GraphicContext &gc, Scene_Impl *scene)
{
	ScopeTimeFunction();
/*
	update_buffers(gc);

	Size viewport = diffuse_color_gbuffer->get_size();
	int tile_size = 16;
	int num_tiles_x = (viewport.width + tile_size - 1) / tile_size;
	int num_tiles_y = (viewport.height + tile_size - 1) / tile_size;

	//ComputeShaderDispatch init_lpv(init_lpv_kernel);
	//init_lpv.bind("shadow_maps", shadow_maps_srv);
	//init_lpv.dispatch(gc, x, y);

	ComputeShaderDispatch render_result(render_result_kernel);
	render_result.bind("normal_z", normal_z_srv);
	render_result.bind("diffuse", diffuse_srv);
	render_result.bind("in_final", final_srv);
	render_result.bind("out_final", final_uav);
	render_result.dispatch(gc, num_tiles_x, num_tiles_y);
*/
}

void DiffuseGIPassCS::update_buffers(GraphicContext &gc)
{
	ScopeTimeFunction();
	if (diffuse_color_gbuffer.updated())
	{
	}
}

ProgramObject DiffuseGIPassCS::compile_and_link(GraphicContext &gc, const std::string &compute_filename)
{
	std::string source = File::read_text(compute_filename);

	ShaderObject compute_shader(gc, shadertype_compute, source);
	if (!compute_shader.compile())
		throw Exception(string_format("Unable to compile %1 compute shader: %2", compute_filename, compute_shader.get_info_log()));

	ProgramObject program(gc);
	program.attach(compute_shader);
	if (!program.link())
		throw Exception(string_format("Failed to link %1: %2", compute_filename, program.get_info_log()));

	// Uniforms
	//program.set_uniform_buffer_index("Uniforms", 0);

	// Storage buffers
	//program.set_storage_buffer_index("Lights", 0);

	// Textures
	//program.set_uniform1i("zminmax", 0);

	// Samplers
	//program.set_uniform1i("shadow_maps_sampler", 5);

	// Images
	//program.set_uniform1i("out_final", 0);

	return program;
}

}

