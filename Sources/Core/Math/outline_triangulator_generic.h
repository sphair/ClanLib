/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

#pragma once

#include <vector>

class CL_DelauneyTriangulator;
class CL_DelauneyTriangulator_Triangle;

struct CL_OutlineTriangulator_Vertex
{
	void *data;
	float x, y;
	int num_triangles;
	int extra;
	CL_DelauneyTriangulator_Triangle const **triangles;
};

struct CL_OutlineTriangulator_Contour
{
	std::vector<CL_OutlineTriangulator_Vertex> vertices;
};

struct CL_OutlineTriangulator_Polygon
{
	std::vector<CL_OutlineTriangulator_Contour> contours;
};

struct CL_OutlineTriangulator_Collision
{
	std::vector<CL_OutlineTriangulator_Vertex *> first;
	std::vector<CL_OutlineTriangulator_Vertex *> second;
	std::vector<CL_DelauneyTriangulator_Triangle const *> triangles;
};

class CL_OutlineTriangulator_Generic
{
/// \name Construction
/// \{

public:
	CL_OutlineTriangulator_Generic();

	~CL_OutlineTriangulator_Generic();


/// \}
/// \name Attributes
/// \{

public:
	CL_OutlineTriangulator_Polygon current_polygon;

	CL_OutlineTriangulator_Contour current_contour;

	std::vector<CL_OutlineTriangulator_Polygon> polygons;

//	std::vector<CL_OutlineTriangulator_Triangle> triangles;


/// \}
/// \name Operations
/// \{

public:
	void insert_points();

	void triangulate();

	void create_ordered_vertex_list(
		std::vector<CL_OutlineTriangulator_Vertex *> &vertices);

	CL_OutlineTriangulator_Collision find_colliding_triangles(
		CL_OutlineTriangulator_Vertex *v1,
		CL_OutlineTriangulator_Vertex *v2);

	void remove_triangle(CL_DelauneyTriangulator_Triangle const *t);

	CL_DelauneyTriangulator_Triangle const **add_triangles(
		CL_DelauneyTriangulator &d1,
		CL_DelauneyTriangulator &d2);

	bool intersects(
		float Ax,
		float Ay,
		float Bx,
		float By,
		float Cx,
		float Cy,
		float Dx,
		float Dy);
/// \}
};


