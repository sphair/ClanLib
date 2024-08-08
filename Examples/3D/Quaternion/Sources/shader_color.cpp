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

#include "shader_color.h"

const char ShaderColor::vertex_hlsl[] =
	"\n"
	"cbuffer ProgramUniforms\n"
	"{\n"
	"	float4x4 cl_ModelViewMatrix;\n"
	"	float4x4 cl_ModelViewProjectionMatrix;\n"
	"	float3x3 cl_NormalMatrix;\n"
	"	float4 padding;\n"
	"	float4 MaterialEmission;\n"
	"	float4 MaterialSpecular;\n"
	"	float4 MaterialAmbient;\n"
	"	float4 LightSpecular;\n"
	"	float4 LightDiffuse;\n"
	"	float4 LightAmbient;\n"
	"	float3 LightVector;\n"
	"	float3 LightHalfVector;\n"
	"	float MaterialShininess;\n"
	"}\n"
	"struct VertexIn\n"
	"	{\n"
	"		float3 InPosition : InPosition;\n"
	"		float3 InNormal : InNormal;\n"
	"	};\n"
	"\n"
	"	struct VertexOut\n"
	"	{\n"
	"		float4 PositionInProjection : SV_Position;\n"
	"		float3 WorldSpaceNormal : WorldSpaceNormal; \n"
	"		float4 ObjPos : ObjPos;\n"
	"	};\n"
	"\n"
	"	VertexOut main(VertexIn input)\n"
	"	{\n"
	"		VertexOut output;\n"
	"		float4 in_position = float4(input.InPosition.xyz, 1.0);\n"
	"		output.PositionInProjection = mul(cl_ModelViewProjectionMatrix,in_position);\n"
	"		output.WorldSpaceNormal = normalize( mul(cl_NormalMatrix,input.InNormal));\n"
	"		output.ObjPos = mul(cl_ModelViewMatrix,in_position);\n"
	"		return output;\n"
	"	}\n";


const char ShaderColor::fragment_hlsl[] =
	"\n"
	"cbuffer ProgramUniforms\n"
	"{\n"
	"	float4x4 cl_ModelViewMatrix;\n"
	"	float4x4 cl_ModelViewProjectionMatrix;\n"
	"	float3x3 cl_NormalMatrix;\n"
	"	float4 padding;\n"
	"	float4 MaterialEmission;\n"
	"	float4 MaterialSpecular;\n"
	"	float4 MaterialAmbient;\n"
	"	float4 LightSpecular;\n"
	"	float4 LightDiffuse;\n"
	"	float4 LightAmbient;\n"
	"	float3 LightVector;\n"
	"	float3 LightHalfVector;\n"
	"	float MaterialShininess;\n"
	"}\n"
	"struct PixelIn\n"
	"{\n"
	"	float4 ScreenPos : SV_Position;\n"
	"	float3 WorldSpaceNormal : WorldSpaceNormal; \n"
	"	float4 ObjPos : ObjPos;\n"
	"};\n"
	"\n"
	"struct PixelOut\n"
	"{\n"
	"	float4 cl_FragColor : SV_Target0;\n"
	"};\n"
	"\n"
	"PixelOut main(PixelIn input)\n"
	"{\n"
	"	PixelOut output;\n"
	"	float3 eye = -normalize(input.ObjPos.xyz); \n"
	"	float4 diff = float4(0,0,0,0); \n"
	"	float4 spec = float4(0,0,0,0); \n"
	"\n"
	"	float3 world_space_normal = normalize(input.WorldSpaceNormal);\n"
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
	"	float4 final_texture_color = float4(MaterialAmbient.rgb,1.0);\n"
	"	output.cl_FragColor = LightAmbient * final_texture_color + (diff + MaterialEmission) * final_texture_color +spec * MaterialSpecular;\n"
	"	output.cl_FragColor.a = MaterialAmbient.a;\n"
	"	return output;\n"
	"}\n";

const char ShaderColor::vertex_glsl[] = R"(
#version 430 core

layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec3 InNormal;

layout (std140) uniform ProgramUniforms
{
    mat4 cl_ModelViewMatrix;
    mat4 cl_ModelViewProjectionMatrix;
    mat3 cl_NormalMatrix;
    vec4 padding;
    vec4 MaterialEmission;
    vec4 MaterialSpecular;
    vec4 MaterialAmbient;
    vec4 LightSpecular;
    vec4 LightDiffuse;
    vec4 LightAmbient;
    vec3 LightVector;
    vec3 LightHalfVector;
    float MaterialShininess;
};

out vec3 WorldSpaceNormal; 
out vec3 WorldSpacePosition; 
out vec4 ObjPos;

void main()
{
    vec4 in_position = vec4(InPosition, 1.0);
    gl_Position = cl_ModelViewProjectionMatrix * in_position;
    WorldSpaceNormal = normalize(cl_NormalMatrix * InNormal);
    WorldSpacePosition = InPosition;
    ObjPos = cl_ModelViewMatrix * in_position;
}
)";

const char ShaderColor::fragment_glsl[] = R"(
#version 430 core

in vec3 WorldSpaceNormal; 
in vec3 WorldSpacePosition; 
in vec4 ObjPos;

out vec4 cl_FragColor;

layout (std140) uniform ProgramUniforms
{
    mat4 cl_ModelViewMatrix;
    mat4 cl_ModelViewProjectionMatrix;
    mat3 cl_NormalMatrix;
    vec4 padding;
    vec4 MaterialEmission;
    vec4 MaterialSpecular;
    vec4 MaterialAmbient;
    vec4 LightSpecular;
    vec4 LightDiffuse;
    vec4 LightAmbient;
    vec3 LightVector;
    vec3 LightHalfVector;
    float MaterialShininess;
};

void main()
{
    vec4 diff = vec4(0); 
    vec4 spec = vec4(0); 

    vec3 world_space_normal = normalize(WorldSpaceNormal);
    float nDotL = max(0.0, dot(world_space_normal, LightVector)); 
    float pf; 

    if (nDotL == 0.0)
    {
        pf = 0.0; 
    }
    else
    {
        float nDotHV = max(0.0, dot(world_space_normal, LightHalfVector));
        pf = pow(nDotHV, MaterialShininess);
    }

    spec += LightSpecular * pf; 
    diff += LightDiffuse * nDotL;

    vec4 final_texture_color = vec4(MaterialAmbient.rgb, 1.0);
    cl_FragColor = LightAmbient * final_texture_color + (diff + MaterialEmission) * final_texture_color + spec * MaterialSpecular;
    cl_FragColor.a = MaterialAmbient.a;
}
)";

ShaderColor::ShaderColor(GraphicContext &gc)
{
	ShaderLanguage shader_language = gc.get_shader_language();
	
	ShaderObject vertex_shader(gc, ShaderType::vertex, shader_language==ShaderLanguage::glsl ? vertex_glsl : vertex_hlsl);
	if(!vertex_shader.compile())
	{
		std::string log = vertex_shader.get_info_log();
		throw Exception(string_format("Unable to compile vertex shader object: %1", log));
	}

	ShaderObject fragment_shader(gc, ShaderType::fragment, shader_language==ShaderLanguage::glsl ? fragment_glsl : fragment_hlsl);
	if(!fragment_shader.compile())
	{
		std::string log = fragment_shader.get_info_log();
		throw Exception(string_format("Unable to compile fragment shader object: %1", log));
	}

	program_object = ProgramObject(gc);
	program_object.attach(vertex_shader);
	program_object.attach(fragment_shader);
	program_object.bind_attribute_location(0, "InPosition");
	program_object.bind_attribute_location(1, "InNormal");
	program_object.bind_frag_data_location(0, "cl_FragColor");
	if (!program_object.link())
	{
		throw Exception(string_format("Unable to link program object: %1", program_object.get_info_log()));
	}

	// Uniforms
	program_object.set_uniform_buffer_index("ProgramUniforms", 0);

	gpu_uniforms = clan::UniformVector<ProgramUniforms>(gc, 1);

	uniforms.MaterialShininess = 64.0f;
	uniforms.MaterialEmission = Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	uniforms.MaterialAmbient =  Vec4f(1.0f, 0.0f, 0.0f, 1.0f);
	uniforms.MaterialSpecular = Vec4f(0.0f, 0.0f, 0.0f, 1.0f);

	uniforms.LightAmbient = Vec4f(0.2f, 0.2f, 0.2f, 1.0f);
	uniforms.LightVector = Vec3f(0.0f, 0.0f, 1.0f);
	uniforms.LightSpecular = Vec4f(0.7f, 0.7f, 0.7f, 1.0f);
	uniforms.LightDiffuse = Vec4f(0.7f, 0.7f, 0.7f, 1.0f);

}

void ShaderColor::Use(GraphicContext &gc, const Mat4f &matrix_modelview, const Mat4f &matrix_modelview_projection, const Mat4f &matrix_normal)
{
	uniforms.cl_ModelViewProjectionMatrix = matrix_modelview_projection;
	uniforms.cl_ModelViewMatrix = matrix_modelview;
	uniforms.cl_NormalMatrix = matrix_normal;
	gpu_uniforms.upload_data(gc, &uniforms, 1);
	gc.set_uniform_buffer(0, gpu_uniforms);
	gc.set_program_object(program_object);
}

void ShaderColor::SetMaterial(float new_material_shininess, const Vec4f &new_material_emission, const Vec4f &new_material_ambient, const Vec4f &new_material_specular)
{
	uniforms.MaterialShininess = new_material_shininess;
	uniforms.MaterialEmission = new_material_emission;
	uniforms.MaterialAmbient =  new_material_ambient;
	uniforms.MaterialSpecular = new_material_specular;
}

void ShaderColor::SetLight(Vec3f &new_light_vector, Vec4f &new_light_specular, Vec4f &new_light_diffuse, Vec4f &new_light_ambient)
{

	uniforms.LightAmbient = new_light_ambient;
	uniforms.LightVector = new_light_vector;
	uniforms.LightSpecular =new_light_specular;
	uniforms.LightDiffuse = new_light_diffuse;

	Vec3f light_halfvector(0.0f, 0.0f, 1.0f);
	light_halfvector += new_light_vector;
	light_halfvector.normalize();
	uniforms.LightHalfVector = light_halfvector;
}



