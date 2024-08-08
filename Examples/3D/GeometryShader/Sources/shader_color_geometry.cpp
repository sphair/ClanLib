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

char ShaderColorGeometry::vertex[] = R"(
#version 430 core

layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec4 InColor;

out vec4 PointColor;

void main()
{
    vec4 in_position = vec4(InPosition, 1.0);
    PointColor = InColor;
    gl_Position = in_position;
}
)";


char ShaderColorGeometry::geometry[] = R"(
#version 430 core

layout(points) in;
layout(triangle_strip, max_vertices = 3) out;

layout (std140) uniform ProgramUniforms
{
    mat4 cl_ModelViewProjectionMatrix;
};

in vec4 PointColor[1];
out vec2 TexCoord0;
out vec4 TextureColor;

void main()
{
    vec4 vertex = cl_ModelViewProjectionMatrix * gl_in[0].gl_Position;
    vec4 scale = cl_ModelViewProjectionMatrix * vec4(1.0, 0.0, 0.0, 0.0);
    float size = 1.5 * length(scale.xyz);
    const float texture_scale = 1.5;

    // First vertex
    TexCoord0 = vec2(-0.5, texture_scale);
    TextureColor = PointColor[0];
    gl_Position = vertex + vec4(size, size, 0.0, 0.0);
    EmitVertex();

    // Second vertex
    TexCoord0 = vec2(texture_scale + 0.5, texture_scale);
    TextureColor = PointColor[0];
    gl_Position = vertex + vec4(-size, size, 0.0, 0.0);
    EmitVertex();

    // Third vertex
    TexCoord0 = vec2(0.5, -0.5);
    TextureColor = PointColor[0];
    gl_Position = vertex + vec4(0.0, -size, 0.0, 0.0);
    EmitVertex();

    EndPrimitive();
}
)";

char ShaderColorGeometry::fragment[] = R"(
#version 430 core

in vec2 TexCoord0;
uniform sampler2D Texture0;
in vec4 TextureColor;

out vec4 cl_FragColor;

void main()
{
    vec4 fragment = texture(Texture0, TexCoord0);
    fragment = TextureColor * fragment;

    if (fragment.a < 0.2) discard;
    cl_FragColor = fragment;
}
)";

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

