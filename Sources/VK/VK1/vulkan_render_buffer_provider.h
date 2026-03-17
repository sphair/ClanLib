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
#include "API/Display/TargetProviders/render_buffer_provider.h"
#include "API/Core/System/disposable_object.h"

namespace clan
{
	class VulkanDevice;

	class VulkanRenderBufferProvider : public RenderBufferProvider, public DisposableObject
	{
	public:
		VulkanRenderBufferProvider();
		~VulkanRenderBufferProvider() override;

		// ---- RenderBufferProvider interface ----
		/// Base class signature: create(width, height, format, multisample_samples).
		/// The VulkanDevice is supplied separately via set_device() before create() is called
		/// (the GC provider calls set_device immediately after alloc_render_buffer()).
		void create(int width, int height, TextureFormat texture_format,
					int multisample_samples) override;

		/// Called by VulkanGraphicContextProvider immediately after alloc_render_buffer().
		void set_device(VulkanDevice *device) { vk_device = device; }

		VkImage	get_image()	const { return image; }
		VkImageView   get_image_view() const { return image_view; }
		VkFormat	get_format()	const { return vk_format; }

		/// Returns the size of this render buffer (width × height in pixels).
		/// Used by VulkanFrameBufferProvider::attach_color(int, RenderBuffer) to
		/// record fb_width / fb_height when a render buffer is attached.
		Size get_size() const { return Size(rb_width, rb_height); }

	private:
		void on_dispose() override;

		static VkFormat to_vk_format(TextureFormat fmt);

		VulkanDevice   *vk_device	= nullptr;
		VkImage		image		= VK_NULL_HANDLE;
		VkDeviceMemory  image_memory  = VK_NULL_HANDLE;
		VkImageView	image_view	= VK_NULL_HANDLE;
		VkFormat		vk_format	= VK_FORMAT_UNDEFINED;
		int			rb_width	= 0;
		int			rb_height	= 0;
	};
}
