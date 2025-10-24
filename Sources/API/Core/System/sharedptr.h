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
#include "system.h"
#include "mutex.h"
#include "exception.h"
#include "memory_pool.h"

#ifndef WIN32
#include <cstring>
#endif

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/System! !header=core.h! !hide! \endxmlonly
class CL_API_CORE CL_SharedPtr_Impl
{
public:
	virtual ~CL_SharedPtr_Impl() { return; }
};

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/System! !header=core.h! !hide! \endxmlonly
template <typename Type>
class CL_SharedPtr_Deleter
{
public:
	CL_SharedPtr_Deleter(Type *instance) : instance(instance) { return; }

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

/// (Internal ClanLib Class)
/// \xmlonly !group=Core/System! !header=core.h! !hide! \endxmlonly
class CL_API_CORE CL_SharedPtr_Link
{
public:
	CL_SharedPtr_Link()
	: mutex(0), prev(0), next(0), weak_link(0)
	{
	}

	CL_SharedPtr_Link(const CL_SharedPtr_Link &copy)
	: mutex(0), prev(0), next(0), weak_link(0)
	{
		connect(copy);
	}

	~CL_SharedPtr_Link()
	{
		if (prev != 0 || next != 0)
			disconnect();
		/*
		mutex = (CL_Mutex *) 0xfdfdfdfd;
		prev = (CL_SharedPtr_Link *) 0xfdfdfdfd;
		next = (CL_SharedPtr_Link *) 0xfdfdfdfd;
		memset(deleter, 0xfd, CL_DELETER_SIZE);
		weak_link = 0xfd;
		*/
	}

	CL_SharedPtr_Link &operator =(const CL_SharedPtr_Link &copy)
	{
		if (this != &copy)
		{
			disconnect();
			connect(copy);
		}
		return *this;
	}

	template <typename Type>
	void create_deleter(Type *instance)
	{
		#ifdef DEBUG_SHAREDPTR
			if (sizeof(CL_SharedPtr_Deleter<Type>) > CL_DELETER_SIZE)
				throw CL_Exception("CL_DELETER_SIZE buffer too small!");
		#endif
		CL_SharedPtr_Deleter<Type> *d = (CL_SharedPtr_Deleter<Type> *) deleter;
		CL_System::call_constructor(d, instance);
	}

	template <typename Type>
	void create_deleter(Type *ptr, CL_MemoryPool *pool)
	{
		#ifdef DEBUG_SHAREDPTR
			if (sizeof(CL_SharedPtr_DeleterPool<Type>) > CL_DELETER_SIZE)
				throw CL_Exception("CL_DELETER_SIZE buffer too small!");
		#endif
		CL_SharedPtr_DeleterPool<Type> *d = (CL_SharedPtr_DeleterPool<Type> *) deleter;
		CL_System::call_constructor(d, ptr, pool);
	}

	template <typename Type>
	void create_deleter(Type *ptr, void (*free_callback)(Type *ptr))
	{
		#ifdef DEBUG_SHAREDPTR
			if (sizeof(CL_SharedPtr_DeleterCallback<Type>) > CL_DELETER_SIZE)
				throw CL_Exception("CL_DELETER_SIZE buffer too small!");
		#endif
		CL_SharedPtr_DeleterCallback<Type> *d = (CL_SharedPtr_DeleterCallback<Type> *) deleter;
		CL_System::call_constructor(d, ptr, free_callback);
	}

	template <typename Type, typename FreeClass>
	void create_deleter(Type *ptr, FreeClass *free_class, void (FreeClass::*free_callback)(Type *ptr))
	{
		#ifdef DEBUG_SHAREDPTR
			if (sizeof(CL_SharedPtr_DeleterClassCallback<Type, FreeClass>) > CL_DELETER_SIZE)
				throw CL_Exception("CL_DELETER_SIZE buffer too small!");
		#endif
		CL_SharedPtr_DeleterClassCallback<Type, FreeClass> *d = (CL_SharedPtr_DeleterClassCallback<Type, FreeClass> *) deleter;
		CL_System::call_constructor(d, ptr, free_class, free_callback);
	}

	void call_deleter()
	{
		CL_SharedPtr_Impl *d = (CL_SharedPtr_Impl *) deleter;
		CL_System::call_destructor(d);
	}

	void connect(const CL_SharedPtr_Link &copy)
	{
		CL_MutexSection mutex_lock(copy.mutex);
		#ifdef DEBUG_SHAREDPTR
			if (prev != 0 || next != 0)
				throw CL_Exception("Memory corruption error in CL_SharedPtr_Link");
			assert_list(&copy);
		#endif
		mutex = copy.mutex;
		next = copy.next;
		prev = (CL_SharedPtr_Link *) &copy;
		copy.next = this;
		if (next)
			next->prev = this;
		memcpy(deleter, copy.deleter, CL_DELETER_SIZE);
		#ifdef DEBUG_SHAREDPTR
			assert_list(prev);
		#endif
	}

	/// \brief Disconnect from linked list.
	/** <p>Returns true if list is empty or only contains weak links.</p>*/
	bool disconnect()
	{
		CL_MutexSection mutex_lock(mutex);
		#ifdef DEBUG_SHAREDPTR
			assert_list(prev);
		#endif
		if (weak_link == 2)
			weak_link = 1;
		if (prev == 0 && next == 0)
			return true;

		if (prev)
			prev->next = next;
		if (next)
			next->prev = prev;
		#ifdef DEBUG_SHAREDPTR
			assert_list(prev);
		#endif

		CL_SharedPtr_Link *cur = prev;
		while (cur)
		{
			if (!cur->weak_link)
			{
				prev = 0;
				next = 0;
				return false;
			}
			cur = cur->prev;
		}

		cur = next;
		while (cur)
		{
			if (!cur->weak_link)
			{
				prev = 0;
				next = 0;
				return false;
			}
			cur = cur->next;
		}

		// Ok, empty or only contains weak links.
		// Invalidate all weak links:

		cur = prev;
		while (cur)
		{
			cur->weak_link = 2;
			cur = cur->prev;
		}

		cur = next;
		while (cur)
		{
			cur->weak_link = 2;
			cur = cur->next;
		}

		prev = 0;
		next = 0;
		return true;
	}

	void set_weak_link() { weak_link = 1; }

	bool is_invalid_weak_link() const { return weak_link != 1; }

	#ifdef DEBUG_SHAREDPTR
		void assert_list(const CL_SharedPtr_Link *ptr)
		{
			if (ptr == 0)
				return;
			while (ptr->prev != 0)
			{
				if (ptr->prev->next != ptr)
					throw CL_Exception("Memory corruption error in CL_SharedPtr_Link");
				ptr = ptr->prev;
			}
			while (ptr && ptr->next)
			{
				if (ptr->next->prev != ptr)
					throw CL_Exception("Memory corruption error in CL_SharedPtr_Link");
				ptr = ptr->next;
			}
		}
	#endif

public:
	mutable CL_Mutex *mutex;

private:
	mutable CL_SharedPtr_Link *prev;

	mutable CL_SharedPtr_Link *next;

	char deleter[CL_DELETER_SIZE];

	unsigned char weak_link;
};

/// \brief CL_UnknownSharedPtr
///
/// \xmlonly !group=Core/System! !header=core.h! \endxmlonly
class CL_API_CORE CL_UnknownSharedPtr : public CL_SharedPtr_Link
{
public:
	CL_UnknownSharedPtr()
	: ptr(0)
	{
	}

	CL_UnknownSharedPtr(const CL_SharedPtr_Link &link, void *ptr)
	: ptr(ptr)
	{
		connect(link);
	}

	CL_UnknownSharedPtr(const CL_UnknownSharedPtr &copy)
	: ptr(0)
	{
		connect(copy);
		ptr = copy.ptr;
	}

	~CL_UnknownSharedPtr()
	{
		disconnect();
	}

	CL_UnknownSharedPtr &operator =(const CL_UnknownSharedPtr &copy)
	{
		if (this == &copy)
			return *this;
		disconnect();
		connect(copy);
		ptr = copy.ptr;
		return *this;
	}

	/// \brief Disconnect from linked list and unset the pointer
	/** If the list is empty or only contains weak links, then the pointer destructor is called
	    \return true if the list is empty or only contains weak links (ie the pointer destructor was called)*/
	bool disconnect()
	{
		bool result = CL_SharedPtr_Link::disconnect();
		if ( result && ptr )
		{
			call_deleter();
		}
		ptr = NULL;
		return result;
	}

	/// \brief Tests if the pointer is unset
	/** \return true, pointer not set*/
	bool is_null() const { return ptr == 0; }

	/// \brief Retrieves the pointer
	/** \return The pointer (May be NULL, if it has not been set)*/
	void *get() { return ptr; }

	/// \brief Retrieves the pointer
	/** \return The pointer (May be NULL, if it has not been set)*/
	const void *get() const { return ptr; }

	operator void *() { return ptr; }

	operator const void *() const { return ptr; }

	void *operator ->() { return ptr; }

	const void *operator ->() const { return ptr; }

	template <typename OtherType>
	bool operator ==(OtherType *other) const { return ptr == other; }

	template <typename OtherType>
	bool operator !=(OtherType *other) const { return ptr != other; }

	template <typename OtherType>
	bool operator <(OtherType *other) const { return ptr < other; }

	template <typename OtherType>
	bool operator <=(OtherType *other) const { return ptr <= other; }

	template <typename OtherType>
	bool operator >(OtherType *other) const { return ptr > other; }

	template <typename OtherType>
	bool operator >=(OtherType *other) const { return ptr >= other; }

public:
	void *ptr;
};

/// \brief CL_SharedPtr
///
/// \xmlonly !group=Core/System! !header=core.h! \endxmlonly
template <typename Type>
class CL_SharedPtr : public CL_SharedPtr_Link
{
public:
	CL_SharedPtr()
	: ptr(0)
	{
	}

	CL_SharedPtr(const CL_SharedPtr<Type> &copy)
	: ptr(0)
	{
		connect(copy);
		ptr = copy.ptr;
	}

	explicit CL_SharedPtr(const CL_UnknownSharedPtr &copy)
	: ptr(0)
	{
		connect(copy);
		ptr = (Type *) copy.ptr;
	}

	template <typename InitType>
	explicit CL_SharedPtr(InitType *ptr)
	: ptr(ptr)
	{
		mutex = CL_System::get_sharedptr_mutex();
		if (ptr)
			create_deleter(ptr);
	}

	template <typename InitType>
	explicit CL_SharedPtr(InitType *ptr, CL_Mutex *ref_mutex)
	: ptr(ptr)
	{
		mutex = ref_mutex;
		if (ptr)
			create_deleter(ptr);
	}

	template <typename InitType>
	explicit CL_SharedPtr(InitType *ptr, CL_MemoryPool *memory_pool)
	: ptr(ptr)
	{
		mutex = CL_System::get_sharedptr_mutex();
		if (ptr)
			create_deleter(ptr, memory_pool);
	}

	template <typename InitType>
	explicit CL_SharedPtr(InitType *ptr, CL_MemoryPool *memory_pool, CL_Mutex *ref_mutex)
	: ptr(ptr)
	{
		mutex = ref_mutex;
		if (ptr)
			create_deleter(ptr, memory_pool);
	}

	template <typename InitType>
	explicit CL_SharedPtr(InitType *ptr, void (*free_callback)(InitType *ptr))
	: ptr(ptr)
	{
		mutex = CL_System::get_sharedptr_mutex();
		if (ptr)
			create_deleter(ptr, free_callback);
	}

	template <typename InitType>
	explicit CL_SharedPtr(InitType *ptr, void (*free_callback)(InitType *ptr), CL_Mutex *ref_mutex)
	: ptr(ptr)
	{
		mutex = ref_mutex;
		if (ptr)
			create_deleter(ptr, free_callback);
	}

	template <typename InitType, typename FreeClass>
	explicit CL_SharedPtr(InitType *ptr, FreeClass *free_class, void (FreeClass::*free_callback)(InitType *ptr))
	: ptr(ptr)
	{
		mutex = CL_System::get_sharedptr_mutex();
		if (ptr)
			create_deleter(ptr, free_class, free_callback);
	}

	template <typename InitType, typename FreeClass>
	explicit CL_SharedPtr(InitType *ptr, FreeClass *free_class, void (FreeClass::*free_callback)(InitType *ptr), CL_Mutex *ref_mutex)
	: ptr(ptr)
	{
		mutex = ref_mutex;
		if (ptr)
			create_deleter(ptr, free_class, free_callback);
	}

	CL_SharedPtr(const CL_SharedPtr_Link &link, Type *ptr)
	: ptr(ptr)
	{
		connect(link);
	}

	~CL_SharedPtr()
	{
		disconnect();
	}

	CL_SharedPtr &operator =(const CL_SharedPtr &copy)
	{
		if (this == &copy)
			return *this;
		disconnect();
		connect(copy);
		ptr = copy.ptr;
		return *this;
	}

	/// \brief Disconnect from linked list and unset the pointer
	/** If the list is empty or only contains weak links, then the pointer destructor is called
	    \return true if the list is empty or only contains weak links (ie the pointer destructor was called)*/
	bool disconnect()
	{
		bool result = CL_SharedPtr_Link::disconnect();
		if ( result && ptr )
		{
			call_deleter();
		}
		ptr = NULL;
		return result;
	}

	/// \brief Tests if the pointer is unset
	/** \return true, pointer not set*/
	bool is_null() const { return ptr == 0; }

	/// \brief Retrieves the pointer
	/** \return The pointer (May be NULL, if it has not been set)*/
	Type *get() { return ptr; }

	/// \brief Retrieves the pointer
	/** \return The pointer (May be NULL, if it has not been set)*/
	const Type *get() const { return ptr; }

	operator Type *() { return ptr; }

	operator const Type *() const { return ptr; }

	operator CL_UnknownSharedPtr() { return CL_UnknownSharedPtr(*this, ptr); }

	operator CL_UnknownSharedPtr() const { return CL_UnknownSharedPtr(*this, (Type *) ptr); }

	Type *operator ->() { return ptr; }

	const Type *operator ->() const { return ptr; }

	template <typename OtherType>
	bool operator ==(OtherType *other) const { return ptr == other; }

	template <typename OtherType>
	bool operator !=(OtherType *other) const { return ptr != other; }

	template <typename OtherType>
	bool operator <(OtherType *other) const { return ptr < other; }

	template <typename OtherType>
	bool operator <=(OtherType *other) const { return ptr <= other; }

	template <typename OtherType>
	bool operator >(OtherType *other) const { return ptr > other; }

	template <typename OtherType>
	bool operator >=(OtherType *other) const { return ptr >= other; }

public:
	Type *ptr;

};



/// \}
