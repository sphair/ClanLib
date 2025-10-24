/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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

#include "API/Core/Math/vec3.h"
#include "API/Core/Math/mat4.h"
#include "API/Core/System/event.h"
#include "API/Core/System/thread.h"
#include "API/Core/Signals/slot.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/Render/frame_buffer.h"
#include "fragment_buffer.h"

class CL_Rasterizer;

class CL_PixelPipeline
{
public:
	CL_PixelPipeline(const CL_Size &size);
	~CL_PixelPipeline();

	void resize(const CL_Size &size);

	int get_width() const { return colorbuffer0.size.width; }
	int get_height() const { return colorbuffer0.size.height; }

	void set_clip_rect(const CL_Rect &new_clip_rect);
	void reset_clip_rect();

	void set_framebuffer(const CL_FrameBuffer &buffer);
	void reset_framebuffer();

	void clear(const CL_Colorf &color);
	void draw_pixels(const CL_Rect &dest, const CL_PixelBufferRef &image, const CL_Colorf &primary_color);
	void draw_triangle(const CL_Vec2f points[3], const CL_Vec4f primcolor[4], const CL_Vec2f texcoords[3], int sampler);
	void draw_sprite(const CL_Vec2f points[3], const CL_Vec4f primcolor[4], const CL_Vec2f texcoords[3], int sampler);

	const CL_Mat4f &get_modelview() const { return modelview; }
	const CL_Mat4f &get_projection() const { return projection; }
	void set_modelview(const CL_Mat4f &new_modelview);
	void set_projection(const CL_Mat4f &new_projection);

	CL_Vec2f transform(const CL_Vec4f &vertex) const;
	void set_sampler(int index, const CL_PixelBuffer &new_sampler);
	void reset_sampler(int index);

	CL_PixelBuffer &to_pixelbuffer();

	enum { num_varying = 6 };

private:
	static int get_fragment_vertices() { return max_shaded_vertices/FragmentBuffer::num_fragments; }
	static int get_fragment_vertex_offset(int fragment) { return fragment*get_fragment_vertices(); }

	CL_Mat4f modelview;
	CL_Mat4f projection;
	mutable CL_Mat4f modelview_projection;
	mutable bool modelview_projection_invalid;

	CL_PixelBuffer primary_colorbuffer0;
	CL_Rect clip_rect;

	struct PixelBufferData
	{
		PixelBufferData() : data(0) { }

		CL_PixelBuffer pixelbuffer;
		CL_Size size;
		unsigned int *data;

		void set(const CL_PixelBuffer &new_pixelbuffer)
		{
			pixelbuffer = new_pixelbuffer;
			size = pixelbuffer.get_size();
			data = static_cast<unsigned int *>(pixelbuffer.get_data());
		}
	};

	CL_PixelBuffer pixelbuffer_white;
	PixelBufferData colorbuffer0;
	enum { max_samplers = 6 };
	PixelBufferData samplers[max_samplers];

	bool framebuffer_set;
	bool cliprect_set;
	CL_FrameBuffer framebuffer;
	CL_Slot slot_framebuffer_modified;

	struct ShadedVertex
	{
		CL_Vec2f position;
		float varying[num_varying];
		unsigned char sampler;
		bool sprite_program;
	};

	enum { max_shaded_vertices = 3*1024*FragmentBuffer::num_fragments, max_cores = 32 };

	ShadedVertex vertices[max_shaded_vertices];
	FragmentBuffer fragment_buffer;
	int current_writer_fragment;
	int write_pos;
	int active_cores;
	CL_Event event_stop;
	std::vector<CL_Thread> worker_threads;

	static int get_num_cores();
	void wait_for_workers();
	void worker_main(int core);

	void process_vertices(CL_Rasterizer &rasterizer, int core, int num_cores);

	int find_first_line_for_core(int y_start, int core, int num_cores);

	void fill_rect(const CL_Rect &dest, const CL_Colorf &primary_color, int core, int num_cores);
	void draw_image(const CL_Rect &dest, const CL_PixelBufferRef &image, const CL_Rect &src, const CL_Colorf &primary_color, int core=0, int num_cores=1);

	void modified_framebuffer();

};
