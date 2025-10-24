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
**    Harry Storbacka
*/

/// \addtogroup clanCore_System clanCore System
/// \{


#pragma once


#include "../api_core.h"
#include "sharedptr.h"

/// \brief Weak pointer class (pointer to a CL_SharedPtr object that dont increase reference count).
///
///   <p>Use CL_WeakPtr when you want to have a pointer that is reference counted by CL_SharedPtr
///    but want a pointer that dont increase the reference count.</p>
///    <p>The purpose of CL_WeakPtr is to avoid circular loop issues. By using CL_WeakPtr you
///    can construct new CL_SharedPtr'ed objects based on the weak pointer.</p> 
/// \xmlonly !group=Core/System! !header=core.h! \endxmlonly
template <typename Type>
class CL_WeakPtr : public CL_SharedPtr_Link
{
public:
	CL_WeakPtr()
	: ptr(0)
	{
		set_weak_link();
	}

	CL_WeakPtr(const CL_WeakPtr<Type> &copy)
	: ptr(0)
	{
		set_weak_link();
		connect(copy);
		ptr = copy.ptr;
	}

	explicit CL_WeakPtr(const CL_SharedPtr<Type> &copy)
	: ptr(0)
	{
		set_weak_link();
		connect(copy);
		ptr = copy.ptr;
	}

	explicit CL_WeakPtr(const CL_UnknownSharedPtr &copy)
	: ptr(0)
	{
		set_weak_link();
		connect(copy);
		ptr = (Type *) copy.ptr;
	}

	~CL_WeakPtr()
	{
		disconnect();
	}

	CL_WeakPtr &operator =(const CL_UnknownSharedPtr &copy)
	{
		disconnect();
		connect(copy);
		ptr = (Type *) copy.ptr;
		set_weak_link();
		return *this;
	}

	CL_WeakPtr &operator =(const CL_SharedPtr<Type> &copy)
	{
		disconnect();
		connect(copy);
		ptr = copy.ptr;
		set_weak_link();
		return *this;
	}

	CL_WeakPtr &operator =(const CL_WeakPtr &copy)
	{
		if (this == &copy)
			return *this;
		disconnect();
		connect(copy);
		ptr = copy.ptr;
		set_weak_link();
		return *this;
	}

	/// \brief Disconnect from linked list and unset the pointer
	/** \return true if the list is empty or only contains weak links*/
	bool disconnect()
	{
		bool result = CL_SharedPtr_Link::disconnect();
		ptr = NULL;
		return result;
	}

	/// \brief Tests if the pointer is unset, or the weak link is no longer valid
	/** \return true, pointer not set (or not valid)*/
	bool is_null() const { return is_invalid_weak_link() || ptr == 0; }

	/// \brief Retrieves the pointer
	/** \return The pointer (May be NULL, if it has not been set or the weak link is no longer valid)*/
	Type *get() { return !is_invalid_weak_link() ? ptr : 0; }

	/// \brief Retrieves the pointer
	/** \return The pointer (May be NULL, if it has not been set or the weak link is no longer valid)	*/
	const Type *get() const { return !is_invalid_weak_link() ? ptr : 0; }

	CL_UnknownSharedPtr to_unknownptr() const
	{
		if (!is_invalid_weak_link())
			return CL_UnknownSharedPtr(*this, (Type *) ptr);
		else
			return CL_UnknownSharedPtr();
	}

	CL_SharedPtr<Type> to_sharedptr() const
	{
		if (!is_invalid_weak_link())
			return CL_SharedPtr<Type>(*this, (Type *) ptr);
		else
			return CL_SharedPtr<Type>();
	}

	operator Type *() { return get(); }

	operator const Type *() const { return get(); }

	operator CL_UnknownSharedPtr() { return to_unknownptr(); }

	operator CL_UnknownSharedPtr() const { return to_unknownptr(); }

	operator CL_SharedPtr<Type>() { return to_sharedptr(); }

	operator CL_SharedPtr<Type>() const { return to_sharedptr(); }

	Type *operator ->() { return get(); }

	const Type *operator ->() const { return get(); }

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
