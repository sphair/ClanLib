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
**    Thomas Gottschalk Larsen
*/

/// \addtogroup clanCore_System clanCore System
/// \{


#pragma once

#include "../api_core.h"
#include "system.h"

#include <cstring>

#if defined(WIN32) || __GNUC__ > 4 ||  (__GNUC__ == 4 & __GNUC_MINOR__ >= 1)
#ifndef CL_SHAREDPTR_INTERLOCKED
#define CL_SHAREDPTR_INTERLOCKED
#endif
#include "interlocked_variable.h"
#else
#include "mutex.h"
#endif

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/System! !header=core.h! !hide! \endxmlonly
class CL_API_CORE CL_SharedPtr_Impl
{
public:
	virtual ~CL_SharedPtr_Impl() { }
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/System! !header=core.h! !hide! \endxmlonly
template <typename Type>
class CL_SharedPtr_Deleter
{
public:
	CL_SharedPtr_Deleter(Type *instance) : instance(instance) { }
	virtual ~CL_SharedPtr_Deleter() { delete instance; }

	Type *instance;
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/System! !header=core.h! !hide! \endxmlonly
template <typename Type>
class CL_SharedPtr_DeleterPool
{
public:
	CL_SharedPtr_DeleterPool(Type *instance, CL_MemoryPool *pool)
	: instance(instance), pool(pool) { return; }

	virtual ~CL_SharedPtr_DeleterPool() { instance->~Type(); operator delete(instance, pool); }

	Type *instance;

	CL_MemoryPool *pool;
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/System! !header=core.h! !hide! \endxmlonly
template <typename Type>
class CL_SharedPtr_DeleterCallback
{
public:
	CL_SharedPtr_DeleterCallback(Type *instance, void (*free_callback)(Type *))
	: instance(instance), free_callback(free_callback) { return; }

	virtual ~CL_SharedPtr_DeleterCallback() { free_callback(instance); }

	Type *instance;

	void (*free_callback)(Type *);
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/System! !header=core.h! !hide! \endxmlonly
template <typename Type, typename FreeClass>
class CL_SharedPtr_DeleterClassCallback
{
public:
	CL_SharedPtr_DeleterClassCallback(Type *instance, FreeClass *free_class, void (FreeClass::*free_callback)(Type *))
	: instance(instance), free_class(free_class), free_callback(free_callback) { return; }

	virtual ~CL_SharedPtr_DeleterClassCallback() { (free_class->*free_callback)(instance); }

	Type *instance;

	FreeClass *free_class;

	void (FreeClass::*free_callback)(Type *);
};

// This was set to 32. This should have been enough for 4 pointers (including VTBL) of 64 bit size
// However, due to compiler issues, in the following example:
//	class X { void (*funx)(void); };
//	class Z { void (X::*funx)(void); };
// Using gcc 4.1.2 on a 64 bit machine:
// sizeof(class X) = 8
// sizeof(class Z) = 16
//
// See: http://www.codeproject.com/cpp/FastDelegate.asp
// "Implementations of Member Function Pointers"
// This has a table showning the various sizeof() operators for various function pointers on various compilers
//
// So, a "safer" option was chosen .. use sizeof((CL_SharedPtr_DeleterClassCallback<int,CL_MemoryPool>)
// Where CL_SharedPtr_DeleterClassCallback contains the greatest number of template pointers
// and CL_MemoryPool is an example class to use for the template.
// (Modified by rombust 21 May 2007)
#define CL_DELETER_SIZE (sizeof(CL_SharedPtr_DeleterClassCallback<int,CL_MemoryPool>))


template <typename Type>
class CL_SharedPtr;

template <typename Type>
class CL_WeakPtr;

/// \brief CL_UnknownSharedPtr
///
/// \xmlonly !group=Core/System! !header=core.h! \endxmlonly
typedef CL_SharedPtr<void> CL_UnknownSharedPtr;

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/System! !header=core.h! !hide! \endxmlonly
class CL_API_CORE CL_SharedPtrData
{
public:
	template <typename Type>
	CL_SharedPtrData(Type *ptr)
	: ptr(ptr)
#if !defined(CL_SHAREDPTR_INTERLOCKED)
	, strong(0), refCount(0)
#endif
	{
		memset(deleter, 0, CL_DELETER_SIZE);
		if (ptr)
			create_deleter(ptr);
	}

	template <typename Type>
	CL_SharedPtrData(Type *ptr, CL_MemoryPool *memory_pool)
	: ptr(ptr)
#if !defined(CL_SHAREDPTR_INTERLOCKED)
	, strong(0), refCount(0)
#endif
	{
		memset(deleter, 0, CL_DELETER_SIZE);
		if (ptr)
			create_deleter(ptr, memory_pool);
	}

	template <typename Type>
	CL_SharedPtrData(Type *ptr, void (*free_callback) (Type *ptr))
	: ptr(ptr)
#if !defined(CL_SHAREDPTR_INTERLOCKED)
	, strong(0), refCount(0)
#endif
	{
		memset(deleter, 0, CL_DELETER_SIZE);
		if (ptr)
			create_deleter(ptr, free_callback);
	}

	template <typename Type, typename FreeClass>
	CL_SharedPtrData(Type *ptr, FreeClass *free_class, void (FreeClass::*free_callback)(Type *ptr))
	: ptr(ptr)
#if !defined(CL_SHAREDPTR_INTERLOCKED)
	, strong(0), refCount(0)
#endif
	{
		memset(deleter, 0, CL_DELETER_SIZE);
		if (ptr)
			create_deleter(ptr, free_class, free_callback);
	}

	void call_deleter()
	{
		CL_SharedPtr_Impl *d = (CL_SharedPtr_Impl*) deleter;
		CL_System::call_destructor(d);
	}

public:
	void *ptr;
#if defined(CL_SHAREDPTR_INTERLOCKED)
	CL_InterlockedVariable strong;
	CL_InterlockedVariable refCount;
#else
	CL_Mutex mutex;
	unsigned long strong;
	unsigned long refCount;
#endif

private:
	template <typename Type>
	void create_deleter(Type *instance)
	{
		CL_SharedPtr_Deleter<Type>* d = (CL_SharedPtr_Deleter<Type>*) deleter;
		CL_System::call_constructor(d, instance);
	}

	template <typename Type>
	void create_deleter(Type *ptr, CL_MemoryPool *pool)
	{
		CL_SharedPtr_DeleterPool<Type>* d = (CL_SharedPtr_DeleterPool<Type>*) deleter;
		CL_System::call_constructor(d, ptr, pool);
	}

	template <typename Type>
	void create_deleter(Type *ptr, void (*free_callback)(Type *ptr))
	{
		CL_SharedPtr_DeleterCallback<Type>* d = (CL_SharedPtr_DeleterCallback<Type>*) deleter;
		CL_System::call_constructor(d, ptr, free_callback);
	}

	template <typename Type, typename FreeClass>
	void create_deleter(Type *ptr, FreeClass *free_class, void (FreeClass::*free_callback)(Type *ptr))
	{
		CL_SharedPtr_DeleterClassCallback<Type, FreeClass>* d = (CL_SharedPtr_DeleterClassCallback<Type, FreeClass>*) deleter;
		CL_System::call_constructor(d, ptr, free_class, free_callback);
	}

private:
	char deleter[CL_DELETER_SIZE];
};


/// \brief CL_SharedPtr
///
/// \xmlonly !group=Core/System! !header=core.h! \endxmlonly
template <typename Type>
class CL_API_CORE CL_SharedPtr
{
public:
	CL_SharedPtr()
	{
		d.data = 0;
	}

	CL_SharedPtr(const CL_SharedPtr<Type> &copy)
	{
		d.data = 0;
		connect(copy.d.data);
	}

	template <typename InitType>
	explicit CL_SharedPtr(InitType *ptr)
	{
		d.data = 0;
		connect(new CL_SharedPtrData(ptr));
	}

	template <typename InitType>
	explicit CL_SharedPtr(InitType *ptr, CL_MemoryPool *memory_pool)
	{
		d.data = 0;
		connect(new CL_SharedPtrData(ptr, memory_pool));
	}

	template <typename InitType>
	explicit CL_SharedPtr(InitType *ptr, void (*free_callback)(InitType *ptr))
	{
		d.data = 0;
		connect(new CL_SharedPtrData(ptr, free_callback));
	}

	template <typename InitType, typename FreeClass>
	explicit CL_SharedPtr(InitType *ptr, FreeClass *free_class, void (FreeClass::*free_callback)(InitType *ptr))
	{
		d.data = 0;
		connect(new CL_SharedPtrData(ptr, free_class, free_callback));
	}

	CL_SharedPtr(CL_SharedPtrData *other)
	{
		d.data = 0;
		connect(other);
	}

	~CL_SharedPtr()
	{
		disconnect();
	}

public:
	CL_SharedPtr &operator =(const CL_SharedPtr &copy)
	{
		if (this == &copy)
			return *this;
		disconnect();
		connect(copy.d.data);
		return *this;
	}

	operator Type*() { return get(); }
	operator const Type*() const { return get(); }

	Type *operator ->() { return get(); }
	const Type *operator ->() const { return get(); }

	template <typename InitType>
	operator CL_SharedPtr<InitType>()
	{
		if (d.data)
			return CL_SharedPtr<InitType>(d.data);
		else
			return CL_SharedPtr<InitType>();
	}

	template <typename InitType>
	operator CL_SharedPtr<InitType>() const
	{
		if (d.data)
			return CL_SharedPtr<InitType>(d.data);
		else
			return CL_SharedPtr<InitType>();
	}

	template <typename OtherType>
	bool operator ==(OtherType *other) const { return get() == other; }

	template <typename OtherType>
	bool operator !=(OtherType *other) const { return get() != other; }

	template <typename OtherType>
	bool operator <=(OtherType *other) const { return get() <= other; }

	template <typename OtherType>
	bool operator >=(OtherType *other) const { return get() >= other; }

	template <typename OtherType>
	bool operator <(OtherType *other) const { return get() < other; }

	template <typename OtherType>
	bool operator >(OtherType *other) const { return get() > other; }

public:
	/// \brief Tests if the pointer is unset
	/** \return true, pointer not set*/
	bool is_null() const { return !(d.data && d.data->ptr); }

	/// \brief Retrieves the pointer
	/** \return The pointer (May be NULL, if it has not been set)*/
	Type *get() { return is_null() ? 0 : *d.p; }

	/// \brief Retrieves the pointer
	/** \return The pointer (May be NULL, if it has not been set)*/
	const Type *get() const { return is_null() ? 0 : *d.p; }

#if defined(CL_SHAREDPTR_INTERLOCKED)

	bool disconnect()
	{
		bool result = false;
		if (d.data)
		{
			if (d.data->strong.decrement() == 0)
			{
				d.data->call_deleter();
				d.data->ptr = 0;
				result = true;
			}
			if (d.data->refCount.decrement() == 0)
				delete d.data;
			d.p = 0;
		}
		return result;
	}

private:
	void connect(CL_SharedPtrData *data)
	{
		if (data)
		{
			data->refCount.increment();
			data->strong.increment();
			d.data = data;
		}
	}

#else

	bool disconnect()
	{
		bool result = false;
		if (d.data)
		{
			CL_MutexSection s(&d.data->mutex);
			if (--d.data->strong == 0)
			{
				d.data->call_deleter();
				d.data->ptr = 0;
				result = true;
			}
			if (--d.data->refCount == 0)
			{
				s.unlock();
				delete d.data;
			}
			d.p = 0;
		}
		return result;
	}

private:
	void connect(CL_SharedPtrData *data)
	{
		if (data)
		{
			CL_MutexSection s(&data->mutex);
			++data->refCount;
			++data->strong;
			d.data = data;
		}
	}

#endif

	union
	{
		Type **p;
		CL_SharedPtrData *data;
	} d;

	friend class CL_WeakPtr<Type>;
};

/// \}
