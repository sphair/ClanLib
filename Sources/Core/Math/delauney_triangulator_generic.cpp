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
*/

#include "Core/precomp.h"
#include "delauney_triangulator_generic.h"
#include <algorithm>

/////////////////////////////////////////////////////////////////////////////
// CL_DelauneyTriangulator_Generic construction:

CL_DelauneyTriangulator_Generic::CL_DelauneyTriangulator_Generic()
{
}

CL_DelauneyTriangulator_Generic::~CL_DelauneyTriangulator_Generic()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_DelauneyTriangulator_Generic attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_DelauneyTriangulator_Generic operations:

void CL_DelauneyTriangulator_Generic::triangulate()
{
	// Order vertices:

	std::vector<CL_DelauneyTriangulator_Vertex *> vertices;
	create_ordered_vertex_list(vertices);

	// Calculate super triangle:

	CL_DelauneyTriangulator_Vertex super_A;
	CL_DelauneyTriangulator_Vertex super_B;
	CL_DelauneyTriangulator_Vertex super_C;
	CL_DelauneyTriangulator_Triangle super_triangle;
	super_triangle.vertex_A = &super_A;
	super_triangle.vertex_B = &super_B;
	super_triangle.vertex_C = &super_C;
	calculate_supertriangle(vertices, super_triangle);

	// Perform delauney triangulation:

	perform_delauney_triangulation(vertices, super_triangle, triangles);
}

struct CL_CompareVertices
{
	bool operator()(CL_DelauneyTriangulator_Vertex *a, CL_DelauneyTriangulator_Vertex *b) const
	{
		if (a->x == b->x) return a->y < b->y;
		return a->x < b->x;
	}
};

void CL_DelauneyTriangulator_Generic::create_ordered_vertex_list(std::vector<CL_DelauneyTriangulator_Vertex *> &vertices)
{
	std::vector<CL_DelauneyTriangulator_Vertex>::size_type index_vertices, num_vertices;
	num_vertices = input_vertices.size();
	for (index_vertices = 0; index_vertices < num_vertices; index_vertices++)
	{
		vertices.push_back(&input_vertices[index_vertices]);
	}

	// Sort list:
	std::sort(vertices.begin(), vertices.end(), CL_CompareVertices());

	// Remove duplicates:
	std::vector<CL_DelauneyTriangulator_Vertex *>::iterator it = vertices.begin();
	if (it == vertices.end()) return;
	float last_x = (*it)->x;
	float last_y = (*it)->y;
	++it;
	while (it != vertices.end())
	{
		if (last_x == (*it)->x && last_y == (*it)->y)
		{
			it = vertices.erase(it);
		}
		else
		{
			last_x = (*it)->x;
			last_y = (*it)->y;
			++it;
		}
	}
}

void CL_DelauneyTriangulator_Generic::calculate_supertriangle(std::vector<CL_DelauneyTriangulator_Vertex *> &vertices, CL_DelauneyTriangulator_Triangle &super_triangle)
{
	// Find min and max values:

	float min_x = 0.0;
	float max_x = 0.0;
	float min_y = 0.0;
	float max_y = 0.0;
	bool first_point = true;

	std::vector<CL_DelauneyTriangulator_Vertex *>::size_type index_vertices, num_vertices;
	num_vertices = vertices.size();
	for (index_vertices = 0; index_vertices < num_vertices; index_vertices++)
	{
		CL_DelauneyTriangulator_Vertex *cur_vertex = vertices[index_vertices];

		if (!first_point)
		{
			if (min_x > cur_vertex->x) min_x = cur_vertex->x;
			if (max_x < cur_vertex->x) max_x = cur_vertex->x;
			if (min_y > cur_vertex->y) min_y = cur_vertex->y;
			if (max_y < cur_vertex->y) max_y = cur_vertex->y;
		}
		else
		{
			first_point = false;
			min_x = cur_vertex->x;
			max_x = cur_vertex->x;
			min_y = cur_vertex->y;
			max_y = cur_vertex->y;
		}
	}

	// Setup super triangle based on min/max values:

	super_triangle.vertex_A->x = min_x-1;
	super_triangle.vertex_A->y = min_y-1;
	super_triangle.vertex_A->data = 0;

	super_triangle.vertex_B->x = max_x+(max_x-min_x)+1;
	super_triangle.vertex_B->y = min_y-1;
	super_triangle.vertex_B->data = 0;

	super_triangle.vertex_C->x = min_x-1;
	super_triangle.vertex_C->y = max_y+(max_y-min_y)+1;
	super_triangle.vertex_C->data = 0;
}

void CL_DelauneyTriangulator_Generic::perform_delauney_triangulation(
	const std::vector<CL_DelauneyTriangulator_Vertex *> &vertices,
	const CL_DelauneyTriangulator_Triangle &super_triangle,
	std::vector<CL_DelauneyTriangulator_Triangle> &triangles)
{
/*
	delauney triangulation algorithm:

	subroutine triangulate
	input : vertex list
	output : triangle list
		initialize the triangle list
		determine the supertriangle
		add supertriangle vertices to the end of the vertex list
		add the supertriangle to the triangle list
		for each sample point in the vertex list
			initialize the edge buffer
			for each triangle currently in the triangle list
				calculate the triangle circumcircle center and radius
				if the point lies in the triangle circumcircle then
					add the three triangle edges to the edge buffer
					remove the triangle from the triangle list
				endif
			endfor
			delete all doubly specified edges from the edge buffer
				this leaves the edges of the enclosing polygon only
			add to the triangle list all triangles formed between the point 
				and the edges of the enclosing polygon
		endfor
		remove any triangles from the triangle list that use the supertriangle vertices
		remove the supertriangle vertices from the vertex list
	end

	See http://astronomy.swin.edu.au/~pbourke/terrain/triangulate/ for more info
*/

	// Reset triangle list.
	triangles.clear();

	// add supertriangle vertices to the end of the vertex list
	triangles.push_back(super_triangle);

	// for each sample point in the vertex list:
	std::vector<CL_DelauneyTriangulator_Vertex *>::size_type index_vertices, num_vertices;
	num_vertices = vertices.size();
	for (index_vertices = 0; index_vertices < num_vertices; index_vertices++)
	{
		CL_DelauneyTriangulator_Vertex *insertion_point = vertices[index_vertices];

		// Init edge buffer
		typedef std::pair<CL_DelauneyTriangulator_Vertex *, CL_DelauneyTriangulator_Vertex *> CL_DelauneyTriangulator_Triangle_Edge;
		std::vector<CL_DelauneyTriangulator_Triangle_Edge> edges;

		// For each triangle currently in the triangle list
		std::vector<CL_DelauneyTriangulator_Triangle>::size_type index_triangles, num_triangles;
		num_triangles = triangles.size();
		for (index_triangles = 0; index_triangles < num_triangles; index_triangles++)
		{
			CL_DelauneyTriangulator_Triangle &cur_triangle = triangles[index_triangles];

			// todo: cache circumcenter in triangle?
			CL_DelauneyTriangulator_Vertex circumcenter = find_circumcenter(cur_triangle);
			float radius_x = cur_triangle.vertex_A->x-circumcenter.x;
			float radius_y = cur_triangle.vertex_A->y-circumcenter.y;
			float radius2 = radius_x*radius_x + radius_y*radius_y;

			// Check if the point lies in the triangle circumcircle:
			float dist_x = insertion_point->x - circumcenter.x;
			float dist_y = insertion_point->y - circumcenter.y;
			float dist2 = dist_x*dist_x + dist_y*dist_y;
			if (dist2 < radius2)
			{
				// Add triangle edges to edge buffer:
				edges.push_back(CL_DelauneyTriangulator_Triangle_Edge(cur_triangle.vertex_A, cur_triangle.vertex_B));
				edges.push_back(CL_DelauneyTriangulator_Triangle_Edge(cur_triangle.vertex_B, cur_triangle.vertex_C));
				edges.push_back(CL_DelauneyTriangulator_Triangle_Edge(cur_triangle.vertex_C, cur_triangle.vertex_A));

				// Remove triange from triangle list:
				triangles.erase(triangles.begin()+index_triangles);
				index_triangles--;
				num_triangles--;
			}
		}

		// Delete all doubly specified edges from the edge buffer
		// this leaves the edges of the enclosing polygon only
		int index_edges1, index_edges2, num_edges; // intentionally integer to allow index to be negative when deleting index.
		num_edges = (int) edges.size();
		for (index_edges1 = 0; index_edges1 < num_edges; index_edges1++)
		{
			CL_DelauneyTriangulator_Triangle_Edge &edge1 = edges[index_edges1];
			for (index_edges2 = 0/*index_edges1+1*/; index_edges2 < num_edges; index_edges2++)
			{
				if (index_edges1 == index_edges2) continue;
				CL_DelauneyTriangulator_Triangle_Edge &edge2 = edges[index_edges2];
				if ((edge1.first == edge2.first && edge1.second == edge2.second) ||
					(edge1.second == edge2.first && edge1.first == edge2.second))
				{
					// Same edges, delete both:
					if (index_edges1 < index_edges2)
					{
						edges.erase(edges.begin()+index_edges2);
						edges.erase(edges.begin()+index_edges1);
					}
					else
					{
						edges.erase(edges.begin()+index_edges1);
						edges.erase(edges.begin()+index_edges2);
					}
					num_edges -= 2;
					index_edges1--;
					break;
				}
			}
		}

		// add to the triangle list all triangles formed between the point 
		// and the edges of the enclosing polygon
		for (index_edges1 = 0; index_edges1 < num_edges; index_edges1++)
		{
			CL_DelauneyTriangulator_Triangle triangle;
			triangle.vertex_A = edges[index_edges1].first;
			triangle.vertex_B = edges[index_edges1].second;
			triangle.vertex_C = insertion_point;
			triangles.push_back(triangle);
		}
	}

	// remove any triangles from the triangle list that use the supertriangle vertices
	std::vector<CL_DelauneyTriangulator_Triangle>::size_type index_triangles, num_triangles;
	num_triangles = triangles.size();
	for (index_triangles = 0; index_triangles < num_triangles; index_triangles++)
	{
		CL_DelauneyTriangulator_Triangle &cur_triangle = triangles[index_triangles];

		if (
			cur_triangle.vertex_A == super_triangle.vertex_A ||
			cur_triangle.vertex_A == super_triangle.vertex_B ||
			cur_triangle.vertex_A == super_triangle.vertex_C ||
			cur_triangle.vertex_B == super_triangle.vertex_A ||
			cur_triangle.vertex_B == super_triangle.vertex_B ||
			cur_triangle.vertex_B == super_triangle.vertex_C ||
			cur_triangle.vertex_C == super_triangle.vertex_A ||
			cur_triangle.vertex_C == super_triangle.vertex_B ||
			cur_triangle.vertex_C == super_triangle.vertex_C)
		{
			// triangle share one or more points with supertriangle, remove it:
			triangles.erase(triangles.begin()+index_triangles);
			index_triangles--;
			num_triangles--;
		}
	}
}

CL_DelauneyTriangulator_Vertex CL_DelauneyTriangulator_Generic::find_circumcenter(
	const CL_DelauneyTriangulator_Triangle &triangle)
{
	// Subject 1.04: How do I generate a circle through three points?
	// http://www.faqs.org/faqs/graphics/algorithms-faq/

	float a_0 = triangle.vertex_A->x;
	float a_1 = triangle.vertex_A->y;
	float b_0 = triangle.vertex_B->x;
	float b_1 = triangle.vertex_B->y;
	float c_0 = triangle.vertex_C->x;
	float c_1 = triangle.vertex_C->y;

	float A = b_0 - a_0;
	float B = b_1 - a_1;
	float C = c_0 - a_0;
	float D = c_1 - a_1;

	float E = A*(a_0 + b_0) + B*(a_1 + b_1);
	float F = C*(a_0 + c_0) + D*(a_1 + c_1);

	float G = 2.0f*(A*(c_1 - b_1)-B*(c_0 - b_0));

	float p_0 = (D*E - B*F) / G;
	float p_1 = (A*F - C*E) / G;

	CL_DelauneyTriangulator_Vertex circumcenter;
	circumcenter.data = 0;
	circumcenter.x = p_0;
	circumcenter.y = p_1;
	return circumcenter;
}

/////////////////////////////////////////////////////////////////////////////
// CL_Tesselator_Generic implementation:
