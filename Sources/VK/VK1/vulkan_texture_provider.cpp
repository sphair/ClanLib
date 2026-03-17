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

#include "VK/precomp.h"
#include "VK/VK1/vulkan_texture_provider.h"
#include "VK/vulkan_device.h"
#include "VK/VK1/vulkan_graphic_context_provider.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/Render/shared_gc_data.h"
#include <cstring>
#include <algorithm>

namespace clan
{
	// ---- Construction ----

	VulkanTextureProvider::VulkanTextureProvider(TextureDimensions texture_dimensions)
	{
		switch (texture_dimensions)
		{
		case TextureDimensions::_1d:
			image_type = VK_IMAGE_TYPE_1D;
			view_type  = VK_IMAGE_VIEW_TYPE_1D;
			break;
		case TextureDimensions::_1d_array:
			image_type = VK_IMAGE_TYPE_1D;
			view_type  = VK_IMAGE_VIEW_TYPE_1D_ARRAY;
			break;
		case TextureDimensions::_2d:
			image_type = VK_IMAGE_TYPE_2D;
			view_type  = VK_IMAGE_VIEW_TYPE_2D;
			break;
		case TextureDimensions::_2d_array:
			image_type = VK_IMAGE_TYPE_2D;
			view_type  = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
			break;
		case TextureDimensions::_3d:
			image_type = VK_IMAGE_TYPE_3D;
			view_type  = VK_IMAGE_VIEW_TYPE_3D;
			break;
		case TextureDimensions::_cube:
			image_type = VK_IMAGE_TYPE_2D;
			view_type  = VK_IMAGE_VIEW_TYPE_CUBE;
			break;
		default:
			throw Exception("Unsupported texture dimensions");
		}
		SharedGCData::add_disposable(this);
	}

	VulkanTextureProvider::VulkanTextureProvider(VulkanDevice *device, VkImage existing,
												VkFormat format, VkImageViewType vtype,
												bool owns)
		: vk_device(device), image(existing), vk_format(format), view_type(vtype),
		owns_image(owns), image_type(VK_IMAGE_TYPE_2D)
	{
		SharedGCData::add_disposable(this);
		create_image_view();
		rebuild_sampler();
	}

	VulkanTextureProvider::~VulkanTextureProvider()
	{
		dispose();
		SharedGCData::remove_disposable(this);
	}

	void VulkanTextureProvider::on_dispose()
	{
		if (!vk_device) return;
		VkDevice dev = vk_device->get_device();

		if (sampler	!= VK_NULL_HANDLE) { vkDestroySampler(dev, sampler, nullptr);		sampler = VK_NULL_HANDLE; }
		if (image_view   != VK_NULL_HANDLE) { vkDestroyImageView(dev, image_view, nullptr);	image_view = VK_NULL_HANDLE; }
		if (owns_image)
		{
			if (image		!= VK_NULL_HANDLE) { vkDestroyImage(dev, image, nullptr);		image = VK_NULL_HANDLE; }
			if (image_memory != VK_NULL_HANDLE) { vkFreeMemory(dev, image_memory, nullptr);	image_memory = VK_NULL_HANDLE; }
		}
	}

	// ---- create() – replaces glTexImage* ----
	void VulkanTextureProvider::create(int new_width, int new_height, int new_depth,
									int new_array_size, TextureFormat texture_format,
									int levels)
	{
		throw_if_disposed();
		width	= new_width;
		height	= new_height;
		depth	= new_depth;
		array_size = new_array_size;
		vk_format  = to_vk_format(texture_format);

		if (levels == 0)
		{
			levels = 1;
			while (std::max(new_width >> levels, 1) != 1 ||
				std::max(new_height >> levels, 1) != 1) levels++;
		}
		mip_levels = levels;

		VkImageCreateInfo img_info{};
		img_info.sType		= VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		img_info.imageType	= image_type;
		img_info.format		= vk_format;
		img_info.extent		= { (uint32_t)width, (uint32_t)std::max(height,1), (uint32_t)std::max(depth,1) };
		img_info.mipLevels	= mip_levels;
		img_info.arrayLayers   = (uint32_t)std::max(array_size, 1);
		img_info.samples	= VK_SAMPLE_COUNT_1_BIT;
		img_info.tiling		= VK_IMAGE_TILING_OPTIMAL;
		img_info.usage		= VK_IMAGE_USAGE_TRANSFER_DST_BIT |
								VK_IMAGE_USAGE_TRANSFER_SRC_BIT |
								VK_IMAGE_USAGE_SAMPLED_BIT;
		img_info.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;
		img_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		if (view_type == VK_IMAGE_VIEW_TYPE_CUBE)
			img_info.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;

		if (vkCreateImage(vk_device->get_device(), &img_info, nullptr, &image) != VK_SUCCESS)
			throw Exception("Failed to create Vulkan texture image");

		VkMemoryRequirements mem_req{};
		vkGetImageMemoryRequirements(vk_device->get_device(), image, &mem_req);

		VkMemoryAllocateInfo alloc_info{};
		alloc_info.sType		= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		alloc_info.allocationSize  = mem_req.size;
		alloc_info.memoryTypeIndex = vk_device->find_memory_type(
			mem_req.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		if (vkAllocateMemory(vk_device->get_device(), &alloc_info, nullptr, &image_memory) != VK_SUCCESS)
			throw Exception("Failed to allocate Vulkan texture memory");

		if (vkBindImageMemory(vk_device->get_device(), image, image_memory, 0) != VK_SUCCESS)
			throw Exception("Failed to bind Vulkan texture image memory");
		current_layout = VK_IMAGE_LAYOUT_UNDEFINED;

		create_image_view();
		rebuild_sampler();
	}

	// ---- copy_from() – replaces glTexSubImage* ----
	// Vulkan image origin is top-left; GL was bottom-left.
	// We flip the pixel data vertically before uploading.
	void VulkanTextureProvider::copy_from(GraphicContext &/*gc*/, int x, int y, int slice,
										int level, const PixelBuffer &src,
										const Rect &src_rect)
	{
		throw_if_disposed();

		// Convert pixel data to RGBA8 for simplicity (mirror of GL3 fallback)
		PixelBuffer converted = src.to_format(TextureFormat::rgba8);

		int copy_width  = src_rect.get_width();
		int copy_height = src_rect.get_height();
		int bytes_per_pixel = 4; // rgba8
		int row_bytes   = copy_width * bytes_per_pixel;
		int total_bytes = row_bytes * copy_height;

		// Flip vertically (GL y_bottom_up → Vulkan y_top_down)
		std::vector<uint8_t> flipped(total_bytes);
		const uint8_t *src_data = converted.get_data<uint8_t>() +
								src_rect.top * converted.get_pitch() +
								src_rect.left * bytes_per_pixel;
		for (int row = 0; row < copy_height; row++)
		{
			const uint8_t *src_row  = src_data  + row * converted.get_pitch();
			uint8_t	*dst_row  = flipped.data() + (copy_height - 1 - row) * row_bytes;
			std::memcpy(dst_row, src_row, row_bytes);
		}

		// Upload via staging buffer
		VkDevice dev = vk_device->get_device();

		VkBuffer	stg_buf{};
		VkDeviceMemory stg_mem{};

		VkBufferCreateInfo stg_info{};
		stg_info.sType	= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		stg_info.size		= total_bytes;
		stg_info.usage	= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		stg_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		if (vkCreateBuffer(dev, &stg_info, nullptr, &stg_buf) != VK_SUCCESS)
			throw Exception("Failed to create Vulkan texture upload staging buffer");

		VkMemoryRequirements stg_req{};
		vkGetBufferMemoryRequirements(dev, stg_buf, &stg_req);
		VkMemoryAllocateInfo stg_alloc{};
		stg_alloc.sType		= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		stg_alloc.allocationSize  = stg_req.size;
		stg_alloc.memoryTypeIndex = vk_device->find_memory_type(
			stg_req.memoryTypeBits,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		if (vkAllocateMemory(dev, &stg_alloc, nullptr, &stg_mem) != VK_SUCCESS)
		{
			vkDestroyBuffer(dev, stg_buf, nullptr);
			throw Exception("Failed to allocate Vulkan texture upload staging memory");
		}
		if (vkBindBufferMemory(dev, stg_buf, stg_mem, 0) != VK_SUCCESS)
		{
			vkFreeMemory(dev, stg_mem, nullptr);
			vkDestroyBuffer(dev, stg_buf, nullptr);
			throw Exception("Failed to bind Vulkan texture upload staging memory");
		}

		void *mapped = nullptr;
		if (vkMapMemory(dev, stg_mem, 0, total_bytes, 0, &mapped) != VK_SUCCESS)
		{
			vkFreeMemory(dev, stg_mem, nullptr);
			vkDestroyBuffer(dev, stg_buf, nullptr);
			throw Exception("Failed to map Vulkan texture upload staging memory");
		}
		std::memcpy(mapped, flipped.data(), total_bytes);
		vkUnmapMemory(dev, stg_mem);

		VkCommandBuffer cmd = vk_device->begin_single_time_commands();

		// Transition to TRANSFER_DST
		VkImageMemoryBarrier barrier{};
		barrier.sType						= VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout					= current_layout;
		barrier.newLayout					= VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.srcQueueFamilyIndex			= VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex			= VK_QUEUE_FAMILY_IGNORED;
		barrier.image						= image;
		barrier.subresourceRange.aspectMask	= VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel   = level;
		barrier.subresourceRange.levelCount	= 1;
		barrier.subresourceRange.baseArrayLayer = slice;
		barrier.subresourceRange.layerCount	= 1;
		barrier.srcAccessMask				= 0;
		barrier.dstAccessMask				= VK_ACCESS_TRANSFER_WRITE_BIT;
		vkCmdPipelineBarrier(cmd,
			VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			0, 0, nullptr, 0, nullptr, 1, &barrier);

		VkBufferImageCopy region{};
		region.bufferOffset					= 0;
		region.bufferRowLength				= 0;
		region.bufferImageHeight			= 0;
		region.imageSubresource.aspectMask	= VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel	= level;
		region.imageSubresource.baseArrayLayer = slice;
		region.imageSubresource.layerCount	= 1;
		region.imageOffset					= { x, y, 0 };
		region.imageExtent					= { (uint32_t)copy_width, (uint32_t)copy_height, 1 };
		vkCmdCopyBufferToImage(cmd, stg_buf, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

		// Transition to SHADER_READ_ONLY
		barrier.oldLayout	= VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout	= VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		vkCmdPipelineBarrier(cmd,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
			0, 0, nullptr, 0, nullptr, 1, &barrier);
		current_layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

		vk_device->end_single_time_commands(cmd);

		vkDestroyBuffer(dev, stg_buf, nullptr);
		vkFreeMemory(dev, stg_mem, nullptr);
	}

	// ---- get_pixeldata() – replaces glGetTexImage ----
	PixelBuffer VulkanTextureProvider::get_pixeldata(GraphicContext &/*gc*/,
													TextureFormat texture_format,
													int level) const
	{
		throw_if_disposed();

		int mip_w = std::max(width  >> level, 1);
		int mip_h = std::max(height >> level, 1);
		int bytes = mip_w * mip_h * 4; // readback as RGBA8

		VkDevice dev = vk_device->get_device();

		VkBuffer	stg_buf{};
		VkDeviceMemory stg_mem{};

		VkBufferCreateInfo stg_info{};
		stg_info.sType	= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		stg_info.size		= bytes;
		stg_info.usage	= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		stg_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		if (vkCreateBuffer(dev, &stg_info, nullptr, &stg_buf) != VK_SUCCESS)
			throw Exception("Failed to create Vulkan texture readback staging buffer");

		VkMemoryRequirements stg_req{};
		vkGetBufferMemoryRequirements(dev, stg_buf, &stg_req);
		VkMemoryAllocateInfo stg_alloc{};
		stg_alloc.sType		= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		stg_alloc.allocationSize  = stg_req.size;
		stg_alloc.memoryTypeIndex = vk_device->find_memory_type(
			stg_req.memoryTypeBits,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		if (vkAllocateMemory(dev, &stg_alloc, nullptr, &stg_mem) != VK_SUCCESS)
		{
			vkDestroyBuffer(dev, stg_buf, nullptr);
			throw Exception("Failed to allocate Vulkan texture readback staging memory");
		}
		if (vkBindBufferMemory(dev, stg_buf, stg_mem, 0) != VK_SUCCESS)
		{
			vkFreeMemory(dev, stg_mem, nullptr);
			vkDestroyBuffer(dev, stg_buf, nullptr);
			throw Exception("Failed to bind Vulkan texture readback staging memory");
		}

		VkCommandBuffer cmd = vk_device->begin_single_time_commands();

		VkImageMemoryBarrier barrier{};
		barrier.sType						= VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout					= current_layout;
		barrier.newLayout					= VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.srcQueueFamilyIndex			= VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex			= VK_QUEUE_FAMILY_IGNORED;
		barrier.image						= image;
		barrier.subresourceRange.aspectMask	= VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel   = level;
		barrier.subresourceRange.levelCount	= 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount	= 1;
		barrier.srcAccessMask				= VK_ACCESS_SHADER_READ_BIT;
		barrier.dstAccessMask				= VK_ACCESS_TRANSFER_READ_BIT;
		vkCmdPipelineBarrier(cmd,
			VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			0, 0, nullptr, 0, nullptr, 1, &barrier);

		VkBufferImageCopy region{};
		region.imageSubresource.aspectMask	= VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel	= level;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount	= 1;
		region.imageExtent					= { (uint32_t)mip_w, (uint32_t)mip_h, 1 };
		vkCmdCopyImageToBuffer(cmd, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, stg_buf, 1, &region);

		// Transition back
		barrier.oldLayout	= VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.newLayout	= VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		vkCmdPipelineBarrier(cmd,
			VK_PIPELINE_STAGE_TRANSFER_BIT,
			VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
			0, 0, nullptr, 0, nullptr, 1, &barrier);

		vk_device->end_single_time_commands(cmd);

		void *mapped = nullptr;
		if (vkMapMemory(dev, stg_mem, 0, bytes, 0, &mapped) != VK_SUCCESS)
		{
			vkDestroyBuffer(dev, stg_buf, nullptr);
			vkFreeMemory(dev, stg_mem, nullptr);
			throw Exception("Failed to map Vulkan texture readback staging memory");
		}
		PixelBuffer result(mip_w, mip_h, TextureFormat::rgba8);
		// Flip back to bottom-up (GL convention) for callers expecting y_bottom_up
		const uint8_t *src_ptr = static_cast<const uint8_t *>(mapped);
		for (int row = 0; row < mip_h; row++)
		{
			const uint8_t *src_row = src_ptr + row * mip_w * 4;
			uint8_t *dst_row = result.get_data<uint8_t>() + (mip_h - 1 - row) * result.get_pitch();
			std::memcpy(dst_row, src_row, mip_w * 4);
		}
		vkUnmapMemory(dev, stg_mem);
		vkDestroyBuffer(dev, stg_buf, nullptr);
		vkFreeMemory(dev, stg_mem, nullptr);

		if (texture_format != TextureFormat::rgba8)
			return result.to_format(texture_format);
		return result;
	}

	// ---- generate_mipmap() – replaces glGenerateMipmap ----
	void VulkanTextureProvider::generate_mipmap()
	{
		throw_if_disposed();
		VkCommandBuffer cmd = vk_device->begin_single_time_commands();

		// Transition level 0 from its current layout to TRANSFER_SRC_OPTIMAL.
		// After create() or copy_from() the image is in SHADER_READ_ONLY_OPTIMAL,
		// not TRANSFER_DST_OPTIMAL, so we cannot assume the loop's first barrier
		// will see the right oldLayout.  We handle level 0 explicitly here.
		{
			VkImageMemoryBarrier b0{};
			b0.sType						= VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			b0.image						= image;
			b0.srcQueueFamilyIndex			= VK_QUEUE_FAMILY_IGNORED;
			b0.dstQueueFamilyIndex			= VK_QUEUE_FAMILY_IGNORED;
			b0.subresourceRange.aspectMask	= VK_IMAGE_ASPECT_COLOR_BIT;
			b0.subresourceRange.baseArrayLayer = 0;
			b0.subresourceRange.layerCount	= 1;
			b0.subresourceRange.baseMipLevel   = 0;
			b0.subresourceRange.levelCount	= 1;
			b0.oldLayout	= current_layout;
			b0.newLayout	= VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			b0.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
			b0.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			vkCmdPipelineBarrier(cmd,
				VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
				VK_PIPELINE_STAGE_TRANSFER_BIT,
				0, 0, nullptr, 0, nullptr, 1, &b0);
		}

		int32_t mip_w = width;
		int32_t mip_h = height;

		for (int i = 1; i < mip_levels; i++)
		{
			// Transition destination level i to TRANSFER_DST_OPTIMAL before blitting into it
			VkImageMemoryBarrier barrier{};
			barrier.sType						= VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			barrier.image						= image;
			barrier.srcQueueFamilyIndex			= VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex			= VK_QUEUE_FAMILY_IGNORED;
			barrier.subresourceRange.aspectMask	= VK_IMAGE_ASPECT_COLOR_BIT;
			barrier.subresourceRange.baseArrayLayer = 0;
			barrier.subresourceRange.layerCount	= 1;
			barrier.subresourceRange.levelCount	= 1;
			barrier.subresourceRange.baseMipLevel   = i;
			barrier.oldLayout	= VK_IMAGE_LAYOUT_UNDEFINED;
			barrier.newLayout	= VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			vkCmdPipelineBarrier(cmd,
				VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
				VK_PIPELINE_STAGE_TRANSFER_BIT,
				0, 0, nullptr, 0, nullptr, 1, &barrier);

			VkImageBlit blit{};
			blit.srcOffsets[0]				= { 0, 0, 0 };
			blit.srcOffsets[1]				= { mip_w, mip_h, 1 };
			blit.srcSubresource.aspectMask	= VK_IMAGE_ASPECT_COLOR_BIT;
			blit.srcSubresource.mipLevel	= i - 1;
			blit.srcSubresource.baseArrayLayer = 0;
			blit.srcSubresource.layerCount	= 1;
			blit.dstOffsets[0]				= { 0, 0, 0 };
			blit.dstOffsets[1]				= { mip_w > 1 ? mip_w / 2 : 1, mip_h > 1 ? mip_h / 2 : 1, 1 };
			blit.dstSubresource.aspectMask	= VK_IMAGE_ASPECT_COLOR_BIT;
			blit.dstSubresource.mipLevel	= i;
			blit.dstSubresource.baseArrayLayer = 0;
			blit.dstSubresource.layerCount	= 1;
			vkCmdBlitImage(cmd,
				image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1, &blit, VK_FILTER_LINEAR);

			// Transition level i from TRANSFER_DST to TRANSFER_SRC so the next
			// iteration can blit from it, then at the end transition to SHADER_READ_ONLY.
			barrier.subresourceRange.baseMipLevel = i;
			barrier.oldLayout	= VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			barrier.newLayout	= VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			vkCmdPipelineBarrier(cmd,
				VK_PIPELINE_STAGE_TRANSFER_BIT,
				VK_PIPELINE_STAGE_TRANSFER_BIT,
				0, 0, nullptr, 0, nullptr, 1, &barrier);

			if (mip_w > 1) mip_w /= 2;
			if (mip_h > 1) mip_h /= 2;
		}

		// Transition all levels from TRANSFER_SRC_OPTIMAL to SHADER_READ_ONLY_OPTIMAL
		// in a single barrier covering the whole mip chain.
		{
			VkImageMemoryBarrier final_barrier{};
			final_barrier.sType						= VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			final_barrier.image						= image;
			final_barrier.srcQueueFamilyIndex			= VK_QUEUE_FAMILY_IGNORED;
			final_barrier.dstQueueFamilyIndex			= VK_QUEUE_FAMILY_IGNORED;
			final_barrier.subresourceRange.aspectMask	= VK_IMAGE_ASPECT_COLOR_BIT;
			final_barrier.subresourceRange.baseArrayLayer = 0;
			final_barrier.subresourceRange.layerCount	= 1;
			final_barrier.subresourceRange.baseMipLevel   = 0;
			final_barrier.subresourceRange.levelCount	= mip_levels;
			final_barrier.oldLayout	= VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			final_barrier.newLayout	= VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			final_barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			final_barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			vkCmdPipelineBarrier(cmd,
				VK_PIPELINE_STAGE_TRANSFER_BIT,
				VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
				0, 0, nullptr, 0, nullptr, 1, &final_barrier);
		}

		vk_device->end_single_time_commands(cmd);
		current_layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	}

	// ---- Sampler state setters – each rebuilds the VkSampler ----
	// Unlike GL where glTexParameter() took effect immediately on the bound texture,
	// Vulkan requires a VkSampler to be explicitly created.

	void VulkanTextureProvider::set_min_lod(double v)	{ min_lod_val  = (float)v;  rebuild_sampler(); }
	void VulkanTextureProvider::set_max_lod(double v)	{ max_lod_val  = (float)v;  rebuild_sampler(); }
	void VulkanTextureProvider::set_lod_bias(double v)   { lod_bias_val = (float)v;  rebuild_sampler(); }
	void VulkanTextureProvider::set_base_level(int /*v*/) { /* Vulkan: use image view's baseMipLevel */ }
	void VulkanTextureProvider::set_max_level(int /*v*/)  { /* Vulkan: use image view's levelCount  */ }

	void VulkanTextureProvider::set_wrap_mode(TextureWrapMode s, TextureWrapMode t, TextureWrapMode r)
	{ wrap_s = to_vk_wrap(s); wrap_t = to_vk_wrap(t); wrap_r = to_vk_wrap(r); rebuild_sampler(); }
	void VulkanTextureProvider::set_wrap_mode(TextureWrapMode s, TextureWrapMode t)
	{ wrap_s = to_vk_wrap(s); wrap_t = to_vk_wrap(t); rebuild_sampler(); }
	void VulkanTextureProvider::set_wrap_mode(TextureWrapMode s)
	{ wrap_s = to_vk_wrap(s); rebuild_sampler(); }

	void VulkanTextureProvider::set_min_filter(TextureFilter f)
	{
		min_filter_vk = to_vk_filter(f);
		mipmap_mode   = (f == TextureFilter::nearest || f == TextureFilter::nearest_mipmap_nearest)
						? VK_SAMPLER_MIPMAP_MODE_NEAREST : VK_SAMPLER_MIPMAP_MODE_LINEAR;
		rebuild_sampler();
	}

	void VulkanTextureProvider::set_mag_filter(TextureFilter f)
	{
		if (f != TextureFilter::nearest && f != TextureFilter::linear)
			throw Exception("Only nearest/linear are valid mag filter options");
		mag_filter_vk = to_vk_filter(f);
		rebuild_sampler();
	}

	void VulkanTextureProvider::set_max_anisotropy(float v)
	{ anisotropy_max = v; rebuild_sampler(); }

	void VulkanTextureProvider::set_texture_compare(TextureCompareMode mode, CompareFunction func)
	{
		compare_enable = (mode != TextureCompareMode::none);
		compare_op	= to_vk_compare(func);
		rebuild_sampler();
	}

	std::unique_ptr<TextureProvider> VulkanTextureProvider::create_view(
		TextureDimensions /*dims*/, TextureFormat /*fmt*/,
		int /*min_level*/, int /*num_levels*/, int /*min_layer*/, int /*num_layers*/)
	{
		// TODO: Create a VkImageView with restricted mip/layer range via vkCreateImageView.
		throw Exception("VulkanTextureProvider::create_view() not yet implemented");
	}

	void VulkanTextureProvider::copy_image_from(int x, int y, int w, int h, int level,
												TextureFormat fmt, GraphicContextProvider *gc)
	{
		// TODO: vkCmdCopyImage from the current framebuffer attachment.
		throw Exception("VulkanTextureProvider::copy_image_from() not yet implemented");
	}

	void VulkanTextureProvider::copy_subimage_from(int ox, int oy, int x, int y,
													int w, int h, int level,
													GraphicContextProvider *gc)
	{
		// TODO: vkCmdCopyImage subregion.
		throw Exception("VulkanTextureProvider::copy_subimage_from() not yet implemented");
	}

	// ---- Private helpers ----

	void VulkanTextureProvider::rebuild_sampler()
	{
		if (!vk_device) return;
		VkDevice dev = vk_device->get_device();
		if (sampler != VK_NULL_HANDLE) { vkDestroySampler(dev, sampler, nullptr); sampler = VK_NULL_HANDLE; }

		VkSamplerCreateInfo si{};
		si.sType				= VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		si.magFilter			= mag_filter_vk;
		si.minFilter			= min_filter_vk;
		si.mipmapMode			= mipmap_mode;
		si.addressModeU			= wrap_s;
		si.addressModeV			= wrap_t;
		si.addressModeW			= wrap_r;
		si.mipLodBias			= lod_bias_val;
		si.anisotropyEnable		= (anisotropy_max > 1.0f) ? VK_TRUE : VK_FALSE;
		si.maxAnisotropy		= anisotropy_max;
		si.compareEnable		= compare_enable ? VK_TRUE : VK_FALSE;
		si.compareOp			= compare_op;
		si.minLod				= min_lod_val;
		si.maxLod				= max_lod_val;
		si.borderColor			= VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
		si.unnormalizedCoordinates = VK_FALSE;

		if (vkCreateSampler(dev, &si, nullptr, &sampler) != VK_SUCCESS)
			throw Exception("Failed to create Vulkan sampler");
	}

	void VulkanTextureProvider::create_image_view()
	{
		if (!vk_device) return;
		if (image_view != VK_NULL_HANDLE)
		{
			vkDestroyImageView(vk_device->get_device(), image_view, nullptr);
			image_view = VK_NULL_HANDLE;
		}

		VkImageAspectFlags aspect = VK_IMAGE_ASPECT_COLOR_BIT;
		// For depth/stencil formats adjust the aspect
		if (vk_format == VK_FORMAT_D24_UNORM_S8_UINT || vk_format == VK_FORMAT_D32_SFLOAT_S8_UINT)
			aspect = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
		else if (vk_format == VK_FORMAT_D32_SFLOAT)
			aspect = VK_IMAGE_ASPECT_DEPTH_BIT;

		VkImageViewCreateInfo ci{};
		ci.sType						= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		ci.image						= image;
		ci.viewType						= view_type;
		ci.format						= vk_format;
		ci.subresourceRange.aspectMask	= aspect;
		ci.subresourceRange.baseMipLevel   = 0;
		ci.subresourceRange.levelCount	= mip_levels;
		ci.subresourceRange.baseArrayLayer = 0;
		ci.subresourceRange.layerCount	= std::max(array_size, 1);

		if (vkCreateImageView(vk_device->get_device(), &ci, nullptr, &image_view) != VK_SUCCESS)
			throw Exception("Failed to create Vulkan image view");
	}

	void VulkanTextureProvider::transition_layout(VkImageLayout new_layout)
	{
		if (current_layout == new_layout) return;
		VkCommandBuffer cmd = vk_device->begin_single_time_commands();

		VkImageMemoryBarrier barrier{};
		barrier.sType						= VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout					= current_layout;
		barrier.newLayout					= new_layout;
		barrier.srcQueueFamilyIndex			= VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex			= VK_QUEUE_FAMILY_IGNORED;
		barrier.image						= image;
		barrier.subresourceRange.aspectMask	= VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel   = 0;
		barrier.subresourceRange.levelCount	= mip_levels;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount	= std::max(array_size, 1);

		VkPipelineStageFlags src_stage, dst_stage;
		if (current_layout == VK_IMAGE_LAYOUT_UNDEFINED &&
			new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			src_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			dst_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else
		{
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			src_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			dst_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}

		vkCmdPipelineBarrier(cmd, src_stage, dst_stage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
		vk_device->end_single_time_commands(cmd);
		current_layout = new_layout;
	}

	// ---- Enum converters ----

	VkFormat VulkanTextureProvider::to_vk_format(TextureFormat fmt)
	{
		switch (fmt)
		{
		case TextureFormat::rgba8:		return VK_FORMAT_R8G8B8A8_UNORM;
		case TextureFormat::bgra8:		return VK_FORMAT_B8G8R8A8_UNORM;
		case TextureFormat::rgb8:			return VK_FORMAT_R8G8B8_UNORM;
		case TextureFormat::r8:			return VK_FORMAT_R8_UNORM;
		case TextureFormat::rg8:			return VK_FORMAT_R8G8_UNORM;
		case TextureFormat::rgba16f:		return VK_FORMAT_R16G16B16A16_SFLOAT;
		case TextureFormat::rgba32f:		return VK_FORMAT_R32G32B32A32_SFLOAT;
		case TextureFormat::depth24_stencil8:return VK_FORMAT_D24_UNORM_S8_UINT;
		case TextureFormat::depth_component32f:		return VK_FORMAT_D32_SFLOAT;
		default:
			throw Exception("VulkanTextureProvider: unsupported TextureFormat");
		}
	}

	VkFilter VulkanTextureProvider::to_vk_filter(TextureFilter f)
	{
		switch (f)
		{
		case TextureFilter::nearest:
		case TextureFilter::nearest_mipmap_nearest:
		case TextureFilter::nearest_mipmap_linear:
			return VK_FILTER_NEAREST;
		default:
			return VK_FILTER_LINEAR;
		}
	}

	VkSamplerAddressMode VulkanTextureProvider::to_vk_wrap(TextureWrapMode m)
	{
		switch (m)
		{
		case TextureWrapMode::clamp_to_edge:   return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		case TextureWrapMode::repeat:		return VK_SAMPLER_ADDRESS_MODE_REPEAT;
		case TextureWrapMode::mirrored_repeat: return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
		default:							return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		}
	}

	VkCompareOp VulkanTextureProvider::to_vk_compare(CompareFunction f)
	{
		switch (f)
		{
		case CompareFunction::lequal:   return VK_COMPARE_OP_LESS_OR_EQUAL;
		case CompareFunction::gequal:   return VK_COMPARE_OP_GREATER_OR_EQUAL;
		case CompareFunction::less:	return VK_COMPARE_OP_LESS;
		case CompareFunction::greater:  return VK_COMPARE_OP_GREATER;
		case CompareFunction::equal:	return VK_COMPARE_OP_EQUAL;
		case CompareFunction::notequal: return VK_COMPARE_OP_NOT_EQUAL;
		case CompareFunction::always:   return VK_COMPARE_OP_ALWAYS;
		case CompareFunction::never:	return VK_COMPARE_OP_NEVER;
		default:						return VK_COMPARE_OP_ALWAYS;
		}
	}
}
