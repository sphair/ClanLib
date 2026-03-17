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
	create_image_views();
	create_render_pass();
	create_depth_resources();
	create_framebuffers();
	create_command_buffers();
	create_sync_objects();

	gc = GraphicContext(new VulkanGraphicContextProvider(this));
}

// ---- Platform-specific hook: create_surface ----

void VulkanWindowProvider_X11::create_surface()
{
	VkXlibSurfaceCreateInfoKHR ci{};
	ci.sType  = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
	ci.dpy	= x11_window.get_display();
	ci.window = x11_window.get_window();

	if (vkCreateXlibSurfaceKHR(vk_device->get_instance(), &ci, nullptr, &surface) != VK_SUCCESS)
		throw Exception("Failed to create Vulkan Xlib surface");
}

// ---- Platform-specific hook: create_swapchain ----

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
	if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pd, surface, &caps) != VK_SUCCESS)
		throw Exception("Failed to query Vulkan surface capabilities (X11)");

	uint32_t fmt_count = 0;
	if (vkGetPhysicalDeviceSurfaceFormatsKHR(pd, surface, &fmt_count, nullptr) != VK_SUCCESS)
		throw Exception("Failed to query Vulkan surface format count (X11)");
	std::vector<VkSurfaceFormatKHR> formats(fmt_count);
	if (vkGetPhysicalDeviceSurfaceFormatsKHR(pd, surface, &fmt_count, formats.data()) != VK_SUCCESS)
		throw Exception("Failed to query Vulkan surface formats (X11)");

	uint32_t mode_count = 0;
	if (vkGetPhysicalDeviceSurfacePresentModesKHR(pd, surface, &mode_count, nullptr) != VK_SUCCESS)
		throw Exception("Failed to query Vulkan present mode count (X11)");
	std::vector<VkPresentModeKHR> modes(mode_count);
	if (vkGetPhysicalDeviceSurfacePresentModesKHR(pd, surface, &mode_count, modes.data()) != VK_SUCCESS)
		throw Exception("Failed to query Vulkan present modes (X11)");

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
		Rect vp = x11_window.get_viewport();
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
	sci.presentMode	= pm;
	sci.clipped		= VK_TRUE;
	sci.oldSwapchain   = swapchain;

	VkSwapchainKHR new_swapchain = VK_NULL_HANDLE;
	if (vkCreateSwapchainKHR(vk_device->get_device(), &sci, nullptr, &new_swapchain) != VK_SUCCESS)
		throw Exception("Failed to create Vulkan swapchain (X11)");

	// The old swapchain is destroyed by cleanup_swapchain() called from
	// do_recreate_swapchain(); on the first call swapchain is VK_NULL_HANDLE.
	swapchain			= new_swapchain;
	swapchain_image_format = fmt.format;
	swapchain_extent	= extent;
	current_swap_interval  = swap_interval;

	uint32_t sc_image_count = 0;
	if (vkGetSwapchainImagesKHR(vk_device->get_device(), swapchain, &sc_image_count, nullptr) != VK_SUCCESS)
		throw Exception("Failed to query Vulkan swapchain image count (X11)");
	swapchain_images.resize(sc_image_count);
	if (vkGetSwapchainImagesKHR(vk_device->get_device(), swapchain, &sc_image_count, swapchain_images.data()) != VK_SUCCESS)
		throw Exception("Failed to retrieve Vulkan swapchain images (X11)");
}

// ---- begin_frame / end_frame / flip (delegate to base) ----

bool VulkanWindowProvider_X11::begin_frame()
{
	return do_begin_frame(gc);
}

void VulkanWindowProvider_X11::end_frame()
{
	do_end_frame(gc);
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

void VulkanWindowProvider_X11::on_window_resized()
{
	do_on_window_resized(gc);
}

bool VulkanWindowProvider_X11::on_clicked(XButtonEvent &event)
{
	if (event.button != 1)   // Left mouse button
		return true;
	return false;
}

// ---- VulkanWindowProviderBase passthrough ----

ProcAddress *VulkanWindowProvider_X11::get_proc_address(const std::string &fn) const
{
	return reinterpret_cast<ProcAddress *>(
		vkGetDeviceProcAddr(vk_device->get_device(), fn.c_str()));
}

// ---- DisplayWindowProvider pass-throughs ----

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

} // namespace clan
