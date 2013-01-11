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
**    Mark Page
*/

#pragma once

#include "API/Display/Render/graphic_context.h"
#include "Display/2D/render_batch_buffer.h"
#include "Display/2D/render_batch_triangle.h"
#include "Display/2D/render_batch_line.h"
#include "Display/2D/render_batch_point.h"
#include "API/Display/2D/canvas.h"
#include "API/Display/Font/font_manager.h"
#include "API/Display/Window/display_window.h"

namespace clan
{

class RenderBatcher;
class RenderBatchTriangle;

class Canvas_Impl
{
public:
	Canvas_Impl(GraphicContext &gc);
	~Canvas_Impl();

	void clear(const Colorf &color);

	void flush();
	void set_batcher(Canvas &canvas, RenderBatcher *batcher);

	void set_cliprect(const Rect &rect);
	void push_cliprect(const Rect &rect);
	void push_cliprect();
	void pop_cliprect();
	void reset_cliprect();

	RenderBatchTriangle *get_triangle_batcher();
	RenderBatchLine *get_line_batcher();
	RenderBatchPoint *get_point_batcher();

	GraphicContext get_gc() const {return gc;}
	GraphicContext& get_gc() {return gc;}

	void set_modelview(const Mat4f &modelview);
	void push_modelview(const Mat4f &modelview);
	void pop_modelview();
	const Mat4f &get_modelview() const;
	const Mat4f &get_projection() const;

	void set_map_mode(MapMode map_mode);
	void set_user_projection(const Mat4f &projection);
	void update_viewport_size();

	std::vector<Rect> cliprects;
	FontManager font_manager;
	DisplayWindow display_window;

private:
	void calculate_map_mode_matrices();
	MapMode get_top_down_map_mode() const;
	void on_window_resized(const Size &size);
	void update_batcher_matrix();
	void write_cliprect(const Rect &rect);

	GraphicContext gc;

	RenderBatcher *active_batcher;
	RenderBatchBuffer render_batcher_buffer;
	std::shared_ptr<RenderBatchTriangle> render_batcher_triangle;
	std::shared_ptr<RenderBatchLine> render_batcher_line;
	std::shared_ptr<RenderBatchPoint> render_batcher_point;

	std::vector<Mat4f> canvas_modelviews;
	Mat4f canvas_projection;
	MapMode canvas_map_mode;
	Size canvas_size;
	Slot slot_window_resized;

	TextureImageYAxis canvas_y_axis;

	Mat4f user_projection;
	ClipZRange gc_clip_z_range;

};

}
