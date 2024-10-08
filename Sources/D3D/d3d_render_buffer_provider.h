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

#include "API/Display/TargetProviders/render_buffer_provider.h"
#include "API/Display/Image/pixel_buffer.h"

namespace clan
{
	class D3DRenderBufferProvider : public RenderBufferProvider
	{
	public:
		D3DRenderBufferProvider(const ComPtr<ID3D11Device> &device);
		~D3DRenderBufferProvider() override;

		ComPtr<ID3D11Texture2D> &get_texture(const ComPtr<ID3D11Device> &device);
		ComPtr<ID3D11RenderTargetView> create_rtv(const ComPtr<ID3D11Device> &device);
		ComPtr<ID3D11DepthStencilView> create_dsv(const ComPtr<ID3D11Device> &device);

		void create(int width, int height, TextureFormat texture_format, int multisample_samples) override;

	private:
		struct DeviceHandles
		{
			DeviceHandles(const ComPtr<ID3D11Device> &device) : device(device) { }

			ComPtr<ID3D11Device> device;
			ComPtr<ID3D11Texture2D> texture;
		};

		void device_destroyed(ID3D11Device *device);
		DeviceHandles &get_handles(const ComPtr<ID3D11Device> &device);

		std::vector<std::shared_ptr<DeviceHandles> > handles;
	};
}
