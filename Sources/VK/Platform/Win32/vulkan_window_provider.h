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

#include "API/Display/TargetProviders/display_window_provider.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Window/input_device.h"
#include "Display/Platform/Win32/win32_window.h"
#include "API/VK/vulkan_context_description.h"
#include "VK/vulkan_window_provider_base.h"
// volk.h sets VK_USE_PLATFORM_WIN32_KHR and includes vulkan_win32.h automatically.
#include "API/VK/volk.h"
#include <vector>
#include <memory>

namespace clan
{
	class VulkanDevice;
	class VulkanGraphicContextProvider;

	class VulkanWindowProvider : public DisplayWindowProvider, public VulkanWindowProviderBase
	{
	public:
		explicit VulkanWindowProvider(VulkanContextDescription &desc);
		~VulkanWindowProvider() override;

		// ---- DisplayWindowProvider interface ----
		Rect		get_geometry()	const override;
		Rect		get_viewport()	const override;
		bool		is_fullscreen()   const override;
		bool		has_focus()	const override;
		bool		is_minimized()	const override;
		bool		is_maximized()	const override;
		bool		is_visible()	const override;
		std::string  get_title()	const override;
		Size		get_minimum_size(bool client_area) const override;
		Size		get_maximum_size(bool client_area) const override;
		float		get_pixel_ratio() const override;

		DisplayWindowHandle get_handle() const override
		{
			DisplayWindowHandle h;
			h.hwnd = win32_window.get_hwnd();
			return h;
		}

		GraphicContext   &get_gc()		override { return gc; }
		InputDevice	&get_keyboard()		override { return win32_window.get_keyboard(); }
		InputDevice	&get_mouse()		override { return win32_window.get_mouse(); }
		std::vector<InputDevice> &get_game_controllers() override { return win32_window.get_game_controllers(); }

		bool		is_clipboard_text_available()  const override;
		bool		is_clipboard_image_available() const override;
		std::string get_clipboard_text()		const override;
		PixelBuffer get_clipboard_image()		const override;

		Point client_to_screen(const Point &client) override;
		Point screen_to_client(const Point &screen) override;
		void  create(DisplayWindowSite *site, const DisplayWindowDescription &description) override;

		void show_system_cursor()  override;
		void hide_system_cursor()  override;
		CursorProvider *create_cursor(const CursorDescription &cursor_description) override;
		void set_cursor(CursorProvider *cursor) override;
		void set_cursor(StandardCursor type)	override;
		void set_cursor_handle(HCURSOR cursor)  override;
		void set_title(const std::string &new_title) override;
		void set_position(const Rect &pos, bool client_area) override;
		void set_size(int width, int height, bool client_area) override;
		void set_minimum_size(int width, int height, bool client_area) override;
		void set_maximum_size(int width, int height, bool client_area) override;
		void set_enabled(bool enable) override;
		void minimize()	override;
		void restore()		override;
		void maximize()	override;
		void toggle_fullscreen() override;
		void show(bool activate) override;
		void hide()		override;
		void bring_to_front() override;
		void set_pixel_ratio(float ratio) override;

		void flip(int interval) override;

		void capture_mouse(bool capture) override;
		void set_clipboard_text(const std::string &text) override;
		void set_clipboard_image(const PixelBuffer &buf) override;
		void request_repaint() override;
		void set_large_icon(const PixelBuffer &image) override;
		void set_small_icon(const PixelBuffer &image) override;
		void enable_alpha_channel(const Rect &blur_rect) override;
		void extend_frame_into_client_area(int left, int top, int right, int bottom) override;

		// ---- VulkanWindowProviderBase interface overrides ----
		VulkanDevice	*get_vulkan_device()		const override { return vk_device.get(); }
		VkRenderPass		get_render_pass()		const override { return render_pass; }
		VkFramebuffer	get_current_framebuffer()	const override { return swapchain_framebuffers[current_image_index]; }
		VkCommandBuffer	get_current_command_buffer()	const override { return command_buffers[current_image_index]; }
		uint32_t			get_current_image_index()	const override { return current_image_index; }
		VkImage			get_swapchain_image(uint32_t i) const override { return swapchain_images[i]; }
		VkExtent2D		get_swapchain_extent()		const override { return swapchain_extent; }
		VkFormat			get_swapchain_format()		const override { return swapchain_image_format; }
		ProcAddress		*get_proc_address(const std::string &fn) const override;

		uint32_t			get_current_frame()  const override { return current_frame; }
		bool				begin_frame()	override;
		bool				is_frame_begun()	const override { return frame_begun; }

		VkSurfaceKHR		get_surface()  const { return surface; }
		VkSwapchainKHR	get_swapchain() const { return swapchain; }

		void end_frame();

	private:
		// ---- Platform-specific hook implementations -------------------------
		void create_surface()					override;
		void create_swapchain(int swap_interval) override;

		void on_window_resized();

		VkSurfaceFormatKHR choose_surface_format(const std::vector<VkSurfaceFormatKHR> &formats) const;
		VkPresentModeKHR   choose_present_mode(const std::vector<VkPresentModeKHR> &modes,
											int interval) const;
		VkExtent2D		choose_extent(const VkSurfaceCapabilitiesKHR &capabilities) const;

		Win32Window	win32_window;
		GraphicContext  gc;
		DisplayWindowSite *site	= nullptr;
		bool		fullscreen   = false;
		HWND		shadow_hwnd  = nullptr;
		bool		shadow_window = false;

		std::unique_ptr<VulkanDevice> vk_device;

		VulkanContextDescription vk_desc;
	};
}
