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

#include "Core/precomp.h"
#include "API/Core/Math/number_pool.h"

CL_NumberPool::CL_NumberPool() :
initial_number_range(0)
{
}

//: Return a unique number.
unsigned short CL_NumberPool::get_number()
{
	unsigned short ret_val;

	// look for available numbers in the number pool
	if (available_numbers.size() > 0)
	{
		std::set<unsigned short>::iterator iter;

		// get the first number available
		iter = available_numbers.begin();

		// store data to the return variable
		ret_val = *iter;

		// now remove the number from the pool
		available_numbers.erase(iter);
	}
	// there are no available numbers in the number pool
	else
	{
		// make sure that the initial_number_range has not reached the max
		if (initial_number_range == USHRT_MAX)
		{
			// throw an exeption stating that no more unique numbers can be generated
			throw CL_Error("CL_NumberPool::get_number failed: All unique numbers have been used up.");
		}

		// generate the a new available number
		ret_val = initial_number_range++;
	}

	return ret_val;
}

//: Pass a number back in so as to beable to reuse the unique number. NOTE: numbers that were not given before or already returned back will be discarded.
void CL_NumberPool::return_number(unsigned short number)
{
	// check if the given number is within the initial_number_range
	// if it's not, discard it
	if (number >= initial_number_range)
		return;

	// add number into list, be it already inserted or not.
	// since a set can only have unique data, so it doesn't matters
	available_numbers.insert(number);
}
