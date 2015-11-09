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
#include "d3d_render_buffer_provider.h"
#include "d3d_texture_provider.h"
#include "API/D3D/d3d_target.h"

namespace clan
{
	D3DRenderBufferProvider::D3DRenderBufferProvider(const ComPtr<ID3D11Device> &device)
	{
		handles.push_back(std::shared_ptr<DeviceHandles>(new DeviceHandles(device)));
	}

	D3DRenderBufferProvider::~D3DRenderBufferProvider()
	{
	}

	ComPtr<ID3D11Texture2D> &D3DRenderBufferProvider::get_texture(const ComPtr<ID3D11Device> &device)
	{
		if (device)
			return get_handles(device).texture;
		else
			return handles.front()->texture;
	}

	ComPtr<ID3D11RenderTargetView> D3DRenderBufferProvider::create_rtv(const ComPtr<ID3D11Device> &device)
	{
		D3D11_RENDER_TARGET_VIEW_DESC rtv_desc;
		rtv_desc.Format = DXGI_FORMAT_UNKNOWN;
		rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		rtv_desc.Texture2D.MipSlice = 0;

		ComPtr<ID3D11RenderTargetView> rtv;
		HRESULT result = device->CreateRenderTargetView(get_handles(device).texture, &rtv_desc, rtv.output_variable());
		D3DTarget::throw_if_failed("ID3D11Device.CreateRenderTargetView failed", result);
		return rtv;
	}

	ComPtr<ID3D11DepthStencilView> D3DRenderBufferProvider::create_dsv(const ComPtr<ID3D11Device> &device)
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc;
		dsv_desc.Format = DXGI_FORMAT_UNKNOWN;
		dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsv_desc.Texture2D.MipSlice = 0;
		dsv_desc.Flags = 0;

		ComPtr<ID3D11DepthStencilView> dsv;
		HRESULT result = device->CreateDepthStencilView(get_handles(device).texture, &dsv_desc, dsv.output_variable());
		D3DTarget::throw_if_failed("ID3D11Device.CreateDepthStencilView failed", result);
		return dsv;
	}

	void D3DRenderBufferProvider::create(int width, int height, TextureFormat texture_format, int multisample_samples)
	{
		handles.resize(1);
		handles.front()->texture.clear();

		D3D11_TEXTURE2D_DESC texture_desc;
		texture_desc.Width = width;
		texture_desc.Height = height;
		texture_desc.MipLevels = 1;
		texture_desc.ArraySize = 1;
		texture_desc.Format = D3DTextureProvider::to_d3d_format(texture_format);
		texture_desc.SampleDesc.Count = multisample_samples;
		texture_desc.SampleDesc.Quality = 0;
		texture_desc.Usage = D3D11_USAGE_DEFAULT;
		texture_desc.CPUAccessFlags = 0;
		texture_desc.MiscFlags = D3D11_RESOURCE_MISC_SHARED;
		if (D3DTextureProvider::is_stencil_or_depth_format(texture_format))
			texture_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		else
			texture_desc.BindFlags = D3D11_BIND_RENDER_TARGET;

		HRESULT result = handles.front()->device->CreateTexture2D(&texture_desc, 0, handles.front()->texture.output_variable());
		D3DTarget::throw_if_failed("ID3D11Device.CreateTexture2D failed", result);
	}

	void D3DRenderBufferProvider::device_destroyed(ID3D11Device *device)
	{
		for (size_t i = 0; i < handles.size(); i++)
		{
			if (handles[i]->device.get() == device)
			{
				handles.erase(handles.begin() + i);
				return;
			}
		}
	}

	D3DRenderBufferProvider::DeviceHandles &D3DRenderBufferProvider::get_handles(const ComPtr<ID3D11Device> &device)
	{
		for (size_t i = 0; i < handles.size(); i++)
			if (handles[i]->device == device)
				return *handles[i];

		ComPtr<IDXGIResource> resource;
		HRESULT result = handles.front()->texture->QueryInterface(__uuidof(IDXGIResource), (void**)resource.output_variable());
		D3DTarget::throw_if_failed("ID3D11Texture2D.QueryInterface(IDXGIResource) failed", result);

		HANDLE handle = 0;
		result = resource->GetSharedHandle(&handle);
		D3DTarget::throw_if_failed("IDXGIResource.GetSharedHandle failed", result);

		ComPtr<ID3D11Texture2D> texture_handle;
		result = device->OpenSharedResource(handle, __uuidof(ID3D11Texture2D), (void**)texture_handle.output_variable());
		D3DTarget::throw_if_failed("ID3D11Device.OpenSharedResource failed", result);

		handles.push_back(std::shared_ptr<DeviceHandles>(new DeviceHandles(device)));
		handles.back()->texture = texture_handle;
		return *handles.back();
	}
}
