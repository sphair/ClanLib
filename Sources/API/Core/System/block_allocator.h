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
#include "sharedptr.h"
#include "system.h"

class CL_BlockAllocator_Impl;

/// \brief Memory allocator that allocates in blocks.
///
/// <p>This allocator will never free any memory until free() is called or
///    the allocator object is destroyed. If any objects have been new'ed using
///    CL_BlockAllocated they have to be delete'ed before free() is called,
///    otherwise the destructors of the objects will not get called. Same rules
///    apply to the new_obj and delete_obj template functions.</p>
///    <p>The allocator allocates memory from the heap in blocks. Each time the
///    allocated block is exhausted, the allocator will double the block size
///    and allocate more memory.</p>
/// \xmlonly !group=Core/System! !header=core.h! \endxmlonly
class CL_API_CORE CL_BlockAllocator
{
/// \name Construction
/// \{

public:
	/// \brief Block Allocator constructor
	CL_BlockAllocator();


/// \}
/// \name Attributes
/// \{

public:


/// \}
/// \name Operations
/// \{

public:
	/// \brief Allocate memory (See note on this class for the allocation method)
	/** Also see: new_obj() which allocates and calls the constructor
	    param: size = Size to allocate (in bytes)
	    \return The memory*/
	void *allocate(int size);

	/// \brief Free the allocated memory
	/** If required, use delete_obj() to call the destructor before using this function*/
	void free();

	/// \brief Allocate a class (And call it's constructor)
	/** \return The class*/
	template<typename Type>
	Type *new_obj()
	{
		Type *data = (Type *) allocate(sizeof(Type));
		CL_System::call_constructor<Type>(data);
		return data;
	}

	/// \brief Allocate a class (And call it's constructor)
	/** param: p1 = Parameter 1
	    \return The class*/
	template<typename Type, typename P1>
	Type *new_obj(P1 p1)
	{
		Type *data = (Type *) allocate(sizeof(Type));
		CL_System::call_constructor<Type, P1>(data, p1);
		return data;
	}

	/// \brief Allocate a class (And call it's constructor)
	/** param: p1 = Parameter 1
	    param: p2 = Parameter 2
	    \return The class*/
	template<typename Type, typename P1, typename P2>
	Type *new_obj(P1 p1, P2 p2)
	{
		Type *data = (Type *) allocate(sizeof(Type));
		CL_System::call_constructor<Type, P1, P2>(data, p1, p2);
		return data;
	}

	/// \brief Allocate a class (And call it's constructor)
	/** param: p1 = Parameter 1
	    param: p2 = Parameter 2
	    param: p3 = Parameter 3
	    \return The class*/
	template<typename Type, typename P1, typename P2, typename P3>
	Type *new_obj(P1 p1, P2 p2, P3 p3)
	{
		Type *data = (Type *) allocate(sizeof(Type));
		CL_System::call_constructor<Type, P1, P2, P3>(data, p1, p2, p3);
		return data;
	}

	/// \brief Allocate a class (And call it's constructor)
	/** param: p1 = Parameter 1
	    param: p2 = Parameter 2
	    param: p3 = Parameter 3
	    param: p4 = Parameter 4
	    \return The class*/
	template<typename Type, typename P1, typename P2, typename P3, typename P4>
	Type *new_obj(P1 p1, P2 p2, P3 p3, P4 p4)
	{
		Type *data = (Type *) allocate(sizeof(Type));
		CL_System::call_constructor<Type, P1, P2, P3, P4>(data, p1, p2, p3, p4);
		return data;
	}

	/// \brief Allocate a class (And call it's constructor)
	/** param: p1 = Parameter 1
	    param: p2 = Parameter 2
	    param: p3 = Parameter 3
	    param: p4 = Parameter 4
	    param: p5 = Parameter 5
	    \return The class*/
	template<typename Type, typename P1, typename P2, typename P3, typename P4, typename P5>
	Type *new_obj(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
	{
		Type *data = (Type *) allocate(sizeof(Type));
		CL_System::call_constructor<Type, P1, P2, P3, P4, P5>(data, p1, p2, p3, p4, p5);
		return data;
	}

	/// \brief Allocate a class (And call it's constructor)
	/** param: p1 = Parameter 1
	    param: p2 = Parameter 2
	    param: p3 = Parameter 3
	    param: p4 = Parameter 4
	    param: p5 = Parameter 5
	    param: p6 = Parameter 6
	    \return The class*/
	template<typename Type, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
	Type *new_obj(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
	{
		Type *data = (Type *) allocate(sizeof(Type));
		CL_System::call_constructor<Type, P1, P2, P3, P4, P5, P6>(data, p1, p2, p3, p4, p5, p6);
		return data;
	}

	/// \brief Allocate a class (And call it's constructor)
	/** param: p1 = Parameter 1
	    param: p2 = Parameter 2
	    param: p3 = Parameter 3
	    param: p4 = Parameter 4
	    param: p5 = Parameter 5
	    param: p6 = Parameter 6
	    param: p7 = Parameter 7
	    \return The class*/
	template<typename Type, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
	Type *new_obj(P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
	{
		Type *data = (Type *) allocate(sizeof(Type));
		CL_System::call_constructor<Type, P1, P2, P3, P4, P5, P6, P7>(data, p1, p2, p3, p4, p5, p6, p7);
		return data;
	}

	/// \brief Call the class destructor
	/** This template does not call free()
	    param: obj = the object which to call the destructor*/
	template<typename Type>
	void delete_obj(Type *obj)
	{
		CL_System::call_destructor(obj);
	}


/// \}
/// \name Implementation
/// \{

private:
	CL_SharedPtr<CL_BlockAllocator_Impl> impl;
/// \}
};

/// \brief Class with operator new/delete overloads for CL_BlockAllocator.
///
///    <p>To use this class, derive your class from CL_BlockAllocated. Then, to
///    allocate your class, use the following operator new syntax:</p>
///    <pre>
///      CL_BlockAllocator allocator;
///      MyObject *obj = new(&allocator) MyObject(..);
///    </pre>
///    <p>Before freeing up the memory using CL_BlockAllocator::free() you have
///    to delete the object first using operator delete. If you fail to do this
///    the destructor will not get called.</p>
/// \xmlonly !group=Core/System! !header=core.h! \endxmlonly
class CL_BlockAllocated
{
/// \name Operations
/// \{

public:
	void *operator new(size_t size, CL_BlockAllocator *allocator);

	void operator delete(void *data, size_t size);

	void operator delete(void *data, CL_BlockAllocator *allocator);
/// \}
};



/// \}
