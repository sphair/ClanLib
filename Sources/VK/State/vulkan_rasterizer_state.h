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
#include <cstring>
#include "API/Display/Render/rasterizer_state_description.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"

namespace clan
{
	// =========================================================================
	//  VulkanRasterizerStateProvider
	// =========================================================================
	class VulkanRasterizerStateProvider : public RasterizerStateProvider
	{
	public:
		explicit VulkanRasterizerStateProvider(const RasterizerStateDescription &d)
			: desc(d.clone()) {}
		~VulkanRasterizerStateProvider() override = default;

		RasterizerStateDescription desc;
	};

	// =========================================================================
	//  Enum converters
	// =========================================================================

	inline VkCullModeFlags vk_cull_mode(bool culled, CullMode mode)
	{
		if (!culled) return VK_CULL_MODE_NONE;
		switch (mode)
		{
		case CullMode::front:		return VK_CULL_MODE_FRONT_BIT;
		case CullMode::back:		return VK_CULL_MODE_BACK_BIT;
		case CullMode::front_and_back: return VK_CULL_MODE_FRONT_AND_BACK;
		default:					return VK_CULL_MODE_BACK_BIT;
		}
	}

	inline VkPolygonMode vk_polygon_mode(FillMode m)
	{
		switch (m)
		{
		case FillMode::line:  return VK_POLYGON_MODE_LINE;
		case FillMode::point: return VK_POLYGON_MODE_POINT;
		default:			return VK_POLYGON_MODE_FILL;
		}
	}

	// =========================================================================
	//  VulkanRasterizerKey
	// =========================================================================
	struct VulkanRasterizerKey
	{
		VkCullModeFlags cull_mode		= VK_CULL_MODE_NONE;
		VkFrontFace	front_face		= VK_FRONT_FACE_COUNTER_CLOCKWISE;
		VkPolygonMode   polygon_mode		= VK_POLYGON_MODE_FILL;
		VkBool32		depth_clamp		= VK_FALSE;
		VkBool32		scissor_test		= VK_FALSE;
		VkBool32		depth_bias_enable   = VK_FALSE;
		float		depth_bias_constant = 0.0f;   // units in glPolygonOffset
		float		depth_bias_slope	= 0.0f;   // factor in glPolygonOffset
		float		line_width		= 1.0f;

		bool operator==(const VulkanRasterizerKey &o) const
		{
			return std::memcmp(this, &o, sizeof(*this)) == 0;
		}

		void fill_from(const RasterizerStateDescription &desc)
		{
			cull_mode	= vk_cull_mode(desc.get_culled(), desc.get_face_cull_mode());
			front_face   = (desc.get_front_face() == FaceSide::counter_clockwise)
							? VK_FRONT_FACE_COUNTER_CLOCKWISE
							: VK_FRONT_FACE_CLOCKWISE;
			polygon_mode = vk_polygon_mode(desc.get_face_fill_mode());
			scissor_test = desc.get_enable_scissor() ? VK_TRUE : VK_FALSE;

			// Any of offset_point / offset_line / offset_fill enables depth bias
			depth_bias_enable = (desc.get_offset_point() ||
								desc.get_offset_line()  ||
								desc.get_offset_fill()) ? VK_TRUE : VK_FALSE;
			if (depth_bias_enable)
			{
				float factor = 0.0f, units = 0.0f;
				desc.get_polygon_offset(factor, units);
				depth_bias_slope	= factor;
				depth_bias_constant = units;
			}
			else
			{
				depth_bias_slope	= 0.0f;
				depth_bias_constant = 0.0f;
			}
			line_width = 1.0f;
		}

		void apply_to(VkPipelineRasterizationStateCreateInfo &rs) const
		{
			rs = {};
			rs.sType				= VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			rs.depthClampEnable		= depth_clamp;
			rs.rasterizerDiscardEnable = VK_FALSE;
			rs.polygonMode			= polygon_mode;
			rs.cullMode				= cull_mode;
			rs.frontFace			= front_face;
			rs.depthBiasEnable		= depth_bias_enable;
			rs.depthBiasConstantFactor = depth_bias_constant;
			rs.depthBiasClamp		= 0.0f;
			rs.depthBiasSlopeFactor	= depth_bias_slope;
			rs.lineWidth			= line_width;
		}
	};

} // namespace clan
