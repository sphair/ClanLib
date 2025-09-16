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
**    Harry Storbacka
**    Kenneth Gangstoe
**    Kevin J Bluck
*/

#pragma once

#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/GL/opengl.h"
#include "API/Core/Math/mat4.h"
#include "API/Core/Signals/signal.h"
#include "API/Display/Render/program_object.h"
#include "API/Display/Render/rasterizer_state_description.h"
#include "API/Display/Render/blend_state_description.h"
#include "API/Display/Render/depth_stencil_state_description.h"
#include "API/Core/System/disposable_object.h"
#include "gl3_standard_programs.h"
#include "GL/opengl_graphic_context_provider.h"
#include "../State/opengl_blend_state.h"
#include "../State/opengl_rasterizer_state.h"
#include "../State/opengl_depth_stencil_state.h"
#include <map>

namespace clan
{
	class DisplayWindowProvider;
	class ShaderObjectProvider;
	class FrameBufferProvider;
	class RenderBufferProvider;
	class GL3FrameBufferProvider;
	class DisposableObject;
	class OpenGLContextDescription;
	class OpenGLWindowProvider;

	class GL3GraphicContextProvider : public OpenGLGraphicContextProvider, public GraphicContextProvider, public DisposableObject
	{
	public:
		/// \brief Creates a new OpenGL graphic context provider for a rendering window.
		GL3GraphicContextProvider(const OpenGLWindowProvider * const render_window);
		~GL3GraphicContextProvider() override;

		int get_max_attributes() override;
		Size get_max_texture_size() const override;

		/// \brief Get the opengl version major number
		int get_opengl_version_major() const { return opengl_version_major; }

		/// \brief Get the opengl version minor number
		int get_opengl_version_minor() const { return opengl_version_minor; }

		/// \brief Get the glsl version major number
		int get_glsl_version_major() const { return shader_version_major; }

		/// \brief Get the glsl version minor number
		int get_glsl_version_minor() const { return shader_version_minor; }

		const DisplayWindowProvider & get_render_window() const;
		Size get_display_window_size() const override;
		float get_pixel_ratio() const override;
		void get_opengl_version(int &version_major, int &version_minor) const override;
		void get_opengl_version(int &version_major, int &version_minor, int &version_release) const override { get_opengl_version(version_major, version_minor); version_release = 0; }
		void get_opengl_shading_language_version(int &version_major, int &version_minor) const override { version_major = shader_version_major; version_minor = shader_version_minor; }

		Signal<void(const Size &)> &sig_window_resized() override { return window_resized_signal; }

		ProgramObject get_program_object(StandardProgram standard_program) const override;

		ClipZRange get_clip_z_range() const override { return ClipZRange::negative_positive_w; }
		TextureImageYAxis get_texture_image_y_axis() const override { return TextureImageYAxis::y_bottom_up; }
		ShaderLanguage get_shader_language() const override { return ShaderLanguage::glsl; }
		int get_major_version() const override { int major = 0, minor = 0; get_opengl_version(major, minor); return major; }
		int get_minor_version() const override { int major = 0, minor = 0; get_opengl_version(major, minor); return minor; }
		bool has_compute_shader_support() const override { return false; }
		std::unique_ptr<TextureProvider> alloc_texture(TextureDimensions texture_dimensions) override;
		std::unique_ptr<OcclusionQueryProvider> alloc_occlusion_query() override;
		std::unique_ptr<ProgramObjectProvider> alloc_program_object() override;
		std::unique_ptr<ShaderObjectProvider> alloc_shader_object() override;
		std::unique_ptr<FrameBufferProvider> alloc_frame_buffer() override;
		std::unique_ptr<RenderBufferProvider> alloc_render_buffer() override;
		std::unique_ptr<PixelBufferProvider> alloc_pixel_buffer() override;
		std::unique_ptr<VertexArrayBufferProvider> alloc_vertex_array_buffer() override;
		std::unique_ptr<UniformBufferProvider> alloc_uniform_buffer() override;
		std::unique_ptr<StorageBufferProvider> alloc_storage_buffer() override;
		std::unique_ptr<ElementArrayBufferProvider> alloc_element_array_buffer() override;
		std::unique_ptr<TransferBufferProvider> alloc_transfer_buffer() override;
		std::unique_ptr<PrimitivesArrayProvider> alloc_primitives_array() override;
		std::unique_ptr<RasterizerStateProvider> alloc_rasterizer_state(const RasterizerStateDescription &desc) override;
		std::unique_ptr<BlendStateProvider> alloc_blend_state(const BlendStateDescription &desc) override;
		std::unique_ptr<DepthStencilStateProvider> alloc_depth_stencil_state(const DepthStencilStateDescription &desc) override;
		void set_rasterizer_state(RasterizerStateProvider *state) override;
		void set_blend_state(BlendStateProvider *state, const Colorf &blend_color, unsigned int sample_mask) override;
		void set_depth_stencil_state(DepthStencilStateProvider *state, int stencil_ref) override;
		PixelBuffer get_pixeldata(const Rect& rect, TextureFormat texture_format, bool clamp) const override;
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
		void set_program_object(StandardProgram standard_program) override;
		void set_program_object(const ProgramObject &program) override;
		void reset_program_object() override;

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

		void add_disposable(DisposableObject *disposable);
		void remove_disposable(DisposableObject *disposable);

		ProcAddress *get_proc_address(const std::string& function_name) const override;

		void make_current() const override;

		void flush() override;

	private:
		void on_dispose() override;
		void create_standard_programs();

		void check_opengl_version();
		void calculate_shading_language_version();

		/// \brief OpenGL render window.
		const OpenGLWindowProvider * const render_window;

		bool framebuffer_bound;

		mutable int opengl_version_major;
		mutable int opengl_version_minor;

		int shader_version_major;
		int shader_version_minor;

		bool scissor_enabled;

		std::vector<DisposableObject *> disposable_objects;

		Signal<void(const Size &)> window_resized_signal;

		OpenGLBlendState selected_blend_state;
		OpenGLRasterizerState selected_rasterizer_state;
		OpenGLDepthStencilState selected_depth_stencil_state;

		GL3StandardPrograms standard_programs;
	};
}
