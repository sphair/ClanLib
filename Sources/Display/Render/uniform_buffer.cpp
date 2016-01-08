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
#include "API/Display/Render/uniform_buffer.h"
#include "API/Display/Render/program_object.h"
#include "API/Display/TargetProviders/uniform_buffer_provider.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/Core/System/exception.h"

namespace clan
{
	class UniformBuffer_Impl
	{
	public:
		UniformBuffer_Impl() : lock_count(0), provider(nullptr) { }
		~UniformBuffer_Impl() { if (provider) delete provider; }

		int lock_count;
		UniformBufferProvider *provider;
	};

	UniformBuffer::UniformBuffer()
	{
	}

	UniformBuffer::UniformBuffer(GraphicContext &gc, int size, BufferUsage usage)
		: impl(std::make_shared<UniformBuffer_Impl>())
	{
		GraphicContextProvider *gc_provider = gc.get_provider();
		impl->provider = gc_provider->alloc_uniform_buffer();
		impl->provider->create(size, usage);
	}

	UniformBuffer::UniformBuffer(GraphicContext &gc, const void *data, int size, BufferUsage usage)
		: impl(std::make_shared<UniformBuffer_Impl>())
	{
		GraphicContextProvider *gc_provider = gc.get_provider();
		impl->provider = gc_provider->alloc_uniform_buffer();
		impl->provider->create(data, size, usage);
	}

	UniformBuffer::UniformBuffer(GraphicContext &gc, ProgramObject &program, const std::string &name, int num_blocks, BufferUsage usage)
		: impl(std::make_shared<UniformBuffer_Impl>())
	{
		GraphicContextProvider *gc_provider = gc.get_provider();
		impl->provider = gc_provider->alloc_uniform_buffer();
		impl->provider->create(program.get_uniform_buffer_size(name) * num_blocks, usage);
	}

	void UniformBuffer::throw_if_null() const
	{
		if (!impl)
			throw Exception("UniformBuffer is null");
	}

	UniformBufferProvider *UniformBuffer::get_provider() const
	{
		return impl->provider;
	}

	bool UniformBuffer::operator==(const UniformBuffer &other) const
	{
		return impl == other.impl;
	}

	void UniformBuffer::upload_data(GraphicContext &gc, const void *data, int size)
	{
		impl->provider->upload_data(gc, data, size);
	}

	void UniformBuffer::copy_from(GraphicContext &gc, TransferBuffer &buffer, int dest_pos, int src_pos, int size)
	{
		impl->provider->copy_from(gc, buffer, dest_pos, src_pos, size);
	}

	void UniformBuffer::copy_to(GraphicContext &gc, TransferBuffer &buffer, int dest_pos, int src_pos, int size)
	{
		impl->provider->copy_to(gc, buffer, dest_pos, src_pos, size);
	}
}
