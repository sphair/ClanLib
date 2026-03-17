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
#include "VK/VK1/vulkan_buffer_object_provider.h"
#include "VK/VK1/vulkan_graphic_context_provider.h"
#include "VK/vulkan_device.h"
#include "API/Display/Render/shared_gc_data.h"
#include <cstring>

namespace clan
{
	VulkanBufferObjectProvider::VulkanBufferObjectProvider()
	{
		SharedGCData::add_disposable(this);
	}

	VulkanBufferObjectProvider::~VulkanBufferObjectProvider()
	{
		dispose();
		SharedGCData::remove_disposable(this);
	}

	void VulkanBufferObjectProvider::on_dispose()
	{
		if (buffer != VK_NULL_HANDLE && vk_device)
		{
			vmaDestroyBuffer(vk_device->get_allocator(), buffer, allocation);
			buffer	= VK_NULL_HANDLE;
			allocation = VK_NULL_HANDLE;
		}
	}

	void VulkanBufferObjectProvider::create(VulkanDevice *device,
											const void *data, int size,
											VkBufferUsageFlags uflags,
											VkMemoryPropertyFlags mprops)
	{
		throw_if_disposed();
		vk_device	= device;
		buffer_size  = size;
		usage_flags  = uflags;
		memory_props = mprops;

		// If data is provided for a DEVICE_LOCAL buffer we need an upload path
		// via a staging buffer.  For HOST_VISIBLE we can map directly.
		bool needs_staging = (mprops & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) &&
							!(mprops & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

		VkBufferCreateInfo buf_info{};
		buf_info.sType	= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		buf_info.size		= size;
		buf_info.usage	= uflags | (needs_staging ? VK_BUFFER_USAGE_TRANSFER_DST_BIT : 0);
		buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VmaAllocationCreateInfo alloc_ci{};
		alloc_ci.requiredFlags = mprops;
		if (mprops & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
			alloc_ci.usage = VMA_MEMORY_USAGE_GPU_ONLY;
		else
			alloc_ci.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

		if (vmaCreateBuffer(vk_device->get_allocator(), &buf_info, &alloc_ci,
							&buffer, &allocation, nullptr) != VK_SUCCESS)
			throw Exception("Failed to create/allocate Vulkan buffer via VMA");

		if (data && size > 0)
		{
			if (needs_staging)
			{
				// Upload via temporary HOST_VISIBLE staging buffer using
				// VMA_MEMORY_USAGE_CPU_ONLY (always HOST_VISIBLE|HOST_COHERENT).
				VkBuffer	staging_buf{};
				VmaAllocation staging_alloc{};

				VkBufferCreateInfo stg_info{};
				stg_info.sType	= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
				stg_info.size		= size;
				stg_info.usage	= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				stg_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

				VmaAllocationCreateInfo stg_ci{};
				stg_ci.usage = VMA_MEMORY_USAGE_CPU_ONLY;
				stg_ci.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;

				VmaAllocationInfo stg_info_out{};
				if (vmaCreateBuffer(vk_device->get_allocator(), &stg_info, &stg_ci,
									&staging_buf, &staging_alloc, &stg_info_out) != VK_SUCCESS)
					throw Exception("Failed to create Vulkan staging buffer via VMA");

				// pMappedData is valid because of VMA_ALLOCATION_CREATE_MAPPED_BIT.
				std::memcpy(stg_info_out.pMappedData, data, size);

				VkCommandBuffer cmd = vk_device->begin_single_time_commands();
				VkBufferCopy copy_region{ 0, 0, static_cast<VkDeviceSize>(size) };
				vkCmdCopyBuffer(cmd, staging_buf, buffer, 1, &copy_region);
				vk_device->end_single_time_commands(cmd);

				vmaDestroyBuffer(vk_device->get_allocator(), staging_buf, staging_alloc);
			}
			else
			{
				void *mapped = nullptr;
				if (vmaMapMemory(vk_device->get_allocator(), allocation, &mapped) != VK_SUCCESS)
					throw Exception("Failed to map Vulkan buffer memory for initial upload");
				std::memcpy(mapped, data, size);
				vmaUnmapMemory(vk_device->get_allocator(), allocation);
			}
		}
	}

	void VulkanBufferObjectProvider::lock(GraphicContext &gc, BufferAccess /*access*/)
	{
		throw_if_disposed();
		lock_gc = gc;
		if (vmaMapMemory(vk_device->get_allocator(), allocation, &mapped_ptr) != VK_SUCCESS)
			throw Exception("Failed to map Vulkan buffer memory for lock");
	}

	void VulkanBufferObjectProvider::unlock()
	{
		throw_if_disposed();
		vmaUnmapMemory(vk_device->get_allocator(), allocation);
		mapped_ptr = nullptr;
		lock_gc	= GraphicContext();
	}

	void *VulkanBufferObjectProvider::get_data()
	{
		if (!mapped_ptr)
			throw Exception("Buffer not locked – call lock() before get_data()");
		return mapped_ptr;
	}

	void VulkanBufferObjectProvider::upload_data(GraphicContext &gc, const void *data, int size)
	{
		upload_data(gc, 0, data, size);
	}

	void VulkanBufferObjectProvider::upload_data(GraphicContext &gc, int offset,
												const void *data, int size)
	{
		throw_if_disposed();

		bool host_visible = (memory_props & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0;

		if (host_visible)
		{
			void *mapped = nullptr;
			if (vmaMapMemory(vk_device->get_allocator(), allocation, &mapped) != VK_SUCCESS)
				throw Exception("Failed to map Vulkan buffer memory for upload_data");
			std::memcpy(static_cast<uint8_t *>(mapped) + offset, data, size);

			// If not HOST_COHERENT, flush the written range explicitly.
			if (!(memory_props & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT))
				vmaFlushAllocation(vk_device->get_allocator(), allocation, offset, size);

			vmaUnmapMemory(vk_device->get_allocator(), allocation);
		}
		else
		{
			// DEVICE_LOCAL: must go through a staging buffer + copy command + fence.
			VkBuffer	stg_buf{};
			VmaAllocation stg_alloc{};

			VkBufferCreateInfo stg_info{};
			stg_info.sType	= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			stg_info.size		= size;
			stg_info.usage	= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			stg_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

			VmaAllocationCreateInfo stg_ci{};
			stg_ci.usage = VMA_MEMORY_USAGE_CPU_ONLY;
			stg_ci.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;

			VmaAllocationInfo stg_alloc_info{};
			if (vmaCreateBuffer(vk_device->get_allocator(), &stg_info, &stg_ci,
								&stg_buf, &stg_alloc, &stg_alloc_info) != VK_SUCCESS)
				throw Exception("Failed to create upload_data staging buffer via VMA");

			std::memcpy(stg_alloc_info.pMappedData, data, size);

			VkCommandBuffer cmd = vk_device->begin_single_time_commands();
			VkBufferCopy copy_region{ 0, static_cast<VkDeviceSize>(offset),
									static_cast<VkDeviceSize>(size) };
			vkCmdCopyBuffer(cmd, stg_buf, buffer, 1, &copy_region);
			vk_device->end_single_time_commands(cmd);  // waits via vkQueueWaitIdle

			vmaDestroyBuffer(vk_device->get_allocator(), stg_buf, stg_alloc);
		}
	}

	void VulkanBufferObjectProvider::copy_from(GraphicContext &/*gc*/,
											VulkanBufferObjectProvider &src,
											int dest_pos, int src_pos, int size)
	{
		throw_if_disposed();
		VkCommandBuffer cmd = vk_device->begin_single_time_commands();
		VkBufferCopy copy_region{
			static_cast<VkDeviceSize>(src_pos),
			static_cast<VkDeviceSize>(dest_pos),
			static_cast<VkDeviceSize>(size)
		};
		vkCmdCopyBuffer(cmd, src.get_buffer(), buffer, 1, &copy_region);
		vk_device->end_single_time_commands(cmd);
	}

	void VulkanBufferObjectProvider::copy_to(GraphicContext &gc,
											VulkanBufferObjectProvider &dst,
											int dest_pos, int src_pos, int size)
	{
		dst.copy_from(gc, *this, dest_pos, src_pos, size);
	}
}
