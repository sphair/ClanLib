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
#include "VK/VK1/vulkan_graphic_context_provider.h"

#ifdef _WIN32
#  include "VK/Platform/Win32/vulkan_window_provider.h"
#else
#  include "VK/Platform/X11/vulkan_window_provider_x11.h"
#endif

#include "VK/vulkan_device.h"
#include "VK/VK1/vulkan_texture_provider.h"
#include "VK/VK1/vulkan_program_object_provider.h"
#include "VK/VK1/vulkan_shader_object_provider.h"
#include "VK/VK1/vulkan_primitives_array_provider.h"
#include "VK/VK1/vulkan_frame_buffer_provider.h"
#include "VK/VK1/vulkan_render_buffer_provider.h"
#include "VK/VK1/vulkan_vertex_array_buffer_provider.h"
#include "VK/VK1/vulkan_element_array_buffer_provider.h"
#include "VK/VK1/vulkan_uniform_buffer_provider.h"
#include "VK/VK1/vulkan_storage_buffer_provider.h"
#include "VK/VK1/vulkan_transfer_buffer_provider.h"
#include "VK/VK1/vulkan_pixel_buffer_provider.h"
#include "VK/VK1/vulkan_occlusion_query_provider.h"
#include "VK/VK1/vulkan_standard_programs.h"
#include "API/Display/Render/shared_gc_data.h"
#include "API/Display/Render/program_object.h"
#include "API/Display/Render/primitives_array.h"
#include "API/Display/Render/frame_buffer.h"
#include "API/Display/Image/pixel_buffer.h"
#include <cstring>
#include <algorithm>
#include <array>

namespace clan
{

// =============================================================================
//  Construction / destruction
// =============================================================================

VulkanGraphicContextProvider::VulkanGraphicContextProvider(
	VulkanWindowProviderBase *window)
	: render_window(window)
	, vk_device(window->get_vulkan_device())
{
	// Explicit placement-new guarantees a fully zero-padded pipeline_key even if
	// the struct constructor is ever changed.  Without this the memcmp-based cache
	// lookup is unreliable (padding bytes stay uninitialised, so logically-equal
	// keys never compare equal and vkCreateGraphicsPipelines is called every frame,
	// ensuring the pipeline cache stays consistent.
	new (&pipeline_key) VulkanPipelineKey();

	create_descriptor_set_layout();
	create_descriptor_pools();
	create_dummy_texture();

	create_standard_programs();
	reset_program_object();

	current_render_pass	= render_window->get_render_pass();
	pipeline_key.render_pass = current_render_pass;

	SharedGCData::add_provider(this);
}

VulkanGraphicContextProvider::~VulkanGraphicContextProvider()
{
	dispose();
}

void VulkanGraphicContextProvider::on_dispose()
{
	end_render_pass_if_active(render_window->get_current_command_buffer());

	while (!disposable_objects.empty())
		disposable_objects.front()->dispose();

	prog_color_only	= ProgramObject();
	prog_single_texture = ProgramObject();
	prog_sprite		= ProgramObject();
	prog_path		= ProgramObject();

	vkDeviceWaitIdle(vk_device->get_device());

	for (auto &[key, pl] : pipeline_cache)
		vkDestroyPipeline(vk_device->get_device(), pl, nullptr);
	pipeline_cache.clear();

	for (auto &[layout, pl] : layout_cache)
		vkDestroyPipelineLayout(vk_device->get_device(), pl, nullptr);
	layout_cache.clear();

	current_descriptor_set = VK_NULL_HANDLE;
	for (int i = 0; i < DESC_POOL_FRAMES; i++)
	{
		if (descriptor_pools[i] != VK_NULL_HANDLE)
		{
			vkDestroyDescriptorPool(vk_device->get_device(), descriptor_pools[i], nullptr);
			descriptor_pools[i] = VK_NULL_HANDLE;
		}
	}
	if (descriptor_layout != VK_NULL_HANDLE)
	{
		vkDestroyDescriptorSetLayout(vk_device->get_device(), descriptor_layout, nullptr);
		descriptor_layout = VK_NULL_HANDLE;
	}

	// Destroy the dummy texture resources
	VkDevice dev_d = vk_device->get_device();
	if (dummy_sampler	!= VK_NULL_HANDLE) { vkDestroySampler(dev_d, dummy_sampler, nullptr);		dummy_sampler	= VK_NULL_HANDLE; }
	if (dummy_image_view   != VK_NULL_HANDLE) { vkDestroyImageView(dev_d, dummy_image_view, nullptr);	dummy_image_view   = VK_NULL_HANDLE; }
	if (dummy_image		!= VK_NULL_HANDLE) { vkDestroyImage(dev_d, dummy_image, nullptr);			dummy_image		= VK_NULL_HANDLE; }
	if (dummy_image_memory != VK_NULL_HANDLE) { vkFreeMemory(dev_d, dummy_image_memory, nullptr);		dummy_image_memory = VK_NULL_HANDLE; }

	SharedGCData::remove_provider(this);
}

// =============================================================================
//  add_disposable / remove_disposable
// =============================================================================

void VulkanGraphicContextProvider::add_disposable(DisposableObject *d)
{
	disposable_objects.push_back(d);
}

void VulkanGraphicContextProvider::remove_disposable(DisposableObject *d)
{
	for (size_t i = 0; i < disposable_objects.size(); i++)
	{
		if (disposable_objects[i] == d)
		{
			disposable_objects.erase(disposable_objects.begin() + i);
			return;
		}
	}
}

// =============================================================================
//  Standard programs
// =============================================================================

void VulkanGraphicContextProvider::create_standard_programs()
{
	std::unique_ptr<std::unique_lock<std::recursive_mutex>> mutex_section;
	std::vector<GraphicContextProvider *> &gc_providers =
		SharedGCData::get_gc_providers(mutex_section);

	unsigned int max = static_cast<unsigned int>(gc_providers.size());
	for (unsigned int cnt = 0; cnt < max; cnt++)
	{
		VulkanGraphicContextProvider *gc_provider =
			dynamic_cast<VulkanGraphicContextProvider *>(gc_providers[cnt]);
		if (gc_provider != nullptr && gc_provider != this)
		{
			prog_color_only	= gc_provider->prog_color_only;
			prog_single_texture = gc_provider->prog_single_texture;
			prog_sprite		= gc_provider->prog_sprite;
			prog_path		= gc_provider->prog_path;
			return;
		}
	}

	VulkanProgramObjectProvider *co = nullptr, *st = nullptr,
								*sp = nullptr, *pa = nullptr;
	VulkanStandardPrograms::create(vk_device, co, st, sp, pa);
	prog_color_only	= ProgramObject(std::unique_ptr<ProgramObjectProvider>(co));
	prog_single_texture = ProgramObject(std::unique_ptr<ProgramObjectProvider>(st));
	prog_sprite		= ProgramObject(std::unique_ptr<ProgramObjectProvider>(sp));
	prog_path		= ProgramObject(std::unique_ptr<ProgramObjectProvider>(pa));
}

// =============================================================================
//  get_proc_address
// =============================================================================
ProcAddress *VulkanGraphicContextProvider::get_proc_address(
	const std::string &function_name) const
{
	return render_window->get_proc_address(function_name);
}

// =============================================================================
//  Capabilities
// =============================================================================

int VulkanGraphicContextProvider::get_max_attributes()
{
	VkPhysicalDeviceProperties props{};
	vkGetPhysicalDeviceProperties(vk_device->get_physical_device(), &props);
	int max_attributes = static_cast<int>(props.limits.maxVertexInputAttributes);
	if (max_attributes < 16) max_attributes = 16;
	return max_attributes;
}

Size VulkanGraphicContextProvider::get_max_texture_size() const
{
	VkPhysicalDeviceProperties props{};
	vkGetPhysicalDeviceProperties(vk_device->get_physical_device(), &props);
	int s = static_cast<int>(props.limits.maxImageDimension2D);
	return Size(s, s);
}

Size VulkanGraphicContextProvider::get_display_window_size() const
{
	VkExtent2D ext = render_window->get_swapchain_extent();
	return Size(static_cast<int>(ext.width), static_cast<int>(ext.height));
}

float VulkanGraphicContextProvider::get_pixel_ratio() const
{
	return render_window->get_pixel_ratio();
}

ProgramObject VulkanGraphicContextProvider::get_program_object(
	StandardProgram sp) const
{
	switch (sp)
	{
	case StandardProgram::color_only:	return prog_color_only;
	case StandardProgram::single_texture: return prog_single_texture;
	case StandardProgram::sprite:		return prog_sprite;
	case StandardProgram::path:		return prog_path;
	default: throw Exception("Unknown StandardProgram");
	}
}

// =============================================================================
//  Descriptor layout and pool
// =============================================================================

void VulkanGraphicContextProvider::create_descriptor_set_layout()
{
	const uint32_t total_bindings = MAX_TEXTURES + MAX_UBOS + MAX_SSBOS;

	std::vector<VkDescriptorSetLayoutBinding> bindings;
	bindings.reserve(total_bindings);

	for (int i = 0; i < MAX_TEXTURES; i++)
	{
		VkDescriptorSetLayoutBinding b{};
		b.binding		= static_cast<uint32_t>(i);
		b.descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		b.descriptorCount = 1;
		b.stageFlags	= VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT |
							VK_SHADER_STAGE_COMPUTE_BIT;
		bindings.push_back(b);
	}
	for (int i = 0; i < MAX_UBOS; i++)
	{
		VkDescriptorSetLayoutBinding b{};
		b.binding		= static_cast<uint32_t>(MAX_TEXTURES + i);
		b.descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		b.descriptorCount = 1;
		b.stageFlags	= VK_SHADER_STAGE_ALL;
		bindings.push_back(b);
	}
	for (int i = 0; i < MAX_SSBOS; i++)
	{
		VkDescriptorSetLayoutBinding b{};
		b.binding		= static_cast<uint32_t>(MAX_TEXTURES + MAX_UBOS + i);
		b.descriptorType  = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		b.descriptorCount = 1;
		b.stageFlags	= VK_SHADER_STAGE_ALL;
		bindings.push_back(b);
	}

	// Mark every binding as PARTIALLY_BOUND so that unwritten slots (e.g. texture
	// bindings when drawing with color_only which has no samplers) are legal as long
	// as the shader does not actually access them.  Without this flag the driver
	// treats any unwritten binding as invalid and returns VK_ERROR_DEVICE_LOST from
	// vkQueueSubmit even when the executing shader never touches those descriptors.
	// VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT requires the
	// descriptorBindingPartiallyBound device feature (enabled in VulkanDevice).
	std::vector<VkDescriptorBindingFlags> binding_flags(
		total_bindings, VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT);

	VkDescriptorSetLayoutBindingFlagsCreateInfo flags_ci{};
	flags_ci.sType		= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;
	flags_ci.bindingCount  = total_bindings;
	flags_ci.pBindingFlags = binding_flags.data();

	VkDescriptorSetLayoutCreateInfo ci{};
	ci.sType		= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	ci.pNext		= &flags_ci;
	ci.bindingCount = static_cast<uint32_t>(bindings.size());
	ci.pBindings	= bindings.data();

	if (vkCreateDescriptorSetLayout(vk_device->get_device(), &ci, nullptr,
									&descriptor_layout) != VK_SUCCESS)
		throw Exception("Failed to create Vulkan descriptor set layout");
}

void VulkanGraphicContextProvider::create_descriptor_pools()
{
	// One pool per frame slot.  The pool is reset (not freed) at the start of each
	// frame after the fence confirms the GPU has finished with that slot's work.
	// This reclaims all descriptor sets from that frame in one call without needing
	// to track or free individual sets, which would be illegal while the command
	// buffer referencing them is still recording.
	constexpr uint32_t SETS_PER_FRAME = 256;
	std::array<VkDescriptorPoolSize, 3> pool_sizes{{
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, SETS_PER_FRAME * MAX_TEXTURES },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,		SETS_PER_FRAME * MAX_UBOS	},
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,		SETS_PER_FRAME * MAX_SSBOS	}
	}};

	VkDescriptorPoolCreateInfo ci{};
	ci.sType		= VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	ci.flags		= 0; // no FREE_DESCRIPTOR_SET_BIT: sets are freed by pool reset only
	ci.maxSets	= SETS_PER_FRAME;
	ci.poolSizeCount = static_cast<uint32_t>(pool_sizes.size());
	ci.pPoolSizes	= pool_sizes.data();

	for (int i = 0; i < DESC_POOL_FRAMES; i++)
	{
		if (vkCreateDescriptorPool(vk_device->get_device(), &ci, nullptr,
								&descriptor_pools[i]) != VK_SUCCESS)
			throw Exception("Failed to create Vulkan descriptor pool");
	}
}

// =============================================================================
//  create_dummy_texture
//  Creates a persistent 1×1 opaque-white RGBA8 texture (image, view, sampler).
//  Used by flush_descriptors() to fill any sprite-texture binding slot (0..15)
//  that has no real texture bound.  Writing a valid VkDescriptorImageInfo for
//  every declared binding guarantees the descriptor set is fully populated and
//  passes strict driver validation without requiring
//  VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT for the texture range.
// =============================================================================

void VulkanGraphicContextProvider::create_dummy_texture()
{
	VkDevice dev = vk_device->get_device();

	// --- 1x1 RGBA8 image ---
	VkImageCreateInfo img_ci{};
	img_ci.sType		= VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	img_ci.imageType	= VK_IMAGE_TYPE_2D;
	img_ci.format		= VK_FORMAT_R8G8B8A8_UNORM;
	img_ci.extent		= { 1, 1, 1 };
	img_ci.mipLevels	= 1;
	img_ci.arrayLayers   = 1;
	img_ci.samples	= VK_SAMPLE_COUNT_1_BIT;
	img_ci.tiling		= VK_IMAGE_TILING_OPTIMAL;
	img_ci.usage		= VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
	img_ci.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;
	img_ci.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	if (vkCreateImage(dev, &img_ci, nullptr, &dummy_image) != VK_SUCCESS)
		throw Exception("Failed to create Vulkan dummy texture image");

	VkMemoryRequirements mem_req{};
	vkGetImageMemoryRequirements(dev, dummy_image, &mem_req);

	VkMemoryAllocateInfo alloc_ci{};
	alloc_ci.sType		= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	alloc_ci.allocationSize  = mem_req.size;
	alloc_ci.memoryTypeIndex = vk_device->find_memory_type(
		mem_req.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	if (vkAllocateMemory(dev, &alloc_ci, nullptr, &dummy_image_memory) != VK_SUCCESS)
		throw Exception("Failed to allocate Vulkan dummy texture memory");

	if (vkBindImageMemory(dev, dummy_image, dummy_image_memory, 0) != VK_SUCCESS)
		throw Exception("Failed to bind Vulkan dummy texture image memory");

	// --- Upload 1×1 white pixel via staging buffer ---
	{
		const uint8_t white_pixel[4] = { 0xFF, 0xFF, 0xFF, 0xFF };

		VkBuffer	stg_buf{};
		VkDeviceMemory stg_mem{};

		VkBufferCreateInfo stg_ci{};
		stg_ci.sType	= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		stg_ci.size		= 4;
		stg_ci.usage	= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		stg_ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		if (vkCreateBuffer(dev, &stg_ci, nullptr, &stg_buf) != VK_SUCCESS)
			throw Exception("Failed to create Vulkan dummy texture staging buffer");

		VkMemoryRequirements stg_req{};
		vkGetBufferMemoryRequirements(dev, stg_buf, &stg_req);

		VkMemoryAllocateInfo stg_alloc{};
		stg_alloc.sType		= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		stg_alloc.allocationSize  = stg_req.size;
		stg_alloc.memoryTypeIndex = vk_device->find_memory_type(
			stg_req.memoryTypeBits,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		if (vkAllocateMemory(dev, &stg_alloc, nullptr, &stg_mem) != VK_SUCCESS)
		{
			vkDestroyBuffer(dev, stg_buf, nullptr);
			throw Exception("Failed to allocate Vulkan dummy texture staging memory");
		}
		if (vkBindBufferMemory(dev, stg_buf, stg_mem, 0) != VK_SUCCESS)
		{
			vkFreeMemory(dev, stg_mem, nullptr);
			vkDestroyBuffer(dev, stg_buf, nullptr);
			throw Exception("Failed to bind Vulkan dummy texture staging buffer memory");
		}

		void *mapped = nullptr;
		if (vkMapMemory(dev, stg_mem, 0, 4, 0, &mapped) != VK_SUCCESS)
		{
			vkFreeMemory(dev, stg_mem, nullptr);
			vkDestroyBuffer(dev, stg_buf, nullptr);
			throw Exception("Failed to map Vulkan dummy texture staging memory");
		}
		std::memcpy(mapped, white_pixel, 4);
		vkUnmapMemory(dev, stg_mem);

		VkCommandBuffer cmd = vk_device->begin_single_time_commands();

		// UNDEFINED -> TRANSFER_DST_OPTIMAL
		VkImageMemoryBarrier barrier{};
		barrier.sType						= VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout					= VK_IMAGE_LAYOUT_UNDEFINED;
		barrier.newLayout					= VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.srcQueueFamilyIndex			= VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex			= VK_QUEUE_FAMILY_IGNORED;
		barrier.image						= dummy_image;
		barrier.subresourceRange				= { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
		barrier.srcAccessMask				= 0;
		barrier.dstAccessMask				= VK_ACCESS_TRANSFER_WRITE_BIT;
		vkCmdPipelineBarrier(cmd,
			VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
			0, 0, nullptr, 0, nullptr, 1, &barrier);

		VkBufferImageCopy region{};
		region.imageSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 };
		region.imageExtent	= { 1, 1, 1 };
		vkCmdCopyBufferToImage(cmd, stg_buf, dummy_image,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

		// TRANSFER_DST_OPTIMAL -> SHADER_READ_ONLY_OPTIMAL
		barrier.oldLayout	= VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout	= VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		vkCmdPipelineBarrier(cmd,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
			0, 0, nullptr, 0, nullptr, 1, &barrier);

		vk_device->end_single_time_commands(cmd);

		vkDestroyBuffer(dev, stg_buf, nullptr);
		vkFreeMemory(dev, stg_mem, nullptr);
	}

	// --- Image view ---
	VkImageViewCreateInfo view_ci{};
	view_ci.sType						= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	view_ci.image						= dummy_image;
	view_ci.viewType						= VK_IMAGE_VIEW_TYPE_2D;
	view_ci.format						= VK_FORMAT_R8G8B8A8_UNORM;
	view_ci.subresourceRange				= { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };

	if (vkCreateImageView(dev, &view_ci, nullptr, &dummy_image_view) != VK_SUCCESS)
		throw Exception("Failed to create Vulkan dummy texture image view");

	// --- Sampler (nearest, clamp-to-edge) ---
	VkSamplerCreateInfo sampler_ci{};
	sampler_ci.sType		= VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	sampler_ci.magFilter	= VK_FILTER_NEAREST;
	sampler_ci.minFilter	= VK_FILTER_NEAREST;
	sampler_ci.mipmapMode   = VK_SAMPLER_MIPMAP_MODE_NEAREST;
	sampler_ci.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	sampler_ci.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	sampler_ci.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
	sampler_ci.maxLod	= VK_LOD_CLAMP_NONE;
	sampler_ci.borderColor  = VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;

	if (vkCreateSampler(dev, &sampler_ci, nullptr, &dummy_sampler) != VK_SUCCESS)
		throw Exception("Failed to create Vulkan dummy texture sampler");
}

void VulkanGraphicContextProvider::reset_descriptor_pool(uint32_t frame_index)
{
	if (vkResetDescriptorPool(vk_device->get_device(), descriptor_pools[frame_index], 0) != VK_SUCCESS)
		throw Exception("Failed to reset Vulkan descriptor pool");
	current_descriptor_set = VK_NULL_HANDLE;
}

void VulkanGraphicContextProvider::begin_frame_gc(uint32_t frame_index)
{
	current_frame_index = frame_index;
	reset_descriptor_pool(frame_index);
}

// =============================================================================
//  Resource allocation
// =============================================================================

std::unique_ptr<TextureProvider>
VulkanGraphicContextProvider::alloc_texture(TextureDimensions dims)
{
	// VulkanTextureProvider needs the device to do any actual Vulkan work.
	// Pass it via the two-argument constructor (device + dimensions).
	auto p = std::make_unique<VulkanTextureProvider>(dims);
	p->set_device(vk_device);
	return p;
}

std::unique_ptr<OcclusionQueryProvider>
VulkanGraphicContextProvider::alloc_occlusion_query()
{ return std::make_unique<VulkanOcclusionQueryProvider>(vk_device); }

std::unique_ptr<ProgramObjectProvider>
VulkanGraphicContextProvider::alloc_program_object()
{ return std::make_unique<VulkanProgramObjectProvider>(vk_device); }

std::unique_ptr<ShaderObjectProvider>
VulkanGraphicContextProvider::alloc_shader_object()
{ return std::make_unique<VulkanShaderObjectProvider>(vk_device); }

std::unique_ptr<FrameBufferProvider>
VulkanGraphicContextProvider::alloc_frame_buffer()
{ return std::make_unique<VulkanFrameBufferProvider>(this); }

std::unique_ptr<RenderBufferProvider>
VulkanGraphicContextProvider::alloc_render_buffer()
{
	// Must supply the device before create() is called on the render buffer.
	auto p = std::make_unique<VulkanRenderBufferProvider>();
	p->set_device(vk_device);
	return p;
}

std::unique_ptr<PixelBufferProvider>
VulkanGraphicContextProvider::alloc_pixel_buffer()
{
	auto p = std::make_unique<VulkanPixelBufferProvider>();
	p->set_device(vk_device);
	return p;
}

std::unique_ptr<VertexArrayBufferProvider>
VulkanGraphicContextProvider::alloc_vertex_array_buffer()
{
	auto p = std::make_unique<VulkanVertexArrayBufferProvider>();
	p->set_device(vk_device);
	return p;
}

std::unique_ptr<UniformBufferProvider>
VulkanGraphicContextProvider::alloc_uniform_buffer()
{
	auto p = std::make_unique<VulkanUniformBufferProvider>();
	p->set_device(vk_device);
	return p;
}

std::unique_ptr<StorageBufferProvider>
VulkanGraphicContextProvider::alloc_storage_buffer()
{
	auto p = std::make_unique<VulkanStorageBufferProvider>();
	p->set_device(vk_device);
	return p;
}

std::unique_ptr<ElementArrayBufferProvider>
VulkanGraphicContextProvider::alloc_element_array_buffer()
{
	auto p = std::make_unique<VulkanElementArrayBufferProvider>();
	p->set_device(vk_device);
	return p;
}

std::unique_ptr<TransferBufferProvider>
VulkanGraphicContextProvider::alloc_transfer_buffer()
{
	auto p = std::make_unique<VulkanTransferBufferProvider>();
	p->set_device(vk_device);
	return p;
}

std::unique_ptr<PrimitivesArrayProvider>
VulkanGraphicContextProvider::alloc_primitives_array()
{ return std::make_unique<VulkanPrimitivesArrayProvider>(vk_device, this); }

std::unique_ptr<RasterizerStateProvider>
VulkanGraphicContextProvider::alloc_rasterizer_state(const RasterizerStateDescription &desc)
{ return std::make_unique<VulkanRasterizerStateProvider>(desc); }

std::unique_ptr<BlendStateProvider>
VulkanGraphicContextProvider::alloc_blend_state(const BlendStateDescription &desc)
{ return std::make_unique<VulkanBlendStateProvider>(desc); }

std::unique_ptr<DepthStencilStateProvider>
VulkanGraphicContextProvider::alloc_depth_stencil_state(const DepthStencilStateDescription &desc)
{ return std::make_unique<VulkanDepthStencilStateProvider>(desc); }

// =============================================================================
//  State setters
// =============================================================================

void VulkanGraphicContextProvider::set_rasterizer_state(RasterizerStateProvider *state)
{
	if (state)
	{
		auto *vk_state = static_cast<VulkanRasterizerStateProvider *>(state);
		if (vk_state)
		{
			VulkanRasterizerKey key;
			key.fill_from(vk_state->desc);

			pipeline_key.cull_mode		= key.cull_mode;
			pipeline_key.front_face		= key.front_face;
			pipeline_key.polygon_mode		= key.polygon_mode;
			pipeline_key.scissor_test		= key.scissor_test;
			pipeline_key.depth_bias_enable   = key.depth_bias_enable;
			pipeline_key.depth_bias_constant = key.depth_bias_constant;
			pipeline_key.depth_bias_slope	= key.depth_bias_slope;
			pipeline_key.line_width		= key.line_width;

			scissor_enabled = vk_state->desc.get_enable_scissor();
		}
	}
}

void VulkanGraphicContextProvider::set_blend_state(BlendStateProvider *state,
													const Colorf &blend_color,
													unsigned int /*sample_mask*/)
{
	if (state)
	{
		auto *vk_state = static_cast<VulkanBlendStateProvider *>(state);
		if (vk_state)
		{
			blend_color_value = blend_color;

			VulkanBlendKey key;
			key.fill_from(vk_state->desc);

			pipeline_key.blend_enable	= key.blend_enable;
			pipeline_key.src_color	= key.src_color;
			pipeline_key.dst_color	= key.dst_color;
			pipeline_key.color_op		= key.color_op;
			pipeline_key.src_alpha	= key.src_alpha;
			pipeline_key.dst_alpha	= key.dst_alpha;
			pipeline_key.alpha_op		= key.alpha_op;
			pipeline_key.color_write	= key.color_write;
			pipeline_key.logic_op_enable = key.logic_op_enable;
			pipeline_key.logic_op		= key.logic_op;
		}
	}
}

void VulkanGraphicContextProvider::set_depth_stencil_state(DepthStencilStateProvider *state,
															int stencil_ref)
{
	if (state)
	{
		auto *vk_state = static_cast<VulkanDepthStencilStateProvider *>(state);
		if (vk_state)
		{
			dynamic_stencil_ref = stencil_ref;

			VulkanDepthStencilKey key;
			key.fill_from(vk_state->desc);

			pipeline_key.depth_test		= key.depth_test;
			pipeline_key.depth_write		= key.depth_write;
			pipeline_key.depth_func		= key.depth_func;
			pipeline_key.stencil_test	= key.stencil_test;
			pipeline_key.front_func		= key.front_func;
			pipeline_key.front_compare_mask = key.front_compare_mask;
			pipeline_key.front_write_mask   = key.front_write_mask;
			pipeline_key.front_fail		= key.front_fail;
			pipeline_key.front_depth_fail   = key.front_depth_fail;
			pipeline_key.front_pass		= key.front_pass;
			pipeline_key.back_func		= key.back_func;
			pipeline_key.back_compare_mask  = key.back_compare_mask;
			pipeline_key.back_write_mask	= key.back_write_mask;
			pipeline_key.back_fail		= key.back_fail;
			pipeline_key.back_depth_fail	= key.back_depth_fail;
			pipeline_key.back_pass		= key.back_pass;
		}
	}
}

// =============================================================================
//  get_pixeldata
// =============================================================================

PixelBuffer VulkanGraphicContextProvider::get_pixeldata(const Rect &rect,
														TextureFormat texture_format,
														bool /*clamp*/) const
{
	bool format_valid = (texture_format == TextureFormat::rgba8  ||
						texture_format == TextureFormat::bgra8  ||
						texture_format == TextureFormat::rgb8   ||
						texture_format == TextureFormat::r8	||
						texture_format == TextureFormat::rgba16f ||
						texture_format == TextureFormat::rgba32f);
	if (!format_valid)
	{
		throw Exception("Unsupported texture format passed to GraphicContext::get_pixeldata");
	}

	auto *self = const_cast<VulkanGraphicContextProvider *>(this);
	VkCommandBuffer cmd = render_window->get_current_command_buffer();
	self->end_render_pass_if_active(cmd);

	int w = rect.get_width(), h = rect.get_height();
	VkDevice	dev   = vk_device->get_device();
	VkDeviceSize bytes = static_cast<VkDeviceSize>(w) * h * 4;

	VkBuffer buf{}; VkDeviceMemory mem{};
	{
		VkBufferCreateInfo ci{};
		ci.sType	= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		ci.size		= bytes;
		ci.usage	= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		if (vkCreateBuffer(dev, &ci, nullptr, &buf) != VK_SUCCESS)
			throw Exception("Failed to create Vulkan pixel readback buffer");

		VkMemoryRequirements mr{};
		vkGetBufferMemoryRequirements(dev, buf, &mr);
		VkMemoryAllocateInfo ai{};
		ai.sType		= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		ai.allocationSize  = mr.size;
		ai.memoryTypeIndex = vk_device->find_memory_type(
			mr.memoryTypeBits,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		if (vkAllocateMemory(dev, &ai, nullptr, &mem) != VK_SUCCESS)
		{
			vkDestroyBuffer(dev, buf, nullptr);
			throw Exception("Failed to allocate Vulkan pixel readback memory");
		}
		if (vkBindBufferMemory(dev, buf, mem, 0) != VK_SUCCESS)
		{
			vkFreeMemory(dev, mem, nullptr);
			vkDestroyBuffer(dev, buf, nullptr);
			throw Exception("Failed to bind Vulkan pixel readback buffer memory");
		}
	}

	VkCommandBuffer one_shot = vk_device->begin_single_time_commands();
	// get_swapchain_image() is now part of the VulkanWindowProviderBase interface
	VkImage sc_image = render_window->get_swapchain_image(
						render_window->get_current_image_index());

	auto barrier = [&](VkImageLayout from, VkImageLayout to,
					VkAccessFlags src_acc, VkAccessFlags dst_acc)
	{
		VkImageMemoryBarrier b{};
		b.sType			= VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		b.oldLayout		= from; b.newLayout = to;
		b.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		b.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		b.image			= sc_image;
		b.subresourceRange	= { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
		b.srcAccessMask	= src_acc; b.dstAccessMask = dst_acc;
		vkCmdPipelineBarrier(one_shot,
			VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
			0, 0, nullptr, 0, nullptr, 1, &b);
	};

	barrier(VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			VK_ACCESS_MEMORY_READ_BIT, VK_ACCESS_TRANSFER_READ_BIT);

	VkBufferImageCopy region{};
	region.imageSubresource = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 };
	region.imageOffset = { rect.left, rect.top, 0 };
	region.imageExtent = { static_cast<uint32_t>(w), static_cast<uint32_t>(h), 1 };
	vkCmdCopyImageToBuffer(one_shot, sc_image,
						VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, buf, 1, &region);

	barrier(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
			VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_MEMORY_READ_BIT);

	vk_device->end_single_time_commands(one_shot);

	void *mapped = nullptr;
	if (vkMapMemory(dev, mem, 0, bytes, 0, &mapped) != VK_SUCCESS)
	{
		vkDestroyBuffer(dev, buf, nullptr);
		vkFreeMemory(dev, mem, nullptr);
		throw Exception("Failed to map Vulkan pixel readback memory");
	}

	PixelBuffer result(w, h, TextureFormat::rgba8);
	const uint8_t *src = static_cast<const uint8_t *>(mapped);
	for (int row = 0; row < h; row++)
	{
		std::memcpy(result.get_data<uint8_t>() + (h - 1 - row) * result.get_pitch(),
					src + row * w * 4, static_cast<size_t>(w * 4));
	}
	vkUnmapMemory(dev, mem);
	vkDestroyBuffer(dev, buf, nullptr);
	vkFreeMemory(dev, mem, nullptr);

	return (texture_format != TextureFormat::rgba8) ? result.to_format(texture_format)
													: result;
}

// =============================================================================
//  Uniform / storage buffers
// =============================================================================

void VulkanGraphicContextProvider::set_uniform_buffer(int index, const UniformBuffer &buffer)
{
	if (index < 0 || index >= MAX_UBOS) return;
	bound_ubos[index] =
		static_cast<VulkanUniformBufferProvider *>(buffer.get_provider());
	descriptors_dirty = true;
}

void VulkanGraphicContextProvider::reset_uniform_buffer(int index)
{
	if (index < 0 || index >= MAX_UBOS) return;
	bound_ubos[index] = nullptr;
	descriptors_dirty = true;
}

void VulkanGraphicContextProvider::set_storage_buffer(int index, const StorageBuffer &buffer)
{
	if (index < 0 || index >= MAX_SSBOS) return;
	bound_ssbos[index] =
		static_cast<VulkanStorageBufferProvider *>(buffer.get_provider());
	descriptors_dirty = true;
}

void VulkanGraphicContextProvider::reset_storage_buffer(int index)
{
	if (index < 0 || index >= MAX_SSBOS) return;
	bound_ssbos[index] = nullptr;
	descriptors_dirty = true;
}

// =============================================================================
//  Textures
// =============================================================================

void VulkanGraphicContextProvider::set_texture(int unit, const Texture &texture)
{
	if (unit < 0 || unit >= MAX_TEXTURES) return;
	bound_textures[unit] = texture.is_null() ? nullptr
		: static_cast<VulkanTextureProvider *>(texture.get_provider());
	descriptors_dirty = true;
}

void VulkanGraphicContextProvider::reset_texture(int unit)
{
	if (unit < 0 || unit >= MAX_TEXTURES) return;
	bound_textures[unit] = nullptr;
	descriptors_dirty = true;
}

void VulkanGraphicContextProvider::set_image_texture(int unit, const Texture &texture)
{ set_texture(unit, texture); }

void VulkanGraphicContextProvider::reset_image_texture(int unit)
{ reset_texture(unit); }

// =============================================================================
//  Frame buffer
// =============================================================================

bool VulkanGraphicContextProvider::is_frame_buffer_owner(const FrameBuffer &fb)
{
	auto *p = dynamic_cast<VulkanFrameBufferProvider *>(fb.get_provider());
	return p && p->get_gc_provider() == this;
}

void VulkanGraphicContextProvider::set_frame_buffer(const FrameBuffer &draw_buffer,
													const FrameBuffer &read_buffer)
{
	auto *draw = dynamic_cast<VulkanFrameBufferProvider *>(draw_buffer.get_provider());
	auto *read = dynamic_cast<VulkanFrameBufferProvider *>(read_buffer.get_provider());

	if (!draw || draw->get_gc_provider() != this ||
		!read || read->get_gc_provider() != this)
	{
		throw Exception("FrameBuffer objects cannot be shared between multiple GraphicContext objects");
	}

	end_render_pass_if_active(render_window->get_current_command_buffer());

	current_framebuffer	= draw;
	current_render_pass	= draw->get_render_pass();
	pipeline_key.render_pass = current_render_pass;
}

void VulkanGraphicContextProvider::reset_frame_buffer()
{
	end_render_pass_if_active(render_window->get_current_command_buffer());
	current_framebuffer	= nullptr;
	current_render_pass	= render_window->get_render_pass();
	pipeline_key.render_pass = current_render_pass;
}

// =============================================================================
//  Program
// =============================================================================

void VulkanGraphicContextProvider::set_program_object(StandardProgram sp)
{ set_program_object(get_program_object(sp)); }

void VulkanGraphicContextProvider::set_program_object(const ProgramObject &program)
{
	current_program	= program.is_null() ? nullptr
		: static_cast<VulkanProgramObjectProvider *>(program.get_provider());
	pipeline_key.program = current_program;
}

void VulkanGraphicContextProvider::reset_program_object()
{
	current_program	= nullptr;
	pipeline_key.program = nullptr;
}

// =============================================================================
//  Draw buffer
// =============================================================================
void VulkanGraphicContextProvider::set_draw_buffer(DrawBuffer /*buffer*/) {}

// =============================================================================
//  Primitives array ownership
// =============================================================================

bool VulkanGraphicContextProvider::is_primitives_array_owner(const PrimitivesArray &pa)
{
	auto *p = dynamic_cast<VulkanPrimitivesArrayProvider *>(pa.get_provider());
	return p && p->get_gc_provider() == this;
}

// =============================================================================
//  Draw calls
// =============================================================================

void VulkanGraphicContextProvider::draw_primitives(PrimitivesType type, int num_vertices,
													const PrimitivesArray &pa)
{
	set_primitives_array(pa);
	draw_primitives_array(type, 0, num_vertices);
	reset_primitives_array();
}

void VulkanGraphicContextProvider::set_primitives_array(const PrimitivesArray &pa)
{
	current_prim_array   = static_cast<const VulkanPrimitivesArrayProvider *>(pa.get_provider());
	pipeline_key.layout_hash = current_prim_array ? current_prim_array->get_layout_hash() : 0;
}

void VulkanGraphicContextProvider::reset_primitives_array()
{
	current_prim_array   = nullptr;
	pipeline_key.layout_hash = 0;
}

static void apply_dynamic_state(VkCommandBuffer cmd,
								const Rectf &vp, float near_d, float far_d,
								bool scissor_enabled, bool scissor_rect_set,
								const Rect &scissor_rect,
								VkExtent2D swapchain_extent,
								bool stencil_test, int stencil_ref,
								const Colorf &blend_color)
{
	VkViewport vk_vp{};
	vk_vp.x		= vp.left;
	vk_vp.y		= vp.top;
	vk_vp.width	= vp.get_width();
	vk_vp.height   = vp.get_height();
	vk_vp.minDepth = near_d;
	vk_vp.maxDepth = far_d;
	vkCmdSetViewport(cmd, 0, 1, &vk_vp);

	VkRect2D sc{};
	if (scissor_enabled && scissor_rect_set)
	{
		sc.offset = { scissor_rect.left, scissor_rect.top };
		sc.extent = { static_cast<uint32_t>(scissor_rect.get_width()),
					static_cast<uint32_t>(scissor_rect.get_height()) };
	}
	else
	{
		sc.offset = { 0, 0 };
		sc.extent = swapchain_extent;
	}
	vkCmdSetScissor(cmd, 0, 1, &sc);

	if (stencil_test)
		vkCmdSetStencilReference(cmd, VK_STENCIL_FACE_FRONT_AND_BACK,
								static_cast<uint32_t>(stencil_ref));

	float bc[4] = { blend_color.r, blend_color.g, blend_color.b, blend_color.a };
	vkCmdSetBlendConstants(cmd, bc);
}

void VulkanGraphicContextProvider::draw_primitives_array(PrimitivesType type,
														int offset, int num_vertices)
{
	// ensure_render_pass_active calls ensure_frame_begun which may call begin_frame()
	// which may recreate the swapchain and change current_image_index.
	// We must fetch cmd AFTER that call so we get the correct live command buffer.
	ensure_render_pass_active(render_window->get_current_command_buffer());
	VkCommandBuffer  cmd	= render_window->get_current_command_buffer();

	VkPipeline	pl	= get_or_create_pipeline(type);
	VkPipelineLayout layout = get_or_create_pipeline_layout();
	vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pl);

	apply_dynamic_state(cmd, current_viewport, depth_range_near, depth_range_far,
						scissor_enabled, scissor_rect_set, scissor_rect,
						render_window->get_swapchain_extent(),
						pipeline_key.stencil_test, dynamic_stencil_ref,
						blend_color_value);
	viewport_dirty = false;

	if (current_prim_array) current_prim_array->bind_vertex_buffers(cmd);

	if (current_program && current_program->has_push_constants())
	{
		const auto &pc = current_program->get_push_constants();
		vkCmdPushConstants(cmd, layout,
						VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
						0, static_cast<uint32_t>(pc.size()), pc.data());
	}
	flush_descriptors(cmd, layout);
	vkCmdDraw(cmd, num_vertices, 1, offset, 0);
}

void VulkanGraphicContextProvider::draw_primitives_array_instanced(
	PrimitivesType type, int offset, int num_vertices, int instance_count)
{
	ensure_render_pass_active(render_window->get_current_command_buffer());
	VkCommandBuffer  cmd	= render_window->get_current_command_buffer();
	VkPipeline	pl	= get_or_create_pipeline(type);
	VkPipelineLayout layout = get_or_create_pipeline_layout();
	vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pl);
	apply_dynamic_state(cmd, current_viewport, depth_range_near, depth_range_far,
						scissor_enabled, scissor_rect_set, scissor_rect,
						render_window->get_swapchain_extent(),
						pipeline_key.stencil_test, dynamic_stencil_ref,
						blend_color_value);
	if (current_prim_array) current_prim_array->bind_vertex_buffers(cmd);
	flush_descriptors(cmd, layout);
	vkCmdDraw(cmd, num_vertices, instance_count, offset, 0);
}

void VulkanGraphicContextProvider::set_primitives_elements(
	ElementArrayBufferProvider *array_provider)
{
	auto *p = static_cast<VulkanElementArrayBufferProvider *>(array_provider);
	current_index_buffer = p->get_buffer();
	current_index_offset = 0;
}

void VulkanGraphicContextProvider::reset_primitives_elements()
{ current_index_buffer = VK_NULL_HANDLE; }

void VulkanGraphicContextProvider::draw_primitives_elements(
	PrimitivesType type, int count,
	VertexAttributeDataType indices_type, size_t offset)
{
	ensure_render_pass_active(render_window->get_current_command_buffer());
	VkCommandBuffer  cmd	= render_window->get_current_command_buffer();
	VkPipeline	pl	= get_or_create_pipeline(type);
	VkPipelineLayout layout = get_or_create_pipeline_layout();
	vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pl);
	apply_dynamic_state(cmd, current_viewport, depth_range_near, depth_range_far,
						scissor_enabled, scissor_rect_set, scissor_rect,
						render_window->get_swapchain_extent(),
						pipeline_key.stencil_test, dynamic_stencil_ref,
						blend_color_value);
	if (current_prim_array) current_prim_array->bind_vertex_buffers(cmd);
	flush_descriptors(cmd, layout);

	VkIndexType idx_type = to_vk_index_type(indices_type);
	uint32_t	idx_size = (idx_type == VK_INDEX_TYPE_UINT32) ? 4u : 2u;
	vkCmdBindIndexBuffer(cmd, current_index_buffer, current_index_offset, idx_type);
	vkCmdDrawIndexed(cmd, count, 1, static_cast<uint32_t>(offset / idx_size), 0, 0);
}

void VulkanGraphicContextProvider::draw_primitives_elements_instanced(
	PrimitivesType type, int count,
	VertexAttributeDataType indices_type, size_t offset, int instance_count)
{
	ensure_render_pass_active(render_window->get_current_command_buffer());
	VkCommandBuffer  cmd	= render_window->get_current_command_buffer();
	VkPipeline	pl	= get_or_create_pipeline(type);
	VkPipelineLayout layout = get_or_create_pipeline_layout();
	vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pl);
	apply_dynamic_state(cmd, current_viewport, depth_range_near, depth_range_far,
						scissor_enabled, scissor_rect_set, scissor_rect,
						render_window->get_swapchain_extent(),
						pipeline_key.stencil_test, dynamic_stencil_ref,
						blend_color_value);
	if (current_prim_array) current_prim_array->bind_vertex_buffers(cmd);
	flush_descriptors(cmd, layout);

	VkIndexType idx_type = to_vk_index_type(indices_type);
	uint32_t	idx_size = (idx_type == VK_INDEX_TYPE_UINT32) ? 4u : 2u;
	vkCmdBindIndexBuffer(cmd, current_index_buffer, current_index_offset, idx_type);
	vkCmdDrawIndexed(cmd, count, instance_count,
					static_cast<uint32_t>(offset / idx_size), 0, 0);
}

void VulkanGraphicContextProvider::draw_primitives_elements(
	PrimitivesType type, int count,
	ElementArrayBufferProvider *array_provider,
	VertexAttributeDataType indices_type, void *offset)
{
	set_primitives_elements(array_provider);
	draw_primitives_elements(type, count, indices_type, reinterpret_cast<size_t>(offset));
}

void VulkanGraphicContextProvider::draw_primitives_elements_instanced(
	PrimitivesType type, int count,
	ElementArrayBufferProvider *array_provider,
	VertexAttributeDataType indices_type, void *offset, int instance_count)
{
	set_primitives_elements(array_provider);
	draw_primitives_elements_instanced(type, count, indices_type,
									reinterpret_cast<size_t>(offset), instance_count);
}

// =============================================================================
//  Scissor
// =============================================================================

void VulkanGraphicContextProvider::set_scissor(const Rect &rect)
{
	if (!scissor_enabled)
	{
		throw Exception("RasterizerState must be set with enable_scissor() for clipping to work");
	}
	scissor_rect_set = true;
	scissor_rect	= rect;
}

void VulkanGraphicContextProvider::reset_scissor()
{
	scissor_rect_set = false;
}

// =============================================================================
//  Viewport / depth range
// =============================================================================

void VulkanGraphicContextProvider::set_viewport(const Rectf &viewport)
{
	current_viewport = viewport;
	viewport_dirty   = true;
}

void VulkanGraphicContextProvider::set_viewport(int index, const Rectf &viewport)
{
	if (index == 0)
		set_viewport(viewport);
}

void VulkanGraphicContextProvider::set_depth_range(float n, float f)
{ depth_range_near = n; depth_range_far = f; viewport_dirty = true; }

void VulkanGraphicContextProvider::set_depth_range(int vp_index, float n, float f)
{
	if (vp_index == 0)
		set_depth_range(n, f);
}

// =============================================================================
//  Clear
// =============================================================================

// =============================================================================
//  ensure_frame_begun
//  Called lazily before every Vulkan recording command.
//  Guarantees the command buffer is in the recording state by delegating to
//  render_window->begin_frame(), which calls vkBeginCommandBuffer if needed.
// =============================================================================

void VulkanGraphicContextProvider::ensure_frame_begun()
{
	// Loop until begin_frame() succeeds.  begin_frame() returns false when the
	// swapchain was out-of-date and had to be recreated; in that case it also
	// calls on_window_resized() on this provider (via recreate_swapchain) to
	// refresh current_render_pass and flush the pipeline cache.  We must retry
	// rather than fall through, because after a false return the command buffer
	// index has changed and no recording has started for this frame yet.
	while (!render_window->is_frame_begun())
	{
		bool ok = render_window->begin_frame();
		if (ok) return;
		// begin_frame() returned false: swapchain was recreated.
		// on_window_resized() has already been called by recreate_swapchain(),
		// so current_render_pass is fresh.  Loop and try to acquire again.
	}
}

// =============================================================================
//  clear_attachment_immediate
//  Per Vulkan spec §19.3 (vkCmdClearAttachments):
//	colorAttachment is only meaningful when aspectMask includes COLOR_BIT.
//  Some drivers read the field for depth/stencil clears too; guard against
//  this by setting VK_ATTACHMENT_UNUSED for non-colour aspects.
// =============================================================================

void VulkanGraphicContextProvider::clear_attachment_immediate(VkCommandBuffer cmd,
															VkClearAttachment att,
															VkExtent2D extent)
{
	// Per Vulkan spec §19.3, colorAttachment is only meaningful when aspectMask
	// includes VK_IMAGE_ASPECT_COLOR_BIT.  Some drivers read the field regardless,
	// so set it to VK_ATTACHMENT_UNUSED for depth/stencil-only clears.
	if (!(att.aspectMask & VK_IMAGE_ASPECT_COLOR_BIT))
		att.colorAttachment = VK_ATTACHMENT_UNUSED;

	VkClearRect clear_rect{};
	clear_rect.rect.offset	= { 0, 0 };
	clear_rect.rect.extent	= extent;
	clear_rect.baseArrayLayer  = 0;
	clear_rect.layerCount	= 1;
	vkCmdClearAttachments(cmd, 1, &att, 1, &clear_rect);
}

void VulkanGraphicContextProvider::clear(const Colorf &color)
{
	pending_clear_color.float32[0] = color.r;
	pending_clear_color.float32[1] = color.g;
	pending_clear_color.float32[2] = color.b;
	pending_clear_color.float32[3] = color.a;

	if (render_pass_active)
	{
		ensure_frame_begun();
		VkCommandBuffer cmd = render_window->get_current_command_buffer();
		VkClearAttachment att{};
		att.aspectMask	= VK_IMAGE_ASPECT_COLOR_BIT;
		att.colorAttachment = 0;
		att.clearValue.color = pending_clear_color;
		VkExtent2D extent = current_framebuffer
			? VkExtent2D{ static_cast<uint32_t>(current_framebuffer->get_size().width),
						static_cast<uint32_t>(current_framebuffer->get_size().height) }
			: render_window->get_swapchain_extent();
		clear_attachment_immediate(cmd, att, extent);
	}
	else
	{
		pending_clear_mask |= VK_IMAGE_ASPECT_COLOR_BIT;
	}
}

void VulkanGraphicContextProvider::clear_depth(float value)
{
	pending_clear_depth = value;

	if (render_pass_active)
	{
		ensure_frame_begun();
		VkCommandBuffer cmd = render_window->get_current_command_buffer();
		VkClearAttachment att{};
		att.aspectMask				= VK_IMAGE_ASPECT_DEPTH_BIT;
		att.clearValue.depthStencil.depth = value;
		VkExtent2D extent = current_framebuffer
			? VkExtent2D{ static_cast<uint32_t>(current_framebuffer->get_size().width),
						static_cast<uint32_t>(current_framebuffer->get_size().height) }
			: render_window->get_swapchain_extent();
		clear_attachment_immediate(cmd, att, extent);
	}
	else
	{
		pending_clear_mask |= VK_IMAGE_ASPECT_DEPTH_BIT;
	}
}

void VulkanGraphicContextProvider::clear_stencil(int value)
{
	pending_clear_stencil = static_cast<uint32_t>(value);

	if (render_pass_active)
	{
		ensure_frame_begun();
		VkCommandBuffer cmd = render_window->get_current_command_buffer();
		VkClearAttachment att{};
		att.aspectMask					= VK_IMAGE_ASPECT_STENCIL_BIT;
		att.clearValue.depthStencil.stencil = static_cast<uint32_t>(value);
		VkExtent2D extent = current_framebuffer
			? VkExtent2D{ static_cast<uint32_t>(current_framebuffer->get_size().width),
						static_cast<uint32_t>(current_framebuffer->get_size().height) }
			: render_window->get_swapchain_extent();
		clear_attachment_immediate(cmd, att, extent);
	}
	else
	{
		pending_clear_mask |= VK_IMAGE_ASPECT_STENCIL_BIT;
	}
}

// =============================================================================
//  Compute
// =============================================================================
void VulkanGraphicContextProvider::dispatch(int x, int y, int z)
{
	ensure_frame_begun();
	VkCommandBuffer cmd = render_window->get_current_command_buffer();
	end_render_pass_if_active(cmd);
	vkCmdDispatch(cmd, x, y, z);
}

// =============================================================================
//  on_window_resized
// =============================================================================
void VulkanGraphicContextProvider::on_window_resized()
{
	for (auto &[key, pl] : pipeline_cache)
		vkDestroyPipeline(vk_device->get_device(), pl, nullptr);
	pipeline_cache.clear();

	current_render_pass	= render_window->get_render_pass();
	pipeline_key.render_pass = current_render_pass;
	viewport_dirty		= true;
	render_pass_active	= false;

	window_resized_signal(render_window->get_viewport().get_size());
}

// =============================================================================
//  flush
// =============================================================================
void VulkanGraphicContextProvider::flush()
{
	// Submission happens at end_frame() in the window provider.
}

// =============================================================================
//  Pipeline creation
// =============================================================================

VkPipelineLayout VulkanGraphicContextProvider::get_or_create_pipeline_layout()
{
	auto it = layout_cache.find(descriptor_layout);
	if (it != layout_cache.end()) return it->second;

	VkPushConstantRange pc_range{};
	pc_range.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	pc_range.offset	= 0;
	pc_range.size	= 128;

	VkPipelineLayoutCreateInfo ci{};
	ci.sType				= VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	ci.setLayoutCount		= 1;
	ci.pSetLayouts			= &descriptor_layout;
	ci.pushConstantRangeCount = 1;
	ci.pPushConstantRanges	= &pc_range;

	VkPipelineLayout layout;
	if (vkCreatePipelineLayout(vk_device->get_device(), &ci, nullptr, &layout) != VK_SUCCESS)
		throw Exception("Failed to create Vulkan pipeline layout");

	layout_cache[descriptor_layout] = layout;
	return layout;
}

VkPipeline VulkanGraphicContextProvider::get_or_create_pipeline(PrimitivesType type)
{
	pipeline_key.topology = to_vk_topology(type);

	// Refresh render pass in case ensure_frame_begun() triggered a swapchain
	// recreation (which destroys the old VkRenderPass and creates a new one).
	// Without this, pci.renderPass would hold a destroyed handle, causing the
	// driver to crash inside vkCreateGraphicsPipelines.
	VkRenderPass live_rp = current_framebuffer
		? current_framebuffer->get_render_pass()
		: render_window->get_render_pass();
	if (live_rp != current_render_pass)
	{
		// The render pass changed (e.g. swapchain was recreated): flush the
		// pipeline cache because all cached pipelines reference the old handle.
		for (auto &[key, pl] : pipeline_cache)
			vkDestroyPipeline(vk_device->get_device(), pl, nullptr);
		pipeline_cache.clear();

		current_render_pass	= live_rp;
		pipeline_key.render_pass = live_rp;
	}

	auto it = pipeline_cache.find(pipeline_key);
	if (it != pipeline_cache.end()) return it->second;

	if (!current_program)	throw Exception("No program object bound before draw call");
	if (!current_prim_array) throw Exception("No primitives array bound before draw call");
	if (pipeline_key.render_pass == VK_NULL_HANDLE)
		throw Exception("No valid render pass available for pipeline creation");

	VkDevice dev = vk_device->get_device();

	const auto &vi_bindings = current_prim_array->get_binding_descriptions();
	const auto &vi_attribs  = current_prim_array->get_attribute_descriptions();
	VkPipelineVertexInputStateCreateInfo vertex_input{};
	vertex_input.sType						= VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertex_input.vertexBindingDescriptionCount   = static_cast<uint32_t>(vi_bindings.size());
	vertex_input.pVertexBindingDescriptions	= vi_bindings.empty()  ? nullptr : vi_bindings.data();
	vertex_input.vertexAttributeDescriptionCount = static_cast<uint32_t>(vi_attribs.size());
	vertex_input.pVertexAttributeDescriptions	= vi_attribs.empty() ? nullptr : vi_attribs.data();

	VkPipelineInputAssemblyStateCreateInfo input_assembly{};
	input_assembly.sType	= VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	input_assembly.topology = pipeline_key.topology;

	VkPipelineViewportStateCreateInfo viewport_state{};
	viewport_state.sType		= VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewport_state.viewportCount = 1;
	viewport_state.scissorCount  = 1;

	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType				= VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable		= pipeline_key.depth_clamp;
	rasterizer.polygonMode			= pipeline_key.polygon_mode;
	rasterizer.cullMode				= pipeline_key.cull_mode;
	rasterizer.frontFace			= pipeline_key.front_face;
	rasterizer.depthBiasEnable		= pipeline_key.depth_bias_enable;
	rasterizer.depthBiasConstantFactor = pipeline_key.depth_bias_constant;
	rasterizer.depthBiasSlopeFactor	= pipeline_key.depth_bias_slope;
	rasterizer.lineWidth			= pipeline_key.line_width;

	VkPipelineMultisampleStateCreateInfo multisample{};
	multisample.sType				= VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisample.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineDepthStencilStateCreateInfo ds{};
	ds.sType				= VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	ds.depthTestEnable	= pipeline_key.depth_test;
	ds.depthWriteEnable	= pipeline_key.depth_write;
	ds.depthCompareOp		= pipeline_key.depth_func;
	ds.stencilTestEnable	= pipeline_key.stencil_test;
	ds.front = { pipeline_key.front_fail, pipeline_key.front_pass,
				pipeline_key.front_depth_fail, pipeline_key.front_func,
				pipeline_key.front_compare_mask, pipeline_key.front_write_mask,
				static_cast<uint32_t>(dynamic_stencil_ref) };
	ds.back  = { pipeline_key.back_fail,  pipeline_key.back_pass,
				pipeline_key.back_depth_fail,  pipeline_key.back_func,
				pipeline_key.back_compare_mask, pipeline_key.back_write_mask,
				static_cast<uint32_t>(dynamic_stencil_ref) };

	VkPipelineColorBlendAttachmentState blend_att{};
	blend_att.blendEnable		= pipeline_key.blend_enable;
	blend_att.srcColorBlendFactor = pipeline_key.src_color;
	blend_att.dstColorBlendFactor = pipeline_key.dst_color;
	blend_att.colorBlendOp		= pipeline_key.color_op;
	blend_att.srcAlphaBlendFactor = pipeline_key.src_alpha;
	blend_att.dstAlphaBlendFactor = pipeline_key.dst_alpha;
	blend_att.alphaBlendOp		= pipeline_key.alpha_op;
	blend_att.colorWriteMask	= pipeline_key.color_write;

	VkPipelineColorBlendStateCreateInfo color_blend{};
	color_blend.sType		= VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	color_blend.logicOpEnable   = pipeline_key.logic_op_enable;
	color_blend.logicOp		= pipeline_key.logic_op;
	color_blend.attachmentCount = 1;
	color_blend.pAttachments	= &blend_att;

	std::array<VkDynamicState, 4> dyn_states = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR,
		VK_DYNAMIC_STATE_STENCIL_REFERENCE,
		VK_DYNAMIC_STATE_BLEND_CONSTANTS
	};
	VkPipelineDynamicStateCreateInfo dynamic_state{};
	dynamic_state.sType			= VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamic_state.dynamicStateCount = static_cast<uint32_t>(dyn_states.size());
	dynamic_state.pDynamicStates	= dyn_states.data();

	VkPipelineLayout layout = get_or_create_pipeline_layout();

	// get_stages() returns a copy of the pipeline_stages vector.  Using a local
	// value (not a const ref) ensures pName pointers remain valid even if the
	// program provider is somehow relinked between now and vkCreateGraphicsPipelines.
	// The entry_point_names deque inside the provider keeps the strings alive for
	// the provider's lifetime; the local copy here keeps the VkShaderModule
	// handles alive through the call.
	const std::vector<VkPipelineShaderStageCreateInfo> stages =
		current_program->get_stages();

	if (stages.empty())
		throw Exception("Program has no shader stages — call link() before drawing");

	VkGraphicsPipelineCreateInfo pci{};
	pci.sType			= VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pci.stageCount		= static_cast<uint32_t>(stages.size());
	pci.pStages			= stages.data();
	pci.pVertexInputState   = &vertex_input;
	pci.pInputAssemblyState = &input_assembly;
	pci.pViewportState	= &viewport_state;
	pci.pRasterizationState = &rasterizer;
	pci.pMultisampleState   = &multisample;
	pci.pDepthStencilState  = &ds;
	pci.pColorBlendState	= &color_blend;
	pci.pDynamicState	= &dynamic_state;
	pci.layout			= layout;
	pci.renderPass		= pipeline_key.render_pass;
	pci.subpass			= 0;

	VkPipeline pipeline = VK_NULL_HANDLE;
	VkResult result = vkCreateGraphicsPipelines(dev, VK_NULL_HANDLE, 1, &pci, nullptr, &pipeline);
	if (result != VK_SUCCESS)
		throw Exception("Failed to create Vulkan graphics pipeline (VkResult = " +
						std::to_string(static_cast<int>(result)) + ")");

	pipeline_cache[pipeline_key] = pipeline;
	return pipeline;
}

// =============================================================================
//  Descriptor set flush
// =============================================================================

void VulkanGraphicContextProvider::flush_descriptors(VkCommandBuffer cmd,
													VkPipelineLayout layout)
{
	if (!descriptors_dirty && current_descriptor_set != VK_NULL_HANDLE)
	{
		vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS,
								layout, 0, 1, &current_descriptor_set, 0, nullptr);
		return;
	}

	// Never free individual descriptor sets - they belong to the per-frame pool
	// and will be reclaimed in bulk when the pool is reset at begin_frame_gc().
	// Freeing a set while it is still referenced by a recording command buffer is
	// undefined behaviour and causes vkQueueSubmit to return VK_ERROR_DEVICE_LOST.
	current_descriptor_set = VK_NULL_HANDLE;

	VkDescriptorSetAllocateInfo ai{};
	ai.sType			= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	ai.descriptorPool	= descriptor_pools[current_frame_index];
	ai.descriptorSetCount = 1;
	ai.pSetLayouts		= &descriptor_layout;
	if (vkAllocateDescriptorSets(vk_device->get_device(), &ai,
								&current_descriptor_set) != VK_SUCCESS)
		throw Exception("Failed to allocate Vulkan descriptor set");

	std::vector<VkWriteDescriptorSet>	writes;
	std::vector<VkDescriptorImageInfo>   img_infos(MAX_TEXTURES);
	std::vector<VkDescriptorBufferInfo>  ubo_infos(MAX_UBOS);
	std::vector<VkDescriptorBufferInfo>  ssbo_infos(MAX_SSBOS);

	// Number of texture slots declared by the sprite shader.
	// Slots 0..SPRITE_TEXTURE_SLOTS-1 must always have a valid descriptor written
	// so the descriptor set is fully populated regardless of which program is active.
	// Unbound slots receive the dummy 1×1 white texture so that:
	//   (a) strict drivers do not report VK_ERROR_DEVICE_LOST on submit, and
	//   (b) the sprite shader's default: case (texindex >= 16 = "no texture") still
	//	produces white * vertex_colour = vertex_colour, matching GL3 behaviour.
	static constexpr int SPRITE_TEXTURE_SLOTS = 16;

	for (int i = 0; i < MAX_TEXTURES; i++)
	{
		auto *tex = bound_textures[i];

		// For the sprite shader's declared texture range (0..15), always write a
		// valid descriptor.  Use the dummy texture when no real texture is bound.
		// For slots beyond the sprite range (16..MAX_TEXTURES-1), skip if unbound
		// and rely on VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT as before.
		bool needs_dummy = (i < SPRITE_TEXTURE_SLOTS) &&
						(!tex || tex->get_image_view() == VK_NULL_HANDLE);

		if (needs_dummy)
		{
			img_infos[i] = { dummy_sampler, dummy_image_view,
							VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL };
		}
		else
		{
			if (!tex || tex->get_image_view() == VK_NULL_HANDLE) continue;
			img_infos[i] = { tex->get_sampler(), tex->get_image_view(),
							VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL };
		}

		VkWriteDescriptorSet w{};
		w.sType		= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		w.dstSet		= current_descriptor_set;
		w.dstBinding	= static_cast<uint32_t>(i);
		w.descriptorCount = 1;
		w.descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		w.pImageInfo	= &img_infos[i];
		writes.push_back(w);
	}
	for (int i = 0; i < MAX_UBOS; i++)
	{
		auto *ubo = bound_ubos[i];
		if (!ubo || ubo->get_buffer() == VK_NULL_HANDLE) continue;
		ubo_infos[i] = { ubo->get_buffer(), 0, VK_WHOLE_SIZE };
		VkWriteDescriptorSet w{};
		w.sType		= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		w.dstSet		= current_descriptor_set;
		w.dstBinding	= static_cast<uint32_t>(MAX_TEXTURES + i);
		w.descriptorCount = 1;
		w.descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		w.pBufferInfo	= &ubo_infos[i];
		writes.push_back(w);
	}
	for (int i = 0; i < MAX_SSBOS; i++)
	{
		auto *ssbo = bound_ssbos[i];
		if (!ssbo || ssbo->get_buffer() == VK_NULL_HANDLE) continue;
		ssbo_infos[i] = { ssbo->get_buffer(), 0, VK_WHOLE_SIZE };
		VkWriteDescriptorSet w{};
		w.sType		= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		w.dstSet		= current_descriptor_set;
		w.dstBinding	= static_cast<uint32_t>(MAX_TEXTURES + MAX_UBOS + i);
		w.descriptorCount = 1;
		w.descriptorType  = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		w.pBufferInfo	= &ssbo_infos[i];
		writes.push_back(w);
	}

	if (!writes.empty())
		vkUpdateDescriptorSets(vk_device->get_device(),
							static_cast<uint32_t>(writes.size()),
							writes.data(), 0, nullptr);

	vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS,
							layout, 0, 1, &current_descriptor_set, 0, nullptr);
	descriptors_dirty = false;
}

// =============================================================================
//  Render pass management
// =============================================================================

void VulkanGraphicContextProvider::ensure_render_pass_active(VkCommandBuffer /*cmd_hint*/)
{
	// Guarantee the command buffer is in the recording state.  ensure_frame_begun()
	// loops until begin_frame() succeeds, which may recreate the swapchain and
	// change current_image_index.  We therefore ignore the caller's cmd_hint and
	// always re-fetch the live command buffer from the window after this call.
	ensure_frame_begun();

	// Re-fetch the live command buffer: current_image_index may have changed if
	// begin_frame() triggered a swapchain recreation inside ensure_frame_begun().
	VkCommandBuffer cmd = render_window->get_current_command_buffer();

	if (render_pass_active) return;

	std::array<VkClearValue, 2> cv{};

	if (pending_clear_mask & VK_IMAGE_ASPECT_COLOR_BIT)
		cv[0].color = pending_clear_color;

	cv[1].depthStencil.depth   = (pending_clear_mask & VK_IMAGE_ASPECT_DEPTH_BIT)
									? pending_clear_depth   : 1.0f;
	cv[1].depthStencil.stencil = (pending_clear_mask & VK_IMAGE_ASPECT_STENCIL_BIT)
									? pending_clear_stencil : 0u;

	pending_clear_mask = 0;

	VkFramebuffer fb; VkExtent2D extent;
	if (current_framebuffer)
	{
		fb = current_framebuffer->get_framebuffer();
		auto sz = current_framebuffer->get_size();
		extent = { static_cast<uint32_t>(sz.width), static_cast<uint32_t>(sz.height) };
	}
	else
	{
		fb	= render_window->get_current_framebuffer();
		extent = render_window->get_swapchain_extent();
	}

	VkRenderPassBeginInfo rp{};
	rp.sType			= VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	rp.renderPass		= current_render_pass;
	rp.framebuffer	= fb;
	rp.renderArea.offset = { 0, 0 };
	rp.renderArea.extent = extent;
	rp.clearValueCount   = static_cast<uint32_t>(cv.size());
	rp.pClearValues	= cv.data();

	vkCmdBeginRenderPass(cmd, &rp, VK_SUBPASS_CONTENTS_INLINE);
	render_pass_active = true;
}

void VulkanGraphicContextProvider::end_render_pass_if_active(VkCommandBuffer /*cmd_hint*/)
{
	if (!render_pass_active) return;
	// Always use the live command buffer from the window — the hint may be stale
	// if a swapchain recreation changed current_image_index since the caller
	// captured it.  The render pass can only be active if begin_frame() was
	// already called, so get_current_command_buffer() is always valid here.
	vkCmdEndRenderPass(render_window->get_current_command_buffer());
	render_pass_active = false;
}

// =============================================================================
//  Enum converters
// =============================================================================

VkPrimitiveTopology VulkanGraphicContextProvider::to_vk_topology(PrimitivesType type)
{
	switch (type)
	{
	case PrimitivesType::points:		return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
	case PrimitivesType::lines:		return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
	case PrimitivesType::line_strip:	return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
	case PrimitivesType::triangles:	return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	case PrimitivesType::triangle_strip: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
	case PrimitivesType::triangle_fan:   return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
	default:							return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	}
}

VkIndexType VulkanGraphicContextProvider::to_vk_index_type(VertexAttributeDataType t)
{
	switch (t)
	{
	case VertexAttributeDataType::type_unsigned_short: return VK_INDEX_TYPE_UINT16;
	case VertexAttributeDataType::type_unsigned_int:   return VK_INDEX_TYPE_UINT32;
	default:									return VK_INDEX_TYPE_UINT16;
	}
}

} // namespace clan
