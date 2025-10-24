/*
**  ClanLib SDK
**  Copyright (c) 1997-2005 The ClanLib Team
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
**    (if your name is missing here, please add it)
*/

//! clanCore="System"
//! header=core.h

#ifndef clanlib_mutexweakptr_header
#define clanlib_mutexweakptr_header

#include "sharedptr.h"

//: Weak pointer class (pointer to a CL_MutexSharedPtr object that dont increase reference count).
//- !group=Core/System!
//- !header=core.h!
//- <p>Use CL_MutexWeakPtr when you want to have a pointer that is reference counted by CL_MutexSharedPtr
//- but want a pointer that dont increase the reference count.</p>
//- <p>The purpose of CL_MutexWeakPtr is to avoid circular loop issues. By using CL_MutexWeakPtr you
//- can construct new CL_MutexSharedPtr'ed objects based on the weak pointer.</p>
template <typename T, typename U = T>
class CL_MutexWeakPtr
{
//! Construction:
public:
	CL_MutexWeakPtr() : impl(0) { return; }

	CL_MutexWeakPtr(CL_MutexSharedPtr<T, U> &other) : impl(other.get_impl()) { return; }

//! Operations:
public:
	//: Returns true if this CL_MutexSharedPtr is not dereferencable.
	bool is_null() const { return impl ? (impl->ptr == 0) : true; }

	//: Returns number of references (including this one) to the data cache.
	//- <p> Returns 0 if this pointer is null. </p>
	int get_ref_count() const { if (impl == 0) return 0; return impl->ref_count; }
	
	//: Gives access to the pointer itself.
	//- <p> Be careful not to keep the returned pointer around after doing any
	//- non-const operations on the CL_LazyCopyPtr; it could be invalid
	//- after that.</p>
	U* get() { return (U*) ((impl != 0) ? impl->ptr : 0); }

	const U* get() const { return (const U*) ((impl != 0) ? impl->ptr : 0); }

	//: Returns the pointer.
	operator const U*() const { return get(); }

	//: Returns the pointer as a shared ptr.
	operator CL_MutexSharedPtr<T, U>() const { return CL_MutexSharedPtr<T, U>(impl); }

	//: Dereferencing operator.
	U& operator*() { return *((U*) impl->ptr); }

	U const& operator*() const { return *((const U*) impl->ptr); }
	
	//: Indirect member access operator.
	U* operator->() { return (U*) impl->ptr; }

	U const* operator->() const { return (const U*) impl->ptr; }

	//: Pointer equality check operator.
	//- <p> This will return true if the CL_MutexSharedPtrs point to the same data. It doesn't
	//- check the data itself for equality. </p>
	bool operator==(const T* other) const { return other == ((impl != 0) ? impl->ptr : 0); }

	bool operator==(const CL_MutexSharedPtr<T, U>& other) const { return other.impl == impl; }

//! Implementation:
private:
	CL_MutexSharedPtr_Generic<T> *impl;
};

#endif
