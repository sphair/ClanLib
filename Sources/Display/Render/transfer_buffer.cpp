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
#include "API/Display/Render/transfer_buffer.h"
#include "API/Display/TargetProviders/transfer_buffer_provider.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/Core/System/exception.h"

namespace clan
{
	class TransferBuffer_Impl
	{
	public:
		TransferBuffer_Impl() : lock_count(0), provider(nullptr) { }
		~TransferBuffer_Impl() { if (provider) delete provider; }

		int lock_count;
		TransferBufferProvider *provider;
	};

	TransferBuffer::TransferBuffer()
	{
	}

	TransferBuffer::TransferBuffer(GraphicContext &gc, int size, BufferUsage usage)
		: impl(std::make_shared<TransferBuffer_Impl>())
	{
		GraphicContextProvider *gc_provider = gc.get_provider();
		impl->provider = gc_provider->alloc_transfer_buffer();
		impl->provider->create(size, usage);
	}

	TransferBuffer::TransferBuffer(GraphicContext &gc, const void *data, int size, BufferUsage usage)
		: impl(std::make_shared<TransferBuffer_Impl>())
	{
		GraphicContextProvider *gc_provider = gc.get_provider();
		impl->provider = gc_provider->alloc_transfer_buffer();
		impl->provider->create((void*)data, size, usage);
	}

	TransferBuffer::~TransferBuffer()
	{
	}

	void TransferBuffer::throw_if_null() const
	{
		if (!impl)
			throw Exception("TransferBuffer is null");
	}

	void *TransferBuffer::get_data()
	{
		if (impl->lock_count <= 0)
			throw Exception("Attempted to retrieve buffer data without locking it first.");
		return impl->provider->get_data();
	}

	TransferBufferProvider *TransferBuffer::get_provider() const
	{
		return impl->provider;
	}

	bool TransferBuffer::operator==(const TransferBuffer &other) const
	{
		return impl == other.impl;
	}

	void TransferBuffer::lock(GraphicContext &gc, BufferAccess access)
	{
		impl->lock_count++;
		if (impl->lock_count == 1)
		{
			impl->provider->lock(gc, access);
		}
	}

	void TransferBuffer::unlock()
	{
		impl->lock_count--;
		if (impl->lock_count == 0)
		{
			impl->provider->unlock();
		}
	}

	void TransferBuffer::upload_data(GraphicContext &gc, int offset, const void *data, int size)
	{
		impl->provider->upload_data(gc, offset, data, size);
	}
}
