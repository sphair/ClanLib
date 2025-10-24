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

#ifndef WIN32
#include "../IOData/datatypes.h"
#endif

// __sync_val_compare_and_swap was not introduced until gcc 4.1
#if defined(WIN32) || __GNUC__ > 4 ||  (__GNUC__ == 4 & __GNUC_MINOR__ >= 1)

/// \brief Interlocked variable class
///
/// \xmlonly !group=Core/System! !header=core.h! \endxmlonly
class CL_InterlockedVariable
{
public:
#ifdef WIN32
	CL_InterlockedVariable()
	: val((LONG*)CL_System::aligned_alloc(sizeof(LONG), 4))
	{
		set(0);
	}

	CL_InterlockedVariable(const CL_InterlockedVariable &src)
	: val((LONG*)CL_System::aligned_alloc(sizeof(LONG), 4))
	{
		set(src.get());
	}

	~CL_InterlockedVariable()
	{
		CL_System::aligned_free((void*)val);
	}

	CL_InterlockedVariable &operator =(const CL_InterlockedVariable &src)
	{
		set(src.get());
		return *this;
	}

	LONG get() const
	{
		return InterlockedCompareExchange(val, 0, 0);
	}

	void set(LONG new_value)
	{
		InterlockedExchange(val, new_value);
	}

	LONG increment()
	{
		return InterlockedIncrement(val);
	}

	LONG decrement()
	{
		return InterlockedDecrement(val);
	}

	bool compare_and_swap(LONG expected_value, LONG new_value)
	{
		return InterlockedCompareExchange(val, new_value, expected_value) == expected_value;
	}

private:
	volatile LONG *val;

#else
	CL_InterlockedVariable()
	: val((cl_int32*)CL_System::aligned_alloc(sizeof(cl_int32), 4))
	{
		set(0);
	}

	CL_InterlockedVariable(const CL_InterlockedVariable &src)
	: val((cl_int32*)CL_System::aligned_alloc(sizeof(cl_int32), 4))
	{
		set(src.get());
	}

	~CL_InterlockedVariable()
	{
		CL_System::aligned_free((void*)val);
	}

	CL_InterlockedVariable &operator =(const CL_InterlockedVariable &src)
	{
		set(src.get());
		return *this;
	}

	int get() const
	{
		return __sync_val_compare_and_swap(val, 0, 0);
	}

	void set(int new_value)
	{
		__sync_lock_test_and_set(val, new_value);
	}

	int increment()
	{
		return __sync_add_and_fetch(val, 1);
	}

	int decrement()
	{
		return __sync_sub_and_fetch(val, 1);
	}

	bool compare_and_swap(int expected_value, int new_value)
	{
		return __sync_bool_compare_and_swap(val, expected_value,  new_value);
	}

private:
	volatile cl_int32 *val;

#endif
};

#endif

/// \}
