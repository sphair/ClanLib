/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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

#include "Display/precomp.h"

#include "glyph_outline.h"
#include "API/Core/Math/ear_clip_result.h"
#include "API/Display/2D/draw.h"
#include "API/Display/Font/font_vector.h"
#include <vector>

/////////////////////////////////////////////////////////////////////////////
// CL_GlyphOutline Construction:

CL_GlyphOutline::CL_GlyphOutline() : prim_array(0), prim_array_outline(0)
{
}

CL_GlyphOutline::~CL_GlyphOutline()
{
	delete prim_array;
	delete prim_array_outline;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GlyphOutline Attributes:

CL_GlyphPrimitivesArray &CL_GlyphOutline::get_triarray()
{
	return *prim_array;
}

CL_GlyphPrimitivesArrayOutline &CL_GlyphOutline::get_outline()
{
	return *prim_array_outline;
}


/////////////////////////////////////////////////////////////////////////////
// CL_GlyphOutline Operations:

void CL_GlyphOutline::add_contour(CL_GlyphContour *contour)
{
	contours.push_back(contour);
}

void CL_GlyphOutline::triangulate()
{
	generate_contour_prim_array();

	std::vector<CL_EarClipResult> earclip_results;

	CL_EarClipTriangulator triangulator;

	unsigned int i;

	// sort contour vector so that it has the ordering:
	// outline_1, ouline_1_hole_1, ouline_1_hole_2, outline_2, ouline_2_hole_1 etc.

	std::vector<CL_GlyphContour*> sorted_contours;

	int outline_count = 0;

	for( i=0; i < contours.size(); ++i )
	{
		if( contours[i] == 0 )
			continue;

		if( contours[i]->is_hole() )
			continue;

		outline_count++;

		sorted_contours.push_back(contours[i]);

		for( unsigned int j=0; j < contours.size(); ++j )
		{
			if( contours[j] == 0 ) continue;

			if( contours[j]->is_hole() && contours[j]->is_inside_contour(*contours[i]) )
			{
				sorted_contours.push_back(contours[j]);
				contours[j] = 0;
			}
		}

		contours[i] = 0;
	}

//	cl_write_console_line(cl_format("num outlines: %1", outline_count));
//	cl_write_console_line(cl_format("num holes: %1", (int)(contours.size() - outline_count)));

	contours = sorted_contours;

//	cl_write_console_line("num contours: %1", (int)contours.size());

	for( i=0; i < contours.size(); ++i )
	{
		if( contours[i]->is_hole() )
		{
			triangulator.begin_hole();
		}

		const std::vector<CL_Pointf> &cpoints = contours[i]->get_contour_points();
		unsigned int num_points = cpoints.size();

		for( unsigned int p=0; p<num_points; ++p )
		{
			triangulator.add_vertex(cpoints[p]);
		}
 
		if( contours[i]->is_hole() )
		{
			triangulator.end_hole();
		}

		// if there are still contours left, but the next contour isn't a hole, then triangulate
		// what has been added to the triangulator so far. Then clear it to start a new contour.
		if( (i+1 < contours.size()) && (contours[i+1]->is_hole() == false) )
		{
			triangulator.set_orientation(cl_counter_clockwise);

			// for debugging triangulator hole support - don't remove!
			joined_outlines.push_back( triangulator.get_vertices() ); // debug

			CL_EarClipResult result = triangulator.triangulate();
			earclip_results.push_back(result);
			triangulator.clear();
		}
	}

	triangulator.set_orientation(cl_counter_clockwise);

	// for debugging triangulator hole support - don't remove!
	joined_outlines.push_back( triangulator.get_vertices() ); // debug

	CL_EarClipResult result = triangulator.triangulate();
	earclip_results.push_back(result);

	int total_triangle_count = 0;

	for( i=0; i<earclip_results.size(); i++ )
	{
		total_triangle_count += earclip_results[i].get_triangles().size();	
	}

	prim_array = new CL_GlyphPrimitivesArray(total_triangle_count);
	int index = 0;

	for( i=0; i<earclip_results.size(); i++ )
	{
		std::vector<CL_EarClipTriangulator_Triangle> &triangles = earclip_results[i].get_triangles();

		for( unsigned int v=0; v < triangles.size(); ++v )	
		{
			// Normalize triangle (make it draw top left to bottom right) while inserting:
			if (triangles[v].x1 > triangles[v].x2 ||
				(triangles[v].x1 == triangles[v].y2 && triangles[v].y1 < triangles[v].y2))
			{
				prim_array->vertex[index] = CL_Vec2f(triangles[v].x3, -triangles[v].y3);
				prim_array->vertex[index+1] = CL_Vec2f(triangles[v].x2, -triangles[v].y2);
				prim_array->vertex[index+2] = CL_Vec2f(triangles[v].x1, -triangles[v].y1);
				index+=3;
			}
			else
			{
				prim_array->vertex[index] = CL_Vec2f(triangles[v].x1, -triangles[v].y1);
				prim_array->vertex[index+1] = CL_Vec2f(triangles[v].x2, -triangles[v].y2);
				prim_array->vertex[index+2] = CL_Vec2f(triangles[v].x3, -triangles[v].y3);
				index+=3;
			}
		}
	}

	// We got the primitive arrays now so get rid of the contours.
	for( std::vector<CL_GlyphContour*>::iterator it = contours.begin(); it != contours.end(); ++it )
	{
		delete (*it);
	}
	contours.clear();
}

/////////////////////////////////////////////////////////////////////////////
// CL_GlyphOutline Implementation:

void CL_GlyphOutline::generate_contour_prim_array()
{
	if( contours.empty() )
	{
		prim_array_outline = new CL_GlyphPrimitivesArrayOutline(0);
		return;
	}
	
	int size = contours.front()->get_contour_points().size();
	prim_array_outline = new CL_GlyphPrimitivesArrayOutline(size);
	
	std::vector<CL_GlyphContour*>::iterator it;

	for( it = contours.begin(); it != contours.end(); ++it )
	{
		if( it != contours.begin() )
			prim_array_outline->new_subarray((*it)->get_contour_points().size());

		const std::vector<CL_Pointf> &points = (*it)->get_contour_points();

		int subarray_index = prim_array_outline->vertex.size() - 1;

		int index = 0;
		for( std::vector<CL_Pointf>::const_iterator it2 = points.begin(); it2 != points.end(); ++it2 )
		{
			prim_array_outline->vertex[subarray_index][index] = CL_Vec2f( (*it2).x, -(*it2).y );
			index++;
		}

		if (index == 0)
			throw CL_Exception("A glyph outline contour does not contain any points");

		prim_array_outline->vertex[subarray_index][index] = CL_Vec2f(points.front().x, -points.front().y);
	}
}

void CL_GlyphOutline::draw_debug_outline(CL_GraphicContext &gc)
{
	for( unsigned int out=0; out<joined_outlines.size(); ++out )
	{
		std::vector<CL_Pointf> &pts = joined_outlines[out];

		for( unsigned int i=1; i<pts.size(); ++i )
		{
			CL_Draw::line( gc, pts[i-1].x, -pts[i-1].y, pts[i].x, -pts[i].y, CL_Colorf::seagreen );
			CL_Draw::point( gc, pts[i].x, -pts[i].y, CL_Colorf::white );
		}

		CL_Draw::line( gc, pts.back().x, -pts.back().y, pts.front().x, -pts.front().y, CL_Colorf::red );
		CL_Draw::point( gc, pts.back().x, -pts.back().y, CL_Colorf::white );
	}
}
