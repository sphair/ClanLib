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
#include "VK/VK1/vulkan_render_buffer_provider.h"
#include "VK/vulkan_device.h"
#include "API/Display/Render/shared_gc_data.h"

namespace clan
{
	VulkanRenderBufferProvider::VulkanRenderBufferProvider()
	{
		SharedGCData::add_disposable(this);
	}

	VulkanRenderBufferProvider::~VulkanRenderBufferProvider()
	{
		dispose();
		SharedGCData::remove_disposable(this);
	}

	void VulkanRenderBufferProvider::on_dispose()
	{
		if (!vk_device) return;
		VkDevice dev = vk_device->get_device();
		if (image_view != VK_NULL_HANDLE)
		{
			vkDestroyImageView(dev, image_view, nullptr);
			image_view = VK_NULL_HANDLE;
		}
		if (image != VK_NULL_HANDLE)
		{
			// vmaDestroyImage destroys both the VkImage and frees the allocation.
			vmaDestroyImage(vk_device->get_allocator(), image, allocation);
			image	= VK_NULL_HANDLE;
			allocation = VK_NULL_HANDLE;
		}
	}

	void VulkanRenderBufferProvider::create(int width, int height,
											TextureFormat texture_format,
											int multisample_samples)
	{
		throw_if_disposed();
		if (!vk_device)
			throw Exception("VulkanRenderBufferProvider::create() called without a device — call set_device() first");

		rb_width  = width;
		rb_height = height;
		vk_format = to_vk_format(texture_format);

		VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;
		if	(multisample_samples >= 64) samples = VK_SAMPLE_COUNT_64_BIT;
		else if (multisample_samples >= 32) samples = VK_SAMPLE_COUNT_32_BIT;
		else if (multisample_samples >= 16) samples = VK_SAMPLE_COUNT_16_BIT;
		else if (multisample_samples >= 8)  samples = VK_SAMPLE_COUNT_8_BIT;
		else if (multisample_samples >= 4)  samples = VK_SAMPLE_COUNT_4_BIT;
		else if (multisample_samples >= 2)  samples = VK_SAMPLE_COUNT_2_BIT;

		bool is_depth = (vk_format == VK_FORMAT_D24_UNORM_S8_UINT ||
						vk_format == VK_FORMAT_D32_SFLOAT_S8_UINT ||
						vk_format == VK_FORMAT_D32_SFLOAT);

		VkImageUsageFlags usage = is_depth
			? VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
			: VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		VkImageCreateInfo img_info{};
		img_info.sType		= VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		img_info.imageType	= VK_IMAGE_TYPE_2D;
		img_info.format		= vk_format;
		img_info.extent		= { (uint32_t)width, (uint32_t)height, 1 };
		img_info.mipLevels	= 1;
		img_info.arrayLayers   = 1;
		img_info.samples	= samples;
		img_info.tiling		= VK_IMAGE_TILING_OPTIMAL;
		img_info.usage		= usage;
		img_info.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;
		img_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		VmaAllocationCreateInfo alloc_ci{};
		alloc_ci.usage = VMA_MEMORY_USAGE_GPU_ONLY;

		if (vmaCreateImage(vk_device->get_allocator(), &img_info, &alloc_ci,
						&image, &allocation, nullptr) != VK_SUCCESS)
			throw Exception("Failed to create Vulkan render buffer image via VMA");

		VkImageAspectFlags aspect = is_depth
			? (VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT)
			: VK_IMAGE_ASPECT_COLOR_BIT;
		// Pure depth-only formats don't have a stencil aspect.
		if (vk_format == VK_FORMAT_D32_SFLOAT)
			aspect = VK_IMAGE_ASPECT_DEPTH_BIT;

		VkImageViewCreateInfo view_info{};
		view_info.sType						= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		view_info.image						= image;
		view_info.viewType						= VK_IMAGE_VIEW_TYPE_2D;
		view_info.format						= vk_format;
		view_info.subresourceRange.aspectMask	= aspect;
		view_info.subresourceRange.baseMipLevel   = 0;
		view_info.subresourceRange.levelCount	= 1;
		view_info.subresourceRange.baseArrayLayer = 0;
		view_info.subresourceRange.layerCount	= 1;

		if (vkCreateImageView(vk_device->get_device(), &view_info, nullptr, &image_view) != VK_SUCCESS)
			throw Exception("Failed to create Vulkan render buffer image view");
	}

	VkFormat VulkanRenderBufferProvider::to_vk_format(TextureFormat fmt)
	{
		switch (fmt)
		{
		case TextureFormat::rgba8:			return VK_FORMAT_R8G8B8A8_UNORM;
		case TextureFormat::bgra8:			return VK_FORMAT_B8G8R8A8_UNORM;
		case TextureFormat::rgba16f:			return VK_FORMAT_R16G16B16A16_SFLOAT;
		case TextureFormat::rgba32f:			return VK_FORMAT_R32G32B32A32_SFLOAT;
		case TextureFormat::depth24_stencil8:   return VK_FORMAT_D24_UNORM_S8_UINT;
		case TextureFormat::depth_component32f: return VK_FORMAT_D32_SFLOAT;
		default:
			throw Exception("VulkanRenderBufferProvider: unsupported TextureFormat");
		}
	}
}
