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
#include "d3d_vertex_array_buffer_provider.h"
#include "d3d_graphic_context_provider.h"
#include "d3d_display_window_provider.h"
#include "d3d_transfer_buffer_provider.h"
#include "API/D3D/d3d_target.h"

namespace clan
{
	D3DVertexArrayBufferProvider::D3DVertexArrayBufferProvider(const ComPtr<ID3D11Device> &device)
		: size(0)
	{
		handles.push_back(std::shared_ptr<DeviceHandles>(new DeviceHandles(device)));
	}

	D3DVertexArrayBufferProvider::~D3DVertexArrayBufferProvider()
	{
	}

	void D3DVertexArrayBufferProvider::create(int new_size, BufferUsage usage)
	{
		size = new_size;

		D3D11_BUFFER_DESC desc;
		desc.ByteWidth = new_size;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = D3D11_RESOURCE_MISC_SHARED;
		desc.StructureByteStride = 0;
		HRESULT result = handles.front()->device->CreateBuffer(&desc, 0, handles.front()->buffer.output_variable());
		D3DTarget::throw_if_failed("Unable to create vertex array buffer", result);
	}

	void D3DVertexArrayBufferProvider::create(void *init_data, int new_size, BufferUsage usage)
	{
		size = new_size;

		D3D11_SUBRESOURCE_DATA resource_data;
		resource_data.pSysMem = init_data;
		resource_data.SysMemPitch = 0;
		resource_data.SysMemSlicePitch = 0;

		D3D11_BUFFER_DESC desc;
		desc.ByteWidth = new_size;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = D3D11_RESOURCE_MISC_SHARED;
		desc.StructureByteStride = 0;
		HRESULT result = handles.front()->device->CreateBuffer(&desc, &resource_data, handles.front()->buffer.output_variable());
		D3DTarget::throw_if_failed("Unable to create vertex array buffer", result);
	}

	ComPtr<ID3D11Buffer> &D3DVertexArrayBufferProvider::get_buffer(const ComPtr<ID3D11Device> &device)
	{
		if (device)
			return get_handles(device).buffer;
		else
			return handles.front()->buffer;
	}

	void D3DVertexArrayBufferProvider::upload_data(GraphicContext &gc, int offset, const void *data, int data_size)
	{
		if ((offset < 0) || (data_size < 0) || ((data_size + offset) > size))
			throw Exception("Vertex array buffer, invalid size");

		const ComPtr<ID3D11Device> &device = static_cast<D3DGraphicContextProvider*>(gc.get_provider())->get_window()->get_device();
		ComPtr<ID3D11DeviceContext> device_context;
		device->GetImmediateContext(device_context.output_variable());

		D3D11_BOX box;
		box.left = offset;
		box.right = offset + data_size;
		box.top = 0;
		box.bottom = 1;
		box.front = 0;
		box.back = 1;

		device_context->UpdateSubresource(get_handles(device).buffer, 0, &box, data, 0, 0);
	}

	void D3DVertexArrayBufferProvider::copy_from(GraphicContext &gc, TransferBuffer &buffer, int dest_pos, int src_pos, int copy_size)
	{
		const ComPtr<ID3D11Device> &device = static_cast<D3DGraphicContextProvider*>(gc.get_provider())->get_window()->get_device();
		ComPtr<ID3D11Buffer> &transfer_buffer = static_cast<D3DTransferBufferProvider*>(buffer.get_provider())->get_buffer(device);
		int transfer_buffer_size = static_cast<D3DTransferBufferProvider*>(buffer.get_provider())->get_size();

		if (copy_size == -1)
			copy_size = transfer_buffer_size;

		if (dest_pos < 0 || copy_size < 0 || dest_pos + copy_size > size || src_pos < 0 || src_pos + copy_size > transfer_buffer_size)
			throw Exception("Out of bounds!");

		ComPtr<ID3D11DeviceContext> device_context;
		device->GetImmediateContext(device_context.output_variable());

		D3D11_BOX box;
		box.left = src_pos;
		box.right = src_pos + copy_size;
		box.top = 0;
		box.bottom = 1;
		box.front = 0;
		box.back = 1;
		device_context->CopySubresourceRegion(get_handles(device).buffer, 0, dest_pos, 0, 0, transfer_buffer, 0, &box);
	}

	void D3DVertexArrayBufferProvider::copy_to(GraphicContext &gc, TransferBuffer &buffer, int dest_pos, int src_pos, int copy_size)
	{
		const ComPtr<ID3D11Device> &device = static_cast<D3DGraphicContextProvider*>(gc.get_provider())->get_window()->get_device();
		ComPtr<ID3D11Buffer> &transfer_buffer = static_cast<D3DTransferBufferProvider*>(buffer.get_provider())->get_buffer(device);
		int transfer_buffer_size = static_cast<D3DTransferBufferProvider*>(buffer.get_provider())->get_size();

		if (copy_size == -1)
			copy_size = transfer_buffer_size;

		if (dest_pos < 0 || copy_size < 0 || dest_pos + copy_size > transfer_buffer_size || src_pos < 0 || src_pos + copy_size > size)
			throw Exception("Out of bounds!");

		ComPtr<ID3D11DeviceContext> device_context;
		device->GetImmediateContext(device_context.output_variable());

		D3D11_BOX box;
		box.left = dest_pos;
		box.right = dest_pos + copy_size;
		box.top = 0;
		box.bottom = 1;
		box.front = 0;
		box.back = 1;
		device_context->CopySubresourceRegion(transfer_buffer, 0, src_pos, 0, 0, get_handles(device).buffer, 0, &box);
	}

	void D3DVertexArrayBufferProvider::device_destroyed(ID3D11Device *device)
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

	D3DVertexArrayBufferProvider::DeviceHandles &D3DVertexArrayBufferProvider::get_handles(const ComPtr<ID3D11Device> &device)
	{
		for (size_t i = 0; i < handles.size(); i++)
			if (handles[i]->device == device)
				return *handles[i];

		ComPtr<IDXGIResource> resource;
		HRESULT result = handles.front()->buffer->QueryInterface(__uuidof(IDXGIResource), (void**)resource.output_variable());
		D3DTarget::throw_if_failed("ID3D11Buffer.QueryInterface(IDXGIResource) failed", result);

		HANDLE handle = 0;
		result = resource->GetSharedHandle(&handle);
		D3DTarget::throw_if_failed("IDXGIResource.GetSharedHandle failed", result);

		ComPtr<ID3D11Buffer> buffer_handle;
		result = device->OpenSharedResource(handle, __uuidof(ID3D11Buffer), (void**)buffer_handle.output_variable());
		D3DTarget::throw_if_failed("ID3D11Device.OpenSharedResource failed", result);

		handles.push_back(std::shared_ptr<DeviceHandles>(new DeviceHandles(device)));
		handles.back()->buffer = buffer_handle;
		return *handles.back();
	}

	D3D11_MAP D3DVertexArrayBufferProvider::to_d3d_map_type(BufferAccess access)
	{
		switch (access)
		{
		case access_read_only:
			return D3D11_MAP_READ;
		case access_write_only:
			return D3D11_MAP_WRITE;
		case access_write_discard:
			return D3D11_MAP_WRITE_DISCARD;
		case access_read_write:
			return D3D11_MAP_READ_WRITE;
		}
		throw Exception("Unsupported access type");
	}
}
