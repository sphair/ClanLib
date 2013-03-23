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
	Size get_display_window_size() const;
	PixelCanvas *get_canvas() const { return canvas.get(); }

#ifdef WIN32
	HDC get_drawable() const;
#endif

	Signal_v1<const Size &> &sig_window_resized() { return window_resized_signal; }
	ProgramObject get_program_object(StandardProgram standard_program) const;

/// \}

/// \name Operations
/// \{
public:
	void draw_pixels_bicubic(float x, float y, int zoom_number, int zoom_denominator, const PixelBuffer &pixels);
	void queue_command(std::unique_ptr<PixelCommand> &command);

	ClipZRange get_clip_z_range() const { return clip_zero_positive_w; }
	TextureImageYAxis get_texture_image_y_axis() const { return y_axis_top_down; }
	ShaderLanguage get_shader_language() const { return shader_hlsl; }
	PixelBuffer get_pixeldata(const Rect& rect, TextureFormat texture_format, bool clamp) const;
	TextureProvider *alloc_texture(TextureDimensions texture_dimensions);
	OcclusionQueryProvider *alloc_occlusion_query();
	ProgramObjectProvider *alloc_program_object();
	ShaderObjectProvider *alloc_shader_object();
	FrameBufferProvider *alloc_frame_buffer();
	RenderBufferProvider *alloc_render_buffer();
	VertexArrayBufferProvider *alloc_vertex_array_buffer();
	UniformBufferProvider *alloc_uniform_buffer();
	StorageBufferProvider *alloc_storage_buffer();
	ElementArrayBufferProvider *alloc_element_array_buffer();
	TransferBufferProvider *alloc_transfer_buffer();
	PixelBufferProvider *alloc_pixel_buffer();
	PrimitivesArrayProvider *alloc_primitives_array();
	std::shared_ptr<RasterizerStateProvider> create_rasterizer_state(const RasterizerStateDescription &desc);
	std::shared_ptr<BlendStateProvider> create_blend_state(const BlendStateDescription &desc);
	std::shared_ptr<DepthStencilStateProvider> create_depth_stencil_state(const DepthStencilStateDescription &desc);
	void set_rasterizer_state(RasterizerStateProvider *state);
	void set_blend_state(BlendStateProvider *state, const Vec4f &blend_color, unsigned int sample_mask);
	void set_depth_stencil_state(DepthStencilStateProvider *state, int stencil_ref);
	void set_program_object(StandardProgram standard_program);
	void set_program_object(const ProgramObject &program);
	void reset_program_object();
	void set_uniform_buffer(int index, const UniformBuffer &buffer);
	void reset_uniform_buffer(int index);
	void set_storage_buffer(int index, const StorageBuffer &buffer);
	void reset_storage_buffer(int index);
	void set_texture(int unit_index, const Texture &texture);
	void reset_texture(int unit_index);
	void set_image_texture(int unit_index, const Texture &texture);
	void reset_image_texture(int unit_index);
	bool is_frame_buffer_owner(const FrameBuffer &fb);
	void set_frame_buffer(const FrameBuffer &write_buffer, const FrameBuffer &read_buffer);
	void reset_frame_buffer();
	void enable_logic_op(bool enabled);
	void set_logic_op(LogicOp op);
	void set_draw_buffer(DrawBuffer buffer);

	void set_point_size(float value);
	void set_point_fade_treshold_size(float value);
	void set_line_width(float value);
	void enable_vertex_program_point_size(bool enabled);
	void set_point_sprite_origin(PointSpriteOrigin origin);

	void set_antialiased(bool value);
	void set_point_offset(bool value);
	void set_line_offset(bool value);
	void set_polygon_offset(bool value);
	void set_offset_factor(float value);
	void set_offset_units(float value);

	bool is_primitives_array_owner(const PrimitivesArray &primitives_array);
	void draw_primitives(PrimitivesType type, int num_vertices, const PrimitivesArray &primitives_array);
	void set_primitives_array(const PrimitivesArray &primitives_array);
	void draw_primitives_array(PrimitivesType type, int offset, int num_vertices);
	void draw_primitives_array_instanced(PrimitivesType type, int offset, int num_vertices, int instance_count);
	void set_primitives_elements(ElementArrayBufferProvider *array_provider);
	void draw_primitives_elements(PrimitivesType type, int count, VertexAttributeDataType indices_type, size_t offset = 0);
	void draw_primitives_elements_instanced(PrimitivesType type, int count, VertexAttributeDataType indices_type, size_t offset, int instance_count);
	void reset_primitives_elements();
	void draw_primitives_elements(PrimitivesType type, int count, ElementArrayBufferProvider *array_provider, VertexAttributeDataType indices_type, void *offset);
	void draw_primitives_elements_instanced(PrimitivesType type, int count, ElementArrayBufferProvider *array_provider, VertexAttributeDataType indices_type, void *offset, int instance_count);
	void reset_primitives_array();
	void set_scissor(const Rect &rect);
	void reset_scissor();
	void dispatch(int x, int y, int z);
	void clear(const Colorf &color);
	void clear_depth(float value);
	void clear_stencil(int value);
	void set_viewport(const Rectf &viewport);
	void set_viewport(int index, const Rectf &viewport);

	void set_depth_range(float n, float f);
	void set_depth_range(int viewport, float n, float f);
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
	SWRenderProgramObjectProvider *current_program_provider;
	bool is_sprite_program;
	static const int num_attribute_fetchers = 32;
	VertexAttributeFetcherPtr attribute_fetchers[num_attribute_fetchers];
	SoftwareProgram_Standard cl_software_program_standard;
	ProgramObject_SWRender program_object_standard;
	Signal_v1<const Size &> window_resized_signal;

/// \}
};

}
