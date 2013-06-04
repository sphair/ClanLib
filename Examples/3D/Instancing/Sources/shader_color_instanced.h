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
**    Mark Page
*/

#pragma once

class ShaderColorInstanced
{
public:
	ShaderColorInstanced(GraphicContext &gc);

	void Use(GraphicContext &gc, const Mat4f &matrix_modelview, const Mat4f &matrix_modelview_projection, const Mat4f &matrix_normal);
	void SetMaterial(float new_material_shininess, const Vec4f &new_material_emission);
	void SetLight(Vec3f &new_light_vector, Vec4f &new_light_specular, Vec4f &new_light_diffuse, Vec4f &new_light_ambient);

	void SetCenters(std::vector<Vec3f> &centers);
	void SetMaterialAmbientSpecular(std::vector<Vec4f> &colors);

private:

	static const int max_instances = 256;
	struct ProgramUniforms
	{
		Vec4f MaterialAmbientSpecular[max_instances];
		Vec4f Centers[max_instances];
		Mat4f cl_ModelViewMatrix;
		Mat4f cl_ModelViewProjectionMatrix;
		Mat4f cl_NormalMatrix;

		Vec4f MaterialEmission;
		Vec4f LightSpecular;
		Vec4f LightDiffuse;
		Vec4f LightAmbient;
		Vec3f LightVector;
		float padding;
		Vec3f LightHalfVector;
		float MaterialShininess;
	};

	clan::UniformVector<ProgramUniforms> gpu_uniforms;
	ProgramUniforms uniforms;

	static const char vertex_hlsl[];
	static const char fragment_hlsl[];
	static const char vertex_glsl[];
	static const char fragment_glsl[];
	ProgramObject program_object;
};
