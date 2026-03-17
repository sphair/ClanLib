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
#include "VK/Platform/Win32/vulkan_window_provider.h"
#include "VK/VK1/vulkan_graphic_context_provider.h"
#include "VK/vulkan_device.h"
#include "API/VK/vulkan_context_description.h"
#include "API/Core/Math/rect.h"
#include "API/Display/Window/display_window_description.h"
#include "Display/Platform/Win32/cursor_provider_win32.h"
#include "Display/Platform/Win32/dwm_functions.h"
#include <algorithm>
#include <array>
#include <limits>
#include <commctrl.h>

namespace clan
{
	VulkanWindowProvider::VulkanWindowProvider(VulkanContextDescription &desc)
		: vk_desc(desc)
	{
		win32_window.func_on_resized() = bind_member(this, &VulkanWindowProvider::on_window_resized);
	}

	VulkanWindowProvider::~VulkanWindowProvider()
	{
		if (!gc.is_null())
		{
			GraphicContextProvider *provider = gc.get_provider();
			if (provider) provider->dispose();
		}

		if (vk_device)
		{
			vkDeviceWaitIdle(vk_device->get_device());
			cleanup_swapchain();

			if (surface != VK_NULL_HANDLE)
				vkDestroySurfaceKHR(vk_device->get_instance(), surface, nullptr);
		}

		if (shadow_window && shadow_hwnd)
			DestroyWindow(shadow_hwnd);
	}

	// ---- DisplayWindowProvider interface ----

	Rect		VulkanWindowProvider::get_geometry()	const { return win32_window.get_geometry(); }
	Rect		VulkanWindowProvider::get_viewport()	const { return win32_window.get_viewport(); }
	bool		VulkanWindowProvider::is_fullscreen()   const { return fullscreen; }
	bool		VulkanWindowProvider::has_focus()	const { return win32_window.has_focus(); }
	bool		VulkanWindowProvider::is_minimized()	const { return win32_window.is_minimized(); }
	bool		VulkanWindowProvider::is_maximized()	const { return win32_window.is_maximized(); }
	bool		VulkanWindowProvider::is_visible()	const { return win32_window.is_visible(); }
	std::string VulkanWindowProvider::get_title()	const { return win32_window.get_title(); }
	Size		VulkanWindowProvider::get_minimum_size(bool ca) const { return win32_window.get_minimum_size(ca); }
	Size		VulkanWindowProvider::get_maximum_size(bool ca) const { return win32_window.get_maximum_size(ca); }
	float	VulkanWindowProvider::get_pixel_ratio() const { return win32_window.get_pixel_ratio(); }
	bool		VulkanWindowProvider::is_clipboard_text_available()  const { return win32_window.is_clipboard_text_available(); }
	bool		VulkanWindowProvider::is_clipboard_image_available() const { return win32_window.is_clipboard_image_available(); }
	std::string VulkanWindowProvider::get_clipboard_text()  const { return win32_window.get_clipboard_text(); }
	PixelBuffer VulkanWindowProvider::get_clipboard_image() const { return win32_window.get_clipboard_image(); }

	Point VulkanWindowProvider::client_to_screen(const Point &c) { return win32_window.client_to_screen(c); }
	Point VulkanWindowProvider::screen_to_client(const Point &s) { return win32_window.screen_to_client(s); }

	// ---- create() ----
	void VulkanWindowProvider::create(DisplayWindowSite *new_site,
									const DisplayWindowDescription &desc)
	{
		site	= new_site;
		fullscreen = desc.is_fullscreen();

		win32_window.create(site, desc);

		if (desc.is_layered() && !DwmFunctions::is_composition_enabled())
		{
			WINDOWINFO wi{};
			wi.cbSize = sizeof(wi);
			GetWindowInfo(win32_window.get_hwnd(), &wi);
			if (wi.dwStyle & WS_VISIBLE) wi.dwStyle -= WS_VISIBLE;

			shadow_hwnd = CreateWindowEx(0, WC_STATIC, TEXT(""), wi.dwStyle,
				0, 0,
				wi.rcWindow.right  - wi.rcWindow.left,
				wi.rcWindow.bottom - wi.rcWindow.top,
				GetParent(win32_window.get_hwnd()), 0, GetModuleHandle(0), 0);
			if (!shadow_hwnd)
				throw Exception("Unable to create Vulkan shadow window");
			shadow_window = true;
		}

		vk_device = std::make_unique<VulkanDevice>(vk_desc);

		create_surface();
		vk_device->init_present_queue(surface);

		current_swap_interval = desc.get_swap_interval();
		create_swapchain(current_swap_interval);
		create_image_views();
		create_render_pass();
		create_depth_resources();
		create_framebuffers();
		create_command_buffers();
		create_sync_objects();

		gc = GraphicContext(new VulkanGraphicContextProvider(this));
	}

	void VulkanWindowProvider::create_surface()
	{
		HWND hwnd = shadow_window ? shadow_hwnd : win32_window.get_hwnd();

		VkWin32SurfaceCreateInfoKHR ci{};
		ci.sType	= VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		ci.hwnd	= hwnd;
		ci.hinstance = GetModuleHandle(nullptr);

		if (vkCreateWin32SurfaceKHR(vk_device->get_instance(), &ci, nullptr, &surface) != VK_SUCCESS)
			throw Exception("Failed to create Vulkan Win32 surface");
	}

	void VulkanWindowProvider::create_swapchain(int swap_interval)
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
		std::vector<VkPresentModeKHR> present_modes(mode_count);
		vkGetPhysicalDeviceSurfacePresentModesKHR(pd, surface, &mode_count, present_modes.data());

		VkSurfaceFormatKHR surface_format = choose_surface_format(formats);
		VkPresentModeKHR   present_mode   = choose_present_mode(present_modes, swap_interval);
		swapchain_extent	= choose_extent(caps);
		swapchain_image_format = surface_format.format;

		uint32_t image_count = caps.minImageCount + 1;
		if (caps.maxImageCount > 0 && image_count > caps.maxImageCount)
			image_count = caps.maxImageCount;

		VkSwapchainCreateInfoKHR ci{};
		ci.sType			= VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		ci.surface		= surface;
		ci.minImageCount	= image_count;
		ci.imageFormat	= surface_format.format;
		ci.imageColorSpace  = surface_format.colorSpace;
		ci.imageExtent	= swapchain_extent;
		ci.imageArrayLayers = 1;
		ci.imageUsage	= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		ci.preTransform	= caps.currentTransform;
		ci.compositeAlpha   = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		ci.presentMode	= present_mode;
		ci.clipped		= VK_TRUE;

		uint32_t queue_families[] = {
			vk_device->get_graphics_family(),
			vk_device->get_present_family()
		};
		if (queue_families[0] != queue_families[1])
		{
			ci.imageSharingMode	= VK_SHARING_MODE_CONCURRENT;
			ci.queueFamilyIndexCount = 2;
			ci.pQueueFamilyIndices   = queue_families;
		}
		else
		{
			ci.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		}

		if (vkCreateSwapchainKHR(vk_device->get_device(), &ci, nullptr, &swapchain) != VK_SUCCESS)
			throw Exception("Failed to create Vulkan swapchain");

		uint32_t sc_image_count = 0;
		vkGetSwapchainImagesKHR(vk_device->get_device(), swapchain, &sc_image_count, nullptr);
		swapchain_images.resize(sc_image_count);
		vkGetSwapchainImagesKHR(vk_device->get_device(), swapchain, &sc_image_count,
								swapchain_images.data());
	}

	void VulkanWindowProvider::create_image_views()
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

			if (vkCreateImageView(vk_device->get_device(), &ci, nullptr,
								&swapchain_image_views[i]) != VK_SUCCESS)
				throw Exception("Failed to create swapchain image views");
		}
	}

	void VulkanWindowProvider::create_render_pass()
	{
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
		depth_attachment.format		= vk_device->find_depth_format();
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

		if (vkCreateRenderPass(vk_device->get_device(), &rp_info, nullptr, &render_pass) != VK_SUCCESS)
			throw Exception("Failed to create Vulkan render pass");
	}

	void VulkanWindowProvider::create_depth_resources()
	{
		VkFormat depth_format = vk_device->find_depth_format();

		VkImageCreateInfo image_info{};
		image_info.sType		= VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		image_info.imageType	= VK_IMAGE_TYPE_2D;
		image_info.extent.width  = swapchain_extent.width;
		image_info.extent.height = swapchain_extent.height;
		image_info.extent.depth  = 1;
		image_info.mipLevels	= 1;
		image_info.arrayLayers   = 1;
		image_info.format		= depth_format;
		image_info.tiling		= VK_IMAGE_TILING_OPTIMAL;
		image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		image_info.usage		= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		image_info.samples	= VK_SAMPLE_COUNT_1_BIT;
		image_info.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateImage(vk_device->get_device(), &image_info, nullptr, &depth_image) != VK_SUCCESS)
			throw Exception("Failed to create depth image");

		VkMemoryRequirements mem_req{};
		vkGetImageMemoryRequirements(vk_device->get_device(), depth_image, &mem_req);

		VkMemoryAllocateInfo alloc_info{};
		alloc_info.sType		= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		alloc_info.allocationSize  = mem_req.size;
		alloc_info.memoryTypeIndex = vk_device->find_memory_type(
			mem_req.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		if (vkAllocateMemory(vk_device->get_device(), &alloc_info, nullptr, &depth_image_memory) != VK_SUCCESS)
			throw Exception("Failed to allocate depth image memory");

		vkBindImageMemory(vk_device->get_device(), depth_image, depth_image_memory, 0);

		VkImageViewCreateInfo view_info{};
		view_info.sType						= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		view_info.image						= depth_image;
		view_info.viewType						= VK_IMAGE_VIEW_TYPE_2D;
		view_info.format						= depth_format;
		view_info.subresourceRange.aspectMask	= VK_IMAGE_ASPECT_DEPTH_BIT;
		view_info.subresourceRange.baseMipLevel   = 0;
		view_info.subresourceRange.levelCount	= 1;
		view_info.subresourceRange.baseArrayLayer = 0;
		view_info.subresourceRange.layerCount	= 1;

		if (vkCreateImageView(vk_device->get_device(), &view_info, nullptr, &depth_image_view) != VK_SUCCESS)
			throw Exception("Failed to create depth image view");
	}

	void VulkanWindowProvider::create_framebuffers()
	{
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

			if (vkCreateFramebuffer(vk_device->get_device(), &fb_info, nullptr,
									&swapchain_framebuffers[i]) != VK_SUCCESS)
				throw Exception("Failed to create framebuffer");
		}
	}

	void VulkanWindowProvider::create_command_buffers()
	{
		command_buffers.resize(swapchain_framebuffers.size());

		VkCommandBufferAllocateInfo alloc_info{};
		alloc_info.sType			= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		alloc_info.commandPool		= vk_device->get_command_pool();
		alloc_info.level			= VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		alloc_info.commandBufferCount = static_cast<uint32_t>(command_buffers.size());

		if (vkAllocateCommandBuffers(vk_device->get_device(), &alloc_info,
									command_buffers.data()) != VK_SUCCESS)
			throw Exception("Failed to allocate command buffers");
	}

	void VulkanWindowProvider::create_sync_objects()
	{
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
			if (vkCreateSemaphore(vk_device->get_device(), &sem_info, nullptr,
								&image_available_semaphores[i]) != VK_SUCCESS ||
				vkCreateSemaphore(vk_device->get_device(), &sem_info, nullptr,
								&render_finished_semaphores[i]) != VK_SUCCESS ||
				vkCreateFence(vk_device->get_device(), &fence_info, nullptr,
							&in_flight_fences[i]) != VK_SUCCESS)
				throw Exception("Failed to create Vulkan sync objects");
		}
	}

	bool VulkanWindowProvider::begin_frame()
	{
		if (frame_begun) return true;

		vkWaitForFences(vk_device->get_device(), 1, &in_flight_fences[current_frame],
						VK_TRUE, UINT64_MAX);

		VkResult result = vkAcquireNextImageKHR(
			vk_device->get_device(), swapchain, UINT64_MAX,
			image_available_semaphores[current_frame], VK_NULL_HANDLE,
			&current_image_index);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			recreate_swapchain();
			return false;
		}
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			throw Exception("Failed to acquire swapchain image");

		if (images_in_flight[current_image_index] != VK_NULL_HANDLE)
			vkWaitForFences(vk_device->get_device(), 1,
							&images_in_flight[current_image_index], VK_TRUE, UINT64_MAX);
		images_in_flight[current_image_index] = in_flight_fences[current_frame];

		// Reset the per-frame descriptor pool now that the fence confirms the GPU has
		// finished with all command buffers (and descriptor sets) from this frame slot.
		if (!gc.is_null())
		{
			auto *gc_provider = static_cast<VulkanGraphicContextProvider *>(gc.get_provider());
			if (gc_provider) gc_provider->begin_frame_gc(current_frame);
		}

		VkCommandBufferBeginInfo begin_info{};
		begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		vkResetCommandBuffer(command_buffers[current_image_index], 0);
		if (vkBeginCommandBuffer(command_buffers[current_image_index], &begin_info) != VK_SUCCESS)
			throw Exception("Failed to begin recording command buffer");

		frame_begun = true;
		return true;
	}

	void VulkanWindowProvider::flip(int interval)
	{
		if (interval != -1 && interval != current_swap_interval)
		{
			current_swap_interval = interval;
			framebuffer_resized = true;
		}

		end_frame();
	}

	void VulkanWindowProvider::end_frame()
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

		VkResult end_result = vkEndCommandBuffer(command_buffers[current_image_index]);

		frame_begun = false;

		if (end_result != VK_SUCCESS)
			throw Exception("Failed to end Vulkan command buffer recording (VkResult = " +
							std::to_string(static_cast<int>(end_result)) + ")");

		vkResetFences(vk_device->get_device(), 1, &in_flight_fences[current_frame]);

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

		VkResult submit_result = vkQueueSubmit(vk_device->get_graphics_queue(), 1, &submit_info,
											in_flight_fences[current_frame]);
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

		VkResult result = vkQueuePresentKHR(vk_device->get_present_queue(), &present_info);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebuffer_resized)
		{
			framebuffer_resized = false;
			recreate_swapchain();
		}
		else if (result != VK_SUCCESS)
		{
			throw Exception("Failed to present Vulkan swapchain image");
		}

		current_frame = (current_frame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	VkFramebuffer VulkanWindowProvider::get_current_framebuffer() const
	{
		return swapchain_framebuffers[current_image_index];
	}

	VkCommandBuffer VulkanWindowProvider::get_current_command_buffer() const
	{
		return command_buffers[current_image_index];
	}

	void VulkanWindowProvider::on_window_resized()
	{
		framebuffer_resized = true;

		if (!gc.is_null())
		{
			auto *provider = static_cast<VulkanGraphicContextProvider *>(gc.get_provider());
			if (provider) provider->on_window_resized();
		}
	}

	void VulkanWindowProvider::recreate_swapchain()
	{
		vkDeviceWaitIdle(vk_device->get_device());

		frame_begun = false;

		cleanup_swapchain();

		create_swapchain(current_swap_interval);
		create_image_views();
		create_render_pass();
		create_depth_resources();
		create_framebuffers();
		create_command_buffers();
		create_sync_objects();

		if (!gc.is_null())
		{
			auto *gc_provider = static_cast<VulkanGraphicContextProvider *>(gc.get_provider());
			if (gc_provider) gc_provider->on_window_resized();
		}
	}

	void VulkanWindowProvider::cleanup_swapchain()
	{
		VkDevice dev = vk_device->get_device();

		if (depth_image_view   != VK_NULL_HANDLE) { vkDestroyImageView(dev, depth_image_view, nullptr);   depth_image_view = VK_NULL_HANDLE; }
		if (depth_image		!= VK_NULL_HANDLE) { vkDestroyImage(dev, depth_image, nullptr);			depth_image = VK_NULL_HANDLE; }
		if (depth_image_memory != VK_NULL_HANDLE) { vkFreeMemory(dev, depth_image_memory, nullptr);		depth_image_memory = VK_NULL_HANDLE; }

		for (auto &fb : swapchain_framebuffers) vkDestroyFramebuffer(dev, fb, nullptr);
		swapchain_framebuffers.clear();

		if (!command_buffers.empty())
		{
			vkFreeCommandBuffers(dev, vk_device->get_command_pool(),
								static_cast<uint32_t>(command_buffers.size()),
								command_buffers.data());
			command_buffers.clear();
		}

		if (render_pass != VK_NULL_HANDLE) { vkDestroyRenderPass(dev, render_pass, nullptr); render_pass = VK_NULL_HANDLE; }

		for (auto &iv : swapchain_image_views) vkDestroyImageView(dev, iv, nullptr);
		swapchain_image_views.clear();

		if (swapchain != VK_NULL_HANDLE) { vkDestroySwapchainKHR(dev, swapchain, nullptr); swapchain = VK_NULL_HANDLE; }

		// Guard against vectors being empty (called before create_sync_objects).
		for (size_t i = 0; i < image_available_semaphores.size(); i++)
		{
			if (image_available_semaphores[i] != VK_NULL_HANDLE) vkDestroySemaphore(dev, image_available_semaphores[i], nullptr);
			if (render_finished_semaphores[i] != VK_NULL_HANDLE) vkDestroySemaphore(dev, render_finished_semaphores[i], nullptr);
			if (in_flight_fences[i]		!= VK_NULL_HANDLE) vkDestroyFence(dev, in_flight_fences[i], nullptr);
		}
		image_available_semaphores.clear();
		render_finished_semaphores.clear();
		in_flight_fences.clear();
		images_in_flight.clear();
	}

	VkSurfaceFormatKHR VulkanWindowProvider::choose_surface_format(
		const std::vector<VkSurfaceFormatKHR> &formats) const
	{
		for (const auto &f : formats)
			if (f.format == VK_FORMAT_B8G8R8A8_SRGB &&
				f.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
				return f;
		return formats[0];
	}

	VkPresentModeKHR VulkanWindowProvider::choose_present_mode(
		const std::vector<VkPresentModeKHR> &modes, int interval) const
	{
		if (interval == 0)
		{
			for (auto m : modes)
				if (m == VK_PRESENT_MODE_IMMEDIATE_KHR) return m;
			for (auto m : modes)
				if (m == VK_PRESENT_MODE_MAILBOX_KHR)   return m;
		}
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D VulkanWindowProvider::choose_extent(
		const VkSurfaceCapabilitiesKHR &caps) const
	{
		if (caps.currentExtent.width != UINT32_MAX)
			return caps.currentExtent;

		RECT rc{};
		GetClientRect(win32_window.get_hwnd(), &rc);
		VkExtent2D extent = {
			static_cast<uint32_t>(rc.right  - rc.left),
			static_cast<uint32_t>(rc.bottom - rc.top)
		};
		extent.width  = std::clamp(extent.width,  caps.minImageExtent.width,  caps.maxImageExtent.width);
		extent.height = std::clamp(extent.height, caps.minImageExtent.height, caps.maxImageExtent.height);
		return extent;
	}

	ProcAddress *VulkanWindowProvider::get_proc_address(const std::string &fn) const
	{
		return reinterpret_cast<ProcAddress *>(
			vkGetDeviceProcAddr(vk_device->get_device(), fn.c_str()));
	}

	void VulkanWindowProvider::show_system_cursor()  { win32_window.show_system_cursor(); }
	void VulkanWindowProvider::hide_system_cursor()  { win32_window.hide_system_cursor(); }

	CursorProvider* VulkanWindowProvider::create_cursor(const CursorDescription& cursor_description)
	{
		return new CursorProvider_Win32(cursor_description);
	}

	void VulkanWindowProvider::set_cursor(CursorProvider* cursor)
	{
		win32_window.set_cursor(static_cast<CursorProvider_Win32*>(cursor));
	}

	void VulkanWindowProvider::set_cursor(StandardCursor t)  { win32_window.set_cursor(t); }
	void VulkanWindowProvider::set_cursor_handle(HCURSOR c)  { win32_window.set_cursor_handle(c); }
	void VulkanWindowProvider::set_title(const std::string &t) { win32_window.set_title(t); }
	void VulkanWindowProvider::set_position(const Rect &p, bool ca) { win32_window.set_position(p, ca); }
	void VulkanWindowProvider::set_size(int w, int h, bool ca)	{ win32_window.set_size(w, h, ca); }
	void VulkanWindowProvider::set_minimum_size(int w, int h, bool ca){ win32_window.set_minimum_size(w, h, ca); }
	void VulkanWindowProvider::set_maximum_size(int w, int h, bool ca){ win32_window.set_maximum_size(w, h, ca); }
	void VulkanWindowProvider::set_enabled(bool e) { win32_window.set_enabled(e); }
	void VulkanWindowProvider::minimize()		{ win32_window.minimize(); }
	void VulkanWindowProvider::restore()		{ win32_window.restore(); }
	void VulkanWindowProvider::maximize()		{ win32_window.maximize(); }
	void VulkanWindowProvider::toggle_fullscreen() { win32_window.toggle_fullscreen(); fullscreen = !fullscreen; }
	void VulkanWindowProvider::show(bool activate) { win32_window.show(activate); }
	void VulkanWindowProvider::hide()			{ win32_window.hide(); }
	void VulkanWindowProvider::bring_to_front()	{ win32_window.bring_to_front(); }
	void VulkanWindowProvider::set_pixel_ratio(float r) { win32_window.set_pixel_ratio(r); }
	void VulkanWindowProvider::capture_mouse(bool c)	{ win32_window.capture_mouse(c); }
	void VulkanWindowProvider::set_clipboard_text(const std::string &t)  { win32_window.set_clipboard_text(t); }
	void VulkanWindowProvider::set_clipboard_image(const PixelBuffer &b) { win32_window.set_clipboard_image(b); }
	void VulkanWindowProvider::request_repaint()   { win32_window.request_repaint(); }
	void VulkanWindowProvider::set_large_icon(const PixelBuffer &img) { win32_window.set_large_icon(img); }
	void VulkanWindowProvider::set_small_icon(const PixelBuffer &img) { win32_window.set_small_icon(img); }
	void VulkanWindowProvider::enable_alpha_channel(const Rect &r)	{ win32_window.enable_alpha_channel(r); }
	void VulkanWindowProvider::extend_frame_into_client_area(int l, int t, int r, int b) { win32_window.extend_frame_into_client_area(l, t, r, b); }
}
