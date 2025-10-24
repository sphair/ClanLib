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
#include "rounded_rect_impl.h"
#include "API/Core/Math/bezier_curve.h"
#include "API/Display/2D/draw.h"
#include "API/Display/2D/color.h"
#include "API/Display/2D/gradient.h"
#include "API/Display/Render/texture.h"
#include "API/Core/Math/ear_clip_result.h"
#include "API/Core/Math/ear_clip_triangulator.h"
#include "API/Core/Math/triangle_math.h"

#define cl_min(a, b) ((a) < (b) ? (a) : (b))

/////////////////////////////////////////////////////////////////////////////
// CL_RoundedRect_Impl construction:

CL_RoundedRect_Impl::CL_RoundedRect_Impl() : 
	cp_tl(0,0),	cp_tr(1,0),	cp_bl(0,1),	cp_br(1,1),
	round_tl(-1.0,-1.0), round_tr(-1.0,-1.0), round_bl(-1.0,-1.0), round_br(-1.0,-1.0), outline_needs_update(true), triangulation_needs_update(true) 
{
}

CL_RoundedRect_Impl::~CL_RoundedRect_Impl()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_RoundedRect_Impl attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_RoundedRect_Impl operations:

void CL_RoundedRect_Impl::draw(CL_GraphicContext &gc, const CL_Pointf &position, const CL_Colorf &color, CL_Origin origin)
{
	if( outline_needs_update )
		calculate_outline_points();

	CL_Pointf origin_offset = CL_Pointf::calc_origin(origin, size);

	gc.push_translate(position.x - origin_offset.x, position.y - origin_offset.y);

	for( unsigned int i=0; i<outline.size(); i++ )
	{
		// todo: add CL_StaticLineStripArray primitives array
		CL_Draw::line(gc, outline[i], outline[(i+1)%outline.size()], color);
	}

	gc.pop_modelview();
}

void CL_RoundedRect_Impl::fill(CL_GraphicContext &gc, const CL_Pointf &position, const CL_Colorf &color, CL_Origin origin)
{
	if( outline_needs_update )
		calculate_outline_points();

	if( triangulation_needs_update )
		triangulate();

	if (triangle_positions.empty())
		return;

	CL_PrimitivesArray prim_array(gc);
	prim_array.set_attributes(0, &triangle_positions[0]);
	prim_array.set_attribute(1, color);

	CL_Pointf origin_offset = CL_Pointf::calc_origin(origin, size);
	gc.push_translate(position.x - origin_offset.x, position.y - origin_offset.y);
	gc.set_program_object(cl_program_color_only);
	gc.draw_primitives(cl_triangles, (int) triangle_positions.size(), prim_array);
	gc.reset_program_object();
	gc.pop_modelview();
}

void CL_RoundedRect_Impl::fill(CL_GraphicContext &gc, const CL_Pointf &position, const CL_Gradient &new_gradient, CL_Origin origin)
{
	if( outline_needs_update )
		calculate_outline_points();

	if( triangulation_needs_update )
		triangulate();

	if (triangle_positions.empty())
		return;

	if( gradient != new_gradient || triangle_colors.size() != triangle_positions.size() )
	{
		gradient = new_gradient;
		triangle_colors.clear();
		triangle_colors.reserve(triangle_positions.size());

		int num_vertices = (int) triangle_positions.size();

		float min_x=99999999.0f, max_x=-99999999.0f, min_y=99999999.0f, max_y=-99999999.0f;
		for( int i=0; i<num_vertices; i++ )
		{
			CL_Pointf P(triangle_positions[i].x, triangle_positions[i].y);
			if( P.x < min_x ) min_x = P.x;
			if( P.x > max_x ) max_x = P.x;
			if( P.y < min_y ) min_y = P.y;
			if( P.y > max_y ) max_y = P.y;
		}

		// gradient rectangle
		CL_Rectf grect(min_x, min_y, max_x, max_y);
			
		CL_Trianglef triangle1_vert( CL_Pointf(min_x, min_y), CL_Pointf(max_x, min_y), CL_Pointf(max_x, max_y) );
		CL_Trianglef triangle2_vert( CL_Pointf(max_x, max_y), CL_Pointf(min_x, max_y), CL_Pointf(min_x, min_y) );

		for( int i=0;  i < num_vertices; i++ )
		{
			CL_Colorf color;
			CL_Pointf P(triangle_positions[i].x, triangle_positions[i].y);

			if( triangle1_vert.point_inside(P) )
			{
				color = point_on_triangle_to_color( triangle1_vert, gradient.top_left, gradient.top_right, gradient.bottom_right, P );
			}
			else
			{
				color = point_on_triangle_to_color( triangle2_vert, gradient.bottom_right, gradient.bottom_left, gradient.top_left, P );
			}

			CL_Vec4f vertex_color;
			vertex_color.r = color.get_red();
			vertex_color.g = color.get_green();
			vertex_color.b = color.get_blue();
			vertex_color.a = color.get_alpha();
			triangle_colors.push_back(vertex_color);
		}
	}

	CL_PrimitivesArray prim_array(gc);
	prim_array.set_attributes(0, &triangle_positions[0]);
	prim_array.set_attributes(1, &triangle_colors[0]);

	CL_Pointf origin_offset = CL_Pointf::calc_origin(origin, size);
	gc.push_translate(position.x - origin_offset.x, position.y - origin_offset.y);
	gc.set_program_object(cl_program_color_only);
	gc.draw_primitives(cl_triangles, (int) triangle_positions.size(), prim_array);
	gc.reset_program_object();
	gc.pop_modelview();
}

/////////////////////////////////////////////////////////////////////////////
// CL_RoundedRect_Impl implementation:

void CL_RoundedRect_Impl::triangulate()
{
	CL_EarClipTriangulator triangulator;

	for (unsigned int cnt = 0; cnt < outline.size(); cnt++)
	{
		triangulator.add_vertex(outline[cnt].x, outline[cnt].y);
	}

	CL_EarClipResult result = triangulator.triangulate();

	triangle_positions.clear();
	triangle_colors.clear();

	std::vector<CL_EarClipTriangulator_Triangle> &result_triangles = result.get_triangles();

	for (unsigned int cnt = 0; cnt < result_triangles.size(); cnt++)
	{
		CL_Vec2f positions[3] =
		{
			CL_Vec2f(result_triangles[cnt].x1, result_triangles[cnt].y1),
			CL_Vec2f(result_triangles[cnt].x2, result_triangles[cnt].y2),
			CL_Vec2f(result_triangles[cnt].x3, result_triangles[cnt].y3)
		};

		triangle_positions.push_back(positions[0]);
		triangle_positions.push_back(positions[1]);
		triangle_positions.push_back(positions[2]);
	}

	triangulation_needs_update = false;
}

void CL_RoundedRect_Impl::calculate_outline_points()
{ 
	outline.clear();

	CL_Sizef unmodified(-1,-1);

	// sanitize rounding values
	float tmp_rounding = rounding;
	float min_rounding = cl_min(size.width/2.0f, size.height/2.0f);
	if( tmp_rounding >= (min_rounding-0.01f) ) // 0.01: hysterezis for floating point errors
	{
		tmp_rounding = min_rounding-0.01f; // avoid duplicating curve endpoints 
	}

	// top right curve
	CL_BezierCurve bez_tr;
	if( round_tr != unmodified)
	{
		bez_tr.add_control_point(size.width*round_tr.width, 0.0f);
		bez_tr.add_control_point(CL_Pointf(cp_tr.x*size.width, cp_tr.y*size.height));
		bez_tr.add_control_point(size.width, round_tr.height*size.height);
	}
	else
	{
		bez_tr.add_control_point(size.width-tmp_rounding, 0.0);
		bez_tr.add_control_point(CL_Pointf(size.width, 0));
		bez_tr.add_control_point(size.width, tmp_rounding);
	}

	// bottom right curve
	CL_BezierCurve bez_br;
	if( round_br != unmodified)
	{
		bez_br.add_control_point(size.width, round_br.height * size.height);
		bez_br.add_control_point(CL_Pointf(cp_br.x*size.width, cp_br.y*size.height));
		bez_br.add_control_point(size.width*round_br.width, size.height);
	}
	else
	{
		bez_br.add_control_point(size.width, size.height-tmp_rounding);
		bez_br.add_control_point(CL_Pointf(size.width, size.height));
		bez_br.add_control_point(size.width-tmp_rounding, size.height);
	}

	// bottom left curve
	CL_BezierCurve bez_bl;
	if( round_bl != unmodified)
	{
		bez_bl.add_control_point(size.width*round_bl.width, size.height);
		bez_bl.add_control_point(CL_Pointf(cp_bl.x*size.width, cp_bl.y*size.height));
		bez_bl.add_control_point(0, round_bl.height*size.height);
	}
	else
	{
		bez_bl.add_control_point(tmp_rounding, size.height);
		bez_bl.add_control_point(CL_Pointf(0, size.height));
		bez_bl.add_control_point(0, size.height-tmp_rounding);
	}

	// top left curve
	CL_BezierCurve bez_tl;
	if( round_tl != unmodified)
	{
		bez_tl.add_control_point(0, round_tl.height*size.height);
		bez_tl.add_control_point(CL_Pointf(cp_tl.x*size.width, cp_tl.y*size.height));
		bez_tl.add_control_point(size.width*round_tl.width, 0);
	}
	else
	{
		bez_tl.add_control_point(0, tmp_rounding);
		bez_tl.add_control_point(CL_Pointf(0, 0));
		bez_tl.add_control_point(tmp_rounding, 0);
	}
	
	std::vector<CL_Pointf> points;

	points = bez_tr.generate_curve_points(CL_Angle::from_degrees(10));
	outline.insert(outline.end(), points.begin(), points.end());

	points = bez_br.generate_curve_points(CL_Angle::from_degrees(10));
	outline.insert(outline.end(), points.begin(), points.end());

	points = bez_bl.generate_curve_points(CL_Angle::from_degrees(10));
	outline.insert(outline.end(), points.begin(), points.end());

	points = bez_tl.generate_curve_points(CL_Angle::from_degrees(10));
	outline.insert(outline.end(), points.begin(), points.end());

	outline_needs_update = false;
}

CL_Colorf CL_RoundedRect_Impl::point_on_triangle_to_color( 
	const CL_Trianglef &triangle,
	const CL_Colorf &c1, const CL_Colorf &c2, const CL_Colorf &c3,
	const CL_Pointf &P )
{
	CL_Pointf Q = simultaneous_2(
		(triangle.q.x-triangle.p.x), (triangle.r.x-triangle.p.x), (P.x-triangle.p.x),
		(triangle.q.y-triangle.p.y), (triangle.r.y-triangle.p.y), (P.y-triangle.p.y) );
	
	CL_Colorf c;
	float r0 = c1.get_red();
	float r1 = c2.get_red();
	float r2 = c3.get_red();
	float g0 = c1.get_green();
	float g1 = c2.get_green();
	float g2 = c3.get_green();
	float b0 = c1.get_blue();
	float b1 = c2.get_blue();
	float b2 = c3.get_blue();
	float a0 = c1.get_alpha();
	float a1 = c2.get_alpha();
	float a2 = c3.get_alpha();

	c.set_red( r0+(Q.x*(r1-r0))+Q.y*(r2-r0) );
	c.set_green( g0+(Q.x*(g1-g0))+Q.y*(g2-g0) );
	c.set_blue( b0+(Q.x*(b1-b0))+Q.y*(b2-b0) );
	c.set_alpha( a0+(Q.x*(a1-a0))+Q.y*(a2-a0) );
	
	return c;
}

float CL_RoundedRect_Impl::determinant_2x2(float a, float b, float c, float d)
{
	return a*d - b*c;
}

CL_Pointf CL_RoundedRect_Impl::simultaneous_2(
	float a1, float b1, float c1,
	float a2, float b2, float c2 )
{

	float D = determinant_2x2(a1,b1,a2,b2);
	float Dx = determinant_2x2(c1,b1,c2,b2);
	float Dy = determinant_2x2(a1,c1,a2,c2);

	return CL_Pointf( Dx/D, Dy/D );
}
