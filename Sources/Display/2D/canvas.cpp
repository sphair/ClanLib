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
#include "API/Display/Font/font.h"

namespace clan
{
	Canvas::Canvas()
	{
	}

	Canvas::Canvas(DisplayWindow &window) : impl(std::make_shared<Canvas_Impl>())
	{
		impl->init(window);
		set_map_mode(map_2d_upper_left);
	}

	Canvas::Canvas(Canvas &canvas, FrameBuffer &framebuffer) : impl(std::make_shared<Canvas_Impl>())
	{
		impl->init(canvas.impl.get(), framebuffer);
		set_map_mode(map_2d_upper_left);
	}

	Canvas::~Canvas()
	{
	}

	Canvas Canvas::create()
	{
		Canvas copy_canvas;
		copy_canvas.impl = std::shared_ptr<Canvas_Impl>(new Canvas_Impl);
		copy_canvas.impl->init(impl.get());
		copy_canvas.set_map_mode(map_2d_upper_left);
		return copy_canvas;
	}

	void Canvas::throw_if_null() const
	{
		if (!impl)
			throw Exception("Canvas is null");
	}

	GraphicContext &Canvas::get_gc() const
	{
		return impl->get_gc();
	}

	const Mat4f &Canvas::get_transform() const
	{
		return impl->get_transform();
	}

	Mat4f &Canvas::get_inverse_transform()
	{
		return impl->get_inverse_transform();
	}

	const Mat4f &Canvas::get_projection() const
	{
		return impl->get_projection();
	}

	Rectf Canvas::get_cliprect() const
	{
		if (!impl->cliprects.empty())
			return impl->cliprects.back();

		return Rectf(0, 0, get_width(), get_height());
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

	void Canvas::set_rasterizer_state(const RasterizerState &state)
	{
		flush();
		get_gc().set_rasterizer_state(state);
	}

	void Canvas::set_blend_state(const BlendState &state, const Colorf &blend_color, unsigned int sample_mask)
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

	void Canvas::set_cliprect(const Rectf &rect)
	{
		flush();
		impl->set_cliprect(rect);
	}

	void Canvas::push_cliprect(const Rectf &rect)
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

	void Canvas::clear_stencil(int value)
	{
		flush();
		get_gc().clear_stencil(value);
	}

	void Canvas::clear_depth(float value)
	{
		flush();
		get_gc().clear_depth(value);
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

	void Canvas::set_viewport(const Rectf &viewport)
	{
		flush();
		impl->set_viewport(viewport);
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

	void Canvas::set_transform(const Mat4f &matrix)
	{
		impl->set_transform(matrix);
	}

	void Canvas::mult_transform(const Mat4f &matrix)
	{
		impl->set_transform(get_transform() * matrix);
	}

	void Canvas::draw_point(float x1, float y1, const Colorf &color)
	{
		Vec2f positions[1] =
		{
			Vec2f(x1, y1)
		};

		RenderBatchPoint *batcher = impl->batcher.get_point_batcher();
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

		RenderBatchLine *batcher = impl->batcher.get_line_batcher();
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

	void Canvas::draw_lines(const Vec2f *positions, int num_vertices, const Colorf &color)
	{
		RenderBatchLine *batcher = impl->batcher.get_line_batcher();
		batcher->draw_lines(*this, positions, color, num_vertices);
	}

	void Canvas::draw_lines(const Vec2f *line_positions, const Vec2f *texture_positions, int num_vertices, const Texture2D &texture, const Colorf &line_color)
	{
		RenderBatchLineTexture *batcher = impl->batcher.get_line_texture_batcher();
		batcher->draw_lines(*this, line_positions, texture_positions, num_vertices, texture, line_color);
	}

	void Canvas::draw_line_strip(const Vec2f *line_positions, int num_vertices, const Colorf &line_color)
	{
		RenderBatchLine *batcher = impl->batcher.get_line_batcher();
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

		RenderBatchLine *batcher = impl->batcher.get_line_batcher();
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

	void Canvas::fill_rect(float x1, float y1, float x2, float y2, const Colorf &color)
	{
		RenderBatchTriangle *batcher = impl->batcher.get_triangle_batcher();
		batcher->fill(*this, x1, y1, x2, y2, color);
	}

	void Canvas::fill_rect(const Pointf &start, const Pointf &end, const Colorf &color)
	{
		fill_rect(start.x, start.y, end.x, end.y, color);
	}

	void Canvas::fill_rect(const Rectf &rect, const Colorf &color)
	{
		fill_rect(rect.left, rect.top, rect.right, rect.bottom, color);
	}

	void Canvas::fill_rect(float x1, float y1, float x2, float y2, const Gradient &gradient)
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

		RenderBatchTriangle *batcher = impl->batcher.get_triangle_batcher();
		batcher->fill_triangle(*this, positions, colors, 6);

	}

	void Canvas::fill_rect(const Pointf &start, const Pointf &end, const Gradient &gradient)
	{
		fill_rect(start.x, start.y, end.x, end.y, gradient);
	}

	void Canvas::fill_rect(const Rectf &rect, const Gradient &gradient)
	{
		fill_rect(rect.left, rect.top, rect.right, rect.bottom, gradient);
	}

	void Canvas::fill_circle(float center_x, float center_y, float radius, const Colorf &color)
	{
		fill_circle(Pointf(center_x, center_y), Pointf(center_x, center_y), radius, Gradient(color, color));
	}

	void Canvas::fill_circle(const Pointf &center, float radius, const Colorf &color)
	{
		fill_circle(center, center, radius, Gradient(color, color));
	}

	void Canvas::fill_circle(const Pointf &center, float radius, const Gradient &gradient)
	{
		fill_circle(center, center, radius, gradient);
	}

	void Canvas::fill_circle(const Pointf &center, const Pointf &centergradient, float radius, const Gradient &gradient)
	{
		float offset_x = 0;
		float offset_y = 0;

		float rotationcount = max(5.0f, (radius - 3.0f));
		float halfpi = 1.5707963267948966192313216916398f;
		float turn = halfpi / rotationcount;

		if (center.distance(center + centergradient) < radius)
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

		Vec4f triangle_colors[4 * 3];
		for (int i = 0; i < 3; i++)
		{
			triangle_colors[0 * 3 + i] = colors[i];
			triangle_colors[1 * 3 + i] = colors[i];
			triangle_colors[2 * 3 + i] = colors[i];
			triangle_colors[3 * 3 + i] = colors[i];
		}

		RenderBatchTriangle *batcher = impl->batcher.get_triangle_batcher();

		for (float i = 0; i < rotationcount; i++)
		{
			float pos1 = cos(i * turn);
			float pos2 = sin(i * turn);
			float pos3 = cos((i + 1) * turn);
			float pos4 = sin((i + 1) * turn);

			Vec2f positions[4 * 3] =
			{
				// 90 triangle:
				Vec2f(center.x + offset_x, center.y + offset_y),
				Vec2f(center.x + ((float)radius * pos1), center.y + ((float)radius * pos2)),
				Vec2f(center.x + ((float)radius * pos3), center.y + ((float)radius * pos4)),

				// 0 triangle:
				Vec2f(center.x + offset_x, center.y + offset_y),
				Vec2f(center.x + ((float)radius * pos2), center.y - ((float)radius * pos1)),
				Vec2f(center.x + ((float)radius * pos4), center.y - ((float)radius * pos3)),

				// 270 triangle:
				Vec2f(center.x + offset_x, center.y + offset_y),
				Vec2f(center.x - ((float)radius * pos1), center.y - ((float)radius * pos2)),
				Vec2f(center.x - ((float)radius * pos3), center.y - ((float)radius * pos4)),

				// 180 triangle:
				Vec2f(center.x + offset_x, center.y + offset_y),
				Vec2f(center.x - ((float)radius * pos2), center.y + ((float)radius * pos1)),
				Vec2f(center.x - ((float)radius * pos4), center.y + ((float)radius * pos3))
			};

			batcher->fill_triangle(*this, positions, triangle_colors, 4 * 3);

		}
	}

	void Canvas::fill_triangle(const Pointf &a, const Pointf &b, const Pointf &c, const Colorf &color)
	{
		Vec2f positions[3] =
		{
			Vec2f(a.x, a.y),
			Vec2f(b.x, b.y),
			Vec2f(c.x, c.y)
		};

		RenderBatchTriangle *batcher = impl->batcher.get_triangle_batcher();
		batcher->fill_triangle(*this, positions, color, 3);
	}

	void Canvas::fill_triangles(const Vec2f *triangle_positions, int num_vertices, const Colorf &color)
	{
		RenderBatchTriangle *batcher = impl->batcher.get_triangle_batcher();
		batcher->fill_triangle(*this, triangle_positions, color, num_vertices);
	}

	void Canvas::fill_triangles(const Vec2f *triangle_positions, const Colorf *colors, int num_vertices)
	{
		RenderBatchTriangle *batcher = impl->batcher.get_triangle_batcher();
		batcher->fill_triangle(*this, triangle_positions, colors, num_vertices);
	}

	void Canvas::fill_triangles(const std::vector<Vec2f> &triangles, const Colorf *colors)
	{
		if (!triangles.empty())
		{
			RenderBatchTriangle *batcher = impl->batcher.get_triangle_batcher();
			batcher->fill_triangle(*this, &triangles[0], colors, triangles.size());
		}
	}

	void Canvas::fill_triangles(const Vec2f *triangle_positions, int num_vertices, const Gradient &gradient)
	{
		if (num_vertices)
		{
			std::vector<Colorf> colors;
			Canvas_Impl::get_gradient_colors(triangle_positions, num_vertices, gradient, colors);

			RenderBatchTriangle *batcher = impl->batcher.get_triangle_batcher();
			batcher->fill_triangle(*this, triangle_positions, &colors[0], num_vertices);

		}
	}

	void Canvas::fill_triangles(const std::vector<Vec2f> &triangles, const Gradient &gradient)
	{
		if (!triangles.empty())
		{
			std::vector<Colorf> colors;
			Canvas_Impl::get_gradient_colors(&triangles[0], triangles.size(), gradient, colors);

			RenderBatchTriangle *batcher = impl->batcher.get_triangle_batcher();
			batcher->fill_triangle(*this, &triangles[0], &colors[0], triangles.size());

		}
	}

	void Canvas::fill_triangles(const std::vector<Vec2f> &triangles, const Colorf &color)
	{
		if (!triangles.empty())
		{
			RenderBatchTriangle *batcher = impl->batcher.get_triangle_batcher();
			batcher->fill_triangle(*this, &triangles[0], color, triangles.size());
		}
	}

	void Canvas::fill_triangle(const Trianglef &dest_triangle, const Colorf &color)
	{
		fill_triangle(dest_triangle.p, dest_triangle.q, dest_triangle.r, color);
	}

	void Canvas::fill_triangles(const Vec2f *positions, const Vec2f *texture_positions, int num_vertices, const Texture2D &texture, const Colorf &color)
	{
		RenderBatchTriangle *batcher = impl->batcher.get_triangle_batcher();
		batcher->fill_triangles(*this, positions, texture_positions, num_vertices, texture, color);
	}

	void Canvas::fill_triangles(const Vec2f *positions, const Vec2f *texture_positions, int num_vertices, const Texture2D &texture, const Colorf *colors)
	{
		RenderBatchTriangle *batcher = impl->batcher.get_triangle_batcher();
		batcher->fill_triangles(*this, positions, texture_positions, num_vertices, texture, colors);
	}

	void Canvas::fill_triangles(const std::vector<Vec2f> &positions, const std::vector<Vec2f> &texture_positions, const Texture2D &texture, const Colorf &color)
	{
		if (!positions.empty())
		{
			RenderBatchTriangle *batcher = impl->batcher.get_triangle_batcher();
			batcher->fill_triangles(*this, &positions[0], &texture_positions[0], positions.size(), texture, color);
		}
	}

	void Canvas::fill_triangles(const std::vector<Vec2f> &positions, const std::vector<Vec2f> &texture_positions, const Texture2D &texture, const std::vector<Colorf> &colors)
	{
		if (!positions.empty())
		{
			RenderBatchTriangle *batcher = impl->batcher.get_triangle_batcher();
			batcher->fill_triangles(*this, &positions[0], &texture_positions[0], positions.size(), texture, &colors[0]);
		}
	}


	void Canvas::fill_triangles(const std::vector<Vec2f> &positions, const Texture2D &texture, const Colorf &color)
	{
		fill_triangles(positions, texture, texture.get_size(), color);
	}

	void Canvas::fill_triangles(const std::vector<Vec2f> &positions, const Texture2D &texture, const Rect &texture_rect, const Colorf &color)
	{
		if (!positions.empty())
		{
			std::vector<Vec2f> texture_positions;
			Canvas_Impl::get_texture_coords(&positions[0], positions.size(), texture, texture_rect, texture_positions);

			RenderBatchTriangle *batcher = impl->batcher.get_triangle_batcher();
			batcher->fill_triangles(*this, &positions[0], &texture_positions[0], positions.size(), texture, color);
		}
	}

	void Canvas::fill_triangles(const Vec2f *triangle_positions, int num_vertices, const Texture2D &texture, const Colorf &color)
	{
		fill_triangles(triangle_positions, num_vertices, texture, texture.get_size(), color);
	}

	void Canvas::fill_triangles(const Vec2f *triangle_positions, int num_vertices, const Texture2D &texture, const Rect &texture_rect, const Colorf &color)
	{
		if (num_vertices)
		{
			std::vector<Vec2f> texture_positions;
			Canvas_Impl::get_texture_coords(triangle_positions, num_vertices, texture, texture_rect, texture_positions);

			RenderBatchTriangle *batcher = impl->batcher.get_triangle_batcher();
			batcher->fill_triangles(*this, triangle_positions, &texture_positions[0], num_vertices, texture, color);
		}
	}

	void Canvas::fill_triangles(const std::vector<Vec2f> &positions, const Texture2D &texture, const Gradient &gradient)
	{
		fill_triangles(positions, texture, texture.get_size(), gradient);
	}

	void Canvas::fill_triangles(const std::vector<Vec2f> &positions, const Texture2D &texture, const Rect &texture_rect, const Gradient &gradient)
	{
		if (!positions.empty())
		{
			std::vector<Vec2f> texture_positions;
			Canvas_Impl::get_texture_coords(&positions[0], positions.size(), texture, texture_rect, texture_positions);
			std::vector<Colorf> colors;
			Canvas_Impl::get_gradient_colors(&positions[0], positions.size(), gradient, colors);

			RenderBatchTriangle *batcher = impl->batcher.get_triangle_batcher();
			batcher->fill_triangles(*this, &positions[0], &texture_positions[0], positions.size(), texture, &colors[0]);
		}
	}

	void Canvas::fill_triangles(const Vec2f *triangle_positions, int num_vertices, const Texture2D &texture, const Gradient &gradient)
	{
		fill_triangles(triangle_positions, num_vertices, texture, texture.get_size(), gradient);
	}

	void Canvas::fill_triangles(const Vec2f *triangle_positions, int num_vertices, const Texture2D &texture, const Rect &texture_rect, const Gradient &gradient)
	{
		if (num_vertices)
		{
			std::vector<Vec2f> texture_positions;
			Canvas_Impl::get_texture_coords(triangle_positions, num_vertices, texture, texture_rect, texture_positions);
			std::vector<Colorf> colors;
			Canvas_Impl::get_gradient_colors(triangle_positions, num_vertices, gradient, colors);

			RenderBatchTriangle *batcher = impl->batcher.get_triangle_batcher();
			batcher->fill_triangles(*this, triangle_positions, &texture_positions[0], num_vertices, texture, &colors[0]);
		}
	}

	void Canvas::fill_ellipse(const Pointf &center, float radius_x, float radius_y, const Colorf &color)
	{
		float max_radius = max(radius_x, radius_y);
		if (max_radius == 0)
			return;

		const Mat4f original_transform = get_transform();
		mult_transform(Mat4f::translate(center.x, center.y, 0));
		mult_transform(Mat4f::scale(radius_x / max_radius, radius_y / max_radius, 1.0f));
		fill_circle(Pointf(0, 0), max_radius, color);
		set_transform(original_transform);
	}

	void Canvas::fill_ellipse(const Pointf &center, float radius_x, float radius_y, const Gradient &gradient)
	{
		float max_radius = max(radius_x, radius_y);
		if (max_radius == 0)
			return;

		const Mat4f original_transform = get_transform();
		mult_transform(Mat4f::translate(center.x, center.y, 0));
		mult_transform(Mat4f::scale(radius_x / max_radius, radius_y / max_radius, 1.0f));
		fill_circle(Pointf(0, 0), max_radius, gradient);
		set_transform(original_transform);
	}

	Pointf Canvas::grid_fit(const Pointf &pos)
	{
		float pixel_ratio = get_gc().get_pixel_ratio();
		Vec4f world_pos = get_transform() * Vec4f(pos.x, pos.y, 0.0f, 1.0f);
		world_pos.x = std::round(world_pos.x * pixel_ratio) / pixel_ratio;
		world_pos.y = std::round(world_pos.y * pixel_ratio) / pixel_ratio;
		Vec4f object_pos = get_inverse_transform() * world_pos;
		return Pointf(object_pos.x, object_pos.y);
	}
}
