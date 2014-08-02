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
*/

#pragma once

#include "API/SWRender/pixel_buffer_data.h"
#include "API/Core/Math/vec3.h"
#include "API/Core/Math/mat4.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/Render/frame_buffer.h"
#include "API/Display/Render/blend_state.h"
#include "API/Display/2D/color.h"

namespace clan
{

class PixelPipeline;
class PixelCommand;
class ProgramObject_SWRender;

class PixelCanvas
{
public:
	PixelCanvas(const Size &size);
	~PixelCanvas();

	void resize(const Size &size);

	Size get_size() const { return colorbuffer0.size; }

	void set_clip_rect(const Rect &new_clip_rect);
	void reset_clip_rect();

	void set_blend_function(BlendFunc src, BlendFunc dest, BlendFunc src_alpha, BlendFunc dest_alpha, const Colorf &const_color);

	void set_framebuffer(const FrameBuffer &buffer);
	void reset_framebuffer();

	void clear(const Colorf &color);
	void draw_pixels(const Rect &dest, const PixelBuffer &image, const Rect &src_rect, const Colorf &primary_color);
	void draw_pixels_bicubic(int x, int y, int zoom_number, int zoom_denominator, const PixelBuffer &pixels);
	void queue_command(std::unique_ptr<PixelCommand> &command);

	void set_sampler(int index, const PixelBuffer &new_sampler);
	void reset_sampler(int index);

	PixelBuffer &to_pixelbuffer();
	PixelPipeline *get_pipeline() const { return pipeline.get(); }

private:
	void modified_framebuffer();

	PixelBuffer primary_colorbuffer0;
	PixelBufferData colorbuffer0;
	bool framebuffer_set;
	FrameBuffer framebuffer;

	Rect clip_rect;
	bool cliprect_set;

	BlendFunc cur_blend_src;
	BlendFunc cur_blend_dest;
	BlendFunc cur_blend_src_alpha;
	BlendFunc cur_blend_dest_alpha;
	Colorf cur_blend_color;
    SlotContainer sc;

	std::unique_ptr<PixelPipeline> pipeline;
};

}
