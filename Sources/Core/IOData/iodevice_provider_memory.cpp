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
#include "API/Core/IOData/iodevice_memory.h"
#include "iodevice_provider_memory.h"

/////////////////////////////////////////////////////////////////////////////
// CL_IODeviceProvider_Memory Construction:

CL_IODeviceProvider_Memory::CL_IODeviceProvider_Memory()
: position(0)
{
}

CL_IODeviceProvider_Memory::CL_IODeviceProvider_Memory(CL_DataBuffer &data)
: data(data), position(0)
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_IODeviceProvider_Memory Attributes:

int CL_IODeviceProvider_Memory::get_size() const
{
	return data.get_size();
}
	
int CL_IODeviceProvider_Memory::get_position() const
{
	validate_position();
	return position;
}

const CL_DataBuffer &CL_IODeviceProvider_Memory::get_data() const
{
	return data;
}

CL_DataBuffer &CL_IODeviceProvider_Memory::get_data()
{
	return data;
}

/////////////////////////////////////////////////////////////////////////////
// CL_IODeviceProvider_Memory Operations:

int CL_IODeviceProvider_Memory::send(const void *send_data, int len, bool send_all)
{
	validate_position();
	int size_needed = position + len;
	if (size_needed > data.get_size())
		data.set_size(size_needed);
	memcpy(data.get_data() + position, send_data, len);
	position += len;
	return len;
}

int CL_IODeviceProvider_Memory::receive(void *recv_data, int len, bool receive_all)
{
	validate_position();
	int data_available = data.get_size() - position;
	if (len > data_available)
		len = data_available;
	memcpy(recv_data, data.get_data() + position, len);
	position += len;
	return len;
}

int CL_IODeviceProvider_Memory::peek(void *recv_data, int len)
{
	validate_position();
	int data_available = data.get_size() - position;
	if (len > data_available)
		len = data_available;
	memcpy(recv_data, data.get_data() + position, len);
	return len;
}

bool CL_IODeviceProvider_Memory::seek(int requested_position, CL_IODevice::SeekMode mode)
{
	validate_position();
	int new_position = position;
	switch (mode)
	{
	case CL_IODevice::seek_set:
		new_position = requested_position;
		break;
	case CL_IODevice::seek_cur:
		new_position += requested_position;
		break;
	case CL_IODevice::seek_end:
		new_position = data.get_size() + requested_position;
		break;
	default:
		return false;
	}

	if (new_position >= 0 && new_position <= data.get_size())
	{
		position = new_position;
		return true;
	}
	else
	{
		return false;
	}
}

CL_IODeviceProvider *CL_IODeviceProvider_Memory::duplicate()
{
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CL_IODeviceProvider_Memory Implementation:

void CL_IODeviceProvider_Memory::validate_position() const
{
	if (position < 0)
		position = 0;
	else if (position > data.get_size())
		position = data.get_size();
}
