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
	unlock();  // unmap if still mapped
	if (vk_device && buffer != VK_NULL_HANDLE)
	{
		vmaDestroyBuffer(vk_device->get_allocator(), buffer, allocation);
		buffer	= VK_NULL_HANDLE;
		allocation = VK_NULL_HANDLE;
	}
}

// =============================================================================
//  Internal allocation helper
// =============================================================================

void VulkanPixelBufferProvider::ensure_buffer(VkDeviceSize bytes)
{
	if (bytes <= allocated_bytes && buffer != VK_NULL_HANDLE) return;

	// Destroy old resources first (unlock mapped pointer if needed).
	if (mapped_ptr)
	{
		vmaUnmapMemory(vk_device->get_allocator(), allocation);
		mapped_ptr = nullptr;
	}
	if (buffer != VK_NULL_HANDLE)
	{
		vmaDestroyBuffer(vk_device->get_allocator(), buffer, allocation);
		buffer	= VK_NULL_HANDLE;
		allocation = VK_NULL_HANDLE;
	}

	// GL_PIXEL_PACK_BUFFER (read-back) and GL_PIXEL_UNPACK_BUFFER (upload) both
	// need TRANSFER_SRC and TRANSFER_DST so one object covers either direction.
	VkBufferCreateInfo ci{};
	ci.sType	= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	ci.size		= bytes;
	ci.usage	= VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	// CPU_TO_GPU covers both upload (write) and read-back (read) with HOST_VISIBLE.
	// We use VMA_MEMORY_USAGE_CPU_ONLY to guarantee HOST_VISIBLE | HOST_COHERENT,
	// which removes the need for explicit flushes/invalidates.
	VmaAllocationCreateInfo alloc_ci{};
	alloc_ci.usage = VMA_MEMORY_USAGE_CPU_ONLY;
	alloc_ci.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;

	VmaAllocationInfo alloc_info{};
	if (vmaCreateBuffer(vk_device->get_allocator(), &ci, &alloc_ci,
						&buffer, &allocation, &alloc_info) != VK_SUCCESS)
		throw Exception("VulkanPixelBufferProvider: vmaCreateBuffer failed");

	// Store the persistent mapped pointer. CPU_ONLY memory is always
	// HOST_VISIBLE|HOST_COHERENT so we do not need explicit flushes.
	mapped_ptr	= alloc_info.pMappedData;
	allocated_bytes = bytes;
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

	int bpp = 4; // default rgba8
	switch (format)
	{
	case TextureFormat::rgba8:
	case TextureFormat::bgra8:  bpp = 4;  break;
	case TextureFormat::rgb8:
	case TextureFormat::bgr8:   bpp = 3;  break;
	case TextureFormat::rg8:	bpp = 2;  break;
	case TextureFormat::r8:	bpp = 1;  break;
	case TextureFormat::rgba16f:
	case TextureFormat::rgba16: bpp = 8;  break;
	case TextureFormat::rgba32f: bpp = 16; break;
	default: bpp = 4; break;
	}

	VkDeviceSize bytes = static_cast<VkDeviceSize>(size.width) * size.height * bpp;
	ensure_buffer(bytes);

	if (data)
		std::memcpy(mapped_ptr, data, static_cast<size_t>(bytes));
}

// =============================================================================
//  lock / unlock
// =============================================================================

void VulkanPixelBufferProvider::lock(GraphicContext & /*gc*/, BufferAccess /*access*/)
{
	// The buffer was created with VMA_ALLOCATION_CREATE_MAPPED_BIT, so
	// mapped_ptr is already valid.  lock()/unlock() are effectively no-ops here
	// but we keep the interface consistent with other providers.
	if (buffer == VK_NULL_HANDLE)
		throw Exception("VulkanPixelBufferProvider: buffer not created — call create() first");
}

void VulkanPixelBufferProvider::unlock()
{
	// Nothing to do — persistent mapping stays active until the buffer is destroyed.
}

void *VulkanPixelBufferProvider::get_data()
{
	return mapped_ptr;
}

int VulkanPixelBufferProvider::get_pitch() const
{
	int bpp = 4;
	switch (tex_format)
	{
	case TextureFormat::rgba8:
	case TextureFormat::bgra8:  bpp = 4;  break;
	case TextureFormat::rgb8:
	case TextureFormat::bgr8:   bpp = 3;  break;
	case TextureFormat::rg8:	bpp = 2;  break;
	case TextureFormat::r8:	bpp = 1;  break;
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
	case TextureFormat::bgra8:  bpp = 4; break;
	case TextureFormat::rgb8:
	case TextureFormat::bgr8:   bpp = 3; break;
	case TextureFormat::rg8:	bpp = 2; break;
	case TextureFormat::r8:	bpp = 1; break;
	default: bpp = 4; break;
	}

	VkDeviceSize bytes = static_cast<VkDeviceSize>(dest_rect.get_width()) *
						dest_rect.get_height() * bpp;

	// The persistently-mapped pointer is always valid for CPU_ONLY memory.
	std::memcpy(mapped_ptr, data, static_cast<size_t>(bytes));
}

} // namespace clan
