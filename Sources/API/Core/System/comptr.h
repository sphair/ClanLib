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

#pragma once


#include "../api_core.h"

template <typename Type>
/// \brief CL_ComPtr
///
/// \xmlonly !group=Core/System! !header=core.h! \endxmlonly
class CL_API_CORE CL_ComPtr
{
public:
	CL_ComPtr() : ptr(0) { }
	CL_ComPtr(Type *ptr) : ptr(ptr) { }
	CL_ComPtr(const CL_ComPtr &copy) : ptr(copy.ptr) { if (ptr) ptr->AddRef(); }
	~CL_ComPtr() { clear(); }
	CL_ComPtr &operator =(const CL_ComPtr &copy)
	{
		if (this == &copy)
			return *this;
		copy.ptr->AddRef();
		if (ptr)
			ptr->Release();
		ptr = copy.ptr;
		return *this;
	}
	const Type * const operator ->() const { return ptr; }
	Type *operator ->() { return ptr; }
	operator Type *() { return ptr; }

	bool is_null() const { return ptr == 0; }
	void clear() { if (ptr) ptr->Release(); ptr = 0; }
	Type **output_variable() { clear(); return &ptr; }

	Type *ptr;
};


