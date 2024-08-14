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
#include "d3d_pixel_buffer_provider.h"
#include "d3d_texture_provider.h"
#include "d3d_graphic_context_provider.h"
#include "d3d_display_window_provider.h"
#include "API/D3D/d3d_target.h"

namespace clan
{
	D3DPixelBufferProvider::D3DPixelBufferProvider(const ComPtr<ID3D11Device> &device)
		: map_gc_provider(0)

	{
		handles.push_back(std::make_shared<DeviceHandles>(device));
		map_data.pData = 0;
		map_data.RowPitch = 0;
		map_data.DepthPitch = 0;
	}

	D3DPixelBufferProvider::~D3DPixelBufferProvider()
	{
	}

	void D3DPixelBufferProvider::create(const void *data, const Size &new_size, PixelBufferDirection direction, TextureFormat format, BufferUsage usage)
	{
		data_locked = false;
		texture_format = format;
		size = new_size;
		handles.resize(1);
		handles.front()->texture.clear();

		D3D11_TEXTURE2D_DESC texture_desc;
		texture_desc.Width = size.width;
		texture_desc.Height = size.height;
		texture_desc.MipLevels = 1;
		texture_desc.ArraySize = 1;
		texture_desc.Format = D3DTextureProvider::to_d3d_format(format);
		texture_desc.SampleDesc.Count = 1;
		texture_desc.SampleDesc.Quality = 0;
		texture_desc.Usage = D3D11_USAGE_STAGING;
		texture_desc.BindFlags = 0;
		texture_desc.CPUAccessFlags = to_d3d_cpu_access(direction);
		texture_desc.MiscFlags = D3D11_RESOURCE_MISC_SHARED;

		if ((usage == BufferUsage::stream_draw) && (direction == PixelBufferDirection::data_to_gpu)) // To do: find a buffer usage API that works well for both Direct3D and OpenGL
		{
			texture_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			texture_desc.Usage = D3D11_USAGE_DYNAMIC;
			texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			texture_desc.MiscFlags = 0;
		}

		HRESULT result = handles.front()->device->CreateTexture2D(&texture_desc, 0, handles.front()->texture.output_variable());
		D3DTarget::throw_if_failed("ID3D11Device.CreateTexture2D failed", result);
	}

	void *D3DPixelBufferProvider::get_data()
	{
		if (!data_locked)
			throw Exception("lock() not called before get_data()");

		return map_data.pData;
	}

	ComPtr<ID3D11Texture2D> &D3DPixelBufferProvider::get_texture_2d(const ComPtr<ID3D11Device> &device)
	{
		if (device)
			return get_handles(device).texture;
		else
			return handles.front()->texture;
	}

	int D3DPixelBufferProvider::get_pitch() const
	{
		if (!data_locked)
			throw Exception("lock() not called before get_pitch()");

		return map_data.RowPitch;
	}


	void D3DPixelBufferProvider::lock(GraphicContext &gc, BufferAccess access)
	{
		map_gc_provider = static_cast<D3DGraphicContextProvider *>(gc.get_provider());
		DeviceHandles &handle = get_handles(map_gc_provider->get_window()->get_device());

		HRESULT result = map_gc_provider->get_window()->get_device_context()->Map(handle.texture, 0, to_d3d_map_type(access), 0, &map_data);

		D3DTarget::throw_if_failed("ID3D11DeviceContext.Map failed", result);
		data_locked = true;
	}

	void D3DPixelBufferProvider::unlock()
	{
		map_gc_provider->get_window()->get_device_context()->Unmap(get_handles(map_gc_provider->get_window()->get_device()).texture, 0);
		map_gc_provider = 0;
		map_data.pData = 0;
		map_data.RowPitch = 0;
		map_data.DepthPitch = 0;
		data_locked = false;
	}

	void D3DPixelBufferProvider::upload_data(GraphicContext &gc, const Rect &dest_rect, const void *data)
	{
		D3DGraphicContextProvider *gc_provider = static_cast<D3DGraphicContextProvider *>(gc.get_provider());
		DeviceHandles &handle = get_handles(gc_provider->get_window()->get_device());

		D3D11_BOX box;
		box.left = dest_rect.left;
		box.top = dest_rect.top;
		box.right = dest_rect.right;
		box.bottom = dest_rect.bottom;
		box.front = 0;
		box.back = 1;

		D3D11_TEXTURE2D_DESC texture_desc;
		handle.texture->GetDesc(&texture_desc);
		int pitch = D3DTextureProvider::get_bytes_per_pixel(texture_desc.Format) * texture_desc.Width;

		gc_provider->get_window()->get_device_context()->UpdateSubresource(handle.texture, 0, &box, data, pitch, pitch * dest_rect.get_height());
	}

	void D3DPixelBufferProvider::device_destroyed(ID3D11Device *device)
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

	D3DPixelBufferProvider::DeviceHandles &D3DPixelBufferProvider::get_handles(const ComPtr<ID3D11Device> &device)
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

		handles.push_back(std::make_shared<DeviceHandles>(device));
		handles.back()->texture = texture_handle;
		return *handles.back();
	}

	D3D11_MAP D3DPixelBufferProvider::to_d3d_map_type(BufferAccess access)
	{
		switch (access)
		{
		case BufferAccess::read_only:
			return D3D11_MAP_READ;
		case BufferAccess::write_only:
			return D3D11_MAP_WRITE;
		case BufferAccess::write_discard:
			return D3D11_MAP_WRITE_DISCARD;
		case BufferAccess::read_write:
			return D3D11_MAP_READ_WRITE;
		}
		throw Exception("Unsupported access type");
	}

	UINT D3DPixelBufferProvider::to_d3d_cpu_access(PixelBufferDirection direction)
	{
		switch (direction)
		{
		case PixelBufferDirection::data_to_gpu:
			return D3D11_CPU_ACCESS_WRITE;
		case PixelBufferDirection::data_from_gpu:
			return D3D11_CPU_ACCESS_READ;
		}
		throw Exception("Unsupported pixel buffer direction");
	}
}
