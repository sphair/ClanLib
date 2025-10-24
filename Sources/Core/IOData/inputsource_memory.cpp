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
#include <API/Core/System/cl_assert.h>
#include <API/Core/System/clanstring.h>
#include <API/Core/IOData/inputsource_memory.h>

CL_InputSource_Memory::CL_InputSource_Memory(const std::string &_data) : pos(0), str_data(_data)
{
	data = (unsigned char *) str_data.c_str();
	data_size = str_data.size();
}

CL_InputSource_Memory::CL_InputSource_Memory(void *_data, int size, bool copy_data) : data((unsigned char *) _data), data_size(size), pos(0)
{
	if (copy_data)
	{
		str_data = std::string((char *) data, size);
		data = (unsigned char *) str_data.c_str();
	}
}

CL_InputSource_Memory::~CL_InputSource_Memory()
{
}

int CL_InputSource_Memory::read(void *dest_data, int size)
{
	if (size < 0)
		return 0;

	if (pos + size > data_size)
	{
		size = data_size - pos;
		if (size <= 0)
			return 0;
	}

	memcpy(dest_data, data + pos, size);
	pos += size;

	return size;
}

void CL_InputSource_Memory::open()
{
	pos = 0;
}

void CL_InputSource_Memory::close()
{
}

CL_InputSource *CL_InputSource_Memory::clone() const
{
	return new CL_InputSource_Memory(data, data_size, true);
}

int CL_InputSource_Memory::tell() const
{
	return pos;
}

void CL_InputSource_Memory::seek(int new_pos, SeekEnum seek_type)
{
	switch (seek_type)
	{
	case seek_set:
		pos = new_pos;
		break;
	
	case seek_cur:
		pos += new_pos;
		break;
	
	case seek_end:
		pos = data_size + new_pos;
		break;
	
	default: // invalid seek type!
		cl_assert(false);
	}
}

int CL_InputSource_Memory::size() const
{
	return data_size;
}

void CL_InputSource_Memory::push_position()
{
	cl_assert(false); // not implemented yet.
}

void CL_InputSource_Memory::pop_position()
{
	cl_assert(false); // not implemented yet.
}

std::string CL_InputSource_Memory::get_data() const
{
	return str_data;
}

