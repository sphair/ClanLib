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
#include "VK/VK1/vulkan_occlusion_query_provider.h"
#include "VK/vulkan_device.h"

namespace clan
{
	VulkanOcclusionQueryProvider::VulkanOcclusionQueryProvider(VulkanDevice *device)
		: vk_device(device)
	{}

	VulkanOcclusionQueryProvider::~VulkanOcclusionQueryProvider()
	{
		destroy();
	}

	void VulkanOcclusionQueryProvider::create()
	{
		if (query_pool != VK_NULL_HANDLE)
			return; // already created

		VkQueryPoolCreateInfo pool_info{};
		pool_info.sType	= VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
		pool_info.queryType  = VK_QUERY_TYPE_OCCLUSION;
		pool_info.queryCount = POOL_SIZE;

		if (vkCreateQueryPool(vk_device->get_device(), &pool_info, nullptr, &query_pool) != VK_SUCCESS)
			throw Exception("Failed to create Vulkan occlusion query pool");

		// Reset the query so it is in a valid initial state.
		VkCommandBuffer cmd = vk_device->begin_single_time_commands();
		vkCmdResetQueryPool(cmd, query_pool, QUERY_INDEX, POOL_SIZE);
		vk_device->end_single_time_commands(cmd);
	}

	void VulkanOcclusionQueryProvider::destroy()
	{
		if (query_pool != VK_NULL_HANDLE)
		{
			vkDestroyQueryPool(vk_device->get_device(), query_pool, nullptr);
			query_pool = VK_NULL_HANDLE;
		}
	}

	void VulkanOcclusionQueryProvider::begin()
	{
		throw Exception("VulkanOcclusionQueryProvider::begin() — use begin(VkCommandBuffer) from VulkanGraphicContextProvider");
	}

	void VulkanOcclusionQueryProvider::end()
	{
		throw Exception("VulkanOcclusionQueryProvider::end() — use end(VkCommandBuffer) from VulkanGraphicContextProvider");
	}

	void VulkanOcclusionQueryProvider::begin(VkCommandBuffer cmd)
	{
		// Reset before use (must be outside a render pass in Vulkan 1.0,
		// but with VK_EXT_host_query_reset it can be done on the host).
		// For maximum compatibility we reset via a prior command.
		vkCmdResetQueryPool(cmd, query_pool, QUERY_INDEX, POOL_SIZE);
		vkCmdBeginQuery(cmd, query_pool, QUERY_INDEX, 0 /* no PRECISE flag by default */);
	}

	void VulkanOcclusionQueryProvider::end(VkCommandBuffer cmd)
	{
		vkCmdEndQuery(cmd, query_pool, QUERY_INDEX);
	}

	bool VulkanOcclusionQueryProvider::is_result_ready() const
	{
		// Pack result + availability into two uint64s.
		uint64_t data[2] = { 0, 0 };
		VkResult res = vkGetQueryPoolResults(
			vk_device->get_device(), query_pool,
			QUERY_INDEX, 1,
			sizeof(data), data,
			sizeof(uint64_t),
			VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WITH_AVAILABILITY_BIT);

		// data[1] == 0 means result is not yet available.
		return (res == VK_SUCCESS) && (data[1] != 0);
	}

	int VulkanOcclusionQueryProvider::get_result() const
	{
		uint64_t result = 0;
		VkResult res = vkGetQueryPoolResults(
			vk_device->get_device(), query_pool,
			QUERY_INDEX, 1,
			sizeof(result), &result,
			sizeof(uint64_t),
			VK_QUERY_RESULT_64_BIT | VK_QUERY_RESULT_WAIT_BIT); // blocking

		if (res != VK_SUCCESS)
			throw Exception("Failed to retrieve Vulkan occlusion query result");

		return static_cast<int>(result);
	}
}
