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

#include "API/VK/vk_mem_alloc_config.h"
#include "API/Display/TargetProviders/texture_provider.h"
#include "API/Core/System/disposable_object.h"

namespace clan
{
	class VulkanDevice;

	class VulkanTextureProvider : public TextureProvider, DisposableObject
	{
	public:
		/// Construct a new empty texture of the given dimensionality.
		explicit VulkanTextureProvider(TextureDimensions texture_dimensions);

		/// Wrap an existing VkImage (e.g. swapchain image – not owned).
		VulkanTextureProvider(VulkanDevice *device, VkImage existing_image,
							VkFormat format, VkImageViewType view_type,
							bool owns_image = false);

		~VulkanTextureProvider() override;

		// ---- Accessors ----
		VkImage	get_image()		const { return image; }
		VkImageView   get_image_view()	const { return image_view; }
		VkSampler	get_sampler()	const { return sampler; }
		VkFormat	get_vk_format()	const { return vk_format; }
		int		get_width()		const { return width; }
		int		get_height()		const { return height; }
		VkImageLayout get_current_layout() const { return current_layout; }
		VkImageViewType get_view_type()   const { return view_type; }

		/// Called by VulkanGraphicContextProvider immediately after alloc_texture().
		void set_device(VulkanDevice *device) { vk_device = device; }

		/// Transition the image to a new layout using a one-shot command buffer.
		void transition_layout(VkImageLayout new_layout);

		// ---- TextureProvider interface ----
		void generate_mipmap() override;
		void create(int width, int height, int depth, int array_size,
					TextureFormat texture_format, int levels) override;

		PixelBuffer get_pixeldata(GraphicContext &gc,
								TextureFormat texture_format, int level) const override;

		void copy_from(GraphicContext &gc, int x, int y, int slice, int level,
					const PixelBuffer &src, const Rect &src_rect) override;

		void copy_image_from(int x, int y, int width, int height, int level,
							TextureFormat texture_format, GraphicContextProvider *gc) override;
		void copy_subimage_from(int offset_x, int offset_y, int x, int y,
								int width, int height, int level,
								GraphicContextProvider *gc) override;

		void set_min_lod(double min_lod) override;
		void set_max_lod(double max_lod) override;
		void set_lod_bias(double lod_bias) override;
		void set_base_level(int base_level) override;
		void set_max_level(int max_level)  override;

		void set_wrap_mode(TextureWrapMode wrap_s,
						TextureWrapMode wrap_t,
						TextureWrapMode wrap_r) override;
		void set_wrap_mode(TextureWrapMode wrap_s, TextureWrapMode wrap_t) override;
		void set_wrap_mode(TextureWrapMode wrap_s) override;

		void set_min_filter(TextureFilter filter) override;
		void set_mag_filter(TextureFilter filter) override;
		void set_max_anisotropy(float v) override;
		void set_texture_compare(TextureCompareMode mode, CompareFunction func) override;

		std::unique_ptr<TextureProvider> create_view(
			TextureDimensions texture_dimensions, TextureFormat texture_format,
			int min_level, int num_levels, int min_layer, int num_layers) override;

	private:
		void on_dispose() override;
		void rebuild_sampler();
		void create_image_view();

		/// Convert ClanLib TextureFormat to VkFormat.
		static VkFormat to_vk_format(TextureFormat fmt);
		static VkFilter to_vk_filter(TextureFilter f);
		static VkSamplerAddressMode to_vk_wrap(TextureWrapMode m);
		static VkCompareOp to_vk_compare(CompareFunction f);

		VulkanDevice   *vk_device	= nullptr;
		VkImage		image		= VK_NULL_HANDLE;
		VmaAllocation   image_memory  = VK_NULL_HANDLE;
		VkImageView	image_view	= VK_NULL_HANDLE;
		VkSampler	sampler	= VK_NULL_HANDLE;
		VkFormat		vk_format	= VK_FORMAT_UNDEFINED;
		VkImageViewType view_type	= VK_IMAGE_VIEW_TYPE_2D;
		VkImageType	image_type	= VK_IMAGE_TYPE_2D;
		mutable VkImageLayout current_layout = VK_IMAGE_LAYOUT_UNDEFINED;

		bool owns_image  = true;

		int width		= 0;
		int height	= 0;
		int depth		= 1;
		int array_size   = 1;
		int mip_levels   = 1;

		// Sampler parameters (rebuilt into VkSampler via rebuild_sampler)
		VkFilter			min_filter_vk	= VK_FILTER_LINEAR;
		VkFilter			mag_filter_vk	= VK_FILTER_LINEAR;
		VkSamplerMipmapMode   mipmap_mode	= VK_SAMPLER_MIPMAP_MODE_LINEAR;
		VkSamplerAddressMode  wrap_s		= VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		VkSamplerAddressMode  wrap_t		= VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		VkSamplerAddressMode  wrap_r		= VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		float				anisotropy_max   = 1.0f;
		float				min_lod_val	= 0.0f;
		float				max_lod_val	= VK_LOD_CLAMP_NONE;
		float				lod_bias_val	= 0.0f;
		bool				compare_enable   = false;
		VkCompareOp		compare_op	= VK_COMPARE_OP_ALWAYS;
	};
}
