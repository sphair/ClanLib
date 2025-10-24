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

/// \addtogroup clanCore_System clanCore System
/// \{


#pragma once


#include "../api_core.h"
#include <new>
#include <memory>

/// \brief Memory allocation interface.
///
/// \xmlonly !group=Core/System! !header=core.h! \endxmlonly
class CL_API_CORE CL_MemoryPool
{
/// \name Construction
/// \{

public:
	virtual ~CL_MemoryPool()
	{
	}


/// \}
/// \name Operations
/// \{

public:
	/// \brief Allocate memory.
	/** param: size = Size to allocate*/
	virtual void *alloc(size_t size) = 0;

	/// \brief Free memory.
	/** param: data = Data to free*/
	virtual void free(void *data) = 0;

	/// \brief Returns the thread temporary pool.
	static CL_MemoryPool *get_temp_pool();
/// \}
};

/// \brief STL allocator using CL_MemoryPool.
///
/// \xmlonly !group=Core/System! !header=core.h! \endxmlonly
template<typename _Type>
class CL_MemoryPoolAllocator
{
public:
	typedef _Type value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;

	CL_MemoryPoolAllocator()
	: _memory_pool(0)
	{
	}

	CL_MemoryPoolAllocator(const CL_MemoryPoolAllocator<_Type> &_src)
	: _memory_pool(_src._memory_pool)
	{
	}

	template<typename _Other>
	CL_MemoryPoolAllocator(const CL_MemoryPoolAllocator<_Other> &_src)
	: _memory_pool(_src._memory_pool)
	{
	}

	CL_MemoryPoolAllocator(CL_MemoryPool *_memory_pool)
	: _memory_pool(_memory_pool)
	{
	}

	pointer address(reference _x) const
	{
		return &_x;
	}

	const_pointer address(const_reference _x) const
	{
		return &_x;
	}

	template<typename _Other>
	pointer allocate(size_type _count, const _Other *_Hint = 0)
	{
		if (_memory_pool)
			return (pointer) _memory_pool->alloc(_count * sizeof(value_type));
		else
			_fallback_alloc.allocate(_count, _Hint);
	}

	void destroy(pointer _ptr, size_type _count)
	{
		if (_memory_pool)
			_memory_pool->free(_ptr);
		else
			_fallback_alloc.destroy(_ptr);
	}

	size_type max_size() const
	{
		return (size_type) -1;
	}

	template<typename _Other>
	struct rebind
	{
		typedef CL_MemoryPoolAllocator<_Other> other;
	};

	void construct(pointer _ptr, const _Type &_value)
	{
		if (_memory_pool)
			new((void *) _ptr) _Type(_value);
		else
			_fallback_alloc.construct(_ptr, _value);
	}

	void deallocate(pointer _ptr)
	{
		if (_memory_pool)
			_ptr->_Type::~_Type();
		else
			_fallback_alloc.destroy(_ptr);
	}

private:
	CL_MemoryPool *_memory_pool;
	std::allocator<_Type> _fallback_alloc;
};

/// \brief Memory pool operator new.
///
/// \xmlonly !group=Core/System! !header=core.h! \endxmlonly
///  \param size = Size to allocate
///  \param pool = Memory pool interface to use. If NULL = use "::operator new(size)"
inline void *operator new(size_t size, CL_MemoryPool *pool)
{
	if (pool)
		return pool->alloc(size);
	else
		return ::operator new(size);
}

/// \brief Memory pool operator delete.
///
/// \xmlonly !group=Core/System! !header=core.h! \endxmlonly
/// \param data = Data to free. (The destructor is not called, see cl_delete(CL_MemoryPool *pool, Type *obj) )
/// \param pool = Memory pool interface to use. If NULL, use "::operator delete(data)"
inline void operator delete(void *data, CL_MemoryPool *pool)
{
	if (pool)
		pool->free(data);
	else
		::operator delete(data);
}

/// \brief Memory pool operator new[].
///
/// \xmlonly !group=Core/System! !header=core.h! \endxmlonly
/// \param size = Size to allocate
/// \param pool = Memory pool interface to use. If NULL = use "::operator new(size)"
inline void *operator new[](size_t size, CL_MemoryPool *pool)
{
	if (pool)
		return pool->alloc(size);
	else
		return ::operator new[](size);
}

/// \brief Memory pool operator delete[].
///
/// \xmlonly !group=Core/System! !header=core.h! \endxmlonly
/// \param data = Data to free. (The destructor is not called, see cl_delete(CL_MemoryPool *pool, Type *obj, size_t array_size) )
/// \param pool = Memory pool interface to use. If NULL, use "::operator delete[](data)"
inline void operator delete[](void *data, CL_MemoryPool *pool)
{
	if (pool)
		pool->free(data);
	else
		::operator delete[](data);
}

/// \brief Memory pool new.
///
/// \xmlonly !group=Core/System! !header=core.h! \endxmlonly
#define cl_new new

/// \brief Memory pool delete.
///
/// \xmlonly !group=Core/System! !header=core.h! \endxmlonly
/// \param pool = Memory pool interface to use. If NULL, object is deleted using ::operator delete(obj)
/// \param obj = Object to free. The destructor is called
template <typename Type>
void cl_delete(CL_MemoryPool *pool, Type *obj)
{
	if (obj)
		obj->~Type();
	operator delete(obj, pool);
}

/// \brief Memory pool delete.
///
/// \xmlonly !group=Core/System! !header=core.h! \endxmlonly
/// \param pool = Memory pool interface to use. If NULL, object is deleted using ::operator delete[](obj)
/// \param obj = Objects to free. The destructor is called
/// \param array_size = Number of objects in the array (Used to call the destructors. The entire array is always deleted even if array_size is short)
template <typename Type>
void cl_delete(CL_MemoryPool *pool, Type *obj, size_t array_size)
{
	if (obj)
		for (size_t pos = 0; pos < array_size; pos++)
			obj[pos].~Type();
#ifdef __GNUC__
	/** GCC does not like "operator delete[](obj,pool), it causes a seg fault*/
	void *data = obj;
	if (pool)
		pool->free(data);
	else
		::operator delete[](data);
#else
	operator delete[](obj, pool);
#endif
}



/// \}
