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
#include "d3d_texture_data.h"
#include "API/D3D/d3d_target.h"
#include "API/Display/Render/shared_gc_data.h"
#include "d3d_graphic_context_provider.h"
#include "d3d_display_window_provider.h"

namespace clan
{
	D3DTextureData::D3DTextureData(const ComPtr<ID3D11Device> &device, D3D_FEATURE_LEVEL feature_level, TextureDimensions texture_dimensions)
		: feature_level(feature_level), texture_dimensions(texture_dimensions)
	{
		handles.push_back(std::shared_ptr<DeviceHandles>(new DeviceHandles(device, decode_texture_type(texture_dimensions))));
	}

	D3DTextureData::~D3DTextureData()
	{
	}

	void D3DTextureData::device_destroyed(ID3D11Device *device)
	{
		for (size_t i = 0; i < handles.size(); i++)
		{
			if (handles[i]->device.get() == device)
			{
				if (handles.size() == 1)	// Only allocated for a single device
				{
					attach_to_another_device(device);
				}

				handles.erase(handles.begin() + i);
				return;
			}
		}
	}

	void D3DTextureData::attach_to_another_device(ID3D11Device *not_this_device)
	{
		// This code is used to ensure the texture is copied to another provider if the (single) owner is destroyed
		std::unique_ptr<std::unique_lock<std::recursive_mutex>> mutex_section;
		std::vector<GraphicContextProvider*> &gc_providers = SharedGCData::get_gc_providers(mutex_section);

		unsigned int max = gc_providers.size();
		for (unsigned int cnt = 0; cnt < max; cnt++)
		{
			D3DGraphicContextProvider* gc_provider = dynamic_cast<D3DGraphicContextProvider *>(gc_providers[cnt]);
			if (gc_provider->get_window()->get_device() != not_this_device)
			{
				ComPtr<ID3D11Device> device = gc_provider->get_window()->get_device();
				get_handles(device);
				return;
			}
		}
	}

	D3DTextureData::DeviceHandles &D3DTextureData::get_handles(const ComPtr<ID3D11Device> &device) const
	{
		if (handles.empty())
			throw Exception("All handles have been destroyed");

		for (size_t i = 0; i < handles.size(); i++)
			if (handles[i]->device == device)
				return *handles[i];

		ComPtr<IDXGIResource> resource;
		HRESULT result = handles.front()->texture->QueryInterface(__uuidof(IDXGIResource), (void**)resource.output_variable());
		D3DTarget::throw_if_failed("ID3D11Resource.QueryInterface(IDXGIResource) failed", result);

		HANDLE handle = 0;
		result = resource->GetSharedHandle(&handle);
		D3DTarget::throw_if_failed("IDXGIResource.GetSharedHandle failed", result);

		DeviceTextureType texture_type = decode_texture_type(texture_dimensions);

		if (texture_type == DeviceTextureType::ID3D11Texture1D)
		{
			ComPtr<ID3D11Texture1D> texture_handle;
			result = device->OpenSharedResource(handle, __uuidof(ID3D11Texture1D), (void**)texture_handle.output_variable());
			D3DTarget::throw_if_failed("ID3D11Device.OpenSharedResource failed", result);
			handles.push_back(std::shared_ptr<DeviceHandles>(new DeviceHandles(device, texture_type)));
			handles.back()->texture = ComPtr<ID3D11Resource>(texture_handle);
		}
		else if (texture_type == DeviceTextureType::ID3D11Texture2D)
		{
			ComPtr<ID3D11Texture2D> texture_handle;
			result = device->OpenSharedResource(handle, __uuidof(ID3D11Texture2D), (void**)texture_handle.output_variable());
			D3DTarget::throw_if_failed("ID3D11Device.OpenSharedResource failed", result);
			handles.push_back(std::shared_ptr<DeviceHandles>(new DeviceHandles(device, texture_type)));
			handles.back()->texture = ComPtr<ID3D11Resource>(texture_handle);
		}
		else
		{
			ComPtr<ID3D11Texture3D> texture_handle;
			result = device->OpenSharedResource(handle, __uuidof(ID3D11Texture3D), (void**)texture_handle.output_variable());
			D3DTarget::throw_if_failed("ID3D11Device.OpenSharedResource failed", result);
			handles.push_back(std::shared_ptr<DeviceHandles>(new DeviceHandles(device, texture_type)));
			handles.back()->texture = ComPtr<ID3D11Resource>(texture_handle);
		}

		return *handles.back();
	}

	D3DTextureData::DeviceTextureType D3DTextureData::decode_texture_type(TextureDimensions texture_dimensions) const
	{
		DeviceTextureType texture_type;

		switch (texture_dimensions)
		{
		case texture_1d:
		case texture_1d_array:
			texture_type = DeviceTextureType::ID3D11Texture1D;
			break;
		case texture_2d:
		case texture_2d_array:
		case texture_cube:
		case texture_cube_array:
			texture_type = DeviceTextureType::ID3D11Texture2D;
			break;
		case texture_3d:
			texture_type = DeviceTextureType::ID3D11Texture3D;
			break;
		default:
			throw Exception("Unknown texture dimension");
		}

		return texture_type;
	}
}
