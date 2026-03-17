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

#include "API/VK/vk_mem_alloc_config.h"
#include <string>
#include <vector>
#include <memory>
#include "API/Core/Math/rect.h"   // for Rect

namespace clan
{
	typedef void (ProcAddress)();

	class VulkanDevice;
	class GraphicContext;

	/// Maximum number of frames that may be in-flight simultaneously.
	/// Shared by the window providers and the GC descriptor pool management.
	static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

	/// \brief Minimal interface shared by both platform window providers.
	///
	/// VulkanGraphicContextProvider stores a VulkanWindowProviderBase* so that
	/// the GC provider header is platform-agnostic.  The concrete derived types
	/// are chosen in vulkan_graphic_context_provider.cpp via a platform #ifdef.
	///
	/// This class also hosts the Vulkan swapchain state and all shared
	/// implementation (render pass, depth resources, framebuffers, command
	/// buffers, synchronisation objects, begin/end frame logic).
	/// Platform-specific code is limited to surface creation and swapchain
	/// creation (extent resolution, format/mode selection), which are provided
	/// by pure-virtual hooks.
	class VulkanWindowProviderBase
	{
	public:
		virtual ~VulkanWindowProviderBase() = default;

		// ---- Public interface used by VulkanGraphicContextProvider ----------

		virtual VulkanDevice   *get_vulkan_device()		const = 0;
		virtual VkRenderPass	get_render_pass()		const = 0;
		virtual VkFramebuffer   get_current_framebuffer()	const = 0;
		virtual VkCommandBuffer get_current_command_buffer()	const = 0;
		virtual uint32_t		get_current_image_index()	const = 0;
		virtual VkImage		get_swapchain_image(uint32_t i) const = 0;
		virtual VkExtent2D	get_swapchain_extent()		const = 0;
		virtual VkFormat		get_swapchain_format()		const = 0;
		virtual Rect			get_viewport()			const = 0;
		virtual float		get_pixel_ratio()		const = 0;
		virtual ProcAddress	*get_proc_address(const std::string &fn) const = 0;

		/// Acquire the next swapchain image and begin command buffer recording.
		///
		/// Must be called once per frame before any rendering commands are
		/// recorded into the command buffer.  Returns false if the swapchain
		/// was out-of-date and has been recreated (the caller should skip
		/// rendering for this frame and call begin_frame() again on the next).
		///
		/// VulkanGraphicContextProvider calls this lazily the first time it
		/// needs to record commands in each new frame, so application code
		/// does not need to call it explicitly.
		///
		/// The call is idempotent within a single frame: if the command buffer
		/// is already in the recording state the function returns true
		/// immediately without re-acquiring the image.
		virtual uint32_t		get_current_frame()		const = 0;
		virtual bool begin_frame() = 0;

		/// Returns true while the current command buffer is in the recording
		/// state (i.e. after begin_frame() and before end_frame() / flip()).
		virtual bool is_frame_begun() const = 0;

	protected:
		// ---- Shared Vulkan swapchain state ----------------------------------

		VkSurfaceKHR		surface			= VK_NULL_HANDLE;
		VkSwapchainKHR		swapchain		= VK_NULL_HANDLE;
		VkRenderPass		render_pass		= VK_NULL_HANDLE;

		VkFormat		swapchain_image_format	= VK_FORMAT_UNDEFINED;
		VkExtent2D		swapchain_extent	= {};

		std::vector<VkImage>		swapchain_images;
		std::vector<VkImageView>	swapchain_image_views;
		std::vector<VkFramebuffer>	swapchain_framebuffers;
		std::vector<VkCommandBuffer>	command_buffers;

		VkImage			depth_image		= VK_NULL_HANDLE;
		VmaAllocation		depth_image_memory	= VK_NULL_HANDLE;
		VkImageView		depth_image_view	= VK_NULL_HANDLE;

		std::vector<VkSemaphore>	image_available_semaphores;
		std::vector<VkSemaphore>	render_finished_semaphores;
		std::vector<VkFence>		in_flight_fences;
		std::vector<VkFence>		images_in_flight;

		uint32_t	current_frame		= 0;
		uint32_t	current_image_index	= 0;
		int		current_swap_interval	= 1;
		bool		framebuffer_resized	= false;
		bool		frame_begun		= false;

		// ---- Shared Vulkan helpers ------------------------------------------

		/// Create image views for all current swapchain_images.
		void create_image_views();

		/// Build the render pass (colour + depth/stencil attachments).
		void create_render_pass();

		/// Allocate the depth image, memory, and image-view.
		void create_depth_resources();

		/// Allocate one VkFramebuffer per swapchain image.
		void create_framebuffers();

		/// Allocate one primary VkCommandBuffer per framebuffer.
		void create_command_buffers();

		/// Create per-frame semaphores and fences.
		void create_sync_objects();

		/// Destroy all swapchain-dependent Vulkan objects.
		void cleanup_swapchain();

		/// Shared begin_frame logic.  Derived class begin_frame() delegates here
		/// after passing its gc member.  Returns false when the swapchain was
		/// out-of-date and was recreated.
		bool do_begin_frame(GraphicContext &gc);

		/// Shared end_frame / present logic.  Derived class end_frame() and
		/// flip() delegate here after passing their gc member.
		void do_end_frame(GraphicContext &gc);

		/// Signal that the window has been resized; forwards resize notification
		/// to the GC provider and marks framebuffer_resized.
		void do_on_window_resized(GraphicContext &gc);

		/// Recreate the swapchain and all dependent objects after a resize or
		/// out-of-date event.  Calls the platform-specific create_swapchain()
		/// and then rebuilds all shared resources via the helpers above.
		void do_recreate_swapchain(GraphicContext &gc);

		// ---- Platform-specific hooks (pure virtual) -------------------------

		/// Build the platform VkSurfaceKHR and store it in `surface`.
		virtual void create_surface() = 0;

		/// Build or rebuild the VkSwapchainKHR.
		/// Must populate:  swapchain, swapchain_images,
		///				swapchain_image_format, swapchain_extent.
		/// (Image views are created by the shared create_image_views()
		///  which is called immediately after by do_recreate_swapchain /
		///  the first-time init sequence in the derived create() method.)
		virtual void create_swapchain(int swap_interval) = 0;
	};

} // namespace clan
