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

#include "Core/precomp.h"
#include "API/Core/System/databuffer.h"
#include "API/Core/System/memory_pool.h"
#include <string.h>

/////////////////////////////////////////////////////////////////////////////
// CL_DataBuffer_Impl Class:

class CL_DataBuffer_Impl
{
public:
	CL_DataBuffer_Impl() : pool(0), data(0), size(0), allocated_size(0)
	{
	}

	~CL_DataBuffer_Impl()
	{
		cl_delete(pool, data, size);
	}

public:
	CL_MemoryPool *pool;

	char *data;

	int size;

	int allocated_size;
};

/////////////////////////////////////////////////////////////////////////////
// CL_DataBuffer Construction:

CL_DataBuffer::CL_DataBuffer()
: impl(cl_new((CL_MemoryPool *) 0) CL_DataBuffer_Impl, (CL_MemoryPool *) 0)
{
}

CL_DataBuffer::CL_DataBuffer(int new_size, CL_MemoryPool *pool)
: impl(cl_new(pool) CL_DataBuffer_Impl, pool)
{
	impl->pool = pool;
	set_size(new_size);
}

CL_DataBuffer::CL_DataBuffer(const void *new_data, int new_size, CL_MemoryPool *pool)
: impl(cl_new(pool) CL_DataBuffer_Impl, pool)
{
	impl->pool = pool;
	set_size(new_size);
	memcpy(impl->data, new_data, new_size);
}

CL_DataBuffer::CL_DataBuffer(const CL_DataBuffer &new_data, int pos, int size, CL_MemoryPool *pool)
: impl(cl_new(pool) CL_DataBuffer_Impl, pool)
{
	impl->pool = pool;
	if (size == -1)
		size = new_data.get_size();
	set_size(size);
	memcpy(impl->data, new_data.get_data(), size);
}

CL_DataBuffer::~CL_DataBuffer()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_DataBuffer Attributes:

char *CL_DataBuffer::get_data()
{
	return impl->data;
}

const char *CL_DataBuffer::get_data() const
{
	return impl->data;
}

int CL_DataBuffer::get_size() const
{
	return impl->size;
}

int CL_DataBuffer::get_capacity() const
{
	return impl->allocated_size;
}

char &CL_DataBuffer::operator[](int i)
{
	return impl->data[i];
}

const char &CL_DataBuffer::operator[](int i) const
{
	return impl->data[i];
}

char &CL_DataBuffer::operator[](unsigned int i)
{
	return impl->data[i];
}

const char &CL_DataBuffer::operator[](unsigned int i) const
{
	return impl->data[i];
}

/////////////////////////////////////////////////////////////////////////////
// CL_DataBuffer Operations:

CL_DataBuffer &CL_DataBuffer::operator =(const CL_DataBuffer &copy)
{
	impl = copy.impl;
	return *this;
}

void CL_DataBuffer::set_size(int new_size)
{
	if (new_size > impl->allocated_size)
	{
		char *old_data = impl->data;
		impl->data = cl_new(impl->pool) char[new_size];
		memcpy(impl->data, old_data, impl->size);
		cl_delete(impl->pool, old_data, impl->size);
		memset(impl->data+impl->size, 0, new_size-impl->size);
		impl->size = new_size;
		impl->allocated_size = new_size;
	}
	else if (new_size <= impl->allocated_size)
	{
		impl->size = new_size;
	}
}

void CL_DataBuffer::set_capacity(int new_capacity)
{
	if (new_capacity > impl->allocated_size)
	{
		char *old_data = impl->data;
		impl->data = cl_new(impl->pool) char[new_capacity];
		memcpy(impl->data, old_data, impl->size);
		cl_delete(impl->pool, old_data, impl->size);
		memset(impl->data+impl->size, 0, new_capacity-impl->size);
		impl->allocated_size = new_capacity;
	}
}

bool CL_DataBuffer::is_null() const
{
	return impl.is_null();
}

/////////////////////////////////////////////////////////////////////////////
// CL_DataBuffer Implementation:
