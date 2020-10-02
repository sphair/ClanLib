/*
**  ClanLib SDK
**  Copyright (c) 1997-2020 The ClanLib Team
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
#include "API/Core/IOData/memory_device.h"
#include "iodevice_provider_memory.h"
#include "API/Core/Math/cl_math.h"

namespace clan
{
	IODeviceProvider_Memory::IODeviceProvider_Memory()
		: position(0)
	{
	}

	IODeviceProvider_Memory::IODeviceProvider_Memory(DataBuffer &data)
		: data(data), position(0)
	{
	}

	size_t IODeviceProvider_Memory::get_size() const
	{
		return data.get_size();
	}

	size_t IODeviceProvider_Memory::get_position() const
	{
		validate_position();
		return position;
	}

	const DataBuffer &IODeviceProvider_Memory::get_data() const
	{
		return data;
	}

	DataBuffer &IODeviceProvider_Memory::get_data()
	{
		return data;
	}

	size_t IODeviceProvider_Memory::send(const void *send_data, size_t len, bool send_all)
	{
		validate_position();
		size_t size_needed = position + len;
		if (size_needed > data.get_size())
		{
			if (size_needed > data.get_capacity())	// Capacity exceeded
			{
				// Estimate the optimum databuffer capacity. TODO: Maybe adjust this class to be link list based, thus removing reallocation and block movement of the DataBuffer
				data.set_capacity(clan::max(size_needed + clan::min(size_needed, 16 * 1024 * 1024), 16 * 1024));
			}

			data.set_size(size_needed);
		}
		memcpy(data.get_data() + position, send_data, len);
		position += len;
		return len;
	}

	size_t IODeviceProvider_Memory::receive(void *recv_data, size_t len, bool receive_all)
	{
		validate_position();
		size_t data_available = data.get_size() - position;
		if (len > data_available)
			len = data_available;
		memcpy(recv_data, data.get_data() + position, len);
		position += len;
		return len;
	}

	size_t IODeviceProvider_Memory::peek(void *recv_data, size_t len)
	{
		validate_position();
		size_t data_available = data.get_size() - position;
		if (len > data_available)
			len = data_available;
		memcpy(recv_data, data.get_data() + position, len);
		return len;
	}

	bool IODeviceProvider_Memory::seek(int requested_position, IODevice::SeekMode mode)
	{
		validate_position();
		int new_position = int(position);
		switch (mode)
		{
		case IODevice::SeekMode::set:
			new_position = requested_position;
			break;
		case IODevice::SeekMode::cur:
			new_position += requested_position;
			break;
		case IODevice::SeekMode::end:
			new_position = int(data.get_size()) + requested_position;
			break;
		default:
			return false;
		}

		if (new_position >= 0 && new_position <= int(data.get_size()))
		{
			position = size_t(new_position);
			return true;
		}
		else
		{
			return false;
		}
	}

	IODeviceProvider *IODeviceProvider_Memory::duplicate()
	{
		return new IODeviceProvider_Memory(data);
	}

	void IODeviceProvider_Memory::validate_position() const
	{
		if (position > data.get_size())
			position = data.get_size();
	}
}
