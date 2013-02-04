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

#include "D3D/precomp.h"
#include "API/Core/Text/string_format.h"
#include "standard_programs.h"

namespace clan
{

class StandardPrograms_Impl
{
public:
	StandardPrograms_Impl() {}
	~StandardPrograms_Impl() {}

	ProgramObject color_only_program;
	ProgramObject single_texture_program;
	ProgramObject sprite_program;

};

StandardPrograms::StandardPrograms()
{
}
StandardPrograms::StandardPrograms(GraphicContext &gc) : impl(new StandardPrograms_Impl())
{
	ProgramObject color_only_program;
	ProgramObject single_texture_program;
	ProgramObject sprite_program;

	color_only_program = compile(gc, color_only_vertex, color_only_fragment);
	color_only_program.bind_attribute_location(0, "VertexPosition");
	color_only_program.bind_attribute_location(1, "VertexColor");
	link(color_only_program, "Unable to link color-only standard program");
	color_only_program.set_uniform_buffer_index("Uniforms", 0);

	single_texture_program = compile(gc, single_texture_vertex, single_texture_fragment);
	single_texture_program.bind_attribute_location(0, "VertexPosition");
	single_texture_program.bind_attribute_location(1, "VertexColor");
	single_texture_program.bind_attribute_location(2, "VertexTexCoord");
	link(single_texture_program, "Unable to link single-texture standard program");
	single_texture_program.set_uniform_buffer_index("Uniforms", 0);
	single_texture_program.set_uniform1i("Texture0", 0);
	single_texture_program.set_uniform1i("Sampler0", 0);

	sprite_program = compile(gc, sprite_vertex, sprite_fragment);
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

	impl->color_only_program = color_only_program;
	impl->single_texture_program = single_texture_program;
	impl->sprite_program = sprite_program;

}

ProgramObject StandardPrograms::get_program_object(StandardProgram standard_program) const
{
	switch (standard_program)
	{
	case program_color_only: return impl->color_only_program;
	case program_single_texture: return impl->single_texture_program;
	case program_sprite: return impl->sprite_program;
	}
	throw Exception("Unsupported standard program");
}

ProgramObject StandardPrograms::compile(GraphicContext &gc, const std::string &vertex_code, const std::string &fragment_code)
{
	ShaderObject vertex_shader(gc, shadertype_vertex, vertex_code);
	if (!vertex_shader.compile())
		throw Exception(string_format("Unable to compile standard vertex shader: %1", vertex_shader.get_info_log()));

	ShaderObject fragment_shader(gc, shadertype_fragment, fragment_code);
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

const char *StandardPrograms::color_only_vertex =
	"struct VertexIn\r\n"
	"{\r\n"
	"	float4 position : VertexPosition;\r\n"
	"	float4 color : VertexColor;\r\n"
	"};\r\n"
	"struct VertexOut\r\n"
	"{\r\n"
	"	float4 position : SV_Position;\r\n"
	"	float4 color : PixelColor;\r\n"
	"};\r\n"
	"VertexOut main(VertexIn input)\r\n"
	"{\r\n"
	"	VertexOut output;\r\n"
	"	output.position = input.position;\r\n"
	"	output.color = input.color;\r\n"
	"	return output;\r\n"
	"}\r\n";

const char *StandardPrograms::color_only_fragment =
	"struct PixelIn\r\n"
	"{\r\n"
	"	float4 screenpos : SV_Position;\r\n"
	"	float4 color : PixelColor;\r\n"
	"};\r\n"
	"struct PixelOut\r\n"
	"{\r\n"
	"	float4 color : SV_Target0;\r\n"
	"};\r\n"
	"PixelOut main(PixelIn input)\r\n"
	"{\r\n"
	"	PixelOut output;\r\n"
	"	output.color = input.color;\r\n"
	"	return output;\r\n"
	"}\r\n";

const char *StandardPrograms::single_texture_vertex =
	"struct VertexIn\r\n"
	"{\r\n"
	"	float4 position : VertexPosition;\r\n"
	"	float4 color : VertexColor;\r\n"
	"	float2 uv : VertexTexCoord;\r\n"
	"};\r\n"
	"struct VertexOut\r\n"
	"{\r\n"
	"	float4 position : SV_Position;\r\n"
	"	float4 color : PixelColor;\r\n"
	"	float2 uv : PixelTexCoord;\r\n"
	"};\r\n"
	"VertexOut main(VertexIn input)\r\n"
	"{\r\n"
	"	VertexOut output;\r\n"
	"	output.position = input.position;\r\n"
	"	output.color = input.color;\r\n"
	"	output.uv = input.uv;\r\n"
	"	return output;\r\n"
	"}\r\n";

const char *StandardPrograms::single_texture_fragment =
	"struct PixelIn\r\n"
	"{\r\n"
	"	float4 screenpos : SV_Position;\r\n"
	"	float4 color : PixelColor;\r\n"
	"	float2 uv : PixelTexCoord;\r\n"
	"};\r\n"
	"struct PixelOut\r\n"
	"{\r\n"
	"	float4 color : SV_Target0;\r\n"
	"};\r\n"
	"Texture2D Texture0;\r\n"
	"SamplerState Sampler0;\r\n"
	"PixelOut main(PixelIn input)\r\n"
	"{\r\n"
	"	PixelOut output;\r\n"
	"	output.color = Texture0.Sample(Sampler0, input.uv) * input.color;\r\n"
	"	return output;\r\n"
	"}\r\n";

const char *StandardPrograms::sprite_vertex =
	"struct VertexIn\r\n"
	"{\r\n"
	"	float4 position : VertexPosition;\r\n"
	"	float4 color : VertexColor;\r\n"
	"	float2 uv : VertexTexCoord;\r\n"
	"	int texindex : VertexTexIndex;\r\n"
	"};\r\n"
	"struct VertexOut\r\n"
	"{\r\n"
	"	float4 position : SV_Position;\r\n"
	"	float4 color : PixelColor;\r\n"
	"	float2 uv : PixelTexCoord;\r\n"
	"	int texindex : PixelTexIndex;\r\n"
	"};\r\n"
	"VertexOut main(VertexIn input)\r\n"
	"{\r\n"
	"	VertexOut output;\r\n"
	"	output.position = input.position;\r\n"
	"	output.color = input.color;\r\n"
	"	output.uv = input.uv;\r\n"
	"	output.texindex = input.texindex;\r\n"
	"	return output;\r\n"
	"}\r\n";

const char *StandardPrograms::sprite_fragment =
	"struct PixelIn\r\n"
	"{\r\n"
	"	float4 screenpos : SV_Position;\r\n"
	"	float4 color : PixelColor;\r\n"
	"	float2 uv : PixelTexCoord;\r\n"
	"	int texindex : PixelTexIndex;\r\n"
	"};\r\n"
	"struct PixelOut\r\n"
	"{\r\n"
	"	float4 color : SV_Target0;\r\n"
	"};\r\n"
	"Texture2D Texture0;\r\n"
	"Texture2D Texture1;\r\n"
	"Texture2D Texture2;\r\n"
	"Texture2D Texture3;\r\n"
	"SamplerState Sampler0;\r\n"
	"SamplerState Sampler1;\r\n"
	"SamplerState Sampler2;\r\n"
	"SamplerState Sampler3;\r\n"
	"PixelOut main(PixelIn input)\r\n"
	"{\r\n"
	"	int index = input.texindex;\r\n"
	"	PixelOut output;\r\n"
	"	if (index == 0)\r\n"
	"		output.color = Texture0.Sample(Sampler0, input.uv) * input.color;\r\n"
	"	else if (index == 1)\r\n"
	"		output.color = Texture1.Sample(Sampler1, input.uv) * input.color;\r\n"
	"	else if (index == 2)\r\n"
	"		output.color = Texture2.Sample(Sampler2, input.uv) * input.color;\r\n"
	"	else if (index == 3)\r\n"
	"		output.color = Texture3.Sample(Sampler3, input.uv) * input.color;\r\n"
	"	else\r\n"
	"		output.color = input.color;\r\n"
	"	return output;\r\n"
	"}\r\n";

}
