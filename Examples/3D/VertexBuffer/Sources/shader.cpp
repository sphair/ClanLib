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

#include "shader.h"

char Shader::vertex[] =
	"\n"
	"#version 120\n"
	"\n"
	"attribute vec4 InPosition;"
	"attribute vec3 InNormal;"
	"attribute vec4 InMaterialAmbient;"
	"uniform mat4 cl_ModelViewMatrix;"
	"uniform mat4 cl_ModelViewProjectionMatrix;"
	"uniform mat3 cl_NormalMatrix;"
	"varying vec3 WorldSpaceNormal; \n"
	"varying vec3 WorldSpacePosition; \n"
	"varying vec3 ObjPos;\n"
	"varying vec4 MaterialAmbient;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	gl_Position = cl_ModelViewProjectionMatrix * InPosition;\n"
	"	WorldSpaceNormal = normalize( cl_NormalMatrix * InNormal );\n"
	"	WorldSpacePosition = InPosition.xyz;\n"
	"   MaterialAmbient = InMaterialAmbient;\n"
	"	ObjPos = vec3(cl_ModelViewMatrix * InPosition);\n"
	"}\n"
	;

char Shader::fragment[] =
	"\n"
	"#version 120\n"
	"\n"
	"varying vec3 WorldSpaceNormal; \n"
	"varying vec3 WorldSpacePosition; \n"
	"varying vec3 ObjPos;\n"
	"varying vec4 MaterialAmbient;\n"
	"\n"
	"uniform float MaterialShininess;\n"
	"uniform vec4 MaterialEmission;\n"
	"uniform vec4 MaterialSpecular;\n"
	"\n"
	"uniform vec4 LightPosition;\n"
	"uniform vec4 LightHalfVector;\n"
	"uniform vec4 LightSpecular;\n"
	"uniform vec4 LightDiffuse;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	vec3 texture_color = MaterialAmbient.rgb;\n"
	"\n"
	"	vec3 eye = -normalize(ObjPos); \n"
	"	vec4 diff = vec4(0); \n"
	"	vec4 spec = vec4(0); \n"
	"\n"
	"	float nDotL = max(0.0, dot(WorldSpaceNormal, LightPosition.xyz)); \n"
	"	float pf; \n"
	"	if (nDotL == 0.0)\n"
	"	{\n"
	"		pf = 0.0; \n"
	"	}else\n"
	"	{\n"
	"			float nDotHV = max(0.0, dot(WorldSpaceNormal, LightHalfVector.xyz));\n"
	"			pf = pow(nDotHV, MaterialShininess);\n"
	"	}\n"
	"	\n"
	"	spec += LightSpecular * pf; \n"
	"	diff += LightDiffuse * nDotL;\n"
	"\n"
	"	vec4 final_texture_color = vec4(texture_color,1.0);\n"
	"\n"
	"	const vec4 ambient_light = vec4(0.05, 0.05, 0.05, 1.0);\n"
	"	gl_FragColor = ambient_light * final_texture_color + (diff + MaterialEmission) * final_texture_color +spec * MaterialSpecular;\n"
	"\n"
	"	gl_FragColor.a = MaterialAmbient.a;\n"

	"}\n"
	;

Shader::Shader(GraphicContext &gc)
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
	program_object.bind_attribute_location(1, "InNormal");
	program_object.bind_attribute_location(2, "InMaterialAmbient");
	if (!program_object.link())
	{
		throw Exception(string_format("Unable to link program object: %1", program_object.get_info_log()));
	}

	material_shininess = 64.0f;
	material_emission = Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	//material_ambient =  Vec4f(0.9f, 0.2f, 0.2f, 1.0f);
	material_specular = Vec4f(0.0f, 0.0f, 0.0f, 1.0f);

	light_position = Vec4f(0.0f, 0.0f, 1.0f, 0.0f);
	light_specular = Vec4f(0.7f, 0.7f, 0.7f, 1.0f);
	light_diffuse = Vec4f(0.7f, 0.7f, 0.7f, 1.0f);


}

void Shader::Set(GraphicContext &gc)
{
		program_object.set_uniform1f("MaterialShininess", material_shininess);
		program_object.set_uniform4f("MaterialEmission", material_emission);
		program_object.set_uniform4f("MaterialSpecular", material_specular);
		//program_object.set_uniform4f("MaterialAmbient", material_ambient);
		program_object.set_uniform4f("LightPosition", light_position);
		Vec4f light_halfvector(0.0f, 0.0f, 1.0f, 0.0f);
		light_halfvector += light_position;
		light_halfvector.normalize3();
		program_object.set_uniform4f("LightHalfVector", light_halfvector);
		program_object.set_uniform4f("LightSpecular", light_specular);
		program_object.set_uniform4f("LightDiffuse", light_diffuse);
}

void Shader::Use(GraphicContext &gc)
{
	gc.set_program_object(program_object);
}

