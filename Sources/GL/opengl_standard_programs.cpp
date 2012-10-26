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
**    Mark Page
*/

#include "GL/precomp.h"
#include "opengl_standard_programs.h"
#include "opengl_program_object_provider.h"
#include "opengl_shader_object_provider.h"
#include "opengl_render_buffer_provider.h"
#include "opengl_vertex_array_buffer_provider.h"


namespace clan
{

const std::string::value_type *cl_glsl15_vertex_color_only = 
	"#version 150\n"
	"in vec4 Position, Color0; "
	"out vec4 Color; "
	"void main(void) { gl_Position = Position; Color = Color0; }";

const std::string::value_type *cl_glsl15_fragment_color_only =
	"#version 150\n"
	"in vec4 Color; "
	"out vec4 cl_FragColor;"
	"void main(void) { cl_FragColor = Color; }";

const std::string::value_type *cl_glsl15_vertex_single_texture =
	"#version 150\n"
	"in vec4 Position, Color0; "
	"in vec2 TexCoord0; "
	"out vec4 Color; "
	"out vec2 TexCoord; "
	"void main(void) { gl_Position = Position; Color = Color0; TexCoord = TexCoord0; }";

const std::string::value_type *cl_glsl15_fragment_single_texture =
	"#version 150\n"
	"uniform sampler2D Texture0; "
	"in vec4 Color; "
	"in vec2 TexCoord; "
	"out vec4 cl_FragColor;"
	"void main(void) { cl_FragColor = Color*texture2D(Texture0, TexCoord); }";

const std::string::value_type *cl_glsl15_vertex_sprite =
	"#version 150\n"
	"in vec4 Position, Color0; "
	"in vec2 TexCoord0; "
	"in float TexIndex0; "
	"out vec4 Color; "
	"out vec2 TexCoord; "
	"out float TexIndex; "
	"void main(void) { gl_Position = Position; Color = Color0; TexCoord = TexCoord0; TexIndex = TexIndex0; }";

const std::string::value_type *cl_glsl15_fragment_sprite =
	"#version 150\n"
	"uniform sampler2D Texture0; "
	"uniform sampler2D Texture1; "
	"uniform sampler2D Texture2; "
	"uniform sampler2D Texture3; "
	"in vec4 Color; "
	"in vec2 TexCoord; "
	"in float TexIndex; "
	"out vec4 cl_FragColor;"
	"highp vec4 sampleTexture(int index, highp vec2 pos) { if (index == 0) return texture2D(Texture0, TexCoord); else if (index == 1) return texture2D(Texture1, TexCoord); else if (index == 2) return texture2D(Texture2, TexCoord); else if (index == 3) return texture2D(Texture3, TexCoord); else return vec4(1.0,1.0,1.0,1.0); }"
	"void main(void) { cl_FragColor = Color*sampleTexture(int(TexIndex), TexCoord); } ";



class OpenGLStandardPrograms_Impl
{
public:
	OpenGLStandardPrograms_Impl() {}
	~OpenGLStandardPrograms_Impl() {}

	ProgramObject color_only_program;
	ProgramObject single_texture_program;
	ProgramObject sprite_program;

};

OpenGLStandardPrograms::OpenGLStandardPrograms()
{
}

OpenGLStandardPrograms::OpenGLStandardPrograms(OpenGLGraphicContextProvider *provider) : impl(new OpenGLStandardPrograms_Impl())
{
	ShaderObject vertex_color_only_shader(provider, shadertype_vertex, cl_glsl15_vertex_color_only);
	if(!vertex_color_only_shader.compile())
		throw Exception("Unable to compile the standard shader program: 'vertex color only' Error:" + vertex_color_only_shader.get_info_log());

	ShaderObject fragment_color_only_shader(provider, shadertype_fragment, cl_glsl15_fragment_color_only);
	if(!fragment_color_only_shader.compile())
		throw Exception("Unable to compile the standard shader program: 'fragment color only' Error:" + fragment_color_only_shader.get_info_log());

	ShaderObject vertex_single_texture_shader(provider, shadertype_vertex, cl_glsl15_vertex_single_texture);
	if(!vertex_single_texture_shader.compile())
		throw Exception("Unable to compile the standard shader program: 'vertex single texture' Error:" + vertex_single_texture_shader.get_info_log());

	ShaderObject fragment_single_texture_shader(provider, shadertype_fragment, cl_glsl15_fragment_single_texture);
	if(!fragment_single_texture_shader.compile())
		throw Exception("Unable to compile the standard shader program: 'fragment single texture' Error:" + fragment_single_texture_shader.get_info_log());

	ShaderObject vertex_sprite_shader(provider, shadertype_vertex, cl_glsl15_vertex_sprite);
	if(!vertex_sprite_shader.compile())
		throw Exception("Unable to compile the standard shader program: 'vertex sprite' Error:" + vertex_sprite_shader.get_info_log());

	ShaderObject fragment_sprite_shader(provider, shadertype_fragment, cl_glsl15_fragment_sprite);
	if(!fragment_sprite_shader.compile())
		throw Exception("Unable to compile the standard shader program: 'fragment sprite' Error:" + fragment_sprite_shader.get_info_log());

	ProgramObject color_only_program(provider);
	color_only_program.attach(vertex_color_only_shader);
	color_only_program.attach(fragment_color_only_shader);
	color_only_program.bind_attribute_location(0, "Position");
	color_only_program.bind_attribute_location(1, "Color0");
	color_only_program.bind_frag_data_location(0, "cl_FragColor");

	if (!color_only_program.link())
		throw Exception("Unable to link the standard shader program: 'color only' Error:" + color_only_program.get_info_log());

	ProgramObject single_texture_program(provider);
	single_texture_program.attach(vertex_single_texture_shader);
	single_texture_program.attach(fragment_single_texture_shader);
	single_texture_program.bind_attribute_location(0, "Position");
	single_texture_program.bind_attribute_location(1, "Color0");
	single_texture_program.bind_attribute_location(2, "TexCoord0");
	single_texture_program.bind_frag_data_location(0, "cl_FragColor");
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
	sprite_program.bind_frag_data_location(0, "cl_FragColor");
	if (!sprite_program.link())
		throw Exception("Unable to link the standard shader program: 'sprite' Error:" + sprite_program.get_info_log());

	sprite_program.set_uniform1i("Texture0", 0);
	sprite_program.set_uniform1i("Texture1", 1);
	sprite_program.set_uniform1i("Texture2", 2);
	sprite_program.set_uniform1i("Texture3", 3);

	impl->color_only_program = color_only_program;
	impl->single_texture_program = single_texture_program;
	impl->sprite_program = sprite_program;

}

OpenGLStandardPrograms::~OpenGLStandardPrograms()
{
}

ProgramObject OpenGLStandardPrograms::get_program_object(StandardProgram standard_program) const
{
	switch (standard_program)
	{
	case program_color_only: return impl->color_only_program;
	case program_single_texture: return impl->single_texture_program;
	case program_sprite: return impl->sprite_program;
	}
	throw Exception("Unsupported standard program");
}



}

