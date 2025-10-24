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

#include "GDI/precomp.h"
#include "API/Display/2D/color.h"
#include "API/Core/Math/cl_math.h"
#include "API/Core/Math/vec2.h"
#include "pixel_pipeline.h"
#include "draw_image.h"
#include "gdi_frame_buffer_provider.h"
#include "pixel_triangle_renderer.h"
#include "pixel_line_renderer.h"
#include <emmintrin.h>

CL_PixelPipeline::CL_PixelPipeline(const CL_Size &size)
: modelview(CL_Mat4f::identity()), projection(CL_Mat4f::identity()),
  modelview_projection(CL_Mat4f::identity()), modelview_projection_invalid(false),
  primary_colorbuffer0(size.width, size.height, size.width*4, CL_PixelFormat::argb8888),
  framebuffer_set(false), cliprect_set(false), current_writer_fragment(-1),
  write_pos(0), active_cores(1)
{
	int num_cores = CL_System::get_num_cores();
	active_cores = num_cores;
	fragment_buffer.set_num_readers(num_cores);
	if (active_cores > 1)
	{
		for (int core = 0; core < num_cores; core++)
		{
			CL_Thread worker_thread;
			worker_thread.start(this, &CL_PixelPipeline::worker_main, core);
			worker_threads.push_back(worker_thread);
		}
	}

	unsigned int white = 0xffffffff;
	pixelbuffer_white = CL_PixelBuffer(1, 1, 4, CL_PixelFormat::argb8888, &white);
	for (int i=0; i<max_samplers; i++)
		samplers[i].set(pixelbuffer_white);

	colorbuffer0.set(primary_colorbuffer0);
	clip_rect = CL_Rect(CL_Point(0,0), size);
	clear(CL_Colorf(0.0f,0.0f,0.0f,0.0f), 0, 1);
}

CL_PixelPipeline::~CL_PixelPipeline()
{
	event_stop.set();
	for (std::vector<CL_Thread>::size_type i = 0; i < worker_threads.size(); i++)
		worker_threads[i].join();
}

void CL_PixelPipeline::set_modelview(const CL_Mat4f &new_modelview)
{
	modelview = new_modelview;
	modelview_projection_invalid = true;
}

void CL_PixelPipeline::set_projection(const CL_Mat4f &new_projection)
{
	projection = new_projection;
	modelview_projection_invalid = true;
}

CL_Vec2f CL_PixelPipeline::transform(const CL_Vec4f &vertex) const
{
	if (modelview_projection_invalid)
	{
		modelview_projection = projection * modelview;
		modelview_projection_invalid = false;
	}

	CL_Vec4f v = modelview_projection * vertex;
	return CL_Vec2f(v.x, v.y);
}

void CL_PixelPipeline::resize(const CL_Size &size)
{
	CL_Size old_size = colorbuffer0.size;
	primary_colorbuffer0 = CL_PixelBuffer(size.width, size.height, size.width*4, CL_PixelFormat::argb8888);
	if (!framebuffer_set)
	{
		wait_for_workers();
		colorbuffer0.set(primary_colorbuffer0);
		CL_Rect rect = clip_rect;
		clip_rect = (CL_Point(0,0),size);
		if (cliprect_set)
			set_clip_rect(rect);
	}
}

void CL_PixelPipeline::set_framebuffer(const CL_FrameBuffer &buffer)
{
	wait_for_workers();

	CL_GDIFrameBufferProvider *gdi_framebuffer = dynamic_cast<CL_GDIFrameBufferProvider *>(buffer.get_provider());
	if (!gdi_framebuffer)
	{
		throw CL_Exception(cl_text("Invalid FrameBuffer"));
	}

	framebuffer_set = true;
	framebuffer = buffer;

	slot_framebuffer_modified = gdi_framebuffer->get_sig_changed_event().connect(this, &CL_PixelPipeline::modified_framebuffer);

	colorbuffer0.set(gdi_framebuffer->get_colorbuffer0());
	CL_Rect rect = clip_rect;
	clip_rect = (CL_Point(0,0),colorbuffer0.size);
	if (cliprect_set)
		set_clip_rect(rect);
}

void CL_PixelPipeline::modified_framebuffer()
{
	wait_for_workers();
	CL_GDIFrameBufferProvider *gdi_framebuffer = dynamic_cast<CL_GDIFrameBufferProvider *>(framebuffer.get_provider());

	colorbuffer0.set(gdi_framebuffer->get_colorbuffer0());
	CL_Rect rect = clip_rect;
	clip_rect = (CL_Point(0,0),colorbuffer0.size);
	if (cliprect_set)
		set_clip_rect(rect);
}

void CL_PixelPipeline::reset_framebuffer()
{
	wait_for_workers();

	framebuffer_set = false;
	slot_framebuffer_modified = CL_Slot();
	colorbuffer0.set(primary_colorbuffer0);

	framebuffer = CL_FrameBuffer();

	CL_Rect rect = clip_rect;
	clip_rect = (CL_Point(0,0),colorbuffer0.size);
	if (cliprect_set)
		set_clip_rect(rect);
}

void CL_PixelPipeline::set_clip_rect(const CL_Rect &new_clip_rect)
{
	wait_for_workers();
	clip_rect = CL_Rect(0, 0, colorbuffer0.size.width, colorbuffer0.size.height);
	clip_rect.overlap(new_clip_rect);
	if (clip_rect.get_width() < 0 || clip_rect.get_height() < 0)
		clip_rect = CL_Rect(0,0,0,0);
	cliprect_set = true;
}

void CL_PixelPipeline::reset_clip_rect()
{
	wait_for_workers();
	clip_rect = CL_Rect(0, 0, colorbuffer0.size.width, colorbuffer0.size.height);
	cliprect_set = false;
}

void CL_PixelPipeline::clear(const CL_Colorf &color)
{
	for (int v=0; v<3; v++)
	{
		while (current_writer_fragment == -1)
		{
			current_writer_fragment = fragment_buffer.get_writer_fragment();
			if (current_writer_fragment == -1)
				wait_for_workers();
		}

		int fragment_vertices = get_fragment_vertices();
		int fragment_offset = get_fragment_vertex_offset(current_writer_fragment);

		vertices[fragment_offset+write_pos].varying[2] = color.r;
		vertices[fragment_offset+write_pos].varying[3] = color.g;
		vertices[fragment_offset+write_pos].varying[4] = color.b;
		vertices[fragment_offset+write_pos].varying[5] = color.a;
		vertices[fragment_offset+write_pos].program = type_clear;
		write_pos++;

		if (write_pos == fragment_vertices)
		{
			fragment_buffer.finish_writer_fragment(write_pos);
			current_writer_fragment = -1;
			write_pos = 0;
		}
	}
}

CL_PixelBuffer &CL_PixelPipeline::to_pixelbuffer()
{
	wait_for_workers();
	return colorbuffer0.pixelbuffer;
}

void CL_PixelPipeline::set_sampler(int index, const CL_PixelBuffer &new_sampler)
{
	if (index >= 0 && index < max_samplers)
	{
		wait_for_workers();
		samplers[index].set(new_sampler);
	}
}

void CL_PixelPipeline::reset_sampler(int index)
{
	if (index >= 0 && index < max_samplers)
	{
		wait_for_workers();
		samplers[index].set(pixelbuffer_white);
	}
}

void CL_PixelPipeline::draw_pixels(const CL_Rect &dest, const CL_PixelBufferRef &image, const CL_Colorf &primary_color)
{
	wait_for_workers();

	if (image.get_format() == CL_PixelFormat::argb8888)
	{
		draw_image(dest, image, CL_Rect(CL_Point(0,0), image.get_size()), primary_color);
	}
	else
	{
		CL_PixelBuffer image_argb8888 = image.to_format(CL_PixelFormat::argb8888);
		draw_pixels(dest, image_argb8888, primary_color);
	}
}

void CL_PixelPipeline::draw_triangle(const CL_Vec2f points[3], const CL_Vec4f primcolor[3], const CL_Vec2f texcoords[3], int sampler)
{
	for (int v=0; v<3; v++)
	{
		while (current_writer_fragment == -1)
		{
			current_writer_fragment = fragment_buffer.get_writer_fragment();
			if (current_writer_fragment == -1)
				wait_for_workers();
		}

		int fragment_vertices = get_fragment_vertices();
		int fragment_offset = get_fragment_vertex_offset(current_writer_fragment);

		vertices[fragment_offset+write_pos].position = points[v];
		vertices[fragment_offset+write_pos].varying[0] = texcoords[v].x;
		vertices[fragment_offset+write_pos].varying[1] = texcoords[v].y;
		vertices[fragment_offset+write_pos].varying[2] = primcolor[v].r;
		vertices[fragment_offset+write_pos].varying[3] = primcolor[v].g;
		vertices[fragment_offset+write_pos].varying[4] = primcolor[v].b;
		vertices[fragment_offset+write_pos].varying[5] = primcolor[v].a;
		vertices[fragment_offset+write_pos].sampler = sampler;
		vertices[fragment_offset+write_pos].program = type_triangle;
		write_pos++;

		if (write_pos == fragment_vertices)
		{
			fragment_buffer.finish_writer_fragment(write_pos);
			current_writer_fragment = -1;
			write_pos = 0;
		}
	}
}

void CL_PixelPipeline::draw_sprite(const CL_Vec2f points[3], const CL_Vec4f primcolor[3], const CL_Vec2f texcoords[3], int sampler)
{
	for (int v=0; v<3; v++)
	{
		while (current_writer_fragment == -1)
		{
			current_writer_fragment = fragment_buffer.get_writer_fragment();
			if (current_writer_fragment == -1)
				wait_for_workers();
		}

		int fragment_vertices = get_fragment_vertices();
		int fragment_offset = get_fragment_vertex_offset(current_writer_fragment);

		vertices[fragment_offset+write_pos].position = points[v];
		vertices[fragment_offset+write_pos].varying[0] = texcoords[v].x;
		vertices[fragment_offset+write_pos].varying[1] = texcoords[v].y;
		vertices[fragment_offset+write_pos].varying[2] = primcolor[v].r;
		vertices[fragment_offset+write_pos].varying[3] = primcolor[v].g;
		vertices[fragment_offset+write_pos].varying[4] = primcolor[v].b;
		vertices[fragment_offset+write_pos].varying[5] = primcolor[v].a;
		vertices[fragment_offset+write_pos].sampler = sampler;
		vertices[fragment_offset+write_pos].program = type_sprite;
		write_pos++;

		if (write_pos == fragment_vertices)
		{
			fragment_buffer.finish_writer_fragment(write_pos);
			current_writer_fragment = -1;
			write_pos = 0;
		}
	}
}

void CL_PixelPipeline::draw_line(const CL_Vec2f points[2], const CL_Vec4f primcolor[2], const CL_Vec2f texcoords[2], int sampler)
{
	for (int v=0; v<2; v++)
	{
		while (current_writer_fragment == -1)
		{
			current_writer_fragment = fragment_buffer.get_writer_fragment();
			if (current_writer_fragment == -1)
				wait_for_workers();
		}

		int fragment_vertices = get_fragment_vertices();
		int fragment_offset = get_fragment_vertex_offset(current_writer_fragment);

		vertices[fragment_offset+write_pos].position = points[v];
		vertices[fragment_offset+write_pos].varying[0] = texcoords[v].x;
		vertices[fragment_offset+write_pos].varying[1] = texcoords[v].y;
		vertices[fragment_offset+write_pos].varying[2] = primcolor[v].r;
		vertices[fragment_offset+write_pos].varying[3] = primcolor[v].g;
		vertices[fragment_offset+write_pos].varying[4] = primcolor[v].b;
		vertices[fragment_offset+write_pos].varying[5] = primcolor[v].a;
		vertices[fragment_offset+write_pos].sampler = sampler;
		vertices[fragment_offset+write_pos].program = type_line;
		write_pos++;

		if (write_pos == fragment_vertices)
		{
			fragment_buffer.finish_writer_fragment(write_pos);
			current_writer_fragment = -1;
			write_pos = 0;
		}
	}
}

void CL_PixelPipeline::wait_for_workers()
{
	if (current_writer_fragment != -1)
	{
		fragment_buffer.finish_writer_fragment(write_pos);
		current_writer_fragment = -1;
		write_pos = 0;
	}

	if (active_cores == 1)
		process_vertices(0, active_cores);
	
	// Wait until all workers are done.
	while (!fragment_buffer.readers_finished())
	{
		fragment_buffer.get_writer_event().wait();
		fragment_buffer.get_writer_event().reset();
	}
}

void CL_PixelPipeline::worker_main(int core)
{
//	SetThreadIdealProcessor(GetCurrentThread(), core);
//	SetThreadAffinityMask(GetCurrentThread(), 1 << core);
	while (true)
	{
		int wakeup_reason = CL_Event::wait(fragment_buffer.get_reader_event(core), event_stop);
		if (wakeup_reason != 0)
			break;
		fragment_buffer.get_reader_event(core).reset();
		process_vertices(core, active_cores);
	}
}

void CL_PixelPipeline::process_vertices(int core, int num_cores)
{
	while (true)
	{
		int fragment = fragment_buffer.get_reader_fragment(core);
		if (fragment == -1)
			break;

		int fragment_vertices = fragment_buffer.get_fragment_vertices(fragment);
		int fragment_offset = get_fragment_vertex_offset(fragment);

		int fragment_offset_end = fragment_offset + fragment_vertices;
		for (int i = fragment_offset; i < fragment_offset_end;)	// Note, this used to be "i+2 < fragment_offset_end", but lines have only 2 vertices. So lets assume the input data is correct :)
		{
			if (vertices[i].program == type_sprite)
			{
				CL_Rect dest(
					vertices[i].position.x,
					vertices[i].position.y,
					vertices[i+1].position.x,
					vertices[i+2].position.y);

				CL_Colorf color(vertices[i].varying[2], vertices[i].varying[3], vertices[i].varying[4], vertices[i].varying[5]);

				int texindex = vertices[i].sampler;
				if (texindex != 4)
				{
					CL_Rect src(
						vertices[i].varying[0]*samplers[texindex].size.width,
						vertices[i].varying[1]*samplers[texindex].size.height,
						vertices[i+1].varying[0]*samplers[texindex].size.width,
						vertices[i+2].varying[1]*samplers[texindex].size.height);

					draw_image(dest, samplers[texindex].pixelbuffer, src, color, core, num_cores);
				}
				else
				{
					fill_rect(dest, color, core, num_cores);
				}
				i+=3;
			}
			else if (vertices[i].program == type_clear)
			{
				CL_Colorf color(vertices[i].varying[2], vertices[i].varying[3], vertices[i].varying[4], vertices[i].varying[5]);
				clear(color, core, num_cores);
				i+=3;
			}
			else if (vertices[i].program == type_triangle)
			{
				float x[3] = { vertices[i].position.x, vertices[i+1].position.x, vertices[i+2].position.x };
				float y[3] = { vertices[i].position.y, vertices[i+1].position.y, vertices[i+2].position.y };
				float tx[3] = { vertices[i].varying[0], vertices[i+1].varying[0], vertices[i+2].varying[0] };
				float ty[3] = { vertices[i].varying[1], vertices[i+1].varying[1], vertices[i+2].varying[1] };
				float red[3] = { vertices[i].varying[2], vertices[i+1].varying[2], vertices[i+2].varying[2] };
				float green[3] = { vertices[i].varying[3], vertices[i+1].varying[3], vertices[i+2].varying[3] };
				float blue[3] = { vertices[i].varying[4], vertices[i+1].varying[4], vertices[i+2].varying[4] };
				float alpha[3] = { vertices[i].varying[5], vertices[i+1].varying[5], vertices[i+2].varying[5] };

				CL_PixelTriangleRenderer triangle_renderer;
				triangle_renderer.set_clip_rect(clip_rect);
				triangle_renderer.set_vertex_arrays(x,y,tx,ty,red,green,blue,alpha);
				triangle_renderer.set_dest(colorbuffer0.data, colorbuffer0.size.width, colorbuffer0.size.height);
				triangle_renderer.set_src(samplers[vertices[i].sampler].data, samplers[vertices[i].sampler].size.width, samplers[vertices[i].sampler].size.height);
				triangle_renderer.set_core(core, num_cores);
				triangle_renderer.render_triangle(0, 1, 2);

				i+=3;
			}
			else	// Must be type_line
			{
				CL_LineSegment2 line(
					CL_Vec2i(vertices[i].position.x, vertices[i].position.y),
					CL_Vec2i(vertices[i+1].position.x, vertices[i+1].position.y));

				CL_Colorf color(vertices[i].varying[2], vertices[i].varying[3], vertices[i].varying[4], vertices[i].varying[5]);

				// Textured line is currently not supported - So ignore the samplers

				CL_PixelLineRenderer line_renderer;
				line_renderer.set_clip_rect(clip_rect);
				line_renderer.set_dest(colorbuffer0.data, colorbuffer0.size.width, colorbuffer0.size.height);
				line_renderer.set_core(core, num_cores);
				line_renderer.draw_line(line, color);
				i+=2;
			}
		}

		fragment_buffer.finish_reader_fragment(core);
	}
}

int CL_PixelPipeline::find_first_line_for_core(int y_start, int core, int num_cores)
{
	int y = y_start / num_cores;
	y *= num_cores;
	y += core;
	if (y < y_start)
		y += num_cores;
	return y;
}

void CL_PixelPipeline::clear(const CL_Colorf &color, int core, int num_cores)
{
	int dest_buffer_width = colorbuffer0.size.width;
	int dest_buffer_height = colorbuffer0.size.height;
	unsigned char *dest_data = (unsigned char *) colorbuffer0.data;

	CL_Color c = color;
	unsigned int color8888 = (c.get_alpha() << 24) + (c.get_red() << 16) + (c.get_green() << 8) + c.get_blue();
	unsigned char *ptr_color8888 = (unsigned char *) &color8888;

	for (int y = core; y < dest_buffer_height; y += num_cores)
	{
		unsigned char *line = dest_data + y * dest_buffer_width * 4;
		unsigned int line_align = ((line) - ((unsigned char *) 0)) & 0xf; // A gcc safe way of obtaining an address
		int pos = 0;
		int length = dest_buffer_width*4;

		// Write single bytes until we are byte aligned:
		if (line_align)
		{
			int prefix_length = cl_min(length, (int) (16 - line_align));
			for (; pos < prefix_length; pos++)
				line[pos] = ptr_color8888[pos&0x3];
		}

		// Figure out how our 16 bytes should look like after we applied the alignment:
		unsigned int b0 = ptr_color8888[(pos+0)&0x3];
		unsigned int b1 = ptr_color8888[(pos+1)&0x3];
		unsigned int b2 = ptr_color8888[(pos+2)&0x3];
		unsigned int b3 = ptr_color8888[(pos+3)&0x3];
		__m128i c_sse = _mm_set1_epi32((b3<<24)+(b2<<16)+(b1<<8)+b0);

		// Fill 16 byte aligned:
		int align_length = length-pos-15;
		for (; pos < align_length; pos+=16)
			_mm_stream_si128((__m128i*)(line+pos), c_sse);

		// Fill remaining bytes:
		for (; pos < length; pos++)
			line[pos] = ptr_color8888[pos&0x3];
	}
}

void CL_PixelPipeline::fill_rect(const CL_Rect &dest, const CL_Colorf &primary_color, int core, int num_cores)
{
	int dest_buffer_width = colorbuffer0.size.width;
	int dest_buffer_height = colorbuffer0.size.height;
	unsigned int *dest_data = colorbuffer0.data;

	int start_x = cl_max(dest.left, clip_rect.left);
	int end_x = cl_min(dest.right, clip_rect.right);
	int start_y = cl_max(dest.top, clip_rect.top);
	int end_y = cl_min(dest.bottom, clip_rect.bottom);
	if (start_x < end_x && start_y < end_y)
	{
		int dest_y = find_first_line_for_core(start_y, core, num_cores);

		int delta_x = start_x-dest.left;
		int delta_y = dest_y-dest.top;

		unsigned int *dest_line = dest_data+dest_y*dest_buffer_width+start_x;

		int line_length = end_x-start_x;
		int dest_line_incr = dest_buffer_width * num_cores;

		unsigned int sred = (unsigned int) (primary_color.r*255);
		unsigned int sgreen = (unsigned int) (primary_color.g*255);
		unsigned int sblue = (unsigned int) (primary_color.b*255);
		unsigned int salpha = (unsigned int) (primary_color.a*255);

		if (salpha == 255)
		{
			unsigned int color = (salpha<<24) + (sred<<16) + (sgreen<<8) + sblue;
			while (dest_y < end_y)
			{
				for (int x = 0; x < line_length; x++)
					dest_line[x] = color;

				dest_y += num_cores;
				dest_line += dest_line_incr;
			}
		}
		else
		{
			unsigned int pos_salpha = salpha*256/255;
			unsigned int neg_salpha = 256-salpha;
			while (dest_y < end_y)
			{
				for (int x = 0; x < line_length; x++)
				{
					#define alpha_component(a) (((a)&0xff000000)>>24)
					#define red_component(a) (((a)&0x00ff0000)>>16)
					#define green_component(a) (((a)&0x0000ff00)>>8)
					#define blue_component(a) ((a)&0x000000ff)

					unsigned int dest_color = dest_line[x];
					unsigned int dred = red_component(dest_color);
					unsigned int dgreen = green_component(dest_color);
					unsigned int dblue = blue_component(dest_color);
					unsigned int dalpha = alpha_component(dest_color);

					unsigned red = (dred * neg_salpha + sred * pos_salpha) >> 8;
					unsigned green = (dgreen * neg_salpha + sgreen * pos_salpha) >> 8;
					unsigned blue = (dblue * neg_salpha + sblue * pos_salpha) >> 8;
					unsigned alpha = (dalpha * neg_salpha + salpha * pos_salpha) >> 8;
					dest_line[x] = (alpha<<24) + (red<<16) + (green<<8) + blue;
				}

				dest_y += num_cores;
				dest_line += dest_line_incr;
			}
		}
	}
}

void CL_PixelPipeline::draw_image(const CL_Rect &dest, const CL_PixelBufferRef &image, const CL_Rect &src, const CL_Colorf &primary_color, int core, int num_cores)
{
	CL_DrawImageParams params;
	params.num_cores = num_cores;
	params.src_x = src.left;
	params.src_y = src.top;
	params.dest_width = dest.get_width();
	params.dest_height = dest.get_height();
	params.src_width = src.get_width();
	params.src_height = src.get_height();
	params.primred = (int) (primary_color.r*256);
	params.primgreen = (int) (primary_color.g*256);
	params.primblue = (int) (primary_color.b*256);
	params.primalpha = (int) (primary_color.a*256);

	params.start_x = cl_max(dest.left, clip_rect.left);
	params.end_x = cl_min(dest.right, clip_rect.right);
	params.start_y = cl_max(dest.top, clip_rect.top);
	params.end_y = cl_min(dest.bottom, clip_rect.bottom);
	if (params.start_x < params.end_x && params.start_y < params.end_y)
	{
		params.dest_buffer_width = colorbuffer0.size.width;
		params.dest_buffer_height = colorbuffer0.size.height;
		params.src_buffer_width = image.get_width();
		params.src_buffer_height = image.get_height();
		params.dest_data = colorbuffer0.data;
		params.src_data = static_cast<const unsigned int *>(image.get_data());

		params.dest_y = find_first_line_for_core(params.start_y, core, num_cores);
		params.delta_x = params.start_x-dest.left;
		params.delta_y = params.dest_y-dest.top;

		if (dest.get_size() == src.get_size())
		{
			if (params.primred == 256 && params.primgreen == 256 && params.primblue == 256 && params.primalpha == 256)
				cl_draw_image_noscale_white(&params);
			else
				cl_draw_image_noscale(&params);
		}
		else
		{
			if (params.primred == 256 && params.primgreen == 256 && params.primblue == 256 && params.primalpha == 256)
				cl_draw_image_nearest_white(&params);
			else
				cl_draw_image_nearest(&params);
		}
	}
}

