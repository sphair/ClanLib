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

#pragma once

class ShaderTexture
{
public:
	ShaderTexture(GraphicContext &gc);

	void Use(GraphicContext &gc);

	void SetMaterial(float new_material_shininess, const Vec4f &new_material_emission, const Vec4f &new_material_ambient, const Vec4f &new_material_specular);
	void SetLight(Vec3f &new_light_vector, Vec4f &new_light_specular, Vec4f &new_light_diffuse, Vec4f &new_light_ambient);
	static float main_time;
private:

	bool material_updated;
	float material_shininess;
	Vec4f material_emission;
	Vec4f material_ambient;
	Vec4f material_specular;

	bool light_updated;
	Vec3f light_vector;
	Vec4f light_specular;
	Vec4f light_diffuse;
	Vec4f light_ambient;

	static char vertex[];
	static char fragment[];
	ProgramObject program_object;
};
