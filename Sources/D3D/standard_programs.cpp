/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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

#include "D3D/precomp.h"
#include "API/Core/Text/string_format.h"
#include "standard_programs.h"

namespace clan
{
	// *** These shaders are built using Resources\StandardShaders\compile_shaders.bat" ***
	#include "Shaders\color_only_vertex.h"
	#include "Shaders\color_only_fragment.h"
	#include "Shaders\single_texture_vertex.h"
	#include "Shaders\single_texture_fragment.h"
	#include "Shaders\sprite_vertex.h"
	#include "Shaders\sprite_fragment.h"
	#include "Shaders\path_vertex.h"
	#include "Shaders\path_fragment.h"

	class StandardPrograms_Impl
	{
	public:
		StandardPrograms_Impl() {}
		~StandardPrograms_Impl() {}

		ProgramObject color_only_program;
		ProgramObject single_texture_program;
		ProgramObject sprite_program;
		ProgramObject path_program;

	};

	StandardPrograms::StandardPrograms()
	{
	}
	StandardPrograms::StandardPrograms(GraphicContext &gc) : impl(std::make_shared<StandardPrograms_Impl>())
	{
		ProgramObject color_only_program;
		ProgramObject single_texture_program;
		ProgramObject sprite_program;
		ProgramObject path_program;


		color_only_program = compile(gc, color_only_vertex, sizeof(color_only_vertex), color_only_fragment, sizeof(color_only_fragment));
		color_only_program.bind_attribute_location(0, "VertexPosition");
		color_only_program.bind_attribute_location(1, "VertexColor");
		link(color_only_program, "Unable to link color-only standard program");
		color_only_program.set_uniform_buffer_index("Uniforms", 0);

		single_texture_program = compile(gc, single_texture_vertex, sizeof(single_texture_vertex), single_texture_fragment, sizeof(single_texture_fragment));
		single_texture_program.bind_attribute_location(0, "VertexPosition");
		single_texture_program.bind_attribute_location(1, "VertexColor");
		single_texture_program.bind_attribute_location(2, "VertexTexCoord");
		link(single_texture_program, "Unable to link single-texture standard program");
		single_texture_program.set_uniform_buffer_index("Uniforms", 0);
		single_texture_program.set_uniform1i("Texture0", 0);
		single_texture_program.set_uniform1i("Sampler0", 0);

		sprite_program = compile(gc, sprite_vertex, sizeof(sprite_vertex), sprite_fragment, sizeof(sprite_fragment));
		sprite_program.bind_attribute_location(0, "VertexPosition");
		sprite_program.bind_attribute_location(1, "VertexColor");
		sprite_program.bind_attribute_location(2, "VertexTexCoord");
		sprite_program.bind_attribute_location(3, "VertexTexIndex");
		link(sprite_program, "Unable to link sprite standard program");
		sprite_program.set_uniform_buffer_index("Uniforms", 0);
		sprite_program.set_uniform1i("Texture0", 0);
		sprite_program.set_uniform1i("Texture1", 1);
		sprite_program.set_uniform1i("Texture2", 2);
		sprite_program.set_uniform1i("Texture3", 3);
		sprite_program.set_uniform1i("Sampler0", 0);
		sprite_program.set_uniform1i("Sampler1", 1);
		sprite_program.set_uniform1i("Sampler2", 2);
		sprite_program.set_uniform1i("Sampler3", 3);

		path_program = compile(gc, path_vertex, sizeof(path_vertex), path_fragment, sizeof(path_fragment));
		path_program.bind_attribute_location(0, "Vertex");
		link(path_program, "Unable to link path standard program");
		path_program.set_uniform_buffer_index("Uniforms", 0);
		path_program.set_uniform1i("mask_texture", 0);
		path_program.set_uniform1i("mask_sampler", 0);
		path_program.set_uniform1i("instance_data", 1);
		path_program.set_uniform1i("image_texture", 2);
		path_program.set_uniform1i("image_sampler", 2);

		impl->color_only_program = color_only_program;
		impl->single_texture_program = single_texture_program;
		impl->sprite_program = sprite_program;
		impl->path_program = path_program;
	}

	ProgramObject StandardPrograms::get_program_object(StandardProgram standard_program) const
	{
		switch (standard_program)
		{
		case program_color_only: return impl->color_only_program;
		case program_single_texture: return impl->single_texture_program;
		case program_sprite: return impl->sprite_program;
		case program_path: return impl->path_program;
		}
		throw Exception("Unsupported standard program");
	}

	ProgramObject StandardPrograms::compile(GraphicContext &gc, const void *vertex_code, int vertex_code_size, const void *fragment_code, int fragment_code_size)
	{
		ShaderObject vertex_shader(gc, shadertype_vertex, vertex_code, vertex_code_size);
		if (!vertex_shader.compile())
			throw Exception(string_format("Unable to compile standard vertex shader: %1", vertex_shader.get_info_log()));

		ShaderObject fragment_shader(gc, shadertype_fragment, fragment_code, fragment_code_size);
		if (!fragment_shader.compile())
			throw Exception(string_format("Unable to compile standard fragment shader: %1", fragment_shader.get_info_log()));

		ProgramObject program(gc);
		program.attach(vertex_shader);
		program.attach(fragment_shader);
		return program;
	}

	void StandardPrograms::link(ProgramObject &program, const std::string &error_message)
	{
		if (!program.link())
			throw Exception(string_format("%1: %2", error_message, program.get_info_log()));
	}
}
