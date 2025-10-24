/*
**  ClanLib SDK
**  Copyright (c) 1997-2010 The ClanLib Team
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
#include "API/Display/Render/element_array_buffer.h"
#include "API/Display/TargetProviders/element_array_buffer_provider.h"
#include "API/Display/Render/graphic_context.h"
#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/Core/System/exception.h"

/////////////////////////////////////////////////////////////////////////////
// CL_ElementArrayBuffer_Impl class:

class CL_ElementArrayBuffer_Impl
{
public:
	CL_ElementArrayBuffer_Impl() : lock_count(0), provider(0) { }

	~CL_ElementArrayBuffer_Impl() { if (provider) provider->destroy(); }

	int lock_count;

	CL_ElementArrayBufferProvider *provider;
};

/////////////////////////////////////////////////////////////////////////////
// CL_ElementArrayBuffer Construction:

CL_ElementArrayBuffer::CL_ElementArrayBuffer()
{
}

CL_ElementArrayBuffer::CL_ElementArrayBuffer(CL_GraphicContext &gc, int size, CL_BufferUsage usage)
: impl(new CL_ElementArrayBuffer_Impl)
{
	CL_GraphicContextProvider *gc_provider = gc.get_provider();
	impl->provider = gc_provider->alloc_element_array_buffer();
	impl->provider->create(size, usage);
}

CL_ElementArrayBuffer::CL_ElementArrayBuffer(CL_GraphicContext &gc, void *data, int size, CL_BufferUsage usage)
: impl(new CL_ElementArrayBuffer_Impl)
{
	CL_GraphicContextProvider *gc_provider = gc.get_provider();
	impl->provider = gc_provider->alloc_element_array_buffer();
	impl->provider->create(data, size, usage);
}

CL_ElementArrayBuffer::~CL_ElementArrayBuffer()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_ElementArrayBuffer Attributes:

void CL_ElementArrayBuffer::throw_if_null() const
{
	if (impl.is_null())
		throw CL_Exception("CL_ElementArrayBuffer is null");
}

void *CL_ElementArrayBuffer::get_data()
{
	if (impl->lock_count <= 0)
		throw CL_Exception("Attempted to retrieve element array buffer data without locking it first.");
	return impl->provider->get_data();
}

CL_ElementArrayBufferProvider *CL_ElementArrayBuffer::get_provider() const
{
	return impl->provider;
}

/////////////////////////////////////////////////////////////////////////////
// CL_ElementArrayBuffer Operations:

bool CL_ElementArrayBuffer::operator==(const CL_ElementArrayBuffer &other) const
{
	return impl == other.impl;
}

void CL_ElementArrayBuffer::lock(CL_BufferAccess access)
{
	impl->lock_count++;
	if (impl->lock_count == 1)
	{
		impl->provider->lock(access);
	}
}

void CL_ElementArrayBuffer::unlock()
{
	impl->lock_count--;
	if (impl->lock_count == 0)
	{
		impl->provider->unlock();
	}
}

void CL_ElementArrayBuffer::upload_data(int offset, void *data, int size)
{
	impl->provider->upload_data(offset, data, size);
}

/////////////////////////////////////////////////////////////////////////////
// CL_ElementArrayBuffer Implementation:
