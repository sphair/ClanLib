/*
**  ClanLib SDK
**  Copyright (c) 1997-2026 The ClanLib Team
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
**	claim that you wrote the original software. If you use this software
**	in a product, an acknowledgment in the product documentation would be
**	appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**	misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**	Magnus Norddahl
**	Mark Page
*/

#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include <array>
#include <cstring>

#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/Core/Math/mat4.h"
#include "API/Core/Signals/signal.h"
#include "API/Display/Render/program_object.h"
#include "API/Core/System/disposable_object.h"
#include "VK/State/vulkan_blend_state.h"
#include "VK/State/vulkan_rasterizer_state.h"
#include "VK/State/vulkan_depth_stencil_state.h"
#include "API/Display/Image/texture_format.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Render/primitives_array.h"
#include "VK/vulkan_window_provider_base.h"

namespace clan
{

	// Forward declarations for Vulkan resource provider types
	class VulkanPrimitivesArrayProvider;
	class VulkanFrameBufferProvider;
	class VulkanTextureProvider;
	class VulkanUniformBufferProvider;
	class VulkanStorageBufferProvider;
	class VulkanProgramObjectProvider;
	class VulkanElementArrayBufferProvider;

	// =========================================================================
	//  VulkanPipelineKey
	// =========================================================================
	struct VulkanPipelineKey
	{
		// ---- Shader program --------------------------------------------------
		VulkanProgramObjectProvider		*program	;

		// ---- Vertex layout (content hash, not pointer) -----------------------
		// Hashing the actual VkVertexInput*Description content means two
		// PrimitivesArray objects with identical layouts share a cached pipeline.
		// A raw provider pointer would cause a cache miss every frame when the
		// caller creates a new PrimitivesArray per draw (common immediate-mode
		// usage), calling vkCreateGraphicsPipelines on every single draw call.
		uint64_t layout_hash			;

		// ---- Blend  (VulkanBlendKey::fill_from populates these) --------------
		VkBool32	blend_enable		;
		VkBlendFactor src_color			;
		VkBlendFactor dst_color			;
		VkBlendOp	color_op			;
		VkBlendFactor src_alpha			;
		VkBlendFactor dst_alpha			;
		VkBlendOp	alpha_op			;
		VkColorComponentFlags color_write  ;
		VkBool32	logic_op_enable	;
		VkLogicOp	logic_op			;

		// ---- Rasterizer  (VulkanRasterizerKey::fill_from populates these) ----
		VkCullModeFlags cull_mode		;
		VkFrontFace	front_face		;
		VkPolygonMode   polygon_mode	;
		VkBool32		depth_clamp		;
		VkBool32		scissor_test	;
		VkBool32		depth_bias_enable  ;
		float		depth_bias_constant;
		float		depth_bias_slope   ;
		float		line_width		;

		// ---- Depth/stencil  (VulkanDepthStencilKey::fill_from) ---------------
		VkBool32	depth_test		;
		VkBool32	depth_write		;
		VkCompareOp   depth_func		;
		VkBool32	stencil_test		;
		VkCompareOp   front_func		;
		uint32_t	front_compare_mask   ;
		uint32_t	front_write_mask	;
		VkStencilOp   front_fail		;
		VkStencilOp   front_depth_fail	;
		VkStencilOp   front_pass		;
		VkCompareOp   back_func			;
		uint32_t	back_compare_mask	;
		uint32_t	back_write_mask	;
		VkStencilOp   back_fail			;
		VkStencilOp   back_depth_fail	;
		VkStencilOp   back_pass			;

		// ---- Primitive topology ----------------------------------------------
		VkPrimitiveTopology topology	;

		// ---- Target render pass ----------------------------------------------
		VkRenderPass render_pass		;

		// ---- Constructor: zero all bytes first, then apply non-zero defaults.
		//	This guarantees every padding byte is 0 so memcmp is reliable.
		VulkanPipelineKey() noexcept
		{
			std::memset(this, 0, sizeof(*this));

			// Non-zero defaults that must be restored after the memset:
			src_color		= VK_BLEND_FACTOR_ONE;
			dst_alpha		= VK_BLEND_FACTOR_ZERO;  // already 0 but explicit
			src_alpha		= VK_BLEND_FACTOR_ONE;
			color_write		= VK_COLOR_COMPONENT_R_BIT |
								VK_COLOR_COMPONENT_G_BIT |
								VK_COLOR_COMPONENT_B_BIT |
								VK_COLOR_COMPONENT_A_BIT;
			logic_op		= VK_LOGIC_OP_COPY;
			front_face		= VK_FRONT_FACE_COUNTER_CLOCKWISE;
			polygon_mode	= VK_POLYGON_MODE_FILL;
			line_width		= 1.0f;
			depth_write		= VK_TRUE;
			depth_func		= VK_COMPARE_OP_LESS;
			front_func		= VK_COMPARE_OP_ALWAYS;
			front_compare_mask = 0xFFu;
			front_write_mask   = 0xFFu;
			back_func		= VK_COMPARE_OP_ALWAYS;
			back_compare_mask  = 0xFFu;
			back_write_mask	= 0xFFu;
			topology		= VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			// render_pass, program remain VK_NULL_HANDLE / nullptr (== 0);
			// layout_hash is already 0 from memset.
		}

		bool operator==(const VulkanPipelineKey &o) const noexcept
		{
			return std::memcmp(this, &o, sizeof(*this)) == 0;
		}
	};

	struct VulkanPipelineKeyHash
	{
		size_t operator()(const VulkanPipelineKey &k) const noexcept
		{
			const uint8_t *p = reinterpret_cast<const uint8_t *>(&k);
			size_t h = 14695981039346656037ULL;
			for (size_t i = 0; i < sizeof(k); i++)
				h = (h ^ p[i]) * 1099511628211ULL;
			return h;
		}
	};

	// =========================================================================
	//  VulkanGraphicContextProvider
	//  Direct structural mirror of GL3GraphicContextProvider with Vulkan calls.
	// =========================================================================
	class VulkanGraphicContextProvider final
		: public GraphicContextProvider
	{
	public:
		explicit VulkanGraphicContextProvider(VulkanWindowProviderBase *render_window);
		~VulkanGraphicContextProvider() override;

		// ---- Capabilities ---------------------------------------------------
		int	get_max_attributes()	override;
		Size   get_max_texture_size()	const override;
		Size   get_display_window_size()  const override;
		float  get_pixel_ratio()		const override;

		ClipZRange		get_clip_z_range()		const override { return ClipZRange::zero_positive_w; }
		TextureImageYAxis get_texture_image_y_axis()  const override { return TextureImageYAxis::y_top_down; }
		ShaderLanguage	get_shader_language()	const override { return ShaderLanguage::glsl; }
		int			get_major_version()		const override { return 1; }
		int			get_minor_version()		const override { return 2; }
		bool			has_compute_shader_support() const override { return true; }

		Signal<void(const Size &)> &sig_window_resized() override { return window_resized_signal; }

		ProgramObject get_program_object(StandardProgram standard_program) const override;

		// ---- Resource allocation  -------------------------------------------
		std::unique_ptr<TextureProvider>			alloc_texture(TextureDimensions) override;
		std::unique_ptr<OcclusionQueryProvider>	alloc_occlusion_query() override;
		std::unique_ptr<ProgramObjectProvider>	alloc_program_object() override;
		std::unique_ptr<ShaderObjectProvider>		alloc_shader_object() override;
		std::unique_ptr<FrameBufferProvider>		alloc_frame_buffer() override;
		std::unique_ptr<RenderBufferProvider>		alloc_render_buffer() override;
		std::unique_ptr<PixelBufferProvider>		alloc_pixel_buffer() override;
		std::unique_ptr<VertexArrayBufferProvider>   alloc_vertex_array_buffer() override;
		std::unique_ptr<UniformBufferProvider>	alloc_uniform_buffer() override;
		std::unique_ptr<StorageBufferProvider>	alloc_storage_buffer() override;
		std::unique_ptr<ElementArrayBufferProvider>  alloc_element_array_buffer() override;
		std::unique_ptr<TransferBufferProvider>	alloc_transfer_buffer() override;
		std::unique_ptr<PrimitivesArrayProvider>	alloc_primitives_array() override;

		std::unique_ptr<RasterizerStateProvider>
			alloc_rasterizer_state(const RasterizerStateDescription &desc) override;
		std::unique_ptr<BlendStateProvider>
			alloc_blend_state(const BlendStateDescription &desc) override;
		std::unique_ptr<DepthStencilStateProvider>
			alloc_depth_stencil_state(const DepthStencilStateDescription &desc) override;

		// ---- State setters --------------------------------------------------
		void set_rasterizer_state(RasterizerStateProvider *state) override;
		void set_blend_state(BlendStateProvider *state, const Colorf &blend_color,
							unsigned int sample_mask) override;
		void set_depth_stencil_state(DepthStencilStateProvider *state, int stencil_ref) override;

		// ---- Pixel readback -------------------------------------------------
		PixelBuffer get_pixeldata(const Rect &rect, TextureFormat texture_format,
								bool clamp) const override;

		// ---- Uniform / storage buffers --------------------------------------
		void set_uniform_buffer(int index, const UniformBuffer &buffer) override;
		void reset_uniform_buffer(int index) override;
		void set_storage_buffer(int index, const StorageBuffer &buffer) override;
		void reset_storage_buffer(int index) override;

		// ---- Textures -------------------------------------------------------
		void set_texture(int unit_index, const Texture &texture) override;
		void reset_texture(int unit_index) override;
		void set_image_texture(int unit_index, const Texture &texture) override;
		void reset_image_texture(int unit_index) override;

		// ---- Frame buffer ---------------------------------------------------
		bool is_frame_buffer_owner(const FrameBuffer &fb) override;
		void set_frame_buffer(const FrameBuffer &write_buffer,
							const FrameBuffer &read_buffer) override;
		void reset_frame_buffer() override;

		// ---- Program --------------------------------------------------------
		void set_program_object(StandardProgram standard_program) override;
		void set_program_object(const ProgramObject &program) override;
		void reset_program_object() override;

		// ---- Draw buffer ----------------------------------------------------
		void set_draw_buffer(DrawBuffer buffer) override;

		// ---- Draw calls -----------------------------------------------------
		bool is_primitives_array_owner(const PrimitivesArray &prim_array) override;
		void draw_primitives(PrimitivesType type, int num_vertices,
							const PrimitivesArray &prim_array) override;
		void set_primitives_array(const PrimitivesArray &prim_array) override;
		void draw_primitives_array(PrimitivesType type, int offset, int num_vertices) override;
		void draw_primitives_array_instanced(PrimitivesType type, int offset,
											int num_vertices, int instance_count) override;
		void set_primitives_elements(ElementArrayBufferProvider *array_provider) override;
		void draw_primitives_elements(PrimitivesType type, int count,
									VertexAttributeDataType indices_type,
									size_t offset = 0) override;
		void draw_primitives_elements_instanced(PrimitivesType type, int count,
												VertexAttributeDataType indices_type,
												size_t offset, int instance_count) override;
		void reset_primitives_elements() override;
		void draw_primitives_elements(PrimitivesType type, int count,
									ElementArrayBufferProvider *array_provider,
									VertexAttributeDataType indices_type,
									void *offset) override;
		void draw_primitives_elements_instanced(PrimitivesType type, int count,
												ElementArrayBufferProvider *array_provider,
												VertexAttributeDataType indices_type,
												void *offset, int instance_count) override;
		void reset_primitives_array() override;

		// ---- Viewport / scissor / clear -------------------------------------
		void set_scissor(const Rect &rect) override;
		void reset_scissor() override;
		void set_viewport(const Rectf &viewport) override;
		void set_viewport(int index, const Rectf &viewport) override;
		void set_depth_range(float n, float f) override;
		void set_depth_range(int viewport, float n, float f) override;

		void clear(const Colorf &color) override;
		void clear_depth(float value) override;
		void clear_stencil(int value) override;

		// ---- Compute --------------------------------------------------------
		void dispatch(int x, int y, int z) override;

		// ---- Window / GC lifecycle ------------------------------------------
		void on_window_resized();

		void add_disposable(DisposableObject *disposable);
		void remove_disposable(DisposableObject *disposable);

		void make_current() const { /* no-op */ }
		ProcAddress *get_proc_address(const std::string &function_name) const;

		void flush() override;

		VulkanWindowProviderBase *get_render_window() const { return render_window; }

		/// Called by the window provider's end_frame() immediately before
		/// vkEndCommandBuffer.  A render pass must be explicitly ended before
		/// the command buffer can be submitted; the GC provider opens one lazily
		/// on the first draw call and this is the guaranteed close-out point.
		void end_render_pass_if_active(VkCommandBuffer cmd);

		/// Called by the window provider at the start of each frame (after
		/// vkWaitForFences confirms the GPU is done with this frame slot).
		/// Resets the per-frame descriptor pool so all sets from frame N-2 are
		/// reclaimed, and clears the current_descriptor_set handle.
		void begin_frame_gc(uint32_t frame_index);

	private:
		void on_dispose() override;
		void create_standard_programs();
		void create_descriptor_pools();
		void reset_descriptor_pool(uint32_t frame_index);
		void create_descriptor_set_layout();

		/// Create a persistent 1×1 opaque-white RGBA8 texture used as a dummy
		/// sampler for any sprite texture slot (0..15) that has no real texture
		/// bound.  Writing a valid descriptor for every declared binding guarantees
		/// the descriptor set is fully populated, eliminating reliance on
		/// VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT on strict drivers.
		void create_dummy_texture();

		VkPipeline	get_or_create_pipeline(PrimitivesType type);
		VkPipelineLayout get_or_create_pipeline_layout();
		void			flush_descriptors(VkCommandBuffer cmd, VkPipelineLayout layout);
		void			ensure_render_pass_active(VkCommandBuffer cmd_hint);

		/// Lazily call render_window->begin_frame() the first time a Vulkan
		/// recording command is needed in this frame.  This guarantees the
		/// command buffer is in the recording state before any
		/// vkCmd* call (vkCmdBeginRenderPass, vkCmdClearAttachments, etc.).
		void			ensure_frame_begun();

		void			clear_attachment_immediate(VkCommandBuffer cmd,
													VkClearAttachment attachment,
													VkExtent2D extent);

		static VkPrimitiveTopology to_vk_topology(PrimitivesType type);
		static VkIndexType		to_vk_index_type(VertexAttributeDataType t);

		// ---- Members --------------------------------------------------------
		VulkanWindowProviderBase  *render_window;
		VulkanDevice			*vk_device;

		VulkanFrameBufferProvider *current_framebuffer = nullptr;
		VkRenderPass			current_render_pass = VK_NULL_HANDLE;

		bool scissor_enabled = false;

		Signal<void(const Size &)> window_resized_signal;

		ProgramObject prog_color_only;
		ProgramObject prog_single_texture;
		ProgramObject prog_sprite;
		ProgramObject prog_path;

		VulkanPipelineKey	pipeline_key;
		Colorf				blend_color_value;
		bool				render_pass_active  = false;

		bool				scissor_rect_set	= false;
		Rect				scissor_rect;
		Rectf				current_viewport;
		bool				viewport_dirty	= true;
		float				depth_range_near	= 0.0f;
		float				depth_range_far	= 1.0f;

		VkClearColorValue	pending_clear_color{};
		float				pending_clear_depth   = 1.0f;
		uint32_t			pending_clear_stencil = 0;
		VkImageAspectFlags	pending_clear_mask	= 0;

		VulkanProgramObjectProvider			*current_program	= nullptr;
		const VulkanPrimitivesArrayProvider	*current_prim_array  = nullptr;
		VkBuffer								current_index_buffer = VK_NULL_HANDLE;
		VkDeviceSize							current_index_offset = 0;

		static constexpr int MAX_TEXTURES = 32;
		static constexpr int MAX_UBOS	= 16;
		static constexpr int MAX_SSBOS	= 16;
		std::array<VulkanTextureProvider *,	MAX_TEXTURES> bound_textures{};
		std::array<VulkanUniformBufferProvider *, MAX_UBOS>	bound_ubos{};
		std::array<VulkanStorageBufferProvider *, MAX_SSBOS>	bound_ssbos{};
		bool descriptors_dirty = false;

		// Per-frame descriptor pools: pool[N] is reset at the start of frame N
		// (after vkWaitForFences confirms the GPU has finished using frame N's sets).
		// This avoids freeing descriptor sets while they are still bound in a recording
		// command buffer, which is undefined behaviour and causes device lost.
		static constexpr int DESC_POOL_FRAMES = 2; // must match MAX_FRAMES_IN_FLIGHT
		VkDescriptorPool	descriptor_pools[DESC_POOL_FRAMES] = {};
		VkDescriptorSetLayout descriptor_layout = VK_NULL_HANDLE;
		VkDescriptorSet	current_descriptor_set = VK_NULL_HANDLE;
		uint32_t			current_frame_index = 0;

		std::unordered_map<VulkanPipelineKey,
						VkPipeline,
						VulkanPipelineKeyHash>		pipeline_cache;
		std::unordered_map<VkDescriptorSetLayout,
						VkPipelineLayout>				layout_cache;

		std::vector<DisposableObject *> disposable_objects;

		int dynamic_stencil_ref = 0;

		// Persistent 1×1 opaque-white dummy texture.  Written into every sprite
		// texture binding slot (0..15) that has no real texture bound so that the
		// descriptor set is fully populated and passes strict driver validation
		// without requiring VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT.
		VkImage        dummy_image        = VK_NULL_HANDLE;
		VkDeviceMemory dummy_image_memory = VK_NULL_HANDLE;
		VkImageView    dummy_image_view   = VK_NULL_HANDLE;
		VkSampler      dummy_sampler      = VK_NULL_HANDLE;
	};

} // namespace clan
