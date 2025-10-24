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

#include "Core/precomp.h"
#include "outline_triangulator_generic.h"
#include "API/Core/Math/delauney_triangulator.h"
#include "API/Core/Math/line_math.h"

/////////////////////////////////////////////////////////////////////////////
// CL_OutlineTriangulator_Generic construction:

CL_OutlineTriangulator_Generic::CL_OutlineTriangulator_Generic()
{
}

CL_OutlineTriangulator_Generic::~CL_OutlineTriangulator_Generic()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_OutlineTriangulator_Generic attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_OutlineTriangulator_Generic operations:

void CL_OutlineTriangulator_Generic::triangulate()
{
	// Order vertices:
	std::vector<CL_OutlineTriangulator_Vertex *> vertices;
	create_ordered_vertex_list(vertices);

	// 1.  Create initial triangulation:
	CL_DelauneyTriangulator delauney;
	std::vector<CL_OutlineTriangulator_Vertex *>::size_type index_vertices, num_vertices;
	num_vertices = vertices.size();
	for (index_vertices = 0; index_vertices < num_vertices; index_vertices++)
	{
		vertices[index_vertices]->num_triangles = 0;
		vertices[index_vertices]->extra = 0;
		vertices[index_vertices]->triangles = 0;

		delauney.add_vertex(
			vertices[index_vertices]->x,
			vertices[index_vertices]->y,
			vertices[index_vertices]);
	}
	delauney.generate();

	// 2.  Link triangles to vertices:

	const std::vector<CL_DelauneyTriangulator_Triangle> &triangles = delauney.get_triangles();

	std::vector<CL_DelauneyTriangulator_Triangle>::size_type index_triangles, num_triangles;
	num_triangles = triangles.size();
	for (index_triangles = 0; index_triangles < num_triangles; index_triangles++)
	{
		CL_OutlineTriangulator_Vertex *data_A = (CL_OutlineTriangulator_Vertex *) triangles[index_triangles].vertex_A->data;
		CL_OutlineTriangulator_Vertex *data_B = (CL_OutlineTriangulator_Vertex *) triangles[index_triangles].vertex_B->data;
		CL_OutlineTriangulator_Vertex *data_C = (CL_OutlineTriangulator_Vertex *) triangles[index_triangles].vertex_C->data;
		data_A->num_triangles++;
		data_B->num_triangles++;
		data_C->num_triangles++;
	}

	CL_DelauneyTriangulator_Triangle const **links = new CL_DelauneyTriangulator_Triangle const *[num_triangles];
	int pos = 0;
	for (index_vertices = 0; index_vertices < num_vertices; index_vertices++)
	{
		vertices[index_vertices]->data = links+pos;
		pos += vertices[index_vertices]->num_triangles;
		vertices[index_vertices]->num_triangles = 0;
	}

	for (index_triangles = 0; index_triangles < num_triangles; index_triangles++)
	{
		CL_OutlineTriangulator_Vertex *data_A = (CL_OutlineTriangulator_Vertex *) triangles[index_triangles].vertex_A->data;
		CL_OutlineTriangulator_Vertex *data_B = (CL_OutlineTriangulator_Vertex *) triangles[index_triangles].vertex_B->data;
		CL_OutlineTriangulator_Vertex *data_C = (CL_OutlineTriangulator_Vertex *) triangles[index_triangles].vertex_C->data;
		data_A->triangles[data_A->num_triangles++] = &triangles[index_triangles];
		data_B->triangles[data_B->num_triangles++] = &triangles[index_triangles];
		data_C->triangles[data_C->num_triangles++] = &triangles[index_triangles];
	}

	// 3.  Walk contours. Check if any triangles intersect with each line segment.
	// 3a. Add each triangle's point that intersect to vertex buffer.
	// 3b. Divide vertices into two lists, one for left and one for right side of line segment.
	// 3c. Delauney triangulate each point list.
	// 3d. Update links to include new triangles.
	// 3e. Add the resulting triangles to triangles list.

	std::list<CL_DelauneyTriangulator_Triangle const *> final_triangles;
	for (index_triangles = 0; index_triangles < num_triangles; index_triangles++)
		final_triangles.push_back(&triangles[index_triangles]);

	std::vector<CL_DelauneyTriangulator_Triangle const **> added_links;
	std::vector<CL_DelauneyTriangulator> extra_triangulations;

	std::vector<CL_OutlineTriangulator_Polygon>::size_type index_polygons, num_polygons;
	num_polygons = polygons.size();
	for (index_polygons = 0; index_polygons < num_polygons; index_polygons++)
	{
		CL_OutlineTriangulator_Polygon &cur_poly = polygons[index_polygons];
		std::vector<CL_OutlineTriangulator_Contour>::size_type index_contours, num_contours;
		num_contours = cur_poly.contours.size();
		for (index_contours = 0; index_contours < num_contours; index_contours++)
		{
			CL_OutlineTriangulator_Contour &cur_contour = cur_poly.contours[index_contours];
			std::vector<CL_OutlineTriangulator_Vertex>::size_type index_vertices, num_vertices;
			num_vertices = cur_contour.vertices.size();
			for (index_vertices = 1; index_vertices < num_vertices; index_vertices++)
			{
				CL_OutlineTriangulator_Vertex *vertex_1 = &cur_contour.vertices[index_vertices-1];
				CL_OutlineTriangulator_Vertex *vertex_2 = &cur_contour.vertices[index_vertices];
				CL_OutlineTriangulator_Collision collision = find_colliding_triangles(vertex_1, vertex_2);
				if (collision.triangles.empty())
					continue;

				std::vector<CL_OutlineTriangulator_Vertex *>::size_type index_points, num_points;

				// Triangulate left and right sides:

				CL_DelauneyTriangulator delauney_first;
				num_points = collision.first.size();
				for (index_points = 0; index_points < num_points; index_points++)
				{
					delauney.add_vertex(
						collision.first[index_points]->x,
						collision.first[index_points]->y,
						collision.first[index_points]);
				}
				delauney_first.generate();

				CL_DelauneyTriangulator delauney_second;
				num_points = collision.second.size();
				for (index_points = 0; index_points < num_points; index_points++)
				{
					delauney.add_vertex(
						collision.second[index_points]->x,
						collision.second[index_points]->y,
						collision.second[index_points]);
				}
				delauney_second.generate();

				extra_triangulations.push_back(delauney_first);
				extra_triangulations.push_back(delauney_second);

				// Remove old triangles:

				std::vector<CL_DelauneyTriangulator_Triangle *>::size_type index_old_triangles, size_old_triangles;
				size_old_triangles = collision.triangles.size();
				for (index_old_triangles = 0; index_old_triangles < size_old_triangles; index_old_triangles++)
				{
					remove_triangle(collision.triangles[index_old_triangles]);
					final_triangles.remove(collision.triangles[index_old_triangles]);
				}

				// Add new triangles:

				CL_DelauneyTriangulator_Triangle const **new_links = add_triangles(delauney_first, delauney_second);
				added_links.push_back(new_links);

				const std::vector<CL_DelauneyTriangulator_Triangle> &triangles1 = delauney_first.get_triangles();
				const std::vector<CL_DelauneyTriangulator_Triangle> &triangles2 = delauney_second.get_triangles();
				std::vector<CL_DelauneyTriangulator_Triangle>::size_type index_triangles, num_triangles1, num_triangles2;
				num_triangles1 = triangles1.size();
				num_triangles2 = triangles2.size();
				for (index_triangles = 0; index_triangles < num_triangles1; index_triangles++)
					final_triangles.push_back(&triangles1[index_triangles]);
				for (index_triangles = 0; index_triangles < num_triangles2; index_triangles++)
					final_triangles.push_back(&triangles2[index_triangles]);
			}
		}
	}

	// 4. Remove outside and hole triangles.

	// 5. Clean up:

	delete[] links;
	std::vector<CL_DelauneyTriangulator_Triangle const **>::size_type index_added_links, size_added_links;
	size_added_links = added_links.size();
	for (index_added_links = 0; index_added_links < size_added_links; index_added_links++)
	{
		delete[] added_links[index_added_links];
	}

	// 6. Generate final list:



}

CL_DelauneyTriangulator_Triangle const **CL_OutlineTriangulator_Generic::add_triangles(
	CL_DelauneyTriangulator &d1,
	CL_DelauneyTriangulator &d2)
{
	const std::vector<CL_DelauneyTriangulator_Triangle> &triangles1 = d1.get_triangles();
	const std::vector<CL_DelauneyTriangulator_Triangle> &triangles2 = d2.get_triangles();
	std::vector<CL_DelauneyTriangulator_Triangle>::size_type index_triangles, num_triangles1, num_triangles2;
	num_triangles1 = triangles1.size();
	for (index_triangles = 0; index_triangles < num_triangles1; index_triangles++)
	{
		CL_OutlineTriangulator_Vertex *data_A = (CL_OutlineTriangulator_Vertex *) triangles1[index_triangles].vertex_A->data;
		CL_OutlineTriangulator_Vertex *data_B = (CL_OutlineTriangulator_Vertex *) triangles1[index_triangles].vertex_B->data;
		CL_OutlineTriangulator_Vertex *data_C = (CL_OutlineTriangulator_Vertex *) triangles1[index_triangles].vertex_C->data;
		data_A->extra++;
		data_B->extra++;
		data_C->extra++;
	}
	num_triangles2 = triangles2.size();
	for (index_triangles = 0; index_triangles < num_triangles2; index_triangles++)
	{
		CL_OutlineTriangulator_Vertex *data_A = (CL_OutlineTriangulator_Vertex *) triangles2[index_triangles].vertex_A->data;
		CL_OutlineTriangulator_Vertex *data_B = (CL_OutlineTriangulator_Vertex *) triangles2[index_triangles].vertex_B->data;
		CL_OutlineTriangulator_Vertex *data_C = (CL_OutlineTriangulator_Vertex *) triangles2[index_triangles].vertex_C->data;
		data_A->extra++;
		data_B->extra++;
		data_C->extra++;
	}

	const std::vector<CL_DelauneyTriangulator_Vertex> &vertices1 = d1.get_vertices();
	const std::vector<CL_DelauneyTriangulator_Vertex> &vertices2 = d2.get_vertices();
	std::vector<CL_DelauneyTriangulator_Vertex *>::size_type index_vertices, num_vertices1, num_vertices2;
	num_vertices1 = d1.get_vertices().size();
	num_vertices2 = d2.get_vertices().size();

	int links_needed = 0;
	for (index_vertices = 0; index_vertices < num_vertices1; index_vertices++)
	{
		CL_OutlineTriangulator_Vertex *v = (CL_OutlineTriangulator_Vertex *) vertices1[index_vertices].data;
		links_needed += v->num_triangles + v->extra;
	}
	for (index_vertices = 0; index_vertices < num_vertices2; index_vertices++)
	{
		CL_OutlineTriangulator_Vertex *v = (CL_OutlineTriangulator_Vertex *) vertices2[index_vertices].data;
		links_needed += v->num_triangles + v->extra;
	}

	CL_DelauneyTriangulator_Triangle const **links = new CL_DelauneyTriangulator_Triangle const *[links_needed];
	int pos = 0;
	for (index_vertices = 0; index_vertices < num_vertices1; index_vertices++)
	{
		CL_OutlineTriangulator_Vertex *v = (CL_OutlineTriangulator_Vertex *) vertices1[index_vertices].data;
		if (v->extra == 0)
			continue;

		for (int i=0; i<v->num_triangles; i++)
			links[pos+i] = v->triangles[i];

		v->data = links+pos;
		pos += v->num_triangles + v->extra;
		v->extra = 0;
	}
	for (index_vertices = 0; index_vertices < num_vertices2; index_vertices++)
	{
		CL_OutlineTriangulator_Vertex *v = (CL_OutlineTriangulator_Vertex *) vertices2[index_vertices].data;
		if (v->extra == 0)
			continue;

		for (int i=0; i<v->num_triangles; i++)
			links[pos+i] = v->triangles[i];

		v->data = links+pos;
		pos += v->num_triangles + v->extra;
		v->extra = 0;
	}

	for (index_triangles = 0; index_triangles < num_triangles1; index_triangles++)
	{
		CL_OutlineTriangulator_Vertex *data_A = (CL_OutlineTriangulator_Vertex *) triangles1[index_triangles].vertex_A->data;
		CL_OutlineTriangulator_Vertex *data_B = (CL_OutlineTriangulator_Vertex *) triangles1[index_triangles].vertex_B->data;
		CL_OutlineTriangulator_Vertex *data_C = (CL_OutlineTriangulator_Vertex *) triangles1[index_triangles].vertex_C->data;
		data_A->triangles[data_A->num_triangles++] = &triangles1[index_triangles];
		data_B->triangles[data_B->num_triangles++] = &triangles1[index_triangles];
		data_C->triangles[data_C->num_triangles++] = &triangles1[index_triangles];
	}
	for (index_triangles = 0; index_triangles < num_triangles2; index_triangles++)
	{
		CL_OutlineTriangulator_Vertex *data_A = (CL_OutlineTriangulator_Vertex *) triangles2[index_triangles].vertex_A->data;
		CL_OutlineTriangulator_Vertex *data_B = (CL_OutlineTriangulator_Vertex *) triangles2[index_triangles].vertex_B->data;
		CL_OutlineTriangulator_Vertex *data_C = (CL_OutlineTriangulator_Vertex *) triangles2[index_triangles].vertex_C->data;
		data_A->triangles[data_A->num_triangles++] = &triangles2[index_triangles];
		data_B->triangles[data_B->num_triangles++] = &triangles2[index_triangles];
		data_C->triangles[data_C->num_triangles++] = &triangles2[index_triangles];
	}

	return links;
}

CL_OutlineTriangulator_Collision CL_OutlineTriangulator_Generic::find_colliding_triangles(
	CL_OutlineTriangulator_Vertex *v1,
	CL_OutlineTriangulator_Vertex *v2)
{
	CL_OutlineTriangulator_Collision collision;

	for (int index_triangles = 0; index_triangles < v1->num_triangles; index_triangles++)
	{
		CL_OutlineTriangulator_Vertex *vA = (CL_OutlineTriangulator_Vertex *) v1->triangles[index_triangles]->vertex_A->data;
		CL_OutlineTriangulator_Vertex *vB = (CL_OutlineTriangulator_Vertex *) v1->triangles[index_triangles]->vertex_B->data;
		CL_OutlineTriangulator_Vertex *vC = (CL_OutlineTriangulator_Vertex *) v1->triangles[index_triangles]->vertex_C->data;

		// If an edge is same line as the constrained line, there can be no colliding triangles:
		if (
			(v1 == vA && v2 == vB) ||
			(v1 == vB && v2 == vA) ||
			(v1 == vB && v2 == vC) ||
			(v1 == vC && v2 == vB) ||
			(v1 == vC && v2 == vA) ||
			(v1 == vA && v2 == vC))
			return collision;

		bool intersectsA, intersectsB, intersectsC;

		if (v1 == vA || v1 == vB || v2 == vA || v2 == vB)
			intersectsA = false;
		else
			intersectsA = intersects(
				v1->x, v1->y, v2->x, v2->y,
				v1->triangles[index_triangles]->vertex_A->x,
				v1->triangles[index_triangles]->vertex_A->y,
				v1->triangles[index_triangles]->vertex_B->x,
				v1->triangles[index_triangles]->vertex_B->y);

		if (v1 == vB || v1 == vC || v2 == vB || v2 == vC)
			intersectsB = false;
		else
			intersectsB = intersects(
				v1->x, v1->y, v2->x, v2->y,
				v1->triangles[index_triangles]->vertex_B->x,
				v1->triangles[index_triangles]->vertex_B->y,
				v1->triangles[index_triangles]->vertex_C->x,
				v1->triangles[index_triangles]->vertex_C->y);

		if (v1 == vC || v1 == vA || v2 == vC || v2 == vA)
			intersectsC = false;
		else
			intersectsC = intersects(
				v1->x, v1->y, v2->x, v2->y,
				v1->triangles[index_triangles]->vertex_C->x,
				v1->triangles[index_triangles]->vertex_C->y,
				v1->triangles[index_triangles]->vertex_A->x,
				v1->triangles[index_triangles]->vertex_A->y);

		if (intersectsA == false && intersectsB == false && intersectsC == false)
			continue;

		// Collision. Add all points to the correct side of the intersected line:

		collision.triangles.push_back(v1->triangles[index_triangles]);

		if (vA == v1 || vA == v2)
		{
			collision.first.push_back(vA);
			collision.second.push_back(vA);
		}
		else if (CL_LineMath::point_right_of_line(vA->x, vA->y, v1->x, v1->y, v2->x, v2->y))
		{
			collision.second.push_back(vA);
		}
		else
		{
			collision.first.push_back(vA);
		}

		if (vB == v1 || vB == v2)
		{
			collision.first.push_back(vB);
			collision.second.push_back(vB);
		}
		else if (CL_LineMath::point_right_of_line(vB->x, vB->y, v1->x, v1->y, v2->x, v2->y))
		{
			collision.second.push_back(vB);
		}
		else
		{
			collision.first.push_back(vB);
		}

		if (vC == v1 || vC == v2)
		{
			collision.first.push_back(vC);
			collision.second.push_back(vC);
		}
		else if (CL_LineMath::point_right_of_line(vC->x, vC->y, v1->x, v1->y, v2->x, v2->y))
		{
			collision.second.push_back(vC);
		}
		else
		{
			collision.first.push_back(vC);
		}
	}

	return collision;
}

void CL_OutlineTriangulator_Generic::remove_triangle(CL_DelauneyTriangulator_Triangle const *t)
{
	for (int vertex = 0; vertex < 3; vertex++)
	{
		CL_OutlineTriangulator_Vertex *data = 0;
		switch (vertex)
		{
		case 0:
			data = (CL_OutlineTriangulator_Vertex *) t->vertex_A->data;
			break;

		case 1:
			data = (CL_OutlineTriangulator_Vertex *) t->vertex_B->data;
			break;

		case 2:
			data = (CL_OutlineTriangulator_Vertex *) t->vertex_C->data;
			break;
		}

		for (int index = 0; index < data->num_triangles; index++)
		{
			if (data->triangles[index] == t)
			{
				for (int index2 = index; index2 < data->num_triangles-1; index2++)
				{
					data->triangles[index2] = data->triangles[index2+1];
				}
				index--;
				data->num_triangles--;
			}
		}
	}
}

struct CL_CompareVertices
{
	bool operator()(CL_OutlineTriangulator_Vertex *a, CL_OutlineTriangulator_Vertex *b) const
	{
		if (a->x == b->x) return a->y < b->y;
		return a->x < b->x;
	}
};

void CL_OutlineTriangulator_Generic::create_ordered_vertex_list(std::vector<CL_OutlineTriangulator_Vertex *> &vertices)
{
	std::vector<CL_OutlineTriangulator_Polygon>::size_type index_polygons, num_polygons;
	num_polygons = polygons.size();
	for (index_polygons = 0; index_polygons < num_polygons; index_polygons++)
	{
		CL_OutlineTriangulator_Polygon &cur_poly = polygons[index_polygons];
		std::vector<CL_OutlineTriangulator_Contour>::size_type index_contours, num_contours;
		num_contours = cur_poly.contours.size();
		for (index_contours = 0; index_contours < num_contours; index_contours++)
		{
			CL_OutlineTriangulator_Contour &cur_contour = cur_poly.contours[index_contours];
			std::vector<CL_OutlineTriangulator_Vertex>::size_type index_vertices, num_vertices;
			num_vertices = cur_contour.vertices.size();
			for (index_vertices = 0; index_vertices < num_vertices; index_vertices++)
			{
				vertices.push_back(&cur_contour.vertices[index_vertices]);
			}
		}
	}

	// Sort list:
	std::sort(vertices.begin(), vertices.end(), CL_CompareVertices());

	// Remove duplicates:
	std::vector<CL_OutlineTriangulator_Vertex *>::iterator it = vertices.begin();
	if (it == vertices.end()) return;
	double last_x = (*it)->x;
	double last_y = (*it)->y;
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

bool CL_OutlineTriangulator_Generic::intersects( 
	double Ax,
	double Ay,
	double Bx,
	double By,
	double Cx,
	double Cy,
	double Dx,
	double Dy)
{
	double denominator = ((Bx-Ax)*(Dy-Cy)-(By-Ay)*(Dx-Cx));	
	
	if( denominator == 0 ) // parallell
	{
		return false;
	}

	double r = ((Ay-Cy)*(Dx-Cx)-(Ax-Cx)*(Dy-Cy)) / denominator;
	double s = ((Ay-Cy)*(Bx-Ax)-(Ax-Cx)*(By-Ay)) / denominator;

	if( (s > 0.0f && s < 1.0f) && (r > 0.0f && r < 1.0f) )
		return true;

	return false;
}

/////////////////////////////////////////////////////////////////////////////
// CL_OutlineTriangulator_Generic implementation:
