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

#include "Display/precomp.h"
#include "API/Display/Render/storage_buffer.h"
#include "API/Display/Render/program_object.h"
#include "API/Display/TargetProviders/storage_buffer_provider.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/Core/System/exception.h"

namespace clan
{
	class StorageBuffer_Impl
	{
	public:
		StorageBuffer_Impl() : provider(nullptr) { }
		~StorageBuffer_Impl() { if (provider) delete provider; }

		StorageBufferProvider *provider;
	};

	StorageBuffer::StorageBuffer()
	{
	}

	StorageBuffer::StorageBuffer(GraphicContext &gc, int size, int stride, BufferUsage usage)
		: impl(std::make_shared<StorageBuffer_Impl>())
	{
		GraphicContextProvider *gc_provider = gc.get_provider();
		impl->provider = gc_provider->alloc_storage_buffer();
		impl->provider->create(size, stride, usage);
	}

	StorageBuffer::StorageBuffer(GraphicContext &gc, const void *data, int size, int stride, BufferUsage usage)
		: impl(std::make_shared<StorageBuffer_Impl>())
	{
		GraphicContextProvider *gc_provider = gc.get_provider();
		impl->provider = gc_provider->alloc_storage_buffer();
		impl->provider->create(data, size, stride, usage);
	}

	void StorageBuffer::throw_if_null() const
	{
		if (!impl)
			throw Exception("StorageBuffer is null");
	}

	StorageBufferProvider *StorageBuffer::get_provider() const
	{
		return impl->provider;
	}

	bool StorageBuffer::operator==(const StorageBuffer &other) const
	{
		return impl == other.impl;
	}

	void StorageBuffer::upload_data(GraphicContext &gc, const void *data, int size)
	{
		impl->provider->upload_data(gc, data, size);
	}

	void StorageBuffer::copy_from(GraphicContext &gc, TransferBuffer &buffer, int dest_pos, int src_pos, int size)
	{
		impl->provider->copy_from(gc, buffer, dest_pos, src_pos, size);
	}

	void StorageBuffer::copy_to(GraphicContext &gc, TransferBuffer &buffer, int dest_pos, int src_pos, int size)
	{
		impl->provider->copy_to(gc, buffer, dest_pos, src_pos, size);
	}
}
