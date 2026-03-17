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
#include <vector>
#include "API/Display/TargetProviders/primitives_array_provider.h"
#include "API/Display/TargetProviders/vertex_array_buffer_provider.h"
#include "API/Core/System/disposable_object.h"

namespace clan
{
	class VulkanGraphicContextProvider;
	class VulkanDevice;

	class VulkanPrimitivesArrayProvider final
		: public PrimitivesArrayProvider, public DisposableObject
	{
	public:
		VulkanPrimitivesArrayProvider(VulkanDevice *device,
									VulkanGraphicContextProvider *gc_provider);
		~VulkanPrimitivesArrayProvider() override;

		VulkanGraphicContextProvider *get_gc_provider() const { return gc_provider; }

		// PrimitivesArrayProvider interface
		void set_attribute(int index, const VertexData &data, bool normalize) override;

		// Called by VulkanGraphicContextProvider immediately before vkCmdDraw*
		void bind_vertex_buffers(VkCommandBuffer cmd) const;

		// Called by get_or_create_pipeline() to build VkPipelineVertexInputStateCreateInfo
		const std::vector<VkVertexInputBindingDescription>   &get_binding_descriptions()   const { return binding_descs;   }
		const std::vector<VkVertexInputAttributeDescription> &get_attribute_descriptions() const { return attribute_descs; }

		/// FNV-1a hash of the vertex binding + attribute description arrays.
		/// Recomputed whenever set_attribute() is called.  Used as the vertex-
		/// layout component of VulkanPipelineKey so pipelines are cached by
		/// layout content rather than by provider address.
		uint64_t get_layout_hash() const { return layout_hash; }

	private:
		void on_dispose() override;

		// Returns a VkFormat for the given VertexData (type + size + normalize)
		static VkFormat to_vk_format(VertexAttributeDataType type, int size, bool normalize);

		struct AttributeRecord
		{
			int			location  = 0;
			VertexData	data;
			bool			normalize = false;
			uint32_t		binding   = 0;   // index into binding_descs
		};

		VulkanDevice				*vk_device;
		VulkanGraphicContextProvider  *gc_provider;
		std::vector<AttributeRecord>   attributes;

		// Rebuilt on every set_attribute() call.
		// binding_buffers[i] holds the VertexArrayBufferProvider* for binding slot i,
		// enabling correct (buffer,stride) deduplication and bind_vertex_buffers lookup.
		std::vector<VkVertexInputBindingDescription>	binding_descs;
		std::vector<VertexArrayBufferProvider *>		binding_buffers;
		std::vector<VkVertexInputAttributeDescription>  attribute_descs;

		/// FNV-1a hash of binding_descs + attribute_descs content.
		/// Recomputed by set_attribute() after rebuilding the description arrays.
		uint64_t layout_hash = 0;
	};

} // namespace clan
