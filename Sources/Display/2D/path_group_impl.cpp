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
**    Harry Storbacka
**    Mark Page
*/

#include "Display/precomp.h"
#include "path_group_impl.h"
#include "API/Core/Math/ear_clip_result.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/Font/font_vector.h"
#include <vector>

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// PathGroup_Impl Construction:

PathGroup_Impl::PathGroup_Impl()
{
}

PathGroup_Impl::~PathGroup_Impl()
{
}

/////////////////////////////////////////////////////////////////////////////
// PathGroup_Impl Attributes:


/////////////////////////////////////////////////////////////////////////////
// PathGroup_Impl Operations:

void PathGroup_Impl::add_path(Path &path)
{
	contours.push_back(path);
}

void PathGroup_Impl::triangulate(PathPrimitivesArray *out_primitives_array, PathPrimitivesArrayOutline *out_primitives_array_outline, PathPrimitivesJoinedOutlines *out_joined_outlines)
{
	if (out_primitives_array_outline)
		generate_contour_prim_array(out_primitives_array_outline);	// Generate array_outline only when required

	if (!out_primitives_array && !out_joined_outlines)
		return;		// Exit now when done

	std::vector<EarClipResult> earclip_results;

	EarClipTriangulator triangulator;

	unsigned int i;

	// sort contour vector so that it has the ordering:
	// outline_1, ouline_1_hole_1, ouline_1_hole_2, outline_2, ouline_2_hole_1 etc.

	std::vector< Path > sorted_contours;

	int outline_count = 0;

	for( i=0; i < contours.size(); ++i )
	{
		if( contours[i].get_contour_points().empty() )
			continue;

		if( contours[i].is_hole() )
			continue;

		outline_count++;

		sorted_contours.push_back(contours[i]);

		for( unsigned int j=0; j < contours.size(); ++j )
		{
			if( contours[j].get_contour_points().empty() ) continue;

			if( contours[j].is_hole() && contours[j].is_inside_contour(contours[i]) )
			{
				sorted_contours.push_back(contours[j]);
				contours[j] = Path();
			}
		}

		contours[i] = Path();
	}

//	cl_write_console_line(string_format("num outlines: %1", outline_count));
//	cl_write_console_line(string_format("num holes: %1", (int)(contours.size() - outline_count)));

	contours = sorted_contours;

//	cl_write_console_line("num contours: %1", (int)contours.size());

	for( i=0; i < contours.size(); ++i )
	{
		if( contours[i].is_hole() )
		{
			triangulator.begin_hole();
		}

		const std::vector<Pointf> &cpoints = contours[i].get_contour_points();
		unsigned int num_points = cpoints.size();

		for( unsigned int p=0; p<num_points; ++p )
		{
			triangulator.add_vertex(cpoints[p]);
		}
 
		if( contours[i].is_hole() )
		{
			triangulator.end_hole();
		}

		// if there are still contours left, but the next contour isn't a hole, then triangulate
		// what has been added to the triangulator so far. Then clear it to start a new contour.
		if( (i+1 < contours.size()) && (contours[i+1].is_hole() == false) )
		{
			triangulator.set_orientation(cl_counter_clockwise);

			// for debugging triangulator hole support - don't remove!
			if (out_joined_outlines)
				out_joined_outlines->push_back( triangulator.get_vertices() ); // debug

			EarClipResult result = triangulator.triangulate();
			earclip_results.push_back(result);
			triangulator.clear();
		}
	}

	triangulator.set_orientation(cl_counter_clockwise);

	// for debugging triangulator hole support - don't remove!
	if (out_joined_outlines)
		out_joined_outlines->push_back( triangulator.get_vertices() ); // debug

	if (!out_primitives_array)
		return;			// Exit now if the primitives are not required

	EarClipResult result = triangulator.triangulate();
	earclip_results.push_back(result);

	int total_triangle_count = 0;

	for( i=0; i<earclip_results.size(); i++ )
	{
		total_triangle_count += earclip_results[i].get_triangles().size();	
	}

	*out_primitives_array = PathPrimitivesArray();
	PathPrimitivesArray &prim_array = *out_primitives_array;

	prim_array.resize(total_triangle_count * 3);
	int index = 0;

	for( i=0; i<earclip_results.size(); i++ )
	{
		std::vector<EarClipTriangulator_Triangle> &triangles = earclip_results[i].get_triangles();

		for( unsigned int v=0; v < triangles.size(); ++v )	
		{
			// Normalize triangle (make it draw top left to bottom right) while inserting:
			if (triangles[v].x1 > triangles[v].x2 ||
				(triangles[v].x1 == triangles[v].y2 && triangles[v].y1 < triangles[v].y2))
			{
				prim_array[index] = Vec2f(triangles[v].x3, -triangles[v].y3);
				prim_array[index+1] = Vec2f(triangles[v].x2, -triangles[v].y2);
				prim_array[index+2] = Vec2f(triangles[v].x1, -triangles[v].y1);
				index+=3;
			}
			else
			{
				prim_array[index] = Vec2f(triangles[v].x1, -triangles[v].y1);
				prim_array[index+1] = Vec2f(triangles[v].x2, -triangles[v].y2);
				prim_array[index+2] = Vec2f(triangles[v].x3, -triangles[v].y3);
				index+=3;
			}
		}
	}

}

/////////////////////////////////////////////////////////////////////////////
// PathGroup_Impl Implementation:

void PathGroup_Impl::generate_contour_prim_array(PathPrimitivesArrayOutline *out_primitives_array_outline)
{
	PathPrimitivesArrayOutline &prim_array_outline = *out_primitives_array_outline;

	if( contours.empty() )
	{
		prim_array_outline = PathPrimitivesArrayOutline();
		return;
	}
	
	int size = contours.front().get_contour_points().size();
	prim_array_outline = PathPrimitivesArrayOutline();
	prim_array_outline.resize(1);
	prim_array_outline[0].resize(size+1);
	
	std::vector< Path >::iterator it;

	for( it = contours.begin(); it != contours.end(); ++it )
	{
		if( it != contours.begin() )
		{
			std::vector<std::vector<Vec2f> >::size_type vertex_size = prim_array_outline.size();
			prim_array_outline.resize(vertex_size+1);
			prim_array_outline[vertex_size].resize((*it).get_contour_points().size()+1);
		}

		const std::vector<Pointf> &points = (*it).get_contour_points();

		int subarray_index = prim_array_outline.size() - 1;

		int index = 0;
		for( std::vector<Pointf>::const_iterator it2 = points.begin(); it2 != points.end(); ++it2 )
		{
			prim_array_outline[subarray_index][index] = Vec2f( (*it2).x, -(*it2).y );
			index++;
		}

		if (index == 0)
			throw Exception("A glyph outline contour does not contain any points");

		prim_array_outline[subarray_index][index] = Vec2f(points.front().x, -points.front().y);
	}
}

/*
void PathGroup_Impl::draw_debug_outline(Canvas &canvas)
{
	for( unsigned int out=0; out<joined_outlines.size(); ++out )
	{
		std::vector<Pointf> &pts = joined_outlines[out];

		for( unsigned int i=1; i<pts.size(); ++i )
		{
			canvas.draw_line(pts[i-1].x, -pts[i-1].y, pts[i].x, -pts[i].y, Colorf::seagreen );
			canvas.draw_point(pts[i].x, -pts[i].y, Colorf::white );
		}

		canvas.draw_line(pts.back().x, -pts.back().y, pts.front().x, -pts.front().y, Colorf::red );
		canvas.draw_point(pts.back().x, -pts.back().y, Colorf::white );
	}
}
*/

}
