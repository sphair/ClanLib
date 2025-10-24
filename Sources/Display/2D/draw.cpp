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
**    Kenneth Gangstoe
**    Harry Storbacka
**    Ken Hirsch
*/

#include "Display/precomp.h"
#include "API/Display/2D/draw.h"
#include "API/Display/Render/texture.h"
#include "API/Display/2D/gradient.h"
#include "API/Display/Render/primitives_array.h"
#include "API/Display/Render/pen.h"
#include "Display/Render/graphic_context_impl.h"
#include "API/Core/Math/line_segment.h"
#include "API/Core/Math/quad.h"
#include "API/Core/Math/triangle_math.h"
#include "render_batch2d.h"

/////////////////////////////////////////////////////////////////////////////
// CL_Draw operations:

void CL_Draw::point(CL_GraphicContext &gc, float x1, float y1, const CL_Colorf &color)
{
	CL_Vec2f positions[1] =
	{
		CL_Vec2f(x1, y1)
	};

	CL_PrimitivesArray prim_array(gc);
	prim_array.set_attributes(0, positions);
	prim_array.set_attribute(1, color);
	gc.set_program_object(cl_program_color_only);
	gc.draw_primitives(cl_points, 1, prim_array);
	gc.reset_program_object();
}

void CL_Draw::point(CL_GraphicContext &gc, const CL_Pointf &p, const CL_Colorf &color)
{
	point(gc, p.x, p.y, color);
}

void CL_Draw::line(CL_GraphicContext &gc, float x1, float y1, float x2, float y2, const CL_Colorf &color)
{
	CL_Vec2f positions[2] =
	{
		CL_Vec2f(x1, y1),
		CL_Vec2f(x2, y2)
	};

	CL_PrimitivesArray prim_array(gc);
	prim_array.set_attributes(0, positions);
	prim_array.set_attribute(1, color);
	gc.set_program_object(cl_program_color_only);
	gc.draw_primitives(cl_lines, 2, prim_array);
	gc.reset_program_object();
}

void CL_Draw::line(CL_GraphicContext &gc, const CL_Pointf &start, const CL_Pointf &end, const CL_Colorf &color)
{
	line(gc, start.x, start.y, end.x, end.y, color);
}

void CL_Draw::line(CL_GraphicContext &gc, const CL_LineSegment2f &line_segment, const CL_Colorf &color)
{
	line(gc, line_segment.p.x, line_segment.p.y, line_segment.q.x, line_segment.q.y, color);
}

void CL_Draw::box(CL_GraphicContext &gc, float x1, float y1, float x2, float y2, const CL_Colorf &color)
{
	CL_Vec2f positions[4] =
	{
		CL_Vec2f(x1, y1),
		CL_Vec2f(x2, y1),
		CL_Vec2f(x2, y2),
		CL_Vec2f(x1, y2)
	};

	CL_PrimitivesArray prim_array(gc);
	prim_array.set_attributes(0, positions);
	prim_array.set_attribute(1, color);
	gc.set_program_object(cl_program_color_only);
	gc.draw_primitives(cl_line_loop, 4, prim_array);
	gc.reset_program_object();
}

void CL_Draw::box(CL_GraphicContext &gc, const CL_Pointf &start, const CL_Pointf &end, const CL_Colorf &color)
{
	box(gc, start.x, start.y, end.x, end.y, color);
}

void CL_Draw::box(CL_GraphicContext &gc, const CL_Rectf &rect, const CL_Colorf &color)
{
	box(gc, rect.left, rect.top, rect.right, rect.bottom, color);
}

void CL_Draw::fill(CL_GraphicContext &gc, float x1, float y1, float x2, float y2, const CL_Colorf &color)
{
	CL_RenderBatcherSprite *batcher = gc.impl->current_internal_batcher;
	batcher->fill(gc, x1, y1, x2, y2, color);
}

void CL_Draw::fill(CL_GraphicContext &gc, const CL_Pointf &start, const CL_Pointf &end, const CL_Colorf &color)
{
	fill(gc, start.x, start.y, end.x, end.y, color);
}

void CL_Draw::fill(CL_GraphicContext &gc, const CL_Rectf &rect, const CL_Colorf &color)
{
	fill(gc, rect.left, rect.top, rect.right, rect.bottom, color);
}

void CL_Draw::texture(
	CL_GraphicContext &gc,
	const CL_Rectf &rect,
	const CL_Colorf &color,
	const CL_Rectf &texture_unit1_coords)
{
	CL_Vec2f positions[6] =
	{
		CL_Vec2f(rect.left, rect.top),
		CL_Vec2f(rect.right, rect.top),
		CL_Vec2f(rect.left, rect.bottom),
		CL_Vec2f(rect.right, rect.top),
		CL_Vec2f(rect.left, rect.bottom),
		CL_Vec2f(rect.right, rect.bottom)
	};

	CL_Vec2f tex1_coords[6] =
	{
		CL_Vec2f(texture_unit1_coords.left, texture_unit1_coords.top),
		CL_Vec2f(texture_unit1_coords.right, texture_unit1_coords.top),
		CL_Vec2f(texture_unit1_coords.left, texture_unit1_coords.bottom),
		CL_Vec2f(texture_unit1_coords.right, texture_unit1_coords.top),
		CL_Vec2f(texture_unit1_coords.left, texture_unit1_coords.bottom),
		CL_Vec2f(texture_unit1_coords.right, texture_unit1_coords.bottom)
	};

	CL_PrimitivesArray prim_array(gc);
	prim_array.set_attributes(0, positions);
	prim_array.set_attribute(1, color);
	prim_array.set_attributes(2, tex1_coords);
	gc.set_program_object(cl_program_single_texture);
	gc.draw_primitives(cl_triangles, 6, prim_array);
	gc.reset_program_object();
}

void CL_Draw::texture(
	CL_GraphicContext &gc,
	const CL_Texture &texture,
	const CL_Quadf &quad,
	const CL_Colorf &color,
	const CL_Rectf &texture_unit1_coords)
{
	CL_Vec2f positions[6] =
	{
		CL_Vec2f(quad.p),
		CL_Vec2f(quad.q),
		CL_Vec2f(quad.s),
		CL_Vec2f(quad.q),
		CL_Vec2f(quad.s),
		CL_Vec2f(quad.r)
	};

	CL_Vec2f tex1_coords[6] =
	{
		CL_Vec2f(texture_unit1_coords.left, texture_unit1_coords.top),
		CL_Vec2f(texture_unit1_coords.right, texture_unit1_coords.top),
		CL_Vec2f(texture_unit1_coords.left, texture_unit1_coords.bottom),
		CL_Vec2f(texture_unit1_coords.right, texture_unit1_coords.top),
		CL_Vec2f(texture_unit1_coords.left, texture_unit1_coords.bottom),
		CL_Vec2f(texture_unit1_coords.right, texture_unit1_coords.bottom)
	};

	CL_PrimitivesArray prim_array(gc);
	prim_array.set_attributes(0, positions);
	prim_array.set_attribute(1, color);
	prim_array.set_attributes(2, tex1_coords);
	gc.set_texture(0, texture);
	gc.set_program_object(cl_program_single_texture);
	gc.draw_primitives(cl_triangles, 6, prim_array);
	gc.reset_program_object();
	gc.reset_texture(0);
}

void CL_Draw::gradient_fill(CL_GraphicContext &gc, float x1, float y1, float x2, float y2, const CL_Gradient &gradient)
{
	CL_Vec2f positions[6] =
	{
		CL_Vec2f(x1, y1),
		CL_Vec2f(x2, y1),
		CL_Vec2f(x1, y2),
		CL_Vec2f(x2, y1),
		CL_Vec2f(x1, y2),
		CL_Vec2f(x2, y2)
	};

	#define cl_color_to_color4d(c) c.get_red(), c.get_green(), c.get_blue(), c.get_alpha()

	CL_Vec4f colors[6] =
	{
		CL_Vec4f(cl_color_to_color4d(gradient.top_left)),
		CL_Vec4f(cl_color_to_color4d(gradient.top_right)),
		CL_Vec4f(cl_color_to_color4d(gradient.bottom_left)),
		CL_Vec4f(cl_color_to_color4d(gradient.top_right)),
		CL_Vec4f(cl_color_to_color4d(gradient.bottom_left)),
		CL_Vec4f(cl_color_to_color4d(gradient.bottom_right))
	};

	CL_PrimitivesArray prim_array(gc);
	prim_array.set_attributes(0, positions);
	prim_array.set_attributes(1, colors);
	gc.set_program_object(cl_program_color_only);
	gc.draw_primitives(cl_triangles, 6, prim_array);
	gc.reset_program_object();
}

void CL_Draw::gradient_fill(CL_GraphicContext &gc, const CL_Pointf &start, const CL_Pointf &end, const CL_Gradient &gradient)
{
	gradient_fill(gc, start.x, start.y, end.x, end.y, gradient);
}

void CL_Draw::gradient_fill(CL_GraphicContext &gc, const CL_Rectf &rect, const CL_Gradient &gradient)
{
	gradient_fill(gc, rect.left, rect.top, rect.right, rect.bottom, gradient);
}

void CL_Draw::circle(CL_GraphicContext &gc, float center_x, float center_y, float radius, const CL_Colorf &color)
{
	gradient_circle(gc, CL_Pointf(center_x, center_y), CL_Pointf(center_x, center_y), radius, CL_Gradient(color, color));
}

void CL_Draw::circle(CL_GraphicContext &gc, const CL_Pointf &center, float radius, const CL_Colorf &color)
{
	gradient_circle(gc, center, center, radius, CL_Gradient(color, color));
}

void CL_Draw::gradient_circle(CL_GraphicContext &gc, const CL_Pointf &center, float radius, const CL_Gradient &gradient)
{
	gradient_circle(gc, center, center, radius, gradient);
}

void CL_Draw::gradient_circle(CL_GraphicContext &gc, const CL_Pointf &center, const CL_Pointf &centergradient, float radius, const CL_Gradient &gradient)
{
	float offset_x = 0;
	float offset_y = 0;

	if(radius < 4)
		radius = 4;

	float rotationcount = (radius - 3);
	float halfpi = 1.5707963267948966192313216916398f;
	float turn = halfpi / rotationcount;

	if(center.distance(center + centergradient) < radius)
	{
		offset_x = centergradient.x;
		offset_y = -centergradient.y;
	}

	CL_Vec4f colors[3] =
	{
		CL_Vec4f(gradient.top_left.get_red(), gradient.top_left.get_green(), gradient.top_left.get_blue(), gradient.top_left.get_alpha()),
		CL_Vec4f(gradient.bottom_right.get_red(), gradient.bottom_right.get_green(), gradient.bottom_right.get_blue(), gradient.bottom_right.get_alpha()),
		CL_Vec4f(gradient.bottom_right.get_red(), gradient.bottom_right.get_green(), gradient.bottom_right.get_blue(), gradient.bottom_right.get_alpha())
	};

	CL_Vec4f triangle_colors[4*3];
	for (int i=0; i<3; i++)
	{
		triangle_colors[0*3+i] = colors[i];
		triangle_colors[1*3+i] = colors[i];
		triangle_colors[2*3+i] = colors[i];
		triangle_colors[3*3+i] = colors[i];
	}

	for(float i = 0; i < rotationcount ; i++)
	{
		float pos1 = cos(i * turn);
		float pos2 = sin(i * turn);
		float pos3 = cos((i+1) * turn);
		float pos4 = sin((i+1) * turn);

		CL_Vec2f positions[4*3] =
		{
			// 90 triangle:
			CL_Vec2f(center.x + offset_x , center.y + offset_y),
			CL_Vec2f(center.x + ((float)radius * pos1), center.y + ((float)radius * pos2)),
			CL_Vec2f(center.x + ((float)radius * pos3), center.y + ((float)radius * pos4)),

			// 0 triangle:
			CL_Vec2f(center.x + offset_x , center.y + offset_y),
			CL_Vec2f(center.x + ((float)radius * pos2), center.y - ((float)radius * pos1)),
			CL_Vec2f(center.x + ((float)radius * pos4), center.y - ((float)radius * pos3)),

			// 270 triangle:
			CL_Vec2f(center.x + offset_x , center.y + offset_y),
			CL_Vec2f(center.x - ((float)radius * pos1), center.y - ((float)radius * pos2)),
			CL_Vec2f(center.x - ((float)radius * pos3), center.y - ((float)radius * pos4)),

			// 180 triangle:
			CL_Vec2f(center.x + offset_x , center.y + offset_y),
			CL_Vec2f(center.x - ((float)radius * pos2), center.y + ((float)radius * pos1)),
			CL_Vec2f(center.x - ((float)radius * pos4), center.y + ((float)radius * pos3))
		};

		CL_PrimitivesArray prim_array(gc);
		prim_array.set_attributes(0, positions);
		prim_array.set_attributes(1, triangle_colors);
		gc.set_program_object(cl_program_color_only);
		gc.draw_primitives(cl_triangles, 4*3, prim_array);
		gc.reset_program_object();
	}
}

void CL_Draw::triangle(CL_GraphicContext &gc, const CL_Pointf &a, const CL_Pointf &b, const CL_Pointf &c, const CL_Colorf &color)
{
	CL_Vec2f positions[3] =
	{
		CL_Vec2f(a.x, a.y),
		CL_Vec2f(b.x, b.y),
		CL_Vec2f(c.x, c.y)
	};

	CL_PrimitivesArray prim_array(gc);
	prim_array.set_attributes(0, positions);
	prim_array.set_attribute(1, color);
	gc.set_program_object(cl_program_color_only);
	gc.draw_primitives(cl_triangles, 3, prim_array);
	gc.reset_program_object();
}

void CL_Draw::triangle(CL_GraphicContext &gc,  const CL_Trianglef &dest_triangle, const CL_Colorf &color)
{
	triangle(gc, dest_triangle.p, dest_triangle.q, dest_triangle.r, color);
}

