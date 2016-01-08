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
**    Harry Storbacka
*/

#include "Display/precomp.h"
#include "API/Display/Render/render_buffer.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/Display/TargetProviders/frame_buffer_provider.h"
#include "API/Display/TargetProviders/render_buffer_provider.h"

namespace clan
{
	class RenderBuffer_Impl
	{
	public:
		RenderBuffer_Impl() : provider(nullptr)
		{
		}

		RenderBufferProvider *provider;
		Size size;
	};

	RenderBuffer::RenderBuffer()
	{
	}

	RenderBuffer::RenderBuffer(GraphicContext &context, int width, int height, TextureFormat texture_format, int multisample_samples)
		: impl(std::make_shared<RenderBuffer_Impl>())
	{
		GraphicContextProvider *gc_provider = context.get_provider();
		impl->provider = gc_provider->alloc_render_buffer();
		impl->provider->create(width, height, texture_format, multisample_samples);
		impl->size.width = width;
		impl->size.height = height;
	}

	void RenderBuffer::throw_if_null() const
	{
		if (!impl)
			throw Exception("RenderBuffer is null");
	}

	RenderBufferProvider *RenderBuffer::get_provider() const
	{
		return impl->provider;
	}

	const Size &RenderBuffer::get_size() const
	{
		return impl->size;
	}

	bool RenderBuffer::operator==(const RenderBuffer &other) const
	{
		return impl == other.impl;
	}
}
