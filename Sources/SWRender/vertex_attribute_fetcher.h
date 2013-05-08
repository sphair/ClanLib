/*
**  ClanLib SDK
**  Copyright (c) 1997-2013 The ClanLib Team
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

#include "API/Display/TargetProviders/graphic_context_provider.h"
#include "API/Core/Math/cl_math.h"
#include "swr_primitives_array_provider.h"
#include "API/Display/TargetProviders/primitives_array_provider.h"
#include "swr_vertex_array_buffer_provider.h"

namespace clan
{
class PrimitivesArrayData;

class VertexAttributeFetcher
{
public:
	virtual Vec4f fetch(int index, const Vec4f &default_value = Vec4f()) = 0;

	virtual void fetch(Vec4f *result, int *indexes, int num, const Vec4f &default_value = Vec4f())
	{
		for (int i = 0; i < num; i++)
			result[i] = fetch(indexes[i], default_value);
	}

	virtual void fetch(Vec2f *result, int *indexes, int num, const Vec4f &default_value = Vec4f())
	{
		for (int i = 0; i < num; i++)
			result[i] = Vec2f(fetch(indexes[i], default_value));
	}

	virtual void fetch(float *result, int *indexes, int num, const Vec4f &default_value = Vec4f())
	{
		for (int i = 0; i < num; i++)
			result[i] = fetch(indexes[i], default_value).x;
	}

protected:
	VertexAttributeFetcher() : bound_attribute_index(-1), range(0), data(0), stride(0), primitives_array_provider(NULL) { }
	virtual ~VertexAttributeFetcher() { }

	void bind(const SWRenderPrimitivesArrayProvider *primitives_array_provider, int bound_attribute_index);
	const void *find_vertex_data(int index);

	const SWRenderPrimitivesArrayProvider *primitives_array_provider;

	int bound_attribute_index;
	unsigned int range;
	const char *data;
	int stride;

	friend class VertexAttributeFetcherPtr;
};

class VertexAttributeFetcherPtr
{
public:
	VertexAttributeFetcherPtr();
	~VertexAttributeFetcherPtr();

	void bind(const SWRenderPrimitivesArrayProvider *primitives_array_provider, int attribute_index);
	void clear();

	VertexAttributeFetcher *operator->() { return fetcher; }
	const VertexAttributeFetcher *operator->() const { return fetcher; }

private:
	VertexAttributeFetcherPtr(const VertexAttributeFetcherPtr &other) { }
	VertexAttributeFetcherPtr &operator =(const VertexAttributeFetcherPtr &other) { return *this; }
	void throw_if_insufficient_buffer(size_t size);

	VertexAttributeFetcher *fetcher;
	enum { alloc_buffer_size = sizeof(VertexAttributeFetcher) };
	char alloc_buffer[alloc_buffer_size];
};

class VertexAttributeFetcherNull : public VertexAttributeFetcher
{
public:
	Vec4f fetch(int index, const Vec4f &default_value = Vec4f()) { return default_value; }
};

class VertexAttributeFetcherSingleValueUnsignedByte : public VertexAttributeFetcher
{
public:
	Vec4f fetch(int index, const Vec4f &default_value = Vec4f());
};

class VertexAttributeFetcherSingleValueUnsignedShort : public VertexAttributeFetcher
{
public:
	Vec4f fetch(int index, const Vec4f &default_value = Vec4f());
};

class VertexAttributeFetcherSingleValueUnsignedInt : public VertexAttributeFetcher
{
public:
	Vec4f fetch(int index, const Vec4f &default_value = Vec4f());
};

class VertexAttributeFetcherSingleValueByte : public VertexAttributeFetcher
{
public:
	Vec4f fetch(int index, const Vec4f &default_value = Vec4f());
};

class VertexAttributeFetcherSingleValueShort : public VertexAttributeFetcher
{
public:
	Vec4f fetch(int index, const Vec4f &default_value = Vec4f());
};

class VertexAttributeFetcherSingleValueInt : public VertexAttributeFetcher
{
public:
	Vec4f fetch(int index, const Vec4f &default_value = Vec4f());
};

class VertexAttributeFetcherSingleValueFloat : public VertexAttributeFetcher
{
public:
	Vec4f fetch(int index, const Vec4f &default_value = Vec4f());
};

class VertexAttributeFetcherArrayUnsignedByte : public VertexAttributeFetcher
{
public:
	Vec4f fetch(int index, const Vec4f &default_value = Vec4f());
};

class VertexAttributeFetcherArrayUnsignedShort : public VertexAttributeFetcher
{
public:
	Vec4f fetch(int index, const Vec4f &default_value = Vec4f());
};

class VertexAttributeFetcherArrayUnsignedInt : public VertexAttributeFetcher
{
public:
	Vec4f fetch(int index, const Vec4f &default_value = Vec4f());
};

class VertexAttributeFetcherArrayByte : public VertexAttributeFetcher
{
public:
	Vec4f fetch(int index, const Vec4f &default_value = Vec4f());
};

class VertexAttributeFetcherArrayShort : public VertexAttributeFetcher
{
public:
	Vec4f fetch(int index, const Vec4f &default_value = Vec4f());
};

class VertexAttributeFetcherArrayInt : public VertexAttributeFetcher
{
public:
	Vec4f fetch(int index, const Vec4f &default_value = Vec4f());
};

class VertexAttributeFetcherArrayFloat : public VertexAttributeFetcher
{
public:
	Vec4f fetch(int index, const Vec4f &default_value = Vec4f());
	void fetch(Vec4f *result, int *indexes, int num, const Vec4f &default_value = Vec4f());
	void fetch(Vec2f *result, int *indexes, int num, const Vec4f &default_value = Vec4f());
	void fetch(float *result, int *indexes, int num, const Vec4f &default_value = Vec4f());
};

inline VertexAttributeFetcherPtr::VertexAttributeFetcherPtr()
: fetcher(0)
{
}

inline VertexAttributeFetcherPtr::~VertexAttributeFetcherPtr()
{
	clear();
}

inline void VertexAttributeFetcherPtr::bind(const SWRenderPrimitivesArrayProvider *primitives_array_provider, int attribute_index)
{
	clear();

	int bound_attribute_index = -1;
	if (attribute_index >=0 && attribute_index <primitives_array_provider->attributes.size())
	{
		bound_attribute_index = attribute_index;
	}

	/* I guess this is not required (replaced by the above code) (See  HACK HACK HACK below)
	int bound_attribute_index = -1;
	for (int j = 0; j < primitives_array_provider->attributes.size(); j++)
	{
		if (primitives_array_provider->attribute_indexes[j] == attribute_index)
		{
			bound_attribute_index = j;
			break;
		}
	}
	*/

	if (bound_attribute_index == -1)
	{
		throw_if_insufficient_buffer(sizeof(VertexAttributeFetcherNull));
		fetcher = new (alloc_buffer) VertexAttributeFetcherNull();
		bound_attribute_index = 0;	// HACK HACK HACK (So doesn't crash later on)
	}
	else
	{
		switch (primitives_array_provider->attributes[bound_attribute_index].type)
		{
		case type_unsigned_byte:
			throw_if_insufficient_buffer(sizeof(VertexAttributeFetcherArrayUnsignedByte));
			fetcher = new (&alloc_buffer) VertexAttributeFetcherArrayUnsignedByte();
			break;
		case type_unsigned_short:
			throw_if_insufficient_buffer(sizeof(VertexAttributeFetcherArrayUnsignedShort));
			fetcher = new (&alloc_buffer) VertexAttributeFetcherArrayUnsignedShort();
			break;
		case type_unsigned_int:
			throw_if_insufficient_buffer(sizeof(VertexAttributeFetcherArrayUnsignedInt));
			fetcher = new (&alloc_buffer) VertexAttributeFetcherArrayUnsignedInt();
			break;
		case type_byte:
			throw_if_insufficient_buffer(sizeof(VertexAttributeFetcherArrayByte));
			fetcher = new (&alloc_buffer) VertexAttributeFetcherArrayByte();
			break;
		case type_short:
			throw_if_insufficient_buffer(sizeof(VertexAttributeFetcherArrayShort));
			fetcher = new (&alloc_buffer) VertexAttributeFetcherArrayShort();
			break;
		case type_int:
			throw_if_insufficient_buffer(sizeof(VertexAttributeFetcherArrayInt));
			fetcher = new (&alloc_buffer) VertexAttributeFetcherArrayInt();
			break;
		case type_float:
			throw_if_insufficient_buffer(sizeof(VertexAttributeFetcherArrayFloat));
			fetcher = new (&alloc_buffer) VertexAttributeFetcherArrayFloat();
			break;
		default:
			throw_if_insufficient_buffer(sizeof(VertexAttributeFetcherNull));
			fetcher = new (&alloc_buffer) VertexAttributeFetcherNull();
			break;
		}
	}

	fetcher->bind(primitives_array_provider, bound_attribute_index);
}

inline void VertexAttributeFetcherPtr::throw_if_insufficient_buffer(size_t size)
{
	if (alloc_buffer_size < size)
		throw Exception("Insufficient buffer size for vertex attribute fetcher object");
}

inline void VertexAttributeFetcherPtr::clear()
{
	if (fetcher)
	{
		fetcher->~VertexAttributeFetcher();
		fetcher = 0;
	}
}

inline void VertexAttributeFetcher::bind(const SWRenderPrimitivesArrayProvider *new_primitives_array_provider, int new_bound_attribute_index)
{
	primitives_array_provider = new_primitives_array_provider;
	bound_attribute_index = new_bound_attribute_index;
	range = min(primitives_array_provider->attributes[bound_attribute_index].size, 4);
}

inline const void *VertexAttributeFetcher::find_vertex_data(int index)
{
	if (data == 0)
	{
		const PrimitivesArrayProvider::VertexData &vertex_data = primitives_array_provider->attributes[bound_attribute_index];
		SWRenderVertexArrayBufferProvider * vertex_provider = static_cast<SWRenderVertexArrayBufferProvider *>(vertex_data.array_provider);
		if (!vertex_provider)
			throw Exception("Invalid SWRenderVertexArrayBufferProvider");

		data = reinterpret_cast<const char*>(vertex_provider->get_data());
		data += vertex_data.offset;
		stride = vertex_data.stride;
		if (stride == 0)
		{
			switch (vertex_data.type)
			{
			case type_unsigned_byte: stride = sizeof(unsigned char) * vertex_data.size; break;
			case type_unsigned_short: stride = sizeof(unsigned short) * vertex_data.size; break;
			case type_unsigned_int: stride = sizeof(unsigned int) * vertex_data.size; break;
			case type_byte: stride = sizeof(char) * vertex_data.size; break;
			case type_short: stride = sizeof(short) * vertex_data.size; break;
			case type_int: stride = sizeof(int) * vertex_data.size; break;
			case type_float: stride = sizeof(float) * vertex_data.size; break;
			default: break;
			}
		}
	}

	return data + stride*index;
}

inline Vec4f VertexAttributeFetcherArrayUnsignedByte::fetch(int index, const Vec4f &default_value)
{
	const unsigned char *v = static_cast<const unsigned char *>(find_vertex_data(index));
	Vec4f result = default_value;
	for (unsigned int i=0; i<range; i++)
		result[i] = v[i];
	return result;
}

inline Vec4f VertexAttributeFetcherArrayUnsignedShort::fetch(int index, const Vec4f &default_value)
{
	const unsigned short *v = static_cast<const unsigned short *>(find_vertex_data(index));
	Vec4f result = default_value;
	for (unsigned int i=0; i<range; i++)
		result[i] = v[i];
	return result;
}

inline Vec4f VertexAttributeFetcherArrayUnsignedInt::fetch(int index, const Vec4f &default_value)
{
	const unsigned int *v = static_cast<const unsigned int *>(find_vertex_data(index));
	Vec4f result = default_value;
	for (unsigned int i=0; i<range; i++)
		result[i] = v[i];
	return result;
}

inline Vec4f VertexAttributeFetcherArrayByte::fetch(int index, const Vec4f &default_value)
{
	const char *v = static_cast<const char *>(find_vertex_data(index));
	Vec4f result = default_value;
	for (unsigned int i=0; i<range; i++)
		result[i] = v[i];
	return result;
}

inline Vec4f VertexAttributeFetcherArrayShort::fetch(int index, const Vec4f &default_value)
{
	const short *v = static_cast<const short *>(find_vertex_data(index));
	Vec4f result = default_value;
	for (unsigned int i=0; i<range; i++)
		result[i] = v[i];
	return result;
}

inline Vec4f VertexAttributeFetcherArrayInt::fetch(int index, const Vec4f &default_value)
{
	const int *v = static_cast<const int *>(find_vertex_data(index));
	Vec4f result = default_value;
	for (unsigned int i=0; i<range; i++)
		result[i] = v[i];
	return result;
}

inline Vec4f VertexAttributeFetcherArrayFloat::fetch(int index, const Vec4f &default_value)
{
	const float *v = static_cast<const float *>(find_vertex_data(index));
	Vec4f result = default_value;
	for (unsigned int i=0; i<range; i++)
		result[i] = v[i];
	return result;
}

inline void VertexAttributeFetcherArrayFloat::fetch(Vec4f *result, int *indexes, int num, const Vec4f &default_value)
{
	for (int i = 0; i < num; i++)
	{
		const float *v = static_cast<const float *>(find_vertex_data(indexes[i]));
		for (unsigned int j=0; j<range; j++)
			result[i][j] = v[j];
		for (unsigned int j=range; j<4; j++)
			result[i][j] = default_value[j];
	}
}

inline void VertexAttributeFetcherArrayFloat::fetch(Vec2f *result, int *indexes, int num, const Vec4f &default_value)
{
	for (int i = 0; i < num; i++)
	{
		const float *v = static_cast<const float *>(find_vertex_data(indexes[i]));
		int r = min( static_cast<int>(range), 2);
		for (unsigned int j=0; j<r; j++)
			result[i][j] = v[j];
		for (unsigned int j=r; j<2; j++)
			result[i][j] = default_value[j];
	}
}

inline void VertexAttributeFetcherArrayFloat::fetch(float *result, int *indexes, int num, const Vec4f &default_value)
{
	for (int i = 0; i < num; i++)
	{
		const float *v = static_cast<const float *>(find_vertex_data(indexes[i]));
		result[i] = range > 0 ? v[0] : default_value.x;
	}
}

}
