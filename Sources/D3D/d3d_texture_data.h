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

#include "d3d_share_list.h"
#include "API/Display/TargetProviders/texture_provider.h"

namespace clan
{
	class D3DTextureData : D3DSharedResource
	{
	public:
		D3DTextureData(const ComPtr<ID3D11Device> &device, D3D_FEATURE_LEVEL feature_level, TextureDimensions texture_dimensions);
		~D3DTextureData();

		enum class DeviceTextureType
		{
			ID3D11Texture1D,
			ID3D11Texture2D,
			ID3D11Texture3D
		};

		struct DeviceHandles
		{
			DeviceHandles(const ComPtr<ID3D11Device> &device, DeviceTextureType texture_type) : device(device), texture_type(texture_type) { }

			ID3D11Texture1D *get_texture_1d()
			{
				if (texture_type != DeviceTextureType::ID3D11Texture1D)
					throw Exception("texture_type not ID3D11Texture1D");
				return static_cast<ID3D11Texture1D *>(texture.get());
			}

			ID3D11Texture2D *get_texture_2d()
			{
				if (texture_type != DeviceTextureType::ID3D11Texture2D)
					throw Exception("texture_type not ID3D11Texture2D");
				return static_cast<ID3D11Texture2D *>(texture.get());
			}

			ID3D11Texture3D *get_texture_3d()
			{
				if (texture_type != DeviceTextureType::ID3D11Texture3D)
					throw Exception("texture_type not ID3D11Texture3D");
				return static_cast<ID3D11Texture3D *>(texture.get());
			}

			ComPtr<ID3D11Device> device;
			ComPtr<ID3D11Resource> texture;
			DeviceTextureType texture_type;
		};

		void device_destroyed(ID3D11Device *device);
		DeviceHandles &get_handles(const ComPtr<ID3D11Device> &device) const;
		DeviceTextureType decode_texture_type(TextureDimensions texture_dimensions) const;
		void attach_to_another_device(ID3D11Device *not_this_device);

		mutable std::vector<std::shared_ptr<DeviceHandles> > handles;
		D3D_FEATURE_LEVEL feature_level;
		TextureDimensions texture_dimensions;
	};
}
