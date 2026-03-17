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

#include "API/VK/volk.h"
#include "API/Display/TargetProviders/occlusion_query_provider.h"

namespace clan
{
	class VulkanDevice;

	class VulkanOcclusionQueryProvider : public OcclusionQueryProvider
	{
	public:
		explicit VulkanOcclusionQueryProvider(VulkanDevice *device);
		~VulkanOcclusionQueryProvider() override;

		// ---- OcclusionQueryProvider interface ----
		void create() override;

		/// Begin occlusion query. No-arg override satisfies base; throws without an
		/// active command buffer. Use begin(VkCommandBuffer) from the GC provider.
		void begin() override;

		/// End occlusion query. Same note as begin().
		void end() override;

		/// Returns true if the GPU result is available without blocking.
		bool is_result_ready() const override;

		/// Returns the number of samples that passed. Blocks until result is ready.
		int get_result() const override;

		// ---- Vulkan-specific: called by VulkanGraphicContextProvider ----

		/// Destroy and release the VkQueryPool. Not part of the base interface.
		void destroy();

		/// Begin the query on an explicit command buffer (inside an active render pass).
		void begin(VkCommandBuffer cmd);

		/// End the query on an explicit command buffer.
		void end(VkCommandBuffer cmd);

	private:
		VulkanDevice *vk_device	= nullptr;
		VkQueryPool   query_pool   = VK_NULL_HANDLE;

		/// Index within the pool (we use one pool per query, index 0).
		static constexpr uint32_t QUERY_INDEX = 0;
		static constexpr uint32_t POOL_SIZE   = 1;
	};
}
