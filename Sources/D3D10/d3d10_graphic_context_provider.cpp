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

#include "D3D10/precomp.h"
#include "d3d10_graphic_context_provider.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_metrics.h"

/////////////////////////////////////////////////////////////////////////////
// CL_D3D10GraphicContextProvider Construction:

CL_D3D10GraphicContextProvider::CL_D3D10GraphicContextProvider()
{
}

CL_D3D10GraphicContextProvider::~CL_D3D10GraphicContextProvider()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_D3D10GraphicContextProvider Attributes:

int CL_D3D10GraphicContextProvider::get_max_attributes()
{
	return 0;
}

CL_Size CL_D3D10GraphicContextProvider::get_max_texture_size() const
{
	return CL_Size();
}

int CL_D3D10GraphicContextProvider::get_width() const
{
	return 0;
}

int CL_D3D10GraphicContextProvider::get_height() const
{
	return 0;
}

HDC CL_D3D10GraphicContextProvider::get_drawable() const
{
	return NULL; //static_cast<CL_D3D10DisplayWindowProvider *>(window)->get_device_context();
}

/////////////////////////////////////////////////////////////////////////////
// CL_D3D10GraphicContextProvider Operations:

void CL_D3D10GraphicContextProvider::destroy()
{
	delete this;
}

CL_GraphicContext CL_D3D10GraphicContextProvider::create_worker_gc()
{
	return CL_GraphicContext();
}

CL_PixelBuffer CL_D3D10GraphicContextProvider::get_pixeldata(const CL_Rect& rect) const
{
	return CL_PixelBuffer();
}

CL_TextureProvider *CL_D3D10GraphicContextProvider::alloc_texture(CL_TextureDimensions texture_dimensions)
{
	return 0;
}

CL_OcclusionQueryProvider *CL_D3D10GraphicContextProvider::alloc_occlusion_query()
{
	return 0;
}

CL_ProgramObjectProvider *CL_D3D10GraphicContextProvider::alloc_program_object()
{
	return 0;
}

CL_ShaderObjectProvider *CL_D3D10GraphicContextProvider::alloc_shader_object()
{
	return 0;
}

CL_FrameBufferProvider *CL_D3D10GraphicContextProvider::alloc_frame_buffer()
{
	return 0;
}

CL_RenderBufferProvider *CL_D3D10GraphicContextProvider::alloc_render_buffer()
{
	return 0;
}

CL_VertexArrayBufferProvider *CL_D3D10GraphicContextProvider::alloc_vertex_array_buffer()
{
	return 0;
}

CL_ElementArrayBufferProvider *CL_D3D10GraphicContextProvider::alloc_element_array_buffer()
{
	return 0;
}

void CL_D3D10GraphicContextProvider::set_program_object(CL_StandardProgram standard_program)
{
}

void CL_D3D10GraphicContextProvider::set_program_object(const CL_ProgramObject &program)
{
}

void CL_D3D10GraphicContextProvider::reset_program_object()
{
}

void CL_D3D10GraphicContextProvider::set_texture(int unit_index, const CL_Texture &texture)
{
}

void CL_D3D10GraphicContextProvider::reset_texture(int unit_index)
{
}

void CL_D3D10GraphicContextProvider::set_frame_buffer(const CL_FrameBuffer &buffer)
{
}

void CL_D3D10GraphicContextProvider::reset_frame_buffer()
{
}

void CL_D3D10GraphicContextProvider::set_blend_mode(const CL_BlendMode &blendmode)
{
}

void CL_D3D10GraphicContextProvider::set_buffer_control(const CL_BufferControl &buffer_control)
{
}

void CL_D3D10GraphicContextProvider::set_pen(const CL_Pen &pen)
{
}

void CL_D3D10GraphicContextProvider::set_polygon_rasterizer(const CL_PolygonRasterizer &raster)
{
}

void CL_D3D10GraphicContextProvider::draw_primitives(CL_PrimitivesType type, int num_vertices, const CL_PrimitivesArrayData * const prim_array)
{
}

void CL_D3D10GraphicContextProvider::set_primitives_array(const CL_PrimitivesArrayData * const prim_array)
{
}

void CL_D3D10GraphicContextProvider::draw_primitives_array(CL_PrimitivesType type, int offset, int num_vertices)
{
}

void CL_D3D10GraphicContextProvider::draw_primitives_elements(CL_PrimitivesType type, int count, unsigned int *indices)
{
}

void CL_D3D10GraphicContextProvider::draw_primitives_elements(CL_PrimitivesType type, int count, unsigned short *indices)
{
}

void CL_D3D10GraphicContextProvider::draw_primitives_elements(CL_PrimitivesType type, int count, unsigned char *indices)
{
}

void CL_D3D10GraphicContextProvider::draw_primitives_elements(CL_PrimitivesType type, int count, CL_ElementArrayBufferProvider *array_provider, CL_VertexAttributeDataType indices_type, void *offset)
{
}

void CL_D3D10GraphicContextProvider::primitives_array_freed(const CL_PrimitivesArrayData * const prim_array)
{
}

void CL_D3D10GraphicContextProvider::reset_primitives_array()
{
}

void CL_D3D10GraphicContextProvider::draw_pixels(float x, float y, float zoom_x, float zoom_y, const CL_PixelBuffer &pixel_buffer, const CL_Rect &src_rect, const CL_Colorf &color)
{
}

void CL_D3D10GraphicContextProvider::set_clip_rect(const CL_Rect &rect)
{
}

void CL_D3D10GraphicContextProvider::reset_clip_rect()
{
}

void CL_D3D10GraphicContextProvider::clear(const CL_Colorf &color)
{
}

void CL_D3D10GraphicContextProvider::clear_depth(float value)
{
}

void CL_D3D10GraphicContextProvider::clear_stencil(int value)
{
}

void CL_D3D10GraphicContextProvider::set_map_mode(CL_MapMode mode)
{
}

void CL_D3D10GraphicContextProvider::set_viewport(const CL_Rectf &viewport)
{
}

void CL_D3D10GraphicContextProvider::set_projection(const CL_Mat4f &matrix)
{
}

void CL_D3D10GraphicContextProvider::set_modelview(const CL_Mat4f &matrix)
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_D3D10GraphicContextProvider Implementation:
