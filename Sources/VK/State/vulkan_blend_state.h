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
#include "API/Display/Render/blend_state_description.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"

namespace clan
{
	// =========================================================================
	//  VulkanBlendStateProvider
	// =========================================================================
	class VulkanBlendStateProvider : public BlendStateProvider
	{
	public:
		explicit VulkanBlendStateProvider(const BlendStateDescription &d)
			: desc(d.clone()) {}
		~VulkanBlendStateProvider() override = default;

		BlendStateDescription desc;
	};

	// =========================================================================
	//  Enum converters
	// =========================================================================

	inline VkBlendFactor vk_blend_factor(BlendFunc f)
	{
		switch (f)
		{
		case BlendFunc::zero:					return VK_BLEND_FACTOR_ZERO;
		case BlendFunc::one:					return VK_BLEND_FACTOR_ONE;
		case BlendFunc::dest_color:			return VK_BLEND_FACTOR_DST_COLOR;
		case BlendFunc::src_color:				return VK_BLEND_FACTOR_SRC_COLOR;
		case BlendFunc::one_minus_dest_color:	return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
		case BlendFunc::src_alpha:				return VK_BLEND_FACTOR_SRC_ALPHA;
		case BlendFunc::one_minus_src_color:	return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
		case BlendFunc::dest_alpha:			return VK_BLEND_FACTOR_DST_ALPHA;
		case BlendFunc::one_minus_dest_alpha:	return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
		case BlendFunc::src_alpha_saturate:	return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
		case BlendFunc::constant_color:		return VK_BLEND_FACTOR_CONSTANT_COLOR;
		case BlendFunc::one_minus_constant_color: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
		case BlendFunc::one_minus_src_alpha:	return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		case BlendFunc::constant_alpha:		return VK_BLEND_FACTOR_CONSTANT_ALPHA;
		case BlendFunc::one_minus_constant_alpha: return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
		default:								return VK_BLEND_FACTOR_ONE;
		}
	}

	inline VkBlendOp vk_blend_op(BlendEquation eq)
	{
		switch (eq)
		{
		case BlendEquation::add:			return VK_BLEND_OP_ADD;
		case BlendEquation::subtract:		return VK_BLEND_OP_SUBTRACT;
		case BlendEquation::reverse_subtract: return VK_BLEND_OP_REVERSE_SUBTRACT;
		case BlendEquation::min:			return VK_BLEND_OP_MIN;
		case BlendEquation::max:			return VK_BLEND_OP_MAX;
		default:							return VK_BLEND_OP_ADD;
		}
	}

	inline VkLogicOp vk_logic_op(LogicOp op)
	{
		switch (op)
		{
		case LogicOp::_clear:		return VK_LOGIC_OP_CLEAR;
		case LogicOp::_and:		return VK_LOGIC_OP_AND;
		case LogicOp::_and_reverse:   return VK_LOGIC_OP_AND_REVERSE;
		case LogicOp::_copy:		return VK_LOGIC_OP_COPY;
		case LogicOp::_and_inverted:  return VK_LOGIC_OP_AND_INVERTED;
		case LogicOp::_noop:		return VK_LOGIC_OP_NO_OP;
		case LogicOp::_xor:		return VK_LOGIC_OP_XOR;
		case LogicOp::_or:		return VK_LOGIC_OP_OR;
		case LogicOp::_nor:		return VK_LOGIC_OP_NOR;
		case LogicOp::_equiv:		return VK_LOGIC_OP_EQUIVALENT;
		case LogicOp::_invert:		return VK_LOGIC_OP_INVERT;
		case LogicOp::_or_reverse:	return VK_LOGIC_OP_OR_REVERSE;
		case LogicOp::_copy_inverted: return VK_LOGIC_OP_COPY_INVERTED;
		case LogicOp::_or_inverted:   return VK_LOGIC_OP_OR_INVERTED;
		case LogicOp::_nand:		return VK_LOGIC_OP_NAND;
		case LogicOp::_set:		return VK_LOGIC_OP_SET;
		default:					return VK_LOGIC_OP_COPY;
		}
	}

	// =========================================================================
	//  VulkanBlendKey
	// =========================================================================
	struct VulkanBlendKey
	{
		VkBool32	blend_enable		= VK_FALSE;
		VkBlendFactor src_color			= VK_BLEND_FACTOR_ONE;
		VkBlendFactor dst_color			= VK_BLEND_FACTOR_ZERO;
		VkBlendOp	color_op			= VK_BLEND_OP_ADD;
		VkBlendFactor src_alpha			= VK_BLEND_FACTOR_ONE;
		VkBlendFactor dst_alpha			= VK_BLEND_FACTOR_ZERO;
		VkBlendOp	alpha_op			= VK_BLEND_OP_ADD;
		VkColorComponentFlags color_write   = VK_COLOR_COMPONENT_R_BIT |
											VK_COLOR_COMPONENT_G_BIT |
											VK_COLOR_COMPONENT_B_BIT |
											VK_COLOR_COMPONENT_A_BIT;
		VkBool32	logic_op_enable	= VK_FALSE;
		VkLogicOp	logic_op			= VK_LOGIC_OP_COPY;

		bool operator==(const VulkanBlendKey &o) const
		{
			return std::memcmp(this, &o, sizeof(*this)) == 0;
		}

		void fill_from(const BlendStateDescription &desc)
		{
			BlendEquation eq_col, eq_alpha;
			BlendFunc	src, dst, src_a, dst_a;
			desc.get_blend_equation(eq_col, eq_alpha);
			desc.get_blend_function(src, dst, src_a, dst_a);
			bool r, g, b, a;
			desc.get_color_write(r, g, b, a);

			blend_enable	= desc.is_blending_enabled() ? VK_TRUE : VK_FALSE;
			src_color	= vk_blend_factor(src);
			dst_color	= vk_blend_factor(dst);
			color_op		= vk_blend_op(eq_col);
			src_alpha	= vk_blend_factor(src_a);
			dst_alpha	= vk_blend_factor(dst_a);
			alpha_op		= vk_blend_op(eq_alpha);
			color_write	= (r ? VK_COLOR_COMPONENT_R_BIT : 0u) |
							(g ? VK_COLOR_COMPONENT_G_BIT : 0u) |
							(b ? VK_COLOR_COMPONENT_B_BIT : 0u) |
							(a ? VK_COLOR_COMPONENT_A_BIT : 0u);
			logic_op_enable = desc.is_logic_op_enabled() ? VK_TRUE : VK_FALSE;
			logic_op		= vk_logic_op(desc.get_logic_op());
		}

		VkPipelineColorBlendAttachmentState to_attachment_state() const
		{
			VkPipelineColorBlendAttachmentState s{};
			s.blendEnable		= blend_enable;
			s.srcColorBlendFactor = src_color;
			s.dstColorBlendFactor = dst_color;
			s.colorBlendOp		= color_op;
			s.srcAlphaBlendFactor = src_alpha;
			s.dstAlphaBlendFactor = dst_alpha;
			s.alphaBlendOp		= alpha_op;
			s.colorWriteMask	= color_write;
			return s;
		}
	};

} // namespace clan
