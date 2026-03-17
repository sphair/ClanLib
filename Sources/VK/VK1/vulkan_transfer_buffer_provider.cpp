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
#include "VK/VK1/vulkan_transfer_buffer_provider.h"
#include "VK/vulkan_device.h"

namespace clan
{
	VulkanTransferBufferProvider::VulkanTransferBufferProvider()  {}
	VulkanTransferBufferProvider::~VulkanTransferBufferProvider() {}

	void VulkanTransferBufferProvider::create(int size, BufferUsage /*usage*/)
	{
		create(nullptr, size, BufferUsage::static_draw);
	}

	void VulkanTransferBufferProvider::create(void *data, int size, BufferUsage /*usage*/)
	{
		if (!vk_device)
			throw Exception("VulkanTransferBufferProvider: set_device() must be called before create()");

		buffer.create(vk_device, data, size,
					VK_BUFFER_USAGE_TRANSFER_SRC_BIT |
					VK_BUFFER_USAGE_TRANSFER_DST_BIT,
					VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
					VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	}
}
