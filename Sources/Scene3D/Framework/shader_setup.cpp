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
#include "shader_setup.h"

namespace clan
{

ProgramObject ShaderSetup::compile(GraphicContext &gc, std::string shader_path, const std::string &vertex, const std::string &fragment, const std::string &defines)
{
	std::string prefix;
	if (gc.get_shader_language() == shader_glsl)
		prefix += "#version 330\r\n";
	std::vector<std::string> define_list = StringHelp::split_text(defines, " ");
	for (size_t i = 0; i < define_list.size(); i++)
		prefix += string_format("#define %1\r\n", define_list[i]);
	prefix += "#line 0\r\n";

	ProgramObject program(gc);

	if (!vertex.empty())
	{
		ShaderObject vertex_shader(gc, shadertype_vertex, prefix + File::read_text(PathHelp::combine(shader_path, vertex)));
		if (!vertex_shader.compile())
			throw Exception(vertex_shader.get_info_log());
		program.attach(vertex_shader);
	}

	if (!fragment.empty())
	{
		ShaderObject fragment_shader(gc, shadertype_fragment, prefix + File::read_text(PathHelp::combine(shader_path, fragment)));
		if (!fragment_shader.compile())
			throw Exception(fragment_shader.get_info_log());
		program.attach(fragment_shader);
	}

	return program;
}

void ShaderSetup::link(ProgramObject &program, const std::string &program_name)
{
	if (!program.link())
		throw Exception(string_format("Failed to link %1: %2", program.get_info_log()));
}

}

