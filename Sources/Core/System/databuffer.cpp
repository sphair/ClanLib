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

#include "Core/precomp.h"
#include "API/Core/System/databuffer.h"
#include <string.h>

namespace clan
{
	class DataBuffer_Impl
	{
	public:
		DataBuffer_Impl() : data(nullptr), size(0), allocated_size(0)
		{
		}

		~DataBuffer_Impl()
		{
			delete[] data;
		}

	public:
		char *data;
		unsigned int size;
		unsigned int allocated_size;
	};

	DataBuffer::DataBuffer()
		: impl(std::make_shared<DataBuffer_Impl>())
	{
	}

	DataBuffer::DataBuffer(unsigned int new_size)
		: impl(std::make_shared<DataBuffer_Impl>())
	{
		set_size(new_size);
	}

	DataBuffer::DataBuffer(const void *new_data, unsigned int new_size)
		: impl(std::make_shared<DataBuffer_Impl>())
	{
		set_size(new_size);
		memcpy(impl->data, new_data, new_size);
	}

	DataBuffer::DataBuffer(const DataBuffer &new_data, unsigned int pos, unsigned int size)
		: impl(std::make_shared<DataBuffer_Impl>())
	{
		set_size(size);
		memcpy(impl->data, new_data.get_data() + pos, size);
	}

	DataBuffer::~DataBuffer()
	{
	}

	char *DataBuffer::get_data()
	{
		return impl->data;
	}

	const char *DataBuffer::get_data() const
	{
		return impl->data;
	}

	unsigned int DataBuffer::get_size() const
	{
		return impl->size;
	}

	unsigned int DataBuffer::get_capacity() const
	{
		return impl->allocated_size;
	}

	char &DataBuffer::operator[](int i)
	{
		return impl->data[i];
	}

	const char &DataBuffer::operator[](int i) const
	{
		return impl->data[i];
	}

	char &DataBuffer::operator[](unsigned int i)
	{
		return impl->data[i];
	}

	const char &DataBuffer::operator[](unsigned int i) const
	{
		return impl->data[i];
	}

	DataBuffer &DataBuffer::operator =(const DataBuffer &copy)
	{
		impl = copy.impl;
		return *this;
	}

	void DataBuffer::set_size(unsigned int new_size)
	{
		if (new_size > impl->allocated_size)
		{
			char *old_data = impl->data;
			impl->data = new char[new_size];
			memcpy(impl->data, old_data, impl->size);
			delete[] old_data;
			memset(impl->data + impl->size, 0, new_size - impl->size);
			impl->size = new_size;
			impl->allocated_size = new_size;
		}
		else
		{
			impl->size = new_size;
		}
	}

	void DataBuffer::set_capacity(unsigned int new_capacity)
	{
		if (new_capacity > impl->allocated_size)
		{
			char *old_data = impl->data;
			impl->data = new char[new_capacity];
			memcpy(impl->data, old_data, impl->size);
			delete[] old_data;
			memset(impl->data + impl->size, 0, new_capacity - impl->size);
			impl->allocated_size = new_capacity;
		}
	}

	bool DataBuffer::is_null() const
	{
		return impl->size == 0;
	}
}
