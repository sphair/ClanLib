/*
**  ClanLib SDK
**  Copyright (c) 1997-2012 The ClanLib Team
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
#include "pipe_listen_impl.h"
#include "API/Core/IOData/pipe_listen.h"
#include "API/Core/IOData/pipe_connection.h"
#include "API/Core/System/exception.h"
#include "API/Core/System/event.h"
#include "API/Core/System/event_provider.h"
#include "API/Core/Text/string_help.h"
#ifndef WIN32
#include "Network/Socket/event_provider_unixsocket.h"
#include <sys/socket.h>
#include <sys/un.h>
#ifndef UNIX_PATH_MAX 
#define UNIX_PATH_MAX 108
#endif
#endif

namespace clan
{

/////////////////////////////////////////////////////////////////////////////
// PipeListen_Impl Construction:

#ifdef WIN32
PipeListen_Impl::PipeListen_Impl(const std::string &name)
: handle(INVALID_HANDLE_VALUE), name(name), async_io(false)
{
	handle = CreateNamedPipe(
		StringHelp::utf8_to_ucs2("\\\\.\\pipe\\" + name).c_str(),
		PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES,
		16*1024,
		16*1024,
		NMPWAIT_USE_DEFAULT_WAIT,
		0);
	if (handle == INVALID_HANDLE_VALUE)
		throw Exception("Unable to create named pipe for " + name);
}
#else
PipeListen_Impl::PipeListen_Impl(const std::string &name)
: handle(-1)
{
	std::string name_local8 = StringHelp::text_to_local8(name);
	if (name_local8.length() >= UNIX_PATH_MAX)
		throw Exception("Pipe name too long!");

	handle = socket(PF_UNIX, SOCK_STREAM, 0);
	if (handle == -1)
		throw Exception("Unable to create unix socket");

	accept_event = Event(
		new EventProvider_UnixSocket(handle, EventProvider::type_fd_read));

	// Create pipe name in the abstract namespace:
	// (abstract namespace starts with first character in sun_path 
	// being the null character)
	sockaddr_un addr;
	memset(&addr, 0, sizeof(sockaddr_un));
	addr.sun_family = AF_UNIX;
	memcpy(addr.sun_path+1, name_local8.data(), name_local8.length());
	
	int result = bind(handle, (sockaddr *) &addr, sizeof(sockaddr_un));
	if (result != -1)
		result = listen(handle, 5);
	if (result == -1)
	{
		::close(handle);
		throw Exception("Unable to bind pipe to " + name);
	}
}
#endif

PipeListen_Impl::~PipeListen_Impl()
{
#ifdef WIN32
	if (async_io)
		cancel_accept();
	if (handle != INVALID_HANDLE_VALUE)
		CloseHandle(handle);
#else
	if (handle != -1)
		::close(handle);
#endif
}

/////////////////////////////////////////////////////////////////////////////
// PipeListen_Impl Attributes:

/////////////////////////////////////////////////////////////////////////////
// PipeListen_Impl Operations:

Event PipeListen_Impl::begin_accept()
{
#ifdef WIN32
	if (async_io)
		return accept_event;

	accept_event.reset();
	memset(&accept_overlapped, 0, sizeof(OVERLAPPED));
	accept_overlapped.hEvent = accept_event.get_event_provider()->get_event_handle(0);
	BOOL result = ConnectNamedPipe(handle, &accept_overlapped);
	DWORD error = (result == FALSE) ? GetLastError() : ERROR_SUCCESS;
	if (result == FALSE && error != ERROR_IO_PENDING)
		throw Exception("ConnectNamedPipe failed");
	async_io = true;
	return accept_event;
#else
	return accept_event;
#endif
}

PipeConnection PipeListen_Impl::complete_accept()
{
#ifdef WIN32
	if (!async_io)
		throw Exception("PipeConnection is not in an async I/O state");

	DWORD bytes_transfered = 0;
	BOOL result = GetOverlappedResult(handle, &accept_overlapped, &bytes_transfered, TRUE);
	if (result != TRUE)
		throw Exception("GetOverlappedResult failed");

	accept_event.reset();
	async_io = false;
	PipeConnection connection(handle, true);

	handle = CreateNamedPipe(
		StringHelp::utf8_to_ucs2("\\\\.\\pipe\\" + name).c_str(),
		PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES,
		16*1024,
		16*1024,
		NMPWAIT_USE_DEFAULT_WAIT,
		0);
	if (handle == INVALID_HANDLE_VALUE)
		throw Exception("Unable to create named pipe for " + name);

	return connection;
#else
	return accept();
#endif
}

void PipeListen_Impl::cancel_accept()
{
#ifdef WIN32
	if (!async_io)
		throw Exception("PipeConnection is not in an async I/O state");

	BOOL result = CancelIo(handle);
	if (result == FALSE)
		throw Exception("CancelIo failed");

	DWORD bytes_transfered = 0;
	result = GetOverlappedResult(handle, &accept_overlapped, &bytes_transfered, TRUE);
	if (result == TRUE)
	{
		result = DisconnectNamedPipe(handle);
	}
	else if (GetLastError() != ERROR_OPERATION_ABORTED)
	{
		throw Exception("GetOverlappedResult failed");
	}
	async_io = false;
#endif
}

PipeConnection PipeListen_Impl::accept()
{
#ifdef WIN32
	OVERLAPPED overlapped;
	memset(&overlapped, 0, sizeof(OVERLAPPED));
	BOOL result = ConnectNamedPipe(handle, &overlapped);
	DWORD error = (result == FALSE) ? GetLastError() : ERROR_SUCCESS;
	if (result == FALSE && error != ERROR_IO_PENDING)
		throw Exception("ConnectNamedPipe failed");

	DWORD bytes_transfered = 0;
	result = GetOverlappedResult(handle, &overlapped, &bytes_transfered, TRUE);
	if (result != TRUE)
		throw Exception("GetOverlappedResult failed");

	PipeConnection connection(handle, true);

	handle = CreateNamedPipe(
		StringHelp::utf8_to_ucs2("\\\\.\\" + name).c_str(),
		PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES,
		16*1024,
		16*1024,
		NMPWAIT_USE_DEFAULT_WAIT,
		0);
	if (handle == INVALID_HANDLE_VALUE)
		throw Exception("Unable to create named pipe for " + name);

	return connection;
#else
	// EAGAIN
	// O_NONBLOCK
	
	sockaddr_un name;
	memset(&name, 0, sizeof(sockaddr_un));
	name.sun_family = AF_UNIX;
	
	socklen_t size = sizeof(sockaddr_un);
	int sock = ::accept(handle, (sockaddr *) &name, &size);
	if (sock == -1)
		throw Exception("Unable to accept pipe connection!");

	return PipeConnection(sock, true);
#endif
}

/////////////////////////////////////////////////////////////////////////////
// PipeListen_Impl Implementation:

}
