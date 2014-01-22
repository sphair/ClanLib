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

#include "model_data_draw_range.h"

namespace clan
{

/// \brief Vertex attributes and draw ranges required to draw a mesh
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
	std::vector<ModelDataDrawRange> draw_ranges;

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
			unsigned int i0 = elements[i + 0];
			unsigned int i1 = elements[i + 1];
			unsigned int i2 = elements[i + 2];

			const Vec3f& v0 = vertices[i0];
			const Vec3f& v1 = vertices[i1];
			const Vec3f& v2 = vertices[i2];

			const Vec2f& w0 = channels[0][i0];
			const Vec2f& w1 = channels[0][i1];
			const Vec2f& w2 = channels[0][i2];

			const Vec3f edge1 = v1 - v0;
			const Vec3f edge2 = v2 - v0;

			const Vec2f delta1 = w1 - w0;
			const Vec2f delta2 = w2 - w0;

			float f = 1.0f / (delta1.x * delta2.y - delta2.x * delta1.y);

			Vec3f tangent = f * (delta2.y * edge1 - delta1.y * edge2);
			Vec3f bitangent = f * (-delta2.x * edge1 - delta1.x * edge2);

			tangent.normalize();
			bitangent.normalize();

			tangents[i0] = tangent;
			tangents[i1] = tangent;
			tangents[i2] = tangent;

			bitangents[i0] = bitangent;
			bitangents[i1] = bitangent;
			bitangents[i2] = bitangent;

			/*
			tan1[i0] += tangent;
			tan1[i1] += tangent;
			tan1[i2] += tangent;

			tan2[i0] += bitangent;
			tan2[i1] += bitangent;
			tan2[i2] += bitangent;
			*/
		}

		/*
		for (size_t i = 0; i < vertices.size(); i++)
		{
			const Vec3f &n = normals[i];
			const Vec3f &t = tan1[i];

			// Gram-Schmidt orthogonalize
			tangents[i] = (t - n * Vec3f::dot(n, t)).normalize();

			// Calculate handedness
			float handedness = (Vec3f::dot(Vec3f::cross(n, t), tan2[i]) < 0.0f) ? -1.0f : 1.0f;

			bitangents[i] = handedness * Vec3f::cross(n, tangents[i]);
		}
		*/

		// Note: the remarked code only works within the same smoothing group, which can't be determined here.
	}
}

}

