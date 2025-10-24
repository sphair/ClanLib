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
#include "API/Core/Text/string_allocator.h"
#include "API/Core/System/databuffer.h"
#include <vector>

#define cl_max(a, b) ((a) > (b) ? (a) : (b))

/////////////////////////////////////////////////////////////////////////////
// CL_StringAllocator_Impl class:

class CL_StringAllocator_Impl
{
//! Construction:
public:
	CL_StringAllocator_Impl() : block_pos(0)
	{
	}

//! Attributes:
public:
	std::vector<CL_DataBuffer> blocks;
	
	int block_pos;

//! Operations:
public:
	void *alloc(int size)
	{
		if (blocks.empty())
			blocks.push_back(CL_DataBuffer(size*10));
		CL_DataBuffer &cur = blocks.back();
		if (block_pos + size <= cur.get_size())
		{
			void *data = cur.get_data() + block_pos;
			block_pos += size;
			return data;
		}
		blocks.push_back(CL_DataBuffer(cl_max(cur.get_size()*2, size)));
		block_pos = size;
		return blocks.back().get_data();
	}
};

/////////////////////////////////////////////////////////////////////////////
// CL_StringAllocator construction:

CL_StringAllocator::CL_StringAllocator()
: impl(new CL_StringAllocator_Impl)
{
}
	
CL_StringAllocator::CL_StringAllocator(const CL_StringAllocator &source)
: impl(source.impl)
{
}

CL_StringAllocator::~CL_StringAllocator()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_StringAlloactor operations:

CL_StringAllocator &CL_StringAllocator::operator =(const CL_StringAllocator &source)
{
	impl = source.impl;
	return *this;
}

CL_StringRef8 CL_StringAllocator::alloc(const CL_StringRef8 &str)
{
	CL_StringRef8::size_type length = str.length();
	char *tmpstr = (char *) impl->alloc(length+1);
	memcpy(tmpstr, str.data(), length);
	tmpstr[length] = 0;
	return CL_StringRef8(tmpstr, length, true);
}

CL_StringRef8 CL_StringAllocator::alloc(const char *str)
{
	CL_StringRef8::size_type length = (CL_StringRef8::size_type) strlen(str);
	char *tmpstr = (char *) impl->alloc(length+1);
	memcpy(tmpstr, str, length);
	tmpstr[length] = 0;
	return CL_StringRef8(tmpstr, length, true);
}

CL_StringRef8 CL_StringAllocator::alloc(
	const char *str,
	CL_StringRef8::size_type length)
{
	char *tmpstr = (char *) impl->alloc(length+1);
	memcpy(tmpstr, str, length);
	tmpstr[length] = 0;
	return CL_StringRef8(tmpstr, length, true);
}

CL_StringRef16 CL_StringAllocator::alloc(const CL_StringRef16 &str)
{
	CL_StringRef16::size_type length = str.length();
	wchar_t *tmpstr = (wchar_t *) impl->alloc((length+1)*sizeof(wchar_t));
	memcpy(tmpstr, str.data(), length * sizeof(wchar_t));
	tmpstr[length] = 0;
	return CL_StringRef16(tmpstr, length, true);
}

CL_StringRef16 CL_StringAllocator::alloc(const wchar_t *str)
{
	CL_StringRef16::size_type length = (CL_StringRef16::size_type) wcslen(str);
	wchar_t *tmpstr = (wchar_t *) impl->alloc((length+1)*sizeof(wchar_t));
	memcpy(tmpstr, str, length * sizeof(wchar_t));
	tmpstr[length] = 0;
	return CL_StringRef16(tmpstr, length, true);
}

CL_StringRef16 CL_StringAllocator::alloc(
	const wchar_t *str,
	CL_StringRef16::size_type length)
{
	wchar_t *tmpstr = (wchar_t *) impl->alloc((length+1)*sizeof(wchar_t));
	memcpy(tmpstr, str, length * sizeof(wchar_t));
	tmpstr[length] = 0;
	return CL_StringRef16(tmpstr, length, true);
}

CL_StringRef CL_StringAllocator::alloc(unsigned int length)
{
	CL_StringRef::char_type *tmpstr = (CL_StringRef::char_type *) impl->alloc((length+1)*sizeof(CL_StringRef::char_type));
	memset(tmpstr, 0, (length+1) * sizeof(CL_StringRef::char_type));
	return CL_StringRef(tmpstr, length, true);
}

CL_StringRef8 CL_StringAllocator::alloc8(unsigned int length)
{
	char *tmpstr = (char *) impl->alloc(length+1);
	memset(tmpstr, 0, length+1);
	return CL_StringRef8(tmpstr, length, true);
}

CL_StringRef16 CL_StringAllocator::alloc16(unsigned int length)
{
	wchar_t *tmpstr = (wchar_t *) impl->alloc((length+1)*sizeof(wchar_t));
	memset(tmpstr, 0, (length+1) * sizeof(wchar_t));
	return CL_StringRef16(tmpstr, length, true);
}

void CL_StringAllocator::clear()
{
	impl->blocks.clear();
	impl->block_pos = 0;
}

/////////////////////////////////////////////////////////////////////////////
// CL_StringAllocator implementation:
