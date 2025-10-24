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
**    Harry Storbacka
*/

#include "Core/precomp.h"
#include "API/Core/Math/ear_clip_triangulator.h"
#include "API/Core/Math/ear_clip_result.h"
#include "API/Core/Math/point.h"
#include "API/Core/Math/triangle_math.h"
#include "API/Core/Math/line_math.h"
#include "API/Core/Text/string_types.h"
#include "ear_clip_triangulator_impl.h"
#include <cfloat>

/////////////////////////////////////////////////////////////////////////////
// CL_EarClipTriangulator_Impl Construction:

CL_EarClipTriangulator_Impl::CL_EarClipTriangulator_Impl()
: orientation(cl_clockwise), vertex_count(0)
{
	target_array = &vertices;
}

CL_EarClipTriangulator_Impl::~CL_EarClipTriangulator_Impl()
{
	for (unsigned int cnt = 0; cnt < vertices.size(); cnt++)
	{
		delete vertices[cnt];
	}

	for (unsigned int cnt = 0; cnt < hole.size(); cnt++)
	{
		delete hole[cnt];
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_EarClipTriangulator_Impl Attributes:

std::vector<CL_Pointf> CL_EarClipTriangulator_Impl::get_vertices()
{
	std::vector<CL_Pointf> points;

	for (unsigned int cnt = 0; cnt < vertices.size(); cnt++)
	{
		points.push_back( CL_Pointf(vertices[cnt]->x, vertices[cnt]->y));
	}

	return points;
}

/////////////////////////////////////////////////////////////////////////////
// CL_EarClipTriangulator_Impl Operations:

void CL_EarClipTriangulator_Impl::set_orientation(CL_PolygonOrientation orient)
{
	orientation = orient;
}

void CL_EarClipTriangulator_Impl::add_vertex(float x, float y)
{
	// Check for duplicated vertice
	std::vector<LinkedVertice *>::size_type size = target_array->size();
	if (size)
	{
		LinkedVertice *last_vertice = (*target_array)[(size-1)];
		if ((last_vertice->x == x) && (last_vertice->y == y) )
		{
			return;	// Ignore this vertice
		}
	}
	target_array->push_back(new LinkedVertice(x,y));
	vertex_count++;
}

void CL_EarClipTriangulator_Impl::clear()
{
	for (unsigned int cnt = 0; cnt < vertices.size(); cnt++)
	{
		delete vertices[cnt];
	}

	vertices.clear();
	hole.clear();
	ear_list.clear();
	vertex_count = 0;
}

CL_EarClipResult CL_EarClipTriangulator_Impl::triangulate()
{
	create_lists(true);

	int num_triangles = vertices.size()-2;
	int tri_count = 0;
	
	CL_EarClipResult result(num_triangles);
	
	while( tri_count < num_triangles )
	{
		if( ear_list.empty() ) // something went wrong, but lets not crash anyway. 
			break;

		LinkedVertice *v = ear_list.back();
		ear_list.pop_back();
 
		CL_EarClipTriangulator_Triangle tri;

		tri.x1 = v->x;
		tri.y1 = v->y;

		tri.x2 = v->previous->x;
		tri.y2 = v->previous->y;

		tri.x3 = v->next->x;
		tri.y3 = v->next->y;

		result.get_triangles().push_back(tri);

		v->next->previous = v->previous;
		v->previous->next = v->next;

		if( is_ear(*v->next) )
		{
			if( v->next->is_ear == false ) // not marked as an ear yet. Mark it, and add to the list.
			{
				v->next->is_ear = true;
				ear_list.push_back(v->next);
			}
		}
		else
		{
			if( v->next->is_ear == true ) // Not an ear any more. Delete from ear list.
			{
				v->next->is_ear = false;
				
				std::vector<LinkedVertice*>::iterator it;
				for( it = ear_list.begin(); it != ear_list.end(); ++it )
				{
					if( (*it) == v->next )
					{
						ear_list.erase(it);
						break;
					}
				}
			}
		}

		if( is_ear(*v->previous) )
		{
			if( v->previous->is_ear == false ) // not marked as an ear yet. Mark it, and add to the list.
			{
				v->previous->is_ear = true;
				ear_list.push_back(v->previous);
			}
		}
		else
		{
			if( v->previous->is_ear == true ) // Not an ear any more. Delete from ear list.
			{
				v->previous->is_ear = false;
				
				std::vector<LinkedVertice*>::iterator it;
				for( it = ear_list.begin(); it != ear_list.end(); ++it )
				{
					if( (*it) == v->previous )
					{
						ear_list.erase(it);
						break;
					}
				}
			}
		}

/*		cl_write_console_line("Ear list:");
		for( std::vector<LinkedVertice*>::iterator it = ear_list.begin(); it != ear_list.end(); ++it )
		{
			cl_write_console_line("    (%1,%2)", (*it)->x, (*it)->y );
		}
		cl_write_console_line("");
*/

		tri_count++;

	}

	// cl_write_console_line("num triangles - final: %1", tri_count ); 

	return result;
}


void CL_EarClipTriangulator_Impl::begin_hole()
{
	target_array = &hole;
}

void CL_EarClipTriangulator_Impl::end_hole()
{
	create_lists(false);

	target_array = &vertices;

	// To be able to triangulate holes the inner and outer vertice arrays are connected
	// so that there isn't actually any hole, just a single polygon with a small gap
	// eliminating the hole.
	//
	// 1. find point on inner contour closest to a vertice on the outer contour.
	//
	// 2. Create bridge start and end points by offsetting the new vertices a bit along the edges to the next and prev vertices.
	// 

	LinkedVertice *outer_vertice = 0;
	LinkedVertice *segment_start;
	LinkedVertice *segment_end;
	CL_Pointf inner_point;
	float inner_point_rel;
	float distance = FLT_MAX;

	for (unsigned int vertex_cnt = 0; vertex_cnt < vertices.size(); vertex_cnt++)
	{
		CL_Pointf tmp_outer_point = CL_Pointf(vertices[vertex_cnt]->x,vertices[vertex_cnt]->y);

		for (unsigned int hole_cnt = 0; hole_cnt < hole.size(); hole_cnt++)
		{

			CL_Pointf tmp_line_start(hole[hole_cnt]->x, hole[hole_cnt]->y);
			CL_Pointf tmp_line_end(hole[hole_cnt]->next->x, hole[hole_cnt]->next->y);
			
			CL_Pointf tmp_inner_point = CL_LineMath::closest_point(tmp_outer_point, tmp_line_start, tmp_line_end);
			
			float tmp_distance = tmp_inner_point.distance(tmp_outer_point);
			
			if( tmp_distance < distance  )
			{
				inner_point_rel = CL_LineMath::closest_point_relative(tmp_outer_point, tmp_line_start, tmp_line_end);
				distance = tmp_distance;
				outer_vertice = vertices[vertex_cnt];
				inner_point = tmp_inner_point;
				segment_start = hole[hole_cnt];
				segment_end = hole[hole_cnt]->next;
			}
		}
	}

	LinkedVertice *outer_bridge_start = new LinkedVertice();
	LinkedVertice *outer_bridge_end = new LinkedVertice();
	LinkedVertice *inner_bridge_start = new LinkedVertice();
	LinkedVertice *inner_bridge_end = new LinkedVertice();

	//  offset new points along old edges
	CL_Pointf outer_point(outer_vertice->x, outer_vertice->y);

	set_bridge_vertice_offset( outer_bridge_start, outer_point, 0.0,             outer_vertice, outer_vertice->previous, 1 );
	set_bridge_vertice_offset( outer_bridge_end,   outer_point, 0.0,             outer_vertice, outer_vertice->next, 1 );
	set_bridge_vertice_offset( inner_bridge_start, inner_point, inner_point_rel, segment_start, segment_end, 1 );
	set_bridge_vertice_offset( inner_bridge_end,   inner_point, inner_point_rel, segment_start, segment_end, -1 );

	// update next pointers to ignore old vertices

	// connections between inner and outer contours
	outer_bridge_start->next = inner_bridge_start;
	inner_bridge_end->next = outer_bridge_end;

	// connections between new and old vertices: outer contour
	outer_bridge_end->next = outer_vertice->next;
	outer_vertice->previous->next = outer_bridge_start;

	// connections between new and old vertices: inner contour
	inner_bridge_start->next = segment_end;
	segment_start->next = inner_bridge_end;

	delete outer_vertice;
	outer_vertice = 0;

	if( inner_point_rel == 0.0 ) // if split point is at line end, remove inner vertex
	{
		segment_start->previous->next = inner_bridge_end;
		delete segment_start;
		segment_start = 0;
	}
	if( inner_point_rel == 1.0 ) // if split point is at line end, remove inner vertex
	{
		inner_bridge_start->next = segment_end->next;
		delete segment_end;
		segment_end = 0;
	}

	hole.clear();

	// rebuild the vector...
	vertices.clear();

	LinkedVertice *test = inner_bridge_start;
	do
	{
		vertices.push_back(test);
		test = test->next;
	} while( test != inner_bridge_start );

	// print the bridge start and end points:
/*	cl_write_console_line("outer_point: %1 %2", outer_point.x, outer_point.y );
	cl_write_console_line("inner_point: %1 %2", inner_point.x, inner_point.y );
	cl_write_console_line("inner_bridge_end: %1 %2", inner_bridge_end->x, inner_bridge_end->y );
	cl_write_console_line("outer_bridge_end: %1 %2", outer_bridge_end->x, outer_bridge_end->y );
	cl_write_console_line("inner_bridge_start: %1 %2", inner_bridge_start->x, inner_bridge_start->y );
	cl_write_console_line("outer_bridge_start: %1 %2", outer_bridge_start->x, outer_bridge_start->y );
*/
}

/////////////////////////////////////////////////////////////////////////////
// CL_EarClipTriangulator_Impl Implementation:


CL_PolygonOrientation CL_EarClipTriangulator_Impl::calculate_polygon_orientation()
{
	float sum = 0;

	unsigned int size = vertices.size();

	for( unsigned int i = 0; i < size; i++ )
	{
		LinkedVertice *p1 = vertices[ i ];
		LinkedVertice *p2 = vertices[ (i+1) % size ];

		sum += (p1->x*p2->y - p2->x*p1->y);
	}

	if( sum < 0.0 )
		return cl_counter_clockwise;

	return cl_clockwise;
}



void CL_EarClipTriangulator_Impl::create_lists(bool create_ear_list)
{
	int vert_size = vertices.size();

	for( int i=0; i < vert_size; ++i )
	{
		LinkedVertice *v = vertices[i];

		if( i == 0 )
			v->previous = vertices.back();
		else
			v->previous = vertices[i-1];

		if( i == (vert_size-1) )
			v->next = vertices.front();
		else
			v->next = vertices[i+1];
	}
 
	unsigned int hole_size = hole.size();

	for( unsigned int j = 0; j < hole.size(); ++j )
	{
		if( j == 0 )
			hole[j]->previous = hole.back();
		else
			hole[j]->previous = hole[j-1];
 
		if( j == (hole_size-1) )
			hole[j]->next = hole.front();
		else
			hole[j]->next = hole[j+1];
	}

	if( create_ear_list )
	{
//		cl_write_console_line("Ear list:");

		for( std::vector<LinkedVertice*>::iterator it = vertices.begin(); it != vertices.end(); ++it )
		{
			if( is_ear(*(*it)) )
			{
				ear_list.push_back((*it));
				(*it)->is_ear = true;

//				cl_write_console_line(cl_format("    (%1,%2)", (*it)->x, (*it)->y ) );
			}

		}
	}
}

bool CL_EarClipTriangulator_Impl::is_ear(const LinkedVertice &v)
{
	if( is_reflex(v) ) return false;

	CL_Trianglef triangle( CL_Pointf(v.x, v.y), CL_Pointf(v.next->x, v.next->y), CL_Pointf(v.previous->x, v.previous->y) );

	LinkedVertice *v_check = v.next->next;

	while( v_check != v.previous )
	{
		if( v_check == v.next || v_check == v.previous )
		{
			v_check = v_check->next;
			continue;
		}

		if( triangle.point_inside( CL_Pointf(v_check->x, v_check->y) ) )
			return false;

		v_check = v_check->next;
	}

	return true;
}

bool CL_EarClipTriangulator_Impl::is_reflex(const LinkedVertice &v)
{
	float x = v.x;
	float y = v.y;
	float nx = v.next->x;
	float ny = v.next->y;
	float px = v.previous->x;
	float py = v.previous->y;

	// Taking the crossproduct (v-1,v)x(v,v+1) tells if the edges sharing vertex v, are concave or convex.

	if( orientation == cl_clockwise )
	{
		if ( ((x-px)*(ny-y) - (y-py)*(nx-x)) < FLT_EPSILON ) 
			return true;
	}
	else
	{
		if ( ((x-px)*(ny-y) - (y-py)*(nx-x)) > FLT_EPSILON ) 
			return true;
	}

	return false;
}


void CL_EarClipTriangulator_Impl::set_bridge_vertice_offset(
	LinkedVertice *target,
	CL_Pointf split_point,
	float split_point_rel,
	LinkedVertice *segment_start,
	LinkedVertice *segment_end,
	int direction )
{
	if( direction == -1 && split_point_rel == 0.0 )
	{
		LinkedVertice *tmp = segment_start;
		segment_start = segment_start->previous;
		segment_end = tmp;
	}
	else if( direction == 1 && split_point_rel == 1.0 )
	{
		segment_start = segment_end;
		segment_end = segment_end->next;
	}

	while( segment_start->x == segment_end->x && segment_start->y == segment_end->y )
	{
		// Sometimes there are multiple points at exactly the same location. (Bug in font?)
		if( direction == 1 )
		{
			segment_start = segment_end;
			segment_end = segment_end->next;
		}
		else
		{
			segment_end = segment_start;
			segment_start = segment_start->previous;
		}
	}

	float dir_x = direction * (segment_end->x - segment_start->x);
	float dir_y = direction * (segment_end->y - segment_start->y);

	// normalize the vector
	float len = sqrt(dir_x*dir_x+dir_y*dir_y);

	if( len != 0 )  // if len is zero something is wrong (duplicated end points on cyclic curve?)
	{
		dir_x /= len;
		dir_y /= len;
	}
	else
	{
		throw CL_Exception("Error: CL_EarClipTriangulator: division by zero in function set_bridge_vertice_offset. Duplicated end points on a cyclic curve?");
	}
	
	// Changed from 0.01f to 0.001f (13 Jan 2009) to stop pixel sized gaps in a font [rombust]
	target->x = split_point.x + 0.001f * dir_x;
	target->y = split_point.y + 0.001f * dir_y;
}

