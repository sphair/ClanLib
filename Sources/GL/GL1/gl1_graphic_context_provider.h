/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
**    Harry Storbacka
**    Kenneth Gangstoe
**    Kevin J Bluck
**    Mark Page
*/

#pragma once

#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/Display/TargetProviders/primitives_array_provider.h"
#include "API/Display/Font/font.h"
#include "API/Display/Font/font_metrics.h"
#include "API/Display/Render/program_object.h"
#include "API/Display/Render/rasterizer_state_description.h"
#include "API/Display/Render/blend_state_description.h"
#include "API/Display/Render/depth_stencil_state_description.h"
#include "API/GL/opengl.h"
#include "API/Core/Math/mat4.h"
#include "API/Core/Signals/signal_v0.h"
#include "GL/opengl_graphic_context_provider.h"
#include "API/Core/System/disposable_object.h"
#include "../State/opengl_blend_state.h"
#include <map>

namespace clan
{

class DisplayWindowProvider;
class ShaderObjectProvider;
class FrameBufferProvider;
class RenderBufferProvider;
class GL1TextureProvider;
class GL1SelectedTexture;
class DisposableObject;
class GL1ProgramObjectProvider;
class OpenGLWindowProvider;

class GL1RasterizerStateProvider : public RasterizerStateProvider
{
public:
	GL1RasterizerStateProvider(const RasterizerStateDescription &desc) : desc(desc.clone()) { }
	RasterizerStateDescription desc;
};

class GL1DepthStencilStateProvider : public DepthStencilStateProvider
{
public:
	GL1DepthStencilStateProvider(const DepthStencilStateDescription &desc) : desc(desc.clone()) { }
	DepthStencilStateDescription desc;
};

class GL1GraphicContextProvider : public OpenGLGraphicContextProvider,  public GraphicContextProvider, public DisposableObject
{
/// \name Construction
/// \{
public:
	/// \brief Creates a new OpenGL graphic context provider for a rendering window.
	GL1GraphicContextProvider(const OpenGLWindowProvider * const render_window);
	~GL1GraphicContextProvider();

/// \}
/// \name Attributes
/// \{
public:
	int get_max_attributes();
	Size get_max_texture_size() const;
	const DisplayWindowProvider & get_render_window() const;
	Size get_display_window_size() const;

	// GL1 Only
	int get_max_texture_coords();

	void get_opengl_version(int &version_major, int &version_minor) { int version_release = 0; get_opengl_version(version_major, version_minor, version_release); }
	void get_opengl_version(int &version_major, int &version_minor, int &version_release);

	Signal_v1<const Size &> &sig_window_resized() { return window_resized_signal; }

	ProgramObject get_program_object(StandardProgram standard_program) const;

/// \}
/// \name Operations
/// \{
public:

	ClipZRange get_clip_z_range() const { return clip_negative_positive_w; }
	TextureImageYAxis get_texture_image_y_axis() const { return y_axis_bottom_up; }
	ShaderLanguage get_shader_language() const { return shader_fixed_function; }
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
	PixelBuffer get_pixeldata(const Rect& rect, TextureFormat texture_format, bool clamp) const;
	void set_uniform_buffer(int index, const UniformBuffer &buffer);
	void reset_uniform_buffer(int index);
	void set_storage_buffer(int index, const StorageBuffer &buffer);
	void reset_storage_buffer(int index);
	void set_texture(int unit_index, const Texture &texture);
	void reset_texture(int unit_index);
	void set_image_texture(int unit_index, const Texture &texture);
	void reset_image_texture(int unit_index);
	bool is_frame_buffer_owner(const FrameBuffer &fb);
	void set_frame_buffer(const FrameBuffer &w_buffer, const FrameBuffer &r_buffer);
	void reset_frame_buffer();
	void set_program_object(StandardProgram standard_program);
	void set_program_object(const ProgramObject &program);
	void reset_program_object();
	void enable_logic_op(bool enabled);
	void set_logic_op(LogicOp op);
	void set_draw_buffer(DrawBuffer buffer);

	void set_antialiased(bool value);
	void set_offset_point(bool value);
	void set_offset_line(bool value);
	void set_offset_fill(bool value);
	void set_polygon_offset(float factor, float units);
	
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
	void set_point_size(float value);
	void set_point_fade_treshold_size(float value);
	void set_line_width(float value);
	void enable_vertex_program_point_size(bool enabled);
	void set_point_sprite_origin(PointSpriteOrigin origin);
	void on_window_resized();

	ProcAddress *get_proc_address(const std::string& function_name) const;

	// GL1 Only
	void set_active() const;

	void add_disposable(DisposableObject *disposable);
	void remove_disposable(DisposableObject *disposable);
	void make_current() const;

/// \}
/// \name Implementation
/// \{
private:
	void on_dispose();
	void check_opengl_version();
	void set_primitive_texture( int texture_index, PrimitivesArrayProvider::VertexData &array_texture, int offset, int num_vertices, int total_vertices);
	void reset_primitive_texture( int texture_index);
	void reset_primitive_texture_all();

	/// \brief OpenGL render window.
	const OpenGLWindowProvider * const render_window;

	bool prim_arrays_set;

	int num_set_tex_arrays;

	int max_texture_coords;

	std::vector<GL1SelectedTexture> selected_textures;

	bool primitives_array_texture_set;
	PrimitivesArrayProvider::VertexData primitives_array_texture;

	bool primitives_array_texindex_set;
	PrimitivesArrayProvider::VertexData primitives_array_texindex;

	std::vector<float> transformed_coords;
	std::vector<DisposableObject *> disposable_objects;

	Signal_v1<const Size &> window_resized_signal;

	ProgramObject internal_program;
	GL1ProgramObjectProvider *internal_program_provider;		// Pointer is owned by "internal_program"

	bool scissor_enabled;

	std::map<RasterizerStateDescription, std::shared_ptr<RasterizerStateProvider> > rasterizer_states;
	std::map<BlendStateDescription, std::shared_ptr<BlendStateProvider> > blend_states;
	std::map<DepthStencilStateDescription, std::shared_ptr<DepthStencilStateProvider> > depth_stencil_states;

	OpenGLBlendStateProvider selected_blend_state;

/// \}
};

}
