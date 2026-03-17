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
#include "API/Display/Render/depth_stencil_state_description.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"

namespace clan
{
	// =========================================================================
	//  VulkanDepthStencilStateProvider
	// =========================================================================
	class VulkanDepthStencilStateProvider : public DepthStencilStateProvider
	{
	public:
		explicit VulkanDepthStencilStateProvider(const DepthStencilStateDescription &d)
			: desc(d.clone()) {}
		~VulkanDepthStencilStateProvider() override = default;

		DepthStencilStateDescription desc;
	};

	// =========================================================================
	//  Enum converters
	// =========================================================================

	// Replaces: glDepthFunc / glStencilFuncSeparate compare-function argument
	inline VkCompareOp vk_compare_op(CompareFunction f)
	{
		switch (f)
		{
		case CompareFunction::never:	return VK_COMPARE_OP_NEVER;
		case CompareFunction::less:	return VK_COMPARE_OP_LESS;
		case CompareFunction::lequal:   return VK_COMPARE_OP_LESS_OR_EQUAL;
		case CompareFunction::greater:  return VK_COMPARE_OP_GREATER;
		case CompareFunction::gequal:   return VK_COMPARE_OP_GREATER_OR_EQUAL;
		case CompareFunction::equal:	return VK_COMPARE_OP_EQUAL;
		case CompareFunction::notequal: return VK_COMPARE_OP_NOT_EQUAL;
		case CompareFunction::always:   return VK_COMPARE_OP_ALWAYS;
		default:						return VK_COMPARE_OP_ALWAYS;
		}
	}

	// Replaces: glStencilOpSeparate stencil-op arguments
	inline VkStencilOp vk_stencil_op(StencilOp op)
	{
		switch (op)
		{
		case StencilOp::keep:	return VK_STENCIL_OP_KEEP;
		case StencilOp::zero:	return VK_STENCIL_OP_ZERO;
		case StencilOp::replace:   return VK_STENCIL_OP_REPLACE;
		case StencilOp::incr:	return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
		case StencilOp::decr:	return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
		case StencilOp::invert:	return VK_STENCIL_OP_INVERT;
		case StencilOp::incr_wrap: return VK_STENCIL_OP_INCREMENT_AND_WRAP;
		case StencilOp::decr_wrap: return VK_STENCIL_OP_DECREMENT_AND_WRAP;
		default:				return VK_STENCIL_OP_KEEP;
		}
	}

	// =========================================================================
	//  VulkanDepthStencilKey
	// =========================================================================
	struct VulkanDepthStencilKey
	{
		VkBool32	depth_test			= VK_FALSE;
		VkBool32	depth_write		= VK_TRUE;
		VkCompareOp   depth_func			= VK_COMPARE_OP_LESS;

		VkBool32	stencil_test		= VK_FALSE;

		VkCompareOp   front_func			= VK_COMPARE_OP_ALWAYS;
		uint32_t	front_compare_mask	= 0xFF;
		uint32_t	front_write_mask	= 0xFF;
		VkStencilOp   front_fail			= VK_STENCIL_OP_KEEP;
		VkStencilOp   front_depth_fail	= VK_STENCIL_OP_KEEP;
		VkStencilOp   front_pass			= VK_STENCIL_OP_KEEP;

		VkCompareOp   back_func			= VK_COMPARE_OP_ALWAYS;
		uint32_t	back_compare_mask	= 0xFF;
		uint32_t	back_write_mask	= 0xFF;
		VkStencilOp   back_fail			= VK_STENCIL_OP_KEEP;
		VkStencilOp   back_depth_fail	= VK_STENCIL_OP_KEEP;
		VkStencilOp   back_pass			= VK_STENCIL_OP_KEEP;

		bool operator==(const VulkanDepthStencilKey &o) const
		{
			return std::memcmp(this, &o, sizeof(*this)) == 0;
		}

		void fill_from(const DepthStencilStateDescription &desc)
		{
			depth_test  = desc.is_depth_test_enabled()  ? VK_TRUE : VK_FALSE;
			depth_write = desc.is_depth_write_enabled() ? VK_TRUE : VK_FALSE;
			depth_func  = vk_compare_op(desc.get_depth_compare_function());

			stencil_test = desc.is_stencil_test_enabled() ? VK_TRUE : VK_FALSE;

			CompareFunction f_func; int f_ref; int f_mask;
			CompareFunction b_func; int b_ref; int b_mask;
			unsigned char   f_write, b_write;
			StencilOp	f_fail, f_dfail, f_dpass;
			StencilOp	b_fail, b_dfail, b_dpass;

			desc.get_stencil_compare_front(f_func, f_ref, f_mask);
			desc.get_stencil_compare_back (b_func, b_ref, b_mask);
			desc.get_stencil_write_mask(f_write, b_write);
			desc.get_stencil_op_front(f_fail, f_dfail, f_dpass);
			desc.get_stencil_op_back (b_fail, b_dfail, b_dpass);

			front_func		= vk_compare_op(f_func);
			front_compare_mask = static_cast<uint32_t>(f_mask);
			front_write_mask   = static_cast<uint32_t>(f_write);
			front_fail		= vk_stencil_op(f_fail);
			front_depth_fail   = vk_stencil_op(f_dfail);
			front_pass		= vk_stencil_op(f_dpass);

			back_func		= vk_compare_op(b_func);
			back_compare_mask  = static_cast<uint32_t>(b_mask);
			back_write_mask	= static_cast<uint32_t>(b_write);
			back_fail		= vk_stencil_op(b_fail);
			back_depth_fail	= vk_stencil_op(b_dfail);
			back_pass		= vk_stencil_op(b_dpass);
		}

		void apply_to(VkPipelineDepthStencilStateCreateInfo &ds,
					uint32_t stencil_ref) const
		{
			ds = {};
			ds.sType				= VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
			ds.depthTestEnable	= depth_test;
			ds.depthWriteEnable	= depth_write;
			ds.depthCompareOp		= depth_func;
			ds.depthBoundsTestEnable = VK_FALSE;
			ds.stencilTestEnable	= stencil_test;

			ds.front.compareOp   = front_func;
			ds.front.compareMask = front_compare_mask;
			ds.front.writeMask   = front_write_mask;
			ds.front.reference   = stencil_ref;
			ds.front.failOp	= front_fail;
			ds.front.depthFailOp = front_depth_fail;
			ds.front.passOp	= front_pass;

			ds.back.compareOp	= back_func;
			ds.back.compareMask  = back_compare_mask;
			ds.back.writeMask	= back_write_mask;
			ds.back.reference	= stencil_ref;
			ds.back.failOp	= back_fail;
			ds.back.depthFailOp  = back_depth_fail;
			ds.back.passOp	= back_pass;
		}
	};

} // namespace clan
