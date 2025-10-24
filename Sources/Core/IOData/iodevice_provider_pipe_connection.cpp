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
#include "iodevice_provider_pipe_connection.h"
#include "API/Core/System/exception.h"
#include "API/Core/System/event.h"
#include "API/Core/Text/string_help.h"
#include "API/Core/Text/string_format.h"
#ifndef WIN32
#include <sys/socket.h>
#include <sys/un.h>
#ifndef UNIX_PATH_MAX
#define UNIX_PATH_MAX 108
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
// CL_IODeviceProvider_PipeConnection Construction:

#ifdef WIN32
CL_IODeviceProvider_PipeConnection::CL_IODeviceProvider_PipeConnection(
	const CL_StringRef &pipe_name)
: handle(INVALID_HANDLE_VALUE), peeked_data(0)
{
	handle = CreateFile(
		CL_StringHelp::utf8_to_ucs2("\\\\.\\pipe\\" + pipe_name).c_str(),
		GENERIC_READ|GENERIC_WRITE,
		0,
		0,
		OPEN_EXISTING,
		FILE_FLAG_OVERLAPPED,
		0);
	if (handle == INVALID_HANDLE_VALUE)
		throw CL_Exception(cl_format("Unable to open pipe %1", pipe_name));

	DWORD pipe_mode = PIPE_READMODE_BYTE|PIPE_WAIT;
	BOOL result = SetNamedPipeHandleState(handle, &pipe_mode, 0, 0);
	if (result == FALSE)
	{
		CloseHandle(handle);
		throw CL_Exception(cl_format("Unable to set named pipe handle state on %1", pipe_name));
	}
}
#else
CL_IODeviceProvider_PipeConnection::CL_IODeviceProvider_PipeConnection(
	const CL_StringRef &pipe_name)
: handle(-1), peeked_data(0)
{
	CL_String8 pipe_name_local8 = CL_StringHelp::text_to_local8(pipe_name);
	if (pipe_name_local8.length() >= UNIX_PATH_MAX)
		throw CL_Exception("Pipe name too long");

	handle = socket(PF_UNIX, SOCK_STREAM, 0);
	if (handle == -1)
		throw CL_Exception("Socket create failed");

	sockaddr_un name;
	memset(&name, 0, sizeof(sockaddr_un));
	name.sun_family = AF_UNIX;
	memcpy(name.sun_path+1, pipe_name_local8.data(), pipe_name_local8.length());

	int res = ::connect(handle, (sockaddr *) &name, sizeof(sockaddr_un));
	if (res == -1)
	{
		::close(handle);
		throw CL_Exception("Could not connect to server");
	}
}
#endif

#ifdef WIN32
CL_IODeviceProvider_PipeConnection::CL_IODeviceProvider_PipeConnection(
	HANDLE pipe_handle)
: handle(pipe_handle), peeked_data(0)
{
	if (handle == INVALID_HANDLE_VALUE)
		throw CL_Exception("Invalid handle value");
}
#else
CL_IODeviceProvider_PipeConnection::CL_IODeviceProvider_PipeConnection(
	int pipe_handle)
: handle(pipe_handle), peeked_data(0)
{
	if (handle == -1)
		throw CL_Exception("Invalid socket value");
}
#endif

CL_IODeviceProvider_PipeConnection::~CL_IODeviceProvider_PipeConnection()
{
	disconnect();
}

/////////////////////////////////////////////////////////////////////////////
// CL_IODeviceProvider_PipeConnection Attributes:

/////////////////////////////////////////////////////////////////////////////
// CL_IODeviceProvider_PipeConnection Operations:

int CL_IODeviceProvider_PipeConnection::send(const void *_data, int length, bool send_all)
{
#ifdef WIN32
	const char *data = (const char *) _data;
	int pos = 0;
	while (pos < length)
	{
		OVERLAPPED overlapped;
		memset(&overlapped, 0, sizeof(OVERLAPPED));
		BOOL result = WriteFile(handle, data+pos, length-pos, 0, &overlapped);
		DWORD error = (result == FALSE) ? GetLastError() : ERROR_SUCCESS;
		if (result == FALSE && error != ERROR_IO_PENDING)
			throw CL_Exception("Write failed to pipe!");
		DWORD written = 0;
		result = GetOverlappedResult(handle, &overlapped, &written, TRUE);
		if (result == FALSE)
			throw CL_Exception("Write failed to pipe!");
		pos += written;
		if (!send_all)
			break;
	}
	return pos;
#else
	const char *data = (const char *) _data;
	int pos = 0;
	while (pos < length)
	{
		int res = ::send(handle, data+pos, length-pos, 0);
		if (res == -1)
			throw CL_Exception("Write failed to socket!");
		pos += res;
		if (!send_all)
			break;
	}
	return pos;
#endif
}

int CL_IODeviceProvider_PipeConnection::receive(void *buffer, int size, bool receive_all)
{
	if (size == 0)
		return 0;
	if (peeked_data.get_size() > 0)
	{
		#define cl_min(a,b) ((a)<(b)?(a):(b))
		int peek_amount = cl_min(size, peeked_data.get_size());
		memcpy(buffer, peeked_data.get_data(), peek_amount);
		memmove(peeked_data.get_data(), peeked_data.get_data()+peek_amount, peeked_data.get_size()-peek_amount);
		peeked_data.set_size(peeked_data.get_size()-peek_amount);
		if (peek_amount <= size)
			return peek_amount + receive((char*) buffer+peek_amount, size-peek_amount, receive_all);
	}

	return lowlevel_read(buffer, size, receive_all);
}

int CL_IODeviceProvider_PipeConnection::peek(void *data, int len)
{
	if (peeked_data.get_size() >= len)
	{
		memcpy(data, peeked_data.get_data(), len);
		return len;
	}
	else
	{
		int old_size = peeked_data.get_size();
		try
		{
			peeked_data.set_size(len);
			int bytes_read = lowlevel_read(peeked_data.get_data()+old_size, len-old_size, false);
			peeked_data.set_size(old_size+bytes_read);
			memcpy(data, peeked_data.get_data(), peeked_data.get_size());
			return peeked_data.get_size();
		}
		catch (CL_Exception e)
		{
			peeked_data.set_size(old_size);
			throw e;
		}
	}
}

void CL_IODeviceProvider_PipeConnection::disconnect()
{
#ifdef WIN32
	if (handle != INVALID_HANDLE_VALUE)
		CloseHandle(handle);
	handle = INVALID_HANDLE_VALUE;
#else
	if (handle != -1)
		::close(handle);
	handle = -1;
#endif
}

CL_IODeviceProvider *CL_IODeviceProvider_PipeConnection::duplicate()
{
	throw CL_Exception("CL_IODeviceProvider_PipeConnection::duplicate() - Not Implemented");
}

/////////////////////////////////////////////////////////////////////////////
// CL_IODeviceProvider_PipeConnection Implementation:

int CL_IODeviceProvider_PipeConnection::lowlevel_read(void *buffer, int length, bool receive_all)
{
#ifdef WIN32
	char *data = (char *) buffer;
	int received = 0;
	while (received < length)
	{
		OVERLAPPED overlapped;
		memset(&overlapped, 0, sizeof(OVERLAPPED));
		BOOL result = ReadFile(handle, data+received, length-received, 0, &overlapped);
		DWORD error = (result == FALSE) ? GetLastError() : ERROR_SUCCESS;
		if (result == FALSE && error != ERROR_IO_PENDING)
			throw CL_Exception("Read failed from pipe!");
		DWORD bytes_read = 0;
		result = GetOverlappedResult(handle, &overlapped, &bytes_read, TRUE);
		if (result == FALSE)
			throw CL_Exception("Read failed from pipe!");
		received += bytes_read;
		if (!receive_all)
			break;
	}
	return received;
#else
	char *data = (char *) buffer;
	int received = 0;
	while (received < length)
	{
		int res = ::recv(handle, data+received, length-received, 0);
		if (res == -1)
			throw CL_Exception("Read failed on socket!");
		received += res;
		if (!receive_all)
			break;
	}
	return received;
#endif
}
