/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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

//! clanCore="Math"
//! header=core.h

#ifndef header_delauney_triangulator
#define header_delauney_triangulator

#ifdef CL_API_DLL
#ifdef CL_CORE_EXPORT
#define CL_API_CORE __declspec(dllexport)
#else
#define CL_API_CORE __declspec(dllimport)
#endif
#else
#define CL_API_CORE
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include "../System/sharedptr.h"

//: Vertex in the delauney triangulation.
class CL_API_CORE CL_DelauneyTriangulator_Vertex
{
//! Attributes:
public:
	//: Data pointer given when adding the vertex.
	void *data;

	//: X position of vertex.
	double x;
	
	//: Y position of vertex.
	double y;
};

//: Triangle generated from a delauney triangulation.
class CL_DelauneyTriangulator_Triangle
{
//! Attributes:
public:
	//: First point in the triangle.
	CL_DelauneyTriangulator_Vertex *vertex_A;

	//: Second point in the triangle.
	CL_DelauneyTriangulator_Vertex *vertex_B;

	//: Third point in the triangle.
	CL_DelauneyTriangulator_Vertex *vertex_C;
};

class CL_DelauneyTriangulator_Generic;

//: Delauney triangulator.
//- <p>This class uses the <a href="http://astronomy.swin.edu.au/~pbourke/terrain/triangulate/">
//- delauney triangulation algorithm</a> to produce
//- triangles between a list of points.</p>
class CL_DelauneyTriangulator
{
//! Construction:
public:
	//: Creates a triangulator object.
	CL_DelauneyTriangulator();

	virtual ~CL_DelauneyTriangulator();

//! Attributes:
public:
	//: Returns the list of vertices in the triangulation.
	const std::vector<CL_DelauneyTriangulator_Vertex> &get_vertices() const;

	//: Returns the resulting triangles produced from triangulation.
	const std::vector<CL_DelauneyTriangulator_Triangle> &get_triangles() const;

//! Operations:
public:
	//: This function specifies a point to be used in the triangulation.
	void add_vertex(double x, double y, void *data);

	//: Converts passed points into triangles.
	void generate();

//! Implementation:
private:
	CL_SharedPtr<CL_DelauneyTriangulator_Generic> impl;
};

#endif
