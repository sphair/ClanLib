/*
**  ClanLib SDK
**  Copyright (c) 1997-2016 The ClanLib Team
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
#include "d3d_blend_state_provider.h"
#include "API/D3D/d3d_target.h"

namespace clan
{
	D3DBlendStateProvider::D3DBlendStateProvider(const ComPtr<ID3D11Device> &device, const BlendStateDescription &desc)
	{
		BlendEquation blend_op_color, blend_op_alpha;
		desc.get_blend_equation(blend_op_color, blend_op_alpha);

		BlendFunc src_blend, dest_blend, src_blend_alpha, dest_blend_alpha;
		desc.get_blend_function(src_blend, dest_blend, src_blend_alpha, dest_blend_alpha);

		bool red, green, blue, alpha;
		desc.get_color_write(red, green, blue, alpha);

		D3D11_BLEND_DESC d3d_desc;
		d3d_desc.AlphaToCoverageEnable = FALSE;
		d3d_desc.IndependentBlendEnable = FALSE;
		for (int i = 0; i < 8; i++)
		{
			d3d_desc.RenderTarget[i].BlendEnable = FALSE;
			d3d_desc.RenderTarget[i].SrcBlend = D3D11_BLEND_ONE;
			d3d_desc.RenderTarget[i].DestBlend = D3D11_BLEND_ZERO;
			d3d_desc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
			d3d_desc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
			d3d_desc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ZERO;
			d3d_desc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			d3d_desc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		}

		d3d_desc.RenderTarget[0].BlendEnable = desc.is_blending_enabled() ? TRUE : FALSE;
		d3d_desc.RenderTarget[0].BlendOp = to_d3d_blend_op(blend_op_color);
		d3d_desc.RenderTarget[0].BlendOpAlpha = to_d3d_blend_op(blend_op_alpha);
		d3d_desc.RenderTarget[0].SrcBlend = to_d3d_blend_func(src_blend);
		d3d_desc.RenderTarget[0].DestBlend = to_d3d_blend_func(dest_blend);
		d3d_desc.RenderTarget[0].SrcBlendAlpha = to_d3d_blend_func(src_blend_alpha);
		d3d_desc.RenderTarget[0].DestBlendAlpha = to_d3d_blend_func(dest_blend_alpha);
		d3d_desc.RenderTarget[0].RenderTargetWriteMask = 0;
		if (red)
			d3d_desc.RenderTarget[0].RenderTargetWriteMask |= D3D11_COLOR_WRITE_ENABLE_RED;
		if (green)
			d3d_desc.RenderTarget[0].RenderTargetWriteMask |= D3D11_COLOR_WRITE_ENABLE_GREEN;
		if (blue)
			d3d_desc.RenderTarget[0].RenderTargetWriteMask |= D3D11_COLOR_WRITE_ENABLE_BLUE;
		if (alpha)
			d3d_desc.RenderTarget[0].RenderTargetWriteMask |= D3D11_COLOR_WRITE_ENABLE_ALPHA;

		HRESULT result = device->CreateBlendState(&d3d_desc, state.output_variable());
		D3DTarget::throw_if_failed("D3D11Device.CreateBlendState failed", result);
	}

	D3D11_BLEND_OP D3DBlendStateProvider::to_d3d_blend_op(BlendEquation op)
	{
		switch (op)
		{
		case BlendEquation::add: return D3D11_BLEND_OP_ADD;
		case BlendEquation::subtract: return D3D11_BLEND_OP_SUBTRACT;
		case BlendEquation::reverse_subtract: return D3D11_BLEND_OP_REV_SUBTRACT;
		case BlendEquation::min: return D3D11_BLEND_OP_MIN;
		case BlendEquation::max: return D3D11_BLEND_OP_MAX;
		}
		throw Exception("Unsupported blend op");
	}

	D3D11_BLEND D3DBlendStateProvider::to_d3d_blend_func(BlendFunc func)
	{
		switch (func)
		{
		case BlendFunc::zero: return D3D11_BLEND_ZERO;
		case BlendFunc::one: return D3D11_BLEND_ONE;
		case BlendFunc::dest_color: return D3D11_BLEND_DEST_COLOR;
		case BlendFunc::src_color: return D3D11_BLEND_SRC_COLOR;
		case BlendFunc::one_minus_dest_color: return D3D11_BLEND_INV_DEST_COLOR;
		case BlendFunc::one_minus_src_color: return D3D11_BLEND_INV_SRC_COLOR;
		case BlendFunc::src_alpha: return D3D11_BLEND_SRC_ALPHA;
		case BlendFunc::one_minus_src_alpha: return D3D11_BLEND_INV_SRC_ALPHA;
		case BlendFunc::dest_alpha: return D3D11_BLEND_DEST_ALPHA;
		case BlendFunc::one_minus_dest_alpha: return D3D11_BLEND_INV_DEST_ALPHA;
		case BlendFunc::src_alpha_saturate: return D3D11_BLEND_SRC_ALPHA_SAT;
		case BlendFunc::constant_color: return D3D11_BLEND_BLEND_FACTOR;
		case BlendFunc::one_minus_constant_color: return D3D11_BLEND_INV_BLEND_FACTOR;
		case BlendFunc::constant_alpha: break;
		case BlendFunc::one_minus_constant_alpha: break;
		}
		throw Exception("Unsupported blend func");
	}
}
