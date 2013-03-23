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

#include "vertex_attribute_fetcher.h"
#include "software_program_standard.h"
#include <map>

namespace clan
{

class PixelCanvas;
class PixelCommand;
class SWRenderDisplayWindowProvider;
class SWRenderProgramObjectProvider;

class SWRenderGraphicContextProvider : public GraphicContextProvider
{
/// \name Construction
/// \{
public:
	SWRenderGraphicContextProvider(SWRenderDisplayWindowProvider *window);
	~SWRenderGraphicContextProvider();
/// \}

/// \name Attributes
/// \{
public:
	int get_max_attributes();
	Size get_max_texture_size() const;
	int get_width() const;
	int get_height() const;
#ifdef WIN32
	HDC get_drawable() const;
#endif
	PixelCanvas *get_canvas() const { return canvas.get(); }
/// \}

/// \name Operations
/// \{
public:
	void destroy();
	PixelBuffer get_pixeldata(const Rect& rect, TextureFormat pixel_format, bool clamp) const;
	TextureProvider *alloc_texture(TextureDimensions texture_dimensions);
	OcclusionQueryProvider *alloc_occlusion_query();
	ProgramObjectProvider *alloc_program_object();
	ShaderObjectProvider *alloc_shader_object();
	FrameBufferProvider *alloc_frame_buffer();
	RenderBufferProvider *alloc_render_buffer();
	VertexArrayBufferProvider *alloc_vertex_array_buffer();
	ElementArrayBufferProvider *alloc_element_array_buffer();
	PixelBufferProvider *alloc_pixel_buffer();
	void set_program_object(StandardProgram standard_program);
	void set_program_object(const ProgramObject &program, int program_matrix_flags);
	void reset_program_object();
	void set_texture(int unit_index, const Texture &texture);
	void reset_texture(int unit_index, const Texture &texture);
	void set_frame_buffer(const FrameBuffer &write_buffer, const FrameBuffer &read_buffer);
	void reset_frame_buffer();
	void set_blend_mode(const BlendMode &blendmode);
	void set_buffer_control(const BufferControl &buffer_control);
	void set_pen(const Pen &pen);
	void set_polygon_rasterizer(const PolygonRasterizer &raster);
	void draw_primitives(PrimitivesType type, int num_vertices, const PrimitivesArrayData * const prim_array);
	void set_primitives_array(const PrimitivesArrayData * const prim_array);
	void draw_primitives_array(PrimitivesType type, int offset, int num_vertices);
	void draw_primitives_array_instanced(PrimitivesType type, int offset, int num_vertices, int instance_count);
	void draw_primitives_elements(PrimitivesType type, int count, unsigned int *indices);
	void draw_primitives_elements(PrimitivesType type, int count, unsigned short *indices);
	void draw_primitives_elements(PrimitivesType type, int count, unsigned char *indices);
	void draw_primitives_elements_instanced(PrimitivesType type, int count, unsigned int *indices, int instance_count);
	void draw_primitives_elements_instanced(PrimitivesType type, int count, unsigned short *indices, int instance_count);
	void draw_primitives_elements_instanced(PrimitivesType type, int count, unsigned char *indices, int instance_count);
	void draw_primitives_elements(PrimitivesType type, int count, ElementArrayBufferProvider *array_provider, VertexAttributeDataType indices_type, void *offset);
	void draw_primitives_elements_instanced(PrimitivesType type, int count, ElementArrayBufferProvider *array_provider, VertexAttributeDataType indices_type, void *offset, int instance_count);
	void primitives_array_freed(const PrimitivesArrayData * const prim_array);
	void reset_primitives_array();
	void draw_pixels(GraphicContext &gc, float x, float y, float zoom_x, float zoom_y, const PixelBuffer &pixel_buffer, const Rect &src_rect, const Colorf &color);
	void draw_pixels_bicubic(float x, float y, int zoom_number, int zoom_denominator, const PixelBuffer &pixels);
	void queue_command(std::unique_ptr<PixelCommand> &command);
	void set_clip_rect(const Rect &rect);
	void reset_clip_rect();
	void clear(const Colorf &color);
	void clear_depth(float value);
	void clear_stencil(int value);
	void set_map_mode(MapMode mode);
	void set_viewport(const Rectf &viewport);
	void set_projection(const Mat4f &matrix);
	void set_modelview(const Mat4f &matrix);
	void on_window_resized();
/// \}

/// \name Implementation
/// \{
private:
	void draw_triangle(int index1, int index2, int index3);
	void draw_sprite(int index1, int index2, int index3);
	void draw_line(int index1, int index2);


	SWRenderDisplayWindowProvider *window;
	std::unique_ptr<PixelCanvas> canvas;
	std::map<int, Texture> bound_textures;
	const PrimitivesArrayData * current_prim_array;
	Mat4f modelview_matrix;
	SWRenderProgramObjectProvider *current_program_provider;
	bool is_sprite_program;
	static const int num_attribute_fetchers = 32;
	VertexAttributeFetcherPtr attribute_fetchers[num_attribute_fetchers];
	SoftwareProgram_Standard cl_software_program_standard;
	ProgramObject_SWRender program_object_standard;
/// \}
};

}
