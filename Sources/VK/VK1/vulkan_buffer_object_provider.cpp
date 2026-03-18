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
			VkDevice dev = vk_device->get_device();
			vkDestroyBuffer(dev, buffer, nullptr);
			vkFreeMemory(dev, device_memory, nullptr);
			buffer		= VK_NULL_HANDLE;
			device_memory = VK_NULL_HANDLE;
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

		VkDevice dev = vk_device->get_device();

		// If data is provided for a DEVICE_LOCAL buffer we need an upload path
		// via a staging buffer.  For HOST_VISIBLE we can map directly.
		bool needs_staging = (mprops & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) &&
							!(mprops & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

		VkBufferCreateInfo buf_info{};
		buf_info.sType	= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		buf_info.size		= size;
		buf_info.usage	= uflags | (needs_staging ? VK_BUFFER_USAGE_TRANSFER_DST_BIT : 0);
		buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(dev, &buf_info, nullptr, &buffer) != VK_SUCCESS)
			throw Exception("Failed to create Vulkan buffer");

		VkMemoryRequirements mem_req{};
		vkGetBufferMemoryRequirements(dev, buffer, &mem_req);

		VkMemoryAllocateInfo alloc_info{};
		alloc_info.sType		= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		alloc_info.allocationSize  = mem_req.size;
		alloc_info.memoryTypeIndex = vk_device->find_memory_type(mem_req.memoryTypeBits, mprops);

		if (vkAllocateMemory(dev, &alloc_info, nullptr, &device_memory) != VK_SUCCESS)
			throw Exception("Failed to allocate Vulkan buffer memory");

		if (vkBindBufferMemory(dev, buffer, device_memory, 0) != VK_SUCCESS)
			throw Exception("Failed to bind Vulkan buffer memory");

		if (data && size > 0)
		{
			if (needs_staging)
			{
				// Upload via temporary HOST_VISIBLE staging buffer
				VkBuffer	staging_buf{};
				VkDeviceMemory staging_mem{};

				VkBufferCreateInfo stg_info{};
				stg_info.sType	= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
				stg_info.size		= size;
				stg_info.usage	= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
				stg_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
				if (vkCreateBuffer(dev, &stg_info, nullptr, &staging_buf) != VK_SUCCESS)
					throw Exception("Failed to create Vulkan buffer staging buffer");

				VkMemoryRequirements stg_req{};
				vkGetBufferMemoryRequirements(dev, staging_buf, &stg_req);

				VkMemoryAllocateInfo stg_alloc{};
				stg_alloc.sType		= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				stg_alloc.allocationSize  = stg_req.size;
				stg_alloc.memoryTypeIndex = vk_device->find_memory_type(
					stg_req.memoryTypeBits,
					VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
				if (vkAllocateMemory(dev, &stg_alloc, nullptr, &staging_mem) != VK_SUCCESS)
				{
					vkDestroyBuffer(dev, staging_buf, nullptr);
					throw Exception("Failed to allocate Vulkan buffer staging memory");
				}
				if (vkBindBufferMemory(dev, staging_buf, staging_mem, 0) != VK_SUCCESS)
				{
					vkFreeMemory(dev, staging_mem, nullptr);
					vkDestroyBuffer(dev, staging_buf, nullptr);
					throw Exception("Failed to bind Vulkan buffer staging memory");
				}

				void *mapped = nullptr;
				if (vkMapMemory(dev, staging_mem, 0, size, 0, &mapped) != VK_SUCCESS)
				{
					vkFreeMemory(dev, staging_mem, nullptr);
					vkDestroyBuffer(dev, staging_buf, nullptr);
					throw Exception("Failed to map Vulkan buffer staging memory");
				}
				std::memcpy(mapped, data, size);
				vkUnmapMemory(dev, staging_mem);

				VkCommandBuffer cmd = vk_device->begin_single_time_commands();
				VkBufferCopy copy_region{ 0, 0, static_cast<VkDeviceSize>(size) };
				vkCmdCopyBuffer(cmd, staging_buf, buffer, 1, &copy_region);
				vk_device->end_single_time_commands(cmd);

				vkDestroyBuffer(dev, staging_buf, nullptr);
				vkFreeMemory(dev, staging_mem, nullptr);
			}
			else
			{
				void *mapped = nullptr;
				if (vkMapMemory(dev, device_memory, 0, size, 0, &mapped) != VK_SUCCESS)
					throw Exception("Failed to map Vulkan buffer memory for upload");
				std::memcpy(mapped, data, size);
				vkUnmapMemory(dev, device_memory);
			}
		}
	}

	void VulkanBufferObjectProvider::lock(GraphicContext &gc, BufferAccess /*access*/)
	{
		throw_if_disposed();
		lock_gc = gc;
		if (vkMapMemory(vk_device->get_device(), device_memory, 0, buffer_size, 0, &mapped_ptr) != VK_SUCCESS)
			throw Exception("Failed to map Vulkan buffer memory for lock");
	}

	void VulkanBufferObjectProvider::unlock()
	{
		throw_if_disposed();
		vkUnmapMemory(vk_device->get_device(), device_memory);
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
			if (vkMapMemory(vk_device->get_device(), device_memory, offset, size, 0, &mapped) != VK_SUCCESS)
				throw Exception("Failed to map Vulkan buffer memory for upload_data");
			std::memcpy(mapped, data, size);

			// If memory is not HOST_COHERENT we must flush the range manually.
			if (!(memory_props & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT))
			{
				VkMappedMemoryRange flush_range{};
				flush_range.sType  = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
				flush_range.memory = device_memory;
				flush_range.offset = offset;
				flush_range.size   = size;
				if (vkFlushMappedMemoryRanges(vk_device->get_device(), 1, &flush_range) != VK_SUCCESS)
					throw Exception("Failed to flush Vulkan mapped memory range");
			}
			vkUnmapMemory(vk_device->get_device(), device_memory);
		}
		else
		{
			// DEVICE_LOCAL: must go through a staging buffer + copy command + fence.
			VkDevice dev = vk_device->get_device();

			VkBuffer	stg_buf{};
			VkDeviceMemory stg_mem{};

			VkBufferCreateInfo stg_info{};
			stg_info.sType	= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			stg_info.size		= size;
			stg_info.usage	= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			stg_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			if (vkCreateBuffer(dev, &stg_info, nullptr, &stg_buf) != VK_SUCCESS)
				throw Exception("Failed to create Vulkan upload_data staging buffer");

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
				throw Exception("Failed to allocate Vulkan upload_data staging memory");
			}
			if (vkBindBufferMemory(dev, stg_buf, stg_mem, 0) != VK_SUCCESS)
			{
				vkFreeMemory(dev, stg_mem, nullptr);
				vkDestroyBuffer(dev, stg_buf, nullptr);
				throw Exception("Failed to bind Vulkan upload_data staging memory");
			}

			void *mapped = nullptr;
			if (vkMapMemory(dev, stg_mem, 0, size, 0, &mapped) != VK_SUCCESS)
			{
				vkFreeMemory(dev, stg_mem, nullptr);
				vkDestroyBuffer(dev, stg_buf, nullptr);
				throw Exception("Failed to map Vulkan upload_data staging memory");
			}
			std::memcpy(mapped, data, size);
			vkUnmapMemory(dev, stg_mem);

			// Issue copy + wait (equivalent to the glFenceSync + glClientWaitSync pattern)
			VkCommandBuffer cmd = vk_device->begin_single_time_commands();
			VkBufferCopy copy_region{ 0, static_cast<VkDeviceSize>(offset), static_cast<VkDeviceSize>(size) };
			vkCmdCopyBuffer(cmd, stg_buf, buffer, 1, &copy_region);
			vk_device->end_single_time_commands(cmd);  // waits via vkQueueWaitIdle

			vkDestroyBuffer(dev, stg_buf, nullptr);
			vkFreeMemory(dev, stg_mem, nullptr);
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
