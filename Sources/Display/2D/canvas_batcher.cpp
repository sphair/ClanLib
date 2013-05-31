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
**    Magnus Norddahl
**    Mark Page
*/

#include "Display/precomp.h"
#include "canvas_batcher.h"
#include "API/Display/Render/render_batcher.h"
#include "API/Display/Render/shared_gc_data.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"

namespace clan
{

class CanvasBatcher_Impl
{
public:
	CanvasBatcher_Impl();
	~CanvasBatcher_Impl();

	void flush();
	bool set_batcher(GraphicContext &gc, RenderBatcher *batcher);
	void update_batcher_matrix(GraphicContext &gc, const Mat4f &modelview, const Mat4f &projection);

	GraphicContext current_gc;

	RenderBatchTriangle *get_triangle_batcher();
	RenderBatchLine *get_line_batcher();
	RenderBatchLineTexture *get_line_texture_batcher();
	RenderBatchPoint *get_point_batcher();

	RenderBatcher *active_batcher;
	RenderBatchBuffer render_batcher_buffer;
	std::shared_ptr<RenderBatchTriangle> render_batcher_triangle;
	std::shared_ptr<RenderBatchLine> render_batcher_line;
	std::shared_ptr<RenderBatchLineTexture> render_batcher_line_texture;
	std::shared_ptr<RenderBatchPoint> render_batcher_point;

};

CanvasBatcher_Impl::CanvasBatcher_Impl() : active_batcher(0)
{
}

CanvasBatcher_Impl::~CanvasBatcher_Impl()
{
}

CanvasBatcher::CanvasBatcher() : impl(new CanvasBatcher_Impl)
{
}

CanvasBatcher::~CanvasBatcher()
{
}


RenderBatchTriangle *CanvasBatcher_Impl::get_triangle_batcher()
{
	if (!render_batcher_triangle)
		render_batcher_triangle = std::shared_ptr<RenderBatchTriangle>(new RenderBatchTriangle(&render_batcher_buffer));
	return render_batcher_triangle.get();
}

RenderBatchLine *CanvasBatcher_Impl::get_line_batcher()
{
	if (!render_batcher_line)
		render_batcher_line = std::shared_ptr<RenderBatchLine>(new RenderBatchLine(&render_batcher_buffer));
	return render_batcher_line.get();
}

RenderBatchLineTexture *CanvasBatcher_Impl::get_line_texture_batcher()
{
	if (!render_batcher_line_texture)
		render_batcher_line_texture = std::shared_ptr<RenderBatchLineTexture>(new RenderBatchLineTexture(&render_batcher_buffer));
	return render_batcher_line_texture.get();
}

RenderBatchPoint *CanvasBatcher_Impl::get_point_batcher()
{
	if (!render_batcher_point)
		render_batcher_point = std::shared_ptr<RenderBatchPoint>(new RenderBatchPoint(&render_batcher_buffer));
	return render_batcher_point.get();
}


RenderBatchTriangle *CanvasBatcher::get_triangle_batcher()
{
	return impl->get_triangle_batcher();
}

RenderBatchLine *CanvasBatcher::get_line_batcher()
{
	return impl->get_line_batcher();
}

RenderBatchLineTexture *CanvasBatcher::get_line_texture_batcher()
{
	return impl->get_line_texture_batcher();
}

RenderBatchPoint *CanvasBatcher::get_point_batcher()
{
	return impl->get_point_batcher();
}


void CanvasBatcher_Impl::flush()
{
	if (active_batcher)
	{
		RenderBatcher *batcher = active_batcher;
		active_batcher = 0;
		batcher->flush(current_gc);
	}
}

void CanvasBatcher_Impl::update_batcher_matrix(GraphicContext &gc, const Mat4f &modelview, const Mat4f &projection)
{
	if (gc != current_gc)
	{
		flush();
		current_gc = gc;
	}

	if (active_batcher)
	{
		active_batcher->matrix_changed(modelview, projection);
	}
}

bool CanvasBatcher_Impl::set_batcher(GraphicContext &gc, RenderBatcher *batcher)
{
	if ( (active_batcher != batcher) || (gc != current_gc) )
	{
		flush();
		current_gc = gc;
		active_batcher = batcher;
		return true;
	}
	return false;
}

void CanvasBatcher::flush()
{
	impl->flush();
}

void CanvasBatcher::update_batcher_matrix(GraphicContext &gc, const Mat4f &modelview, const Mat4f &projection)
{
	impl->update_batcher_matrix(gc, modelview, projection);
}

bool CanvasBatcher::set_batcher(GraphicContext &gc, RenderBatcher *batcher)
{
	return impl->set_batcher(gc, batcher);
}

}
