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
#include "inputsource_memory_generic.h"
#include "outputsource_memory_generic.h"

CL_InputSource_MemoryGeneric::CL_InputSource_MemoryGeneric(
	void *_data,
	int size,
	bool delete_data)
{
	data = (unsigned char *) _data;
	pos = 0;
	length = size;
	delete_data = delete_data;
}

CL_InputSource_MemoryGeneric::CL_InputSource_MemoryGeneric(
	const CL_InputSource_MemoryGeneric *MG)
{
	data = new unsigned char[MG->size()];
	pos = 0;
	length = MG->size();
	delete_data = true;
	memcpy(data, MG->data, length);
}

CL_InputSource_MemoryGeneric::~CL_InputSource_MemoryGeneric()
{
	if (delete_data) delete[] data;
}

int CL_InputSource_MemoryGeneric::read(void *_data, int size)
{
	cl_assert(pos + size <= length);
	
	memcpy(_data, data + pos, size);
	pos += size;

	return size;
}

void CL_InputSource_MemoryGeneric::open()
{
	pos = 0;
}

void CL_InputSource_MemoryGeneric::close()
{
}

CL_InputSource *CL_InputSource_MemoryGeneric::clone() const
{
	CL_InputSource_MemoryGeneric *ret = new CL_InputSource_MemoryGeneric(data, length, delete_data);
	ret->seek(pos, seek_set);
	return ret;
}

int CL_InputSource_MemoryGeneric::tell() const
{
	return pos;
}

void CL_InputSource_MemoryGeneric::seek(int _pos, SeekEnum seek_type)
{
	switch (seek_type)
	{
	case seek_set:
		pos = _pos;
		break;
	
	case seek_cur:
		pos += _pos;
		break;
	
	case seek_end:
		pos = length + _pos;
		break;
	
	default: // invalid seek type!
		cl_assert(false);
	}
}

int CL_InputSource_MemoryGeneric::size() const
{
	return length;
}

void CL_InputSource_MemoryGeneric::push_position()
{
	cl_assert(false); // not implemented yet.
}

void CL_InputSource_MemoryGeneric::pop_position()
{
	cl_assert(false); // not implemented yet.
}

void CL_InputSource_MemoryGeneric::purge()
{
	memset(data, 0, length);
}

CL_InputSourceProvider_Memory::CL_InputSourceProvider_Memory(unsigned char *_data, unsigned int _size, bool _delete_data)
{
	data = _data;
	size = _size;
	delete_data = _delete_data;
}

CL_InputSourceProvider_Memory::CL_InputSourceProvider_Memory(CL_InputSource_MemoryGeneric *MG)
{
	data = new unsigned char[MG->size()];
	size = MG->size();
	delete_data = true;
	MG->read(data, size);
}

CL_InputSourceProvider_Memory::CL_InputSourceProvider_Memory(CL_OutputSource_MemoryGeneric *MG)
{
	data = new unsigned char[MG->size()];
	size = MG->size();
	delete_data = true;
	memcpy(data, MG->get_data(), size);
}

CL_InputSource *CL_InputSourceProvider_Memory::open_source(const std::string &)
{
	return new CL_InputSource_MemoryGeneric(data, size, delete_data);
}

CL_InputSourceProvider *CL_InputSourceProvider_Memory::clone()
{
 	cl_assert(false);
 	return NULL;
}

