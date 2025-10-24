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
#include <API/Core/System/cl_assert.h>
#include <API/Core/IOData/outputsource_memory.h>

CL_OutputSource_Memory::CL_OutputSource_Memory()
{
	pos = 0;
}

CL_OutputSource_Memory::~CL_OutputSource_Memory()
{
}

std::string CL_OutputSource_Memory::get_data() const
{
	return data;
}

int CL_OutputSource_Memory::write(const void *source_data, int size)
{
	data.append((const char *) source_data, size);
	pos += size;
	
	return size;
}

void CL_OutputSource_Memory::open()
{
}

void CL_OutputSource_Memory::close()
{
}

CL_OutputSource *CL_OutputSource_Memory::clone()
{
	cl_assert(false); // not implemented yet.
	return NULL;
}

int CL_OutputSource_Memory::tell() const 
{
	return pos;
}

int CL_OutputSource_Memory::size() const 
{
	return pos;
}
