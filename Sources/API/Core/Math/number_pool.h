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

//! clanCore="Math"
//! header=core.h

#ifndef header_number_pool
#define header_number_pool

#ifdef CL_API_DLL
#ifdef CL_CORE_EXPORT
#define CL_API_CORE __declspec(dllexport)
#else
#define CL_API_CORE __declspec(dllimport)
#endif
#else
#define CL_API_CORE
#endif

#if _MSC_VER > 1000
#pragma once
#endif

#include <climits>
#include <set>

//: A number generator pool.
//- !group=Core/Math!
//- !header=core.h!
//- <p>It generates unique numbers from a pool of ushort16.</p>
class CL_API_CORE CL_NumberPool
{
//! Construction:
public:
	//: Constructs a number pool.
	CL_NumberPool();

//! Operations:
public:
	//: Return a unique number.
	unsigned short get_number();

	//: Pass a number back in so as to beable to reuse the unique number.
	//- <p>NOTE: numbers that were not given before or already returned back will be discarded.</p>
	void return_number(unsigned short number);

//! Implementation:
private:
	//: A pool of available unique number.
	std::set<unsigned short> available_numbers;

	//: The initial number range.
	unsigned short initial_number_range;

};

#endif
