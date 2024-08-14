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

#pragma once

#include "API/Display/TargetProviders/texture_provider.h"
#include "API/Display/Image/pixel_buffer.h"
#include "d3d_share_list.h"
#include "sampler_state.h"
#include "d3d_texture_data.h"

namespace clan
{
	class D3DTextureProvider : public TextureProvider, D3DSharedResource
	{
	public:
		D3DTextureProvider(const ComPtr<ID3D11Device> &device, D3D_FEATURE_LEVEL feature_level, TextureDimensions texture_dimensions);
		D3DTextureProvider(D3DTextureProvider *orig_texture, TextureDimensions texture_dimensions, TextureFormat texture_format, int min_level, int num_levels, int min_layer, int num_layers);
		~D3DTextureProvider() override;

		static bool is_stencil_or_depth_format(TextureFormat format);
		static DXGI_FORMAT to_d3d_format(TextureFormat format);
		static TextureFormat from_d3d_format(DXGI_FORMAT d3d_format);
		static int get_bytes_per_pixel(DXGI_FORMAT d3d_format);

		ComPtr<ID3D11SamplerState> &get_sampler_state(const ComPtr<ID3D11Device> &device);
		ID3D11Texture1D *get_texture_1d(const ComPtr<ID3D11Device> &device);
		ID3D11Texture2D *get_texture_2d(const ComPtr<ID3D11Device> &device);
		ID3D11Texture3D *get_texture_3d(const ComPtr<ID3D11Device> &device);
		ComPtr<ID3D11ShaderResourceView> &get_srv(const ComPtr<ID3D11Device> &device);
		ComPtr<ID3D11UnorderedAccessView> &get_uav(const ComPtr<ID3D11Device> &device);

		void generate_mipmap() override;
		void create(int width, int height, int depth, int array_size, TextureFormat texture_format, int levels) override;
		PixelBuffer get_pixeldata(GraphicContext &gc, TextureFormat texture_format, int level) const override;

		void copy_from(GraphicContext &gc, int x, int y, int slice, int level, const PixelBuffer &src, const Rect &src_rect) override;

		void copy_image_from(
			int x,
			int y,
			int width,
			int height,
			int level,
			TextureFormat texture_format,
			GraphicContextProvider *gc) override;

		void copy_subimage_from(
			int offset_x,
			int offset_y,
			int x,
			int y,
			int width,
			int height,
			int level,
			GraphicContextProvider *gc) override;

		void set_min_lod(double min_lod) override;
		void set_max_lod(double max_lod) override;
		void set_lod_bias(double lod_bias) override;
		void set_base_level(int base_level) override;
		void set_max_level(int max_level) override;

		void set_wrap_mode(
			TextureWrapMode wrap_s,
			TextureWrapMode wrap_t,
			TextureWrapMode wrap_r) override;

		void set_wrap_mode(
			TextureWrapMode wrap_s,
			TextureWrapMode wrap_t) override;

		void set_wrap_mode(
			TextureWrapMode wrap_s) override;

		void set_min_filter(TextureFilter filter) override;
		void set_mag_filter(TextureFilter filter) override;
		void set_max_anisotropy(float v) override;
		void set_texture_compare(TextureCompareMode mode, CompareFunction func) override;

		std::unique_ptr<TextureProvider> create_view(TextureDimensions texture_dimensions, TextureFormat texture_format, int min_level, int num_levels, int min_layer, int num_layers) override;

		ComPtr<ID3D11RenderTargetView> create_rtv(const ComPtr<ID3D11Device> &device, int level, int slice, TextureSubtype subtype);
		ComPtr<ID3D11DepthStencilView> create_dsv(const ComPtr<ID3D11Device> &device, int level, int slice, TextureSubtype subtype);

	private:
		void create_1d(int width, int height, int depth, int array_size, TextureFormat texture_format, int levels);
		void create_2d(int width, int height, int depth, int array_size, TextureFormat texture_format, int levels);
		void create_3d(int width, int height, int depth, int array_size, TextureFormat texture_format, int levels);

		struct ViewHandles
		{
			ViewHandles(const ComPtr<ID3D11Device> &device) : device(device), sampler_state(device) { }

			ComPtr<ID3D11Device> device;
			ComPtr<ID3D11ShaderResourceView> srv_base, srv_mipmap;
			ComPtr<ID3D11UnorderedAccessView> uav;
			SamplerState sampler_state;
		};

		void device_destroyed(ID3D11Device *device) override;
		ViewHandles &get_view_handles(const ComPtr<ID3D11Device> &device) const;

		std::shared_ptr<D3DTextureData> data;
		mutable std::vector<std::shared_ptr<ViewHandles> > view_handles;

		int view_min_layer;
	};
}
