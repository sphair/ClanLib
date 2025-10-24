/*
**  ClanLib SDK
**  Copyright (c) 1997-2009 The ClanLib Team
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

#ifndef WIN32
#include "API/Core/IOData/datatypes.h"
#endif

class InterlockedVariable
{
public:
	InterlockedVariable()
	{
		set(0);
	}

#ifdef WIN32
	LONG get()
	{
		return InterlockedCompareExchange(&val, 0, 0);
	}

	void set(LONG new_value)
	{
		InterlockedExchange(&val, new_value);
	}

	LONG increment()
	{
		return InterlockedIncrement(&val);
	}

	LONG decrement()
	{
		return InterlockedDecrement(&val);
	}

	bool compare_and_swap(LONG expected_value, LONG new_value)
	{
		return InterlockedCompareExchange(&val, new_value, expected_value) == expected_value;
	}

private:
	__declspec(align(32)) volatile LONG val;

#else
	int get()
	{
		return __sync_val_compare_and_swap(&val, 0, 0);
	}

	void set(int new_value)
	{
		__sync_lock_test_and_set(&val, new_value);
	}

	int increment()
	{
		return __sync_fetch_and_add(&val, 1) + 1;
	}

	int decrement()
	{
		return __sync_fetch_and_add(&val, -1) - 1;
	}

	bool compare_and_swap(int expected_value, int new_value)
	{
		return __sync_val_compare_and_swap(&val, new_value, expected_value) == expected_value;
	}

private:
	__attribute__ ((aligned(32))) volatile cl_int32 val;

#endif
};
