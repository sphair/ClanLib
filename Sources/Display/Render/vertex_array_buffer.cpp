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

#include "Display/precomp.h"
#include "API/Display/Render/vertex_array_buffer.h"
#include "API/Display/TargetProviders/vertex_array_buffer_provider.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/Core/System/exception.h"

namespace clan
{
	class VertexArrayBuffer_Impl
	{
	public:
		VertexArrayBuffer_Impl() : lock_count(0), provider(nullptr) { }
		~VertexArrayBuffer_Impl() { if (provider) delete provider; }

		int lock_count;
		VertexArrayBufferProvider *provider;
	};

	VertexArrayBuffer::VertexArrayBuffer()
	{
	}

	VertexArrayBuffer::VertexArrayBuffer(GraphicContext &gc, int size, BufferUsage usage)
		: impl(std::make_shared<VertexArrayBuffer_Impl>())
	{
		GraphicContextProvider *gc_provider = gc.get_provider();
		impl->provider = gc_provider->alloc_vertex_array_buffer();
		impl->provider->create(size, usage);
	}

	VertexArrayBuffer::VertexArrayBuffer(GraphicContext &gc, const void *data, int size, BufferUsage usage)
		: impl(std::make_shared<VertexArrayBuffer_Impl>())
	{
		GraphicContextProvider *gc_provider = gc.get_provider();
		impl->provider = gc_provider->alloc_vertex_array_buffer();
		impl->provider->create((void*)data, size, usage);
	}

	VertexArrayBuffer::~VertexArrayBuffer()
	{
	}

	void VertexArrayBuffer::throw_if_null() const
	{
		if (!impl)
			throw Exception("VertexArrayBuffer is null");
	}

	VertexArrayBufferProvider *VertexArrayBuffer::get_provider() const
	{
		return impl->provider;
	}

	bool VertexArrayBuffer::operator==(const VertexArrayBuffer &other) const
	{
		return impl == other.impl;
	}

	void VertexArrayBuffer::upload_data(GraphicContext &gc, int offset, const void *data, int size)
	{
		impl->provider->upload_data(gc, offset, data, size);
	}

	void VertexArrayBuffer::copy_from(GraphicContext &gc, TransferBuffer &buffer, int dest_pos, int src_pos, int size)
	{
		impl->provider->copy_from(gc, buffer, dest_pos, src_pos, size);
	}

	void VertexArrayBuffer::copy_to(GraphicContext &gc, TransferBuffer &buffer, int dest_pos, int src_pos, int size)
	{
		impl->provider->copy_to(gc, buffer, dest_pos, src_pos, size);
	}
}
