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
#include "VK/VK1/vulkan_frame_buffer_provider.h"
#include "VK/VK1/vulkan_graphic_context_provider.h"
#include "VK/VK1/vulkan_texture_provider.h"
#include "VK/VK1/vulkan_render_buffer_provider.h"
#include "VK/vulkan_device.h"
#include "API/Display/Render/texture.h"
#include "API/Display/Render/render_buffer.h"
#ifdef _WIN32
#  include "VK/Platform/Win32/vulkan_window_provider.h"
#else
#  include "VK/Platform/X11/vulkan_window_provider_x11.h"
#endif

namespace clan
{

VulkanFrameBufferProvider::VulkanFrameBufferProvider(VulkanGraphicContextProvider *gc)
	: gc_provider(gc)
	, vk_device(gc->get_render_window()->get_vulkan_device())
{
	gc_provider->add_disposable(this);
}

VulkanFrameBufferProvider::~VulkanFrameBufferProvider()
{
	dispose();
}

void VulkanFrameBufferProvider::on_dispose()
{
	VkDevice dev = vk_device->get_device();
	if (framebuffer != VK_NULL_HANDLE)
	{
		vkDestroyFramebuffer(dev, framebuffer, nullptr);
		framebuffer = VK_NULL_HANDLE;
	}
	if (render_pass != VK_NULL_HANDLE)
	{
		vkDestroyRenderPass(dev, render_pass, nullptr);
		render_pass = VK_NULL_HANDLE;
	}
	if (gc_provider)
		gc_provider->remove_disposable(this);
}

void VulkanFrameBufferProvider::invalidate()
{
	dirty = true;
	// Destroy existing objects so they are rebuilt with new attachments
	VkDevice dev = vk_device->get_device();
	if (framebuffer != VK_NULL_HANDLE)
	{ vkDestroyFramebuffer(dev, framebuffer, nullptr); framebuffer = VK_NULL_HANDLE; }
	if (render_pass != VK_NULL_HANDLE)
	{ vkDestroyRenderPass(dev, render_pass, nullptr); render_pass = VK_NULL_HANDLE; }
}

// ---- internal helpers ----

void VulkanFrameBufferProvider::attach_color_texture(int idx, const Texture &tex, int /*level*/)
{
	if (idx < 0 || idx >= MAX_COLOR) return;
	auto *p = static_cast<VulkanTextureProvider *>(tex.get_provider());
	color[idx].view   = p->get_image_view();
	color[idx].format = p->get_vk_format();
	color[idx].used   = true;
	fb_width  = p->get_width();
	fb_height = p->get_height();
	invalidate();
}

void VulkanFrameBufferProvider::attach_depth_texture(const Texture &tex, int /*level*/)
{
	auto *p	= static_cast<VulkanTextureProvider *>(tex.get_provider());
	depth_view   = p->get_image_view();
	depth_format = p->get_vk_format();
	has_depth	= true;
	invalidate();
}

// ---- attach_color overloads ----

void VulkanFrameBufferProvider::attach_color(int idx, const RenderBuffer &rb)
{
	if (idx < 0 || idx >= MAX_COLOR) return;
	auto *p = static_cast<VulkanRenderBufferProvider *>(rb.get_provider());
	color[idx].view   = p->get_image_view();
	color[idx].format = p->get_format();
	color[idx].used   = true;
	auto sz = p->get_size();
	fb_width = sz.width; fb_height = sz.height;
	invalidate();
}

void VulkanFrameBufferProvider::attach_color(int idx, const Texture1D &tex, int level)
{ attach_color_texture(idx, tex, level); }

void VulkanFrameBufferProvider::attach_color(int idx, const Texture1DArray &tex, int /*array_index*/, int level)
{ attach_color_texture(idx, tex, level); }

void VulkanFrameBufferProvider::attach_color(int idx, const Texture2D &tex, int level)
{ attach_color_texture(idx, tex, level); }

void VulkanFrameBufferProvider::attach_color(int idx, const Texture2DArray &tex, int /*array_index*/, int level)
{ attach_color_texture(idx, tex, level); }

void VulkanFrameBufferProvider::attach_color(int idx, const Texture3D &tex, int /*depth*/, int level)
{ attach_color_texture(idx, tex, level); }

void VulkanFrameBufferProvider::attach_color(int idx, const TextureCube &tex, TextureSubtype /*subtype*/, int level)
{ attach_color_texture(idx, tex, level); }

void VulkanFrameBufferProvider::detach_color(int idx)
{
	if (idx < 0 || idx >= MAX_COLOR) return;
	color[idx] = {};
	invalidate();
}

// ---- attach_depth overloads ----

void VulkanFrameBufferProvider::attach_depth(const RenderBuffer &rb)
{
	auto *p	= static_cast<VulkanRenderBufferProvider *>(rb.get_provider());
	depth_view   = p->get_image_view();
	depth_format = p->get_format();
	has_depth	= true;
	invalidate();
}

void VulkanFrameBufferProvider::attach_depth(const Texture2D &tex, int level)
{ attach_depth_texture(tex, level); }

void VulkanFrameBufferProvider::attach_depth(const TextureCube &tex, TextureSubtype /*subtype*/, int level)
{ attach_depth_texture(tex, level); }

void VulkanFrameBufferProvider::detach_depth()
{ depth_view = VK_NULL_HANDLE; depth_format = VK_FORMAT_UNDEFINED; has_depth = false; invalidate(); }

// ---- attach_stencil overloads (map to depth slot — combined depth/stencil in Vulkan) ----

void VulkanFrameBufferProvider::attach_stencil(const RenderBuffer &rb)
{ attach_depth(rb); }

void VulkanFrameBufferProvider::attach_stencil(const Texture2D &tex, int level)
{ attach_depth_texture(tex, level); }

void VulkanFrameBufferProvider::attach_stencil(const TextureCube &tex, TextureSubtype /*subtype*/, int level)
{ attach_depth_texture(tex, level); }

void VulkanFrameBufferProvider::detach_stencil()
{ detach_depth(); }

// ---- attach_depth_stencil overloads ----

void VulkanFrameBufferProvider::attach_depth_stencil(const RenderBuffer &rb)
{ attach_depth(rb); }

void VulkanFrameBufferProvider::attach_depth_stencil(const Texture2D &tex, int level)
{ attach_depth_texture(tex, level); }

void VulkanFrameBufferProvider::attach_depth_stencil(const TextureCube &tex, TextureSubtype /*subtype*/, int level)
{ attach_depth_texture(tex, level); }

void VulkanFrameBufferProvider::detach_depth_stencil()
{ detach_depth(); }
// =============================================================================
//  Lazy rebuild
//  Replaces: driver-side FBO completeness check from glCheckFramebufferStatus
// =============================================================================

void VulkanFrameBufferProvider::rebuild()
{
	if (!dirty) return;
	dirty = false;

	VkDevice dev = vk_device->get_device();

	// ---- Render pass --------------------------------------------------------
	std::vector<VkAttachmentDescription> attachments;
	std::vector<VkAttachmentReference>   color_refs;
	VkAttachmentReference				depth_ref{};
	bool								has_depth_ref = false;

	for (int i = 0; i < MAX_COLOR; i++)
	{
		if (!color[i].used) continue;

		VkAttachmentDescription ad{};
		ad.format		= color[i].format;
		ad.samples		= VK_SAMPLE_COUNT_1_BIT;
		ad.loadOp		= VK_ATTACHMENT_LOAD_OP_LOAD;
		ad.storeOp		= VK_ATTACHMENT_STORE_OP_STORE;
		ad.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		ad.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		ad.initialLayout  = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		ad.finalLayout	= VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentReference ref{};
		ref.attachment = static_cast<uint32_t>(attachments.size());
		ref.layout	= VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		attachments.push_back(ad);
		color_refs.push_back(ref);
	}

	if (has_depth && depth_view != VK_NULL_HANDLE)
	{
		VkAttachmentDescription ad{};
		ad.format		= depth_format;
		ad.samples		= VK_SAMPLE_COUNT_1_BIT;
		ad.loadOp		= VK_ATTACHMENT_LOAD_OP_LOAD;
		ad.storeOp		= VK_ATTACHMENT_STORE_OP_STORE;
		ad.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_LOAD;
		ad.stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
		ad.initialLayout  = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		ad.finalLayout	= VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		depth_ref.attachment = static_cast<uint32_t>(attachments.size());
		depth_ref.layout	= VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		has_depth_ref		= true;
		attachments.push_back(ad);
	}

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint	= VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount	= static_cast<uint32_t>(color_refs.size());
	subpass.pColorAttachments	= color_refs.empty() ? nullptr : color_refs.data();
	subpass.pDepthStencilAttachment = has_depth_ref ? &depth_ref : nullptr;

	// Subpass dependency so colour writes are visible to subsequent reads
	VkSubpassDependency dep{};
	dep.srcSubpass	= VK_SUBPASS_EXTERNAL;
	dep.dstSubpass	= 0;
	dep.srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
						VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dep.srcAccessMask = 0;
	dep.dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
						VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dep.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
						VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo rp_ci{};
	rp_ci.sType		= VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	rp_ci.attachmentCount = static_cast<uint32_t>(attachments.size());
	rp_ci.pAttachments	= attachments.data();
	rp_ci.subpassCount	= 1;
	rp_ci.pSubpasses	= &subpass;
	rp_ci.dependencyCount = 1;
	rp_ci.pDependencies   = &dep;

	if (vkCreateRenderPass(dev, &rp_ci, nullptr, &render_pass) != VK_SUCCESS)
		throw Exception("VulkanFrameBufferProvider: failed to create render pass");

	// ---- Framebuffer --------------------------------------------------------
	std::vector<VkImageView> fb_views;
	for (int i = 0; i < MAX_COLOR; i++)
		if (color[i].used) fb_views.push_back(color[i].view);
	if (has_depth_ref) fb_views.push_back(depth_view);

	VkFramebufferCreateInfo fb_ci{};
	fb_ci.sType		= VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	fb_ci.renderPass	= render_pass;
	fb_ci.attachmentCount = static_cast<uint32_t>(fb_views.size());
	fb_ci.pAttachments	= fb_views.data();
	fb_ci.width		= static_cast<uint32_t>(fb_width);
	fb_ci.height		= static_cast<uint32_t>(fb_height);
	fb_ci.layers		= 1;

	if (vkCreateFramebuffer(dev, &fb_ci, nullptr, &framebuffer) != VK_SUCCESS)
		throw Exception("VulkanFrameBufferProvider: failed to create framebuffer");
}

VkRenderPass VulkanFrameBufferProvider::get_render_pass()
{ rebuild(); return render_pass; }

VkFramebuffer VulkanFrameBufferProvider::get_framebuffer()
{ rebuild(); return framebuffer; }

Size VulkanFrameBufferProvider::get_size() const
{ return Size(fb_width, fb_height); }

} // namespace clan
