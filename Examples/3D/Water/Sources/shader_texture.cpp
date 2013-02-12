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

#include "shader_texture.h"

float ShaderTexture::main_time = 0.0f;

char ShaderTexture::vertex[] =
	"\n"
	"#version 120\n"
	"\n"
	"attribute vec3 InPosition;"
	"attribute vec3 InNormal;"
	"attribute vec2 InTexCoord0;\n"
	"uniform mat4 cl_ModelViewMatrix;"
	"uniform mat4 cl_ModelViewProjectionMatrix;"
	"uniform mat3 cl_NormalMatrix;"
	"\n"
	"varying vec3 WorldSpaceNormal; \n"
	"varying vec3 WorldSpacePosition; \n"
	"varying vec4 ObjPos;\n"
	"varying vec4 MaterialAmbient;\n"
	"varying vec2 TexCoord0;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	vec4 in_position = vec4(InPosition.xyz, 1.0);\n"
	"	gl_Position = cl_ModelViewProjectionMatrix * in_position;\n"
	"	WorldSpaceNormal = normalize( cl_NormalMatrix * InNormal);\n"
	"	WorldSpacePosition = InPosition;\n"
	"	ObjPos = cl_ModelViewMatrix * in_position;\n"
	"	TexCoord0 = InTexCoord0;\n"
	"}\n"
	;

char ShaderTexture::fragment[] =
	"\n"
	"#version 120\n"
	"\n"
	"varying vec3 WorldSpaceNormal; \n"
	"varying vec3 WorldSpacePosition; \n"
	"varying vec4 ObjPos;\n"
	"varying vec2 TexCoord0;\n"
	"\n"
	"uniform sampler2D Texture0;\n"
	"uniform sampler2D Texture1;\n"
	"\n"
	"uniform float MaterialShininess;\n"
	"uniform vec4 MaterialEmission;\n"
	"uniform vec4 MaterialSpecular;\n"
	"uniform vec4 MaterialAmbient;\n"
	"\n"
	"uniform vec3 LightVector;\n"
	"uniform vec3 LightHalfVector;\n"
	"uniform vec4 LightSpecular;\n"
	"uniform vec4 LightDiffuse;\n"
	"uniform vec4 LightAmbient;\n"
	"uniform float Time;\n"
	"\n"
	"void main()\n"
	"{\n"
	"   vec2 coord = TexCoord0.xy;\n"

	"   float amplitude = 0.008;\n"

	"   float speed = 6.0;\n"
	"   float speed2 = 7.0;\n"
	"   float speed3 = 8.0;\n"
	"   float speed4 = 9.0;\n"
	"   vec2 direction = vec2(1.0, 0.0);\n"
	"   vec2 direction2 = vec2(0.0, -1.0);\n"
	"   vec2 direction3 = vec2(0.3, -0.7);\n"
	"   vec2 direction4 = vec2(0.6, -0.4);\n"
	"	float wavelength = 20.0;\n"
	"	float wavelength2 = 19.0;\n"
	"	float wavelength3 = 18.0;\n"
	"	float wavelength4 = 17.0;\n"

	"	float frequency = 2*3.14159/wavelength;\n"
	"	float phase = speed * frequency;\n"
	"	float height = sin(dot(ObjPos.xz, direction) * frequency + Time * phase);\n"

	"	frequency = 2*3.14159/wavelength2;\n"
	"	phase = speed2 * frequency;\n"
	"	height += sin( dot(ObjPos.xz, direction2) * frequency + Time * phase);\n"

	"	frequency = 2*3.14159/wavelength3;\n"
	"	phase = speed3 * frequency;\n"
	"	height += sin(dot(ObjPos.xz, direction3) * frequency + Time * phase);\n"

	"	frequency = 2*3.14159/wavelength4;\n"
	"	phase = speed4 * frequency;\n"
	"	height += sin(dot(ObjPos.xz, direction4) * frequency + Time * phase);\n"
	"	height /= 4.0;\n"

	"	vec3 water_normal = vec3( -height, -height, 1.0);\n"

	"	height *= amplitude;\n"

	"	vec2 coord2 = coord;"
	"   coord.x -= height;\n"
	"   coord.y -= height;\n"

	"   coord2.x += height;\n"
	"   coord2.y += height;\n"

	// Colour the underwater texture
	"	vec4 texture_color = texture2D(Texture0, coord);\n"
	"	texture_color.r = texture_color.r / 2.0 + 0.4;\n"
	"	texture_color.g = texture_color.g / 2.0 + 0.4;\n"
	"	texture_color.b = texture_color.b / 2.0 + 0.8;\n"

	// Add on the reflected texture
	"	vec4 background_color = texture2D(Texture1, coord2);\n"
	"	texture_color.r = texture_color.r + background_color.r / 2.0;\n"
	"	texture_color.g = texture_color.g + background_color.g / 2.0;\n"
	"	texture_color.b = texture_color.b + background_color.b / 2.0;\n"

	"\n"
	"   if (texture_color.a < 0.01) discard;\n"
	"	vec3 eye = -normalize(ObjPos.xyz); \n"
	"	vec4 diff = vec4(0); \n"
	"	vec4 spec = vec4(0); \n"
	"\n"
	"	vec3 world_space_normal = normalize(WorldSpaceNormal + water_normal);\n"
	"	float nDotL = max(0.0, dot(world_space_normal, LightVector)); \n"
	"	float pf; \n"
	"	if (nDotL == 0.0)\n"
	"	{\n"
	"		pf = 0.0; \n"
	"	}else\n"
	"	{\n"
	"			float nDotHV = max(0.0, dot(world_space_normal, LightHalfVector));\n"
	"			pf = pow(nDotHV, MaterialShininess);\n"
	"	}\n"
	"	spec += LightSpecular * pf; \n"
	"	diff += LightDiffuse * nDotL;\n"
	"	vec4 final_texture_color = vec4(texture_color.rgb,1.0);\n"
	"	gl_FragColor = LightAmbient * final_texture_color + (diff + MaterialEmission) * final_texture_color +spec * MaterialSpecular;\n"
	"	gl_FragColor.a = MaterialAmbient.a;\n"
	"}\n"
	;

ShaderTexture::ShaderTexture(GraphicContext &gc)
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
	program_object.bind_attribute_location(2, "InTexCoord0");
	if (!program_object.link())
	{
		throw Exception(string_format("Unable to link program object: %1", program_object.get_info_log()));
	}


	material_updated = false;
	light_updated = false;

	material_shininess = 64.0f;
	material_emission = Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	material_ambient =  Vec4f(1.0f, 0.0f, 0.0f, 1.0f);
	material_specular = Vec4f(0.0f, 0.0f, 0.0f, 1.0f);

	light_ambient = Vec4f(0.2f, 0.2f, 0.2f, 1.0f);
	light_vector = Vec3f(0.0f, 0.0f, 1.0f);
	light_specular = Vec4f(0.7f, 0.7f, 0.7f, 1.0f);
	light_diffuse = Vec4f(0.7f, 0.7f, 0.7f, 1.0f);

	program_object.set_uniform1i("Texture0", 0);
	program_object.set_uniform1i("Texture1", 1);

}

void ShaderTexture::Use(GraphicContext &gc)
{
	program_object.set_uniform1f("Time", main_time);

	if (!material_updated)
	{
		material_updated = true;
		program_object.set_uniform1f("MaterialShininess", material_shininess);
		program_object.set_uniform4f("MaterialEmission", material_emission);
		program_object.set_uniform4f("MaterialSpecular", material_specular);
		program_object.set_uniform4f("MaterialAmbient", material_ambient);
	}

	if (!light_updated)
	{
		light_updated = true;
		program_object.set_uniform3f("LightVector", light_vector);
		Vec3f light_halfvector(0.0f, 0.0f, 1.0f);
		light_halfvector += light_vector;
		light_halfvector.normalize();
		program_object.set_uniform3f("LightHalfVector", light_halfvector);
		program_object.set_uniform4f("LightSpecular", light_specular);
		program_object.set_uniform4f("LightDiffuse", light_diffuse);
		program_object.set_uniform4f("LightAmbient", light_ambient);
	}

	gc.set_program_object(program_object);
}

void ShaderTexture::SetMaterial(float new_material_shininess, const Vec4f &new_material_emission, const Vec4f &new_material_ambient, const Vec4f &new_material_specular)
{
	if (new_material_shininess != material_shininess)
	{
		material_updated = false;
		material_shininess = new_material_shininess;
	}

	if (new_material_emission != material_emission)
	{
		material_updated = false;
		material_emission = new_material_emission;
	}

	if (new_material_ambient != material_ambient)
	{
		material_updated = false;
		material_ambient = new_material_ambient;
	}

	if (new_material_specular != material_specular)
	{
		material_updated = false;
		material_specular = new_material_specular;
	}
}

void ShaderTexture::SetLight(Vec3f &new_light_vector, Vec4f &new_light_specular, Vec4f &new_light_diffuse, Vec4f &new_light_ambient)
{
	if (new_light_vector != light_vector)
	{
		light_updated = false;
		light_vector = new_light_vector;
	}
	if (new_light_specular != light_specular)
	{
		light_updated = false;
		light_specular = new_light_specular;
	}
	if (new_light_diffuse != light_diffuse)
	{
		light_updated = false;
		light_diffuse = new_light_diffuse;
	}
	if (new_light_ambient != light_ambient)
	{
		light_updated = false;
		light_ambient = new_light_ambient;
	}
}

