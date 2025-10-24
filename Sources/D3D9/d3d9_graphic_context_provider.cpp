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
**    Kenneth Gangstoe
*/

#include "D3D9/precomp.h"
#include "d3d9_graphic_context_provider.h"
#include "API/Core/System/comptr.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_metrics.h"
#include "d3d9_display_window_provider.h"
#include "d3d9_texture_provider.h"
#include "d3d9_vertex_array_buffer_provider.h"

/////////////////////////////////////////////////////////////////////////////
// CL_D3D9GraphicContextProvider Construction:

CL_D3D9GraphicContextProvider::CL_D3D9GraphicContextProvider(CL_D3D9DisplayWindowProvider *window)
: window(window), projection_matrix(CL_Mat4f::identity()), modelview_matrix(CL_Mat4f::identity()), map_mode(cl_user_projection)
{
	memset(&backbuffer_desc, 0, sizeof(D3DSURFACE_DESC));
	memset(&caps, 0, sizeof(D3DCAPS9));

	CL_ComPtr<IDirect3DSurface9> surface;
	HRESULT result = window->get_device()->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, surface.output_variable());
	if (FAILED(result))
		throw CL_Exception("Unable to get D3D back buffer");
	result = surface->GetDesc(&backbuffer_desc);
	if (FAILED(result))
		throw CL_Exception("Unable to get D3D back buffer surface description");
	result = window->get_device()->GetDeviceCaps(&caps);
	if (FAILED(result))
		throw CL_Exception("Unable to get D3D device capabilities");

}

CL_D3D9GraphicContextProvider::~CL_D3D9GraphicContextProvider()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_D3D9GraphicContextProvider Attributes:

int CL_D3D9GraphicContextProvider::get_max_attributes()
{
	return 8;
}

CL_Size CL_D3D9GraphicContextProvider::get_max_texture_size() const
{
	return CL_Size(caps.MaxTextureWidth, caps.MaxTextureHeight);
}

int CL_D3D9GraphicContextProvider::get_width() const
{
	return backbuffer_desc.Width;
}

int CL_D3D9GraphicContextProvider::get_height() const
{
	return backbuffer_desc.Height;
}

HDC CL_D3D9GraphicContextProvider::get_drawable() const
{
//	return static_cast<CL_D3D9DisplayWindowProvider *>(window)->get_device_context();
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CL_D3D9GraphicContextProvider Operations:

void CL_D3D9GraphicContextProvider::destroy()
{
	delete this;
}

CL_GraphicContext CL_D3D9GraphicContextProvider::create_worker_gc()
{
	return CL_GraphicContext(new CL_D3D9GraphicContextProvider(window));
}

CL_PixelBuffer CL_D3D9GraphicContextProvider::get_pixeldata(const CL_Rect& rect) const 
{
	return CL_PixelBuffer();
}

CL_TextureProvider *CL_D3D9GraphicContextProvider::alloc_texture(CL_TextureDimensions texture_dimensions)
{
	return new CL_D3D9TextureProvider(this, texture_dimensions);
}

CL_OcclusionQueryProvider *CL_D3D9GraphicContextProvider::alloc_occlusion_query()
{
	return 0;
}

CL_ProgramObjectProvider *CL_D3D9GraphicContextProvider::alloc_program_object()
{
	return 0;
}

CL_ShaderObjectProvider *CL_D3D9GraphicContextProvider::alloc_shader_object()
{
	return 0;
}

CL_FrameBufferProvider *CL_D3D9GraphicContextProvider::alloc_frame_buffer()
{
	return 0;
}

CL_RenderBufferProvider *CL_D3D9GraphicContextProvider::alloc_render_buffer()
{
	return 0;
}

CL_PixelBufferProvider *CL_D3D9GraphicContextProvider::alloc_pixel_buffer()
{
	return NULL;
}

CL_VertexArrayBufferProvider *CL_D3D9GraphicContextProvider::alloc_vertex_array_buffer()
{
	return new CL_D3D9VertexArrayBufferProvider(window);
}

CL_ElementArrayBufferProvider *CL_D3D9GraphicContextProvider::alloc_element_array_buffer()
{
	return 0;
}

void CL_D3D9GraphicContextProvider::set_program_object(CL_StandardProgram standard_program)
{
}

void CL_D3D9GraphicContextProvider::set_program_object(const CL_ProgramObject &program, int program_matrix_flags)
{
}

void CL_D3D9GraphicContextProvider::reset_program_object()
{
}

void CL_D3D9GraphicContextProvider::set_texture(int unit_index, const CL_Texture &texture)
{
}

void CL_D3D9GraphicContextProvider::reset_texture(int unit_index)
{
}

void CL_D3D9GraphicContextProvider::set_frame_buffer(const CL_FrameBuffer &w_buffer, const CL_FrameBuffer &r_buffer)
{
}

void CL_D3D9GraphicContextProvider::reset_frame_buffer()
{
}

void CL_D3D9GraphicContextProvider::set_blend_mode(const CL_BlendMode &blendmode)
{
	BOOL blend_enabled = blendmode.is_blend_enabled() ? TRUE : FALSE;
	window->get_device()->SetRenderState(D3DRS_ALPHABLENDENABLE, blend_enabled);
	window->get_device()->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, blend_enabled);
	window->get_device()->SetRenderState(D3DRS_BLENDOP, to_d3d_blend_op(blendmode.get_blend_equation()));
	window->get_device()->SetRenderState(D3DRS_BLENDOPALPHA, to_d3d_blend_op(blendmode.get_blend_equation_alpha()));
	window->get_device()->SetRenderState(D3DRS_SRCBLEND, to_d3d_blend_func(blendmode.get_blend_function_src()));
	window->get_device()->SetRenderState(D3DRS_SRCBLENDALPHA, to_d3d_blend_func(blendmode.get_blend_function_src_alpha()));
	window->get_device()->SetRenderState(D3DRS_DESTBLEND, to_d3d_blend_func(blendmode.get_blend_function_dest()));
	window->get_device()->SetRenderState(D3DRS_DESTBLENDALPHA, to_d3d_blend_func(blendmode.get_blend_function_dest_alpha()));
}

void CL_D3D9GraphicContextProvider::set_buffer_control(const CL_BufferControl &buffer_control)
{
	BOOL stencil_enabled = buffer_control.is_stencil_test_enabled() ? TRUE : FALSE;
	window->get_device()->SetRenderState(D3DRS_STENCILENABLE, stencil_enabled);
	window->get_device()->SetRenderState(D3DRS_TWOSIDEDSTENCILMODE, stencil_enabled);
	window->get_device()->SetRenderState(D3DRS_STENCILFUNC, to_d3d_cmp_func(buffer_control.get_stencil_compare_func_front()));
	window->get_device()->SetRenderState(D3DRS_CCW_STENCILFUNC, to_d3d_cmp_func(buffer_control.get_stencil_compare_func_back()));
	window->get_device()->SetRenderState(D3DRS_STENCILREF, buffer_control.get_stencil_compare_reference_front());
//	window->get_device()->SetRenderState(D3DRS_CCW_STENCILREF, buffer_control.get_stencil_compare_reference_back());
	window->get_device()->SetRenderState(D3DRS_STENCILFAIL, to_d3d_stencil_op(buffer_control.get_stencil_fail_front()));
	window->get_device()->SetRenderState(D3DRS_CCW_STENCILFAIL, to_d3d_stencil_op(buffer_control.get_stencil_fail_back()));
//	window->get_device()->SetRenderState(D3DRS_STENCILZPASS, to_d3d_stencil_op(buffer_control.get_stencil_pass_depth_pass_front()));
//	window->get_device()->SetRenderState(D3DRS_CCW_STENCILZPASS, to_d3d_stencil_op(buffer_control.get_stencil_pass_depth_pass_back()));
	window->get_device()->SetRenderState(D3DRS_STENCILZFAIL, to_d3d_stencil_op(buffer_control.get_stencil_pass_depth_fail_front()));
	window->get_device()->SetRenderState(D3DRS_CCW_STENCILZFAIL, to_d3d_stencil_op(buffer_control.get_stencil_pass_depth_fail_back()));
	window->get_device()->SetRenderState(D3DRS_STENCILMASK, buffer_control.get_stencil_compare_mask_front());
//	window->get_device()->SetRenderState(D3DRS_CCW_STENCILMASK, buffer_control.get_stencil_compare_mask_back());
	window->get_device()->SetRenderState(D3DRS_STENCILWRITEMASK, buffer_control.get_stencil_write_mask_front());
//	window->get_device()->SetRenderState(D3DRS_CCW_STENCILWRITEMASK, buffer_control.get_stencil_write_mask_back());

	window->get_device()->SetRenderState(D3DRS_ZENABLE, buffer_control.is_depth_test_enabled() ? TRUE : FALSE);
	window->get_device()->SetRenderState(D3DRS_ZWRITEENABLE, buffer_control.is_depth_write_enabled() ? TRUE : FALSE);
	window->get_device()->SetRenderState(D3DRS_ZFUNC, to_d3d_cmp_func(buffer_control.get_depth_compare_function()));

	bool red = false, green = false, blue = false, alpha = false;
	buffer_control.is_color_write_enabled(red, green, blue, alpha);
	DWORD color_write_flags = 0;
	if (red) color_write_flags |= D3DCOLORWRITEENABLE_RED;
	if (green) color_write_flags |= D3DCOLORWRITEENABLE_GREEN;
	if (blue) color_write_flags |= D3DCOLORWRITEENABLE_BLUE;
	if (alpha) color_write_flags |= D3DCOLORWRITEENABLE_ALPHA;
	window->get_device()->SetRenderState(D3DRS_COLORWRITEENABLE , color_write_flags);
}

void CL_D3D9GraphicContextProvider::set_pen(const CL_Pen &pen)
{
	float point_size = pen.get_point_size();
//	float max_point_size = pen.get_max_point_size();
//	float min_point_size = pen.get_min_point_size();
//	float a, b, c;
//	pen.get_point_distance_attenuation(a, b, c);

	window->get_device()->SetRenderState(D3DRS_POINTSIZE, *((DWORD*) &point_size));
//	window->get_device()->SetRenderState(D3DRS_POINTSIZE_MAX, *((DWORD*) &max_point_size));
//	window->get_device()->SetRenderState(D3DRS_POINTSIZE_MIN, *((DWORD*) &min_point_size));
	window->get_device()->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE);
//	window->get_device()->SetRenderState(D3DRS_POINTSCALE_A, *((DWORD*) &a));
//	window->get_device()->SetRenderState(D3DRS_POINTSCALE_B, *((DWORD*) &b));
//	window->get_device()->SetRenderState(D3DRS_POINTSCALE_C, *((DWORD*) &c));
//	window->get_device()->SetRenderState(D3DRS_POINTSPRITEENABLE, pen.is_using_point_sprites() ? TRUE : FALSE);
	window->get_device()->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, pen.is_line_antialiased() ? TRUE : FALSE);
/*
	float get_point_fade_treshold_size() const;
	bool is_point_antialiased() const;
	float get_line_width() const;
	unsigned short get_line_stipple_pattern() const; 
	int get_line_stipple_repeat_count() const;
	bool is_line_stippled() const;
	bool is_using_vertex_program_point_sizes() const;
	CL_PointSpriteOrigin get_point_sprite_origin() const;
*/
}

void CL_D3D9GraphicContextProvider::set_polygon_rasterizer(const CL_PolygonRasterizer &raster)
{
	if (raster.is_culled())
		window->get_device()->SetRenderState(D3DRS_CULLMODE, to_d3d_cull_mode(raster.get_face_cull_mode(), raster.get_front_face()));
	else
		window->get_device()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (raster.get_front_face() == cl_face_side_clockwise)
		window->get_device()->SetRenderState(D3DRS_FILLMODE, to_d3d_fill_mode(raster.get_face_fill_mode_front()));
	else
		window->get_device()->SetRenderState(D3DRS_FILLMODE, to_d3d_fill_mode(raster.get_face_fill_mode_back()));

	window->get_device()->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, raster.is_antialiased() ? TRUE : FALSE);
	window->get_device()->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, (DWORD) raster.get_offset_factor());
	window->get_device()->SetRenderState(D3DRS_DEPTHBIAS, (DWORD) raster.get_offset_units());
/*
	bool is_point_offset() const;
	bool is_line_offset() const;
	bool is_polygon_offset() const;
*/
}

void CL_D3D9GraphicContextProvider::draw_primitives(CL_PrimitivesType type, int num_vertices, const CL_PrimitivesArrayData * const prim_array)
{
	set_primitives_array(prim_array);
	draw_primitives_array(type, 0, num_vertices);
	reset_primitives_array();
}

void CL_D3D9GraphicContextProvider::set_primitives_array(const CL_PrimitivesArrayData * const prim_array)
{
	D3DVERTEXELEMENT9 end_element = D3DDECL_END();

	vertex_declaration.clear();
	vertex_elements.clear();
	streams.clear();
/*
	add_vertelement(prim_array->position, D3DDECLUSAGE_POSITION, 0);
	add_vertelement(prim_array->normal, D3DDECLUSAGE_NORMAL, 0);
	add_vertelement(prim_array->primary_color, D3DDECLUSAGE_COLOR, 0);
	for (int i = 0; i < prim_array->num_tex_coords; i++)
		add_vertelement(prim_array->tex_coords[i], D3DDECLUSAGE_TEXCOORD, prim_array->tex_coord_indexes[i]);
*/
	for (int i = 0; i < prim_array->num_attributes; i++)
		add_vertelement(prim_array->attributes[i], D3DDECLUSAGE_TEXCOORD, prim_array->attribute_indexes[i]);
	vertex_elements.push_back(end_element);

	HRESULT result = window->get_device()->CreateVertexDeclaration(&vertex_elements[0], vertex_declaration.output_variable());
	if (FAILED(result))
		throw CL_Exception("Unable to create vertex declaration");

	std::vector<StreamSource>::size_type pos, size = streams.size();
	for (pos = 0; pos < size; pos++)
	{
		result = window->get_device()->SetStreamSource(pos, streams[pos].buffer, streams[pos].offset, streams[pos].stride);
		if (FAILED(result))
			throw CL_Exception("Unable to set stream source");
	}
	result = window->get_device()->SetVertexDeclaration(vertex_declaration);
	if (FAILED(result))
		throw CL_Exception("Unable to set vertex declaration");
}

void CL_D3D9GraphicContextProvider::draw_primitives_array_instanced(CL_PrimitivesType type, int offset, int num_vertices, int instance_count)
{
}

void CL_D3D9GraphicContextProvider::add_vertelement(const CL_PrimitivesArrayData::VertexData &data, BYTE usage, BYTE usage_index)
{
	if (data.array_provider)
	{
		CL_D3D9VertexArrayBufferProvider *buffer_provider = static_cast<CL_D3D9VertexArrayBufferProvider*>(data.array_provider);
		StreamSource source;
		source.buffer = buffer_provider->get_buffer();
		source.offset = reinterpret_cast<unsigned int>(data.data);
		source.stride = data.stride;

		D3DVERTEXELEMENT9 element;
		element.Stream = streams.size();
		element.Offset = 0;
		element.Method = D3DDECLMETHOD_DEFAULT;
		element.Type = D3DDECLTYPE_UNUSED;
		switch (data.type)
		{
		case cl_type_unsigned_byte:
			if (data.size == 4)
			{
				element.Type = D3DDECLTYPE_UBYTE4;
				if (source.stride == 0)
					source.stride = 4;
			}
			break;
		case cl_type_unsigned_short: break;
		case cl_type_unsigned_int: break;
		case cl_type_byte: break;
		case cl_type_short:
			if (data.size == 2)
			{
				element.Type = D3DDECLTYPE_SHORT2;
				if (source.stride == 0)
					source.stride = sizeof(short) * 2;
			}
			else if (data.size == 4)
			{
				element.Type = D3DDECLTYPE_SHORT4;
				if (source.stride == 0)
					source.stride = sizeof(short) * 4;
			}
			break;
		case cl_type_int: break;
		case cl_type_float:
			element.Type = D3DDECLTYPE_FLOAT1 + data.size-1;
			if (source.stride == 0)
				source.stride = sizeof(float) * data.size;
			break;
		default: break;
		}
		element.Usage = usage;
		element.UsageIndex = usage_index;

		streams.push_back(source);
		vertex_elements.push_back(element);
	}
	else
	{
		// todo: fill data into a dynamic vertex buffer
		// throw CL_Exception("Currently only vertex array buffers are supported in primitive arrays");
/*
		CL_ComPtr<IDirect3DVertexDeclaration9> vertex_buffer;
		HRESULT result = window->get_device()->CreateVertexBuffer(size, D3DUSAGE_DYNAMIC, D3DUSAGE_WRITEONLY, D3DPOOL_DEFAULT, vertex_buffer.output_variable(), 0);
		if (FAILED(result))
			throw CL_Exception("Unable to create vertex buffer");
*/
	}
}

void CL_D3D9GraphicContextProvider::draw_primitives_array(CL_PrimitivesType type, int offset, int num_vertices)
{
	HRESULT result = E_FAIL;
	switch (type)
	{
	case cl_points:
		result = window->get_device()->DrawPrimitive(D3DPT_POINTLIST, offset, num_vertices);
		break;
	case cl_line_strip:
		result = window->get_device()->DrawPrimitive(D3DPT_LINESTRIP, offset, num_vertices-1);
		break;
	case cl_line_loop:
		break;
	case cl_lines:
		result = window->get_device()->DrawPrimitive(D3DPT_LINELIST, offset, num_vertices/2);
		break;
	case cl_triangle_strip:
		result = window->get_device()->DrawPrimitive(D3DPT_TRIANGLESTRIP, offset, num_vertices-2);
		break;
	case cl_triangle_fan:
		result = window->get_device()->DrawPrimitive(D3DPT_TRIANGLEFAN, offset, num_vertices-2);
		break;
	case cl_triangles:
		result = window->get_device()->DrawPrimitive(D3DPT_TRIANGLESTRIP, offset, num_vertices/3);
		break;
	}
	if (FAILED(result))
		throw CL_Exception("Unable to draw primitives array");
}

void CL_D3D9GraphicContextProvider::draw_primitives_elements(CL_PrimitivesType type, int count, unsigned int *indices)
{
	// hmm!
	// CL_ComPtr<IDirect3DIndexBuffer> index_buffer;
	// result = window->get_device()->CreateIndexBuffer(size, D3DUSAGE_DYNAMIC, D3DFMT_INDEX32, D3DPOOL_DEFAULT, index_buffer.output_variable(), 0);
	// result = window->get_device()->SetIndices(index_buffer);
	// result = window->get_device()->DrawIndexedPrimitive(type, reinterpret_cast<int>(offset), 0, count, 0, count/3);
}

void CL_D3D9GraphicContextProvider::draw_primitives_elements(CL_PrimitivesType type, int count, unsigned short *indices)
{
}

void CL_D3D9GraphicContextProvider::draw_primitives_elements(CL_PrimitivesType type, int count, unsigned char *indices)
{
}

void CL_D3D9GraphicContextProvider::draw_primitives_elements(CL_PrimitivesType type, int count, CL_ElementArrayBufferProvider *array_provider, CL_VertexAttributeDataType indices_type, void *offset)
{
	// hmm!
	// result = window->get_device()->SetIndices(index_buffer);
	// result = window->get_device()->DrawIndexedPrimitive(type, reinterpret_cast<int>(offset), 0, count, 0, count/3);
}

void CL_D3D9GraphicContextProvider::primitives_array_freed(const CL_PrimitivesArrayData * const prim_array)
{
}

void CL_D3D9GraphicContextProvider::reset_primitives_array()
{
	vertex_declaration.clear();
	vertex_elements.clear();
	streams.clear();
}

void CL_D3D9GraphicContextProvider::draw_pixels(float x, float y, float zoom_x, float zoom_y, const CL_PixelBuffer &pixel_buffer, const CL_Rect &src_rect, const CL_Colorf &color)
{
}

void CL_D3D9GraphicContextProvider::set_clip_rect(const CL_Rect &rect)
{
	RECT d3d_rect = { rect.left, rect.top, rect.right, rect.bottom };
	window->get_device()->SetScissorRect(&d3d_rect);
	window->get_device()->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
}

void CL_D3D9GraphicContextProvider::reset_clip_rect()
{
	window->get_device()->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
}

void CL_D3D9GraphicContextProvider::clear(const CL_Colorf &color)
{
	DWORD i_red = (DWORD) (color.r * 255);
	DWORD i_green = (DWORD) (color.g * 255);
	DWORD i_blue = (DWORD) (color.b * 255);
	DWORD i_alpha = (DWORD) (color.a * 255);
	D3DCOLOR colord = (i_alpha << 24) + (i_red << 16) + (i_green << 8) + (i_blue);
	window->get_device()->Clear(0, 0, D3DCLEAR_TARGET, colord, 0.0f, 0);
}

void CL_D3D9GraphicContextProvider::clear_depth(float value)
{
	window->get_device()->Clear(0, 0, D3DCLEAR_ZBUFFER, 0, (float) value, 0);
}

void CL_D3D9GraphicContextProvider::clear_stencil(int value)
{
	window->get_device()->Clear(0, 0, D3DCLEAR_STENCIL, 0, 0.0f, value);
}

void CL_D3D9GraphicContextProvider::set_map_mode(CL_MapMode mode)
{
	map_mode = mode;
	switch (map_mode)
	{
	case cl_map_2d_upper_left:
		{
			int width = get_width();
			int height = get_height();
			set_viewport(CL_Rect(0, 0, width, height));
			CL_Mat4f projection = CL_Mat4f::ortho_2d(0.0f, (float)width, (float)height, 0.0f);
			CL_Mat4f modelview = CL_Mat4f::translate(cl_pixelcenter_constant, cl_pixelcenter_constant, 0.0f);
			D3DMATRIX matrix;
			to_d3d_matrix(&projection, &matrix);
			window->get_device()->SetTransform(D3DTS_PROJECTION, &matrix);
			to_d3d_matrix(&modelview, &matrix);
			window->get_device()->SetTransform(D3DTS_VIEW, &matrix);
		}
		break;
	case cl_map_2d_lower_left:
		{
			int width = get_width();
			int height = get_height();
			set_viewport(CL_Rect(0, 0, width, height));
			CL_Mat4f projection = CL_Mat4f::ortho_2d(0.0f, (float)width, 0.0f, (float)height);
			CL_Mat4f modelview = CL_Mat4f::translate(cl_pixelcenter_constant, cl_pixelcenter_constant, 0.0f);
			D3DMATRIX matrix;
			to_d3d_matrix(&projection, &matrix);
			window->get_device()->SetTransform(D3DTS_PROJECTION, &matrix);
			to_d3d_matrix(&modelview, &matrix);
			window->get_device()->SetTransform(D3DTS_VIEW, &matrix);
		}
		break;
	case cl_user_projection:
		{
			D3DMATRIX matrix;
			to_d3d_matrix(&projection_matrix, &matrix);
			window->get_device()->SetTransform(D3DTS_PROJECTION, &matrix);
		}
		break;
	}
}

void CL_D3D9GraphicContextProvider::set_viewport(const CL_Rectf &viewport)
{
	D3DVIEWPORT9 viewport9;
	viewport9.X = (DWORD)viewport.left;
	viewport9.Y = (DWORD)viewport.top;
	viewport9.Width = (DWORD)viewport.get_width();
	viewport9.Height = (DWORD)viewport.get_height();
	viewport9.MinZ = 0.0f;
	viewport9.MaxZ = 1.0f;
	window->get_device()->SetViewport(&viewport9);
}

void CL_D3D9GraphicContextProvider::set_projection(const CL_Mat4f &matrix)
{
	projection_matrix = matrix;
	if (map_mode == cl_user_projection)
	{
		D3DMATRIX matrix;
		to_d3d_matrix(&projection_matrix, &matrix);
		window->get_device()->SetTransform(D3DTS_PROJECTION, &matrix);
	}
}

void CL_D3D9GraphicContextProvider::set_modelview(const CL_Mat4f &matrix)
{
	modelview_matrix = matrix;
}

void CL_D3D9GraphicContextProvider::on_window_resized()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_D3D9GraphicContextProvider Implementation:

void CL_D3D9GraphicContextProvider::to_d3d_matrix(CL_Mat4f *matrix, D3DMATRIX *d3d_matrix)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			d3d_matrix->m[i][j] = *matrix[i*4+j];
}

void CL_D3D9GraphicContextProvider::to_d3d_matrix(CL_Mat4f *matrix, D3DXMATRIX *d3d_matrix)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			d3d_matrix->m[i][j] = *matrix[i*4+j];
}

DWORD CL_D3D9GraphicContextProvider::to_d3d_blend_func(CL_BlendFunc blend_func)
{
	switch (blend_func)
	{
	case cl_blend_zero:
		return D3DBLEND_ZERO;
	case cl_blend_one:
		return D3DBLEND_ONE;
	case cl_blend_dest_color:
		return D3DBLEND_DESTCOLOR;
	case cl_blend_src_color:
		return D3DBLEND_SRCCOLOR;
	case cl_blend_one_minus_dest_color:
		return D3DBLEND_INVDESTCOLOR;
	case cl_blend_one_minus_src_color:
		return D3DBLEND_INVSRCCOLOR;
	case cl_blend_src_alpha:
		return D3DBLEND_SRCALPHA;
	case cl_blend_one_minus_src_alpha:
		return D3DBLEND_INVSRCALPHA;
	case cl_blend_dest_alpha:
		return D3DBLEND_DESTALPHA;
	case cl_blend_one_minus_dest_alpha:
		return D3DBLEND_INVDESTALPHA;
	case cl_blend_src_alpha_saturate:
		return D3DBLEND_SRCALPHASAT;
	case cl_blend_constant_color:
	case cl_blend_constant_alpha:
		return D3DBLEND_BLENDFACTOR;
	case cl_blend_one_minus_constant_color:
	case cl_blend_one_minus_constant_alpha:
		return D3DBLEND_INVBLENDFACTOR;
	default:
		return 0;
	}
}

DWORD CL_D3D9GraphicContextProvider::to_d3d_blend_op(CL_BlendEquation blend_equation)
{
	switch (blend_equation)
	{
	case cl_blend_equation_add:
		return D3DBLENDOP_ADD;
	case cl_blend_equation_subtract:
		return D3DBLENDOP_SUBTRACT;
	case cl_blend_equation_reverse_subtract:
		return D3DBLENDOP_REVSUBTRACT;
	case cl_blend_equation_min:
		return D3DBLENDOP_MIN;
	case cl_blend_equation_max:
		return D3DBLENDOP_MAX;
	default:
		return 0;
	}
}

DWORD CL_D3D9GraphicContextProvider::to_d3d_cmp_func(CL_CompareFunction compare_func)
{
	switch (compare_func)
	{
	case cl_comparefunc_lequal:
		return D3DCMP_LESSEQUAL;
	case cl_comparefunc_gequal:
		return D3DCMP_GREATEREQUAL;
	case cl_comparefunc_less:
		return D3DCMP_LESS;
	case cl_comparefunc_greater:
		return D3DCMP_GREATER;
	case cl_comparefunc_equal:
		return D3DCMP_EQUAL;
	case cl_comparefunc_notequal:
		return D3DCMP_NOTEQUAL;
	case cl_comparefunc_always:
		return D3DCMP_ALWAYS;
	case cl_comparefunc_never:
		return D3DCMP_NEVER;
	default:
		return 0;
	}
}

DWORD CL_D3D9GraphicContextProvider::to_d3d_stencil_op(CL_StencilOp stencil_op)
{
	switch (stencil_op)
	{
	case cl_stencil_keep:
		return D3DSTENCILOP_KEEP;
	case cl_stencil_zero:
		return D3DSTENCILOP_ZERO;
	case cl_stencil_replace:
		return D3DSTENCILOP_REPLACE;
	case cl_stencil_incr:
		return D3DSTENCILOP_INCRSAT;
	case cl_stencil_decr:
		return D3DSTENCILOP_DECRSAT;
	case cl_stencil_invert:
		return D3DSTENCILOP_INVERT;
	case cl_stencil_incr_wrap:
		return D3DSTENCILOP_INCR;
	case cl_stencil_decr_wrap:
		return D3DSTENCILOP_DECR;
	default:
		return 0;
	}
}

DWORD CL_D3D9GraphicContextProvider::to_d3d_cull_mode(CL_CullMode cull_mode, CL_FaceSide face_side)
{
	switch (face_side)
	{
	case cl_face_side_clockwise:
		switch (cull_mode)
		{
		case cl_cull_front:
			return D3DCULL_CCW;
		case cl_cull_back:
			return D3DCULL_CW;
		case cl_cull_front_and_back:
			return D3DCULL_NONE;
		default:
			return 0;
		}
	case cl_face_side_counter_clockwise:
		switch (cull_mode)
		{
		case cl_cull_front:
			return D3DCULL_CW;
		case cl_cull_back:
			return D3DCULL_CCW;
		case cl_cull_front_and_back:
			return D3DCULL_NONE;
		default:
			return 0;
		}
	default:
		return 0;
	}
}

DWORD CL_D3D9GraphicContextProvider::to_d3d_fill_mode(CL_FillMode fill_mode)
{
	switch (fill_mode)
	{
	case cl_fill_point:
		return D3DFILL_POINT;
	case cl_fill_line:
		return D3DFILL_WIREFRAME;
	case cl_fill_polygon:
		return D3DFILL_SOLID;
	default:
		return 0;
	}
}
