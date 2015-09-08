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
**    Mark Page
*/

#pragma once

#include "API/Display/Render/graphic_context.h"
#include "Display/2D/render_batch_buffer.h"
#include "Display/2D/render_batch_triangle.h"
#include "Display/2D/render_batch_line.h"
#include "Display/2D/render_batch_line_texture.h"
#include "Display/2D/render_batch_point.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/Window/display_window.h"
#include "canvas_batcher.h"

namespace clan
{
	class RenderBatcher;
	class RenderBatchTriangle;

	class Canvas_Impl
	{
	public:
		Canvas_Impl();
		~Canvas_Impl();

		void init(Canvas_Impl *canvas);
		void init(Canvas_Impl *canvas, FrameBuffer &framebuffer);
		void init(DisplayWindow &window);

		void clear(const Colorf &color);

		void flush();
		void set_batcher(Canvas &canvas, RenderBatcher *batcher);

		void set_cliprect(const Rectf &rect);
		void push_cliprect(const Rectf &rect);
		void push_cliprect();
		void pop_cliprect();
		void reset_cliprect();

		GraphicContext get_gc() const { return gc; }
		GraphicContext& get_gc() { return gc; }

		void set_transform(const Mat4f &matrix);
		const Mat4f &get_transform() const;
		Mat4f &get_inverse_transform();
		const Mat4f &get_projection() const;

		void set_map_mode(MapMode map_mode);
		void set_user_projection(const Mat4f &projection);
		void update_viewport_size();
		void set_viewport(const Rectf &viewport);

		static void get_gradient_colors(const Vec2f *triangles, int num_vertex, const Gradient &gradient, std::vector<Colorf> &out_colors);
		static Rectf get_triangles_bounding_box(const Vec2f *triangles, int num_vertex);
		static void get_texture_coords(const Vec2f *triangles, int num_vertex, const Texture2D &texture, const Rect &texture_rect, std::vector<Vec2f> &out_texture_positions);

		std::vector<Rectf> cliprects;
		CanvasBatcher batcher;

	private:
		void setup(GraphicContext &new_gc);
		void calculate_map_mode_matrices();
		MapMode get_top_down_map_mode() const;
		void on_window_resized(const Size &size);
		void update_batcher_matrix();
		void write_cliprect(const Rectf &rect);
		void on_window_flip();

		GraphicContext gc;
		SlotContainer sc;

		Mat4f canvas_transform;
		bool canvas_inverse_transform_set = false;
		Mat4f canvas_inverse_transform;
		Mat4f canvas_projection;
		MapMode canvas_map_mode;
		Rectf viewport_rect;

		DisplayWindow current_window;

		TextureImageYAxis canvas_y_axis;

		Mat4f user_projection;
		ClipZRange gc_clip_z_range;
	};
}
