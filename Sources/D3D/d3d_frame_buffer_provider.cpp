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
#include "d3d_frame_buffer_provider.h"
#include "API/Display/Render/texture_1d.h"
#include "API/Display/Render/texture_1d_array.h"
#include "API/Display/Render/texture_2d.h"
#include "API/Display/Render/texture_2d_array.h"
#include "API/Display/Render/texture_3d.h"
#include "API/Display/Render/texture_cube.h"

namespace clan
{
	D3DFrameBufferProvider::D3DFrameBufferProvider(const ComPtr<ID3D11Device> &device)
		: device(device), bind_target(framebuffer_draw)
	{
	}

	D3DFrameBufferProvider::~D3DFrameBufferProvider()
	{
	}

	Size D3DFrameBufferProvider::get_size() const
	{
		Size size;
		for (size_t i = 0; i < color_buffers.size(); i++)
		{
			D3D11_TEXTURE2D_DESC desc;
			if (!color_buffers[i].texture.is_null())
				color_buffers[i].get_texture_provider()->get_texture_2d(device)->GetDesc(&desc);
			else if (!color_buffers[i].render_buffer.is_null())
				color_buffers[i].get_render_buffer_provider()->get_texture(device)->GetDesc(&desc);

			size.width = max(desc.Width, UINT(size.width));
			size.height = max(desc.Height, UINT(size.height));
		}
		return size;
	}

	FrameBufferBindTarget D3DFrameBufferProvider::get_bind_target() const
	{
		return bind_target;
	}

	std::vector<ID3D11RenderTargetView*> D3DFrameBufferProvider::get_views(ID3D11DepthStencilView *&out_dsv)
	{
		std::vector<ID3D11RenderTargetView*> views;
		for (size_t i = 0; i < color_buffers.size(); i++)
		{
			if (color_buffers[i].rtv.is_null())
			{
				if (!color_buffers[i].texture.is_null())
				{
					color_buffers[i].rtv = color_buffers[i].get_texture_provider()->create_rtv(device, color_buffers[i].level, color_buffers[i].slice, color_buffers[i].subtype);
				}
				else if (!color_buffers[i].render_buffer.is_null())
				{
					color_buffers[i].rtv = color_buffers[i].get_render_buffer_provider()->create_rtv(device);
				}
			}
			views.push_back(color_buffers[i].rtv);
		}

		if (depth_buffer.dsv.is_null())
		{
			if (!depth_buffer.texture.is_null())
				depth_buffer.dsv = depth_buffer.get_texture_provider()->create_dsv(device, depth_buffer.level, depth_buffer.slice, depth_buffer.subtype);
			else if (!depth_buffer.render_buffer.is_null())
				depth_buffer.dsv = depth_buffer.get_render_buffer_provider()->create_dsv(device);
		}
		out_dsv = depth_buffer.dsv;

		return views;
	}

	void D3DFrameBufferProvider::attach_color(int attachment_index, const RenderBuffer &render_buffer)
	{
		color_buffers.resize(max((size_t)attachment_index + 1, color_buffers.size()));
		color_buffers[attachment_index] = AttachedBuffer(render_buffer);
	}

	void D3DFrameBufferProvider::attach_color(int attachment_index, const Texture1D &texture, int level)
	{
		color_buffers.resize(max((size_t)attachment_index + 1, color_buffers.size()));
		color_buffers[attachment_index] = AttachedBuffer(texture, level);
	}

	void D3DFrameBufferProvider::attach_color(int attachment_index, const Texture1DArray &texture, int array_index, int level)
	{
		color_buffers.resize(max((size_t)attachment_index + 1, color_buffers.size()));
		color_buffers[attachment_index] = AttachedBuffer(texture, level, array_index);
	}

	void D3DFrameBufferProvider::attach_color(int attachment_index, const Texture2D &texture, int level)
	{
		color_buffers.resize(max((size_t)attachment_index + 1, color_buffers.size()));
		color_buffers[attachment_index] = AttachedBuffer(texture, level);
	}

	void D3DFrameBufferProvider::attach_color(int attachment_index, const Texture2DArray &texture, int array_index, int level)
	{
		color_buffers.resize(max((size_t)attachment_index + 1, color_buffers.size()));
		color_buffers[attachment_index] = AttachedBuffer(texture, level, array_index);
	}

	void D3DFrameBufferProvider::attach_color(int attachment_index, const Texture3D &texture, int depth, int level)
	{
		color_buffers.resize(max((size_t)attachment_index + 1, color_buffers.size()));
		color_buffers[attachment_index] = AttachedBuffer(texture, level, depth);
	}

	void D3DFrameBufferProvider::attach_color(int attachment_index, const TextureCube &texture, TextureSubtype subtype, int level)
	{
		color_buffers.resize(max((size_t)attachment_index + 1, color_buffers.size()));
		color_buffers[attachment_index] = AttachedBuffer(texture, level, 0, subtype);
	}

	void D3DFrameBufferProvider::detach_color(int attachment_index)
	{
		color_buffers.resize(max((size_t)attachment_index + 1, color_buffers.size()));
		color_buffers[attachment_index] = AttachedBuffer();
	}

	void D3DFrameBufferProvider::attach_stencil(const RenderBuffer &render_buffer)
	{
		stencil_buffer = AttachedBuffer(render_buffer);
	}

	void D3DFrameBufferProvider::attach_stencil(const Texture2D &texture, int level)
	{
		stencil_buffer = AttachedBuffer(texture, level);
	}

	void D3DFrameBufferProvider::attach_stencil(const TextureCube &texture, TextureSubtype subtype, int level)
	{
		stencil_buffer = AttachedBuffer(texture, level, 0, subtype);
	}

	void D3DFrameBufferProvider::detach_stencil()
	{
		stencil_buffer = AttachedBuffer();
	}

	void D3DFrameBufferProvider::attach_depth(const RenderBuffer &render_buffer)
	{
		depth_buffer = AttachedBuffer(render_buffer);
	}

	void D3DFrameBufferProvider::attach_depth(const Texture2D &texture, int level)
	{
		depth_buffer = AttachedBuffer(texture, level);
	}

	void D3DFrameBufferProvider::attach_depth(const TextureCube &texture, TextureSubtype subtype, int level)
	{
		depth_buffer = AttachedBuffer(texture, level, 0, subtype);
	}

	void D3DFrameBufferProvider::detach_depth()
	{
		depth_buffer = AttachedBuffer();
	}

	void D3DFrameBufferProvider::attach_depth_stencil(const RenderBuffer &render_buffer)
	{
		depth_buffer = AttachedBuffer(render_buffer);
		stencil_buffer = AttachedBuffer(render_buffer);
	}

	void D3DFrameBufferProvider::attach_depth_stencil(const Texture2D &texture, int level)
	{
		depth_buffer = AttachedBuffer(texture, level);
		stencil_buffer = AttachedBuffer(texture, level);
	}

	void D3DFrameBufferProvider::attach_depth_stencil(const TextureCube &texture, TextureSubtype subtype, int level)
	{
		depth_buffer = AttachedBuffer(texture, level, 0, subtype);
		stencil_buffer = AttachedBuffer(texture, level, 0, subtype);
	}

	void D3DFrameBufferProvider::detach_depth_stencil()
	{
		depth_buffer = AttachedBuffer();
		stencil_buffer = AttachedBuffer();
	}

	void D3DFrameBufferProvider::set_bind_target(FrameBufferBindTarget target)
	{
		bind_target = target;
	}
}
