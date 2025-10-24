/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
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
**    Mark Page
*/

#include "Core/precomp.h"
#include "API/Core/IOData/iodevice.h"
#include "API/Core/IOData/iodevice_provider.h"
#include "API/Core/IOData/cl_endian.h"
#include "iodevice_impl.h"

/////////////////////////////////////////////////////////////////////////////
// CL_IODevice Construction:

CL_IODevice::CL_IODevice()
{
}

CL_IODevice::CL_IODevice(CL_IODeviceProvider *provider)
: impl(new CL_IODevice_Impl)
{
	impl->provider = provider;
}

CL_IODevice::~CL_IODevice()
{
}

/////////////////////////////////////////////////////////////////////////////
// CL_IODevice Attributes:

void CL_IODevice::throw_if_null() const
{
	if (!impl)
		throw CL_Exception("CL_IODevice is null");
}

int CL_IODevice::get_size() const
{
	if (impl)
		return impl->provider->get_size();
	return -1;
}

int CL_IODevice::get_position() const
{
	if (impl)
		return impl->provider->get_position();
	return -1;
}

bool CL_IODevice::is_little_endian() const
{
	return impl->little_endian_mode;
}

const CL_IODeviceProvider *CL_IODevice::get_provider() const
{
	throw_if_null();
	return impl->provider;
}

CL_IODeviceProvider *CL_IODevice::get_provider()
{
	throw_if_null();
	return impl->provider;
}

/////////////////////////////////////////////////////////////////////////////
// CL_IODevice Operations:

int CL_IODevice::send(const void *data, int len, bool send_all)
{
	if (impl)
		return impl->provider->send(data, len, send_all);
	return -1;
}

int CL_IODevice::receive(void *data, int len, bool receive_all)
{
	if (impl)
		return impl->provider->receive(data, len, receive_all);
	return -1;
}

int CL_IODevice::peek(void *data, int len)
{
	if (impl)
		return impl->provider->peek(data, len);
	return -1;
}

bool CL_IODevice::seek(int position, SeekMode mode)
{
	if (impl)
		return impl->provider->seek(position, mode);
	return false;
}

int CL_IODevice::read(void *data, int len, bool receive_all)
{
	return receive(data, len, receive_all);
}

int CL_IODevice::write(const void *data, int len, bool send_all)
{
	return send(data, len, send_all);
}

void CL_IODevice::set_system_mode()
{
	impl->little_endian_mode = !CL_Endian::is_system_big();
}

void CL_IODevice::set_big_endian_mode()
{
	impl->little_endian_mode = false;
}

void CL_IODevice::set_little_endian_mode()
{
	impl->little_endian_mode = true;
}

void CL_IODevice::write_int64(cl_byte64 data)
{
	cl_byte64 final = data;
	if (impl->little_endian_mode)
	{
		SWAP_IF_BIG(final);
	}
	else
	{
		SWAP_IF_LITTLE(final);
	}
	write(&final, sizeof(cl_byte64));
}

void CL_IODevice::write_uint64(cl_ubyte64 data)
{
	cl_ubyte64 final = data;
	if (impl->little_endian_mode)
	{
		SWAP_IF_BIG(final);
	}
	else
	{
		SWAP_IF_LITTLE(final);
	}
	write(&final, sizeof(cl_ubyte64));
}

void CL_IODevice::write_int32(cl_byte32 data)
{
	cl_byte32 final = data;
	if (impl->little_endian_mode)
	{
		SWAP_IF_BIG(final);
	}
	else
	{
		SWAP_IF_LITTLE(final);
	}
	write(&final, sizeof(cl_byte32));
}

void CL_IODevice::write_uint32(cl_ubyte32 data)
{
	cl_ubyte32 final = data;
	if (impl->little_endian_mode)
	{
		SWAP_IF_BIG(final);
	}
	else
	{
		SWAP_IF_LITTLE(final);
	}
	write(&final, sizeof(cl_ubyte32));
}

void CL_IODevice::write_int16(cl_byte16 data)
{
	cl_byte16 final = data;
	if (impl->little_endian_mode)
	{
		SWAP_IF_BIG(final);
	}
	else
	{
		SWAP_IF_LITTLE(final);
	}
	write(&final, sizeof(cl_byte16));
}

void CL_IODevice::write_uint16(cl_ubyte16 data)
{
	cl_ubyte16 final = data;
	if (impl->little_endian_mode)
	{
		SWAP_IF_BIG(final);
	}
	else
	{
		SWAP_IF_LITTLE(final);
	}
	write(&final, sizeof(cl_ubyte16));
}

void CL_IODevice::write_int8(cl_byte8 data)
{
	write(&data, sizeof(cl_byte8));
}

void CL_IODevice::write_uint8(cl_ubyte8 data)
{
	write(&data, sizeof(cl_ubyte8));
}

void CL_IODevice::write_float(float data)
{
	float final = data;
	if (impl->little_endian_mode)
	{
		SWAP_IF_BIG(final);
	}
	else
	{
		SWAP_IF_LITTLE(final);
	}
	write(&final, sizeof(float));
}

void CL_IODevice::write_string_a(const CL_StringRef8 &str)
{
	int size = str.length();
	write_int32(size);
	write(str.data(), size);
}

void CL_IODevice::write_string_nul(const CL_StringRef8 &str)
{
	write(str.c_str(), str.length() + 1);
}

void CL_IODevice::write_string_text(const CL_StringRef8 &str)
{
	write(str.data(), str.length());
#ifdef WIN32
	write("\r\n", 2);
#else
	write("\n", 1);
#endif
}

cl_byte64 CL_IODevice::read_int64()
{
	cl_byte64 answer;
	if (read(&answer, sizeof(cl_byte64)) != sizeof(cl_byte64)) throw CL_Exception("CL_IODevice_Datafile::read_int64() failed");
	if (impl->little_endian_mode)
	{
		SWAP_IF_BIG(answer);
	}
	else
	{
		SWAP_IF_LITTLE(answer);
	}
	return answer;
}

cl_ubyte64 CL_IODevice::read_uint64()
{
	cl_ubyte64 answer;
	if (read(&answer, sizeof(cl_ubyte64)) != sizeof(cl_ubyte64)) throw CL_Exception("CL_IODevice_Datafile::read_uint64() failed");
	if (impl->little_endian_mode)
	{
		SWAP_IF_BIG(answer);
	}
	else
	{
		SWAP_IF_LITTLE(answer);
	}
	return answer;
}

cl_byte32 CL_IODevice::read_int32()
{
	cl_byte32 answer;
	if (read(&answer, sizeof(cl_byte32)) != sizeof(cl_byte32)) throw CL_Exception("CL_IODevice_Datafile::read_int32() failed");
	if (impl->little_endian_mode)
	{
		SWAP_IF_BIG(answer);
	}
	else
	{
		SWAP_IF_LITTLE(answer);
	}
	return answer;
}

cl_ubyte32 CL_IODevice::read_uint32()
{
	cl_ubyte32 answer;
	if (read(&answer, sizeof(cl_ubyte32)) != sizeof(cl_ubyte32)) throw CL_Exception("CL_IODevice_Datafile::read_uint32() failed");
	if (impl->little_endian_mode)
	{
		SWAP_IF_BIG(answer);
	}
	else
	{
		SWAP_IF_LITTLE(answer);
	}
	return answer;
}

cl_byte16 CL_IODevice::read_int16()
{
	cl_byte16 answer;
	if (read(&answer, sizeof(cl_byte16)) != sizeof(cl_byte16)) throw CL_Exception("CL_IODevice_Datafile::read_int16() failed");
	if (impl->little_endian_mode)
	{
		SWAP_IF_BIG(answer);
	}
	else
	{
		SWAP_IF_LITTLE(answer);
	}
	return answer;
}

cl_ubyte16 CL_IODevice::read_uint16()
{
	cl_ubyte16 answer;
	if (read(&answer, sizeof(cl_ubyte16)) != sizeof(cl_ubyte16)) throw CL_Exception("CL_IODevice_Datafile::read_uint16() failed");
	if (impl->little_endian_mode)
	{
		SWAP_IF_BIG(answer);
	}
	else
	{
		SWAP_IF_LITTLE(answer);
	}
	return answer;
}

cl_byte8 CL_IODevice::read_int8()
{
	cl_byte8 answer;
	if (read(&answer, sizeof(cl_byte8)) != sizeof(cl_byte8)) throw CL_Exception("CL_IODevice_Datafile::read_int8() failed");
	if (impl->little_endian_mode)
	{
		SWAP_IF_BIG(answer);
	}
	else
	{
		SWAP_IF_LITTLE(answer);
	}
	return answer;
}

cl_ubyte8 CL_IODevice::read_uint8()
{
	cl_ubyte8 answer;
	if (read(&answer, sizeof(cl_ubyte8)) != sizeof(cl_ubyte8)) throw CL_Exception("CL_IODevice_Datafile::read_uint8() failed");
	if (impl->little_endian_mode)
	{
		SWAP_IF_BIG(answer);
	}
	else
	{
		SWAP_IF_LITTLE(answer);
	}
	return answer;
}

float CL_IODevice::read_float()
{
	float answer;
	if (read(&answer, sizeof(float)) != sizeof(float)) throw CL_Exception("CL_IODevice_Datafile::read_float() failed");
	if (impl->little_endian_mode)
	{
		SWAP_IF_BIG(answer);
	}
	else
	{
		SWAP_IF_LITTLE(answer);
	}
	return answer;
}

CL_String8 CL_IODevice::read_string_a()
{
	int size = read_int32();

	char *str = new char[size];
	try
	{
		read(str, size);
		
		CL_String8 ret(str, size);
		delete[] str;

		return ret;
	}
	catch (...)
	{
		delete[] str;
		throw;
	}
}

CL_String8 CL_IODevice::read_string_nul()
{
	return (read_string_text(NULL, NULL, false));
}

CL_String8 CL_IODevice::read_string_text(const char *skip_initial_chars, const char *read_until_chars, bool allow_eof)
{
	const int buffer_size = 64;
	char buffer[buffer_size];
	int read_size;
	int size = 0;
	bool find_flag = true;
	bool null_found = false;
	int current_position = get_position();

	// Skip initial unwanted chars
	if (skip_initial_chars)
	{
		while(find_flag)	// For the entire file
		{
			read_size = receive(buffer, buffer_size, true);

			char *dptr = buffer;
			for (int cnt=0; cnt<read_size; cnt++, dptr++)	// Search the buffer
			{
				char letter = *dptr;
				bool match_flag = false;
				for (const char *rptr = skip_initial_chars; *rptr; rptr++)	// Search the initial chars array
				{
					if (letter == *rptr)
					{
						match_flag = true;
						break;
					}
				}
				if (!match_flag)	// Not found a match
				{
					find_flag = false;
					break;
				}
				current_position++;	// Skip the char		
			}

			if ((find_flag) && (read_size != buffer_size))	// eof found
			{
				if (!allow_eof)
				{
					throw CL_Exception("CL_IODevice_Datafile::read_string_text() failed - end of file");
				}
				break;
			}
		}

		seek(current_position, seek_set);	// Set new position
	}

	find_flag = true;

	// Calculate the length of the string
	while(find_flag)
	{
		read_size = receive(buffer, buffer_size, true);
		char *dptr = buffer;
		for (int cnt=0; cnt<read_size; cnt++, dptr++)
		{
			char letter = *dptr;
			// Treat NUL as a special terminating character
			if (letter == 0)
			{
				null_found = true;
				find_flag = false;
				break;
			}
			bool match_flag = false;
			if (read_until_chars)
			{
				for (const char *rptr = read_until_chars; *rptr; rptr++)	// Search the read until chars array
				{
					if (letter == *rptr)
					{
						match_flag = true;
						break;
					}
				}
				if (match_flag)	// Found a match
				{
					find_flag = false;
					break;
				}
			}

			size++;
		}

		if ((find_flag) && (read_size != buffer_size))	// eof found
		{
			if (!allow_eof)
			{
				throw CL_Exception("CL_IODevice_Datafile::read_string_text() failed - end of file");
			}
			break;
		}
	}
	seek(current_position, seek_set);

	// Read the string, now that we know its length

	char *str = new char[size];
	try
	{
		read(str, size);
		CL_String8 ret(str, size);
		if (null_found)
		{
			read(buffer, 1);	// Read the NUL terminator
		}
		delete[] str;

		return ret;
	}
	catch (...)
	{
		delete[] str;
		throw;
	}
}

CL_IODevice CL_IODevice::duplicate()
{
	return CL_IODevice(impl->provider->duplicate());
}

/////////////////////////////////////////////////////////////////////////////
// CL_IODevice Implementation:

