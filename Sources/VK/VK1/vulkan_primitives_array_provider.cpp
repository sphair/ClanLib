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
#include "VK/VK1/vulkan_primitives_array_provider.h"
#include "VK/VK1/vulkan_graphic_context_provider.h"
#include "VK/VK1/vulkan_vertex_array_buffer_provider.h"
#include "VK/vulkan_device.h"
#include "API/Display/TargetProviders/primitives_array_provider.h"

namespace clan
{

VulkanPrimitivesArrayProvider::VulkanPrimitivesArrayProvider(
	VulkanDevice				*device,
	VulkanGraphicContextProvider  *gc)
	: vk_device(device)
	, gc_provider(gc)
{
	gc_provider->add_disposable(this);
}

VulkanPrimitivesArrayProvider::~VulkanPrimitivesArrayProvider()
{
	dispose();
}

void VulkanPrimitivesArrayProvider::on_dispose()
{
	attributes.clear();
	binding_descs.clear();
	binding_buffers.clear();
	attribute_descs.clear();
	if (gc_provider)
		gc_provider->remove_disposable(this);
}

// =============================================================================
//  set_attribute
// =============================================================================

void VulkanPrimitivesArrayProvider::set_attribute(int index,
												const VertexData &data,
												bool normalize)
{
	// Remove any previous record for this location
	attributes.erase(
		std::remove_if(attributes.begin(), attributes.end(),
					[index](const AttributeRecord &r){ return r.location == index; }),
		attributes.end());

	// Find or create a binding slot for this (buffer, stride) pair.
	// binding_buffers[i] records which array_provider corresponds to binding slot i,
	// allowing multiple attributes to share one binding when they use the same
	// buffer and stride (interleaved layout).
	VertexArrayBufferProvider *src_provider = data.array_provider;

	uint32_t binding_index = UINT32_MAX;
	for (uint32_t i = 0; i < static_cast<uint32_t>(binding_descs.size()); i++)
	{
		if (binding_descs[i].stride == static_cast<uint32_t>(data.stride) &&
			binding_buffers[i] == src_provider)
		{
			binding_index = i;
			break;
		}
	}

	if (binding_index == UINT32_MAX)
	{
		binding_index = static_cast<uint32_t>(binding_descs.size());
		VkVertexInputBindingDescription bd{};
		bd.binding   = binding_index;
		bd.stride	= static_cast<uint32_t>(data.stride);
		bd.inputRate = VK_VERTEX_INPUT_RATE_VERTEX; // per-vertex (no instancing at this level)
		binding_descs.push_back(bd);
		binding_buffers.push_back(src_provider);
	}

	AttributeRecord rec;
	rec.location  = index;
	rec.data	= data;
	rec.normalize = normalize;
	rec.binding   = binding_index;
	attributes.push_back(rec);

	// Rebuild attribute descriptions
	attribute_descs.clear();
	for (const auto &a : attributes)
	{
		VkVertexInputAttributeDescription ad{};
		ad.location = static_cast<uint32_t>(a.location);
		ad.binding  = a.binding;
		ad.format   = to_vk_format(a.data.type, a.data.size, a.normalize);
		ad.offset   = static_cast<uint32_t>(a.data.offset);
		attribute_descs.push_back(ad);
	}

	// Recompute the FNV-1a content hash so the pipeline cache can key on layout
	// content rather than provider address.  Two PrimitivesArray objects with
	// identical binding + attribute descriptions now share the same cached pipeline,
	// preventing vkCreateGraphicsPipelines from being called on every draw call when
	// the application creates a new PrimitivesArray each frame (immediate-mode usage).
	{
		constexpr uint64_t FNV_OFFSET = 14695981039346656037ULL;
		constexpr uint64_t FNV_PRIME  = 1099511628211ULL;
		uint64_t h = FNV_OFFSET;

		const auto hash_bytes = [&](const void *data, size_t len)
		{
			const uint8_t *p = static_cast<const uint8_t *>(data);
			for (size_t i = 0; i < len; i++)
				h = (h ^ p[i]) * FNV_PRIME;
		};

		for (const auto &bd : binding_descs)
			hash_bytes(&bd, sizeof(bd));
		for (const auto &ad : attribute_descs)
			hash_bytes(&ad, sizeof(ad));

		layout_hash = h;
	}
}

// =============================================================================
//  bind_vertex_buffers
// =============================================================================

void VulkanPrimitivesArrayProvider::bind_vertex_buffers(VkCommandBuffer cmd) const
{
	if (binding_descs.empty()) return;

	// Build one VkBuffer/offset entry per binding slot using binding_buffers,
	// which records the exact provider for each slot (set during set_attribute).
	std::vector<VkBuffer>	buffers(binding_descs.size(), VK_NULL_HANDLE);
	std::vector<VkDeviceSize> offsets(binding_descs.size(), 0);

	for (uint32_t i = 0; i < static_cast<uint32_t>(binding_buffers.size()); i++)
	{
		if (binding_buffers[i])
		{
			auto *vab = static_cast<VulkanVertexArrayBufferProvider *>(binding_buffers[i]);
			buffers[i] = vab->get_buffer();
		}
	}

	// vkCmdBindVertexBuffers replaces: glBindVertexArray binding the VAO's VBO slots
	vkCmdBindVertexBuffers(cmd,
						0,
						static_cast<uint32_t>(buffers.size()),
						buffers.data(),
						offsets.data());
}

// =============================================================================
//  to_vk_format
// =============================================================================

VkFormat VulkanPrimitivesArrayProvider::to_vk_format(VertexAttributeDataType type,
													int size,
													bool normalize)
{
	// Normalised float paths (replaces GL_UNSIGNED_BYTE/normalize=true etc.)
	if (normalize)
	{
		switch (size)
		{
		case 1:
			return (type == VertexAttributeDataType::type_unsigned_byte)
				? VK_FORMAT_R8_UNORM : VK_FORMAT_R8_SNORM;
		case 2:
			return (type == VertexAttributeDataType::type_unsigned_byte)
				? VK_FORMAT_R8G8_UNORM : VK_FORMAT_R8G8_SNORM;
		case 3:
			return (type == VertexAttributeDataType::type_unsigned_byte)
				? VK_FORMAT_R8G8B8_UNORM : VK_FORMAT_R8G8B8_SNORM;
		case 4:
			return (type == VertexAttributeDataType::type_unsigned_byte)
				? VK_FORMAT_R8G8B8A8_UNORM : VK_FORMAT_R8G8B8A8_SNORM;
		}
	}

	switch (type)
	{
	case VertexAttributeDataType::type_float:
		switch (size)
		{
		case 1: return VK_FORMAT_R32_SFLOAT;
		case 2: return VK_FORMAT_R32G32_SFLOAT;
		case 3: return VK_FORMAT_R32G32B32_SFLOAT;
		case 4: return VK_FORMAT_R32G32B32A32_SFLOAT;
		}
		break;

	case VertexAttributeDataType::type_int:
		switch (size)
		{
		case 1: return VK_FORMAT_R32_SINT;
		case 2: return VK_FORMAT_R32G32_SINT;
		case 3: return VK_FORMAT_R32G32B32_SINT;
		case 4: return VK_FORMAT_R32G32B32A32_SINT;
		}
		break;

	case VertexAttributeDataType::type_unsigned_int:
		switch (size)
		{
		case 1: return VK_FORMAT_R32_UINT;
		case 2: return VK_FORMAT_R32G32_UINT;
		case 3: return VK_FORMAT_R32G32B32_UINT;
		case 4: return VK_FORMAT_R32G32B32A32_UINT;
		}
		break;

	case VertexAttributeDataType::type_short:
		// glVertexAttribIPointer(GL_SHORT) - signed 16-bit
		switch (size)
		{
		case 1: return VK_FORMAT_R16_SINT;
		case 2: return VK_FORMAT_R16G16_SINT;
		case 3: return VK_FORMAT_R16G16B16_SINT;
		case 4: return VK_FORMAT_R16G16B16A16_SINT;
		}
		break;

	case VertexAttributeDataType::type_unsigned_short:
		switch (size)
		{
		case 1: return VK_FORMAT_R16_UINT;
		case 2: return VK_FORMAT_R16G16_UINT;
		case 3: return VK_FORMAT_R16G16B16_UINT;
		case 4: return VK_FORMAT_R16G16B16A16_UINT;
		}
		break;

	case VertexAttributeDataType::type_byte:
		switch (size)
		{
		case 1: return VK_FORMAT_R8_SINT;
		case 2: return VK_FORMAT_R8G8_SINT;
		case 3: return VK_FORMAT_R8G8B8_SINT;
		case 4: return VK_FORMAT_R8G8B8A8_SINT;
		}
		break;

	case VertexAttributeDataType::type_unsigned_byte:
		switch (size)
		{
		case 1: return VK_FORMAT_R8_UINT;
		case 2: return VK_FORMAT_R8G8_UINT;
		case 3: return VK_FORMAT_R8G8B8_UINT;
		case 4: return VK_FORMAT_R8G8B8A8_UINT;
		}
		break;

	default:
		break;
	}

	return VK_FORMAT_R32G32B32A32_SFLOAT; // safe fallback
}

} // namespace clan
