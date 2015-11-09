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

#include "API/Display/TargetProviders/pixel_buffer_provider.h"
#include "d3d_share_list.h"

namespace clan
{
	class D3DGraphicContextProvider;

	class D3DPixelBufferProvider : public PixelBufferProvider, D3DSharedResource
	{
	public:
		D3DPixelBufferProvider(const ComPtr<ID3D11Device> &device);
		~D3DPixelBufferProvider();
		void create(const void *data, const Size &new_size, PixelBufferDirection direction, TextureFormat new_format, BufferUsage usage);

		void *get_data();
		int get_pitch() const;

		ComPtr<ID3D11Texture2D> &get_texture_2d(const ComPtr<ID3D11Device> &device);
		Size get_size() const { return size; }
		bool is_gpu() const { return true; }

		TextureFormat get_format() const { return texture_format; };

		void lock(GraphicContext &gc, BufferAccess access);
		void unlock();
		void upload_data(GraphicContext &gc, const Rect &dest_rect, const void *data);

	private:
		struct DeviceHandles
		{
			DeviceHandles(const ComPtr<ID3D11Device> &device) : device(device) { }

			ComPtr<ID3D11Device> device;
			ComPtr<ID3D11Texture2D> texture;
		};

		void device_destroyed(ID3D11Device *device);
		DeviceHandles &get_handles(const ComPtr<ID3D11Device> &device);

		static D3D11_MAP to_d3d_map_type(BufferAccess access);
		static UINT to_d3d_cpu_access(PixelBufferDirection direction);

		std::vector<std::shared_ptr<DeviceHandles> > handles;
		D3D11_MAPPED_SUBRESOURCE map_data;
		D3DGraphicContextProvider *map_gc_provider;

		Size size;
		TextureFormat texture_format;
		bool data_locked;	// lock() has been called
	};
}
