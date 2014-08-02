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
**    Mark Page
*/

#include "SWRender/precomp.h"
#include "API/SWRender/swr_graphic_context.h"
#include "API/SWRender/pixel_command.h"
#include "swr_graphic_context_provider.h"
#include "Canvas/pixel_canvas.h"

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// GraphicContext_SWRender_Impl Class:

class GraphicContext_SWRender_Impl
{
public:
	GraphicContext_SWRender_Impl() : provider(0)
	{
	}

	~GraphicContext_SWRender_Impl()
	{
	}
	SWRenderGraphicContextProvider *provider;
};

/////////////////////////////////////////////////////////////////////////////
// GraphicContext_SWRender Construction:

GraphicContext_SWRender::GraphicContext_SWRender(GraphicContext &gc) : GraphicContext(gc),
 impl(std::make_shared<GraphicContext_SWRender_Impl>())
{
	impl->provider = dynamic_cast <SWRenderGraphicContextProvider *> (GraphicContext::get_provider());
	if (!impl->provider)
	{
			throw Exception("Graphic Context is not from a SWRender target");
	}

}

GraphicContext_SWRender::~GraphicContext_SWRender()
{
}

/////////////////////////////////////////////////////////////////////////////
// GraphicContext_SWRender Attributes:

void GraphicContext_SWRender::throw_if_null() const
{
	if (!impl)
		throw Exception("GraphicContext_SWRender is null");
}

PixelPipeline *GraphicContext_SWRender::get_pipeline() const
{
	return impl->provider->get_canvas()->get_pipeline();
}

/////////////////////////////////////////////////////////////////////////////
// GraphicContext_SWRender Operations:

void GraphicContext_SWRender::draw_pixels_bicubic(int x, int y, int zoom_number, int zoom_denominator, const PixelBuffer &pixels)
{
	impl->provider->draw_pixels_bicubic(x, y, zoom_number, zoom_denominator, pixels);
}

void GraphicContext_SWRender::draw_pixels(float x, float y, float zoom_x, float zoom_y, const PixelBuffer &pixel_buffer, const Rect &src_rect, const Colorf &color)
{
	impl->provider->draw_pixels(x, y, zoom_x, zoom_y, pixel_buffer, src_rect, color);
}

void GraphicContext_SWRender::queue_command(std::unique_ptr<PixelCommand> &command)
{
	impl->provider->queue_command(command);
}

/////////////////////////////////////////////////////////////////////////////
// GraphicContext_SWRender Implementation:
}
