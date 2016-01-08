/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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

#include <vector>

namespace clan
{
	class DelauneyTriangulator;
	class DelauneyTriangulator_Triangle;

	struct OutlineTriangulator_Vertex
	{
		void *data;
		float x, y;
		int num_triangles;
		int extra;
		DelauneyTriangulator_Triangle const **triangles;
	};

	struct OutlineTriangulator_Contour
	{
		std::vector<OutlineTriangulator_Vertex> vertices;
	};

	struct OutlineTriangulator_Polygon
	{
		std::vector<OutlineTriangulator_Contour> contours;
	};

	struct OutlineTriangulator_Collision
	{
		std::vector<OutlineTriangulator_Vertex *> first;
		std::vector<OutlineTriangulator_Vertex *> second;
		std::vector<DelauneyTriangulator_Triangle const *> triangles;
	};

	class OutlineTriangulator_Impl
	{
	public:
		OutlineTriangulator_Impl();
		~OutlineTriangulator_Impl();

		OutlineTriangulator_Polygon current_polygon;
		OutlineTriangulator_Contour current_contour;
		std::vector<OutlineTriangulator_Polygon> polygons;
		//std::vector<OutlineTriangulator_Triangle> triangles;

		void insert_points();
		void triangulate();

		void create_ordered_vertex_list(
			std::vector<OutlineTriangulator_Vertex *> &vertices);

		OutlineTriangulator_Collision find_colliding_triangles(
			OutlineTriangulator_Vertex *v1,
			OutlineTriangulator_Vertex *v2);

		void remove_triangle(DelauneyTriangulator_Triangle const *t);

		DelauneyTriangulator_Triangle const **add_triangles(
			DelauneyTriangulator &d1,
			DelauneyTriangulator &d2);

		bool intersects(
			float Ax,
			float Ay,
			float Bx,
			float By,
			float Cx,
			float Cy,
			float Dx,
			float Dy);
	};
}
