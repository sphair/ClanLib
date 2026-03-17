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

// volk.h sets VK_USE_PLATFORM_XLIB_KHR and includes vulkan_xlib.h automatically.
#include "API/VK/volk.h"

#include <memory>
#include <vector>

#include "API/Display/TargetProviders/display_window_provider.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/Window/input_device.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/VK/vulkan_context_description.h"
#include "VK/vulkan_window_provider_base.h"
#include "Display/Platform/X11/x11_window.h"

#include <X11/Xlib.h>

namespace clan
{
	class VulkanDevice;
	class VulkanGraphicContextProvider;

	class VulkanWindowProvider_X11 final : public DisplayWindowProvider, public VulkanWindowProviderBase
	{
	public:
		explicit VulkanWindowProvider_X11(VulkanContextDescription &vk_desc);
		~VulkanWindowProvider_X11() override;

		// ---- DisplayWindowProvider interface --------------------------------
		Rect  get_geometry()  const override { return x11_window.get_geometry(); }
		Rect  get_viewport()  const override { return x11_window.get_viewport(); }
		float get_pixel_ratio() const override { return x11_window.get_pixel_ratio(); }

		bool has_focus()	const override { return x11_window.has_focus(); }
		bool is_fullscreen() const override { return x11_window.is_fullscreen(); }
		bool is_minimized()  const override { return x11_window.is_minimized(); }
		bool is_maximized()  const override { return x11_window.is_maximized(); }
		bool is_visible()	const override { return x11_window.is_visible(); }
		bool is_clipboard_text_available()  const override { return x11_window.is_clipboard_text_available(); }
		bool is_clipboard_image_available() const override { return x11_window.is_clipboard_image_available(); }

		std::string  get_title()	const override { return x11_window.get_title(); }
		Size get_minimum_size(bool ca) const override { return x11_window.get_minimum_size(ca); }
		Size get_maximum_size(bool ca) const override { return x11_window.get_maximum_size(ca); }

		std::string  get_clipboard_text()  const override { return x11_window.get_clipboard_text(); }
		PixelBuffer  get_clipboard_image() const override { return x11_window.get_clipboard_image(); }
		DisplayWindowHandle get_handle()   const override { return x11_window.get_handle(); }

		GraphicContext &get_gc() override { return gc; }
		InputDevice	&get_keyboard()	override { return x11_window.get_keyboard(); }
		InputDevice	&get_mouse()		override { return x11_window.get_mouse(); }
		std::vector<InputDevice> &get_game_controllers() override { return x11_window.get_game_controllers(); }

		// ---- Window operations ----------------------------------------------
		void create(DisplayWindowSite *site, const DisplayWindowDescription &desc) override;
		void destroy() { delete this; }

		Point client_to_screen(const Point &p) override { return x11_window.client_to_screen(p); }
		Point screen_to_client(const Point &p) override { return x11_window.screen_to_client(p); }

		void show_system_cursor() override { x11_window.show_system_cursor(); }
		void hide_system_cursor() override { x11_window.hide_system_cursor(); }
		CursorProvider *create_cursor(const CursorDescription &desc) override;
		void set_cursor(CursorProvider *cursor) override;
		void set_cursor(StandardCursor type) override { x11_window.set_cursor(type); }

		void set_title(const std::string &t) override { x11_window.set_title(t); }
		void set_position(const Rect &pos, bool ca) override { x11_window.set_position(pos, ca); }
		void set_size(int w, int h, bool ca) override { x11_window.set_size(w, h, ca); }
		void set_minimum_size(int w, int h, bool ca) override { x11_window.set_minimum_size(w, h, ca); }
		void set_maximum_size(int w, int h, bool ca) override { x11_window.set_maximum_size(w, h, ca); }
		void set_pixel_ratio(float r) override { x11_window.set_pixel_ratio(r); }
		void set_enabled(bool enable) override { x11_window.set_enabled(enable); }

		void minimize()		override { x11_window.minimize(); }
		void restore()		override { x11_window.restore(); }
		void maximize()		override { x11_window.maximize(); }
		void toggle_fullscreen() override { x11_window.toggle_fullscreen(); }
		void show(bool activate) override { x11_window.show(activate); }
		void hide()			override { x11_window.hide(); }
		void bring_to_front()	override { x11_window.bring_to_front(); }

		void capture_mouse(bool capture) override { x11_window.capture_mouse(capture); }
		void request_repaint()		override { x11_window.request_repaint(); }

		void set_clipboard_text(const std::string &t) override { x11_window.set_clipboard_text(t); }
		void set_clipboard_image(const PixelBuffer &b) override { x11_window.set_clipboard_image(b); }

		void set_large_icon(const PixelBuffer &image) override;
		void set_small_icon(const PixelBuffer &image) override;
		void enable_alpha_channel(const Rect &blur_rect) override;
		void extend_frame_into_client_area(int l, int t, int r, int b) override;

		void flip(int interval) override;

		// ---- VulkanWindowProviderBase interface overrides -------------------
		VulkanDevice   *get_vulkan_device()		const override { return vk_device.get(); }
		VkRenderPass	get_render_pass()			const override { return render_pass; }
		VkFramebuffer   get_current_framebuffer()	const override { return swapchain_framebuffers[current_image_index]; }
		VkCommandBuffer get_current_command_buffer()  const override { return command_buffers[current_image_index]; }
		uint32_t		get_current_image_index()	const override { return current_image_index; }
		VkImage		get_swapchain_image(uint32_t i) const override { return swapchain_images[i]; }
		VkExtent2D	get_swapchain_extent()		const override { return swapchain_extent; }
		VkFormat		get_swapchain_format()		const override { return swapchain_image_format; }
		ProcAddress	*get_proc_address(const std::string &function_name) const override;

		uint32_t		get_current_frame()  const override { return static_cast<uint32_t>(current_frame); }
		bool			begin_frame()		override;
		bool			is_frame_begun()	const override { return frame_begun; }

		void end_frame();

	private:
		// ---- Platform-specific hook implementations -------------------------
		void create_surface()					override;
		void create_swapchain(int swap_interval) override;

		void on_window_resized();
		bool on_clicked(XButtonEvent &event);

		static VkPresentModeKHR  choose_present_mode(
			const std::vector<VkPresentModeKHR> &modes, int swap_interval);
		static VkSurfaceFormatKHR choose_surface_format(
			const std::vector<VkSurfaceFormatKHR> &formats, bool want_alpha);

		// ---- Members --------------------------------------------------------
		X11Window					x11_window;
		std::unique_ptr<VulkanDevice> vk_device;
		VulkanContextDescription	vk_desc;

		GraphicContext	gc;
		DisplayWindowSite   *site = nullptr;
	};

} // namespace clan
