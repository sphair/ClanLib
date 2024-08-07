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
**    Harry Storbacka
*/

#include "Display/precomp.h"
#include "API/Display/Render/frame_buffer.h"
#include "API/Display/Render/texture_2d.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/Display/TargetProviders/frame_buffer_provider.h"

namespace clan
{
	class FrameBuffer_Impl
	{
	public:
		std::unique_ptr<FrameBufferProvider> provider;
		float pixel_ratio = 0.0f;
	};

	FrameBuffer::FrameBuffer()
	{
	}

	FrameBuffer::FrameBuffer(GraphicContext &context)
		: impl(std::make_shared<FrameBuffer_Impl>())
	{
		GraphicContextProvider *gc_provider = context.get_provider();
		impl->provider = gc_provider->alloc_frame_buffer();
	}

	float FrameBuffer::get_pixel_ratio() const
	{
		return impl->pixel_ratio;
	}

	void FrameBuffer::throw_if_null() const
	{
		if (!impl)
			throw Exception("FrameBuffer is null");
	}

	FrameBufferProvider *FrameBuffer::get_provider() const
	{
		return impl->provider.get();
	}

	Size FrameBuffer::get_size() const
	{
		return impl->provider->get_size();
	}

	FrameBufferBindTarget FrameBuffer::get_bind_target() const
	{
		return impl->provider->get_bind_target();
	}

	bool FrameBuffer::operator==(const FrameBuffer &other) const
	{
		return impl == other.impl;
	}

	void FrameBuffer::attach_color(int attachment_index, const RenderBuffer &render_buffer)
	{
		impl->provider->attach_color(attachment_index, render_buffer);
	}

	void FrameBuffer::attach_color(int attachment_index, const Texture1D &texture, int level)
	{
		impl->provider->attach_color(attachment_index, texture, level);
	}

	void FrameBuffer::attach_color(int attachment_index, const Texture1DArray &texture, int array_index, int level)
	{
		impl->provider->attach_color(attachment_index, texture, array_index, level);
	}

	void FrameBuffer::attach_color(int attachment_index, const Texture2D &texture, int level)
	{
		if (impl->pixel_ratio == 0.0f)
			impl->pixel_ratio = texture.get_pixel_ratio();

		impl->provider->attach_color(attachment_index, texture, level);
	}

	void FrameBuffer::attach_color(int attachment_index, const Texture2DArray &texture, int array_index, int level)
	{
		impl->provider->attach_color(attachment_index, texture, array_index, level);
	}

	void FrameBuffer::attach_color(int attachment_index, const Texture3D &texture, int depth, int level)
	{
		impl->provider->attach_color(attachment_index, texture, depth, level);
	}

	void FrameBuffer::attach_color(int attachment_index, const TextureCube &texture, TextureSubtype subtype, int level)
	{
		impl->provider->attach_color(attachment_index, texture, subtype, level);
	}

	void FrameBuffer::detach_color(int attachment_index)
	{
		impl->provider->detach_color(attachment_index);
	}

	void FrameBuffer::attach_stencil(const RenderBuffer &render_buffer)
	{
		impl->provider->attach_stencil(render_buffer);
	}

	void FrameBuffer::attach_stencil(const Texture2D &texture, int level)
	{
		if (impl->pixel_ratio == 0.0f)
			impl->pixel_ratio = texture.get_pixel_ratio();
		impl->provider->attach_stencil(texture, level);
	}

	void FrameBuffer::attach_stencil(const TextureCube &texture, TextureSubtype subtype, int level)
	{
		impl->provider->attach_stencil(texture, subtype, level);
	}

	void FrameBuffer::detach_stencil()
	{
		impl->provider->detach_stencil();
	}

	void FrameBuffer::attach_depth(const RenderBuffer &render_buffer)
	{
		impl->provider->attach_depth(render_buffer);
	}

	void FrameBuffer::attach_depth(const Texture2D &texture, int level)
	{
		if (impl->pixel_ratio == 0.0f)
			impl->pixel_ratio = texture.get_pixel_ratio();
		impl->provider->attach_depth(texture, level);
	}

	void FrameBuffer::attach_depth(const TextureCube &texture, TextureSubtype subtype, int level)
	{
		impl->provider->attach_depth(texture, subtype, level);
	}

	void FrameBuffer::detach_depth()
	{
		impl->provider->detach_depth();
	}

	void FrameBuffer::attach_depth_stencil(const RenderBuffer &render_buffer)
	{
		impl->provider->attach_depth_stencil(render_buffer);
	}

	void FrameBuffer::attach_depth_stencil(const Texture2D &texture, int level)
	{
		if (impl->pixel_ratio == 0.0f)
			impl->pixel_ratio = texture.get_pixel_ratio();
		impl->provider->attach_depth_stencil(texture, level);
	}

	void FrameBuffer::attach_depth_stencil(const TextureCube &texture, TextureSubtype subtype, int level)
	{
		impl->provider->attach_depth_stencil(texture, subtype, level);
	}

	void FrameBuffer::detach_depth_stencil()
	{
		impl->provider->detach_depth_stencil();
	}

	void FrameBuffer::set_bind_target(FrameBufferBindTarget target)
	{
		impl->provider->set_bind_target(target);
	}
}
