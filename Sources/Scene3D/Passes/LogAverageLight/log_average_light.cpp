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
#include "log_average_light.h"
#include "Scene3D/Framework/shader_setup.h"

namespace clan
{


LogAverageLight::LogAverageLight(GraphicContext &gc, int iterations)
: iterations(iterations), current_index(0)
{
	Size texture_size(1 << iterations, 1 << iterations);

	std::string vertex_program =
		"#version 330\r\n"
		"in vec4 PositionInProjection;\r\n"
		"void main()\r\n"
		"{\r\n"
		"	gl_Position = PositionInProjection;\r\n"
		"}\r\n";

	std::string fragment_program0 = string_format(
		"#version 330\r\n"
		"out vec4 FragAverage;\r\n"
		"uniform sampler2D Texture;\r\n"
		"void main()\r\n"
		"{\r\n"
		"	vec3 rgb = texture(Texture, gl_FragCoord.xy / %1).rgb;\r\n"
		"	float light = max(max(rgb.r, rgb.g), rgb.b);\r\n"
		"	FragAverage = vec4(log(0.0001 + light), 0, 0, 0);\r\n"
		"}\r\n",
		texture_size.width);

	std::string fragment_program1 =
		"#version 330\r\n"
		"out vec4 FragAverage;\r\n"
		"uniform sampler2D Texture;\r\n"
		"void main()\r\n"
		"{\r\n"
		"	ivec2 pos = ivec2(gl_FragCoord.xy) * 2;\r\n"
		"	float light0 = texelFetch(Texture, pos, 0).x; \r\n"
		"	float light1 = texelFetch(Texture, pos + ivec2(1,0), 0).x; \r\n"
		"	float light2 = texelFetch(Texture, pos + ivec2(0,1), 0).x; \r\n"
		"	float light3 = texelFetch(Texture, pos + ivec2(1,1), 0).x; \r\n"
		"	FragAverage = vec4((light0 + light1 + light2 + light3) * 0.25, 0, 0, 0);\r\n"
		"}\r\n";

	std::string fragment_program2 =
		"#version 330\r\n"
		"out vec4 FragAverage;\r\n"
		"uniform sampler2D TexturePrev;\r\n"
		"uniform sampler2D TextureCurrent;\r\n"
		"void main()\r\n"
		"{\r\n"
		"	float lightPrev = texelFetch(TexturePrev, ivec2(0,0), 0).x; \r\n"
		"	float lightCurrent = exp(texelFetch(TextureCurrent, ivec2(0,0), 0).x); \r\n"
		"	float c = 0.04;\r\n"
		"	FragAverage = vec4(lightPrev + c * (lightCurrent - lightPrev), 0, 0, 0);\r\n"
		"}\r\n";

	program0 = compile_and_link(gc, vertex_program, fragment_program0);
	program1 = compile_and_link(gc, vertex_program, fragment_program1);
	program2 = compile_and_link(gc, vertex_program, fragment_program2);

	Vec4f positions[6] =
	{
		Vec4f(-1.0f, -1.0f, 1.0f, 1.0f),
		Vec4f( 1.0f, -1.0f, 1.0f, 1.0f),
		Vec4f(-1.0f,  1.0f, 1.0f, 1.0f),
		Vec4f( 1.0f, -1.0f, 1.0f, 1.0f),
		Vec4f(-1.0f,  1.0f, 1.0f, 1.0f),
		Vec4f( 1.0f,  1.0f, 1.0f, 1.0f)
	};
	vertices = VertexArrayVector<Vec4f>(gc, positions, 6);

	prim_array = PrimitivesArray(gc);
	prim_array.set_attributes(0, vertices);

	texture0 = Texture2D(gc, texture_size.width, texture_size.height, tf_r32f);
	texture0.set_min_filter(filter_nearest);
	texture0.set_mag_filter(filter_nearest);
	texture0.set_wrap_mode(wrap_clamp_to_edge, wrap_clamp_to_edge);

	texture1 = Texture2D(gc, texture_size.width, texture_size.height, tf_r32f);
	texture1.set_min_filter(filter_nearest);
	texture1.set_mag_filter(filter_nearest);
	texture1.set_wrap_mode(wrap_clamp_to_edge, wrap_clamp_to_edge);

	PixelBuffer black(1, 1, tf_r32f);
	black.get_data<float>()[0] = 0.0f;

	result_texture0 = Texture2D(gc, 1, 1, tf_r32f);
	result_texture0.set_min_filter(filter_nearest);
	result_texture0.set_mag_filter(filter_nearest);
	result_texture0.set_wrap_mode(wrap_clamp_to_edge, wrap_clamp_to_edge);
	result_texture0.set_subimage(gc, 0, 0, black, black.get_size());

	result_texture1 = Texture2D(gc, 1, 1, tf_r32f);
	result_texture1.set_min_filter(filter_nearest);
	result_texture1.set_mag_filter(filter_nearest);
	result_texture1.set_wrap_mode(wrap_clamp_to_edge, wrap_clamp_to_edge);
	result_texture1.set_subimage(gc, 0, 0, black, black.get_size());

	fb0 = FrameBuffer(gc);
	fb0.attach_color(0, texture0);

	fb1 = FrameBuffer(gc);
	fb1.attach_color(0, texture1);

	fb2 = FrameBuffer(gc);
	fb2.attach_color(0, result_texture0);

	fb3 = FrameBuffer(gc);
	fb3.attach_color(0, result_texture1);

	BlendStateDescription blend_desc;
	blend_desc.enable_blending(false);
	blend_state = BlendState(gc, blend_desc);
}

Texture2D &LogAverageLight::find_log_average_light(GraphicContext &gc, Texture2D &hdr_texture)
{
	hdr_texture.set_min_filter(filter_nearest);
	hdr_texture.set_mag_filter(filter_nearest);
	hdr_texture.set_wrap_mode(wrap_clamp_to_edge, wrap_clamp_to_edge);

	gc.set_primitives_array(prim_array);
	gc.set_program_object(program0);
	gc.set_blend_state(blend_state);

	Size texture_size(1 << iterations, 1 << iterations);

	for (int i = 0; i <= iterations; i++)
	{
		if (i == 1)
			gc.set_program_object(program1);

		int iteration_width = texture_size.width >> i;
		int iteration_height = texture_size.height >> i;

		gc.set_frame_buffer((i % 2 == 0) ? fb0 : fb1);
		gc.set_viewport(Size(iteration_width, iteration_height));
		if (i == 0)
			gc.set_texture(0, hdr_texture);
		else
			gc.set_texture(0, (i % 2 == 0) ? texture1 : texture0);
		gc.draw_primitives_array(type_triangles, 6);
	}

	current_index = (current_index + 1) % 2;

	gc.set_program_object(program2);
	gc.set_frame_buffer(current_index == 0 ? fb2 : fb3);
	gc.set_viewport(Rectf(0.0f, 0.0f, 1.0f, 1.0f));
	gc.set_texture(0, (iterations % 2 == 0) ? texture1 : texture0);
	gc.set_texture(1, current_index == 0 ? result_texture1 : result_texture0);
	gc.draw_primitives_array(type_triangles, 6);

	gc.reset_texture(0);
	gc.reset_program_object();
	gc.reset_primitives_array();
	gc.reset_frame_buffer();
	gc.set_viewport(viewport->get_size());

	return current_index == 0 ? result_texture0 : result_texture1;
}

ProgramObject LogAverageLight::compile_and_link(GraphicContext &gc, const std::string &vertex_source, const std::string &fragment_source)
{
	ShaderObject vertex_shader(gc, shadertype_vertex, vertex_source);
	ShaderObject fragment_shader(gc, shadertype_fragment, fragment_source);
	if (!vertex_shader.compile())
		throw Exception(string_format("LogAverageLight vertex program could not compile: %1", vertex_shader.get_info_log()));
	if (!fragment_shader.compile())
		throw Exception(string_format("LogAverageLight fragment program could not compile: %1", fragment_shader.get_info_log()));

	ProgramObject program(gc);
	program.attach(vertex_shader);
	program.attach(fragment_shader);
	program.bind_attribute_location(0, "PositionInProjection");
	program.bind_frag_data_location(0, "FragAverage");

	ShaderSetup::link(program, "log average light program");

	program.set_uniform1i("Texture", 0);
	program.set_uniform1i("TextureCurrent", 0);
	program.set_uniform1i("TexturePrev", 1);
	return program;
}

}

