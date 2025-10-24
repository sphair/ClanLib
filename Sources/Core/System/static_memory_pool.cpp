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

#include "Core/precomp.h"
#include "API/Core/System/static_memory_pool.h"
#include "API/Core/System/exception.h"

/////////////////////////////////////////////////////////////////////////////
// CL_StaticMemoryPool Construction:

CL_StaticMemoryPool::CL_StaticMemoryPool(
	void *buffer,
	size_t size,
	CL_MemoryPool *alternative_pool)
: pool_buffer(buffer), size(size), alternative_pool(alternative_pool), next(0)
{
}

CL_StaticMemoryPool::~CL_StaticMemoryPool()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_StaticMemoryPool Operations:

void *CL_StaticMemoryPool::alloc(size_t req_size)
{
	if (req_size > size - next)
	{
		if (alternative_pool)
			return alternative_pool->alloc(req_size);
		else
			return new char[req_size];
	}

	char *ptr = (char *) pool_buffer + next;
	next += req_size;
	return ptr;
}
	
void CL_StaticMemoryPool::free(void *data)
{
	// Check if memory was allocated by this pool or not:
	if (data < pool_buffer || data >= ((char *) pool_buffer) + size)
	{
		// Punt free to the alternative pool, if present:
		if (alternative_pool)
			alternative_pool->free(data);
		else
			delete[] ((char *) data);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CL_StaticMemoryPool Implementation:
