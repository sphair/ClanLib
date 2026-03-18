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

#include <vulkan/vulkan.h>
#include <string>
#include "API/Core/Math/rect.h"   // for Rect

namespace clan
{
	typedef void (ProcAddress)();

	class VulkanDevice;

	/// \brief Minimal interface shared by both platform window providers.
	///
	/// VulkanGraphicContextProvider stores a VulkanWindowProviderBase* so that
	/// the GC provider header is platform-agnostic.  The concrete derived types
	/// are chosen in vulkan_graphic_context_provider.cpp via a platform #ifdef.
	/// Maximum number of frames that may be in-flight simultaneously.
	/// Shared by the window providers and the GC descriptor pool management.
	static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

	class VulkanWindowProviderBase
	{
	public:
		virtual ~VulkanWindowProviderBase() = default;

		virtual VulkanDevice   *get_vulkan_device()			const = 0;
		virtual VkRenderPass	get_render_pass()			const = 0;
		virtual VkFramebuffer   get_current_framebuffer()	const = 0;
		virtual VkCommandBuffer get_current_command_buffer()	const = 0;
		virtual uint32_t		get_current_image_index()	const = 0;
		virtual VkImage		get_swapchain_image(uint32_t i) const = 0;
		virtual VkExtent2D	get_swapchain_extent()		const = 0;
		virtual VkFormat		get_swapchain_format()		const = 0;
		virtual Rect			get_viewport()				const = 0;
		virtual float		get_pixel_ratio()			const = 0;
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
		virtual uint32_t		get_current_frame()			const = 0;
		virtual bool begin_frame() = 0;

		/// Returns true while the current command buffer is in the recording
		/// state (i.e. after begin_frame() and before end_frame() / flip()).
		virtual bool is_frame_begun() const = 0;
	};

} // namespace clan
