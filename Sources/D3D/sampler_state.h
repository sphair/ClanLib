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

#pragma once

#include "API/Display/TargetProviders/texture_provider.h"
#include <map>

namespace clan
{
	class SamplerState
	{
	public:
		SamplerState(const ComPtr<ID3D11Device> &device);
		~SamplerState();

		void set_min_lod(double min_lod);
		void set_max_lod(double max_lod);
		void set_lod_bias(double lod_bias);
		void set_border_color(float r, float g, float b, float a);

		void set_wrap_mode(
			TextureWrapMode wrap_s,
			TextureWrapMode wrap_t,
			TextureWrapMode wrap_r);

		void set_wrap_mode(
			TextureWrapMode wrap_s,
			TextureWrapMode wrap_t);

		void set_wrap_mode(
			TextureWrapMode wrap_s);

		void set_min_filter(TextureFilter filter);
		void set_mag_filter(TextureFilter filter);
		void set_max_anisotropy(float v);
		void set_texture_compare(TextureCompareMode mode, CompareFunction func);

		ComPtr<ID3D11SamplerState> &get_sampler_state();

		bool uses_mipmaps() const;

	private:
		void sampler_state_changed();
		void update_filter();
		static D3D11_FILTER to_d3d_filter(TextureFilter min_filter, TextureFilter mag_filter, TextureCompareMode compare_mode, float max_anisotropy);
		static D3D11_TEXTURE_ADDRESS_MODE to_d3d_texture_address(TextureWrapMode wrap);
		static D3D11_COMPARISON_FUNC to_d3d_compare_func(CompareFunction func);

		ComPtr<ID3D11Device> device;
		D3D11_SAMPLER_DESC sampler_desc;
		ComPtr<ID3D11SamplerState> sampler_state;
		float max_anisotropy;
		TextureCompareMode compare_mode;
		TextureFilter min_filter, mag_filter;

		struct D3D11SamplerDescLess
		{
			bool operator()(const D3D11_SAMPLER_DESC &a, const D3D11_SAMPLER_DESC &b) const
			{
				return memcmp(&a, &b, sizeof(D3D11_SAMPLER_DESC)) < 0;
			}
		};

		std::map<D3D11_SAMPLER_DESC, ComPtr<ID3D11SamplerState>, D3D11SamplerDescLess> state_cache;
	};
}
