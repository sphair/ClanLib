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
#include "API/Display/TargetProviders/vertex_array_buffer_provider.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Core/System/disposable_object.h"

namespace clan
{
	class VulkanDevice;

	/// \brief Vulkan buffer + VMA allocation
	class VulkanBufferObjectProvider : public DisposableObject
	{
	public:
		VulkanBufferObjectProvider();
		~VulkanBufferObjectProvider();

		/// Create the buffer.
		/// \param data		Initial data (may be nullptr).
		/// \param size		Size in bytes.
		/// \param usage_flags  VkBufferUsageFlags (e.g. VK_BUFFER_USAGE_VERTEX_BUFFER_BIT).
		/// \param memory_props Desired memory properties
		///					(HOST_VISIBLE|COHERENT for CPU-writable,
		///					DEVICE_LOCAL for GPU-only + staging upload).
		void create(VulkanDevice *device,
					const void *data, int size,
					VkBufferUsageFlags usage_flags,
					VkMemoryPropertyFlags memory_props);

		VkBuffer get_buffer() const { return buffer; }
		int	get_size()   const { return buffer_size; }

		/// Map the buffer for CPU access.  Requires HOST_VISIBLE memory.
		void  lock(GraphicContext &gc, BufferAccess access);
		void  unlock();
		void *get_data();

		/// Upload a sub-range.  For HOST_VISIBLE buffers this maps/memcpys/unmaps.
		/// For DEVICE_LOCAL buffers a staging buffer + vkCmdCopyBuffer is used.
		void upload_data(GraphicContext &gc, int offset, const void *data, int size);
		void upload_data(GraphicContext &gc, const void *data, int size);

		/// Copy between two Vulkan buffers via vkCmdCopyBuffer
		void copy_from(GraphicContext &gc, VulkanBufferObjectProvider &src,
					int dest_pos, int src_pos, int size);
		void copy_to(GraphicContext &gc, VulkanBufferObjectProvider &dst,
					int dest_pos, int src_pos, int size);

	private:
		void on_dispose() override;

		VulkanDevice	*vk_device	= nullptr;
		VkBuffer		buffer	= VK_NULL_HANDLE;
		VmaAllocation	allocation   = VK_NULL_HANDLE;
		int			buffer_size  = 0;

		void			*mapped_ptr   = nullptr;
		GraphicContext   lock_gc;

		VkBufferUsageFlags	usage_flags   = 0;
		VkMemoryPropertyFlags memory_props  = 0;
	};
}
