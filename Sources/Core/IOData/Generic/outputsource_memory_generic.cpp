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

#include <cstring>
#include "Core/precomp.h"

#include "API/Core/System/cl_assert.h"
#include <API/Core/IOData/outputsource.h>
#include "outputsource_memory_generic.h"

CL_OutputSource_MemoryGeneric::CL_OutputSource_MemoryGeneric(unsigned int blocksize)
{
	m_blocksize = blocksize;
	m_size = m_blocksize;
	m_data = new unsigned char[m_blocksize];
	m_pos = 0;
}

CL_OutputSource_MemoryGeneric::~CL_OutputSource_MemoryGeneric()
{
	delete[] m_data;
}

void *CL_OutputSource_MemoryGeneric::get_data() const
{
	return (void *) m_data;
}

int CL_OutputSource_MemoryGeneric::write(const void *data, int size)
{
	if (m_pos + size > m_size) // extend array with blocksize
	{
		int new_size = m_size + size + m_blocksize;

		unsigned char *old = m_data;
		m_data = new unsigned char[new_size];

		memcpy(m_data, old, m_pos);
		delete[] old;
		m_size = new_size;
	}
	
	memcpy(m_data + m_pos, data, size);
	m_pos += size;
	
	return size;
}

void CL_OutputSource_MemoryGeneric::open()
{
}

void CL_OutputSource_MemoryGeneric::close()
{
}

CL_OutputSource *CL_OutputSource_MemoryGeneric::clone()
{
	cl_assert(false); // not implemented yet.
	return NULL;
}

int CL_OutputSource_MemoryGeneric::tell() const
{
	return m_pos;
}

int CL_OutputSource_MemoryGeneric::size() const
{
	return m_pos;
}

void CL_OutputSource_MemoryGeneric::purge()
{
	memset(m_data,0,m_size);
}
