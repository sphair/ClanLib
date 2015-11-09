/*
**  ClanLib SDK
**  Copyright (c) 1997-2015 The ClanLib Team
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
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Magnus Norddahl
*/

#include "D3D/precomp.h"
#include "d3d_depth_stencil_state_provider.h"
#include "API/D3D/d3d_target.h"

namespace clan
{
	D3DDepthStencilStateProvider::D3DDepthStencilStateProvider(const ComPtr<ID3D11Device> &device, const DepthStencilStateDescription &desc)
	{
		CompareFunction front, back;
		int front_ref, front_mask, back_ref, back_mask;
		desc.get_stencil_compare_front(front, front_ref, front_mask);
		desc.get_stencil_compare_back(back, back_ref, back_mask);

		unsigned char front_write_mask, back_write_mask;
		desc.get_stencil_write_mask(front_write_mask, back_write_mask);

		StencilOp fail_front, pass_depth_fail_front, pass_depth_pass_front;
		StencilOp fail_back, pass_depth_fail_back, pass_depth_pass_back;
		desc.get_stencil_op_front(fail_front, pass_depth_fail_front, pass_depth_pass_front);
		desc.get_stencil_op_back(fail_back, pass_depth_fail_back, pass_depth_pass_back);

		D3D11_DEPTH_STENCIL_DESC d3d_desc;
		d3d_desc.DepthEnable = desc.is_depth_test_enabled() ? TRUE : FALSE;
		d3d_desc.DepthWriteMask = desc.is_depth_write_enabled() ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
		d3d_desc.DepthFunc = to_d3d_compare_func(desc.get_depth_compare_function());
		d3d_desc.StencilEnable = desc.is_stencil_test_enabled();
		d3d_desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK; // To do: should this use the write masks instead?
		d3d_desc.StencilWriteMask = front_write_mask | back_write_mask;
		d3d_desc.FrontFace.StencilFunc = to_d3d_compare_func(front);
		d3d_desc.FrontFace.StencilDepthFailOp = to_d3d_stencil_op(pass_depth_fail_front);
		d3d_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		d3d_desc.FrontFace.StencilFailOp = to_d3d_stencil_op(fail_front);
		d3d_desc.BackFace.StencilFunc = to_d3d_compare_func(back);
		d3d_desc.BackFace.StencilDepthFailOp = to_d3d_stencil_op(pass_depth_fail_back);
		d3d_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		d3d_desc.BackFace.StencilFailOp = to_d3d_stencil_op(fail_back);

		HRESULT result = device->CreateDepthStencilState(&d3d_desc, state.output_variable());
		D3DTarget::throw_if_failed("D3D11Device.CreateDepthStencilState failed", result);

		// "The formats that support stenciling are DXGI_FORMAT_D24_UNORM_S8_UINT and DXGI_FORMAT_D32_FLOAT_S8X24_UINT."
	}

	D3D11_STENCIL_OP D3DDepthStencilStateProvider::to_d3d_stencil_op(StencilOp stencil_op)
	{
		switch (stencil_op)
		{
		case stencil_keep: return D3D11_STENCIL_OP_KEEP;
		case stencil_zero: return D3D11_STENCIL_OP_ZERO;
		case stencil_replace: return D3D11_STENCIL_OP_REPLACE;
		case stencil_incr: return D3D11_STENCIL_OP_INCR_SAT;
		case stencil_decr: return D3D11_STENCIL_OP_DECR_SAT;
		case stencil_invert: return D3D11_STENCIL_OP_INVERT;
		case stencil_incr_wrap: return D3D11_STENCIL_OP_INCR;
		case stencil_decr_wrap: return D3D11_STENCIL_OP_DECR;
		}
		throw Exception("Unsupported stencil operation");
	}

	D3D11_COMPARISON_FUNC D3DDepthStencilStateProvider::to_d3d_compare_func(CompareFunction func)
	{
		switch (func)
		{
		case compare_lequal: return D3D11_COMPARISON_LESS_EQUAL;
		case compare_gequal: return D3D11_COMPARISON_GREATER_EQUAL;
		case compare_less: return D3D11_COMPARISON_LESS;
		case compare_greater: return D3D11_COMPARISON_GREATER;
		case compare_equal: return D3D11_COMPARISON_EQUAL;
		case compare_notequal: return D3D11_COMPARISON_NOT_EQUAL;
		case compare_always: return D3D11_COMPARISON_ALWAYS;
		case compare_never: return D3D11_COMPARISON_NEVER;
		}
		throw Exception("Unsupported compare function");
	}
}
