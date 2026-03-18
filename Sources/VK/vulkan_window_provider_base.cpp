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
#include "VK/vulkan_window_provider_base.h"
#include "VK/vulkan_device.h"
#include "VK/VK1/vulkan_graphic_context_provider.h"
#include "API/Display/Render/graphic_context.h"

#include <array>
#include <stdexcept>

namespace clan
{

// ---- create_image_views -------------------------------------------------------

void VulkanWindowProviderBase::create_image_views()
{
	swapchain_image_views.resize(swapchain_images.size());
	for (size_t i = 0; i < swapchain_images.size(); i++)
	{
		VkImageViewCreateInfo ci{};
		ci.sType						= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		ci.image						= swapchain_images[i];
		ci.viewType						= VK_IMAGE_VIEW_TYPE_2D;
		ci.format						= swapchain_image_format;
		ci.components					= { VK_COMPONENT_SWIZZLE_IDENTITY,
											VK_COMPONENT_SWIZZLE_IDENTITY,
											VK_COMPONENT_SWIZZLE_IDENTITY,
											VK_COMPONENT_SWIZZLE_IDENTITY };
		ci.subresourceRange.aspectMask	= VK_IMAGE_ASPECT_COLOR_BIT;
		ci.subresourceRange.baseMipLevel   = 0;
		ci.subresourceRange.levelCount	= 1;
		ci.subresourceRange.baseArrayLayer = 0;
		ci.subresourceRange.layerCount	= 1;

		if (vkCreateImageView(get_vulkan_device()->get_device(), &ci, nullptr,
							&swapchain_image_views[i]) != VK_SUCCESS)
			throw Exception("Failed to create swapchain image views");
	}
}

// ---- create_render_pass -------------------------------------------------------

void VulkanWindowProviderBase::create_render_pass()
{
	VulkanDevice *dev = get_vulkan_device();

	VkAttachmentDescription color_attachment{};
	color_attachment.format		= swapchain_image_format;
	color_attachment.samples		= VK_SAMPLE_COUNT_1_BIT;
	color_attachment.loadOp		= VK_ATTACHMENT_LOAD_OP_CLEAR;
	color_attachment.storeOp		= VK_ATTACHMENT_STORE_OP_STORE;
	color_attachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	color_attachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
	color_attachment.finalLayout	= VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentDescription depth_attachment{};
	depth_attachment.format		= dev->find_depth_format();
	depth_attachment.samples		= VK_SAMPLE_COUNT_1_BIT;
	depth_attachment.loadOp		= VK_ATTACHMENT_LOAD_OP_CLEAR;
	depth_attachment.storeOp		= VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depth_attachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depth_attachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
	depth_attachment.finalLayout	= VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference color_ref{};
	color_ref.attachment = 0;
	color_ref.layout	= VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depth_ref{};
	depth_ref.attachment = 1;
	depth_ref.layout	= VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint	= VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount	= 1;
	subpass.pColorAttachments	= &color_ref;
	subpass.pDepthStencilAttachment = &depth_ref;

	VkSubpassDependency dependency{};
	dependency.srcSubpass	= VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass	= 0;
	dependency.srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
							VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
							VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
							VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	std::array<VkAttachmentDescription, 2> attachments = { color_attachment, depth_attachment };

	VkRenderPassCreateInfo rp_info{};
	rp_info.sType		= VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	rp_info.attachmentCount = static_cast<uint32_t>(attachments.size());
	rp_info.pAttachments	= attachments.data();
	rp_info.subpassCount	= 1;
	rp_info.pSubpasses	= &subpass;
	rp_info.dependencyCount = 1;
	rp_info.pDependencies   = &dependency;

	if (vkCreateRenderPass(dev->get_device(), &rp_info, nullptr, &render_pass) != VK_SUCCESS)
		throw Exception("Failed to create Vulkan render pass");
}

// ---- create_depth_resources ---------------------------------------------------

void VulkanWindowProviderBase::create_depth_resources()
{
	VulkanDevice *dev		= get_vulkan_device();
	VkDevice	vk_dev	= dev->get_device();
	VkFormat	depth_fmt  = dev->find_depth_format();

	VkImageCreateInfo image_info{};
	image_info.sType		= VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	image_info.imageType	= VK_IMAGE_TYPE_2D;
	image_info.extent.width  = swapchain_extent.width;
	image_info.extent.height = swapchain_extent.height;
	image_info.extent.depth  = 1;
	image_info.mipLevels	= 1;
	image_info.arrayLayers   = 1;
	image_info.format		= depth_fmt;
	image_info.tiling		= VK_IMAGE_TILING_OPTIMAL;
	image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	image_info.usage		= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	image_info.samples	= VK_SAMPLE_COUNT_1_BIT;
	image_info.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateImage(vk_dev, &image_info, nullptr, &depth_image) != VK_SUCCESS)
		throw Exception("Failed to create depth image");

	VkMemoryRequirements mem_req{};
	vkGetImageMemoryRequirements(vk_dev, depth_image, &mem_req);

	VkMemoryAllocateInfo alloc_info{};
	alloc_info.sType		= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	alloc_info.allocationSize  = mem_req.size;
	alloc_info.memoryTypeIndex = dev->find_memory_type(
		mem_req.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	if (vkAllocateMemory(vk_dev, &alloc_info, nullptr, &depth_image_memory) != VK_SUCCESS)
		throw Exception("Failed to allocate depth image memory");

	if (vkBindImageMemory(vk_dev, depth_image, depth_image_memory, 0) != VK_SUCCESS)
		throw Exception("Failed to bind depth image memory");

	VkImageViewCreateInfo view_info{};
	view_info.sType						= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	view_info.image						= depth_image;
	view_info.viewType						= VK_IMAGE_VIEW_TYPE_2D;
	view_info.format						= depth_fmt;
	view_info.subresourceRange.aspectMask	= VK_IMAGE_ASPECT_DEPTH_BIT;
	view_info.subresourceRange.baseMipLevel   = 0;
	view_info.subresourceRange.levelCount	= 1;
	view_info.subresourceRange.baseArrayLayer = 0;
	view_info.subresourceRange.layerCount	= 1;

	if (vkCreateImageView(vk_dev, &view_info, nullptr, &depth_image_view) != VK_SUCCESS)
		throw Exception("Failed to create depth image view");
}

// ---- create_framebuffers ------------------------------------------------------

void VulkanWindowProviderBase::create_framebuffers()
{
	VkDevice vk_dev = get_vulkan_device()->get_device();

	swapchain_framebuffers.resize(swapchain_image_views.size());
	for (size_t i = 0; i < swapchain_image_views.size(); i++)
	{
		std::array<VkImageView, 2> attachments = {
			swapchain_image_views[i],
			depth_image_view
		};

		VkFramebufferCreateInfo fb_info{};
		fb_info.sType		= VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		fb_info.renderPass	= render_pass;
		fb_info.attachmentCount = static_cast<uint32_t>(attachments.size());
		fb_info.pAttachments	= attachments.data();
		fb_info.width		= swapchain_extent.width;
		fb_info.height		= swapchain_extent.height;
		fb_info.layers		= 1;

		if (vkCreateFramebuffer(vk_dev, &fb_info, nullptr,
								&swapchain_framebuffers[i]) != VK_SUCCESS)
			throw Exception("Failed to create framebuffer");
	}
}

// ---- create_command_buffers ---------------------------------------------------

void VulkanWindowProviderBase::create_command_buffers()
{
	VulkanDevice *dev = get_vulkan_device();

	command_buffers.resize(swapchain_framebuffers.size());

	VkCommandBufferAllocateInfo alloc_info{};
	alloc_info.sType			= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	alloc_info.commandPool		= dev->get_command_pool();
	alloc_info.level			= VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	alloc_info.commandBufferCount = static_cast<uint32_t>(command_buffers.size());

	if (vkAllocateCommandBuffers(dev->get_device(), &alloc_info,
								command_buffers.data()) != VK_SUCCESS)
		throw Exception("Failed to allocate command buffers");
}

// ---- create_sync_objects ------------------------------------------------------

void VulkanWindowProviderBase::create_sync_objects()
{
	VkDevice vk_dev = get_vulkan_device()->get_device();

	image_available_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
	render_finished_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
	in_flight_fences.resize(MAX_FRAMES_IN_FLIGHT);
	images_in_flight.assign(swapchain_images.size(), VK_NULL_HANDLE);

	VkSemaphoreCreateInfo sem_info{};
	sem_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fence_info{};
	fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		if (vkCreateSemaphore(vk_dev, &sem_info, nullptr,
							&image_available_semaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(vk_dev, &sem_info, nullptr,
							&render_finished_semaphores[i]) != VK_SUCCESS ||
			vkCreateFence(vk_dev, &fence_info, nullptr,
						&in_flight_fences[i]) != VK_SUCCESS)
			throw Exception("Failed to create Vulkan sync objects");
	}
}

// ---- cleanup_swapchain --------------------------------------------------------

void VulkanWindowProviderBase::cleanup_swapchain()
{
	VulkanDevice *dev	= get_vulkan_device();
	VkDevice	vk_dev = dev->get_device();

	if (depth_image_view   != VK_NULL_HANDLE) { vkDestroyImageView(vk_dev, depth_image_view,   nullptr); depth_image_view   = VK_NULL_HANDLE; }
	if (depth_image		!= VK_NULL_HANDLE) { vkDestroyImage	(vk_dev, depth_image,		nullptr); depth_image		= VK_NULL_HANDLE; }
	if (depth_image_memory != VK_NULL_HANDLE) { vkFreeMemory	(vk_dev, depth_image_memory,  nullptr); depth_image_memory = VK_NULL_HANDLE; }

	for (auto &fb : swapchain_framebuffers) vkDestroyFramebuffer(vk_dev, fb, nullptr);
	swapchain_framebuffers.clear();

	if (!command_buffers.empty())
	{
		vkFreeCommandBuffers(vk_dev, dev->get_command_pool(),
							static_cast<uint32_t>(command_buffers.size()),
							command_buffers.data());
		command_buffers.clear();
	}

	if (render_pass != VK_NULL_HANDLE) { vkDestroyRenderPass(vk_dev, render_pass, nullptr); render_pass = VK_NULL_HANDLE; }

	for (auto &iv : swapchain_image_views) vkDestroyImageView(vk_dev, iv, nullptr);
	swapchain_image_views.clear();

	if (swapchain != VK_NULL_HANDLE) { vkDestroySwapchainKHR(vk_dev, swapchain, nullptr); swapchain = VK_NULL_HANDLE; }

	// Guard: vectors may be empty if called before create_sync_objects.
	for (size_t i = 0; i < image_available_semaphores.size(); i++)
	{
		if (image_available_semaphores[i] != VK_NULL_HANDLE) vkDestroySemaphore(vk_dev, image_available_semaphores[i], nullptr);
		if (render_finished_semaphores[i] != VK_NULL_HANDLE) vkDestroySemaphore(vk_dev, render_finished_semaphores[i], nullptr);
		if (in_flight_fences[i]		!= VK_NULL_HANDLE) vkDestroyFence	(vk_dev, in_flight_fences[i],		nullptr);
	}
	image_available_semaphores.clear();
	render_finished_semaphores.clear();
	in_flight_fences.clear();
	images_in_flight.clear();
}

// ---- do_begin_frame -----------------------------------------------------------

bool VulkanWindowProviderBase::do_begin_frame(GraphicContext &gc)
{
	if (frame_begun) return true;

	VkDevice vk_dev = get_vulkan_device()->get_device();

	if (vkWaitForFences(vk_dev, 1, &in_flight_fences[current_frame], VK_TRUE, UINT64_MAX) != VK_SUCCESS)
		throw Exception("Failed to wait for Vulkan in-flight fence");

	VkResult result = vkAcquireNextImageKHR(
		vk_dev, swapchain, UINT64_MAX,
		image_available_semaphores[current_frame], VK_NULL_HANDLE,
		&current_image_index);

	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		do_recreate_swapchain(gc);
		return false;
	}
	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		throw Exception("Failed to acquire swapchain image");

	if (images_in_flight[current_image_index] != VK_NULL_HANDLE)
		if (vkWaitForFences(vk_dev, 1, &images_in_flight[current_image_index], VK_TRUE, UINT64_MAX) != VK_SUCCESS)
			throw Exception("Failed to wait for Vulkan image-in-flight fence");
	images_in_flight[current_image_index] = in_flight_fences[current_frame];

	// Reset the per-frame descriptor pool now that the fence confirms the GPU
	// has finished with all command buffers (and descriptor sets) from this frame slot.
	if (!gc.is_null())
	{
		auto *gc_provider = static_cast<VulkanGraphicContextProvider *>(gc.get_provider());
		if (gc_provider) gc_provider->begin_frame_gc(current_frame);
	}

	VkCommandBufferBeginInfo begin_info{};
	begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	if (vkResetCommandBuffer(command_buffers[current_image_index], 0) != VK_SUCCESS)
		throw Exception("Failed to reset Vulkan command buffer");
	if (vkBeginCommandBuffer(command_buffers[current_image_index], &begin_info) != VK_SUCCESS)
		throw Exception("Failed to begin recording command buffer");

	frame_begun = true;
	return true;
}

// ---- do_end_frame -------------------------------------------------------------

void VulkanWindowProviderBase::do_end_frame(GraphicContext &gc)
{
	if (!frame_begun)
	{
		if (!do_begin_frame(gc))
			return;
	}

	if (!gc.is_null())
	{
		auto *gc_provider = static_cast<VulkanGraphicContextProvider *>(gc.get_provider());
		if (gc_provider)
			gc_provider->end_render_pass_if_active(command_buffers[current_image_index]);
	}

	VkResult end_result = vkEndCommandBuffer(command_buffers[current_image_index]);

	frame_begun = false;

	if (end_result != VK_SUCCESS)
		throw Exception("Failed to end Vulkan command buffer recording (VkResult = " +
						std::to_string(static_cast<int>(end_result)) + ")");

	VkDevice vk_dev = get_vulkan_device()->get_device();
	if (vkResetFences(vk_dev, 1, &in_flight_fences[current_frame]) != VK_SUCCESS)
		throw Exception("Failed to reset Vulkan in-flight fence");

	VkPipelineStageFlags wait_stages[] = {
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
	};

	VkSubmitInfo submit_info{};
	submit_info.sType				= VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.waitSemaphoreCount   = 1;
	submit_info.pWaitSemaphores	= &image_available_semaphores[current_frame];
	submit_info.pWaitDstStageMask	= wait_stages;
	submit_info.commandBufferCount   = 1;
	submit_info.pCommandBuffers	= &command_buffers[current_image_index];
	submit_info.signalSemaphoreCount = 1;
	submit_info.pSignalSemaphores	= &render_finished_semaphores[current_frame];

	VkResult submit_result = vkQueueSubmit(get_vulkan_device()->get_graphics_queue(), 1,
										&submit_info, in_flight_fences[current_frame]);
	if (submit_result != VK_SUCCESS)
		throw Exception("Failed to submit Vulkan draw command buffer (VkResult = " +
						std::to_string(static_cast<int>(submit_result)) + ")");

	VkPresentInfoKHR present_info{};
	present_info.sType			= VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	present_info.waitSemaphoreCount = 1;
	present_info.pWaitSemaphores	= &render_finished_semaphores[current_frame];
	present_info.swapchainCount	= 1;
	present_info.pSwapchains		= &swapchain;
	present_info.pImageIndices	= &current_image_index;

	VkResult result = vkQueuePresentKHR(get_vulkan_device()->get_present_queue(), &present_info);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebuffer_resized)
	{
		framebuffer_resized = false;
		do_recreate_swapchain(gc);
	}
	else if (result != VK_SUCCESS)
	{
		throw Exception("Failed to present Vulkan swapchain image");
	}

	current_frame = (current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
}

// ---- do_on_window_resized -----------------------------------------------------

void VulkanWindowProviderBase::do_on_window_resized(GraphicContext &gc)
{
	framebuffer_resized = true;
	if (!gc.is_null())
	{
		auto *provider = static_cast<VulkanGraphicContextProvider *>(gc.get_provider());
		if (provider) provider->on_window_resized();
	}
}

// ---- do_recreate_swapchain ----------------------------------------------------

void VulkanWindowProviderBase::do_recreate_swapchain(GraphicContext &gc)
{
	vkDeviceWaitIdle(get_vulkan_device()->get_device());

	frame_begun = false;

	cleanup_swapchain();

	create_swapchain(current_swap_interval);
	create_image_views();
	create_render_pass();
	create_depth_resources();
	create_framebuffers();
	create_command_buffers();
	create_sync_objects();

	framebuffer_resized = false;

	if (!gc.is_null())
	{
		auto *gc_provider = static_cast<VulkanGraphicContextProvider *>(gc.get_provider());
		if (gc_provider) gc_provider->on_window_resized();
	}
}

} // namespace clan
