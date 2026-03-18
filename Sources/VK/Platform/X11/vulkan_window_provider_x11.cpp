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
#include "VK/Platform/X11/vulkan_window_provider_x11.h"
#include "VK/VK1/vulkan_graphic_context_provider.h"
#include "VK/vulkan_device.h"
#include "API/Display/Window/display_window_description.h"
#include "API/Display/Window/cursor_description.h"
#include "Display/Platform/X11/cursor_provider_x11.h"
#include "Display/Platform/X11/display_message_queue_x11.h"

#include <algorithm>
#include <array>
#include <cstring>
#include <limits>

#ifdef HAVE_X11_EXTENSIONS_XRENDER_H
#include <X11/extensions/Xrender.h>
#endif

namespace clan
{

VulkanWindowProvider_X11::VulkanWindowProvider_X11(VulkanContextDescription &desc)
	: vk_desc(desc)
{
	x11_window.func_on_resized() = bind_member(this, &VulkanWindowProvider_X11::on_window_resized);
}

VulkanWindowProvider_X11::~VulkanWindowProvider_X11()
{
	if (!vk_device)
		return;

	vkDeviceWaitIdle(vk_device->get_device());

	if (!gc.is_null())
		if (auto *p = gc.get_provider()) p->dispose();

	cleanup_swapchain();

	if (surface != VK_NULL_HANDLE)
		vkDestroySurfaceKHR(vk_device->get_instance(), surface, nullptr);
}

void VulkanWindowProvider_X11::create(DisplayWindowSite *new_site,
									const DisplayWindowDescription &desc)
{
	site = new_site;

	x11_window.create(nullptr, site, desc);

	vk_device = std::make_unique<VulkanDevice>(vk_desc);

	create_surface();
	vk_device->init_present_queue(surface);

	int swap_interval = desc.get_swap_interval();
	current_swap_interval = (swap_interval == -1) ? 1 : swap_interval;

	create_swapchain(current_swap_interval);
	create_render_pass();
	create_depth_resources();
	create_framebuffers();
	create_command_buffers();
	create_sync_objects();

	gc = GraphicContext(new VulkanGraphicContextProvider(this));
}

void VulkanWindowProvider_X11::create_surface()
{
	VkXlibSurfaceCreateInfoKHR ci{};
	ci.sType  = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
	ci.dpy	= x11_window.get_display();
	ci.window = x11_window.get_window();

	if (vkCreateXlibSurfaceKHR(vk_device->get_instance(), &ci, nullptr, &surface) != VK_SUCCESS)
		throw Exception("Failed to create Vulkan Xlib surface");
}

VkPresentModeKHR VulkanWindowProvider_X11::choose_present_mode(
	const std::vector<VkPresentModeKHR> &modes, int swap_interval)
{
	if (swap_interval == 0)
		for (auto m : modes)
			if (m == VK_PRESENT_MODE_MAILBOX_KHR) return m;
	return VK_PRESENT_MODE_FIFO_KHR;
}

VkSurfaceFormatKHR VulkanWindowProvider_X11::choose_surface_format(
	const std::vector<VkSurfaceFormatKHR> &formats, bool want_alpha)
{
	if (want_alpha)
		for (const auto &f : formats)
			if (f.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR &&
				(f.format == VK_FORMAT_B8G8R8A8_UNORM ||
				f.format == VK_FORMAT_R8G8B8A8_UNORM))
				return f;

	for (const auto &f : formats)
		if (f.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR &&
			f.format == VK_FORMAT_B8G8R8A8_SRGB)
			return f;
	return formats[0];
}

void VulkanWindowProvider_X11::create_swapchain(int swap_interval)
{
	VkPhysicalDevice pd = vk_device->get_physical_device();

	VkSurfaceCapabilitiesKHR caps{};
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pd, surface, &caps);

	uint32_t fmt_count = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(pd, surface, &fmt_count, nullptr);
	std::vector<VkSurfaceFormatKHR> formats(fmt_count);
	vkGetPhysicalDeviceSurfaceFormatsKHR(pd, surface, &fmt_count, formats.data());

	uint32_t mode_count = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(pd, surface, &mode_count, nullptr);
	std::vector<VkPresentModeKHR> modes(mode_count);
	vkGetPhysicalDeviceSurfacePresentModesKHR(pd, surface, &mode_count, modes.data());

	bool want_alpha = false;
	VkSurfaceFormatKHR fmt = choose_surface_format(formats, want_alpha);
	VkPresentModeKHR   pm  = choose_present_mode(modes, swap_interval);

	VkExtent2D extent;
	if (caps.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		extent = caps.currentExtent;
	}
	else
	{
		Rect vp	= x11_window.get_viewport();
		extent.width  = std::clamp(static_cast<uint32_t>(vp.get_width()),
								caps.minImageExtent.width, caps.maxImageExtent.width);
		extent.height = std::clamp(static_cast<uint32_t>(vp.get_height()),
								caps.minImageExtent.height, caps.maxImageExtent.height);
	}

	uint32_t image_count = caps.minImageCount + 1;
	if (caps.maxImageCount > 0 && image_count > caps.maxImageCount)
		image_count = caps.maxImageCount;

	VkSwapchainCreateInfoKHR sci{};
	sci.sType			= VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	sci.surface		= surface;
	sci.minImageCount	= image_count;
	sci.imageFormat	= fmt.format;
	sci.imageColorSpace  = fmt.colorSpace;
	sci.imageExtent	= extent;
	sci.imageArrayLayers = 1;
	sci.imageUsage	= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

	uint32_t gfx_family	= vk_device->get_graphics_family();
	uint32_t present_family = vk_device->get_present_family();
	if (gfx_family != present_family)
	{
		uint32_t families[] = { gfx_family, present_family };
		sci.imageSharingMode	= VK_SHARING_MODE_CONCURRENT;
		sci.queueFamilyIndexCount = 2;
		sci.pQueueFamilyIndices   = families;
	}
	else
	{
		sci.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	sci.preTransform   = caps.currentTransform;
	sci.compositeAlpha = (caps.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR)
						&& want_alpha
						? VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR
						: VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	sci.presentMode  = pm;
	sci.clipped	= VK_TRUE;
	sci.oldSwapchain = swapchain;

	VkSwapchainKHR new_swapchain = VK_NULL_HANDLE;
	if (vkCreateSwapchainKHR(vk_device->get_device(), &sci, nullptr, &new_swapchain) != VK_SUCCESS)
		throw Exception("Failed to create Vulkan swapchain (X11)");

	if (swapchain != VK_NULL_HANDLE)
		vkDestroySwapchainKHR(vk_device->get_device(), swapchain, nullptr);

	swapchain			= new_swapchain;
	swapchain_image_format = fmt.format;
	swapchain_extent	= extent;
	current_swap_interval  = swap_interval;

	uint32_t sc_image_count = 0;
	vkGetSwapchainImagesKHR(vk_device->get_device(), swapchain, &sc_image_count, nullptr);
	swapchain_images.resize(sc_image_count);
	vkGetSwapchainImagesKHR(vk_device->get_device(), swapchain, &sc_image_count, swapchain_images.data());

	swapchain_image_views.resize(swapchain_images.size());
	for (size_t i = 0; i < swapchain_images.size(); i++)
	{
		VkImageViewCreateInfo ivci{};
		ivci.sType	= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		ivci.image	= swapchain_images[i];
		ivci.viewType = VK_IMAGE_VIEW_TYPE_2D;
		ivci.format   = swapchain_image_format;
		ivci.components = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY,
							VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
		ivci.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
		if (vkCreateImageView(vk_device->get_device(), &ivci, nullptr,
							&swapchain_image_views[i]) != VK_SUCCESS)
			throw Exception("Failed to create swapchain image view (X11)");
	}
}

void VulkanWindowProvider_X11::create_render_pass()
{
	VkFormat depth_format = vk_device->find_depth_format();

	std::array<VkAttachmentDescription, 2> attachments{};

	attachments[0].format		= swapchain_image_format;
	attachments[0].samples		= VK_SAMPLE_COUNT_1_BIT;
	attachments[0].loadOp		= VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[0].storeOp		= VK_ATTACHMENT_STORE_OP_STORE;
	attachments[0].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[0].initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
	attachments[0].finalLayout	= VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	attachments[1].format		= depth_format;
	attachments[1].samples		= VK_SAMPLE_COUNT_1_BIT;
	attachments[1].loadOp		= VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[1].storeOp		= VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[1].stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[1].initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
	attachments[1].finalLayout	= VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkAttachmentReference color_ref{ 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
	VkAttachmentReference depth_ref{ 1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint	= VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount	= 1;
	subpass.pColorAttachments	= &color_ref;
	subpass.pDepthStencilAttachment = &depth_ref;

	VkSubpassDependency dep{};
	dep.srcSubpass	= VK_SUBPASS_EXTERNAL;
	dep.dstSubpass	= 0;
	dep.srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
						VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dep.srcAccessMask = 0;
	dep.dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
						VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dep.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
						VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo rp_ci{};
	rp_ci.sType		= VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	rp_ci.attachmentCount = static_cast<uint32_t>(attachments.size());
	rp_ci.pAttachments	= attachments.data();
	rp_ci.subpassCount	= 1;
	rp_ci.pSubpasses	= &subpass;
	rp_ci.dependencyCount = 1;
	rp_ci.pDependencies   = &dep;

	if (vkCreateRenderPass(vk_device->get_device(), &rp_ci, nullptr, &render_pass) != VK_SUCCESS)
		throw Exception("Failed to create Vulkan render pass (X11)");
}

void VulkanWindowProvider_X11::create_depth_resources()
{
	VkFormat depth_format = vk_device->find_depth_format();

	VkImageCreateInfo ici{};
	ici.sType		= VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	ici.imageType	= VK_IMAGE_TYPE_2D;
	ici.format		= depth_format;
	ici.extent		= { swapchain_extent.width, swapchain_extent.height, 1 };
	ici.mipLevels	= 1;
	ici.arrayLayers   = 1;
	ici.samples	= VK_SAMPLE_COUNT_1_BIT;
	ici.tiling		= VK_IMAGE_TILING_OPTIMAL;
	ici.usage		= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	ici.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;
	ici.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	vkCreateImage(vk_device->get_device(), &ici, nullptr, &depth_image);

	VkMemoryRequirements mr{};
	vkGetImageMemoryRequirements(vk_device->get_device(), depth_image, &mr);

	VkMemoryAllocateInfo ai{};
	ai.sType		= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	ai.allocationSize  = mr.size;
	ai.memoryTypeIndex = vk_device->find_memory_type(mr.memoryTypeBits,
													VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
	vkAllocateMemory(vk_device->get_device(), &ai, nullptr, &depth_image_memory);
	vkBindImageMemory(vk_device->get_device(), depth_image, depth_image_memory, 0);

	VkImageViewCreateInfo ivci{};
	ivci.sType	= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	ivci.image	= depth_image;
	ivci.viewType = VK_IMAGE_VIEW_TYPE_2D;
	ivci.format   = depth_format;
	ivci.subresourceRange = { VK_IMAGE_ASPECT_DEPTH_BIT, 0, 1, 0, 1 };
	vkCreateImageView(vk_device->get_device(), &ivci, nullptr, &depth_image_view);
}

void VulkanWindowProvider_X11::create_framebuffers()
{
	swapchain_framebuffers.resize(swapchain_image_views.size());
	for (size_t i = 0; i < swapchain_image_views.size(); i++)
	{
		std::array<VkImageView, 2> views = { swapchain_image_views[i], depth_image_view };
		VkFramebufferCreateInfo fb_ci{};
		fb_ci.sType		= VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		fb_ci.renderPass	= render_pass;
		fb_ci.attachmentCount = static_cast<uint32_t>(views.size());
		fb_ci.pAttachments	= views.data();
		fb_ci.width		= swapchain_extent.width;
		fb_ci.height		= swapchain_extent.height;
		fb_ci.layers		= 1;
		if (vkCreateFramebuffer(vk_device->get_device(), &fb_ci, nullptr,
								&swapchain_framebuffers[i]) != VK_SUCCESS)
			throw Exception("Failed to create framebuffer (X11)");
	}
}

void VulkanWindowProvider_X11::create_command_buffers()
{
	command_buffers.resize(swapchain_framebuffers.size());
	VkCommandBufferAllocateInfo ai{};
	ai.sType			= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	ai.commandPool		= vk_device->get_command_pool();
	ai.level			= VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	ai.commandBufferCount = static_cast<uint32_t>(command_buffers.size());
	if (vkAllocateCommandBuffers(vk_device->get_device(), &ai,
								command_buffers.data()) != VK_SUCCESS)
		throw Exception("Failed to allocate command buffers (X11)");
}

void VulkanWindowProvider_X11::create_sync_objects()
{
	image_available_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
	render_finished_semaphores.resize(MAX_FRAMES_IN_FLIGHT);
	in_flight_fences.resize(MAX_FRAMES_IN_FLIGHT);
	images_in_flight.assign(swapchain_images.size(), VK_NULL_HANDLE);

	VkSemaphoreCreateInfo si{};
	si.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	VkFenceCreateInfo fi{};
	fi.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fi.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		if (vkCreateSemaphore(vk_device->get_device(), &si, nullptr,
							&image_available_semaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(vk_device->get_device(), &si, nullptr,
							&render_finished_semaphores[i]) != VK_SUCCESS ||
			vkCreateFence(vk_device->get_device(), &fi, nullptr,
						&in_flight_fences[i]) != VK_SUCCESS)
			throw Exception("Failed to create synchronisation objects (X11)");
	}
}

void VulkanWindowProvider_X11::cleanup_swapchain()
{
	VkDevice dev = vk_device->get_device();

	if (!command_buffers.empty())
	{
		vkFreeCommandBuffers(dev, vk_device->get_command_pool(),
							static_cast<uint32_t>(command_buffers.size()),
							command_buffers.data());
		command_buffers.clear();
	}

	if (depth_image_view  != VK_NULL_HANDLE) { vkDestroyImageView(dev, depth_image_view,  nullptr); depth_image_view  = VK_NULL_HANDLE; }
	if (depth_image	!= VK_NULL_HANDLE) { vkDestroyImage(dev, depth_image,		nullptr); depth_image	= VK_NULL_HANDLE; }
	if (depth_image_memory != VK_NULL_HANDLE){ vkFreeMemory(dev, depth_image_memory,	nullptr); depth_image_memory = VK_NULL_HANDLE; }

	for (auto fb : swapchain_framebuffers) vkDestroyFramebuffer(dev, fb, nullptr);
	swapchain_framebuffers.clear();

	for (auto iv : swapchain_image_views)  vkDestroyImageView(dev, iv, nullptr);
	swapchain_image_views.clear();

	if (render_pass != VK_NULL_HANDLE) { vkDestroyRenderPass(dev, render_pass, nullptr); render_pass = VK_NULL_HANDLE; }
	if (swapchain   != VK_NULL_HANDLE) { vkDestroySwapchainKHR(dev, swapchain, nullptr); swapchain   = VK_NULL_HANDLE; }

	// Guard against vectors being empty (called before create_sync_objects).
	for (size_t i = 0; i < image_available_semaphores.size(); i++)
		vkDestroySemaphore(dev, image_available_semaphores[i], nullptr);
	for (size_t i = 0; i < render_finished_semaphores.size(); i++)
		vkDestroySemaphore(dev, render_finished_semaphores[i], nullptr);
	for (size_t i = 0; i < in_flight_fences.size(); i++)
		vkDestroyFence(dev, in_flight_fences[i], nullptr);
	image_available_semaphores.clear();
	render_finished_semaphores.clear();
	in_flight_fences.clear();
	images_in_flight.clear();
}

void VulkanWindowProvider_X11::recreate_swapchain()
{
	vkDeviceWaitIdle(vk_device->get_device());

	frame_begun = false;

	cleanup_swapchain();

	create_swapchain(current_swap_interval);
	create_render_pass();
	create_depth_resources();
	create_framebuffers();
	create_command_buffers();
	create_sync_objects();

	// Do NOT reset current_frame here.  vkDeviceWaitIdle() above guarantees all
	// GPU work is complete, so the existing current_frame slot is valid to continue
	// from.  Resetting to 0 would desync the per-frame descriptor pool management
	// and the images_in_flight fence tracking relative to the Win32 path, which
	// never resets current_frame on swapchain recreation.
	framebuffer_resized = false;
	{
		auto *gc_provider = static_cast<VulkanGraphicContextProvider *>(gc.get_provider());
		if (gc_provider) gc_provider->on_window_resized();
	}
}

bool VulkanWindowProvider_X11::begin_frame()
{
	if (frame_begun) return true;

	vkWaitForFences(vk_device->get_device(), 1, &in_flight_fences[current_frame],
					VK_TRUE, UINT64_MAX);

	VkResult result = vkAcquireNextImageKHR(
		vk_device->get_device(), swapchain, UINT64_MAX,
		image_available_semaphores[current_frame], VK_NULL_HANDLE,
		&current_image_index);

	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{ recreate_swapchain(); return false; }
	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		throw Exception("vkAcquireNextImageKHR failed");

	if (images_in_flight[current_image_index] != VK_NULL_HANDLE)
		vkWaitForFences(vk_device->get_device(), 1,
						&images_in_flight[current_image_index], VK_TRUE, UINT64_MAX);
	images_in_flight[current_image_index] = in_flight_fences[current_frame];

	// Reset the per-frame descriptor pool now that the fence confirms the GPU has
	// finished with all command buffers (and descriptor sets) from this frame slot.
	if (!gc.is_null())
	{
		auto *gc_provider = static_cast<VulkanGraphicContextProvider *>(gc.get_provider());
		if (gc_provider) gc_provider->begin_frame_gc(static_cast<uint32_t>(current_frame));
	}

	VkCommandBuffer cmd = command_buffers[current_image_index];
	vkResetCommandBuffer(cmd, 0);
	VkCommandBufferBeginInfo bi{};
	bi.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	if (vkBeginCommandBuffer(cmd, &bi) != VK_SUCCESS)
		throw Exception("Failed to begin recording command buffer (X11)");

	frame_begun = true;
	return true;
}

void VulkanWindowProvider_X11::end_frame()
{
	if (!frame_begun)
	{
		if (!begin_frame())
			return;
	}

	if (!gc.is_null())
	{
		auto *gc_provider = static_cast<VulkanGraphicContextProvider *>(gc.get_provider());
		if (gc_provider)
			gc_provider->end_render_pass_if_active(command_buffers[current_image_index]);
	}

	VkCommandBuffer cmd = command_buffers[current_image_index];

	VkResult end_result = vkEndCommandBuffer(cmd);

	frame_begun = false;

	if (end_result != VK_SUCCESS)
		throw Exception("Failed to end Vulkan command buffer recording (X11, VkResult = " +
						std::to_string(static_cast<int>(end_result)) + ")");

	vkResetFences(vk_device->get_device(), 1, &in_flight_fences[current_frame]);

	VkPipelineStageFlags wait_stages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	VkSubmitInfo si{};
	si.sType				= VK_STRUCTURE_TYPE_SUBMIT_INFO;
	si.waitSemaphoreCount   = 1;
	si.pWaitSemaphores	= &image_available_semaphores[current_frame];
	si.pWaitDstStageMask	= &wait_stages;
	si.commandBufferCount   = 1;
	si.pCommandBuffers	= &cmd;
	si.signalSemaphoreCount = 1;
	si.pSignalSemaphores	= &render_finished_semaphores[current_frame];

	VkResult submit_result = vkQueueSubmit(vk_device->get_graphics_queue(), 1, &si,
										in_flight_fences[current_frame]);
	if (submit_result != VK_SUCCESS)
		throw Exception("vkQueueSubmit failed (X11, VkResult = " +
						std::to_string(static_cast<int>(submit_result)) + ")");

	VkPresentInfoKHR pi{};
	pi.sType			= VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	pi.waitSemaphoreCount = 1;
	pi.pWaitSemaphores	= &render_finished_semaphores[current_frame];
	pi.swapchainCount	= 1;
	pi.pSwapchains		= &swapchain;
	pi.pImageIndices	= &current_image_index;

	VkResult result = vkQueuePresentKHR(vk_device->get_present_queue(), &pi);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR
		|| framebuffer_resized)
		recreate_swapchain();
	else if (result != VK_SUCCESS)
		throw Exception("vkQueuePresentKHR failed (X11)");

	current_frame = (current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void VulkanWindowProvider_X11::flip(int interval)
{
	if (interval != -1 && interval != current_swap_interval)
	{
		current_swap_interval = interval;
		framebuffer_resized   = true;
	}
	end_frame();
}

VkFramebuffer VulkanWindowProvider_X11::get_current_framebuffer() const
{
	return swapchain_framebuffers[current_image_index];
}

VkCommandBuffer VulkanWindowProvider_X11::get_current_command_buffer() const
{
	return command_buffers[current_image_index];
}

void VulkanWindowProvider_X11::on_window_resized()
{
	framebuffer_resized = true;
	if (!gc.is_null())
	{
		auto *provider = dynamic_cast<VulkanGraphicContextProvider *>(gc.get_provider());
		if (provider) provider->on_window_resized();
	}
}

bool VulkanWindowProvider_X11::on_clicked(XButtonEvent &event)
{
	if (event.button != 1) return true;

	VkDevice	dev	= vk_device->get_device();
	VkDeviceSize   bytes  = 4;

	VkBuffer	stg_buf{}; VkDeviceMemory stg_mem{};
	{
		VkBufferCreateInfo ci{};
		ci.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		ci.size  = bytes;
		ci.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		vkCreateBuffer(dev, &ci, nullptr, &stg_buf);

		VkMemoryRequirements mr{};
		vkGetBufferMemoryRequirements(dev, stg_buf, &mr);
		VkMemoryAllocateInfo ai{};
		ai.sType		= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		ai.allocationSize  = mr.size;
		ai.memoryTypeIndex = vk_device->find_memory_type(
			mr.memoryTypeBits,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		vkAllocateMemory(dev, &ai, nullptr, &stg_mem);
		vkBindBufferMemory(dev, stg_buf, stg_mem, 0);
	}

	VkCommandBuffer one_shot = vk_device->begin_single_time_commands();
	VkImage sc_img = swapchain_images[current_image_index];

	auto barrier = [&](VkImageLayout from, VkImageLayout to,
					VkAccessFlags src, VkAccessFlags dst)
	{
		VkImageMemoryBarrier b{};
		b.sType			= VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		b.oldLayout		= from; b.newLayout = to;
		b.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		b.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		b.image			= sc_img;
		b.subresourceRange	= { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
		b.srcAccessMask	= src; b.dstAccessMask = dst;
		vkCmdPipelineBarrier(one_shot,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
			0, 0, nullptr, 0, nullptr, 1, &b);
	};

	barrier(VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			VK_ACCESS_MEMORY_READ_BIT, VK_ACCESS_TRANSFER_READ_BIT);

	VkBufferImageCopy region{};
	region.imageSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 };
	region.imageOffset	= { event.x, event.y, 0 };
	region.imageExtent	= { 1, 1, 1 };
	vkCmdCopyImageToBuffer(one_shot, sc_img,
						VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, stg_buf, 1, &region);

	barrier(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
			VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_MEMORY_READ_BIT);

	vk_device->end_single_time_commands(one_shot);

	void *mapped = nullptr;
	vkMapMemory(dev, stg_mem, 0, bytes, 0, &mapped);
	const uint32_t pixel = *static_cast<const uint32_t *>(mapped);
	vkUnmapMemory(dev, stg_mem);
	vkDestroyBuffer(dev, stg_buf, nullptr);
	vkFreeMemory(dev, stg_mem, nullptr);

	if ((pixel & 0xFF) < 10)
	{
		XLowerWindow(x11_window.get_display(), x11_window.get_window());
		return false;
	}
	return true;
}

CursorProvider *VulkanWindowProvider_X11::create_cursor(const CursorDescription &desc)
{
	return new CursorProvider_X11(desc, desc.get_hotspot());
}

void VulkanWindowProvider_X11::set_cursor(CursorProvider *cursor)
{
	x11_window.set_cursor(static_cast<CursorProvider_X11 *>(cursor));
}

void VulkanWindowProvider_X11::set_large_icon(const PixelBuffer &image)
{ x11_window.set_large_icon(image); }

void VulkanWindowProvider_X11::set_small_icon(const PixelBuffer &image)
{ x11_window.set_small_icon(image); }

void VulkanWindowProvider_X11::enable_alpha_channel(const Rect & /*blur_rect*/) {}
void VulkanWindowProvider_X11::extend_frame_into_client_area(int, int, int, int) {}

ProcAddress *VulkanWindowProvider_X11::get_proc_address(const std::string &fn) const
{
	return reinterpret_cast<ProcAddress *>(
		vkGetDeviceProcAddr(vk_device->get_device(), fn.c_str()));
}

} // namespace clan
