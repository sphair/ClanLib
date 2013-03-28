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

class Icosahedron
{
public:
	Icosahedron(GraphicContext &gc, bool use_radius_of_inscribed_sphere = false);

	VertexArrayVector<Vec3f> vertices;
	ElementArrayVector<unsigned int> elements;

	static const int num_vertices = 12;
	static const int num_elements = 20 * 3;
};

inline Icosahedron::Icosahedron(GraphicContext &gc, bool use_radius_of_inscribed_sphere)
{
	// radius of a circumscribed sphere (one that touches the icosahedron at all vertices)
	float x = 0.525731112119133606f;
	float z = 0.850650808352039932f;

	if (use_radius_of_inscribed_sphere) // tangent to each of the icosahedron's faces
	{
		x *= 1.32316908f;
		z *= 1.32316908f;
	}

	Vec3f local_vertices[num_vertices] =
	{
		Vec3f(-x, 0.0f, z),
		Vec3f( x, 0.0f, z),
		Vec3f(-x, 0.0f, -z),
		Vec3f( x, 0.0f, -z),
		Vec3f( 0.0f, z, x),
		Vec3f( 0.0f, z, -x),
		Vec3f( 0.0f, -z, x),
		Vec3f( 0.0f, -z, -x),
		Vec3f( z, x, 0.0f),
		Vec3f(-z, x, 0.0f),
		Vec3f( z, -x, 0.0f),
		Vec3f(-z, -x, 0.0f)
	};

	unsigned int local_elements[num_elements] =
	{
		0,4,1,
		0,9,4,
		9,5,4,
		4,5,8,
		4,8,1,
		8,10,1,
		8,3,10,
		5,3,8,
		5,2,3,
		2,7,3,
		7,10,3,
		7,6,10,
		7,11,6,
		11,0,6,
		0,1,6,
		6,1,10,
		9,0,11,
		9,11,2,
		9,2,5,
		7,2,11
	};

	vertices = VertexArrayVector<Vec3f>(gc, local_vertices, num_vertices);
	elements = ElementArrayVector<unsigned int>(gc, local_elements, num_elements);
}

}

