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
**    Magnus Norddahl
*/

#include "SWRender/precomp.h"
#include "pixel_canvas.h"
#include "Pipeline/pixel_pipeline.h"
#include "Commands/pixel_command_bicubic.h"
#include "Commands/pixel_command_clear.h"
#include "Commands/pixel_command_line.h"
#include "Commands/pixel_command_pixels.h"
#include "Commands/pixel_command_set_framebuffer.h"
#include "Commands/pixel_command_set_blendfunc.h"
#include "Commands/pixel_command_set_cliprect.h"
#include "Commands/pixel_command_set_sampler.h"
#include "Commands/pixel_command_sprite.h"
#include "Commands/pixel_command_triangle.h"
#include "../swr_frame_buffer_provider.h"

namespace clan
{

PixelCanvas::PixelCanvas(const Size &size)
: primary_colorbuffer0(size.width, size.height, tf_bgra8),
  framebuffer_set(false), cliprect_set(false),
  cur_blend_src(cl_blend_src_alpha),
  cur_blend_dest(cl_blend_one_minus_src_alpha),
  cur_blend_src_alpha(cl_blend_one), 
  cur_blend_dest_alpha(cl_blend_one_minus_src_alpha)
{
	pipeline.reset(new PixelPipeline());

	colorbuffer0.set(primary_colorbuffer0);
	pipeline->queue(new(pipeline.get()) PixelCommandSetFrameBuffer(colorbuffer0));
	clip_rect = Rect(Point(0,0), size);
	pipeline->queue(new(pipeline.get()) PixelCommandSetClipRect(clip_rect));
	clear(Colorf::black);
}

PixelCanvas::~PixelCanvas()
{
	pipeline.reset();
}

void PixelCanvas::resize(const Size &size)
{
	Size old_size = colorbuffer0.size;
	primary_colorbuffer0 = PixelBuffer(size.width, size.height, tf_bgra8);
	if (!framebuffer_set)
	{
		pipeline->wait_for_workers();
		colorbuffer0.set(primary_colorbuffer0);
		pipeline->queue(new(pipeline.get()) PixelCommandSetFrameBuffer(colorbuffer0));
		Rect rect = clip_rect;
		clip_rect = (Point(0,0),size);
		if (cliprect_set)
			set_clip_rect(rect);
		else
			pipeline->queue(new(pipeline.get()) PixelCommandSetClipRect(clip_rect));
	}
}

void PixelCanvas::set_clip_rect(const Rect &new_clip_rect)
{
	clip_rect = Rect(0, 0, colorbuffer0.size.width, colorbuffer0.size.height);
	clip_rect.overlap(new_clip_rect);
	if (clip_rect.get_width() < 0 || clip_rect.get_height() < 0)
		clip_rect = Rect(0,0,0,0);
	cliprect_set = true;
	pipeline->queue(new(pipeline.get()) PixelCommandSetClipRect(clip_rect));
}

void PixelCanvas::reset_clip_rect()
{
	clip_rect = Rect(0, 0, colorbuffer0.size.width, colorbuffer0.size.height);
	cliprect_set = false;
	pipeline->queue(new(pipeline.get()) PixelCommandSetClipRect(clip_rect));
}

void PixelCanvas::set_blend_function(BlendFunc src, BlendFunc dest, BlendFunc src_alpha, BlendFunc dest_alpha, const Colorf &const_color)
{
	if (cur_blend_src != src || cur_blend_dest != dest || cur_blend_src_alpha != src_alpha || cur_blend_dest_alpha != dest_alpha || cur_blend_color != const_color)
	{
		cur_blend_src = src;
		cur_blend_dest = dest;
		cur_blend_src_alpha = src_alpha;
		cur_blend_dest_alpha = dest_alpha;
		cur_blend_color = const_color;
		pipeline->queue(new(pipeline.get()) PixelCommandSetBlendFunc(cur_blend_src, cur_blend_dest, cur_blend_src_alpha, cur_blend_dest_alpha, cur_blend_color));
	}
}

void PixelCanvas::set_framebuffer(const FrameBuffer &buffer)
{
	pipeline->wait_for_workers();

	SWRenderFrameBufferProvider *gdi_framebuffer = dynamic_cast<SWRenderFrameBufferProvider *>(buffer.get_provider());
	if (!gdi_framebuffer)
	{
		throw Exception("Invalid FrameBuffer");
	}

	framebuffer_set = true;
	framebuffer = buffer;

	slot_framebuffer_modified = gdi_framebuffer->get_sig_changed_event().connect(this, &PixelCanvas::modified_framebuffer);

	colorbuffer0.set(gdi_framebuffer->get_colorbuffer0());
	pipeline->queue(new(pipeline.get()) PixelCommandSetFrameBuffer(colorbuffer0));
	Rect rect = clip_rect;
	clip_rect = Rect(Point(0,0),colorbuffer0.size);
	if (cliprect_set)
		set_clip_rect(rect);
	else
		pipeline->queue(new(pipeline.get()) PixelCommandSetClipRect(clip_rect));
}

void PixelCanvas::reset_framebuffer()
{
	pipeline->wait_for_workers();

	framebuffer_set = false;
	slot_framebuffer_modified = Slot();
	colorbuffer0.set(primary_colorbuffer0);
	pipeline->queue(new(pipeline.get()) PixelCommandSetFrameBuffer(colorbuffer0));

	framebuffer = FrameBuffer();

	Rect rect = clip_rect;
	clip_rect = Rect(Point(0,0),colorbuffer0.size);
	if (cliprect_set)
		set_clip_rect(rect);
	else
		pipeline->queue(new(pipeline.get()) PixelCommandSetClipRect(clip_rect));
}

void PixelCanvas::clear(const Colorf &color)
{
	pipeline->queue(new(pipeline.get()) PixelCommandClear(color));
}

void PixelCanvas::draw_pixels(const Rect &dest, const PixelBuffer &image, const Rect &src_rect, const Colorf &primary_color)
{
	if (image.get_format() == tf_bgra8)
	{
		pipeline->queue(new(pipeline.get()) PixelCommandPixels(dest, image, src_rect, primary_color));
		pipeline->wait_for_workers();
	}
	else
	{
		PixelBuffer image_argb8 = image.to_format(tf_bgra8);
		draw_pixels(dest, image_argb8, src_rect, primary_color);
	}
}

void PixelCanvas::draw_pixels_bicubic(int x, int y, int zoom_number, int zoom_denominator, const PixelBuffer &image)
{
	if (image.get_format() == tf_bgra8)
	{
		pipeline->queue(new(pipeline.get()) PixelCommandBicubic(x, y, zoom_number, zoom_denominator, image));
		pipeline->wait_for_workers();
	}
	else
	{
		PixelBuffer image_argb8 = image.to_format(tf_bgra8);
		draw_pixels_bicubic(x, y, zoom_number, zoom_denominator, image_argb8);
	}
}

void PixelCanvas::queue_command(std::unique_ptr<PixelCommand> &command)
{
	pipeline->queue(command);
}

void PixelCanvas::set_sampler(int index, const PixelBuffer &new_sampler)
{
	pipeline->queue(new(pipeline.get()) PixelCommandSetSampler(index, new_sampler));
}

void PixelCanvas::reset_sampler(int index)
{
	pipeline->queue(new(pipeline.get()) PixelCommandSetSampler(index));
}

PixelBuffer &PixelCanvas::to_pixelbuffer()
{
	pipeline->wait_for_workers();
	return colorbuffer0.pixelbuffer;
}

void PixelCanvas::modified_framebuffer()
{
	pipeline->wait_for_workers();
	SWRenderFrameBufferProvider *gdi_framebuffer = dynamic_cast<SWRenderFrameBufferProvider *>(framebuffer.get_provider());

	colorbuffer0.set(gdi_framebuffer->get_colorbuffer0());
	pipeline->queue(new(pipeline.get()) PixelCommandSetFrameBuffer(colorbuffer0));
	Rect rect = clip_rect;
	clip_rect = Rect(Point(0,0),colorbuffer0.size);
	if (cliprect_set)
		set_clip_rect(rect);
	else
		pipeline->queue(new(pipeline.get()) PixelCommandSetClipRect(clip_rect));
}

}
