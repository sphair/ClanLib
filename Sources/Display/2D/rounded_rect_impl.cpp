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
*/

#include "Display/precomp.h"
#include "rounded_rect_impl.h"
#include "API/Core/Math/bezier_curve.h"
#include "API/Core/Math/cl_math.h"
#include "API/Display/2D/color.h"
#include "API/Display/2D/gradient.h"
#include "API/Display/Render/texture.h"
#include "API/Core/Math/ear_clip_result.h"
#include "API/Core/Math/ear_clip_triangulator.h"
#include "API/Core/Math/triangle_math.h"
#include "API/Display/2D/canvas.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// RoundedRect_Impl construction:

RoundedRect_Impl::RoundedRect_Impl() : 
	cp_tl(0,0),	cp_tr(1,0),	cp_bl(0,1),	cp_br(1,1),
	round_tl(-1.0,-1.0), round_tr(-1.0,-1.0), round_bl(-1.0,-1.0), round_br(-1.0,-1.0), outline_needs_update(true), triangulation_needs_update(true) 
{
}

RoundedRect_Impl::~RoundedRect_Impl()
{
}

/////////////////////////////////////////////////////////////////////////////
// RoundedRect_Impl attributes:

/////////////////////////////////////////////////////////////////////////////
// RoundedRect_Impl operations:

void RoundedRect_Impl::draw(Canvas &canvas, const Pointf &position, const Colorf &color, Origin origin)
{
	if( outline_needs_update )
		calculate_outline_points();

	if (!outline.empty())
	{
		Pointf origin_offset = Pointf::calc_origin(origin, size);
		canvas.push_translate(position.x - origin_offset.x, position.y - origin_offset.y);
		canvas.draw_line_strip(&outline[0], outline.size(), color);
		canvas.pop_modelview();
	}
}

void RoundedRect_Impl::fill(Canvas &canvas, const Pointf &position, const Colorf &color, Origin origin)
{
	if( outline_needs_update )
		calculate_outline_points();

	GraphicContext &gc = canvas.get_gc();

	if( triangulation_needs_update )
	{
		triangulate();

		num_vectors = triangle_positions.size();
		if (num_vectors)
		{
			std::vector<Vec4f> triangle_colors(num_vectors, color);
			gpu_positions = VertexArrayVector<Vec2f>(gc, &triangle_positions[0], num_vectors);
			gpu_colors = VertexArrayVector<Vec4f>(gc, &triangle_colors[0], num_vectors);

		}

	}

	if (num_vectors == 0)
		return;

	PrimitivesArray prim_array(gc);
	prim_array.set_attributes(0, gpu_positions);
	prim_array.set_attributes(1, gpu_colors);

	Pointf origin_offset = Pointf::calc_origin(origin, size);
	canvas.push_translate(position.x - origin_offset.x, position.y - origin_offset.y);
	canvas.set_program_object(program_color_only);
	gc.draw_primitives(type_triangles, (int) num_vectors, prim_array);
	gc.reset_program_object();
	canvas.pop_modelview();

}

void RoundedRect_Impl::fill(Canvas &canvas, const Pointf &position, const Gradient &new_gradient, Origin origin)
{
	if( outline_needs_update )
		calculate_outline_points();

	bool update_colors_flag = triangulation_needs_update;
	GraphicContext &gc = canvas.get_gc();

	if( triangulation_needs_update )
	{
		triangulate();

		num_vectors = triangle_positions.size();
		if (num_vectors)
		{
			gpu_positions = VertexArrayVector<Vec2f>(gc, &triangle_positions[0], num_vectors);
		}
	}

	if (num_vectors == 0)
		return;

	if( gradient != new_gradient || update_colors_flag )
	{
		std::vector<Vec4f> triangle_colors;

		gradient = new_gradient;
		triangle_colors.clear();
		triangle_colors.reserve(num_vectors);

		float min_x=99999999.0f, max_x=-99999999.0f, min_y=99999999.0f, max_y=-99999999.0f;
		for( unsigned int i=0; i<num_vectors; i++ )
		{
			Pointf P(triangle_positions[i].x, triangle_positions[i].y);
			if( P.x < min_x ) min_x = P.x;
			if( P.x > max_x ) max_x = P.x;
			if( P.y < min_y ) min_y = P.y;
			if( P.y > max_y ) max_y = P.y;
		}

		// gradient rectangle
		Rectf grect(min_x, min_y, max_x, max_y);
			
		Trianglef triangle1_vert( Pointf(min_x, min_y), Pointf(max_x, min_y), Pointf(max_x, max_y) );
		Trianglef triangle2_vert( Pointf(max_x, max_y), Pointf(min_x, max_y), Pointf(min_x, min_y) );

		for( unsigned int i=0;  i < num_vectors; i++ )
		{
			Colorf color;
			Pointf P(triangle_positions[i].x, triangle_positions[i].y);

			if( triangle1_vert.point_inside(P) )
			{
				color = point_on_triangle_to_color( triangle1_vert, gradient.top_left, gradient.top_right, gradient.bottom_right, P );
			}
			else
			{
				color = point_on_triangle_to_color( triangle2_vert, gradient.bottom_right, gradient.bottom_left, gradient.top_left, P );
			}

			Vec4f vertex_color;
			vertex_color.r = color.get_red();
			vertex_color.g = color.get_green();
			vertex_color.b = color.get_blue();
			vertex_color.a = color.get_alpha();
			triangle_colors.push_back(vertex_color);
		}

		gpu_colors = VertexArrayVector<Vec4f>(gc, &triangle_colors[0], num_vectors);
	}


	PrimitivesArray prim_array(gc);
	prim_array.set_attributes(0, gpu_positions);
	prim_array.set_attributes(1, gpu_colors);

	Pointf origin_offset = Pointf::calc_origin(origin, size);
	canvas.push_translate(position.x - origin_offset.x, position.y - origin_offset.y);
	canvas.set_program_object(program_color_only);
	gc.draw_primitives(type_triangles, (int) num_vectors, prim_array);
	gc.reset_program_object();
	canvas.pop_modelview();
}

/////////////////////////////////////////////////////////////////////////////
// RoundedRect_Impl implementation:

void RoundedRect_Impl::triangulate()
{
	EarClipTriangulator triangulator;

	for (unsigned int cnt = 0; cnt < outline.size(); cnt++)
	{
		triangulator.add_vertex(outline[cnt].x, outline[cnt].y);
	}

	EarClipResult result = triangulator.triangulate();

	triangle_positions.clear();

	std::vector<EarClipTriangulator_Triangle> &result_triangles = result.get_triangles();

	for (unsigned int cnt = 0; cnt < result_triangles.size(); cnt++)
	{
		Vec2f positions[3] =
		{
			Vec2f(result_triangles[cnt].x1, result_triangles[cnt].y1),
			Vec2f(result_triangles[cnt].x2, result_triangles[cnt].y2),
			Vec2f(result_triangles[cnt].x3, result_triangles[cnt].y3)
		};

		triangle_positions.push_back(positions[0]);
		triangle_positions.push_back(positions[1]);
		triangle_positions.push_back(positions[2]);
	}

	triangulation_needs_update = false;
}

void RoundedRect_Impl::calculate_outline_points()
{ 
	outline.clear();

	Sizef unmodified(-1,-1);

	// sanitize rounding values
	float tmp_rounding = rounding;
	float min_rounding = min(size.width/2.0f, size.height/2.0f);
	if( tmp_rounding >= (min_rounding-0.01f) ) // 0.01: hysterezis for floating point errors
	{
		tmp_rounding = min_rounding-0.01f; // avoid duplicating curve endpoints 
	}

	// top right curve
	BezierCurve bez_tr;
	if( round_tr != unmodified)
	{
		bez_tr.add_control_point(size.width*round_tr.width, 0.0f);
		bez_tr.add_control_point(Pointf(cp_tr.x*size.width, cp_tr.y*size.height));
		bez_tr.add_control_point(size.width, round_tr.height*size.height);
	}
	else
	{
		bez_tr.add_control_point(size.width-tmp_rounding, 0.0);
		bez_tr.add_control_point(Pointf(size.width, 0));
		bez_tr.add_control_point(size.width, tmp_rounding);
	}

	// bottom right curve
	BezierCurve bez_br;
	if( round_br != unmodified)
	{
		bez_br.add_control_point(size.width, round_br.height * size.height);
		bez_br.add_control_point(Pointf(cp_br.x*size.width, cp_br.y*size.height));
		bez_br.add_control_point(size.width*round_br.width, size.height);
	}
	else
	{
		bez_br.add_control_point(size.width, size.height-tmp_rounding);
		bez_br.add_control_point(Pointf(size.width, size.height));
		bez_br.add_control_point(size.width-tmp_rounding, size.height);
	}

	// bottom left curve
	BezierCurve bez_bl;
	if( round_bl != unmodified)
	{
		bez_bl.add_control_point(size.width*round_bl.width, size.height);
		bez_bl.add_control_point(Pointf(cp_bl.x*size.width, cp_bl.y*size.height));
		bez_bl.add_control_point(0, round_bl.height*size.height);
	}
	else
	{
		bez_bl.add_control_point(tmp_rounding, size.height);
		bez_bl.add_control_point(Pointf(0, size.height));
		bez_bl.add_control_point(0, size.height-tmp_rounding);
	}

	// top left curve
	BezierCurve bez_tl;
	if( round_tl != unmodified)
	{
		bez_tl.add_control_point(0, round_tl.height*size.height);
		bez_tl.add_control_point(Pointf(cp_tl.x*size.width, cp_tl.y*size.height));
		bez_tl.add_control_point(size.width*round_tl.width, 0);
	}
	else
	{
		bez_tl.add_control_point(0, tmp_rounding);
		bez_tl.add_control_point(Pointf(0, 0));
		bez_tl.add_control_point(tmp_rounding, 0);
	}
	
	std::vector<Pointf> points;

	points = bez_tr.generate_curve_points(Angle::from_degrees(10));
	outline.insert(outline.end(), points.begin(), points.end());

	points = bez_br.generate_curve_points(Angle::from_degrees(10));
	outline.insert(outline.end(), points.begin(), points.end());

	points = bez_bl.generate_curve_points(Angle::from_degrees(10));
	outline.insert(outline.end(), points.begin(), points.end());

	points = bez_tl.generate_curve_points(Angle::from_degrees(10));
	outline.insert(outline.end(), points.begin(), points.end());

	outline.push_back(outline[0]);

	outline_needs_update = false;
}

Colorf RoundedRect_Impl::point_on_triangle_to_color( 
	const Trianglef &triangle,
	const Colorf &c1, const Colorf &c2, const Colorf &c3,
	const Pointf &P )
{
	Pointf Q = simultaneous_2(
		(triangle.q.x-triangle.p.x), (triangle.r.x-triangle.p.x), (P.x-triangle.p.x),
		(triangle.q.y-triangle.p.y), (triangle.r.y-triangle.p.y), (P.y-triangle.p.y) );
	
	Colorf c;
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

float RoundedRect_Impl::determinant_2x2(float a, float b, float c, float d)
{
	return a*d - b*c;
}

Pointf RoundedRect_Impl::simultaneous_2(
	float a1, float b1, float c1,
	float a2, float b2, float c2 )
{

	float D = determinant_2x2(a1,b1,a2,b2);
	float Dx = determinant_2x2(c1,b1,c2,b2);
	float Dy = determinant_2x2(a1,c1,a2,c2);

	return Pointf( Dx/D, Dy/D );
}

}
