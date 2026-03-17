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
#include "VK/VK1/vulkan_pixel_buffer_provider.h"
#include "VK/vulkan_device.h"

namespace clan
{

VulkanPixelBufferProvider::VulkanPixelBufferProvider()  = default;
VulkanPixelBufferProvider::~VulkanPixelBufferProvider() { dispose(); }

void VulkanPixelBufferProvider::on_dispose()
{
	unlock();   // unmap if still mapped
	if (vk_device)
	{
		VkDevice dev = vk_device->get_device();
		if (buffer != VK_NULL_HANDLE)
		{ vkDestroyBuffer(dev, buffer, nullptr); buffer = VK_NULL_HANDLE; }
		if (memory != VK_NULL_HANDLE)
		{ vkFreeMemory(dev, memory, nullptr);  memory = VK_NULL_HANDLE; }
	}
}

// =============================================================================
//  Internal allocation helper
// =============================================================================

void VulkanPixelBufferProvider::ensure_buffer(VkDeviceSize bytes)
{
	if (bytes <= allocated_bytes && buffer != VK_NULL_HANDLE) return;

	// Destroy old resources
	VkDevice dev = vk_device->get_device();
	if (mapped_ptr) { vkUnmapMemory(dev, memory); mapped_ptr = nullptr; }
	if (buffer != VK_NULL_HANDLE) { vkDestroyBuffer(dev, buffer, nullptr); buffer = VK_NULL_HANDLE; }
	if (memory != VK_NULL_HANDLE) { vkFreeMemory(dev, memory, nullptr);   memory = VK_NULL_HANDLE; }

	// GL_PIXEL_PACK_BUFFER (read-back) and GL_PIXEL_UNPACK_BUFFER (upload) both
	// need TRANSFER_SRC and TRANSFER_DST so one object covers either direction.
	VkBufferCreateInfo ci{};
	ci.sType	= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	ci.size		= bytes;
	ci.usage	= VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(dev, &ci, nullptr, &buffer) != VK_SUCCESS)
		throw Exception("VulkanPixelBufferProvider: vkCreateBuffer failed");

	VkMemoryRequirements mr{};
	vkGetBufferMemoryRequirements(dev, buffer, &mr);

	VkMemoryAllocateInfo ai{};
	ai.sType		= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	ai.allocationSize  = mr.size;
	ai.memoryTypeIndex = vk_device->find_memory_type(
		mr.memoryTypeBits,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	if (vkAllocateMemory(dev, &ai, nullptr, &memory) != VK_SUCCESS)
		throw Exception("VulkanPixelBufferProvider: vkAllocateMemory failed");

	vkBindBufferMemory(dev, buffer, memory, 0);
	allocated_bytes = mr.size;
}

// =============================================================================
//  create
// =============================================================================

void VulkanPixelBufferProvider::create(const void *data, const Size &size,
										PixelBufferDirection /*direction*/,
										TextureFormat format,
										BufferUsage /*usage*/)
{
	buf_size   = size;
	tex_format = format;

	// Determine bytes per pixel from the format
	int bpp = 4; // default rgba8
	switch (format)
	{
	case TextureFormat::rgba8:
	case TextureFormat::bgra8: bpp = 4; break;
	case TextureFormat::rgb8:
	case TextureFormat::bgr8:  bpp = 3; break;
	case TextureFormat::rg8:   bpp = 2; break;
	case TextureFormat::r8:	bpp = 1; break;
	case TextureFormat::rgba16f:
	case TextureFormat::rgba16: bpp = 8; break;
	case TextureFormat::rgba32f: bpp = 16; break;
	default: bpp = 4; break;
	}

	VkDeviceSize bytes = static_cast<VkDeviceSize>(size.width) * size.height * bpp;
	ensure_buffer(bytes);

	if (data)
	{
		// Upload initial data (replaces glBufferData with non-null pointer)
		void *mapped = nullptr;
		vkMapMemory(vk_device->get_device(), memory, 0, bytes, 0, &mapped);
		std::memcpy(mapped, data, static_cast<size_t>(bytes));
		vkUnmapMemory(vk_device->get_device(), memory);
	}
}

// =============================================================================
//  lock / unlock
// =============================================================================

void VulkanPixelBufferProvider::lock(GraphicContext & /*gc*/, BufferAccess /*access*/)
{
	if (mapped_ptr || buffer == VK_NULL_HANDLE) return;
	vkMapMemory(vk_device->get_device(), memory, 0, allocated_bytes, 0, &mapped_ptr);
}

void VulkanPixelBufferProvider::unlock()
{
	if (!mapped_ptr || memory == VK_NULL_HANDLE) return;
	vkUnmapMemory(vk_device->get_device(), memory);
	mapped_ptr = nullptr;
}

void *VulkanPixelBufferProvider::get_data()
{
	// Caller must have called lock() first
	return mapped_ptr;
}

int VulkanPixelBufferProvider::get_pitch() const
{
	// Vulkan staging buffers are tightly packed — no row padding.
	// pitch = width * bytes_per_pixel
	int bpp = 4;
	switch (tex_format)
	{
	case TextureFormat::rgba8:
	case TextureFormat::bgra8: bpp = 4; break;
	case TextureFormat::rgb8:
	case TextureFormat::bgr8:  bpp = 3; break;
	case TextureFormat::rg8:   bpp = 2; break;
	case TextureFormat::r8:	bpp = 1; break;
	case TextureFormat::rgba16f: bpp = 8; break;
	case TextureFormat::rgba32f: bpp = 16; break;
	default: bpp = 4; break;
	}
	return buf_size.width * bpp;
}

// =============================================================================
//  upload_data
// =============================================================================

void VulkanPixelBufferProvider::upload_data(GraphicContext & /*gc*/,
											const Rect &dest_rect,
											const void *data)
{
	if (!data || buffer == VK_NULL_HANDLE) return;

	int bpp = 4;
	switch (tex_format)
	{
	case TextureFormat::rgba8:
	case TextureFormat::bgra8: bpp = 4; break;
	case TextureFormat::rgb8:
	case TextureFormat::bgr8:  bpp = 3; break;
	case TextureFormat::rg8:   bpp = 2; break;
	case TextureFormat::r8:	bpp = 1; break;
	default: bpp = 4; break;
	}

	VkDeviceSize bytes = static_cast<VkDeviceSize>(dest_rect.get_width()) *
						dest_rect.get_height() * bpp;

	void *mapped = nullptr;
	vkMapMemory(vk_device->get_device(), memory, 0, bytes, 0, &mapped);
	std::memcpy(mapped, data, static_cast<size_t>(bytes));
	vkUnmapMemory(vk_device->get_device(), memory);
}

} // namespace clan
