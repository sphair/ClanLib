/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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

#include "shader_color_geometry.h"

char ShaderColorGeometry::vertex[] =
	"\n"
	"#version 130\n"
	"\n"
	"in vec3 InPosition;\n"
	"in vec4 InColor;\n"
	"out vec4 PointColor;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	vec4 in_position = vec4(InPosition.xyz, 1.0);\n"
	"	PointColor = InColor;"
	"	gl_Position = in_position;\n"
	"}\n"
	;

char ShaderColorGeometry::geometry[] =
	"\n"
	"#version 150\n"
	"layout(points) in;\n"
	"layout(triangle_strip, max_vertices=3) out;\n"
	"\n"
	"layout (std140) uniform ProgramUniforms\n"
	"{\n"
	"	mat4 cl_ModelViewProjectionMatrix;\n"
	"};\n"
	"out vec2 TexCoord0;\n"
	"in vec4 PointColor[1];\n"
	"out vec4 TextureColor;\n"
	"\n"
	"void main()\n"
	"{\n"
   	"	int i;\n"
	"	vec4 vertex = cl_ModelViewProjectionMatrix * gl_in[0].gl_Position;\n"
	"	vec4 scale = cl_ModelViewProjectionMatrix * vec4(1.0, 0.0, 0.0, 0.0);\n"
	"	float size = 1.5 * length(scale.xyz);"
	"	const float texture_scale = 1.5;"
	"	TexCoord0 = vec2(-0.5, texture_scale);"

	"	TextureColor = PointColor[0];"
	"	gl_Position = vertex + vec4(size, size, 0.0, 0.0);\n"
	"	EmitVertex();\n"

	"	TexCoord0 = vec2(texture_scale+ 0.5, texture_scale);"
	"	TextureColor = PointColor[0];"
	"	gl_Position = vertex + vec4(-size, size, 0.0, 0.0);\n"
	"	EmitVertex();\n"

	"	TexCoord0 = vec2(0.5, -0.5);"
	"	TextureColor = PointColor[0];"
	"	gl_Position = vertex + vec4(0.0, -size, 0.0, 0.0);\n"
	"	EmitVertex();\n"

   	"	EndPrimitive();\n"
	"	\n"
	"}\n"
	;

char ShaderColorGeometry::fragment[] =
	"\n"
	"#version 130\n"
	"\n"
	"in vec2 TexCoord0;\n"
	"uniform sampler2D Texture0;\n"
	"in vec4 TextureColor;\n"
	"out vec4 cl_FragColor;"
	"\n"
	"void main()\n"
	"{\n"
	"	vec4 fragment = texture2D(Texture0, TexCoord0);\n"
	"	fragment = TextureColor * fragment;"
	"\n"
	"   if (fragment.a < 0.2) discard;\n"
	"	cl_FragColor = fragment;\n"
	"}\n"
	;

ShaderColorGeometry::ShaderColorGeometry(GraphicContext &gc)
{
	ShaderObject vertex_shader(gc, ShaderType::vertex, vertex);
	if(!vertex_shader.compile())
	{
		throw Exception(string_format("Unable to compile vertex shader object: %1", vertex_shader.get_info_log()));
	}

	ShaderObject geometry_shader(gc, ShaderType::geometry, geometry);
	if(!geometry_shader.compile())
	{
		throw Exception(string_format("Unable to compile geometry shader object: %1", geometry_shader.get_info_log()));
	}

	if (!glProgramParameteri)
		throw Exception("Geomtry shader is not available");

	ShaderObject fragment_shader(gc, ShaderType::fragment, fragment);
	if(!fragment_shader.compile())
	{
		throw Exception(string_format("Unable to compile fragment shader object: %1", fragment_shader.get_info_log()));
	}

	program_object = ProgramObject(gc);
	program_object.attach(vertex_shader);
	program_object.attach(geometry_shader);
	program_object.attach(fragment_shader);
	program_object.bind_attribute_location(0, "InPosition");
	program_object.bind_attribute_location(1, "InColor");
	program_object.bind_frag_data_location(0, "cl_FragColor");

	if (!program_object.link())
	{
		throw Exception(string_format("Unable to link program object: %1", program_object.get_info_log()));
	}
	
	program_object.set_uniform1i("Texture0", 0);

	gpu_uniforms = clan::UniformVector<ProgramUniforms>(gc, 1);


}

void ShaderColorGeometry::Use(GraphicContext &gc, const Mat4f &matrix_modelview_projection)
{
	ProgramUniforms uniforms;

	uniforms.cl_ModelViewProjectionMatrix = matrix_modelview_projection;
	gpu_uniforms.upload_data(gc, &uniforms, 1);
	gc.set_uniform_buffer(0, gpu_uniforms);

	gc.set_program_object(program_object);
}

