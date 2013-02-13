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

#pragma once
namespace clan
{

class ModelMaterialUniforms
{
public:
	ModelMaterialUniforms() : material_glossiness(), material_specular_level(), model_index(), vectors_per_instance(), material_offset() { }
	Vec4f material_ambient;
	Vec4f material_diffuse;
	Vec4f material_specular;
	float material_glossiness;
	float material_specular_level;
	unsigned int model_index;
	unsigned int vectors_per_instance;
	unsigned int material_offset;
	unsigned int padding0;
	unsigned int padding1;
	unsigned int padding2;
};

class ModelMeshBuffers
{
public:
	VertexArrayVector<Vec3f> vertices;
	VertexArrayVector<Vec3f> normals;
	VertexArrayVector<Vec3f> bitangents;
	VertexArrayVector<Vec3f> tangents;
	VertexArrayVector<Vec4ub> bone_weights;
	VertexArrayVector<Vec4ub> bone_selectors;
	VertexArrayVector<Vec4ub> colors;
	ElementArrayVector<unsigned int> elements;
	std::vector<VertexArrayBuffer> channels;
	std::vector<UniformVector<ModelMaterialUniforms> > uniforms;
	PrimitivesArray primitives_array;
};

}

