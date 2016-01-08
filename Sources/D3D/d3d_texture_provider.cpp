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
#include "d3d_texture_provider.h"
#include "d3d_pixel_buffer_provider.h"
#include "d3d_graphic_context_provider.h"
#include "d3d_display_window_provider.h"
#include "API/Display/Image/pixel_buffer.h"
#include "API/Display/Render/transfer_texture.h"
#include "API/D3D/d3d_target.h"

namespace clan
{
	D3DTextureProvider::D3DTextureProvider(const ComPtr<ID3D11Device> &device, D3D_FEATURE_LEVEL feature_level, TextureDimensions texture_dimensions)
	: data(new D3DTextureData(device, feature_level, texture_dimensions)), view_min_layer(-1)
	{
		view_handles.push_back(std::shared_ptr<ViewHandles>(new ViewHandles(device)));
	}

	D3DTextureProvider::D3DTextureProvider(D3DTextureProvider *orig_texture, TextureDimensions texture_dimensions, TextureFormat texture_format, int min_level, int num_levels, int min_layer, int num_layers)
	: data(orig_texture->data), view_min_layer(min_layer)
	{
		// To do: save and use all view parameters
		view_handles.push_back(std::shared_ptr<ViewHandles>(new ViewHandles(data->handles.front()->device)));
	}

	D3DTextureProvider::~D3DTextureProvider()
	{
	}

	ComPtr<ID3D11SamplerState> &D3DTextureProvider::get_sampler_state(const ComPtr<ID3D11Device> &device)
	{
		return get_view_handles(device).sampler_state.get_sampler_state();
	}

	ID3D11Texture1D *D3DTextureProvider::get_texture_1d(const ComPtr<ID3D11Device> &device)
	{
		if (device)
			return data->get_handles(device).get_texture_1d();
		else
			return data->handles.front()->get_texture_1d();
	}

	ID3D11Texture2D *D3DTextureProvider::get_texture_2d(const ComPtr<ID3D11Device> &device)
	{
		if (device)
			return data->get_handles(device).get_texture_2d();
		else
			return data->handles.front()->get_texture_2d();
	}

	ID3D11Texture3D *D3DTextureProvider::get_texture_3d(const ComPtr<ID3D11Device> &device)
	{
		if (device)
			return data->get_handles(device).get_texture_3d();
		else
			return data->handles.front()->get_texture_3d();
	}

	ComPtr<ID3D11ShaderResourceView> &D3DTextureProvider::get_srv(const ComPtr<ID3D11Device> &device)
	{
		D3DTextureData::DeviceHandles &data_handles = data->get_handles(device);
		ViewHandles &handles = get_view_handles(device);

		if (handles.sampler_state.uses_mipmaps())
		{
			// To do: use set_base_level, set_max_level to calculate MipLevels and MostDetailedMip

			if (!handles.srv_mipmap)
			{
				D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;

				if (data_handles.texture_type == D3DTextureData::DeviceTextureType::ID3D11Texture1D)
				{
					D3D11_TEXTURE1D_DESC texture_desc;
					data_handles.get_texture_1d()->GetDesc(&texture_desc);

					srv_desc.Format = texture_desc.Format;
					if (data->texture_dimensions == texture_1d)
					{
						srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
						srv_desc.Texture1D.MipLevels = -1;
						srv_desc.Texture1D.MostDetailedMip = 0;
					}
					else if (data->texture_dimensions == texture_1d_array)
					{
						srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1DARRAY;
						srv_desc.Texture1DArray.FirstArraySlice = 0;
						srv_desc.Texture1DArray.ArraySize = texture_desc.ArraySize;
						srv_desc.Texture1DArray.MipLevels = -1;
						srv_desc.Texture1DArray.MostDetailedMip = 0;

						if (view_min_layer != -1)
						{
							srv_desc.Texture1DArray.FirstArraySlice = view_min_layer;
							srv_desc.Texture1DArray.ArraySize = 1;
						}
					}

				}
				else if (data_handles.texture_type == D3DTextureData::DeviceTextureType::ID3D11Texture2D)
				{
					D3D11_TEXTURE2D_DESC texture_desc;
					data_handles.get_texture_2d()->GetDesc(&texture_desc);

					srv_desc.Format = texture_desc.Format;
					if (data->texture_dimensions == texture_2d)
					{
						srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
						srv_desc.Texture2D.MipLevels = -1;
						srv_desc.Texture2D.MostDetailedMip = 0;
					}
					else if (data->texture_dimensions == texture_2d_array)
					{
						srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
						srv_desc.Texture2DArray.FirstArraySlice = 0;
						srv_desc.Texture2DArray.ArraySize = texture_desc.ArraySize;
						srv_desc.Texture2DArray.MipLevels = -1;
						srv_desc.Texture2DArray.MostDetailedMip = 0;

						if (view_min_layer != -1)
						{
							srv_desc.Texture2DArray.FirstArraySlice = view_min_layer;
							srv_desc.Texture2DArray.ArraySize = 1;
						}
					}
					else if (data->texture_dimensions == texture_cube)
					{
						srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
						srv_desc.TextureCube.MipLevels = -1;
						srv_desc.TextureCube.MostDetailedMip = 0;
					}
					else if (data->texture_dimensions == texture_cube_array)
					{
						srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
						srv_desc.TextureCubeArray.First2DArrayFace = 0;
						srv_desc.TextureCubeArray.NumCubes = texture_desc.ArraySize / 6;
						srv_desc.TextureCubeArray.MipLevels = -1;
						srv_desc.TextureCubeArray.MostDetailedMip = 0;
					}
				}
				else
				{
					D3D11_TEXTURE3D_DESC texture_desc;
					data_handles.get_texture_3d()->GetDesc(&texture_desc);

					srv_desc.Format = texture_desc.Format;
					srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
					srv_desc.Texture3D.MipLevels = -1;
					srv_desc.Texture3D.MostDetailedMip = 0;
				}
				HRESULT result = device->CreateShaderResourceView(data_handles.texture, &srv_desc, handles.srv_mipmap.output_variable());
				D3DTarget::throw_if_failed("ID3D11Device.CreateShaderResourceView failed", result);
			}
			return handles.srv_mipmap;
		}
		else
		{
			if (!handles.srv_base)
			{
				D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;

				if (data_handles.texture_type == D3DTextureData::DeviceTextureType::ID3D11Texture1D)
				{
					D3D11_TEXTURE1D_DESC texture_desc;
					data_handles.get_texture_1d()->GetDesc(&texture_desc);

					srv_desc.Format = texture_desc.Format;
					if (data->texture_dimensions == texture_1d)
					{
						srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
						srv_desc.Texture1D.MipLevels = 1;
						srv_desc.Texture1D.MostDetailedMip = 0;
					}
					else if (data->texture_dimensions == texture_1d_array)
					{
						srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1DARRAY;
						srv_desc.Texture1DArray.FirstArraySlice = 0;
						srv_desc.Texture1DArray.ArraySize = texture_desc.ArraySize;
						srv_desc.Texture1DArray.MipLevels = 1;
						srv_desc.Texture1DArray.MostDetailedMip = 0;

						if (view_min_layer != -1)
						{
							srv_desc.Texture1DArray.FirstArraySlice = view_min_layer;
							srv_desc.Texture1DArray.ArraySize = 1;
						}
					}
				}
				else if (data_handles.texture_type == D3DTextureData::DeviceTextureType::ID3D11Texture2D)
				{
					D3D11_TEXTURE2D_DESC texture_desc;
					data_handles.get_texture_2d()->GetDesc(&texture_desc);

					srv_desc.Format = texture_desc.Format;
					if (data->texture_dimensions == texture_2d)
					{
						srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
						srv_desc.Texture2D.MipLevels = 1;
						srv_desc.Texture2D.MostDetailedMip = 0;
					}
					else if (data->texture_dimensions == texture_2d_array)
					{
						srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
						srv_desc.Texture2DArray.FirstArraySlice = 0;
						srv_desc.Texture2DArray.ArraySize = texture_desc.ArraySize;
						srv_desc.Texture2DArray.MipLevels = 1;
						srv_desc.Texture2DArray.MostDetailedMip = 0;

						if (view_min_layer != -1)
						{
							srv_desc.Texture2DArray.FirstArraySlice = view_min_layer;
							srv_desc.Texture2DArray.ArraySize = 1;
						}
					}
					else if (data->texture_dimensions == texture_cube)
					{
						srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
						srv_desc.TextureCube.MipLevels = 1;
						srv_desc.TextureCube.MostDetailedMip = 0;
					}
					else if (data->texture_dimensions == texture_cube_array)
					{
						srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
						srv_desc.TextureCubeArray.First2DArrayFace = 0;
						srv_desc.TextureCubeArray.NumCubes = texture_desc.ArraySize / 6;
						srv_desc.TextureCubeArray.MipLevels = 1;
						srv_desc.TextureCubeArray.MostDetailedMip = 0;
					}
				}
				else
				{
					D3D11_TEXTURE3D_DESC texture_desc;
					data_handles.get_texture_3d()->GetDesc(&texture_desc);

					srv_desc.Format = texture_desc.Format;
					srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
					srv_desc.Texture3D.MipLevels = 1;
					srv_desc.Texture3D.MostDetailedMip = 0;
				}

				HRESULT result = device->CreateShaderResourceView(data_handles.texture, &srv_desc, handles.srv_base.output_variable());
				D3DTarget::throw_if_failed("ID3D11Device.CreateShaderResourceView failed", result);
			}
			return handles.srv_base;
		}
	}

	ComPtr<ID3D11UnorderedAccessView> &D3DTextureProvider::get_uav(const ComPtr<ID3D11Device> &device)
	{
		D3DTextureData::DeviceHandles &data_handles = data->get_handles(device);
		ViewHandles &handles = get_view_handles(device);
		if (!handles.uav)
		{
			D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc;

			if (data_handles.texture_type == D3DTextureData::DeviceTextureType::ID3D11Texture1D)
			{
				D3D11_TEXTURE1D_DESC texture_desc;
				data_handles.get_texture_1d()->GetDesc(&texture_desc);

				uav_desc.Format = texture_desc.Format;
				if (data->texture_dimensions == texture_1d)
				{
					uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE1D;
					uav_desc.Texture1D.MipSlice = 0;
				}
				else if (data->texture_dimensions == texture_1d_array)
				{
					uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE1DARRAY;
					uav_desc.Texture1DArray.FirstArraySlice = 0;
					uav_desc.Texture1DArray.ArraySize = texture_desc.ArraySize;
					uav_desc.Texture1DArray.MipSlice = 0;

					if (view_min_layer != -1)
					{
						uav_desc.Texture1DArray.FirstArraySlice = view_min_layer;
						uav_desc.Texture1DArray.ArraySize = 1;
					}
				}

			}
			else if (data_handles.texture_type == D3DTextureData::DeviceTextureType::ID3D11Texture2D)
			{
				D3D11_TEXTURE2D_DESC texture_desc;
				data_handles.get_texture_2d()->GetDesc(&texture_desc);

				uav_desc.Format = texture_desc.Format;
				if (data->texture_dimensions == texture_2d)
				{
					uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
					uav_desc.Texture2D.MipSlice = 0;
				}
				else if (data->texture_dimensions == texture_2d_array)
				{
					uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
					uav_desc.Texture2DArray.FirstArraySlice = 0;
					uav_desc.Texture2DArray.ArraySize = texture_desc.ArraySize;
					uav_desc.Texture2DArray.MipSlice = 0;

					if (view_min_layer != -1)
					{
						uav_desc.Texture2DArray.FirstArraySlice = view_min_layer;
						uav_desc.Texture2DArray.ArraySize = 1;
					}
				}

			}
			else
			{
				D3D11_TEXTURE3D_DESC texture_desc;
				data_handles.get_texture_3d()->GetDesc(&texture_desc);

				uav_desc.Format = texture_desc.Format;
				uav_desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE3D;
				uav_desc.Texture3D.MipSlice = 0;
			}
			HRESULT result = device->CreateUnorderedAccessView(data_handles.texture, &uav_desc, handles.uav.output_variable());
			D3DTarget::throw_if_failed("ID3D11Device.CreateUnorderedAccessView failed", result);
		}
		return handles.uav;
	}

	ComPtr<ID3D11RenderTargetView> D3DTextureProvider::create_rtv(const ComPtr<ID3D11Device> &device, int level, int slice, TextureSubtype subtype)
	{
		D3DTextureData::DeviceHandles &data_handles = data->get_handles(device);

		D3D11_RENDER_TARGET_VIEW_DESC rtv_desc;

		if (data_handles.texture_type == D3DTextureData::DeviceTextureType::ID3D11Texture1D)
		{
			D3D11_TEXTURE1D_DESC texture_desc;
			data_handles.get_texture_1d()->GetDesc(&texture_desc);

			rtv_desc.Format = texture_desc.Format;
			if (data->texture_dimensions == texture_1d)
			{
				rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE1D;
				rtv_desc.Texture1D.MipSlice = level;
			}
			else if (data->texture_dimensions == texture_1d_array)
			{
				rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE1DARRAY;
				if (slice == -1)
				{
					rtv_desc.Texture1DArray.FirstArraySlice = 0;
					rtv_desc.Texture1DArray.ArraySize = texture_desc.ArraySize;

					if (view_min_layer != -1)
					{
						rtv_desc.Texture1DArray.FirstArraySlice = view_min_layer;
						rtv_desc.Texture1DArray.ArraySize = 1;
					}
				}
				else
				{
					rtv_desc.Texture1DArray.FirstArraySlice = slice;
					rtv_desc.Texture1DArray.ArraySize = 1;
				}
				rtv_desc.Texture1DArray.MipSlice = level;
			}

		}
		else if (data_handles.texture_type == D3DTextureData::DeviceTextureType::ID3D11Texture2D)
		{
			D3D11_TEXTURE2D_DESC texture_desc;
			data_handles.get_texture_2d()->GetDesc(&texture_desc);

			rtv_desc.Format = texture_desc.Format;
			if (data->texture_dimensions == texture_2d)
			{
				rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
				rtv_desc.Texture2D.MipSlice = level;
			}
			else if (data->texture_dimensions == texture_2d_array)
			{
				rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
				if (slice == -1)
				{
					rtv_desc.Texture2DArray.FirstArraySlice = 0;
					rtv_desc.Texture2DArray.ArraySize = texture_desc.ArraySize;

					if (view_min_layer != -1)
					{
						rtv_desc.Texture2DArray.FirstArraySlice = view_min_layer;
						rtv_desc.Texture2DArray.ArraySize = 1;
					}
				}
				else
				{
					rtv_desc.Texture2DArray.FirstArraySlice = slice;
					rtv_desc.Texture2DArray.ArraySize = 1;
				}
				rtv_desc.Texture2DArray.MipSlice = level;
			}

		}
		else
		{
			D3D11_TEXTURE3D_DESC texture_desc;
			data_handles.get_texture_3d()->GetDesc(&texture_desc);

			rtv_desc.Format = texture_desc.Format;
			rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE3D;
			rtv_desc.Texture3D.MipSlice = level;
		}

		ComPtr<ID3D11RenderTargetView> rtv;
		HRESULT result = device->CreateRenderTargetView(data_handles.texture, &rtv_desc, rtv.output_variable());
		D3DTarget::throw_if_failed("ID3D11Device.CreateRenderTargetView failed", result);
		return rtv;
	}

	ComPtr<ID3D11DepthStencilView> D3DTextureProvider::create_dsv(const ComPtr<ID3D11Device> &device, int level, int slice, TextureSubtype subtype)
	{
		D3DTextureData::DeviceHandles &data_handles = data->get_handles(device);
		D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc;

		if (data_handles.texture_type == D3DTextureData::DeviceTextureType::ID3D11Texture1D)
		{
			D3D11_TEXTURE1D_DESC texture_desc;
			data_handles.get_texture_1d()->GetDesc(&texture_desc);

			dsv_desc.Format = texture_desc.Format;
			if (data->texture_dimensions == texture_1d)
			{
				dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE1D;
				dsv_desc.Texture1D.MipSlice = level;
			}
			else if (data->texture_dimensions == texture_1d_array)
			{
				dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE1DARRAY;
				if (slice == -1)
				{
					dsv_desc.Texture1DArray.FirstArraySlice = 0;
					dsv_desc.Texture1DArray.ArraySize = texture_desc.ArraySize;
				}
				else
				{
					dsv_desc.Texture1DArray.FirstArraySlice = slice;
					dsv_desc.Texture1DArray.ArraySize = 1;
				}
				dsv_desc.Texture1DArray.MipSlice = level;
			}
		}
		else if (data_handles.texture_type == D3DTextureData::DeviceTextureType::ID3D11Texture2D)
		{
			D3D11_TEXTURE2D_DESC texture_desc;
			data_handles.get_texture_2d()->GetDesc(&texture_desc);

			dsv_desc.Format = texture_desc.Format;
			if (data->texture_dimensions == texture_2d)
			{
				dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
				dsv_desc.Texture2D.MipSlice = level;
			}
			else if (data->texture_dimensions == texture_2d_array)
			{
				dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
				if (slice == -1)
				{
					dsv_desc.Texture2DArray.FirstArraySlice = 0;
					dsv_desc.Texture2DArray.ArraySize = texture_desc.ArraySize;
				}
				else
				{
					dsv_desc.Texture2DArray.FirstArraySlice = slice;
					dsv_desc.Texture2DArray.ArraySize = 1;
				}
				dsv_desc.Texture2DArray.MipSlice = level;
			}
		}
		else
		{
			throw Exception("D3D does not support D3D11_DSV_DIMENSION_TEXTURE3D afaik");
		}
		dsv_desc.Flags = 0;

		ComPtr<ID3D11DepthStencilView> dsv;
		HRESULT result = device->CreateDepthStencilView(data_handles.texture, &dsv_desc, dsv.output_variable());
		D3DTarget::throw_if_failed("ID3D11Device.CreateDepthStencilView failed", result);
		return dsv;
	}

	TextureProvider *D3DTextureProvider::create_view(TextureDimensions texture_dimensions, TextureFormat texture_format, int min_level, int num_levels, int min_layer, int num_layers)
	{
		return new D3DTextureProvider(this, texture_dimensions, texture_format, min_level, num_levels, min_layer, num_layers);
	}

	void D3DTextureProvider::generate_mipmap()
	{
		// To do: maybe change function to take a gc parameter to make it clear which context the operation should be executed on

		ComPtr<ID3D11DeviceContext> device_context;
		view_handles.front()->device->GetImmediateContext(device_context.output_variable());
		device_context->GenerateMips(get_srv(view_handles.front()->device));
	}

	void D3DTextureProvider::create(int width, int height, int depth, int array_size, TextureFormat texture_format, int levels)
	{
		if (data->texture_dimensions == texture_1d || data->texture_dimensions == texture_1d_array)
		{
			create_1d(width, height, depth, array_size, texture_format, levels);
		}
		else if (data->texture_dimensions == texture_2d || data->texture_dimensions == texture_2d_array || data->texture_dimensions == texture_cube || data->texture_dimensions == texture_cube_array)
		{
			create_2d(width, height, depth, array_size, texture_format, levels);
		}
		else if (data->texture_dimensions == texture_3d)
		{
			create_3d(width, height, depth, array_size, texture_format, levels);
		}
		else
		{
			throw Exception("Unknown texture dimensions type");
		}
	}

	void D3DTextureProvider::create_1d(int width, int height, int depth, int array_size, TextureFormat texture_format, int levels)
	{
		D3D11_TEXTURE1D_DESC texture_desc;
		texture_desc.Width = width;
		texture_desc.MipLevels = levels;
		texture_desc.ArraySize = array_size;
		texture_desc.Format = to_d3d_format(texture_format);
		texture_desc.Usage = D3D11_USAGE_DEFAULT;
		texture_desc.CPUAccessFlags = 0;
		texture_desc.MiscFlags = D3D11_RESOURCE_MISC_SHARED;

		UINT bind_flags_with_compute = 0;
		UINT bind_flags_without_compute = 0;

		if (PixelBuffer::is_compressed(texture_format))
		{
			bind_flags_without_compute = D3D11_BIND_SHADER_RESOURCE;
			bind_flags_with_compute = bind_flags_without_compute;
		}
		else
		{
			bind_flags_without_compute = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			bind_flags_with_compute = bind_flags_without_compute | D3D11_BIND_UNORDERED_ACCESS;

			texture_desc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
		}

		texture_desc.BindFlags = bind_flags_with_compute;

		HRESULT result = data->handles.front()->device->CreateTexture1D(&texture_desc, 0, (ID3D11Texture1D **) (data->handles.front()->texture.output_variable()));
		if (result == E_INVALIDARG && bind_flags_with_compute != bind_flags_without_compute)
		{
			texture_desc.BindFlags = bind_flags_without_compute;
			result = data->handles.front()->device->CreateTexture1D(&texture_desc, 0, (ID3D11Texture1D **) (data->handles.front()->texture.output_variable()));
		}

		D3DTarget::throw_if_failed("ID3D11Device.CreateTexture1D failed", result);
	}

	void D3DTextureProvider::create_2d(int width, int height, int depth, int array_size, TextureFormat texture_format, int levels)
	{
		D3D11_TEXTURE2D_DESC texture_desc;
		texture_desc.Width = width;
		texture_desc.Height = height;
		texture_desc.MipLevels = levels;
		texture_desc.Format = to_d3d_format(texture_format);
		texture_desc.SampleDesc.Count = 1;
		texture_desc.SampleDesc.Quality = 0;
		texture_desc.Usage = D3D11_USAGE_DEFAULT;
		texture_desc.CPUAccessFlags = 0;
		texture_desc.MiscFlags = D3D11_RESOURCE_MISC_SHARED;

		if (data->texture_dimensions == texture_cube)
		{
			texture_desc.ArraySize = 6;
			texture_desc.MiscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;
		}
		else if (data->texture_dimensions == texture_cube_array)
		{
			texture_desc.ArraySize = 6 * array_size;
			texture_desc.MiscFlags |= D3D11_RESOURCE_MISC_TEXTURECUBE;
		}
		else
		{
			texture_desc.ArraySize = array_size;
		}

		UINT bind_flags_with_compute = 0;
		UINT bind_flags_without_compute = 0;

		if (is_stencil_or_depth_format(texture_format))
		{
			texture_desc.MipLevels = 1;

			bind_flags_without_compute = D3D11_BIND_DEPTH_STENCIL;
			bind_flags_with_compute = bind_flags_without_compute;
		}
		else if (PixelBuffer::is_compressed(texture_format))
		{
			bind_flags_without_compute = D3D11_BIND_SHADER_RESOURCE;
			bind_flags_with_compute = bind_flags_without_compute;
		}
		else
		{
			bind_flags_without_compute = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			bind_flags_with_compute = bind_flags_without_compute | D3D11_BIND_UNORDERED_ACCESS;

			texture_desc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
		}

		texture_desc.BindFlags = bind_flags_with_compute;

		HRESULT result = data->handles.front()->device->CreateTexture2D(&texture_desc, 0, (ID3D11Texture2D **) (data->handles.front()->texture.output_variable()));
		if (result == E_INVALIDARG && bind_flags_with_compute != bind_flags_without_compute)
		{
			texture_desc.BindFlags = bind_flags_without_compute;
			result = data->handles.front()->device->CreateTexture2D(&texture_desc, 0, (ID3D11Texture2D **) (data->handles.front()->texture.output_variable()));
		}

		D3DTarget::throw_if_failed("ID3D11Device.CreateTexture2D failed", result);
	}

	void D3DTextureProvider::create_3d(int width, int height, int depth, int array_size, TextureFormat texture_format, int levels)
	{
		D3D11_TEXTURE3D_DESC texture_desc;
		texture_desc.Width = width;
		texture_desc.Height = height;
		texture_desc.MipLevels = levels;
		texture_desc.Depth = depth;
		texture_desc.Format = to_d3d_format(texture_format);
		texture_desc.Usage = D3D11_USAGE_DEFAULT;
		texture_desc.CPUAccessFlags = 0;
		texture_desc.MiscFlags = D3D11_RESOURCE_MISC_SHARED;

		UINT bind_flags_with_compute = 0;
		UINT bind_flags_without_compute = 0;

		if (PixelBuffer::is_compressed(texture_format))
		{
			bind_flags_without_compute = D3D11_BIND_SHADER_RESOURCE;
		}
		else
		{
			bind_flags_without_compute = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			bind_flags_with_compute = bind_flags_without_compute | D3D11_BIND_UNORDERED_ACCESS;

			texture_desc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
		}

		texture_desc.BindFlags = bind_flags_with_compute;

		HRESULT result = data->handles.front()->device->CreateTexture3D(&texture_desc, 0, (ID3D11Texture3D **) (data->handles.front()->texture.output_variable()));
		if (result == E_INVALIDARG && bind_flags_with_compute != bind_flags_without_compute)
		{
			texture_desc.BindFlags = bind_flags_without_compute;
			result = data->handles.front()->device->CreateTexture3D(&texture_desc, 0, (ID3D11Texture3D **) (data->handles.front()->texture.output_variable()));
		}

		D3DTarget::throw_if_failed("ID3D11Device.CreateTexture3D failed", result);
	}

	PixelBuffer D3DTextureProvider::get_pixeldata(GraphicContext &gc, TextureFormat texture_format, int level) const
	{
		D3DGraphicContextProvider *gc_provider = static_cast<D3DGraphicContextProvider*>(gc.get_provider());
		D3DTextureData::DeviceHandles &data_handles = data->get_handles(gc_provider->get_window()->get_device());

		int width;
		int height;
		DXGI_FORMAT format;

		if (data_handles.texture_type == D3DTextureData::DeviceTextureType::ID3D11Texture1D)
		{
			D3D11_TEXTURE1D_DESC texture_desc;
			data_handles.get_texture_1d()->GetDesc(&texture_desc);

			width = max(texture_desc.Width >> level, (UINT)1);
			height = 1;
			format = texture_desc.Format;
		}
		else if (data_handles.texture_type == D3DTextureData::DeviceTextureType::ID3D11Texture2D)
		{
			D3D11_TEXTURE2D_DESC texture_desc;
			data_handles.get_texture_2d()->GetDesc(&texture_desc);

			width = max(texture_desc.Width >> level, (UINT)1);
			height = max(texture_desc.Height >> level, (UINT)1);
			format = texture_desc.Format;
		}
		else
		{
			D3D11_TEXTURE3D_DESC texture_desc;
			data_handles.get_texture_3d()->GetDesc(&texture_desc);

			width = max(texture_desc.Width >> level, (UINT)1);
			height = max(texture_desc.Height >> level, (UINT)1);
			format = texture_desc.Format;
		}

		TransferTexture pixels(gc, width, height, data_from_gpu, from_d3d_format(format));
		D3DPixelBufferProvider *pb_provider = static_cast<D3DPixelBufferProvider *>(pixels.get_provider());
		D3D11_BOX box;
		box.left = 0;
		box.top = 0;
		box.right = width;
		box.bottom = height;
		box.front = 0;
		box.back = 1;
		gc_provider->get_window()->get_device_context()->CopySubresourceRegion(pb_provider->get_texture_2d(gc_provider->get_window()->get_device()), level, 0, 0, 0, data_handles.texture, 0, &box);
		return pixels;
	}

	void D3DTextureProvider::copy_from(GraphicContext &gc, int x, int y, int slice, int level, const PixelBuffer &source_image, const Rect &src_rect)
	{
		D3DGraphicContextProvider *gc_provider = static_cast<D3DGraphicContextProvider*>(gc.get_provider());
		D3DTextureData::DeviceHandles &data_handles = data->get_handles(gc_provider->get_window()->get_device());

		ComPtr<ID3D11DeviceContext> device_context;
		data_handles.device->GetImmediateContext(device_context.output_variable());

		int array_slice = slice;
		int z = 0;

		int texture_width;
		int texture_height;
		DXGI_FORMAT format;
		int mip_levels;

		if (data_handles.texture_type == D3DTextureData::DeviceTextureType::ID3D11Texture1D)
		{
			D3D11_TEXTURE1D_DESC texture_desc;
			data_handles.get_texture_1d()->GetDesc(&texture_desc);
			texture_width = max(texture_desc.Width >> level, (UINT)1);
			texture_height = 1;
			format = texture_desc.Format;
			mip_levels = texture_desc.MipLevels;
		}
		else if (data_handles.texture_type == D3DTextureData::DeviceTextureType::ID3D11Texture2D)
		{
			D3D11_TEXTURE2D_DESC texture_desc;
			data_handles.get_texture_2d()->GetDesc(&texture_desc);
			texture_width = max(texture_desc.Width >> level, (UINT)1);
			texture_height = max(texture_desc.Height >> level, (UINT)1);
			format = texture_desc.Format;
			mip_levels = texture_desc.MipLevels;
		}
		else
		{
			D3D11_TEXTURE3D_DESC texture_desc;
			data_handles.get_texture_3d()->GetDesc(&texture_desc);
			texture_width = max(texture_desc.Width >> level, (UINT)1);
			texture_height = max(texture_desc.Height >> level, (UINT)1);
			format = texture_desc.Format;
			mip_levels = texture_desc.MipLevels;
			z = slice;
			array_slice = 0;
		}

		if (x < 0 || x + src_rect.get_width() > texture_width || y < 0 || y + src_rect.get_height() > texture_height)
			throw Exception("out of bounds!");

		int dest_subresource = D3D11CalcSubresource(level, array_slice, mip_levels);

		D3DPixelBufferProvider *pb_provider = dynamic_cast<D3DPixelBufferProvider*>(source_image.get_provider());
		if (pb_provider)
		{
			int src_subresource = D3D11CalcSubresource(0, 0, 1);

			D3D11_BOX box;
			box.left = src_rect.left;
			box.top = src_rect.top;
			box.right = src_rect.right;
			box.bottom = src_rect.bottom;
			box.front = 0;
			box.back = 1;
			device_context->CopySubresourceRegion(data_handles.texture, dest_subresource, x, y, z, pb_provider->get_texture_2d(data_handles.device), src_subresource, &box);
		}
		else
		{
			TextureFormat dest_format = from_d3d_format(format);

			PixelBuffer src_image_converted;
			if (dest_format == source_image.get_format())
				src_image_converted = source_image;
			else
				src_image_converted = source_image.to_format(dest_format);

			D3D11_BOX box;
			box.left = x;
			box.top = y;
			box.right = x + src_rect.get_width();
			box.bottom = y + src_rect.get_height();
			box.front = z;
			box.back = z + 1;

			if (source_image.is_compressed())
			{
				int block_x = src_rect.left / 4;
				int block_y = src_rect.top / 4;

				int blocks_per_row = (source_image.get_width() + 3) / 4;
				int bytes_per_block = src_image_converted.get_bytes_per_block();

				int row_pitch = blocks_per_row * bytes_per_block;
				int slice_pitch = 0;

				const unsigned char *src_data = src_image_converted.get_data_uint8();
				//src_data += (block_x + block_y * blocks_per_row) * bytes_per_block;

				device_context->UpdateSubresource(data_handles.texture, dest_subresource, &box, src_data, row_pitch, slice_pitch);
			}
			else
			{
				int row_pitch = src_image_converted.get_pitch();
				int slice_pitch = src_image_converted.get_pitch() * src_image_converted.get_height();

				const unsigned char *src_data = src_image_converted.get_data_uint8();
				//src_data += src_rect.left * src_image_converted.get_bytes_per_pixel() + src_rect.top * src_image_converted.get_pitch();

				device_context->UpdateSubresource(data_handles.texture, dest_subresource, &box, src_data, row_pitch, slice_pitch);
			}
		}
	}

	void D3DTextureProvider::copy_image_from(
		int x,
		int y,
		int width,
		int height,
		int level,
		TextureFormat texture_format,
		GraphicContextProvider *gc)
	{
		throw Exception("copy_image_from not supported yet by D3D target");
	}

	void D3DTextureProvider::copy_subimage_from(
		int offset_x,
		int offset_y,
		int x,
		int y,
		int width,
		int height,
		int level,
		GraphicContextProvider *gc)
	{
		throw Exception("copy_subimage_from not supported yet by D3D target");
	}

	void D3DTextureProvider::set_min_lod(double min_lod)
	{
		for (size_t i = 0; i < view_handles.size(); i++)
			view_handles[i]->sampler_state.set_min_lod(min_lod);
	}

	void D3DTextureProvider::set_max_lod(double max_lod)
	{
		for (size_t i = 0; i < view_handles.size(); i++)
			view_handles[i]->sampler_state.set_max_lod(max_lod);
	}

	void D3DTextureProvider::set_lod_bias(double lod_bias)
	{
		for (size_t i = 0; i < view_handles.size(); i++)
			view_handles[i]->sampler_state.set_lod_bias(lod_bias);
	}

	void D3DTextureProvider::set_base_level(int base_level)
	{
	}

	void D3DTextureProvider::set_max_level(int max_level)
	{
	}

	void D3DTextureProvider::set_wrap_mode(
		TextureWrapMode wrap_s,
		TextureWrapMode wrap_t,
		TextureWrapMode wrap_r)
	{
		for (size_t i = 0; i < view_handles.size(); i++)
			view_handles[i]->sampler_state.set_wrap_mode(wrap_s, wrap_t, wrap_r);
	}

	void D3DTextureProvider::set_wrap_mode(
		TextureWrapMode wrap_s,
		TextureWrapMode wrap_t)
	{
		for (size_t i = 0; i < view_handles.size(); i++)
			view_handles[i]->sampler_state.set_wrap_mode(wrap_s, wrap_t);
	}

	void D3DTextureProvider::set_wrap_mode(
		TextureWrapMode wrap_s)
	{
		for (size_t i = 0; i < view_handles.size(); i++)
			view_handles[i]->sampler_state.set_wrap_mode(wrap_s);
	}

	void D3DTextureProvider::set_min_filter(TextureFilter filter)
	{
		for (size_t i = 0; i < view_handles.size(); i++)
			view_handles[i]->sampler_state.set_min_filter(filter);
	}

	void D3DTextureProvider::set_mag_filter(TextureFilter filter)
	{
		for (size_t i = 0; i < view_handles.size(); i++)
			view_handles[i]->sampler_state.set_mag_filter(filter);
	}

	void D3DTextureProvider::set_max_anisotropy(float v)
	{
		for (size_t i = 0; i < view_handles.size(); i++)
			view_handles[i]->sampler_state.set_max_anisotropy(v);
	}

	void D3DTextureProvider::set_texture_compare(TextureCompareMode mode, CompareFunction func)
	{
		for (size_t i = 0; i < view_handles.size(); i++)
			view_handles[i]->sampler_state.set_texture_compare(mode, func);
	}

	void D3DTextureProvider::device_destroyed(ID3D11Device *device)
	{
		for (size_t i = 0; i < view_handles.size(); i++)
		{
			if (view_handles[i]->device.get() == device)
			{
				view_handles.erase(view_handles.begin() + i);
				return;
			}
		}
	}

	D3DTextureProvider::ViewHandles &D3DTextureProvider::get_view_handles(const ComPtr<ID3D11Device> &device) const
	{
		for (size_t i = 0; i < view_handles.size(); i++)
			if (view_handles[i]->device == device)
				return *view_handles[i];

		view_handles.push_back(std::shared_ptr<ViewHandles>(new ViewHandles(device)));
		return *view_handles.back();
	}

	bool D3DTextureProvider::is_stencil_or_depth_format(TextureFormat format)
	{
		switch (format)
		{
		case tf_depth_component16:
		case tf_depth_component24:
		case tf_depth_component32:
		case tf_depth_component32f:
		case tf_depth24_stencil8:
		case tf_depth32f_stencil8:
		case tf_stencil_index1:
		case tf_stencil_index4:
		case tf_stencil_index8:
		case tf_stencil_index16:
			return true;
		}
		return false;
	}

	DXGI_FORMAT D3DTextureProvider::to_d3d_format(TextureFormat format)
	{
		switch (format)
		{
		case tf_rgba8: return DXGI_FORMAT_R8G8B8A8_UNORM;
		case tf_rgb8: break;
		case tf_bgra8: return DXGI_FORMAT_B8G8R8A8_UNORM;
		case tf_bgr8: break;
		case tf_stencil_index1: return DXGI_FORMAT_R1_UNORM;
		case tf_stencil_index4: break;
		case tf_stencil_index8: return DXGI_FORMAT_R8_SINT;
		case tf_stencil_index16: return DXGI_FORMAT_R16_SINT;
		case tf_r8: return DXGI_FORMAT_R8_UNORM;
		case tf_r8_snorm: return DXGI_FORMAT_R8_SNORM;
		case tf_r16: return DXGI_FORMAT_R16_UNORM;
		case tf_r16_snorm: return DXGI_FORMAT_R16_SNORM;
		case tf_rg8: return DXGI_FORMAT_R8G8_UNORM;
		case tf_rg8_snorm: return DXGI_FORMAT_R8G8_SNORM;
		case tf_rg16: return DXGI_FORMAT_R16G16_UNORM;
		case tf_rg16_snorm: return DXGI_FORMAT_R16G16_SNORM;
		case tf_r3_g3_b2: break;
		case tf_rgb4: break;
		case tf_rgb5: break;
		case tf_rgb8_snorm: break;
		case tf_rgb10: break;
		case tf_rgb12: break;
		case tf_rgb16: break;
		case tf_rgb16_snorm: break;
		case tf_rgba2: break;
		case tf_rgba4: break;
		case tf_rgb5_a1: break;
		case tf_rgba8_snorm: break;
		case tf_rgb10_a2: return DXGI_FORMAT_R10G10B10A2_UNORM;
		case tf_rgba12: break;
		case tf_rgba16: return DXGI_FORMAT_R16G16B16A16_UNORM;
		case tf_rgba16_snorm: return DXGI_FORMAT_R16G16B16A16_SNORM;
		case tf_srgb8: break;
		case tf_srgb8_alpha8: return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		case tf_r16f: return DXGI_FORMAT_R16_FLOAT;
		case tf_rg16f: return DXGI_FORMAT_R16G16_FLOAT;
		case tf_rgb16f: break;
		case tf_rgba16f: return DXGI_FORMAT_R16G16B16A16_FLOAT;
		case tf_r32f: return DXGI_FORMAT_R32_FLOAT;
		case tf_rg32f: return DXGI_FORMAT_R32G32_FLOAT;
		case tf_rgb32f: return DXGI_FORMAT_R32G32B32_FLOAT;
		case tf_rgba32f: return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case tf_r11f_g11f_b10f: return DXGI_FORMAT_R11G11B10_FLOAT;
		case tf_rgb9_e5: return DXGI_FORMAT_R9G9B9E5_SHAREDEXP;
		case tf_r8i: return DXGI_FORMAT_R8_SINT;
		case tf_r8ui: return DXGI_FORMAT_R8_UINT;
		case tf_r16i: return DXGI_FORMAT_R16_SINT;
		case tf_r16ui: return DXGI_FORMAT_R16_UINT;
		case tf_r32i: return DXGI_FORMAT_R32_SINT;
		case tf_r32ui: return DXGI_FORMAT_R8_UINT;
		case tf_rg8i: return DXGI_FORMAT_R8G8_SINT;
		case tf_rg8ui: return DXGI_FORMAT_R8G8_UINT;
		case tf_rg16i: return DXGI_FORMAT_R16G16_SINT;
		case tf_rg16ui: return DXGI_FORMAT_R16G16_UINT;
		case tf_rg32i: return DXGI_FORMAT_R32G32_SINT;
		case tf_rg32ui: return DXGI_FORMAT_R32G32_UINT;
		case tf_rgb8i: break;
		case tf_rgb8ui: break;
		case tf_rgb16i: break;
		case tf_rgb16ui: break;
		case tf_rgb32i: return DXGI_FORMAT_R32G32B32_SINT;
		case tf_rgb32ui: return DXGI_FORMAT_R32G32B32_UINT;
		case tf_rgba8i: return DXGI_FORMAT_R8G8B8A8_SINT;
		case tf_rgba8ui: return DXGI_FORMAT_R8G8B8A8_UINT;
		case tf_rgba16i: return DXGI_FORMAT_R16G16B16A16_SINT;
		case tf_rgba16ui: return DXGI_FORMAT_R16G16B16A16_UINT;
		case tf_rgba32i: return DXGI_FORMAT_R32G32B32A32_SINT;
		case tf_rgba32ui: return DXGI_FORMAT_R32G32B32A32_UINT;
		case tf_depth_component16: return DXGI_FORMAT_D16_UNORM;
		case tf_depth_component24: return DXGI_FORMAT_D24_UNORM_S8_UINT;
		case tf_depth_component32: return DXGI_FORMAT_D32_FLOAT;
		case tf_depth_component32f: return DXGI_FORMAT_D32_FLOAT;
		case tf_depth24_stencil8: return DXGI_FORMAT_D24_UNORM_S8_UINT;
		case tf_depth32f_stencil8: return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
		case tf_compressed_red: break;
		case tf_compressed_rg: break;
		case tf_compressed_rgb: break;
		case tf_compressed_rgba: break;
		case tf_compressed_srgb: break;
		case tf_compressed_srgb_alpha: break;
		case tf_compressed_red_rgtc1: break;
		case tf_compressed_signed_red_rgtc1: break;
		case tf_compressed_rg_rgtc2: break;
		case tf_compressed_signed_rg_rgtc2: break;
		case tf_compressed_rgb_s3tc_dxt1: return DXGI_FORMAT_BC1_UNORM;
		case tf_compressed_rgba_s3tc_dxt1: return DXGI_FORMAT_BC1_UNORM;
		case tf_compressed_rgba_s3tc_dxt3: return DXGI_FORMAT_BC2_UNORM;
		case tf_compressed_rgba_s3tc_dxt5: return DXGI_FORMAT_BC3_UNORM;
		case tf_compressed_srgb_s3tc_dxt1: return DXGI_FORMAT_BC1_UNORM_SRGB;
		case tf_compressed_srgb_alpha_s3tc_dxt1: return DXGI_FORMAT_BC1_UNORM_SRGB;
		case tf_compressed_srgb_alpha_s3tc_dxt3: return DXGI_FORMAT_BC2_UNORM_SRGB;
		case tf_compressed_srgb_alpha_s3tc_dxt5: return DXGI_FORMAT_BC3_UNORM_SRGB;
		}
		throw Exception("Unsupported format");
	}

	TextureFormat D3DTextureProvider::from_d3d_format(DXGI_FORMAT d3d_format)
	{
		switch (d3d_format)
		{
		case DXGI_FORMAT_UNKNOWN: break;
		case DXGI_FORMAT_R32G32B32A32_TYPELESS: break;
		case DXGI_FORMAT_R32G32B32A32_FLOAT: return tf_rgba32f;
		case DXGI_FORMAT_R32G32B32A32_UINT: return tf_rgba32ui;
		case DXGI_FORMAT_R32G32B32A32_SINT: return tf_rgba32i;
		case DXGI_FORMAT_R32G32B32_TYPELESS: break;
		case DXGI_FORMAT_R32G32B32_FLOAT: return tf_rgb32f;
		case DXGI_FORMAT_R32G32B32_UINT: return tf_rgb32ui;
		case DXGI_FORMAT_R32G32B32_SINT: return tf_rgb32i;
		case DXGI_FORMAT_R16G16B16A16_TYPELESS: break;
		case DXGI_FORMAT_R16G16B16A16_FLOAT: return tf_rgba16f;
		case DXGI_FORMAT_R16G16B16A16_UNORM: return tf_rgba16;
		case DXGI_FORMAT_R16G16B16A16_UINT: return tf_rgba16ui;
		case DXGI_FORMAT_R16G16B16A16_SNORM: return tf_rgba16_snorm;
		case DXGI_FORMAT_R16G16B16A16_SINT: return tf_rgba16i;
		case DXGI_FORMAT_R32G32_TYPELESS: break;
		case DXGI_FORMAT_R32G32_FLOAT: return tf_rg32f;
		case DXGI_FORMAT_R32G32_UINT: return tf_rg32ui;
		case DXGI_FORMAT_R32G32_SINT: return tf_rg32i;
		case DXGI_FORMAT_R32G8X24_TYPELESS: break;
		case DXGI_FORMAT_D32_FLOAT_S8X24_UINT: return tf_depth32f_stencil8;
		case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS: break;
		case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT: break;
		case DXGI_FORMAT_R10G10B10A2_TYPELESS: break;
		case DXGI_FORMAT_R10G10B10A2_UNORM: return tf_rgb10_a2;
		case DXGI_FORMAT_R10G10B10A2_UINT: break;
		case DXGI_FORMAT_R11G11B10_FLOAT: break;
		case DXGI_FORMAT_R8G8B8A8_TYPELESS: break;
		case DXGI_FORMAT_R8G8B8A8_UNORM: return tf_rgba8;
		case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB: return tf_srgb8_alpha8;
		case DXGI_FORMAT_R8G8B8A8_UINT: return tf_rgba8ui;
		case DXGI_FORMAT_R8G8B8A8_SNORM: return tf_rgba8_snorm;
		case DXGI_FORMAT_R8G8B8A8_SINT: return tf_rgba8i;
		case DXGI_FORMAT_R16G16_TYPELESS: break;
		case DXGI_FORMAT_R16G16_FLOAT: return tf_rg16f;
		case DXGI_FORMAT_R16G16_UNORM: return tf_rg16;
		case DXGI_FORMAT_R16G16_UINT: return tf_rg16ui;
		case DXGI_FORMAT_R16G16_SNORM: return tf_rg16_snorm;
		case DXGI_FORMAT_R16G16_SINT: return tf_rg16i;
		case DXGI_FORMAT_R32_TYPELESS: break;
		case DXGI_FORMAT_D32_FLOAT: return tf_depth_component32f;
		case DXGI_FORMAT_R32_FLOAT: return tf_r32f;
		case DXGI_FORMAT_R32_UINT: return tf_r32ui;
		case DXGI_FORMAT_R32_SINT: return tf_r32i;
		case DXGI_FORMAT_R24G8_TYPELESS: break;
		case DXGI_FORMAT_D24_UNORM_S8_UINT: return tf_depth24_stencil8;
		case DXGI_FORMAT_R24_UNORM_X8_TYPELESS: break;
		case DXGI_FORMAT_X24_TYPELESS_G8_UINT: break;
		case DXGI_FORMAT_R8G8_TYPELESS: break;
		case DXGI_FORMAT_R8G8_UNORM: return tf_rg8;
		case DXGI_FORMAT_R8G8_UINT: return tf_rg8ui;
		case DXGI_FORMAT_R8G8_SNORM: return tf_rg8_snorm;
		case DXGI_FORMAT_R8G8_SINT: return tf_rg8i;
		case DXGI_FORMAT_R16_TYPELESS: break;
		case DXGI_FORMAT_R16_FLOAT: return tf_r16f;
		case DXGI_FORMAT_D16_UNORM: break;
		case DXGI_FORMAT_R16_UNORM: return tf_r16;
		case DXGI_FORMAT_R16_UINT: return tf_r16ui;
		case DXGI_FORMAT_R16_SNORM: return tf_r16_snorm;
		case DXGI_FORMAT_R16_SINT: return tf_r16i;
		case DXGI_FORMAT_R8_TYPELESS: break;
		case DXGI_FORMAT_R8_UNORM: return tf_r8;
		case DXGI_FORMAT_R8_UINT: return tf_r8ui;
		case DXGI_FORMAT_R8_SNORM: return tf_r8_snorm;
		case DXGI_FORMAT_R8_SINT: return tf_r8i;
		case DXGI_FORMAT_A8_UNORM: break;
		case DXGI_FORMAT_R1_UNORM: break;
		case DXGI_FORMAT_R9G9B9E5_SHAREDEXP: return tf_rgb9_e5;
		case DXGI_FORMAT_R8G8_B8G8_UNORM: break;
		case DXGI_FORMAT_G8R8_G8B8_UNORM: break;
		case DXGI_FORMAT_BC1_TYPELESS: break;
		case DXGI_FORMAT_BC1_UNORM: return tf_compressed_rgb_s3tc_dxt1;
		case DXGI_FORMAT_BC1_UNORM_SRGB: return tf_compressed_srgb_s3tc_dxt1;
		case DXGI_FORMAT_BC2_TYPELESS: break;
		case DXGI_FORMAT_BC2_UNORM: return tf_compressed_rgba_s3tc_dxt3;
		case DXGI_FORMAT_BC2_UNORM_SRGB: return tf_compressed_srgb_alpha_s3tc_dxt3;
		case DXGI_FORMAT_BC3_TYPELESS: break;
		case DXGI_FORMAT_BC3_UNORM: return tf_compressed_rgba_s3tc_dxt5;
		case DXGI_FORMAT_BC3_UNORM_SRGB: return tf_compressed_srgb_alpha_s3tc_dxt5;
		case DXGI_FORMAT_BC4_TYPELESS: break;
		case DXGI_FORMAT_BC4_UNORM: break;
		case DXGI_FORMAT_BC4_SNORM: break;
		case DXGI_FORMAT_BC5_TYPELESS: break;
		case DXGI_FORMAT_BC5_UNORM: break;
		case DXGI_FORMAT_BC5_SNORM: break;
		case DXGI_FORMAT_B5G6R5_UNORM: break;
		case DXGI_FORMAT_B5G5R5A1_UNORM: break;
		case DXGI_FORMAT_B8G8R8A8_UNORM: return tf_bgra8;
		case DXGI_FORMAT_B8G8R8X8_UNORM: break;
		case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM: break;
		case DXGI_FORMAT_B8G8R8A8_TYPELESS: break;
		case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB: break;
		case DXGI_FORMAT_B8G8R8X8_TYPELESS: break;
		case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB: break;
		case DXGI_FORMAT_BC6H_TYPELESS: break;
		case DXGI_FORMAT_BC6H_UF16: break;
		case DXGI_FORMAT_BC6H_SF16: break;
		case DXGI_FORMAT_BC7_TYPELESS: break;
		case DXGI_FORMAT_BC7_UNORM: break;
		case DXGI_FORMAT_BC7_UNORM_SRGB: break;
		};
		throw Exception("Unsupported format");
	}

	int D3DTextureProvider::get_bytes_per_pixel(DXGI_FORMAT d3d_format)
	{
		switch (d3d_format)
		{
		case DXGI_FORMAT_UNKNOWN: break;
		case DXGI_FORMAT_R32G32B32A32_TYPELESS:
		case DXGI_FORMAT_R32G32B32A32_FLOAT:
		case DXGI_FORMAT_R32G32B32A32_UINT:
		case DXGI_FORMAT_R32G32B32A32_SINT:
			 return 16;
		case DXGI_FORMAT_R32G32B32_TYPELESS:
		case DXGI_FORMAT_R32G32B32_FLOAT:
		case DXGI_FORMAT_R32G32B32_UINT:
		case DXGI_FORMAT_R32G32B32_SINT:
			return 12;
		case DXGI_FORMAT_R16G16B16A16_TYPELESS:
		case DXGI_FORMAT_R16G16B16A16_FLOAT:
		case DXGI_FORMAT_R16G16B16A16_UNORM:
		case DXGI_FORMAT_R16G16B16A16_UINT:
		case DXGI_FORMAT_R16G16B16A16_SNORM:
		case DXGI_FORMAT_R16G16B16A16_SINT:
		case DXGI_FORMAT_R32G32_TYPELESS:
		case DXGI_FORMAT_R32G32_FLOAT:
		case DXGI_FORMAT_R32G32_UINT:
		case DXGI_FORMAT_R32G32_SINT:
		case DXGI_FORMAT_R32G8X24_TYPELESS:
		case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
		case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
		case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
			return 8;
		case DXGI_FORMAT_R10G10B10A2_TYPELESS:
		case DXGI_FORMAT_R10G10B10A2_UNORM:
		case DXGI_FORMAT_R10G10B10A2_UINT:
		case DXGI_FORMAT_R11G11B10_FLOAT:
		case DXGI_FORMAT_R8G8B8A8_TYPELESS:
		case DXGI_FORMAT_R8G8B8A8_UNORM:
		case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
		case DXGI_FORMAT_R8G8B8A8_UINT:
		case DXGI_FORMAT_R8G8B8A8_SNORM:
		case DXGI_FORMAT_R8G8B8A8_SINT:
		case DXGI_FORMAT_R16G16_TYPELESS:
		case DXGI_FORMAT_R16G16_FLOAT:
		case DXGI_FORMAT_R16G16_UNORM:
		case DXGI_FORMAT_R16G16_UINT:
		case DXGI_FORMAT_R16G16_SNORM:
		case DXGI_FORMAT_R16G16_SINT:
		case DXGI_FORMAT_R32_TYPELESS:
		case DXGI_FORMAT_D32_FLOAT:
		case DXGI_FORMAT_R32_FLOAT:
		case DXGI_FORMAT_R32_UINT:
		case DXGI_FORMAT_R32_SINT:
		case DXGI_FORMAT_R24G8_TYPELESS:
		case DXGI_FORMAT_D24_UNORM_S8_UINT:
		case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
		case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
			return 4;
		case DXGI_FORMAT_R8G8_TYPELESS:
		case DXGI_FORMAT_R8G8_UNORM:
		case DXGI_FORMAT_R8G8_UINT:
		case DXGI_FORMAT_R8G8_SNORM:
		case DXGI_FORMAT_R8G8_SINT:
		case DXGI_FORMAT_R16_TYPELESS:
		case DXGI_FORMAT_R16_FLOAT:
		case DXGI_FORMAT_D16_UNORM:
		case DXGI_FORMAT_R16_UNORM:
		case DXGI_FORMAT_R16_UINT:
		case DXGI_FORMAT_R16_SNORM:
		case DXGI_FORMAT_R16_SINT:
			return 2;
		case DXGI_FORMAT_R8_TYPELESS:
		case DXGI_FORMAT_R8_UNORM:
		case DXGI_FORMAT_R8_UINT:
		case DXGI_FORMAT_R8_SNORM:
		case DXGI_FORMAT_R8_SINT:
		case DXGI_FORMAT_A8_UNORM:
			return 1;
		case DXGI_FORMAT_R1_UNORM:
			break;
		case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
		case DXGI_FORMAT_R8G8_B8G8_UNORM:
		case DXGI_FORMAT_G8R8_G8B8_UNORM:
			return 4;
		case DXGI_FORMAT_BC1_TYPELESS:
		case DXGI_FORMAT_BC1_UNORM:
		case DXGI_FORMAT_BC1_UNORM_SRGB:
		case DXGI_FORMAT_BC2_TYPELESS:
		case DXGI_FORMAT_BC2_UNORM:
		case DXGI_FORMAT_BC2_UNORM_SRGB:
		case DXGI_FORMAT_BC3_TYPELESS:
		case DXGI_FORMAT_BC3_UNORM:
		case DXGI_FORMAT_BC3_UNORM_SRGB:
		case DXGI_FORMAT_BC4_TYPELESS:
		case DXGI_FORMAT_BC4_UNORM:
		case DXGI_FORMAT_BC4_SNORM:
		case DXGI_FORMAT_BC5_TYPELESS:
		case DXGI_FORMAT_BC5_UNORM:
		case DXGI_FORMAT_BC5_SNORM:
			break;
		case DXGI_FORMAT_B5G6R5_UNORM:
		case DXGI_FORMAT_B5G5R5A1_UNORM:
			return 2;
		case DXGI_FORMAT_B8G8R8A8_UNORM:
		case DXGI_FORMAT_B8G8R8X8_UNORM:
		case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
		case DXGI_FORMAT_B8G8R8A8_TYPELESS:
		case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
		case DXGI_FORMAT_B8G8R8X8_TYPELESS:
		case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
			return 4;
		case DXGI_FORMAT_BC6H_TYPELESS:
		case DXGI_FORMAT_BC6H_UF16:
		case DXGI_FORMAT_BC6H_SF16:
		case DXGI_FORMAT_BC7_TYPELESS:
		case DXGI_FORMAT_BC7_UNORM:
		case DXGI_FORMAT_BC7_UNORM_SRGB:
			break;
		};
		throw Exception("Unsupported format");
	}
}
