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

#include "API/VK/volk.h"
#include "API/Display/TargetProviders/pixel_buffer_provider.h"
#include "API/Core/System/disposable_object.h"

namespace clan
{
	class VulkanDevice;

	class VulkanPixelBufferProvider final
		: public PixelBufferProvider, public DisposableObject
	{
	public:
		VulkanPixelBufferProvider();
		~VulkanPixelBufferProvider() override;

		void set_device(VulkanDevice *device) { vk_device = device; }

		// ---- PixelBufferProvider interface ----------------------------------

		void create(const void *data, const Size &size,
					PixelBufferDirection direction, TextureFormat format,
					BufferUsage usage) override;

		void *get_data() override;

		// Pitch = width * bytes_per_pixel (tightly packed, no row padding in Vulkan staging buffers)
		int get_pitch() const override;

		Size get_size() const override { return buf_size; }

		// Vulkan pixel buffers are always GPU-backed staging buffers
		bool is_gpu() const override { return true; }

		// Returns the texture format this buffer was created with
		TextureFormat get_format() const override { return tex_format; }

		void lock(GraphicContext &gc, BufferAccess access) override;
		void unlock() override;
		void upload_data(GraphicContext &gc, const Rect &dest_rect,
						const void *data) override;

		// Vulkan-specific helpers used by texture / GC providers
		VkBuffer get_buffer()  const { return buffer;  }
		VkDeviceMemory get_memory() const { return memory; }
		VkDeviceSize get_bytes() const { return allocated_bytes; }

	private:
		void on_dispose() override;
		void ensure_buffer(VkDeviceSize bytes);

		VulkanDevice  *vk_device	= nullptr;
		VkBuffer	buffer	= VK_NULL_HANDLE;
		VkDeviceMemory memory	= VK_NULL_HANDLE;
		void		*mapped_ptr   = nullptr;
		Size		buf_size;
		VkDeviceSize   allocated_bytes = 0;
		TextureFormat  tex_format   = TextureFormat::rgba8;
	};

} // namespace clan
