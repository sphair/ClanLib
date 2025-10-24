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

#ifndef clanlib_owningptr_header
#define clanlib_owningptr_header

//: Posessive pointer class (copying the pointer copies what it points to).
//- !group=Core/System!
//- !header=core.h!
//- <p> Use CL_OwningPtr when you require the data to be on the heap, but otherwise
//- want it to have very stack-instace-like semantics for copying, destruction, etc. </p>
template <typename T, typename U = T> class CL_OwningPtr
{
	//! Construction:
	public:
		//: Constructs a CL_OwningPtr.
		//param t: A pointer which was the return value of a 'new' call.
		//- <p> After the CL_OwningPtr has been constructed based on a passed pointer,
		//- the CL_OwningPtr takes full control over that data. The original pointer shouldn't
		//- be used to access or delete the data anymore; instead, use the CL_OwningPtr. </p>
		CL_OwningPtr() : ptr(0) {}
		CL_OwningPtr(const CL_OwningPtr<T, U>& other) : ptr((T*)(other.ptr->clone())) {}
		CL_OwningPtr(T* t) : ptr(t) {}
		
		~CL_OwningPtr() {delete ptr;}
	
	//! Attributes:
	public:		
		//: Returns true if this CL_OwningPtr is not dereferencable.
		bool is_null() const {if (ptr == 0) return true; return false;}
		
		//: Gives access to the pointer itself.
		//- <p> Be careful not to keep the returned pointer around after doing any
		//- non-const operations on the CL_LazyCopyPtr; it could be invalid
		//- after that.</p>
		U* get() {return (U*)ptr;}
		const U* get() const {return (const U*)ptr;}
		
	//! Operations:
	public:	
		//: Copy assignment operator.
		//param t: A pointer which was the return value of a 'new' call.
		//- <p> Once the assignment statement is finished when assigning a passed pointer,
		//- the CL_OwningPtr takes full control over that data. The original pointer shouldn't
		//- be used to access or delete the data anymore; instead, use the CL_OwningPtr. </p>
		CL_OwningPtr<T, U>& operator=(const CL_OwningPtr<T, U>& other)
		{
			if (&other != this)
			{
				T* cpy;
				if (other.ptr != 0)
					cpy = (T*)(other.ptr->clone());
				else
					cpy = 0;
				
				delete ptr;
				ptr = cpy;
			}
			
			return *this;
		}
		CL_OwningPtr<T, U>& operator=(T* t)
		{
			delete ptr;
			ptr = t;
			return *this;
		}
		
		//: Dereferencing operator.
		U& operator*() {return *((U*)ptr);}
		U const& operator*() const {return *((const U*)ptr);}
		
		//: Forces a const dereference.
		//- <p> Like operator*, but forces a const deref even if in non-const context. 
		//- This is intended mostly to make it easy to exchange CL_OwningPtr and CL_LazyCopyPtr. </p>
		U const& cd() const {return *((const U*)ptr);}
		
		//: Indirect member access operator.
		U* operator->() {return (U*)ptr;}
		U const* operator->() const {return (const U*)ptr;}
	
	//! Implementation:
	private:
		//Pointer to new-allocated data, or else 0
		T* ptr;
		
		//Not to be called, this just ensures that T is derived from CL_Clonable
		void dummy() { ptr->must_be_derived_from_cl_clonable(); }
};

#endif
