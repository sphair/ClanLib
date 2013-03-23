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

#pragma once


#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/Display/Font/font.h"
#include "API/SWRender/swr_program_object.h"
#include "API/Core/System/uniqueptr.h"
#include "vertex_attribute_fetcher.h"
#include "software_program_standard.h"
#include <map>

namespace clan
{

class CL_PixelCanvas;
class CL_PixelCommand;
class CL_SWRenderDisplayWindowProvider;
class CL_SWRenderProgramObjectProvider;

class CL_SWRenderGraphicContextProvider : public CL_GraphicContextProvider
{
/// \name Construction
/// \{
public:
	CL_SWRenderGraphicContextProvider(CL_SWRenderDisplayWindowProvider *window);
	~CL_SWRenderGraphicContextProvider();
/// \}

/// \name Attributes
/// \{
public:
	int get_max_attributes();
	CL_Size get_max_texture_size() const;
	int get_width() const;
	int get_height() const;
#ifdef WIN32
	HDC get_drawable() const;
#endif
	CL_PixelCanvas *get_canvas() const { return canvas.get(); }
/// \}

/// \name Operations
/// \{
public:
	void destroy();
	CL_PixelBuffer get_pixeldata(const CL_Rect& rect, CL_TextureFormat pixel_format, bool clamp) const;
	CL_TextureProvider *alloc_texture(CL_TextureDimensions texture_dimensions);
	CL_OcclusionQueryProvider *alloc_occlusion_query();
	CL_ProgramObjectProvider *alloc_program_object();
	CL_ShaderObjectProvider *alloc_shader_object();
	CL_FrameBufferProvider *alloc_frame_buffer();
	CL_RenderBufferProvider *alloc_render_buffer();
	CL_VertexArrayBufferProvider *alloc_vertex_array_buffer();
	CL_ElementArrayBufferProvider *alloc_element_array_buffer();
	CL_PixelBufferProvider *alloc_pixel_buffer();
	void set_program_object(CL_StandardProgram standard_program);
	void set_program_object(const CL_ProgramObject &program, int program_matrix_flags);
	void reset_program_object();
	void set_texture(int unit_index, const CL_Texture &texture);
	void reset_texture(int unit_index, const CL_Texture &texture);
	void set_frame_buffer(const CL_FrameBuffer &write_buffer, const CL_FrameBuffer &read_buffer);
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
	void draw_primitives_elements_instanced(CL_PrimitivesType type, int count, unsigned int *indices, int instance_count);
	void draw_primitives_elements_instanced(CL_PrimitivesType type, int count, unsigned short *indices, int instance_count);
	void draw_primitives_elements_instanced(CL_PrimitivesType type, int count, unsigned char *indices, int instance_count);
	void draw_primitives_elements(CL_PrimitivesType type, int count, CL_ElementArrayBufferProvider *array_provider, CL_VertexAttributeDataType indices_type, void *offset);
	void draw_primitives_elements_instanced(CL_PrimitivesType type, int count, CL_ElementArrayBufferProvider *array_provider, CL_VertexAttributeDataType indices_type, void *offset, int instance_count);
	void primitives_array_freed(const CL_PrimitivesArrayData * const prim_array);
	void reset_primitives_array();
	void draw_pixels(CL_GraphicContext &gc, float x, float y, float zoom_x, float zoom_y, const CL_PixelBuffer &pixel_buffer, const CL_Rect &src_rect, const CL_Colorf &color);
	void draw_pixels_bicubic(float x, float y, int zoom_number, int zoom_denominator, const CL_PixelBuffer &pixels);
	void queue_command(CL_UniquePtr<CL_PixelCommand> &command);
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
/// \}

/// \name Implementation
/// \{
private:
	void draw_triangle(int index1, int index2, int index3);
	void draw_sprite(int index1, int index2, int index3);
	void draw_line(int index1, int index2);


	CL_SWRenderDisplayWindowProvider *window;
	CL_UniquePtr<CL_PixelCanvas> canvas;
	std::map<int, CL_Texture> bound_textures;
	const CL_PrimitivesArrayData * current_prim_array;
	CL_Mat4f modelview_matrix;
	CL_SWRenderProgramObjectProvider *current_program_provider;
	bool is_sprite_program;
	static const int num_attribute_fetchers = 32;
	VertexAttributeFetcherPtr attribute_fetchers[num_attribute_fetchers];
	CL_SoftwareProgram_Standard cl_software_program_standard;
	CL_ProgramObject_SWRender program_object_standard;
/// \}
};

}
