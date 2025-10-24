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
**    Kenneth Gangstoe
*/

#pragma once


#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/Display/Font/font.h"
#include "API/Display/Render/blend_mode.h"
#include "API/Display/Render/buffer_control.h"
#include "API/Display/Render/polygon_rasterizer.h"
#include "API/Display/Render/pen.h"
#include "API/Core/System/comptr.h"
#include <d3d9.h>
#include <d3dx9tex.h>

class CL_D3D9DisplayWindowProvider;

class CL_D3D9GraphicContextProvider : public CL_GraphicContextProvider
{
/// \name Construction
/// \{

public:
	CL_D3D9GraphicContextProvider(CL_D3D9DisplayWindowProvider *window);

	~CL_D3D9GraphicContextProvider();


/// \}
/// \name Attributes
/// \{

public:
	int get_max_attributes();

	CL_Size get_max_texture_size() const;

	int get_width() const;

	int get_height() const;

	HDC get_drawable() const;


/// \}
/// \name Operations
/// \{

public:
	void destroy();

	CL_GraphicContext create_worker_gc();

	CL_PixelBuffer get_pixeldata(const CL_Rect& rect) const;

	CL_TextureProvider *alloc_texture(CL_TextureDimensions texture_dimensions);

	CL_OcclusionQueryProvider *alloc_occlusion_query();

	CL_ProgramObjectProvider *alloc_program_object();

	CL_ShaderObjectProvider *alloc_shader_object();

	CL_FrameBufferProvider *alloc_frame_buffer();

	CL_RenderBufferProvider *alloc_render_buffer();

	CL_PixelBufferProvider *alloc_pixel_buffer();

	CL_VertexArrayBufferProvider *alloc_vertex_array_buffer();

	CL_ElementArrayBufferProvider *alloc_element_array_buffer();

	void set_program_object(CL_StandardProgram standard_program);

	void set_program_object(const CL_ProgramObject &program, int program_matrix_flags);

	void reset_program_object();

	void set_texture(int unit_index, const CL_Texture &texture);

	void reset_texture(int unit_index);

	void set_frame_buffer(const CL_FrameBuffer &w_buffer, const CL_FrameBuffer &r_buffer);

	void reset_frame_buffer();

	void set_blend_mode(const CL_BlendMode &blendmode);

	void set_buffer_control(const CL_BufferControl &buffer_control);

	void set_pen(const CL_Pen &pen);

	void set_polygon_rasterizer(const CL_PolygonRasterizer &raster);

	void draw_primitives(CL_PrimitivesType type, int num_vertices, const CL_PrimitivesArrayData * const prim_array);

	void set_primitives_array(const CL_PrimitivesArrayData * const prim_array);

	void draw_primitives_array(CL_PrimitivesType type, int offset, int num_vertices);

	void draw_primitives_array_instanced(CL_PrimitivesType type, int offset, int num_vertices, int instance_count);

	void draw_primitives_elements(CL_PrimitivesType type, int count, unsigned int *indices);

	void draw_primitives_elements(CL_PrimitivesType type, int count, unsigned short *indices);

	void draw_primitives_elements(CL_PrimitivesType type, int count, unsigned char *indices);

	void draw_primitives_elements(CL_PrimitivesType type, int count, CL_ElementArrayBufferProvider *array_provider, CL_VertexAttributeDataType indices_type, void *offset);

	void primitives_array_freed(const CL_PrimitivesArrayData * const prim_array);

	void reset_primitives_array();

	void draw_pixels(float x, float y, float zoom_x, float zoom_y, const CL_PixelBuffer &pixel_buffer, const CL_Rect &src_rect, const CL_Colorf &color);

	void set_clip_rect(const CL_Rect &rect);

	void reset_clip_rect();

	void clear(const CL_Colorf &color);

	void clear_depth(float value);

	void clear_stencil(int value);

	void set_map_mode(CL_MapMode mode);

	void set_viewport(const CL_Rectf &viewport);

	void set_projection(const CL_Mat4f &matrix);

	void set_modelview(const CL_Mat4f &matrix);

	void on_window_resized();

	static void to_d3d_matrix(CL_Mat4f *matrix, D3DMATRIX *d3d_matrix);

	static void to_d3d_matrix(CL_Mat4f *matrix, D3DXMATRIX *d3d_matrix);

	static DWORD to_d3d_blend_func(CL_BlendFunc blend_func);

	static DWORD to_d3d_blend_op(CL_BlendEquation blend_equation);

	static DWORD to_d3d_cmp_func(CL_CompareFunction compare_func);

	static DWORD to_d3d_stencil_op(CL_StencilOp stencil_op);

	static DWORD to_d3d_cull_mode(CL_CullMode cull_mode, CL_FaceSide face_side);

	static DWORD to_d3d_fill_mode(CL_FillMode fill_mode);


/// \}
/// \name Implementation
/// \{

private:
	void add_vertelement(const CL_PrimitivesArrayData::VertexData &data, BYTE usage, BYTE usage_index);

	struct StreamSource
	{
		StreamSource() : offset(0), stride(0) { }

		CL_ComPtr<IDirect3DVertexBuffer9> buffer;
		unsigned int offset;
		int stride;
	};

	CL_D3D9DisplayWindowProvider *window;
	D3DCAPS9 caps;
	D3DSURFACE_DESC backbuffer_desc;
	CL_Mat4f projection_matrix, modelview_matrix;
	CL_MapMode map_mode;
	std::vector<D3DVERTEXELEMENT9> vertex_elements;
	std::vector<StreamSource> streams;
	CL_ComPtr<IDirect3DVertexDeclaration9> vertex_declaration;
/// \}
};



