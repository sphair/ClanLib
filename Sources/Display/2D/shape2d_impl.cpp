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
**    Harry Storbacka
**    Mark Page
*/

#include "Display/precomp.h"
#include "shape2d_impl.h"
#include "API/Core/Math/ear_clip_result.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/Font/font_vector.h"
#include <vector>
#include "API/Core/Math/bezier_curve.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// Shape2D_Impl Construction:

Shape2D_Impl::Shape2D_Impl()
{
}

Shape2D_Impl::~Shape2D_Impl()
{
}

/////////////////////////////////////////////////////////////////////////////
// Shape2D_Impl Attributes:


/////////////////////////////////////////////////////////////////////////////
// Shape2D_Impl Operations:

void Shape2D_Impl::add_path(Path2D &path)
{
	contours.push_back(path);
}

void Shape2D_Impl::get_triangles(std::vector<Vec2f> &out_primitives_array, PolygonOrientation polygon_orientation) const
{
	//std::vector< std::vector<Pointf> > *out_joined_outlines = NULL;	

	std::vector<EarClipResult> earclip_results;

	EarClipTriangulator triangulator;


	// Create the hole cache
	std::vector< bool > contour_empty;
	std::vector< bool > contour_hole_cache;
	contour_empty.resize(contours.size());
	contour_hole_cache.resize(contours.size());
	for( size_t cnt=0; cnt < contours.size(); cnt++ )
	{
		contour_empty[cnt] = contours[cnt].get_contour_points().empty();
		if( contour_empty[cnt] )
			continue;

		contour_hole_cache[cnt] = contours[cnt].is_hole(polygon_orientation);
			continue;
	}

	// sort contour vector so that it has the ordering:
	// outline_1, ouline_1_hole_1, ouline_1_hole_2, outline_2, ouline_2_hole_1 etc.

	std::vector< Path2D > sorted_contours;
	std::vector< bool > sorted_hole_cache;

	int outline_count = 0;

	unsigned int i;
	for( i=0; i < contours.size(); ++i )
	{
		if( contour_empty[i] )
			continue;

		if( contour_hole_cache[i] )
			continue;

		outline_count++;

		sorted_contours.push_back(contours[i]);
		sorted_hole_cache.push_back(false);

		for( unsigned int j=0; j < contours.size(); ++j )
		{
			if( contour_empty[j] )
				continue;

			if( contour_hole_cache[j] && contours[j].is_inside_contour(contours[i]) )
			{
				sorted_contours.push_back(contours[j]);
				sorted_hole_cache.push_back(true);
				contour_empty[j] = true;
			}
		}

		contour_empty[i] = true;
	}

//	cl_write_console_line(string_format("num outlines: %1", outline_count));
//	cl_write_console_line(string_format("num holes: %1", (int)(contours.size() - outline_count)));

//	cl_write_console_line("num sorted_contours: %1", (int)sorted_contours.size());

	for( i=0; i < sorted_contours.size(); ++i )
	{
		if( sorted_hole_cache[i] )
		{
			triangulator.begin_hole();
		}

		const std::vector<Pointf> &cpoints = sorted_contours[i].get_contour_points();
		unsigned int num_points = cpoints.size();

		for( unsigned int p=0; p<num_points; ++p )
		{
			triangulator.add_vertex(cpoints[p]);
		}
 
		if( sorted_contours[i].is_hole(polygon_orientation) )
		{
			triangulator.end_hole();
		}

		// if there are still sorted_contours left, but the next contour isn't a hole, then triangulate
		// what has been added to the triangulator so far. Then clear it to start a new contour.
		if( (i+1 < sorted_contours.size()) && (sorted_hole_cache[i+1] == false) )
		{
			triangulator.set_orientation(polygon_orientation);

			// for debugging triangulator hole support - don't remove!
			//if (out_joined_outlines)
			//	out_joined_outlines->push_back( triangulator.get_vertices() ); // debug

			EarClipResult result = triangulator.triangulate();
			earclip_results.push_back(result);
			triangulator.clear();
		}
	}

	triangulator.set_orientation(polygon_orientation);

	// for debugging triangulator hole support - don't remove!
	//if (out_joined_outlines)
	//	out_joined_outlines->push_back( triangulator.get_vertices() ); // debug

	EarClipResult result = triangulator.triangulate();
	earclip_results.push_back(result);

	int total_triangle_count = 0;

	for( i=0; i<earclip_results.size(); i++ )
	{
		total_triangle_count += earclip_results[i].get_triangles().size();	
	}

	out_primitives_array.clear();
	std::vector<Vec2f> &prim_array = out_primitives_array;

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
				prim_array[index] = Vec2f(triangles[v].x3, triangles[v].y3);
				prim_array[index+1] = Vec2f(triangles[v].x2, triangles[v].y2);
				prim_array[index+2] = Vec2f(triangles[v].x1, triangles[v].y1);
				index+=3;
			}
			else
			{
				prim_array[index] = Vec2f(triangles[v].x1, triangles[v].y1);
				prim_array[index+1] = Vec2f(triangles[v].x2, triangles[v].y2);
				prim_array[index+2] = Vec2f(triangles[v].x3, triangles[v].y3);
				index+=3;
			}
		}
	}

}

void Shape2D_Impl::get_outline(std::vector< std::vector<Vec2f> > &out_primitives_array_outline) const
{
	out_primitives_array_outline.resize(contours.size());

	for( size_t contours_index = 0; contours_index < contours.size(); contours_index++ )
	{
		const std::vector<Pointf> &points = contours[contours_index].get_contour_points();
		out_primitives_array_outline[contours_index].resize( points.size()+1);

		for( size_t points_index = 0; points_index < points.size(); points_index++ )
		{
			out_primitives_array_outline[contours_index][points_index] = Vec2f( points[points_index].x, points[points_index].y );
		}

		if (!points.empty())
			out_primitives_array_outline[contours_index][points.size()] = Vec2f(points.front().x, points.front().y);
	}
}

void Shape2D_Impl::add_rotated_curve(Path2D &path, const Pointf &center, const Angle &angle, Pointf point_1,  Pointf point_2,  Pointf point_3)
{
	if (angle.to_radians() != 0.0f)
	{
		point_1.rotate(center, angle);
		point_2.rotate(center, angle);
		point_3.rotate(center, angle);
	}
	BezierCurve curve;
	curve.add_control_point(point_1);
	curve.add_control_point(point_2);
	curve.add_control_point(point_3);
	path.add_curve(curve);

}

/////////////////////////////////////////////////////////////////////////////
// Shape2D_Impl Implementation:

/*
void Shape2D_Impl::draw_debug_outline(Canvas &canvas)
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
