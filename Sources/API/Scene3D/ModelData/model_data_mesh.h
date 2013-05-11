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
*/

#pragma once

#include "model_data_material_range.h"

namespace clan
{

class ModelDataMesh
{
public:
	std::vector<Vec3f> vertices;
	std::vector<Vec3f> normals;
	std::vector<Vec3f> tangents;
	std::vector<Vec3f> bitangents;
	std::vector<Vec4ub> bone_weights;
	std::vector<Vec4ub> bone_selectors;
	std::vector<Vec4ub> colors;
	std::vector< std::vector<Vec2f> > channels;
	std::vector<unsigned int> elements;
	std::vector<ModelDataMaterialRange> material_ranges;

	void calculate_tangents();
};

inline void ModelDataMesh::calculate_tangents()
{
	if (!channels.empty())
	{
		tangents.resize(vertices.size());
		bitangents.resize(vertices.size());

		std::vector<Vec3f> tan1, tan2;
		tan1.resize(vertices.size());
		tan2.resize(vertices.size());
		for (size_t i = 0; i + 2 < elements.size(); i += 3)
		{
			unsigned int i1 = elements[i + 0];
			unsigned int i2 = elements[i + 1];
			unsigned int i3 = elements[i + 2];
        
			const Vec3f& v1 = vertices[i1];
			const Vec3f& v2 = vertices[i2];
			const Vec3f& v3 = vertices[i3];
        
			const Vec2f& w1 = channels[0][i1];
			const Vec2f& w2 = channels[0][i2];
			const Vec2f& w3 = channels[0][i3];
        
			float x1 = v2.x - v1.x;
			float x2 = v3.x - v1.x;
			float y1 = v2.y - v1.y;
			float y2 = v3.y - v1.y;
			float z1 = v2.z - v1.z;
			float z2 = v3.z - v1.z;
        
			float s1 = w2.x - w1.x;
			float s2 = w3.x - w1.x;
			float t1 = w2.y - w1.y;
			float t2 = w3.y - w1.y;
        
			float r = 1.0f / (s1 * t2 - s2 * t1);
			Vec3f sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);
			Vec3f tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r);
        
			tan1[i1] += sdir;
			tan1[i2] += sdir;
			tan1[i3] += sdir;
        
			tan2[i1] += tdir;
			tan2[i2] += tdir;
			tan2[i3] += tdir;
		}

		for (size_t i = 0; i < vertices.size(); i++)
		{
			const Vec3f &n = normals[i];
			const Vec3f &t = tan1[i];

			// Gram-Schmidt orthogonalize
			tangents[i] = (t - n * Vec3f::dot(n, t)).normalize();

			// Calculate handedness
			float handedness = (Vec3f::dot(Vec3f::cross(n, t), tan2[i]) < 0.0f) ? -1.0f : 1.0f;

			bitangents[i] = handedness * Vec3f::cross(n, t);
		}
	}
}

}

