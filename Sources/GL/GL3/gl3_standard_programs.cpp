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
**    Magnus Norddahl
**    Mark Page
*/

#include "GL/precomp.h"
#include "gl3_standard_programs.h"
#include "gl3_program_object_provider.h"
#include "gl3_shader_object_provider.h"
#include "gl3_render_buffer_provider.h"
#include "gl3_vertex_array_buffer_provider.h"
#include "Display/2D/render_batch_triangle.h"

namespace clan
{
	const std::string::value_type *cl_glsl_vertex_color_only = R"(
#version 430

layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Color0;

out vec4 Color;

void main() {
	gl_Position = Position;
	Color = Color0;
	}
)";

	const std::string::value_type *cl_glsl_fragment_color_only = R"(
#version 430

in vec4 Color;
out vec4 cl_FragColor;

void main() {
	cl_FragColor = Color;
	}
)";

	const std::string::value_type *cl_glsl_vertex_single_texture =R"(
#version 430

layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Color0;
layout(location = 2) in vec2 TexCoord0;

out vec4 Color;
out vec2 TexCoord;

void main() {
    gl_Position = Position;
    Color = Color0;
    TexCoord = TexCoord0;
}
)";

	const std::string::value_type *cl_glsl_fragment_single_texture = R"(
#version 430

uniform sampler2D Texture0;

in vec4 Color;
in vec2 TexCoord;

out vec4 cl_FragColor;

void main() {
    cl_FragColor = Color * texture(Texture0, TexCoord);
}
)";

	const std::string::value_type *cl_glsl_vertex_sprite = R"(
#version 430

layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Color0;
layout(location = 2) in vec2 TexCoord0;
layout(location = 3) in int TexIndex0;

out vec4 Color;
out vec2 TexCoord;
flat out int TexIndex;

void main() {
    gl_Position = Position;
    Color = Color0;
    TexCoord = TexCoord0;
    TexIndex = TexIndex0;
}
)";

	const std::string::value_type *cl_glsl_fragment_sprite = R"(
#version 430

uniform sampler2D Texture0;
uniform sampler2D Texture1;
uniform sampler2D Texture2;
uniform sampler2D Texture3;
uniform sampler2D Texture4;
uniform sampler2D Texture5;
uniform sampler2D Texture6;
uniform sampler2D Texture7;
uniform sampler2D Texture8;
uniform sampler2D Texture9;
uniform sampler2D Texture10;
uniform sampler2D Texture11;
uniform sampler2D Texture12;
uniform sampler2D Texture13;
uniform sampler2D Texture14;
uniform sampler2D Texture15;

in vec4 Color;
in vec2 TexCoord;
flat in int TexIndex;
out vec4 cl_FragColor;

highp vec4 sampleTexture(int index, highp vec2 pos)
{ 
    switch (index)
    { 
        case 0: return texture(Texture0, pos);
        case 1: return texture(Texture1, pos);
        case 2: return texture(Texture2, pos);
        case 3: return texture(Texture3, pos);
        case 4: return texture(Texture4, pos);
        case 5: return texture(Texture5, pos);
        case 6: return texture(Texture6, pos);
        case 7: return texture(Texture7, pos);
        case 8: return texture(Texture8, pos);
        case 9: return texture(Texture9, pos);
        case 10: return texture(Texture10, pos);
        case 11: return texture(Texture11, pos);
        case 12: return texture(Texture12, pos);
        case 13: return texture(Texture13, pos);
        case 14: return texture(Texture14, pos);
        case 15: return texture(Texture15, pos);
        default: return vec4(1.0, 1.0, 1.0, 1.0);
    }
}

void main() {
    cl_FragColor = Color * sampleTexture(TexIndex, TexCoord);
}
)";

	const std::string::value_type* cl_glsl_vertex_path = R"(
#version 430

layout(location = 0) in ivec4 Vertex;
flat out vec4 brush_data1;
flat out vec4 brush_data2;
out vec4 vary_data;
out vec2 mask_position;
uniform sampler2D instance_data;
uniform float ypos_scale;
flat out ivec2 instance_offset;

void main()
{
    const int mask_block_size = 16;
    const int mask_width = 1024;
    const int instance_width = 512;

    vec4 canvas_data = texelFetch(instance_data, ivec2(0, 0), 0);
    ivec2 size = ivec2( (Vertex.z % 2), (Vertex.z / 2) );
    gl_Position = vec4(((Vertex.x+size.x * mask_block_size)*2.0 / canvas_data.x) - 1.0, ypos_scale * (((Vertex.y+size.y * mask_block_size)*-2.0 / canvas_data.y) + 1.0), 0.0, 1.0);

    int mask_offset = Vertex.w % 65536;

    int y_offset = (mask_offset*mask_block_size) / mask_width;
    mask_position = vec2(mask_offset * mask_block_size - y_offset * mask_width, y_offset * mask_block_size);
    mask_position += size * mask_block_size;
    mask_position /= mask_width;

    int instance_block = Vertex.w / 65536;

    y_offset = instance_block / instance_width;
    instance_offset = ivec2(instance_block - y_offset *instance_width, y_offset);

    brush_data1 = texelFetch(instance_data, instance_offset, 0);
    brush_data2 = texelFetch(instance_data, ivec2(instance_offset.x + 1, instance_offset.y), 0);
    vec4 brush_data3 = texelFetch(instance_data, ivec2(instance_offset.x + 2, instance_offset.y), 0);

    // Calculate for linear and radial gradient
    vary_data.x = Vertex.x + size.x * mask_block_size - brush_data3.x;
    vary_data.y = Vertex.y + size.y * mask_block_size - brush_data3.y;

    // Calculate for texture coords
    vary_data.z = (Vertex.x+size.x * mask_block_size);
    vary_data.w = (Vertex.y+size.y * mask_block_size);

    mat4 inv_transform = mat4(
            texelFetch(instance_data, ivec2(instance_offset.x + 2, instance_offset.y), 0),
            texelFetch(instance_data, ivec2(instance_offset.x + 3, instance_offset.y), 0),
            texelFetch(instance_data, ivec2(instance_offset.x + 4, instance_offset.y), 0),
            texelFetch(instance_data, ivec2(instance_offset.x + 5, instance_offset.y), 0));
    vary_data.zw = (vec4(vary_data.zw, 0, 1) * transpose(inv_transform)).xy;

    vary_data.z = (vary_data.z + brush_data1.x) / brush_data2.x;
    vary_data.w = (vary_data.w + brush_data1.y) / brush_data2.y;
}
)";

	const std::string::value_type* cl_glsl_fragment_path = R"(
#version 430

in vec2 mask_position;
flat in vec4 brush_data1;
flat in vec4 brush_data2;
in vec4 vary_data;
out vec4 cl_FragColor;
flat in ivec2 instance_offset;

uniform sampler2D instance_data;
uniform sampler2D image_texture;
uniform sampler2D mask_texture;

vec4 mask(vec4 color)
{
    return color * texture(mask_texture, mask_position).r;
}

void solid_fill()
{
    vec4 fill_color = brush_data2;
    cl_FragColor = mask(fill_color);
}

vec4 gradient_color(int stop_start, int stop_end, float t)
{
    vec4 color = texelFetch(instance_data, ivec2(instance_offset.x + stop_start, instance_offset.y), 0);
    float last_stop_pos = texelFetch(instance_data, ivec2(instance_offset.x + stop_start + 1, instance_offset.y), 0).x;
    for (int i = stop_start; i < stop_end; i+=2)
    {
        vec4 stop_color = texelFetch(instance_data, ivec2(instance_offset.x + i, instance_offset.y), 0);
        float stop_pos = texelFetch(instance_data, ivec2(instance_offset.x + i + 1, instance_offset.y), 0).x;
        float tt = clamp((t - last_stop_pos)/(stop_pos - last_stop_pos), 0.0, 1.0);
        color = mix(color, stop_color, tt);
        last_stop_pos = stop_pos;
    }
    return color;
}

void linear_gradient_fill()
{
    vec2 grad_start = vary_data.xy;
    vec2 grad_dir = brush_data1.zw;
    float rcp_grad_length = brush_data2.x;
    int stop_start = int(brush_data2.y);
    int stop_end = int(brush_data2.z);

    float t = dot(grad_start, grad_dir) * rcp_grad_length;
    cl_FragColor = mask(gradient_color(stop_start, stop_end, t));
}

void radial_gradient_fill()
{
    vec2 grad_center = vary_data.xy;
    float rcp_grad_length = brush_data2.x;
    int stop_start = int(brush_data2.y);
    int stop_end = int(brush_data2.z);

    float t = length(grad_center) * rcp_grad_length;
    cl_FragColor = mask(gradient_color(stop_start, stop_end, t));
}

void image_fill()
{
    vec2 uv = vary_data.zw;
    cl_FragColor = mask(texture(image_texture, uv));
}

void main()
{
    switch (int(brush_data1.x))
    {
    default:
    case 0: solid_fill(); break;
    case 1: linear_gradient_fill(); break;
    case 2: radial_gradient_fill(); break;
    case 3: image_fill(); break;
    }
}
)";

	class GL3StandardPrograms_Impl
	{
	public:
		GL3StandardPrograms_Impl() {}
		~GL3StandardPrograms_Impl() {}

		ProgramObject color_only_program;
		ProgramObject single_texture_program;
		ProgramObject sprite_program;
		ProgramObject path_program;

	};

	GL3StandardPrograms::GL3StandardPrograms()
	{
	}

	GL3StandardPrograms::GL3StandardPrograms(GL3GraphicContextProvider *provider) : impl(std::make_shared<GL3StandardPrograms_Impl>())
	{
		//int glsl_version_major;
		//int glsl_version_minor;
		//provider->get_opengl_shading_language_version(glsl_version_major, glsl_version_minor);

		ShaderObject vertex_color_only_shader(provider, ShaderType::vertex, cl_glsl_vertex_color_only);
		if (!vertex_color_only_shader.compile())
			throw Exception("Unable to compile the standard shader program: 'vertex color only' Error:" + vertex_color_only_shader.get_info_log());

		ShaderObject fragment_color_only_shader(provider, ShaderType::fragment, cl_glsl_fragment_color_only);
		if (!fragment_color_only_shader.compile())
			throw Exception("Unable to compile the standard shader program: 'fragment color only' Error:" + fragment_color_only_shader.get_info_log());

		ShaderObject vertex_single_texture_shader(provider, ShaderType::vertex, cl_glsl_vertex_single_texture);
		if (!vertex_single_texture_shader.compile())
			throw Exception("Unable to compile the standard shader program: 'vertex single texture' Error:" + vertex_single_texture_shader.get_info_log());

		ShaderObject fragment_single_texture_shader(provider, ShaderType::fragment, cl_glsl_fragment_single_texture);
		if (!fragment_single_texture_shader.compile())
			throw Exception("Unable to compile the standard shader program: 'fragment single texture' Error:" + fragment_single_texture_shader.get_info_log());

		ShaderObject vertex_sprite_shader(provider, ShaderType::vertex,cl_glsl_vertex_sprite);
		if (!vertex_sprite_shader.compile())
			throw Exception("Unable to compile the standard shader program: 'vertex sprite' Error:" + vertex_sprite_shader.get_info_log());

		ShaderObject fragment_sprite_shader(provider, ShaderType::fragment, cl_glsl_fragment_sprite);
		if (!fragment_sprite_shader.compile())
			throw Exception("Unable to compile the standard shader program: 'fragment sprite' Error:" + fragment_sprite_shader.get_info_log());

		ShaderObject vertex_path_shader(provider, ShaderType::vertex, cl_glsl_vertex_path);
		if (!vertex_path_shader.compile())
			throw Exception("Unable to compile the standard shader program: 'vertex path' Error:" + vertex_path_shader.get_info_log());

		ShaderObject fragment_path_shader(provider, ShaderType::fragment, cl_glsl_fragment_path);
		if (!fragment_path_shader.compile())
			throw Exception("Unable to compile the standard shader program: 'fragment path' Error:" + fragment_path_shader.get_info_log());

		ProgramObject color_only_program(provider);
		color_only_program.attach(vertex_color_only_shader);
		color_only_program.attach(fragment_color_only_shader);
		color_only_program.bind_attribute_location(0, "Position");
		color_only_program.bind_attribute_location(1, "Color0");

#ifndef CLANLIB_OPENGL_ES3
		color_only_program.bind_frag_data_location(0, "cl_FragColor");
#endif
		if (!color_only_program.link())
			throw Exception("Unable to link the standard shader program: 'color only' Error:" + color_only_program.get_info_log());

		ProgramObject single_texture_program(provider);
		single_texture_program.attach(vertex_single_texture_shader);
		single_texture_program.attach(fragment_single_texture_shader);
		single_texture_program.bind_attribute_location(0, "Position");
		single_texture_program.bind_attribute_location(1, "Color0");
		single_texture_program.bind_attribute_location(2, "TexCoord0");

#ifndef CLANLIB_OPENGL_ES3
		single_texture_program.bind_frag_data_location(0, "cl_FragColor");
#endif

		if (!single_texture_program.link())
			throw Exception("Unable to link the standard shader program: 'single texture' Error:" + single_texture_program.get_info_log());
		single_texture_program.set_uniform1i("Texture0", 0);

		ProgramObject sprite_program(provider);
		sprite_program.attach(vertex_sprite_shader);
		sprite_program.attach(fragment_sprite_shader);
		sprite_program.bind_attribute_location(0, "Position");
		sprite_program.bind_attribute_location(1, "Color0");
		sprite_program.bind_attribute_location(2, "TexCoord0");
		sprite_program.bind_attribute_location(3, "TexIndex0");

#ifndef CLANLIB_OPENGL_ES3
		sprite_program.bind_frag_data_location(0, "cl_FragColor");
#endif

		if (!sprite_program.link())
			throw Exception("Unable to link the standard shader program: 'sprite' Error:" + sprite_program.get_info_log());

		sprite_program.set_uniform1i("Texture0", 0);
		sprite_program.set_uniform1i("Texture1", 1);
		sprite_program.set_uniform1i("Texture2", 2);
		sprite_program.set_uniform1i("Texture3", 3);
		sprite_program.set_uniform1i("Texture4", 4);
		sprite_program.set_uniform1i("Texture5", 5);
		sprite_program.set_uniform1i("Texture6", 6);
		sprite_program.set_uniform1i("Texture7", 7);
		sprite_program.set_uniform1i("Texture8", 8);
		sprite_program.set_uniform1i("Texture9", 9);
		sprite_program.set_uniform1i("Texture10", 10);
		sprite_program.set_uniform1i("Texture11", 11);
		sprite_program.set_uniform1i("Texture12", 12);
		sprite_program.set_uniform1i("Texture13", 13);
		sprite_program.set_uniform1i("Texture14", 14);
		sprite_program.set_uniform1i("Texture15", 15);

		ProgramObject path_program(provider);
		path_program.attach(vertex_path_shader);
		path_program.attach(fragment_path_shader);
		path_program.bind_attribute_location(0, "Vertex");

#ifndef CLANLIB_OPENGL_ES3
		path_program.bind_frag_data_location(0, "cl_FragColor");
#endif

		if (!path_program.link())
			throw Exception("Unable to link the standard shader program: 'path' Error:" + path_program.get_info_log());
		path_program.set_uniform1i("mask_texture", 0);
		path_program.set_uniform1i("instance_data", 1);
		path_program.set_uniform1i("image_texture", 2);

		impl->color_only_program = color_only_program;
		impl->single_texture_program = single_texture_program;
		impl->sprite_program = sprite_program;
		impl->path_program = path_program;

		RenderBatchTriangle::max_textures = 16; // Too many hacks..
	}

	GL3StandardPrograms::~GL3StandardPrograms()
	{
	}

	ProgramObject GL3StandardPrograms::get_program_object(StandardProgram standard_program) const
	{
		switch (standard_program)
		{
		case StandardProgram::color_only: return impl->color_only_program;
		case StandardProgram::single_texture: return impl->single_texture_program;
		case StandardProgram::sprite: return impl->sprite_program;
		case StandardProgram::path: return impl->path_program;
		}
		throw Exception("Unsupported standard program");
	}
}
