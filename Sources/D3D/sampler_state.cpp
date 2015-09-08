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
#include "sampler_state.h"
#include "API/D3D/d3d_target.h"
#include <float.h>

namespace clan
{
	SamplerState::SamplerState(const ComPtr<ID3D11Device> &device)
		: device(device), max_anisotropy(0.0f), compare_mode(comparemode_none), min_filter(filter_linear_mipmap_linear), mag_filter(filter_linear)
	{
		sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.MipLODBias = 0.0f;
		sampler_desc.MinLOD = -FLT_MAX;
		sampler_desc.MaxLOD = FLT_MAX;
		sampler_desc.MaxAnisotropy = 16;
		sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampler_desc.BorderColor[0] = 0.0f;
		sampler_desc.BorderColor[1] = 0.0f;
		sampler_desc.BorderColor[2] = 0.0f;
		sampler_desc.BorderColor[3] = 0.0f;

		// Modify state to same defaults used by OpenGL target:
		set_min_filter(filter_linear);
		set_mag_filter(filter_linear);
		set_wrap_mode(wrap_clamp_to_edge, wrap_clamp_to_edge, wrap_clamp_to_edge);
	}

	SamplerState::~SamplerState()
	{
	}

	void SamplerState::set_min_lod(double min_lod)
	{
		if (sampler_desc.MinLOD != (float)min_lod)
		{
			sampler_desc.MinLOD = (float)min_lod;
			sampler_state_changed();
		}
	}

	void SamplerState::set_max_lod(double max_lod)
	{
		if (sampler_desc.MaxLOD != (float)max_lod)
		{
			sampler_desc.MaxLOD = (float)max_lod;
			sampler_state_changed();
		}
	}

	void SamplerState::set_lod_bias(double lod_bias)
	{
		if (sampler_desc.MipLODBias != (double)lod_bias)
		{
			sampler_desc.MipLODBias = (double)lod_bias;
			sampler_state_changed();
		}
	}

	void SamplerState::set_border_color(float r, float g, float b, float a)
	{
		if (sampler_desc.BorderColor[0] != r || sampler_desc.BorderColor[1] != g || sampler_desc.BorderColor[2] != b || sampler_desc.BorderColor[3] != a)
		{
			sampler_desc.BorderColor[0] = r;
			sampler_desc.BorderColor[1] = g;
			sampler_desc.BorderColor[2] = b;
			sampler_desc.BorderColor[3] = a;
			sampler_state_changed();
		}
	}

	void SamplerState::set_wrap_mode(TextureWrapMode wrap_s, TextureWrapMode wrap_t, TextureWrapMode wrap_r)
	{
		D3D11_TEXTURE_ADDRESS_MODE mode_u = to_d3d_texture_address(wrap_s);
		D3D11_TEXTURE_ADDRESS_MODE mode_v = to_d3d_texture_address(wrap_t);
		D3D11_TEXTURE_ADDRESS_MODE mode_w = to_d3d_texture_address(wrap_r);
		if (mode_u != sampler_desc.AddressU || mode_v != sampler_desc.AddressV || mode_w != sampler_desc.AddressW)
		{
			sampler_desc.AddressU = mode_u;
			sampler_desc.AddressV = mode_v;
			sampler_desc.AddressW = mode_w;
			sampler_state_changed();
		}
	}

	void SamplerState::set_wrap_mode(TextureWrapMode wrap_s, TextureWrapMode wrap_t)
	{
		D3D11_TEXTURE_ADDRESS_MODE mode_u = to_d3d_texture_address(wrap_s);
		D3D11_TEXTURE_ADDRESS_MODE mode_v = to_d3d_texture_address(wrap_t);
		if (mode_u != sampler_desc.AddressU || mode_v != sampler_desc.AddressV)
		{
			sampler_desc.AddressU = mode_u;
			sampler_desc.AddressV = mode_v;
			sampler_state_changed();
		}
	}

	void SamplerState::set_wrap_mode(TextureWrapMode wrap_s)
	{
		D3D11_TEXTURE_ADDRESS_MODE mode_u = to_d3d_texture_address(wrap_s);
		if (mode_u != sampler_desc.AddressU)
		{
			sampler_desc.AddressU = mode_u;
			sampler_state_changed();
		}
	}

	void SamplerState::set_min_filter(TextureFilter filter)
	{
		if (min_filter != filter)
		{
			min_filter = filter;
			sampler_desc.Filter = to_d3d_filter(min_filter, mag_filter, compare_mode, max_anisotropy);
			sampler_state_changed();
		}
	}

	void SamplerState::set_mag_filter(TextureFilter filter)
	{
		if (mag_filter != filter)
		{
			mag_filter = filter;
			sampler_desc.Filter = to_d3d_filter(min_filter, mag_filter, compare_mode, max_anisotropy);
			sampler_state_changed();
		}
	}

	void SamplerState::set_max_anisotropy(float v)
	{
		if (max_anisotropy != v)
		{
			max_anisotropy = v;
			sampler_desc.Filter = to_d3d_filter(min_filter, mag_filter, compare_mode, max_anisotropy);
			sampler_desc.MaxAnisotropy = clamp((int)v, 1, 16);
			sampler_state_changed();
		}
	}

	void SamplerState::set_texture_compare(TextureCompareMode mode, CompareFunction func)
	{
		D3D11_COMPARISON_FUNC comp_func = to_d3d_compare_func(func);
		if (compare_mode != mode || sampler_desc.ComparisonFunc != comp_func)
		{
			compare_mode = mode;
			sampler_desc.Filter = to_d3d_filter(min_filter, mag_filter, compare_mode, max_anisotropy);
			sampler_desc.ComparisonFunc = comp_func;
			sampler_state_changed();
		}
	}

	ComPtr<ID3D11SamplerState> &SamplerState::get_sampler_state()
	{
		if (!sampler_state)
		{
			std::map<D3D11_SAMPLER_DESC, ComPtr<ID3D11SamplerState>, D3D11SamplerDescLess>::iterator it = state_cache.find(sampler_desc);
			if (it != state_cache.end())
			{
				sampler_state = it->second;
			}
			else
			{
				HRESULT result = device->CreateSamplerState(&sampler_desc, sampler_state.output_variable());
				D3DTarget::throw_if_failed("CreateSamplerState failed!", result);
				state_cache[sampler_desc] = sampler_state;
			}
		}
		return sampler_state;
	}

	void SamplerState::sampler_state_changed()
	{
		sampler_state.clear();
	}

	bool SamplerState::uses_mipmaps() const
	{
		if (max_anisotropy != 0.0f)
			return true;
		switch (min_filter)
		{
		default:
		case filter_nearest:
		case filter_linear:
			return false;
		case filter_nearest_mipmap_nearest:
		case filter_nearest_mipmap_linear:
		case filter_linear_mipmap_nearest:
		case filter_linear_mipmap_linear:
			return true;
		}
	}

	D3D11_FILTER SamplerState::to_d3d_filter(TextureFilter min_filter, TextureFilter mag_filter, TextureCompareMode compare_mode, float max_anisotropy)
	{
		if (max_anisotropy != 0.0f)
			return compare_mode == comparemode_compare_r_to_texture ? D3D11_FILTER_COMPARISON_ANISOTROPIC : D3D11_FILTER_ANISOTROPIC;

		// 0 = nearest min, nearest mag, nearest level
		// 0x01 = linear level
		// 0x04 = linear magnification
		// 0x10 = linear min
		// 0x80 = compare R to texture

		unsigned int filter = 0;

		switch (min_filter)
		{
		case filter_nearest: break;
		case filter_linear: filter = 0x10; break;
		case filter_nearest_mipmap_nearest: break;
		case filter_nearest_mipmap_linear: filter = 0x01; break;
		case filter_linear_mipmap_nearest: filter = 0x10; break;
		case filter_linear_mipmap_linear: filter = 0x11; break;
		}

		if (mag_filter == filter_linear)
			filter |= 0x04;

		if (compare_mode == comparemode_compare_r_to_texture)
			filter |= 0x80;

		return (D3D11_FILTER)filter;
	}

	D3D11_TEXTURE_ADDRESS_MODE SamplerState::to_d3d_texture_address(TextureWrapMode wrap)
	{
		switch (wrap)
		{
		case wrap_clamp_to_edge: return D3D11_TEXTURE_ADDRESS_CLAMP;
		case wrap_repeat: return D3D11_TEXTURE_ADDRESS_WRAP;
		case wrap_mirrored_repeat: return D3D11_TEXTURE_ADDRESS_MIRROR;
		}
		throw Exception("Unsupported wrap mode");
	}

	D3D11_COMPARISON_FUNC SamplerState::to_d3d_compare_func(CompareFunction func)
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
