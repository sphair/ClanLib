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
#include "gdi_graphic_context_provider.h"
#include "gdi_display_window_provider.h"
#include "gdi_texture_provider.h"
#include "gdi_element_array_buffer_provider.h"
#include "gdi_frame_buffer_provider.h"
#include "gdi_occlusion_query_provider.h"
#include "gdi_program_object_provider.h"
#include "gdi_render_buffer_provider.h"
#include "gdi_shader_object_provider.h"
#include "gdi_vertex_array_buffer_provider.h"
//#include "Display/Font/font_provider_win32.h"
#include "Display/Font/font_cache_native.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_metrics.h"

/////////////////////////////////////////////////////////////////////////////
// CL_GDIGraphicContextProvider Construction:

CL_GDIGraphicContextProvider::CL_GDIGraphicContextProvider(CL_GDIDisplayWindowProvider *window)
: window(window), pixel_canvas(0), current_prim_array(0), modelview_matrix(CL_Mat4f::identity()),
  is_sprite_program(false)
{
	pixel_canvas = new CL_PixelPipeline(window->get_viewport().get_size());
}

CL_GDIGraphicContextProvider::~CL_GDIGraphicContextProvider()
{
	delete pixel_canvas;
}

/////////////////////////////////////////////////////////////////////////////
// CL_GDIGraphicContextProvider Attributes:

int CL_GDIGraphicContextProvider::get_max_attributes()
{
	return 4;
}

CL_Size CL_GDIGraphicContextProvider::get_max_texture_size() const
{
	return CL_Size(8*1024, 8*1024);
}

int CL_GDIGraphicContextProvider::get_width() const
{
	return pixel_canvas->get_width();
}

int CL_GDIGraphicContextProvider::get_height() const
{
	return pixel_canvas->get_height();
}

#ifdef WIN32
HDC CL_GDIGraphicContextProvider::get_drawable() const
{
	return 0; //static_cast<CL_GDIDisplayWindowProvider *>(window)->get_device_context();
}
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_GDIGraphicContextProvider Operations:

void CL_GDIGraphicContextProvider::destroy()
{
	delete this;
}

CL_GraphicContext CL_GDIGraphicContextProvider::create_worker_gc()
{
	return CL_GraphicContext();
}

CL_PixelBuffer CL_GDIGraphicContextProvider::get_pixeldata(const CL_Rect& rect)
{
	return pixel_canvas->to_pixelbuffer().get_subimage(rect);
}

CL_TextureProvider *CL_GDIGraphicContextProvider::alloc_texture(CL_TextureDimensions texture_dimensions)
{
	return new CL_GDITextureProvider;
}

CL_OcclusionQueryProvider *CL_GDIGraphicContextProvider::alloc_occlusion_query()
{
	return new CL_GDIOcclusionQueryProvider;
}

CL_ProgramObjectProvider *CL_GDIGraphicContextProvider::alloc_program_object()
{
	return new CL_GDIProgramObjectProvider;
}

CL_ShaderObjectProvider *CL_GDIGraphicContextProvider::alloc_shader_object()
{
	return new CL_GDIShaderObjectProvider;
}

CL_FrameBufferProvider *CL_GDIGraphicContextProvider::alloc_frame_buffer()
{
	return new CL_GDIFrameBufferProvider;
}

CL_RenderBufferProvider *CL_GDIGraphicContextProvider::alloc_render_buffer()
{
	return new CL_GDIRenderBufferProvider;
}

CL_VertexArrayBufferProvider *CL_GDIGraphicContextProvider::alloc_vertex_array_buffer()
{
	return new CL_GDIVertexArrayBufferProvider;
}

CL_ElementArrayBufferProvider *CL_GDIGraphicContextProvider::alloc_element_array_buffer()
{
	return new CL_GDIElementArrayBufferProvider;
}

void CL_GDIGraphicContextProvider::set_program_object(CL_StandardProgram standard_program)
{
	if (standard_program == cl_program_sprite)
		is_sprite_program = true;
	else
		is_sprite_program = false;
}

void CL_GDIGraphicContextProvider::set_program_object(const CL_ProgramObject &program)
{
	is_sprite_program = false;
}

void CL_GDIGraphicContextProvider::reset_program_object()
{
	is_sprite_program = false;
}

void CL_GDIGraphicContextProvider::set_texture(int unit_index, const CL_Texture &texture)
{
	bound_textures[unit_index] = texture;
	pixel_canvas->set_sampler(unit_index, dynamic_cast<CL_GDITextureProvider*>(bound_textures[unit_index].get_provider())->get_image());
}

void CL_GDIGraphicContextProvider::reset_texture(int unit_index)
{
	std::map<int, CL_Texture>::iterator it = bound_textures.find(unit_index);
	if (it != bound_textures.end())
		bound_textures.erase(it);
	pixel_canvas->reset_sampler(unit_index);
}

void CL_GDIGraphicContextProvider::set_frame_buffer(const CL_FrameBuffer &buffer)
{
	pixel_canvas->set_framebuffer(buffer);
}

void CL_GDIGraphicContextProvider::reset_frame_buffer()
{
	pixel_canvas->reset_framebuffer();
}

void CL_GDIGraphicContextProvider::set_blend_mode(const CL_BlendMode &blendmode)
{
}

void CL_GDIGraphicContextProvider::set_buffer_control(const CL_BufferControl &buffer_control)
{
}

void CL_GDIGraphicContextProvider::set_pen(const CL_Pen &pen)
{
}

void CL_GDIGraphicContextProvider::set_polygon_rasterizer(const CL_PolygonRasterizer &raster)
{
}

void CL_GDIGraphicContextProvider::draw_primitives(CL_PrimitivesType type, int num_vertices, const CL_PrimitivesArrayData * const prim_array)
{
	set_primitives_array(prim_array);
	draw_primitives_array(type, 0, num_vertices);
	reset_primitives_array();
}

void CL_GDIGraphicContextProvider::set_primitives_array(const CL_PrimitivesArrayData * const prim_array)
{
	current_prim_array = prim_array;
	pos_fetcher.bind(current_prim_array, 0);
	color_fetcher.bind(current_prim_array, 1);
	tex_fetcher.bind(current_prim_array, 2);
	tex_index_fetcher.bind(current_prim_array, 3);
}

void CL_GDIGraphicContextProvider::draw_primitives_array(CL_PrimitivesType type, int offset, int num_vertices)
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
}

void CL_GDIGraphicContextProvider::draw_primitives_elements(CL_PrimitivesType type, int count, unsigned int *indices)
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
}

void CL_GDIGraphicContextProvider::draw_primitives_elements(CL_PrimitivesType type, int count, unsigned short *indices)
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
}

void CL_GDIGraphicContextProvider::draw_primitives_elements(CL_PrimitivesType type, int count, unsigned char *indices)
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
}

void CL_GDIGraphicContextProvider::draw_primitives_elements(CL_PrimitivesType type, int count, CL_ElementArrayBufferProvider *array_provider, CL_VertexAttributeDataType indices_type, void *offset)
{
}

void CL_GDIGraphicContextProvider::primitives_array_freed(const CL_PrimitivesArrayData * const prim_array)
{
}

void CL_GDIGraphicContextProvider::reset_primitives_array()
{
	pos_fetcher.clear();
	tex_fetcher.clear();
	color_fetcher.clear();
	tex_index_fetcher.clear();
	current_prim_array = 0;
}

void CL_GDIGraphicContextProvider::draw_pixels(float x, float y, float zoom_x, float zoom_y, const CL_PixelBufferRef &pixel_buffer, const CL_Colorf &color)
{
	CL_Vec4f pos(x, y, 1.0, 1.0);
	CL_Vec2f screen_pos = pixel_canvas->transform(pos);

	CL_Rect dest(CL_Point((int)screen_pos.x,(int)screen_pos.y),CL_Size((int)(pixel_buffer.get_width()*zoom_x),(int)(pixel_buffer.get_height()*zoom_y)));
	pixel_canvas->draw_pixels(dest, pixel_buffer, color);
}

void CL_GDIGraphicContextProvider::set_clip_rect(const CL_Rect &rect)
{
	pixel_canvas->set_clip_rect(rect);
}

void CL_GDIGraphicContextProvider::reset_clip_rect()
{
	pixel_canvas->reset_clip_rect();
}

void CL_GDIGraphicContextProvider::clear(const CL_Colorf &color)
{
	pixel_canvas->clear(color);
}

void CL_GDIGraphicContextProvider::clear_depth(float value)
{
}

void CL_GDIGraphicContextProvider::clear_stencil(int value)
{
}

void CL_GDIGraphicContextProvider::set_map_mode(CL_MapMode mode)
{
}

void CL_GDIGraphicContextProvider::set_viewport(const CL_Rectf &viewport)
{
}

void CL_GDIGraphicContextProvider::set_projection(const CL_Mat4f &matrix)
{
}

void CL_GDIGraphicContextProvider::set_modelview(const CL_Mat4f &matrix)
{
	modelview_matrix = matrix;
	pixel_canvas->set_modelview(matrix);
}

void CL_GDIGraphicContextProvider::on_window_resized()
{
	pixel_canvas->resize(window->get_viewport().get_size());
}

/////////////////////////////////////////////////////////////////////////////
// CL_GDIGraphicContextProvider Implementation:

void CL_GDIGraphicContextProvider::draw_triangle(int index1, int index2, int index3)
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
		screen_pos[v] = pixel_canvas->transform(pos[v]);

	pixel_canvas->draw_triangle(screen_pos, primary_color, tex_coords, sampler_index.x);
}

void CL_GDIGraphicContextProvider::draw_sprite(int index1, int index2, int index3)
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
		screen_pos[v] = pixel_canvas->transform(pos[v]);

	pixel_canvas->draw_sprite(screen_pos, primary_color, tex_coords, sampler_index.x);
}
