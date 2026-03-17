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

	// ---- Platform-specific hook: create_surface ----
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

	// ---- Platform-specific hook: create_swapchain ----
	void VulkanWindowProvider::create_swapchain(int swap_interval)
	{
		VkPhysicalDevice pd = vk_device->get_physical_device();

		VkSurfaceCapabilitiesKHR caps{};
		if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pd, surface, &caps) != VK_SUCCESS)
			throw Exception("Failed to query Vulkan surface capabilities");

		uint32_t fmt_count = 0;
		if (vkGetPhysicalDeviceSurfaceFormatsKHR(pd, surface, &fmt_count, nullptr) != VK_SUCCESS)
			throw Exception("Failed to query Vulkan surface format count");
		std::vector<VkSurfaceFormatKHR> formats(fmt_count);
		if (vkGetPhysicalDeviceSurfaceFormatsKHR(pd, surface, &fmt_count, formats.data()) != VK_SUCCESS)
			throw Exception("Failed to query Vulkan surface formats");

		uint32_t mode_count = 0;
		if (vkGetPhysicalDeviceSurfacePresentModesKHR(pd, surface, &mode_count, nullptr) != VK_SUCCESS)
			throw Exception("Failed to query Vulkan present mode count");
		std::vector<VkPresentModeKHR> present_modes(mode_count);
		if (vkGetPhysicalDeviceSurfacePresentModesKHR(pd, surface, &mode_count, present_modes.data()) != VK_SUCCESS)
			throw Exception("Failed to query Vulkan present modes");

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
		if (vkGetSwapchainImagesKHR(vk_device->get_device(), swapchain, &sc_image_count, nullptr) != VK_SUCCESS)
			throw Exception("Failed to query Vulkan swapchain image count");
		swapchain_images.resize(sc_image_count);
		if (vkGetSwapchainImagesKHR(vk_device->get_device(), swapchain, &sc_image_count,
								swapchain_images.data()) != VK_SUCCESS)
			throw Exception("Failed to retrieve Vulkan swapchain images");
	}

	// ---- begin_frame / end_frame / flip (delegate to base) ----

	bool VulkanWindowProvider::begin_frame()
	{
		return do_begin_frame(gc);
	}

	void VulkanWindowProvider::end_frame()
	{
		do_end_frame(gc);
	}

	void VulkanWindowProvider::flip(int interval)
	{
		if (interval != -1 && interval != current_swap_interval)
		{
			current_swap_interval = interval;
			framebuffer_resized   = true;
		}
		end_frame();
	}

	void VulkanWindowProvider::on_window_resized()
	{
		do_on_window_resized(gc);
	}

	// ---- Format / mode / extent helpers (Win32-specific) ----

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

	// ---- Remaining DisplayWindowProvider pass-throughs ----

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
