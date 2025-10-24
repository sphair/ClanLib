/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
#include "swr_graphic_context_provider.h"
#include "swr_display_window_provider.h"
#include "swr_texture_provider.h"
#include "swr_element_array_buffer_provider.h"
#include "swr_pixel_buffer_provider.h"
#include "swr_frame_buffer_provider.h"
#include "swr_occlusion_query_provider.h"
#include "swr_program_object_provider.h"
#include "swr_render_buffer_provider.h"
#include "swr_shader_object_provider.h"
#include "swr_vertex_array_buffer_provider.h"
#include "Canvas/pixel_canvas.h"
#include "API/SWRender/pixel_command.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/Render/blend_mode.h"
#include "API/SWRender/swr_program_object.h"

/////////////////////////////////////////////////////////////////////////////
// CL_SWRenderGraphicContextProvider Construction:

CL_SWRenderGraphicContextProvider::CL_SWRenderGraphicContextProvider(CL_SWRenderDisplayWindowProvider *window)
: window(window), current_prim_array(0), modelview_matrix(CL_Mat4f::identity()), is_sprite_program(false)
{
	canvas.reset(new CL_PixelCanvas(window->get_viewport().get_size()));
}

CL_SWRenderGraphicContextProvider::~CL_SWRenderGraphicContextProvider()
{
	canvas.reset();
}

/////////////////////////////////////////////////////////////////////////////
// CL_SWRenderGraphicContextProvider Attributes:

int CL_SWRenderGraphicContextProvider::get_max_attributes()
{
	return 4;
}

CL_Size CL_SWRenderGraphicContextProvider::get_max_texture_size() const
{
	return CL_Size(8*1024, 8*1024);
}

int CL_SWRenderGraphicContextProvider::get_width() const
{
	return canvas->get_width();
}

int CL_SWRenderGraphicContextProvider::get_height() const
{
	return canvas->get_height();
}

#ifdef WIN32
HDC CL_SWRenderGraphicContextProvider::get_drawable() const
{
	return 0; //static_cast<CL_SWRenderDisplayWindowProvider *>(window)->get_device_context();
}
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_SWRenderGraphicContextProvider Operations:

void CL_SWRenderGraphicContextProvider::destroy()
{
	delete this;
}

CL_GraphicContext CL_SWRenderGraphicContextProvider::create_worker_gc()
{
	return CL_GraphicContext();
}

CL_PixelBuffer CL_SWRenderGraphicContextProvider::get_pixeldata(const CL_Rect& rect) const
{
	return canvas->to_pixelbuffer().copy(rect);
}

CL_TextureProvider *CL_SWRenderGraphicContextProvider::alloc_texture(CL_TextureDimensions texture_dimensions)
{
	return new CL_SWRenderTextureProvider;
}

CL_OcclusionQueryProvider *CL_SWRenderGraphicContextProvider::alloc_occlusion_query()
{
	return new CL_SWRenderOcclusionQueryProvider;
}

CL_ProgramObjectProvider *CL_SWRenderGraphicContextProvider::alloc_program_object()
{
	return new CL_SWRenderProgramObjectProvider;
}

CL_ShaderObjectProvider *CL_SWRenderGraphicContextProvider::alloc_shader_object()
{
	return new CL_SWRenderShaderObjectProvider;
}

CL_FrameBufferProvider *CL_SWRenderGraphicContextProvider::alloc_frame_buffer()
{
	return new CL_SWRenderFrameBufferProvider;
}

CL_RenderBufferProvider *CL_SWRenderGraphicContextProvider::alloc_render_buffer()
{
	return new CL_SWRenderRenderBufferProvider;
}

CL_VertexArrayBufferProvider *CL_SWRenderGraphicContextProvider::alloc_vertex_array_buffer()
{
	return new CL_SWRenderVertexArrayBufferProvider;
}

CL_ElementArrayBufferProvider *CL_SWRenderGraphicContextProvider::alloc_element_array_buffer()
{
	return new CL_SWRenderElementArrayBufferProvider;
}

CL_PixelBufferProvider *CL_SWRenderGraphicContextProvider::alloc_pixel_buffer()
{
	return new CL_SWRenderPixelBufferProvider;
}

void CL_SWRenderGraphicContextProvider::set_program_object(CL_StandardProgram standard_program)
{
	if (standard_program == cl_program_sprite)
		is_sprite_program = true;
	else
		is_sprite_program = false;
}

void CL_SWRenderGraphicContextProvider::set_program_object(const CL_ProgramObject &program, int program_matrix_flags)
{
	CL_ProgramObject_SWRender swr_program(program);
	is_sprite_program = swr_program.is_sprite_program();
	canvas->set_program_object(swr_program);
}

void CL_SWRenderGraphicContextProvider::reset_program_object()
{
	is_sprite_program = false;
	canvas->reset_program_object();
}

void CL_SWRenderGraphicContextProvider::set_texture(int unit_index, const CL_Texture &texture)
{
	bound_textures[unit_index] = texture;
	canvas->set_sampler(unit_index, dynamic_cast<CL_SWRenderTextureProvider*>(bound_textures[unit_index].get_provider())->get_image());
}

void CL_SWRenderGraphicContextProvider::reset_texture(int unit_index, const CL_Texture &texture)
{
	std::map<int, CL_Texture>::iterator it = bound_textures.find(unit_index);
	if (it != bound_textures.end())
		bound_textures.erase(it);
	canvas->reset_sampler(unit_index);
}

void CL_SWRenderGraphicContextProvider::set_frame_buffer(const CL_FrameBuffer &write_buffer, const CL_FrameBuffer &read_buffer)
{
	canvas->set_framebuffer(write_buffer);
}

void CL_SWRenderGraphicContextProvider::reset_frame_buffer()
{
	canvas->reset_framebuffer();
}

void CL_SWRenderGraphicContextProvider::set_blend_mode(const CL_BlendMode &blendmode)
{
	if (blendmode.is_blend_enabled())
	{
		canvas->set_blend_function(
			blendmode.get_blend_function_src(),
			blendmode.get_blend_function_dest(),
			blendmode.get_blend_function_src_alpha(),
			blendmode.get_blend_function_dest_alpha());
	}
	else
	{
		canvas->set_blend_function(cl_blend_one, cl_blend_zero, cl_blend_one, cl_blend_zero);
	}
}

void CL_SWRenderGraphicContextProvider::set_buffer_control(const CL_BufferControl &buffer_control)
{
}

void CL_SWRenderGraphicContextProvider::set_pen(const CL_Pen &pen)
{
}

void CL_SWRenderGraphicContextProvider::set_polygon_rasterizer(const CL_PolygonRasterizer &raster)
{
}

void CL_SWRenderGraphicContextProvider::draw_primitives(CL_PrimitivesType type, int num_vertices, const CL_PrimitivesArrayData * const prim_array)
{
	set_primitives_array(prim_array);
	draw_primitives_array(type, 0, num_vertices);
	reset_primitives_array();
}

void CL_SWRenderGraphicContextProvider::set_primitives_array(const CL_PrimitivesArrayData * const prim_array)
{
	current_prim_array = prim_array;
	pos_fetcher.bind(current_prim_array, 0);
	color_fetcher.bind(current_prim_array, 1);
	tex_fetcher.bind(current_prim_array, 2);
	tex_index_fetcher.bind(current_prim_array, 3);
}

void CL_SWRenderGraphicContextProvider::draw_primitives_array(CL_PrimitivesType type, int offset, int num_vertices)
{
	if (type == cl_triangles)
	{
		int end_vertices = offset+num_vertices;

		if (is_sprite_program)
		{
/*
			static CL_Vec4f default_pos(0.0f, 0.0f, 1.0f, 1.0f);
			static CL_Vec4f default_color(1.0f, 1.0f, 1.0f, 1.0f);
			static CL_Vec4f default_tex_coord(0.0f, 0.0f, 0.0f, 0.0f);
			static CL_Vec4f default_sampler(0.0f, 0.0f, 0.0f, 0.0f);

			int indexes[3] = { index1, index2, index3 };
			CL_Vec4f pos[3];
			CL_Vec4f primary_color[3];
			CL_Vec2f tex_coords[3];
			CL_Vec1f sampler_index;

			pos_fetcher->fetch(pos, indexes, 3, default_pos);
			color_fetcher->fetch(primary_color, indexes, 3, default_color);
			tex_fetcher->fetch(tex_coords, indexes, 3, default_tex_coord);
			tex_index_fetcher->fetch(&sampler_index, indexes, 1, default_sampler);

			CL_Vec2f screen_pos[3];
			for (int v=0; v<3; v++)
				screen_pos[v] = pixel_canvas->transform(pos[v]);

			pixel_canvas->draw_sprite(screen_pos, primary_color, tex_coords, sampler_index.x);
*/

			for (int i = offset; i+2 < end_vertices; i+=6)
				draw_sprite(i+0, i+1, i+2);
		}
		else
		{
			for (int i = offset; i+2 < end_vertices; i+=3)
				draw_triangle(i+0, i+1, i+2);
		}
	}
	else if (type == cl_lines)
	{
		int end_vertices = offset+num_vertices;

		for (int i = offset; i+1 < end_vertices; i+=2)
			draw_line(i+0, i+1);
	}
	else if (type == cl_line_loop)
	{
		int end_vertices = offset+num_vertices;
		int i;
		
		for (i = offset; i < end_vertices-1; i++)
		{
			draw_line(i, i+1);
		}
		draw_line(i, offset);
	}
}

void CL_SWRenderGraphicContextProvider::draw_primitives_array_instanced(CL_PrimitivesType type, int offset, int num_vertices, int instance_count)
{
}

void CL_SWRenderGraphicContextProvider::draw_primitives_elements(CL_PrimitivesType type, int count, unsigned int *indices)
{
	if (type == cl_triangles)
	{
		if (is_sprite_program)
		{
			for (int i = 0; i+2 < count; i+=6)
				draw_sprite(indices[i], indices[i+1], indices[i+2]);
		}
		else
		{
			for (int i = 0; i+2 < count; i+=3)
				draw_triangle(indices[i], indices[i+1], indices[i+2]);
		}
	}
	else if (type == cl_lines)
	{
			for (int i = 0; i+1 < count; i+=2)
				draw_line(indices[i], indices[i+1]);
	}
	else if (type == cl_line_loop)
	{
		int i;
		
		for (i = 0; i < count-1; i++)
		{
			draw_line(indices[i], indices[i+1]);
		}
		draw_line(indices[i], indices[0]);
	}
}

void CL_SWRenderGraphicContextProvider::draw_primitives_elements(CL_PrimitivesType type, int count, unsigned short *indices)
{
	if (type == cl_triangles)
	{
		if (is_sprite_program)
		{
			for (int i = 0; i+2 < count; i+=6)
				draw_sprite(indices[i], indices[i+1], indices[i+2]);
		}
		else
		{
			for (int i = 0; i+2 < count; i+=3)
				draw_triangle(indices[i], indices[i+1], indices[i+2]);
		}
	}
	else if (type == cl_lines)
	{
		for (int i = 0; i+1 < count; i+=2)
			draw_line(indices[i], indices[i+1]);
	}
	else if (type == cl_line_loop)
	{
		int i;
		
		for (i = 0; i < count-1; i++)
		{
			draw_line(indices[i], indices[i+1]);
		}
		draw_line(indices[i], indices[0]);
	}
}

void CL_SWRenderGraphicContextProvider::draw_primitives_elements(CL_PrimitivesType type, int count, unsigned char *indices)
{
	if (type == cl_triangles)
	{
		if (is_sprite_program)
		{
			for (int i = 0; i+2 < count; i+=6)
				draw_sprite(indices[i], indices[i+1], indices[i+2]);
		}
		else
		{
			for (int i = 0; i+2 < count; i+=3)
				draw_triangle(indices[i], indices[i+1], indices[i+2]);
		}
	}
	else if (type == cl_lines)
	{
		for (int i = 0; i+1 < count; i+=2)
			draw_line(indices[i], indices[i+1]);
	}
	else if (type == cl_line_loop)
	{
		int i;
		
		for (i = 0; i < count-1; i++)
		{
			draw_line(indices[i], indices[i+1]);
		}
		draw_line(indices[i], indices[0]);
	}
}

void CL_SWRenderGraphicContextProvider::draw_primitives_elements(CL_PrimitivesType type, int count, CL_ElementArrayBufferProvider *array_provider, CL_VertexAttributeDataType indices_type, void *offset)
{
}

void CL_SWRenderGraphicContextProvider::primitives_array_freed(const CL_PrimitivesArrayData * const prim_array)
{
}

void CL_SWRenderGraphicContextProvider::reset_primitives_array()
{
	pos_fetcher.clear();
	tex_fetcher.clear();
	color_fetcher.clear();
	tex_index_fetcher.clear();
	current_prim_array = 0;
}

void CL_SWRenderGraphicContextProvider::draw_pixels(CL_GraphicContext &gc, float x, float y, float zoom_x, float zoom_y, const CL_PixelBuffer &pixel_buffer, const CL_Rect &src_rect, const CL_Colorf &color)
{
	CL_Vec4f pos(x, y, 1.0, 1.0);
	CL_Vec2f screen_pos = canvas->transform(pos);

	CL_Rect dest(CL_Point((int)screen_pos.x,(int)screen_pos.y),CL_Size((int)(src_rect.get_width()*zoom_x),(int)(src_rect.get_height()*zoom_y)));
	canvas->draw_pixels(dest, pixel_buffer, src_rect, color);
}

void CL_SWRenderGraphicContextProvider::draw_pixels_bicubic(float x, float y, int zoom_number, int zoom_denominator, const CL_PixelBuffer &pixels)
{
	CL_Vec4f pos(x, y, 1.0, 1.0);
	CL_Vec2f screen_pos = canvas->transform(pos);
	canvas->draw_pixels_bicubic((int)screen_pos.x,(int)screen_pos.y, zoom_number, zoom_denominator, pixels);
}

void CL_SWRenderGraphicContextProvider::queue_command(std::auto_ptr<CL_PixelCommand> command)
{
	canvas->queue_command(command);
}

void CL_SWRenderGraphicContextProvider::set_clip_rect(const CL_Rect &rect)
{
	canvas->set_clip_rect(rect);
}

void CL_SWRenderGraphicContextProvider::reset_clip_rect()
{
	canvas->reset_clip_rect();
}

void CL_SWRenderGraphicContextProvider::clear(const CL_Colorf &color)
{
	canvas->clear(color);
}

void CL_SWRenderGraphicContextProvider::clear_depth(float value)
{
}

void CL_SWRenderGraphicContextProvider::clear_stencil(int value)
{
}

void CL_SWRenderGraphicContextProvider::set_map_mode(CL_MapMode mode)
{
}

void CL_SWRenderGraphicContextProvider::set_viewport(const CL_Rectf &viewport)
{
}

void CL_SWRenderGraphicContextProvider::set_projection(const CL_Mat4f &matrix)
{
}

void CL_SWRenderGraphicContextProvider::set_modelview(const CL_Mat4f &matrix)
{
	modelview_matrix = matrix;
	canvas->set_modelview(matrix);
}

void CL_SWRenderGraphicContextProvider::on_window_resized()
{
	canvas->resize(window->get_viewport().get_size());
}

/////////////////////////////////////////////////////////////////////////////
// CL_SWRenderGraphicContextProvider Implementation:

void CL_SWRenderGraphicContextProvider::draw_triangle(int index1, int index2, int index3)
{
	static CL_Vec4f default_pos(0.0f, 0.0f, 1.0f, 1.0f);
	static CL_Vec4f default_color(1.0f, 1.0f, 1.0f, 1.0f);
	static CL_Vec4f default_tex_coord(0.0f, 0.0f, 0.0f, 0.0f);
	static CL_Vec4f default_sampler(0.0f, 0.0f, 0.0f, 0.0f);

	int indexes[3] = { index1, index2, index3 };
	CL_Vec4f pos[3];
	CL_Vec4f primary_color[3];
	CL_Vec2f tex_coords[3];
	CL_Vec1f sampler_index;

	pos_fetcher->fetch(pos, indexes, 3, default_pos);
	color_fetcher->fetch(primary_color, indexes, 3, default_color);
	tex_fetcher->fetch(tex_coords, indexes, 3, default_tex_coord);
	tex_index_fetcher->fetch(&sampler_index, indexes, 1, default_sampler);

	CL_Vec2f screen_pos[3];
	for (int v=0; v<3; v++)
		screen_pos[v] = canvas->transform(pos[v]);

	canvas->draw_triangle(screen_pos, primary_color, tex_coords, sampler_index.x);
}

void CL_SWRenderGraphicContextProvider::draw_sprite(int index1, int index2, int index3)
{
	static CL_Vec4f default_pos(0.0f, 0.0f, 1.0f, 1.0f);
	static CL_Vec4f default_color(1.0f, 1.0f, 1.0f, 1.0f);
	static CL_Vec4f default_tex_coord(0.0f, 0.0f, 0.0f, 0.0f);
	static CL_Vec4f default_sampler(0.0f, 0.0f, 0.0f, 0.0f);

	int indexes[3] = { index1, index2, index3 };
	CL_Vec4f pos[3];
	CL_Vec4f primary_color[3];
	CL_Vec2f tex_coords[3];
	CL_Vec1f sampler_index;

	pos_fetcher->fetch(pos, indexes, 3, default_pos);
	color_fetcher->fetch(primary_color, indexes, 3, default_color);
	tex_fetcher->fetch(tex_coords, indexes, 3, default_tex_coord);
	tex_index_fetcher->fetch(&sampler_index, indexes, 1, default_sampler);

	CL_Vec2f screen_pos[3];
	for (int v=0; v<3; v++)
		screen_pos[v] = canvas->transform(pos[v]);

	// Check for non-rotated sprite
	if (   ( (int) screen_pos[0].y == (int) (screen_pos[1].y) )
		&& ( (int) screen_pos[0].x == (int) (screen_pos[2].x) ) )
	{
		canvas->draw_sprite(screen_pos, primary_color, tex_coords, sampler_index.x);
	}
	else
	{
		// Rotated Sprite.
		canvas->draw_triangle(screen_pos, primary_color, tex_coords, sampler_index.x);
		CL_Vec2f alt_screen_pos[3];
		alt_screen_pos[0] = screen_pos[1];
		alt_screen_pos[1].x = screen_pos[1].x + (screen_pos[2].x - screen_pos[0].x);
		alt_screen_pos[1].y = screen_pos[1].y + (screen_pos[2].y - screen_pos[0].y);
		alt_screen_pos[2] = screen_pos[2];

		CL_Vec2f alt_tex_coords[3];
		alt_tex_coords[0] = tex_coords[1];
		alt_tex_coords[1].x = tex_coords[1].x + (tex_coords[2].x - tex_coords[0].x);
		alt_tex_coords[1].y = tex_coords[1].y + (tex_coords[2].y - tex_coords[0].y);
		alt_tex_coords[2] = tex_coords[2];

		canvas->draw_triangle(alt_screen_pos, primary_color, alt_tex_coords, sampler_index.x);
	}
}

void CL_SWRenderGraphicContextProvider::draw_line(int index1, int index2)
{
	static CL_Vec4f default_pos(0.0f, 0.0f, 1.0f, 1.0f);
	static CL_Vec4f default_color(1.0f, 1.0f, 1.0f, 1.0f);
	static CL_Vec4f default_tex_coord(0.0f, 0.0f, 0.0f, 0.0f);
	static CL_Vec4f default_sampler(0.0f, 0.0f, 0.0f, 0.0f);

	int indexes[2] = { index1, index2 };
	CL_Vec4f pos[2];
	CL_Vec4f primary_color[2];
	CL_Vec2f tex_coords[2];
	CL_Vec1f sampler_index;

	pos_fetcher->fetch(pos, indexes, 2, default_pos);
	color_fetcher->fetch(primary_color, indexes, 2, default_color);
	tex_fetcher->fetch(tex_coords, indexes, 2, default_tex_coord);
	tex_index_fetcher->fetch(&sampler_index, indexes, 1, default_sampler);

	CL_Vec2f screen_pos[2];
	for (int v=0; v<2; v++)
		screen_pos[v] = canvas->transform(pos[v]);

	canvas->draw_line(screen_pos, primary_color, tex_coords, sampler_index.x);
}
