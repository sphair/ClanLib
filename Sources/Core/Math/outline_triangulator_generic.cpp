/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
#include "outline_triangulator_generic.h"
#include "API/Core/Math/delauney_triangulator.h"
#include "API/Core/Math/line_math.h"
#include <list>
#include <algorithm>

namespace clan
{
	OutlineTriangulator_Impl::OutlineTriangulator_Impl()
	{
	}

	OutlineTriangulator_Impl::~OutlineTriangulator_Impl()
	{
	}

	void OutlineTriangulator_Impl::triangulate()
	{
		// Order vertices:
		std::vector<OutlineTriangulator_Vertex *> vertices;
		create_ordered_vertex_list(vertices);

		// 1.  Create initial triangulation:
		DelauneyTriangulator delauney;
		std::vector<OutlineTriangulator_Vertex *>::size_type index_vertices, num_vertices;
		num_vertices = vertices.size();
		for (index_vertices = 0; index_vertices < num_vertices; index_vertices++)
		{
			vertices[index_vertices]->num_triangles = 0;
			vertices[index_vertices]->extra = 0;
			vertices[index_vertices]->triangles = nullptr;

			delauney.add_vertex(
				vertices[index_vertices]->x,
				vertices[index_vertices]->y,
				vertices[index_vertices]);
		}
		delauney.generate();

		// 2.  Link triangles to vertices:

		const std::vector<DelauneyTriangulator_Triangle> &triangles = delauney.get_triangles();

		std::vector<DelauneyTriangulator_Triangle>::size_type index_triangles, num_triangles;
		num_triangles = triangles.size();
		for (index_triangles = 0; index_triangles < num_triangles; index_triangles++)
		{
			OutlineTriangulator_Vertex *data_A = static_cast<OutlineTriangulator_Vertex *>(triangles[index_triangles].vertex_A->data);
			OutlineTriangulator_Vertex *data_B = static_cast<OutlineTriangulator_Vertex *>(triangles[index_triangles].vertex_B->data);
			OutlineTriangulator_Vertex *data_C = static_cast<OutlineTriangulator_Vertex *>(triangles[index_triangles].vertex_C->data);
			data_A->num_triangles++;
			data_B->num_triangles++;
			data_C->num_triangles++;
		}

		auto links = new DelauneyTriangulator_Triangle const *[num_triangles];
		int pos = 0;
		for (index_vertices = 0; index_vertices < num_vertices; index_vertices++)
		{
			vertices[index_vertices]->data = links + pos;
			pos += vertices[index_vertices]->num_triangles;
			vertices[index_vertices]->num_triangles = 0;
		}

		for (index_triangles = 0; index_triangles < num_triangles; index_triangles++)
		{
			OutlineTriangulator_Vertex *data_A = static_cast<OutlineTriangulator_Vertex *>(triangles[index_triangles].vertex_A->data);
			OutlineTriangulator_Vertex *data_B = static_cast<OutlineTriangulator_Vertex *>(triangles[index_triangles].vertex_B->data);
			OutlineTriangulator_Vertex *data_C = static_cast<OutlineTriangulator_Vertex *>(triangles[index_triangles].vertex_C->data);
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

		std::list<DelauneyTriangulator_Triangle const *> final_triangles;
		for (index_triangles = 0; index_triangles < num_triangles; index_triangles++)
			final_triangles.push_back(&triangles[index_triangles]);

		std::vector<DelauneyTriangulator_Triangle const **> added_links;
		std::vector<DelauneyTriangulator> extra_triangulations;

		std::vector<OutlineTriangulator_Polygon>::size_type index_polygons, num_polygons;
		num_polygons = polygons.size();
		for (index_polygons = 0; index_polygons < num_polygons; index_polygons++)
		{
			OutlineTriangulator_Polygon &cur_poly = polygons[index_polygons];
			std::vector<OutlineTriangulator_Contour>::size_type index_contours, num_contours;
			num_contours = cur_poly.contours.size();
			for (index_contours = 0; index_contours < num_contours; index_contours++)
			{
				OutlineTriangulator_Contour &cur_contour = cur_poly.contours[index_contours];
				std::vector<OutlineTriangulator_Vertex>::size_type index_vertices, num_vertices;
				num_vertices = cur_contour.vertices.size();
				for (index_vertices = 1; index_vertices < num_vertices; index_vertices++)
				{
					OutlineTriangulator_Vertex *vertex_1 = &cur_contour.vertices[index_vertices - 1];
					OutlineTriangulator_Vertex *vertex_2 = &cur_contour.vertices[index_vertices];
					OutlineTriangulator_Collision collision = find_colliding_triangles(vertex_1, vertex_2);
					if (collision.triangles.empty())
						continue;

					std::vector<OutlineTriangulator_Vertex *>::size_type index_points, num_points;

					// Triangulate left and right sides:

					DelauneyTriangulator delauney_first;
					num_points = collision.first.size();
					for (index_points = 0; index_points < num_points; index_points++)
					{
						delauney.add_vertex(
							collision.first[index_points]->x,
							collision.first[index_points]->y,
							collision.first[index_points]);
					}
					delauney_first.generate();

					DelauneyTriangulator delauney_second;
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

					std::vector<DelauneyTriangulator_Triangle *>::size_type index_old_triangles, size_old_triangles;
					size_old_triangles = collision.triangles.size();
					for (index_old_triangles = 0; index_old_triangles < size_old_triangles; index_old_triangles++)
					{
						remove_triangle(collision.triangles[index_old_triangles]);
						final_triangles.remove(collision.triangles[index_old_triangles]);
					}

					// Add new triangles:

					DelauneyTriangulator_Triangle const **new_links = add_triangles(delauney_first, delauney_second);
					added_links.push_back(new_links);

					const std::vector<DelauneyTriangulator_Triangle> &triangles1 = delauney_first.get_triangles();
					const std::vector<DelauneyTriangulator_Triangle> &triangles2 = delauney_second.get_triangles();
					std::vector<DelauneyTriangulator_Triangle>::size_type index_triangles, num_triangles1, num_triangles2;
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
		std::vector<DelauneyTriangulator_Triangle const **>::size_type index_added_links, size_added_links;
		size_added_links = added_links.size();
		for (index_added_links = 0; index_added_links < size_added_links; index_added_links++)
		{
			delete[] added_links[index_added_links];
		}

		// 6. Generate final list:



	}

	DelauneyTriangulator_Triangle const **OutlineTriangulator_Impl::add_triangles(
		DelauneyTriangulator &d1,
		DelauneyTriangulator &d2)
	{
		const std::vector<DelauneyTriangulator_Triangle> &triangles1 = d1.get_triangles();
		const std::vector<DelauneyTriangulator_Triangle> &triangles2 = d2.get_triangles();
		std::vector<DelauneyTriangulator_Triangle>::size_type index_triangles, num_triangles1, num_triangles2;
		num_triangles1 = triangles1.size();
		for (index_triangles = 0; index_triangles < num_triangles1; index_triangles++)
		{
			OutlineTriangulator_Vertex *data_A = static_cast<OutlineTriangulator_Vertex *>(triangles1[index_triangles].vertex_A->data);
			OutlineTriangulator_Vertex *data_B = static_cast<OutlineTriangulator_Vertex *>(triangles1[index_triangles].vertex_B->data);
			OutlineTriangulator_Vertex *data_C = static_cast<OutlineTriangulator_Vertex *>(triangles1[index_triangles].vertex_C->data);
			data_A->extra++;
			data_B->extra++;
			data_C->extra++;
		}
		num_triangles2 = triangles2.size();
		for (index_triangles = 0; index_triangles < num_triangles2; index_triangles++)
		{
			OutlineTriangulator_Vertex *data_A = static_cast<OutlineTriangulator_Vertex *>(triangles2[index_triangles].vertex_A->data);
			OutlineTriangulator_Vertex *data_B = static_cast<OutlineTriangulator_Vertex *>(triangles2[index_triangles].vertex_B->data);
			OutlineTriangulator_Vertex *data_C = static_cast<OutlineTriangulator_Vertex *>(triangles2[index_triangles].vertex_C->data);
			data_A->extra++;
			data_B->extra++;
			data_C->extra++;
		}

		const std::vector<DelauneyTriangulator_Vertex> &vertices1 = d1.get_vertices();
		const std::vector<DelauneyTriangulator_Vertex> &vertices2 = d2.get_vertices();
		std::vector<DelauneyTriangulator_Vertex *>::size_type index_vertices, num_vertices1, num_vertices2;
		num_vertices1 = d1.get_vertices().size();
		num_vertices2 = d2.get_vertices().size();

		int links_needed = 0;
		for (index_vertices = 0; index_vertices < num_vertices1; index_vertices++)
		{
			OutlineTriangulator_Vertex *v = static_cast<OutlineTriangulator_Vertex *>(vertices1[index_vertices].data);
			links_needed += v->num_triangles + v->extra;
		}
		for (index_vertices = 0; index_vertices < num_vertices2; index_vertices++)
		{
			OutlineTriangulator_Vertex *v = static_cast<OutlineTriangulator_Vertex *>(vertices2[index_vertices].data);
			links_needed += v->num_triangles + v->extra;
		}

		auto links = new DelauneyTriangulator_Triangle const *[links_needed];
		int pos = 0;
		for (index_vertices = 0; index_vertices < num_vertices1; index_vertices++)
		{
			OutlineTriangulator_Vertex *v = static_cast<OutlineTriangulator_Vertex *>(vertices1[index_vertices].data);
			if (v->extra == 0)
				continue;

			for (int i = 0; i < v->num_triangles; i++)
				links[pos + i] = v->triangles[i];

			v->data = links + pos;
			pos += v->num_triangles + v->extra;
			v->extra = 0;
		}
		for (index_vertices = 0; index_vertices < num_vertices2; index_vertices++)
		{
			OutlineTriangulator_Vertex *v = static_cast<OutlineTriangulator_Vertex *>(vertices2[index_vertices].data);
			if (v->extra == 0)
				continue;

			for (int i = 0; i < v->num_triangles; i++)
				links[pos + i] = v->triangles[i];

			v->data = links + pos;
			pos += v->num_triangles + v->extra;
			v->extra = 0;
		}

		for (index_triangles = 0; index_triangles < num_triangles1; index_triangles++)
		{
			OutlineTriangulator_Vertex *data_A = static_cast<OutlineTriangulator_Vertex *>(triangles1[index_triangles].vertex_A->data);
			OutlineTriangulator_Vertex *data_B = static_cast<OutlineTriangulator_Vertex *>(triangles1[index_triangles].vertex_B->data);
			OutlineTriangulator_Vertex *data_C = static_cast<OutlineTriangulator_Vertex *>(triangles1[index_triangles].vertex_C->data);
			data_A->triangles[data_A->num_triangles++] = &triangles1[index_triangles];
			data_B->triangles[data_B->num_triangles++] = &triangles1[index_triangles];
			data_C->triangles[data_C->num_triangles++] = &triangles1[index_triangles];
		}
		for (index_triangles = 0; index_triangles < num_triangles2; index_triangles++)
		{
			OutlineTriangulator_Vertex *data_A = static_cast<OutlineTriangulator_Vertex *>(triangles2[index_triangles].vertex_A->data);
			OutlineTriangulator_Vertex *data_B = static_cast<OutlineTriangulator_Vertex *>(triangles2[index_triangles].vertex_B->data);
			OutlineTriangulator_Vertex *data_C = static_cast<OutlineTriangulator_Vertex *>(triangles2[index_triangles].vertex_C->data);
			data_A->triangles[data_A->num_triangles++] = &triangles2[index_triangles];
			data_B->triangles[data_B->num_triangles++] = &triangles2[index_triangles];
			data_C->triangles[data_C->num_triangles++] = &triangles2[index_triangles];
		}

		return links;
	}

	OutlineTriangulator_Collision OutlineTriangulator_Impl::find_colliding_triangles(
		OutlineTriangulator_Vertex *v1,
		OutlineTriangulator_Vertex *v2)
	{
		OutlineTriangulator_Collision collision;

		for (int index_triangles = 0; index_triangles < v1->num_triangles; index_triangles++)
		{
			OutlineTriangulator_Vertex *vA = static_cast<OutlineTriangulator_Vertex *>(v1->triangles[index_triangles]->vertex_A->data);
			OutlineTriangulator_Vertex *vB = static_cast<OutlineTriangulator_Vertex *>(v1->triangles[index_triangles]->vertex_B->data);
			OutlineTriangulator_Vertex *vC = static_cast<OutlineTriangulator_Vertex *>(v1->triangles[index_triangles]->vertex_C->data);

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
			else if (LineMath::point_right_of_line(vA->x, vA->y, v1->x, v1->y, v2->x, v2->y))
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
			else if (LineMath::point_right_of_line(vB->x, vB->y, v1->x, v1->y, v2->x, v2->y))
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
			else if (LineMath::point_right_of_line(vC->x, vC->y, v1->x, v1->y, v2->x, v2->y))
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

	void OutlineTriangulator_Impl::remove_triangle(DelauneyTriangulator_Triangle const *t)
	{
		for (int vertex = 0; vertex < 3; vertex++)
		{
			OutlineTriangulator_Vertex *data = nullptr;
			switch (vertex)
			{
			case 0:
				data = static_cast<OutlineTriangulator_Vertex *>(t->vertex_A->data);
				break;

			case 1:
				data = static_cast<OutlineTriangulator_Vertex *>(t->vertex_B->data);
				break;

			case 2:
				data = static_cast<OutlineTriangulator_Vertex *>(t->vertex_C->data);
				break;
			}

			for (int index = 0; index < data->num_triangles; index++)
			{
				if (data->triangles[index] == t)
				{
					for (int index2 = index; index2 < data->num_triangles - 1; index2++)
					{
						data->triangles[index2] = data->triangles[index2 + 1];
					}
					index--;
					data->num_triangles--;
				}
			}
		}
	}

	struct CompareVertices
	{
		bool operator()(OutlineTriangulator_Vertex *a, OutlineTriangulator_Vertex *b) const
		{
			if (a->x == b->x) return a->y < b->y;
			return a->x < b->x;
		}
	};

	void OutlineTriangulator_Impl::create_ordered_vertex_list(std::vector<OutlineTriangulator_Vertex *> &vertices)
	{
		std::vector<OutlineTriangulator_Polygon>::size_type index_polygons, num_polygons;
		num_polygons = polygons.size();
		for (index_polygons = 0; index_polygons < num_polygons; index_polygons++)
		{
			OutlineTriangulator_Polygon &cur_poly = polygons[index_polygons];
			std::vector<OutlineTriangulator_Contour>::size_type index_contours, num_contours;
			num_contours = cur_poly.contours.size();
			for (index_contours = 0; index_contours < num_contours; index_contours++)
			{
				OutlineTriangulator_Contour &cur_contour = cur_poly.contours[index_contours];
				std::vector<OutlineTriangulator_Vertex>::size_type index_vertices, num_vertices;
				num_vertices = cur_contour.vertices.size();
				for (index_vertices = 0; index_vertices < num_vertices; index_vertices++)
				{
					vertices.push_back(&cur_contour.vertices[index_vertices]);
				}
			}
		}

		// Sort list:
		std::sort(vertices.begin(), vertices.end(), CompareVertices());

		// Remove duplicates:
		auto it = vertices.begin();
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

	bool OutlineTriangulator_Impl::intersects(
		float Ax,
		float Ay,
		float Bx,
		float By,
		float Cx,
		float Cy,
		float Dx,
		float Dy)
	{
		float denominator = ((Bx - Ax)*(Dy - Cy) - (By - Ay)*(Dx - Cx));

		if (denominator == 0) // parallell
		{
			return false;
		}

		float r = ((Ay - Cy)*(Dx - Cx) - (Ax - Cx)*(Dy - Cy)) / denominator;
		float s = ((Ay - Cy)*(Bx - Ax) - (Ax - Cx)*(By - Ay)) / denominator;

		if ((s > 0.0 && s < 1.0) && (r > 0.0 && r < 1.0))
			return true;

		return false;
	}
}
