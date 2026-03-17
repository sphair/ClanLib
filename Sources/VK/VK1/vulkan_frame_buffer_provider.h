
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
#include <vector>
#include "API/Display/TargetProviders/frame_buffer_provider.h"
#include "API/Core/System/disposable_object.h"
#include "API/Display/Render/texture_1d.h"
#include "API/Display/Render/texture_1d_array.h"
#include "API/Display/Render/texture_2d.h"
#include "API/Display/Render/texture_2d_array.h"
#include "API/Display/Render/texture_3d.h"
#include "API/Display/Render/texture_cube.h"

namespace clan
{
	class VulkanGraphicContextProvider;
	class VulkanDevice;
	class VulkanTextureProvider;
	class VulkanRenderBufferProvider;

	class VulkanFrameBufferProvider final
		: public FrameBufferProvider, public DisposableObject
	{
	public:
		explicit VulkanFrameBufferProvider(VulkanGraphicContextProvider *gc_provider);
		~VulkanFrameBufferProvider() override;

		// ---- FrameBufferProvider interface ----------------------------------

		Size get_size() const override;
		FrameBufferBindTarget get_bind_target() const override { return bind_target; }
		void set_bind_target(FrameBufferBindTarget target) override { bind_target = target; }

		// attach_color — typed overloads matching FrameBufferProvider exactly
		void attach_color(int attachment_index, const RenderBuffer &render_buffer) override;
		void attach_color(int attachment_index, const Texture1D &texture, int level) override;
		void attach_color(int attachment_index, const Texture1DArray &texture, int array_index, int level) override;
		void attach_color(int attachment_index, const Texture2D &texture, int level) override;
		void attach_color(int attachment_index, const Texture2DArray &texture, int array_index, int level) override;
		void attach_color(int attachment_index, const Texture3D &texture, int depth, int level) override;
		void attach_color(int attachment_index, const TextureCube &texture, TextureSubtype subtype, int level) override;
		void detach_color(int attachment_index) override;

		// attach_depth
		void attach_depth(const RenderBuffer &render_buffer) override;
		void attach_depth(const Texture2D &texture, int level) override;
		void attach_depth(const TextureCube &texture, TextureSubtype subtype, int level) override;
		void detach_depth() override;

		// attach_stencil
		void attach_stencil(const RenderBuffer &render_buffer) override;
		void attach_stencil(const Texture2D &texture, int level) override;
		void attach_stencil(const TextureCube &texture, TextureSubtype subtype, int level) override;
		void detach_stencil() override;

		// attach_depth_stencil
		void attach_depth_stencil(const RenderBuffer &render_buffer) override;
		void attach_depth_stencil(const Texture2D &texture, int level) override;
		void attach_depth_stencil(const TextureCube &texture, TextureSubtype subtype, int level) override;
		void detach_depth_stencil() override;

		// ---- Vulkan accessors used by GC provider ---------------------------
		VulkanGraphicContextProvider *get_gc_provider() const { return gc_provider; }
		VkRenderPass   get_render_pass();   // lazily (re)built
		VkFramebuffer  get_framebuffer();   // lazily (re)built

	private:
		void on_dispose() override;
		void invalidate();				// mark dirty when attachments change
		void rebuild();					// build VkRenderPass + VkFramebuffer
		void attach_color_texture(int idx, const Texture &tex, int level);
		void attach_depth_texture(const Texture &tex, int level);

		VulkanGraphicContextProvider *gc_provider;
		VulkanDevice				*vk_device;
		FrameBufferBindTarget		bind_target = FrameBufferBindTarget::draw;

		struct ColorAttachment
		{
			VkImageView   view   = VK_NULL_HANDLE;
			VkFormat	format = VK_FORMAT_UNDEFINED;
			bool		used   = false;
		};

		static constexpr int MAX_COLOR = 8;
		ColorAttachment  color[MAX_COLOR];
		VkImageView	depth_view   = VK_NULL_HANDLE;
		VkFormat		depth_format = VK_FORMAT_UNDEFINED;
		bool			has_depth	= false;
		int			fb_width	= 0;
		int			fb_height	= 0;

		VkRenderPass  render_pass  = VK_NULL_HANDLE;
		VkFramebuffer framebuffer  = VK_NULL_HANDLE;
		bool		dirty		= true;
	};

} // namespace clan
