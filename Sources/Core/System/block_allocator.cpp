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
#include "API/Core/System/block_allocator.h"
#include "API/Core/System/databuffer.h"
#include <vector>

#define cl_max(a, b) ((a) > (b) ? (a) : (b))

/////////////////////////////////////////////////////////////////////////////
// CL_BlockAllocator_Impl class:

class CL_BlockAllocator_Impl
{
public:
	std::vector<CL_DataBuffer> blocks;

	int block_pos;
};

/////////////////////////////////////////////////////////////////////////////
// CL_BlockAllocator construction:

CL_BlockAllocator::CL_BlockAllocator()
: impl(new CL_BlockAllocator_Impl)
{
	impl->block_pos = 0;
}

/////////////////////////////////////////////////////////////////////////////
// CL_BlockAllocator attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_BlockAllocator operations:

void *CL_BlockAllocator::allocate(int size)
{
	if (impl->blocks.empty())
		impl->blocks.push_back(CL_DataBuffer(size*10));
	CL_DataBuffer &cur = impl->blocks.back();
	if (impl->block_pos + size <= cur.get_size())
	{
		void *data = cur.get_data() + impl->block_pos;
		impl->block_pos += size;
		return data;
	}
	impl->blocks.push_back(CL_DataBuffer(cl_max(cur.get_size()*2, size)));
	impl->block_pos = size;
	return impl->blocks.back().get_data();
}

void CL_BlockAllocator::free()
{
	impl->blocks.clear();
	impl->block_pos = 0;
}

/////////////////////////////////////////////////////////////////////////////
// CL_BlockAllocator implementation:

/////////////////////////////////////////////////////////////////////////////
// CL_BlockAllocated operations:

void *CL_BlockAllocated::operator new(size_t size, CL_BlockAllocator *allocator)
{
	return allocator->allocate(size);
}

void CL_BlockAllocated::operator delete(void *data, size_t size)
{
}

void CL_BlockAllocated::operator delete(void *data, CL_BlockAllocator *allocator)
{
}
