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

#pragma once

#include "API/Display/TargetProviders/frame_buffer_provider.h"
#include "API/Display/Render/render_buffer.h"
#include "API/Display/Render/texture_2d.h"
#include "d3d_render_buffer_provider.h"
#include "d3d_texture_provider.h"

namespace clan
{
	class D3DFrameBufferProvider : public FrameBufferProvider
	{
	public:
		D3DFrameBufferProvider(const ComPtr<ID3D11Device> &device);
		~D3DFrameBufferProvider();

		Size get_size() const;
		FrameBufferBindTarget get_bind_target() const;

		ComPtr<ID3D11Device> &get_device() { return device; }
		std::vector<ID3D11RenderTargetView*> get_views(ID3D11DepthStencilView *&out_dsv);

		void attach_color(int attachment_index, const RenderBuffer &render_buffer);
		void attach_color(int attachment_index, const Texture1D &texture, int level);
		void attach_color(int attachment_index, const Texture1DArray &texture, int array_index, int level);
		void attach_color(int attachment_index, const Texture2D &texture, int level);
		void attach_color(int attachment_index, const Texture2DArray &texture, int array_index, int level);
		void attach_color(int attachment_index, const Texture3D &texture, int depth, int level);
		void attach_color(int attachment_index, const TextureCube &texture, TextureSubtype subtype, int level);
		void detach_color(int attachment_index);

		void attach_stencil(const RenderBuffer &render_buffer);
		void attach_stencil(const Texture2D &texture, int level);
		void attach_stencil(const TextureCube &texture, TextureSubtype subtype, int level);
		void detach_stencil();

		void attach_depth(const RenderBuffer &render_buffer);
		void attach_depth(const Texture2D &texture, int level);
		void attach_depth(const TextureCube &texture, TextureSubtype subtype, int level);
		void detach_depth();

		void attach_depth_stencil(const RenderBuffer &render_buffer);
		void attach_depth_stencil(const Texture2D &texture, int level);
		void attach_depth_stencil(const TextureCube &texture, TextureSubtype subtype, int level);
		void detach_depth_stencil();

		void set_bind_target(FrameBufferBindTarget target);

	private:
		struct AttachedBuffer
		{
			AttachedBuffer() : level(0), slice(0), subtype() { }
			AttachedBuffer(RenderBuffer render_buffer) : render_buffer(render_buffer), level(0), slice(0), subtype() { }
			AttachedBuffer(Texture texture, int level = 0, int slice = 0, TextureSubtype subtype = TextureSubtype()) : texture(texture), level(level), slice(slice), subtype(subtype) { }

			D3DRenderBufferProvider *get_render_buffer_provider() const { return static_cast<D3DRenderBufferProvider*>(render_buffer.get_provider()); }
			D3DTextureProvider *get_texture_provider() const { return static_cast<D3DTextureProvider*>(texture.get_provider()); }

			RenderBuffer render_buffer;
			Texture texture;
			int level;
			int slice;
			TextureSubtype subtype;

			ComPtr<ID3D11RenderTargetView> rtv;
			ComPtr<ID3D11DepthStencilView> dsv;
		};

		ComPtr<ID3D11Device> device;
		std::vector<AttachedBuffer> color_buffers;
		AttachedBuffer depth_buffer;
		AttachedBuffer stencil_buffer;
		FrameBufferBindTarget bind_target;
	};
}
