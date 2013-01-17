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
**    Magnus Norddahl
**    Harry Storbacka
**    Kenneth Gangstoe
**    Mark Page
*/

#include "Display/precomp.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/2D/gradient.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/Render/primitives_array.h"
#include "API/Display/Window/display_window.h"
#include "API/Core/Math/line_segment.h"
#include "API/Core/Math/quad.h"
#include "API/Core/Math/triangle_math.h"
#include "render_batch_triangle.h"
#include "canvas_impl.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// Canvas Construction:

Canvas::Canvas()
{
}

Canvas::Canvas(GraphicContext &context) : impl(new Canvas_Impl(context))
{
	set_map_mode(map_2d_upper_left);

}

Canvas::Canvas(DisplayWindow &window) : impl(new Canvas_Impl(window.get_gc()))
{
	impl->display_window = window;
	set_map_mode(map_2d_upper_left);

}

Canvas::Canvas(DisplayWindowDescription &desc)
{
	DisplayWindow window(desc);
	*this = Canvas(window);
}

Canvas::Canvas(GraphicContext &context, FrameBuffer &framebuffer)
{
	GraphicContext new_context = context.create(framebuffer);
	*this = Canvas(new_context);
}

Canvas Canvas::create(FrameBuffer &framebuffer)
{
	flush();
	GraphicContext new_context = get_gc().create(framebuffer);
	return Canvas(new_context);
}

Canvas Canvas::create()
{
	flush();
	GraphicContext new_context = get_gc().create();
	Canvas new_canvas = Canvas(new_context);

	new_canvas.impl->display_window = impl->display_window;

	return new_canvas;
}

Canvas::~Canvas()
{
}

/////////////////////////////////////////////////////////////////////////////
// Canvas Attributes:

void Canvas::throw_if_null() const
{
	if (!impl)
		throw Exception("Canvas is null");
}

FontManager Canvas::get_font_manager() const
{
	return impl->font_manager;
}

GraphicContext &Canvas::get_gc() const
{
	return impl->get_gc();
}

const Mat4f &Canvas::get_modelview() const
{
	return impl->get_modelview();
}

const Mat4f &Canvas::get_projection() const
{
	return impl->get_projection();
}

Rect Canvas::get_cliprect() const
{
	if (!impl->cliprects.empty())
		return impl->cliprects.back();

	return Rect(0,0,get_width(),get_height());
}

PixelBuffer Canvas::get_pixeldata(const Rect &rect2, TextureFormat texture_format, bool clamp)
{
	flush();
	return get_gc().get_pixeldata(rect2, texture_format, clamp);
}

PixelBuffer Canvas::get_pixeldata(TextureFormat texture_format, bool clamp)
{
	flush();
	return get_gc().get_pixeldata(texture_format, clamp);
}

DisplayWindow Canvas::get_window() const
{
	return impl->display_window;
}

/////////////////////////////////////////////////////////////////////////////
// Canvas Operations:

void Canvas::set_rasterizer_state(const RasterizerState &state)
{
	flush();
	get_gc().set_rasterizer_state(state);
}

void Canvas::set_blend_state(const BlendState &state, const Vec4f &blend_color, unsigned int sample_mask)
{
	flush();
	get_gc().set_blend_state(state, blend_color, sample_mask);
}

void Canvas::set_depth_stencil_state(const DepthStencilState &state, int stencil_ref)
{
	flush();
	get_gc().set_depth_stencil_state(state, stencil_ref);
}

void Canvas::reset_rasterizer_state()
{
	flush();
	get_gc().reset_rasterizer_state();
}

void Canvas::reset_blend_state()
{
	flush();
	get_gc().reset_blend_state();
}

void Canvas::reset_depth_stencil_state()
{
	flush();
	get_gc().reset_depth_stencil_state();
}

void Canvas::set_cliprect(const Rect &rect)
{
	flush();
	impl->set_cliprect(rect);
}

void Canvas::push_cliprect(const Rect &rect)
{
	flush();
	impl->push_cliprect(rect);
}

void Canvas::push_cliprect()
{
	flush();
	impl->push_cliprect();
}

void Canvas::pop_cliprect()
{
	flush();
	impl->pop_cliprect();
}

void Canvas::reset_cliprect()
{
	flush();
	impl->reset_cliprect();
}

void Canvas::clear(const Colorf &color)
{
	flush();
	impl->clear(color);
}

void Canvas::set_program_object(StandardProgram standard_program)
{
	flush();
	get_gc().set_program_object(standard_program);
}

void Canvas::set_map_mode(MapMode mode)
{
	flush();
	impl->set_map_mode(mode);
}

void Canvas::set_projection(const Mat4f &matrix)
{
	impl->set_user_projection(matrix);
}

void Canvas::set_batcher(RenderBatcher *batcher)
{
	impl->set_batcher(*this, batcher);
}

void Canvas::flush()
{
	impl->flush();
}

void Canvas::set_modelview(const Mat4f &matrix)
{
	impl->set_modelview(matrix);
}

void Canvas::mult_modelview(const Mat4f &matrix)
{
	impl->set_modelview(get_modelview() * matrix);
}

void Canvas::push_modelview()
{
	impl->push_modelview(impl->get_modelview());
}

void Canvas::set_translate(float x, float y, float z)
{
	set_modelview(Mat4f::translate(x, y, z));
}

void Canvas::mult_translate(float x, float y, float z)
{
	mult_modelview(Mat4f::translate(x, y, z));
}

void Canvas::push_translate(float x, float y, float z)
{
	push_modelview();
	mult_translate(x, y, z);
}

void Canvas::set_rotate(const Angle &angle, float x, float y, float z, bool normalize)
{
	set_modelview(Mat4f::rotate(angle, x, y, z, normalize));
}

void Canvas::mult_rotate(const Angle &angle, float x, float y, float z, bool normalize)
{
	mult_modelview(Mat4f::rotate(angle, x, y, z, normalize));
}

void Canvas::push_rotate(const Angle &angle, float x, float y, float z)
{
	push_modelview();
	mult_rotate(angle, x, y, z);
}

void Canvas::set_scale(float x, float y, float z)
{
	Mat4f matrix = Mat4f::scale(x, y, z);
	set_modelview(matrix);
}

void Canvas::mult_scale(float x, float y, float z)
{
	Mat4f matrix = Mat4f::scale(x, y, z);
	mult_modelview(matrix);
}

void Canvas::push_scale(float x, float y, float z)
{
	push_modelview();
	mult_scale(x, y, z);
}

void Canvas::pop_modelview()
{
	impl->pop_modelview();
}

void Canvas::draw_point(float x1, float y1, const Colorf &color)
{
	Vec2f positions[1] =
	{
		Vec2f(x1, y1)
	};

	RenderBatchPoint *batcher = impl->get_point_batcher();
	batcher->draw_point(*this, positions, color, 1);
}

void Canvas::draw_point(const Pointf &p, const Colorf &color)
{
	draw_point(p.x, p.y, color);
}

void Canvas::draw_line(float x1, float y1, float x2, float y2, const Colorf &color)
{
	Vec2f positions[2] =
	{
		Vec2f(x1, y1),
		Vec2f(x2, y2)
	};

	RenderBatchLine *batcher = impl->get_line_batcher();
	batcher->draw_line_strip(*this, positions, color, 2);
}

void Canvas::draw_line(const Pointf &start, const Pointf &end, const Colorf &color)
{
	draw_line(start.x, start.y, end.x, end.y, color);
}

void Canvas::draw_line(const LineSegment2f &line_segment, const Colorf &color)
{
	draw_line(line_segment.p.x, line_segment.p.y, line_segment.q.x, line_segment.q.y, color);
}

void Canvas::draw_lines(Vec2f *positions, int num_vertices, const Colorf &color)
{
	RenderBatchLine *batcher = impl->get_line_batcher();
	batcher->draw_lines(*this, positions, color, num_vertices);
}

void Canvas::draw_lines(Vec2f *line_positions, Vec2f *texture_positions, int num_vertices, const Texture2D &texture, const Colorf &line_color)
{
	RenderBatchLineTexture *batcher = impl->get_line_texture_batcher();
	batcher->draw_lines(*this, line_positions, texture_positions, num_vertices, texture, line_color);
}

void Canvas::draw_line_strip(Vec2f *line_positions, int num_vertices, const Colorf &line_color)
{
	RenderBatchLine *batcher = impl->get_line_batcher();
	batcher->draw_line_strip(*this, line_positions, line_color, num_vertices);
}

void Canvas::draw_box(float x1, float y1, float x2, float y2, const Colorf &color)
{
	Vec2f positions[5] =
	{
		Vec2f(x1, y1),
		Vec2f(x2, y1),
		Vec2f(x2, y2),
		Vec2f(x1, y2),
		Vec2f(x1, y1),
	};

	RenderBatchLine *batcher = impl->get_line_batcher();
	batcher->draw_line_strip(*this, positions, color, 5);

}

void Canvas::draw_box(const Pointf &start, const Pointf &end, const Colorf &color)
{
	draw_box(start.x, start.y, end.x, end.y, color);
}

void Canvas::draw_box(const Rectf &rect, const Colorf &color)
{
	draw_box(rect.left, rect.top, rect.right, rect.bottom, color);
}

void Canvas::draw_fill(float x1, float y1, float x2, float y2, const Colorf &color)
{
	RenderBatchTriangle *batcher = impl->get_triangle_batcher();
	batcher->fill(*this, x1, y1, x2, y2, color);
}

void Canvas::draw_fill(const Pointf &start, const Pointf &end, const Colorf &color)
{
	draw_fill(start.x, start.y, end.x, end.y, color);
}

void Canvas::draw_fill(const Rectf &rect, const Colorf &color)
{
	draw_fill(rect.left, rect.top, rect.right, rect.bottom, color);
}

void Canvas::draw_gradient_fill(float x1, float y1, float x2, float y2, const Gradient &gradient)
{
	Vec2f positions[6] =
	{
		Vec2f(x1, y1),
		Vec2f(x2, y1),
		Vec2f(x1, y2),
		Vec2f(x2, y1),
		Vec2f(x1, y2),
		Vec2f(x2, y2)
	};

	Vec4f colors[6] =
	{
		Vec4f(gradient.top_left),
		Vec4f(gradient.top_right),
		Vec4f(gradient.bottom_left),
		Vec4f(gradient.top_right),
		Vec4f(gradient.bottom_left),
		Vec4f(gradient.bottom_right)
	};

	RenderBatchTriangle *batcher = impl->get_triangle_batcher();
	batcher->draw_triangle(*this, positions, colors, 6);

}

void Canvas::draw_gradient_fill(const Pointf &start, const Pointf &end, const Gradient &gradient)
{
	draw_gradient_fill(start.x, start.y, end.x, end.y, gradient);
}

void Canvas::draw_gradient_fill(const Rectf &rect, const Gradient &gradient)
{
	draw_gradient_fill(rect.left, rect.top, rect.right, rect.bottom, gradient);
}

void Canvas::draw_circle(float center_x, float center_y, float radius, const Colorf &color)
{
	draw_gradient_circle(Pointf(center_x, center_y), Pointf(center_x, center_y), radius, Gradient(color, color));
}

void Canvas::draw_circle(const Pointf &center, float radius, const Colorf &color)
{
	draw_gradient_circle(center, center, radius, Gradient(color, color));
}

void Canvas::draw_gradient_circle(const Pointf &center, float radius, const Gradient &gradient)
{
	draw_gradient_circle(center, center, radius, gradient);
}

void Canvas::draw_gradient_circle(const Pointf &center, const Pointf &centergradient, float radius, const Gradient &gradient)
{
	float offset_x = 0;
	float offset_y = 0;

	float rotationcount = max(5, (radius - 3));
	float halfpi = 1.5707963267948966192313216916398f;
	float turn = halfpi / rotationcount;

	if(center.distance(center + centergradient) < radius)
	{
		offset_x = centergradient.x;
		offset_y = -centergradient.y;
	}

	Vec4f colors[3] =
	{
		Vec4f(gradient.top_left.get_red(), gradient.top_left.get_green(), gradient.top_left.get_blue(), gradient.top_left.get_alpha()),
		Vec4f(gradient.bottom_right.get_red(), gradient.bottom_right.get_green(), gradient.bottom_right.get_blue(), gradient.bottom_right.get_alpha()),
		Vec4f(gradient.bottom_right.get_red(), gradient.bottom_right.get_green(), gradient.bottom_right.get_blue(), gradient.bottom_right.get_alpha())
	};

	Vec4f triangle_colors[4*3];
	for (int i=0; i<3; i++)
	{
		triangle_colors[0*3+i] = colors[i];
		triangle_colors[1*3+i] = colors[i];
		triangle_colors[2*3+i] = colors[i];
		triangle_colors[3*3+i] = colors[i];
	}

	RenderBatchTriangle *batcher = impl->get_triangle_batcher();

	for(float i = 0; i < rotationcount ; i++)
	{
		float pos1 = cos(i * turn);
		float pos2 = sin(i * turn);
		float pos3 = cos((i+1) * turn);
		float pos4 = sin((i+1) * turn);

		Vec2f positions[4*3] =
		{
			// 90 triangle:
			Vec2f(center.x + offset_x , center.y + offset_y),
			Vec2f(center.x + ((float)radius * pos1), center.y + ((float)radius * pos2)),
			Vec2f(center.x + ((float)radius * pos3), center.y + ((float)radius * pos4)),

			// 0 triangle:
			Vec2f(center.x + offset_x , center.y + offset_y),
			Vec2f(center.x + ((float)radius * pos2), center.y - ((float)radius * pos1)),
			Vec2f(center.x + ((float)radius * pos4), center.y - ((float)radius * pos3)),

			// 270 triangle:
			Vec2f(center.x + offset_x , center.y + offset_y),
			Vec2f(center.x - ((float)radius * pos1), center.y - ((float)radius * pos2)),
			Vec2f(center.x - ((float)radius * pos3), center.y - ((float)radius * pos4)),

			// 180 triangle:
			Vec2f(center.x + offset_x , center.y + offset_y),
			Vec2f(center.x - ((float)radius * pos2), center.y + ((float)radius * pos1)),
			Vec2f(center.x - ((float)radius * pos4), center.y + ((float)radius * pos3))
		};

		batcher->draw_triangle(*this, positions, triangle_colors, 4*3);

	}
}

void Canvas::draw_triangle(const Pointf &a, const Pointf &b, const Pointf &c, const Colorf &color)
{
	Vec2f positions[3] =
	{
		Vec2f(a.x, a.y),
		Vec2f(b.x, b.y),
		Vec2f(c.x, c.y)
	};

	RenderBatchTriangle *batcher = impl->get_triangle_batcher();
	batcher->draw_triangle(*this, positions, color, 3);
}

void Canvas::draw_triangles(Vec2f *triangle_positions, int num_vertices, const Colorf &color)
{
	RenderBatchTriangle *batcher = impl->get_triangle_batcher();
	batcher->draw_triangle(*this, triangle_positions, color, num_vertices);
}

void Canvas::draw_triangle(const Trianglef &dest_triangle, const Colorf &color)
{
	draw_triangle(dest_triangle.p, dest_triangle.q, dest_triangle.r, color);
}

void Canvas::draw_triangles(Vec2f *positions, Vec2f *texture_positions, int num_vertices, const Texture2D &texture, const Colorf &color)
{
	RenderBatchTriangle *batcher = impl->get_triangle_batcher();
	batcher->draw_triangles(*this, positions, texture_positions, num_vertices, texture, color);
}

void Canvas::draw_ellipse(const Pointf &center, float radius_x, float radius_y, const Colorf &color )
{
	float max_radius = max(radius_x, radius_y);
	if (max_radius == 0)
		return;

	push_translate(center);
	mult_scale(radius_x/max_radius, radius_y/max_radius);
	draw_circle(Pointf(0,0), max_radius, color);
	pop_modelview();
}

void Canvas::draw_ellipse_gradient(const Pointf &center, float radius_x, float radius_y, const Gradient &gradient)
{
	float max_radius = max(radius_x, radius_y);
	if (max_radius == 0)
		return;

	push_translate(center);
	mult_scale(radius_x/max_radius, radius_y/max_radius);
	draw_gradient_circle( Pointf(0,0), max_radius, gradient);
	pop_modelview();
}

void Canvas::set_font_manager(FontManager &font_manager)
{
	impl->font_manager = font_manager;
}

void Canvas::flip(DisplayWindow &window, int interval)
{
	window.throw_if_null();
	flush();
	window.flip(interval);
}

void Canvas::flip(int interval)
{
	if (impl->display_window.is_null())
		throw Exception("The display window is not known to this canvas");
	flush();
	impl->display_window.flip(interval);
}

/////////////////////////////////////////////////////////////////////////////
// Canvas Implementation:

}
