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

CL_PixelCanvas::CL_PixelCanvas(const CL_Size &size)
: primary_colorbuffer0(size.width, size.height, cl_argb8),
  framebuffer_set(false), cliprect_set(false),
  cur_blend_src(cl_blend_src_alpha),
  cur_blend_dest(cl_blend_one_minus_src_alpha),
  cur_blend_src_alpha(cl_blend_one), 
  cur_blend_dest_alpha(cl_blend_one_minus_src_alpha)
{
	pipeline.reset(new CL_PixelPipeline());

	colorbuffer0.set(primary_colorbuffer0);
	pipeline->queue(new(pipeline.get()) CL_PixelCommandSetFrameBuffer(colorbuffer0));
	clip_rect = CL_Rect(CL_Point(0,0), size);
	pipeline->queue(new(pipeline.get()) CL_PixelCommandSetClipRect(clip_rect));
	clear(CL_Colorf::black);
}

CL_PixelCanvas::~CL_PixelCanvas()
{
	pipeline.reset();
}

void CL_PixelCanvas::resize(const CL_Size &size)
{
	CL_Size old_size = colorbuffer0.size;
	primary_colorbuffer0 = CL_PixelBuffer(size.width, size.height, cl_argb8);
	if (!framebuffer_set)
	{
		pipeline->wait_for_workers();
		colorbuffer0.set(primary_colorbuffer0);
		pipeline->queue(new(pipeline.get()) CL_PixelCommandSetFrameBuffer(colorbuffer0));
		CL_Rect rect = clip_rect;
		clip_rect = (CL_Point(0,0),size);
		if (cliprect_set)
			set_clip_rect(rect);
		else
			pipeline->queue(new(pipeline.get()) CL_PixelCommandSetClipRect(clip_rect));
	}
}

void CL_PixelCanvas::set_clip_rect(const CL_Rect &new_clip_rect)
{
	clip_rect = CL_Rect(0, 0, colorbuffer0.size.width, colorbuffer0.size.height);
	clip_rect.overlap(new_clip_rect);
	if (clip_rect.get_width() < 0 || clip_rect.get_height() < 0)
		clip_rect = CL_Rect(0,0,0,0);
	cliprect_set = true;
	pipeline->queue(new(pipeline.get()) CL_PixelCommandSetClipRect(clip_rect));
}

void CL_PixelCanvas::reset_clip_rect()
{
	clip_rect = CL_Rect(0, 0, colorbuffer0.size.width, colorbuffer0.size.height);
	cliprect_set = false;
	pipeline->queue(new(pipeline.get()) CL_PixelCommandSetClipRect(clip_rect));
}

void CL_PixelCanvas::set_blend_function(CL_BlendFunc src, CL_BlendFunc dest, CL_BlendFunc src_alpha, CL_BlendFunc dest_alpha, const CL_Colorf &const_color)
{
	if (cur_blend_src != src || cur_blend_dest != dest || cur_blend_src_alpha != src_alpha || cur_blend_dest_alpha != dest_alpha || cur_blend_color != const_color)
	{
		cur_blend_src = src;
		cur_blend_dest = dest;
		cur_blend_src_alpha = src_alpha;
		cur_blend_dest_alpha = dest_alpha;
		cur_blend_color = const_color;
		pipeline->queue(new(pipeline.get()) CL_PixelCommandSetBlendFunc(cur_blend_src, cur_blend_dest, cur_blend_src_alpha, cur_blend_dest_alpha, cur_blend_color));
	}
}

void CL_PixelCanvas::set_framebuffer(const CL_FrameBuffer &buffer)
{
	pipeline->wait_for_workers();

	CL_SWRenderFrameBufferProvider *gdi_framebuffer = dynamic_cast<CL_SWRenderFrameBufferProvider *>(buffer.get_provider());
	if (!gdi_framebuffer)
	{
		throw CL_Exception("Invalid FrameBuffer");
	}

	framebuffer_set = true;
	framebuffer = buffer;

	slot_framebuffer_modified = gdi_framebuffer->get_sig_changed_event().connect(this, &CL_PixelCanvas::modified_framebuffer);

	colorbuffer0.set(gdi_framebuffer->get_colorbuffer0());
	pipeline->queue(new(pipeline.get()) CL_PixelCommandSetFrameBuffer(colorbuffer0));
	CL_Rect rect = clip_rect;
	clip_rect = CL_Rect(CL_Point(0,0),colorbuffer0.size);
	if (cliprect_set)
		set_clip_rect(rect);
	else
		pipeline->queue(new(pipeline.get()) CL_PixelCommandSetClipRect(clip_rect));
}

void CL_PixelCanvas::reset_framebuffer()
{
	pipeline->wait_for_workers();

	framebuffer_set = false;
	slot_framebuffer_modified = CL_Slot();
	colorbuffer0.set(primary_colorbuffer0);
	pipeline->queue(new(pipeline.get()) CL_PixelCommandSetFrameBuffer(colorbuffer0));

	framebuffer = CL_FrameBuffer();

	CL_Rect rect = clip_rect;
	clip_rect = CL_Rect(CL_Point(0,0),colorbuffer0.size);
	if (cliprect_set)
		set_clip_rect(rect);
	else
		pipeline->queue(new(pipeline.get()) CL_PixelCommandSetClipRect(clip_rect));
}

void CL_PixelCanvas::clear(const CL_Colorf &color)
{
	pipeline->queue(new(pipeline.get()) CL_PixelCommandClear(color));
}

void CL_PixelCanvas::draw_pixels(const CL_Rect &dest, const CL_PixelBuffer &image, const CL_Rect &src_rect, const CL_Colorf &primary_color)
{
	if (image.get_format() == cl_argb8)
	{
		pipeline->queue(new(pipeline.get()) CL_PixelCommandPixels(dest, image, src_rect, primary_color));
		pipeline->wait_for_workers();
	}
	else
	{
		CL_PixelBuffer image_argb8 = image.to_format(cl_argb8);
		draw_pixels(dest, image_argb8, src_rect, primary_color);
	}
}

void CL_PixelCanvas::draw_pixels_bicubic(int x, int y, int zoom_number, int zoom_denominator, const CL_PixelBuffer &image)
{
	if (image.get_format() == cl_argb8)
	{
		pipeline->queue(new(pipeline.get()) CL_PixelCommandBicubic(x, y, zoom_number, zoom_denominator, image));
		pipeline->wait_for_workers();
	}
	else
	{
		CL_PixelBuffer image_argb8 = image.to_format(cl_argb8);
		draw_pixels_bicubic(x, y, zoom_number, zoom_denominator, image_argb8);
	}
}

void CL_PixelCanvas::queue_command(CL_UniquePtr<CL_PixelCommand> &command)
{
	pipeline->queue(command);
}

void CL_PixelCanvas::set_sampler(int index, const CL_PixelBuffer &new_sampler)
{
	pipeline->queue(new(pipeline.get()) CL_PixelCommandSetSampler(index, new_sampler));
}

void CL_PixelCanvas::reset_sampler(int index)
{
	pipeline->queue(new(pipeline.get()) CL_PixelCommandSetSampler(index));
}

CL_PixelBuffer &CL_PixelCanvas::to_pixelbuffer()
{
	pipeline->wait_for_workers();
	return colorbuffer0.pixelbuffer;
}

void CL_PixelCanvas::modified_framebuffer()
{
	pipeline->wait_for_workers();
	CL_SWRenderFrameBufferProvider *gdi_framebuffer = dynamic_cast<CL_SWRenderFrameBufferProvider *>(framebuffer.get_provider());

	colorbuffer0.set(gdi_framebuffer->get_colorbuffer0());
	pipeline->queue(new(pipeline.get()) CL_PixelCommandSetFrameBuffer(colorbuffer0));
	CL_Rect rect = clip_rect;
	clip_rect = CL_Rect(CL_Point(0,0),colorbuffer0.size);
	if (cliprect_set)
		set_clip_rect(rect);
	else
		pipeline->queue(new(pipeline.get()) CL_PixelCommandSetClipRect(clip_rect));
}
