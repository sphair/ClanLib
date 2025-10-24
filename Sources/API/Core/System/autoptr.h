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

/// \addtogroup clanCore_System clanCore System
/// \{

template<typename Type>
/// \brief CL_AutoPtr
///
/// \xmlonly !group=Core/System! !header=core.h! \endxmlonly
class CL_AutoPtr
{
public:
	CL_AutoPtr()
	: ptr(0)
	{
	}

	CL_AutoPtr<Type>(CL_AutoPtr<Type> &other)
	: ptr(other.release())
	{
	}

	explicit CL_AutoPtr<Type>(Type *ptr)
	: ptr(ptr)
	{
	}

	~CL_AutoPtr()
	{
		delete ptr;
	}

	CL_AutoPtr<Type> &operator =(CL_AutoPtr<Type> &other)
	{
		if (this != &other)
			reset(other.release());
		return *this;
	}

	CL_AutoPtr<Type> &operator =(Type *ptr)
	{
		reset(ptr);
		return *this;
	}

	void reset(Type *p = 0)
	{
		delete ptr;
		ptr = p;
	}

	Type *release()
	{
		Type *p = ptr;
		ptr = 0;
		return p;
	}

	Type *get() { return ptr; }
	const Type *get() const { return ptr; }

	operator Type *() { return get(); }
	operator const Type *() const { return get(); }

	Type *operator->() { return get(); }
	const Type *operator->() const { return get(); }

private:
	Type *ptr;
};

template<typename Container>
void cl_delete_container(Container &container)
{
	for (typename Container::iterator it = container.begin(); it != container.end(); ++it)
		delete (*it);
	container.clear();
}

/// \}
