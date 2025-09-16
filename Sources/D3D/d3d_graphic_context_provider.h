/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
#include "API/Display/Render/blend_state_description.h"
#include "API/Display/Render/depth_stencil_state_description.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Render/program_object.h"
#include "d3d_rasterizer_state_provider.h"
#include "d3d_blend_state_provider.h"
#include "d3d_depth_stencil_state_provider.h"
#include "standard_programs.h"
#include "d3d_unit_map.h"
#include <map>
#include <memory>
#include "d3d_render_buffer_provider.h"

namespace clan
{
	class D3DDisplayWindowProvider;
	class D3DProgramObjectProvider;
	class D3DPrimitivesArrayProvider;
	class D3DElementArrayBufferProvider;
	class DisplayWindowDescription;

	class D3DGraphicContextProvider : public GraphicContextProvider
	{
	public:
		D3DGraphicContextProvider(D3DDisplayWindowProvider *window, const DisplayWindowDescription &display_desc);
		~D3DGraphicContextProvider() override;

		int get_max_attributes() override;
		Size get_max_texture_size() const override;
		Size get_display_window_size() const override;
		float get_pixel_ratio() const override;
		D3DDisplayWindowProvider *get_window() const { return window; }

#ifdef WIN32
		HDC get_drawable() const;
#endif

		Signal<void(const Size &)> &sig_window_resized() override { return window_resized_signal; }
		ProgramObject get_program_object(StandardProgram standard_program) const override;

		StandardPrograms standard_programs;

		ClipZRange get_clip_z_range() const override { return ClipZRange::zero_positive_w; }
		TextureImageYAxis get_texture_image_y_axis() const override { return TextureImageYAxis::y_top_down; }
		ShaderLanguage get_shader_language() const override { return ShaderLanguage::hlsl; }
		int get_major_version() const override;
		int get_minor_version() const override;
		bool has_compute_shader_support() const override;
		PixelBuffer get_pixeldata(const Rect& rect, TextureFormat texture_format, bool clamp) const override;
		std::unique_ptr<TextureProvider> alloc_texture(TextureDimensions texture_dimensions) override;
		std::unique_ptr<OcclusionQueryProvider> alloc_occlusion_query() override;
		std::unique_ptr<ProgramObjectProvider> alloc_program_object() override;
		std::unique_ptr<ShaderObjectProvider> alloc_shader_object() override;
		std::unique_ptr<FrameBufferProvider> alloc_frame_buffer() override;
		std::unique_ptr<RenderBufferProvider> alloc_render_buffer() override;
		std::unique_ptr<VertexArrayBufferProvider> alloc_vertex_array_buffer() override;
		std::unique_ptr<UniformBufferProvider> alloc_uniform_buffer() override;
		std::unique_ptr<StorageBufferProvider> alloc_storage_buffer() override;
		std::unique_ptr<ElementArrayBufferProvider> alloc_element_array_buffer() override;
		std::unique_ptr<TransferBufferProvider> alloc_transfer_buffer() override;
		std::unique_ptr<PixelBufferProvider> alloc_pixel_buffer() override;
		std::unique_ptr<PrimitivesArrayProvider> alloc_primitives_array() override;
		std::unique_ptr<RasterizerStateProvider> alloc_rasterizer_state(const RasterizerStateDescription &desc) override;
		std::unique_ptr<BlendStateProvider> alloc_blend_state(const BlendStateDescription &desc) override;
		std::unique_ptr<DepthStencilStateProvider> alloc_depth_stencil_state(const DepthStencilStateDescription &desc) override;
		void set_rasterizer_state(RasterizerStateProvider *state) override;
		void set_blend_state(BlendStateProvider *state, const Colorf &blend_color, unsigned int sample_mask) override;
		void set_depth_stencil_state(DepthStencilStateProvider *state, int stencil_ref) override;
		void set_program_object(StandardProgram standard_program) override;
		void set_program_object(const ProgramObject &program) override;
		void reset_program_object() override;
		void set_uniform_buffer(int index, const UniformBuffer &buffer) override;
		void reset_uniform_buffer(int index) override;
		void set_storage_buffer(int index, const StorageBuffer &buffer) override;
		void reset_storage_buffer(int index) override;
		void set_texture(int unit_index, const Texture &texture) override;
		void reset_texture(int unit_index) override;
		void set_image_texture(int unit_index, const Texture &texture) override;
		void reset_image_texture(int unit_index) override;
		bool is_frame_buffer_owner(const FrameBuffer &fb) override;
		void set_frame_buffer(const FrameBuffer &write_buffer, const FrameBuffer &read_buffer) override;
		void reset_frame_buffer() override;
		void set_draw_buffer(DrawBuffer buffer) override;

		bool is_primitives_array_owner(const PrimitivesArray &primitives_array) override;
		void draw_primitives(PrimitivesType type, int num_vertices, const PrimitivesArray &primitives_array) override;
		void set_primitives_array(const PrimitivesArray &primitives_array) override;
		void draw_primitives_array(PrimitivesType type, int offset, int num_vertices) override;
		void draw_primitives_array_instanced(PrimitivesType type, int offset, int num_vertices, int instance_count) override;
		void set_primitives_elements(ElementArrayBufferProvider *array_provider) override;
		void draw_primitives_elements(PrimitivesType type, int count, VertexAttributeDataType indices_type, size_t offset = 0) override;
		void draw_primitives_elements_instanced(PrimitivesType type, int count, VertexAttributeDataType indices_type, size_t offset, int instance_count) override;
		void reset_primitives_elements() override;
		void draw_primitives_elements(PrimitivesType type, int count, ElementArrayBufferProvider *array_provider, VertexAttributeDataType indices_type, void *offset) override;
		void draw_primitives_elements_instanced(PrimitivesType type, int count, ElementArrayBufferProvider *array_provider, VertexAttributeDataType indices_type, void *offset, int instance_count) override;
		void reset_primitives_array() override;
		void set_scissor(const Rect &rect) override;
		void reset_scissor() override;
		void dispatch(int x, int y, int z) override;
		void clear(const Colorf &color) override;
		void clear_depth(float value) override;
		void clear_stencil(int value) override;
		void set_viewport(const Rectf &viewport) override;
		void set_viewport(int index, const Rectf &viewport) override;

		void set_depth_range(float n, float f) override;
		void set_depth_range(int viewport, float n, float f) override;
		void on_window_resized();

		void begin_resize_swap_chain();
		void end_resize_swap_chain();

		void flush() override;

	private:
		void set_default_dsv();
		static D3D11_PRIMITIVE_TOPOLOGY to_d3d_primitive_topology(PrimitivesType type);
		static DXGI_FORMAT to_d3d_format(VertexAttributeDataType indices_type);
		static UINT to_d3d_index_location(VertexAttributeDataType indices_type, size_t offset);
		void apply_input_layout();
		void clear_input_layout();

		D3DDisplayWindowProvider *window;
		D3DPrimitivesArrayProvider *current_prim_array_provider;
		D3DProgramObjectProvider *current_program_provider;
		D3DElementArrayBufferProvider *current_element_array_provider;
		D3D11_VIEWPORT viewports[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
		D3D11_RECT scissor_rects[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
		bool input_layout_set;
		D3DUnitMap unit_map;
		bool shader_bound[static_cast<int>(ShaderType::num_types)];

		Signal<void(const Size &)> window_resized_signal;

		std::shared_ptr<D3DRenderBufferProvider> default_depth_render_buffer;
		ComPtr<ID3D11DepthStencilView> default_dsv;
		int default_depth;
	};
}
