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

const std::string::value_type *cl_glsl15_vertex_color_only = 
	"#version 150\n"
	"in vec4 Position, Color0; "
	"out vec4 Color; "
	"void main() { gl_Position = Position; Color = Color0; }";

const std::string::value_type *cl_glsl_vertex_color_only = 
	"#version 130\n"
	"in vec4 Position, Color0; "
	"out vec4 Color; "
	"void main() { gl_Position = Position; Color = Color0; }";

const std::string::value_type *cl_glsl15_fragment_color_only =
	"#version 150\n"
	"in vec4 Color; "
	"out vec4 cl_FragColor;"
	"void main() { cl_FragColor = Color; }";

const std::string::value_type *cl_glsl_fragment_color_only =
	"#version 130\n"
	"in vec4 Color; "
	"void main() { gl_FragColor = Color; }";

const std::string::value_type *cl_glsl15_vertex_single_texture =
	"#version 150\n"
	"in vec4 Position, Color0; "
	"in vec2 TexCoord0; "
	"out vec4 Color; "
	"out vec2 TexCoord; "
	"void main() { gl_Position = Position; Color = Color0; TexCoord = TexCoord0; }";

const std::string::value_type *cl_glsl_vertex_single_texture =
	"#version 130\n"
	"in vec4 Position, Color0; "
	"in vec2 TexCoord0; "
	"out vec4 Color; "
	"out vec2 TexCoord; "
	"void main() { gl_Position = Position; Color = Color0; TexCoord = TexCoord0; }";

const std::string::value_type *cl_glsl15_fragment_single_texture =
	"#version 150\n"
	"uniform sampler2D Texture0; "
	"in vec4 Color; "
	"in vec2 TexCoord; "
	"out vec4 cl_FragColor;"
	"void main() { cl_FragColor = Color*texture(Texture0, TexCoord); }";

const std::string::value_type *cl_glsl_fragment_single_texture =
	"#version 130\n"
	"uniform sampler2D Texture0; "
	"in vec4 Color; "
	"in vec2 TexCoord; "
	"void main() { gl_FragColor = Color*texture(Texture0, TexCoord); }";

const std::string::value_type *cl_glsl15_vertex_sprite =
	"#version 150\n"
	"in vec4 Position, Color0; "
	"in vec2 TexCoord0; "
	"in int TexIndex0; "
	"out vec4 Color; "
	"out vec2 TexCoord; "
	"flat out int TexIndex; "
	"void main() { gl_Position = Position; Color = Color0; TexCoord = TexCoord0; TexIndex = TexIndex0; }";

const std::string::value_type *cl_glsl_vertex_sprite =
	"#version 130\n"
	"in vec4 Position, Color0; "
	"in vec2 TexCoord0; "
	"in int TexIndex0; "
	"out vec4 Color; "
	"out vec2 TexCoord; "
	"flat out int TexIndex; "
	"void main() { gl_Position = Position; Color = Color0; TexCoord = TexCoord0; TexIndex = TexIndex0; }";

const std::string::value_type *cl_glsl15_fragment_sprite =
	"#version 150\n"
	"uniform sampler2D Texture0; "
	"uniform sampler2D Texture1; "
	"uniform sampler2D Texture2; "
	"uniform sampler2D Texture3; "
	"uniform sampler2D Texture4; "
	"uniform sampler2D Texture5; "
	"uniform sampler2D Texture6; "
	"uniform sampler2D Texture7; "
	"uniform sampler2D Texture8; "
	"uniform sampler2D Texture9; "
	"uniform sampler2D Texture10; "
	"uniform sampler2D Texture11; "
	"uniform sampler2D Texture12; "
	"uniform sampler2D Texture13; "
	"uniform sampler2D Texture14; "
	"uniform sampler2D Texture15; "
	"in vec4 Color; "
	"in vec2 TexCoord; "
	"flat in int TexIndex; "
	"out vec4 cl_FragColor; "
	"highp vec4 sampleTexture(int index, highp vec2 pos)"
	"{ "
		"switch (index) "
		"{ "
			"case 0: return texture(Texture0, TexCoord); "
			"case 1: return texture(Texture1, TexCoord); "
			"case 2: return texture(Texture2, TexCoord); "
			"case 3: return texture(Texture3, TexCoord); "
			"case 4: return texture(Texture4, TexCoord); "
			"case 5: return texture(Texture5, TexCoord); "
			"case 6: return texture(Texture6, TexCoord); "
			"case 7: return texture(Texture7, TexCoord); "
			"case 8: return texture(Texture8, TexCoord); "
			"case 9: return texture(Texture9, TexCoord); "
			"case 10: return texture(Texture10, TexCoord); "
			"case 11: return texture(Texture11, TexCoord); "
			"case 12: return texture(Texture12, TexCoord); "
			"case 13: return texture(Texture13, TexCoord); "
			"case 14: return texture(Texture14, TexCoord); "
			"case 15: return texture(Texture15, TexCoord); "
			"default: return vec4(1.0,1.0,1.0,1.0); "
		"} "
	"} "
	"void main() { cl_FragColor = Color*sampleTexture(TexIndex, TexCoord); } ";

const std::string::value_type *cl_glsl_fragment_sprite =
	"#version 130\n"
	"uniform sampler2D Texture0; "
	"uniform sampler2D Texture1; "
	"uniform sampler2D Texture2; "
	"uniform sampler2D Texture3; "
	"uniform sampler2D Texture4; "
	"uniform sampler2D Texture5; "
	"uniform sampler2D Texture6; "
	"uniform sampler2D Texture7; "
	"uniform sampler2D Texture8; "
	"uniform sampler2D Texture9; "
	"uniform sampler2D Texture10; "
	"uniform sampler2D Texture11; "
	"uniform sampler2D Texture12; "
	"uniform sampler2D Texture13; "
	"uniform sampler2D Texture14; "
	"uniform sampler2D Texture15; "
	"in vec4 Color; "
	"in vec2 TexCoord; "
	"flat in int TexIndex; "
	"vec4 sampleTexture(int index, vec2 pos) "
	"{ "
		"switch (index) "
		"{ "
			"case 0: return texture(Texture0, TexCoord); "
			"case 1: return texture(Texture1, TexCoord); "
			"case 2: return texture(Texture2, TexCoord); "
			"case 3: return texture(Texture3, TexCoord); "
			"case 4: return texture(Texture4, TexCoord); "
			"case 5: return texture(Texture5, TexCoord); "
			"case 6: return texture(Texture6, TexCoord); "
			"case 7: return texture(Texture7, TexCoord); "
			"case 8: return texture(Texture8, TexCoord); "
			"case 9: return texture(Texture9, TexCoord); "
			"case 10: return texture(Texture10, TexCoord); "
			"case 11: return texture(Texture11, TexCoord); "
			"case 12: return texture(Texture12, TexCoord); "
			"case 13: return texture(Texture13, TexCoord); "
			"case 14: return texture(Texture14, TexCoord); "
			"case 15: return texture(Texture15, TexCoord); "
			"default: return vec4(1.0,1.0,1.0,1.0); "
		"} "
	"} "
	"void main() { gl_FragColor = Color*sampleTexture(TexIndex, TexCoord); } ";


const std::string::value_type *cl_glsl15_vertex_path = R"shaderend(
			#version 150
			in vec4 Position;
			in vec4 Color0;
			in vec2 TexCoord0;
			out vec4 Color;
			out vec2 TexCoord;
			void main()
			{
				gl_Position = Position;
				Color = Color0;
				TexCoord = TexCoord0;
			}
		)shaderend";

const std::string::value_type *cl_glsl_vertex_path =
"#version 130\n"
"in vec4 Position, Color0; "
"in vec2 TexCoord0; "
"in int TexIndex0; "
"out vec4 Color; "
"out vec2 TexCoord; "
"void main() { gl_Position = Position; Color = Color0; TexCoord = TexCoord0; }";

const std::string::value_type *cl_glsl15_fragment_path = R"shaderend(
			#version 150
			uniform sampler2D Mask;
			in vec4 Color;
			in vec2 TexCoord;
			out vec4 cl_FragColor;
			void main()
			{
				float alpha = pow(texture(Mask, TexCoord).r, 2.2);
				cl_FragColor = Color * alpha;
			}
		)shaderend";

const std::string::value_type *cl_glsl_fragment_path =
"#version 130\n"
"uniform sampler2D Mask; "
"in vec4 Color; "
"in vec2 TexCoord; "
"void main() { float alpha = pow(texture(Mask, TexCoord).r, 2.2); gl_FragColor = Color * alpha;} ";

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
	bool use_glsl_150 = false;
	int glsl_version_major;
	int glsl_version_minor;
	provider->get_opengl_shading_language_version(glsl_version_major, glsl_version_minor);
	if ( glsl_version_major == 1)
	{
		if ( glsl_version_minor >= 50)
			use_glsl_150 = true;
	}
	if ( glsl_version_major > 1)
			use_glsl_150 = true;

	ShaderObject vertex_color_only_shader(provider, shadertype_vertex,  use_glsl_150 ? cl_glsl15_vertex_color_only : cl_glsl_vertex_color_only);
	if(!vertex_color_only_shader.compile())
		throw Exception("Unable to compile the standard shader program: 'vertex color only' Error:" + vertex_color_only_shader.get_info_log());

	ShaderObject fragment_color_only_shader(provider, shadertype_fragment, use_glsl_150 ? cl_glsl15_fragment_color_only : cl_glsl_fragment_color_only);
	if(!fragment_color_only_shader.compile())
		throw Exception("Unable to compile the standard shader program: 'fragment color only' Error:" + fragment_color_only_shader.get_info_log());

	ShaderObject vertex_single_texture_shader(provider, shadertype_vertex, use_glsl_150 ? cl_glsl15_vertex_single_texture : cl_glsl_vertex_single_texture);
	if(!vertex_single_texture_shader.compile())
		throw Exception("Unable to compile the standard shader program: 'vertex single texture' Error:" + vertex_single_texture_shader.get_info_log());

	ShaderObject fragment_single_texture_shader(provider, shadertype_fragment, use_glsl_150 ? cl_glsl15_fragment_single_texture : cl_glsl_fragment_single_texture);
	if(!fragment_single_texture_shader.compile())
		throw Exception("Unable to compile the standard shader program: 'fragment single texture' Error:" + fragment_single_texture_shader.get_info_log());

	ShaderObject vertex_sprite_shader(provider, shadertype_vertex, use_glsl_150 ? cl_glsl15_vertex_sprite : cl_glsl_vertex_sprite);
	if(!vertex_sprite_shader.compile())
		throw Exception("Unable to compile the standard shader program: 'vertex sprite' Error:" + vertex_sprite_shader.get_info_log());

	ShaderObject fragment_sprite_shader(provider, shadertype_fragment, use_glsl_150 ? cl_glsl15_fragment_sprite : cl_glsl_fragment_sprite);
	if(!fragment_sprite_shader.compile())
		throw Exception("Unable to compile the standard shader program: 'fragment sprite' Error:" + fragment_sprite_shader.get_info_log());

	ShaderObject vertex_path_shader(provider, shadertype_vertex, use_glsl_150 ? cl_glsl15_vertex_path : cl_glsl_vertex_path);
	if (!vertex_path_shader.compile())
		throw Exception("Unable to compile the standard shader program: 'vertex path' Error:" + vertex_path_shader.get_info_log());

	ShaderObject fragment_path_shader(provider, shadertype_fragment, use_glsl_150 ? cl_glsl15_fragment_path : cl_glsl_fragment_path);
	if (!fragment_path_shader.compile())
		throw Exception("Unable to compile the standard shader program: 'fragment path' Error:" + fragment_path_shader.get_info_log());

	ProgramObject color_only_program(provider);
	color_only_program.attach(vertex_color_only_shader);
	color_only_program.attach(fragment_color_only_shader);
	color_only_program.bind_attribute_location(0, "Position");
	color_only_program.bind_attribute_location(1, "Color0");

	if (use_glsl_150)
		color_only_program.bind_frag_data_location(0, "cl_FragColor");

	if (!color_only_program.link())
		throw Exception("Unable to link the standard shader program: 'color only' Error:" + color_only_program.get_info_log());

	ProgramObject single_texture_program(provider);
	single_texture_program.attach(vertex_single_texture_shader);
	single_texture_program.attach(fragment_single_texture_shader);
	single_texture_program.bind_attribute_location(0, "Position");
	single_texture_program.bind_attribute_location(1, "Color0");
	single_texture_program.bind_attribute_location(2, "TexCoord0");

	if (use_glsl_150)
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

	if (use_glsl_150)
		sprite_program.bind_frag_data_location(0, "cl_FragColor");

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
	path_program.bind_attribute_location(0, "Position");
	path_program.bind_attribute_location(1, "Color0");
	path_program.bind_attribute_location(2, "TexCoord0");

	if (use_glsl_150)
		path_program.bind_frag_data_location(0, "cl_FragColor");

	if (!path_program.link())
		throw Exception("Unable to link the standard shader program: 'path' Error:" + path_program.get_info_log());
	path_program.set_uniform1i("Mask", 0);

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
	case program_color_only: return impl->color_only_program;
	case program_single_texture: return impl->single_texture_program;
	case program_sprite: return impl->sprite_program;
	case program_path: return impl->path_program;
	}
	throw Exception("Unsupported standard program");
}



}

