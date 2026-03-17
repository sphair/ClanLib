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
#include "VK/VK1/vulkan_storage_buffer_provider.h"
#include "VK/VK1/vulkan_transfer_buffer_provider.h"
#include "VK/vulkan_device.h"

namespace clan
{
	VulkanStorageBufferProvider::VulkanStorageBufferProvider()  {}
	VulkanStorageBufferProvider::~VulkanStorageBufferProvider() {}

	void VulkanStorageBufferProvider::create(int size, int /*stride*/, BufferUsage /*usage*/)
	{
		if (!vk_device)
			throw Exception("VulkanStorageBufferProvider: set_device() must be called before create()");

		buffer.create(vk_device, nullptr, size,
					VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
					VK_BUFFER_USAGE_TRANSFER_DST_BIT   |
					VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
					VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	}

	void VulkanStorageBufferProvider::create(const void *data, int size, int /*stride*/,
											BufferUsage /*usage*/)
	{
		if (!vk_device)
			throw Exception("VulkanStorageBufferProvider: set_device() must be called before create()");

		// Initial data provided: create DEVICE_LOCAL buffer; VulkanBufferObjectProvider
		// will upload via an internal staging buffer.
		buffer.create(vk_device, data, size,
					VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
					VK_BUFFER_USAGE_TRANSFER_DST_BIT   |
					VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
					VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	}

	void VulkanStorageBufferProvider::copy_from(GraphicContext &gc,
												TransferBuffer &transfer_buffer,
												int dest_pos, int src_pos, int size)
	{
		auto *src = static_cast<VulkanTransferBufferProvider *>(transfer_buffer.get_provider());
		buffer.copy_from(gc, src->get_buffer_provider(), dest_pos, src_pos, size);
	}

	void VulkanStorageBufferProvider::copy_to(GraphicContext &gc,
											TransferBuffer &transfer_buffer,
											int dest_pos, int src_pos, int size)
	{
		auto *dst = static_cast<VulkanTransferBufferProvider *>(transfer_buffer.get_provider());
		buffer.copy_to(gc, dst->get_buffer_provider(), dest_pos, src_pos, size);
	}
}
