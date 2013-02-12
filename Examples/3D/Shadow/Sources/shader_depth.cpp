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
**    Mark Page
*/

#include "precomp.h"

#include "shader_depth.h"

char ShaderDepth::vertex[] =
	"\n"
	"#version 120\n"
	"\n"
	"attribute vec3 InPosition;"
	"varying vec4 MaterialAmbient;\n"
	"uniform mat4 cl_ModelViewProjectionMatrix;"
	"\n"
	"void main()\n"
	"{\n"
	"	vec4 in_position = vec4(InPosition.xyz, 1.0);\n"
	"	gl_Position = cl_ModelViewProjectionMatrix * in_position;\n"
	"}\n"
	;

char ShaderDepth::fragment[] =
	"\n"
	"#version 120\n"
	"\n"
	"uniform vec4 MaterialAmbient;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	gl_FragColor = MaterialAmbient;\n"
	"}\n"
	;

ShaderDepth::ShaderDepth(GraphicContext &gc)
{
	ShaderObject vertex_shader(gc, shadertype_vertex, vertex);
	if(!vertex_shader.compile())
	{
		throw Exception(string_format("Unable to compile vertex shader object: %1", vertex_shader.get_info_log()));
	}

	ShaderObject fragment_shader(gc, shadertype_fragment, fragment);
	if(!fragment_shader.compile())
	{
		throw Exception(string_format("Unable to compile fragment shader object: %1", fragment_shader.get_info_log()));
	}

	program_object = ProgramObject(gc);
	program_object.attach(vertex_shader);
	program_object.attach(fragment_shader);
	program_object.bind_attribute_location(0, "InPosition");
	if (!program_object.link())
	{
		throw Exception(string_format("Unable to link program object: %1", program_object.get_info_log()));
	}

	material_updated = false;

	material_ambient =  Vec4f(0.9f, 0.2f, 0.2f, 1.0f);

}

void ShaderDepth::Use(GraphicContext &gc)
{
	if (!material_updated)
	{
		material_updated = true;
		program_object.set_uniform4f("MaterialAmbient", material_ambient);
	}

	gc.set_program_object(program_object);
}

void ShaderDepth::SetMaterial(const Vec4f &new_material_ambient)
{
	if (new_material_ambient != material_ambient)
	{
		material_updated = false;
		material_ambient = new_material_ambient;
	}
}


