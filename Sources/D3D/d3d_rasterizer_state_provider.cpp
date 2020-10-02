/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
#include "d3d_rasterizer_state_provider.h"
#include "API/D3D/d3d_target.h"

namespace clan
{
	D3DRasterizerStateProvider::D3DRasterizerStateProvider(const ComPtr<ID3D11Device> &device, const RasterizerStateDescription &desc)
	{
		D3D11_RASTERIZER_DESC d3d_desc;
		d3d_desc.FrontCounterClockwise = (desc.get_front_face() == FaceSide::counter_clockwise) ? TRUE : FALSE;
		d3d_desc.DepthBias = 0;
		d3d_desc.SlopeScaledDepthBias = 0.0f;
		d3d_desc.DepthBiasClamp = 0.0f;
		d3d_desc.ScissorEnable = desc.get_enable_scissor() ? TRUE : FALSE;
		d3d_desc.MultisampleEnable = FALSE;
		d3d_desc.AntialiasedLineEnable = desc.get_enable_line_antialiasing() ? TRUE : FALSE;

		if (desc.get_culled())
		{
			switch (desc.get_face_cull_mode())
			{
			case CullMode::front: d3d_desc.CullMode = D3D11_CULL_FRONT; break;
			case CullMode::back: d3d_desc.CullMode = D3D11_CULL_BACK; break;
			case CullMode::front_and_back: d3d_desc.CullMode = D3D11_CULL_NONE; break;
			}
		}
		else
		{
			d3d_desc.CullMode = D3D11_CULL_NONE;
		}

		switch (desc.get_face_fill_mode())
		{
		case FillMode::point: throw Exception("Point fill mode not supported by D3D target");
		case FillMode::line: d3d_desc.FillMode = D3D11_FILL_WIREFRAME; break;
		case FillMode::polygon: d3d_desc.FillMode = D3D11_FILL_SOLID; break;
		}

		HRESULT result = device->CreateRasterizerState(&d3d_desc, state.output_variable());
		D3DTarget::throw_if_failed("D3D11Device.CreateRasterizerState failed", result);
	}
}
